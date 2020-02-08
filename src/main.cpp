#include "application.h"

int main(int argc, char** argv)
{
    if (remyroez::application app; !app.setup(argc, argv)) {
        // セットアップ失敗

    } else {
        // 実行（restart が返ったらループ）
        while (app.boot() == remyroez::application::done_code::restart) {}

        // 終了
        app.teardown();
    }

    return 0;
}
