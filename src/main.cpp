#include <DxLib.h>
#include <lua.hpp>
#include <sol/sol.hpp>
#include <argagg/argagg.hpp>

int nogame(std::string_view path = std::string_view()) {
    if (!path.empty()) {
        printfDx(_T("パス: %s\n\n"), path.data());
    }
    printfDx(_T("Lua スクリプトファイルが見つかりませんでした\n"));
    printfDx(_T("何かキーを押すと終了します\n"));
    return WaitKey();
}

bool loadScript(sol::state &lua, std::string_view path) {
    bool succeeded = false;

    auto size = FileRead_size(path.data());
    if (size < 0) {
        // 見つからなかった

    } else {
        auto file = FileRead_open(path.data());
        std::vector<std::byte> buffer;
        buffer.resize(size);
        if (FileRead_read(buffer.data(), size, file) >= 0) {
            auto result = lua.load_buffer(buffer.data(), buffer.size());
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
    std::string scriptPath = "main.lua";

    argagg::parser argparser{ {
        { "console", {"-c", "--console"}, "コンソールを使用します", 0},
        { "window", {"-w", "--window"}, "強制的にウィンドウモードで起動します", 0},
        { "fullscreen", {"-f", "--fullscreen"}, "強制的にフルスクリーンモードで起動します", 0},
        { "version", {"-v", "--version"}, "バージョン情報を表示します", 0},
        { "help", {"-h", "--help"}, "今表示している引数のヘルプを表示します", 0},
    } };

    sol::state lua;
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
        scriptPath = args.as<std::string>();
    }

    lua.script("print('Hello World' , 123 , 'moge')");
    printf("test\n");

    auto loaded = loadScript(lua, scriptPath);

    ChangeWindowMode(window);//非全画面にセット
    SetGraphMode(640, 480, 32);//画面サイズ指定
    //SetOutApplicationLogValidFlag(FALSE);//Log.txtを生成しないように設定
    if (DxLib_Init() == 1){ return -1; }//初期化に失敗時にエラーを吐かせて終了

    //
    //ここで画像・音を読み込み
    //

    if (loaded) {
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
    if (attachedConsole || console) {
        FreeConsole();
    }
#endif
    return 0;
}
