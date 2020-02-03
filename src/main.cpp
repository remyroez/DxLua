#include <DxLib.h>
#include <lua.hpp>
#include <sol/sol.hpp>
#include <argagg/argagg.hpp>

#include <filesystem>

#include "DxLua/DxLua.h"

inline void my_panic(sol::optional<std::string> maybe_msg) {
    std::cerr << "Lua is in a panic state and will now abort() the application" << std::endl;
    if (maybe_msg) {
        const std::string& msg = maybe_msg.value();
        std::cerr << "\terror message: " << msg << std::endl;
    }
    // When this function exits, Lua will exhibit default behavior and abort()
}

bool loadScript(sol::state &lua, std::filesystem::path path, std::ostringstream &message) {
    bool succeeded = false;

    auto size = FileRead_size(path.string().c_str());
    if (size < 0) {
        // 見つからなかった

    } else {
        auto file = FileRead_open(path.string().c_str());
        std::vector<std::byte> buffer;
        buffer.resize(size);
        if (FileRead_read(buffer.data(), size, file) >= 0) {
            auto result = lua.load_buffer(buffer.data(), buffer.size(), path.filename().string().c_str());
            if (result.valid()) {
                auto presult = result.call();

                if (presult.valid()) {
                    succeeded = true;

                } else {
                    sol::error err = presult;
                    message << err.what() << std::endl;
                }

            } else {
                sol::error err = result;
                message << err.what() << std::endl;
            }
        }
        FileRead_close(file);
    }

    return succeeded;
}

