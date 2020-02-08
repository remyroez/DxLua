#include "application.h"

int main(int argc, char** argv)
{
    // アプリケーション
    remyroez::application app;

    if (!app.setup(argc, argv)) {
        // セットアップ失敗

    } else {
        using done_code = remyroez::application::done_code;

        // 実行（restart が返ったらループ）
        while (app.boot() == done_code::restart) {}

        // 終了
        app.teardown();
    }

    return 0;
}
