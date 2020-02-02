#include <DxLib.h>
#include <lua.hpp>
#include <sol/sol.hpp>
#include <argagg/argagg.hpp>

#include <filesystem>

inline void my_panic(sol::optional<std::string> maybe_msg) {
    std::cerr << "Lua is in a panic state and will now abort() the application" << std::endl;
    if (maybe_msg) {
        const std::string& msg = maybe_msg.value();
        std::cerr << "\terror message: " << msg << std::endl;
    }
    // When this function exits, Lua will exhibit default behavior and abort()
}

int nogame(std::filesystem::path path) {
    if (!path.empty()) {
        printfDx(_T("パス: %s\n\n"), path.string().c_str());
    }
    printfDx(_T("Lua スクリプトファイルが見つかりませんでした\n"));
    printfDx(_T("何かキーを押すと終了します\n"));
    return WaitKey();
}

bool loadScript(sol::state &lua, std::filesystem::path path) {
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
                    std::cerr << err.what() << std::endl;
                }

            } else {
                sol::error err = result;
                std::cerr << err.what() << std::endl;
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

    argagg::parser argparser{ {
        { "console", {"-c", "--console"}, "コンソールを使用します", 0},
        { "window", {"-w", "--window"}, "強制的にウィンドウモードで起動します", 0},
        { "fullscreen", {"-f", "--fullscreen"}, "強制的にフルスクリーンモードで起動します", 0},
        { "version", {"-v", "--version"}, "バージョン情報を表示します", 0},
        { "help", {"-h", "--help"}, "今表示している引数のヘルプを表示します", 0},
    } };

    sol::state lua(sol::c_call<decltype(&my_panic), &my_panic>);
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::io);

    argagg::parser_results args;
    try {
        args = argparser.parse(argc, argv);

    } catch (const std::exception & e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    if (args["console"]) {
        console = true;
    }
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

    if (args["version"]) {
        std::cout << "DxLua 0.1.0" << std::endl;
    }
    if (args["help"]) {
        std::cout << "使い方:" << std::endl;
        std::cout << "    DxLua [オプション] luaスクリプトファイル" << std::endl;
        std::cout << "    DxLua [オプション] main.luaが入っているディレクトリ" << std::endl;
        std::cout << "    DxLua [オプション] main.luaが入っているdxaファイル" << std::endl;
        std::cout << std::endl;
        std::cout << "オプション:" << std::endl;
        std::cout << argparser;
    }
    if (args.pos.size() > 0) {
        // 指定パス
        std::filesystem::path path = args.as<std::string>();

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

        } else if (std::filesystem::exists("game/main.lua")) {
            // game ディレクトリの下にスクリプト main.lua があった
            std::filesystem::current_path("./game");

        } else if (std::filesystem::exists("game.dxa")) {
            // アーカイブ game.dxa があった
            scriptPath = "game/main.lua";
        }
    }

    // 初期設定
    SetMainWindowText("DxLua");
    ChangeWindowMode(window);//非全画面にセット
    SetGraphMode(640, 480, 32);//画面サイズ指定
    //SetOutApplicationLogValidFlag(FALSE);//Log.txtを生成しないように設定

    // スクリプトのロード
    auto loaded = loadScript(lua, scriptPath);

    if (DxLib_Init() == 1){ return -1; }//初期化に失敗時にエラーを吐かせて終了

    //
    //ここで画像・音を読み込み
    //

    if (loaded) {
        printfDx(_T("パス: %s\n\n読み込みに成功しました"), scriptPath.string().c_str());

        // DxLua.boot を実行
        if (sol::object mod = lua["DxLua"]; mod.is<sol::table>()) {
            if (sol::object boot = mod.as<sol::table>()["boot"]; boot.is<sol::function>()) {
                if (auto result = boot.as<sol::protected_function>().call(123, "hoge"); !result.valid()) {
                    sol::error err = result;
                    std::cerr << err.what() << std::endl;
                }
            }
        }

        // メインループ
        while (ProcessMessage() == 0) {
            ClearDrawScreen();//裏画面消す
            SetDrawScreen(DX_SCREEN_BACK);//描画先を裏画面に

            //
            //ここに毎フレーム呼ぶ処理を書く
            //

            ScreenFlip();//裏画面を表画面にコピー
        }

    } else {
        nogame(scriptPath);
    }

    DxLib_End();

#ifdef _WIN32
    if (attachedConsole || allocedConsole) {
        FreeConsole();
    }
#endif

    return 0;
}
