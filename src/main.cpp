#include "application.h"

int main(int argc, char** argv)
{
    // アプリケーション
    remyroez::application app;

    if (!app.setup(argc, argv)) {
        // セットアップ失敗

    } else {
        // 実行
        app.boot();

        // 終了
        app.teardown();
    }

    return 0;
}
