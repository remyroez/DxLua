#include <DxLib.h>
#include <lua.hpp>
#include <sol/sol.hpp>

int main(int argc, char** argv)
{
    if (!AttachConsole(ATTACH_PARENT_PROCESS)) {
        AllocConsole();
        SetConsoleTitle(TEXT("DxLua Console"));
    }
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout); /* 標準出力(stdout)を新しいコンソールに向ける */
    freopen_s(&fp, "CONIN$", "r", stdin);
    freopen_s(&fp, "CONOUT$", "w", stderr); /* 標準エラー出力(stderr)を新しいコンソールに向ける */

    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::io);
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
    lua.script("print('Hello World' , 123 , 'moge')");

    printf("test");

    ChangeWindowMode(TRUE);//非全画面にセット
    SetGraphMode(640, 480, 32);//画面サイズ指定
    //SetOutApplicationLogValidFlag(FALSE);//Log.txtを生成しないように設定
    if (DxLib_Init() == 1){ return -1; }//初期化に失敗時にエラーを吐かせて終了

    //
    //ここで画像・音を読み込み
    //

    while (ProcessMessage() == 0)
    {
        ClearDrawScreen();//裏画面消す
        SetDrawScreen(DX_SCREEN_BACK);//描画先を裏画面に

        //
        //ここに毎フレーム呼ぶ処理を書く
        //

        ScreenFlip();//裏画面を表画面にコピー
    }

    DxLib_End();
    return 0;
}