int main(int argc, char** argv)
{
    bool attachedConsole = false;
    bool allocedConsole = false;
    bool console = true;
    bool window = true;
    std::filesystem::path scriptFilename = "main.lua";
    std::filesystem::path scriptPath = scriptFilename;
    std::filesystem::path argPath;

    argagg::parser argparser{ {
        { "console", {"-c", "--console"}, "コンソールを使用します", 0},
        { "window", {"-w", "--window"}, "強制的にウィンドウモードで起動します", 0},
        { "fullscreen", {"-f", "--fullscreen"}, "強制的にフルスクリーンモードで起動します", 0},
        { "version", {"-v", "--version"}, "バージョン情報を表示します", 0},
        { "help", {"-h", "--help"}, "今表示している引数のヘルプを表示します", 0},
    } };

    sol::state lua(sol::c_call<decltype(&my_panic), &my_panic>);
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::io, sol::lib::bit32);
    auto DxLuaObject = lua.require("DxLua", sol::c_call<decltype(&DxLua::openDxLua), &DxLua::openDxLua>);

    // 引数の解析
    argagg::parser_results args;
    try {
        args = argparser.parse(argc, argv);

    } catch (const std::exception & e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // コンソール使用
    if (args["console"]) {
        console = true;
    }

    // フルスクリーン／ウィンドウモード
    if (args["fullscreen"]) {
        window = false;

    } else if (args["window"]) {
        window = true;
    }

#ifdef _WIN32
    // コンソールの使用
    if (console) {
        if (AttachConsole(ATTACH_PARENT_PROCESS)) {
            attachedConsole = true;

        } else {
            allocedConsole = AllocConsole();
            if (allocedConsole) {
                SetConsoleTitle(TEXT("DxLua Console"));
            }
        }
        SetConsoleOutputCP(CP_UTF8);
    }

    if (attachedConsole || allocedConsole) {
        // アタッチ／生成したコンソールへ標準入出力を設定
        FILE* fp;
        freopen_s(&fp, "CONIN$", "r", stdin);
        freopen_s(&fp, "CONOUT$", "w", stdout); /* 標準出力(stdout)を新しいコンソールに向ける */
        freopen_s(&fp, "CONOUT$", "w", stderr); /* 標準エラー出力(stderr)を新しいコンソールに向ける */

        // Lua 関数を C++ stdout に合わせて修正
        lua.set_function(
            "print",
            [](sol::variadic_args va) {
                int i = 0;
                for (auto v : va) {
                    auto str = v.get<std::string>();
                    if (i) putchar('\t');
                    fwrite(str.c_str(), 1, str.size(), stdout);
                    i++;
                }
                putchar('\n');
            }
        );
    }
#endif // _WIN32

    // バージョン
    if (args["version"]) {
        std::cout << "DxLua 0.1.0" << std::endl;
    }

    // ヘルプ
    if (args["help"]) {
        std::cout << "使い方:" << std::endl;
        std::cout << "    DxLua [オプション] luaスクリプトファイル" << std::endl;
        std::cout << "    DxLua [オプション] main.luaが入っているディレクトリ" << std::endl;
        std::cout << "    DxLua [オプション] main.luaが入っているdxaファイル" << std::endl;
        std::cout << std::endl;
        std::cout << "オプション:" << std::endl;
        std::cout << argparser;
    }

    // パスの解析
    if (args.pos.size() > 0) {
        // 指定パス
        std::filesystem::path path = args.as<std::string>();
        argPath = path;

        // 絶対パスに変換
        if (!path.is_absolute()) {
            path = std::filesystem::absolute(path);
        }

        // パスの対象別に読み込みパスの作成
        if (std::filesystem::is_directory(path)) {
            // ディレクトリ
            std::filesystem::current_path(path);
            scriptPath = scriptFilename;

        } else if (std::filesystem::is_regular_file(path)) {
            // ファイル
            auto ext = path.extension();
            if (ext == ".lua") {
                // スクリプト
                scriptFilename = path.filename().string();
                std::filesystem::current_path(path.remove_filename());
                scriptPath = scriptFilename;

            } else if (ext == ".dxa") {
                // アーカイブ
                std::filesystem::current_path(path.remove_filename());
                auto archive = path.stem();
                scriptPath = archive.append(scriptFilename.string());
            }
        }

    } else {
        // パスが指定されていない
        if (std::filesystem::exists("main.lua")) {
            // スクリプト main.lua があった
            argPath = scriptPath;

        } else if (std::filesystem::exists("game/main.lua")) {
            // game ディレクトリの下にスクリプト main.lua があった
            std::filesystem::current_path("./game");
            argPath = "game/main.lua";

        } else if (std::filesystem::exists("game.dxa")) {
            // アーカイブ game.dxa があった
            scriptPath = "game/main.lua";
            argPath = scriptPath;
        }
    }

    // 初期設定
    SetMainWindowText("DxLua");
    ChangeWindowMode(window);
    //SetOutApplicationLogValidFlag(FALSE);//Log.txtを生成しないように設定

    // スクリプトのロード
    std::ostringstream message;
    auto loaded = loadScript(lua, scriptPath, message);

    // ＤＸライブラリ初期化
    if (DxLib_Init()){
        return -1;
    }

    // メッセージ出力
    {
        auto messageDx = message.str();
        if (messageDx.size() > 0) {
            printfDx(_T("%s\n"), messageDx.c_str());
        }
    }

    bool waitBeforeEnd = false;
    if (loaded) {
        // 読み込み済み
        //printfDx(_T("パス: %s\n\nLua スクリプトファイルの読み込みに成功しました\n"), argPath.string().c_str());

        // DxLua.Boot を実行
        bool booted = false;
        if (DxLuaObject.is<sol::table>()) {
            if (sol::object Boot = DxLuaObject.as<sol::table>()["Boot"]; Boot.is<sol::function>()) {
                if (auto result = Boot.as<sol::protected_function>().call(123, "hoge"); !result.valid()) {
                    sol::error err = result;
                    std::cerr << err.what() << std::endl;
                    
                } else {
                    // 正常に終了
                    booted = true;
                }

            } else {
                printfDx(_T("変数 DxLua.Boot が関数ではない、または存在しませんでした"));
            }

        } else {
            printfDx(_T("変数 DxLua がテーブルではない、または存在しませんでした"));
        }

        // ブートに失敗したらキー入力待ち
        if (!booted) {
            waitBeforeEnd = true;
        }

    } else {
        // 読み込みなし
        if (!scriptPath.empty()) {
            printfDx(_T("パス: %s\n\n"), argPath.string().c_str());
        }
        printfDx(_T("Lua スクリプトファイルの読み込みに失敗しました\n"));
        waitBeforeEnd = true;
    }

    if (waitBeforeEnd) {
        printfDx(_T("\n何かキーを押すと終了します\n"));
        WaitKey();
    }

    // ＤＸライブラリ終了処理
    DxLib_End();

#ifdef _WIN32
    // コンソール開放
    if (attachedConsole || allocedConsole) {
        FreeConsole();
    }
#endif

    return 0;
}
