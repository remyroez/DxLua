#include <DxLib.h>
#include <lua.hpp>
#include <sol/sol.hpp>

#include "DxLua.h"

namespace DxLua {

sol::table openDxLua(sol::this_state s)
{
    sol::state_view lua(s);
    sol::table library = lua.create_table();

    // 実行関数
    {
        auto result = lua.safe_script(R"lua( return function (...)
    -- ＤＸライブラリ初期化処理
    if type(DxLua.Init) == 'function' then
        local result = DxLua.Init()
        if result then
            return result -- エラーが起きたら直ちに終了
        end
    end

    -- ループ
    if type(DxLua.Update) == 'function' then
        while (DxLua.ProcessMessage() == 0 and DxLua.CheckHitKey(DxLua.KEY_INPUT_ESCAPE) == 0) do
            local result = DxLua.Update()
            if result then
                return result -- エラーが起きたら直ちに終了
            end
        end
    end

    -- ＤＸライブラリ使用の終了処理
    if type(DxLua.End) == 'function' then
        DxLua.End()
    end

    return 'exit'
end)lua"
        );
        if (!result.valid()) {
            sol::error err = result;
            std::cerr << err.what() << std::endl;

        } else {
            if (sol::object fn = result.get<sol::function>(); fn.is<sol::function>()) {
                library["Run"] = fn;
            }
        }
    }

#define DXLUA_INSTALL(T, X) T[#X] = X;

    DXLUA_INSTALL(library, ProcessMessage);

    DXLUA_INSTALL(library, WaitTimer);
    DXLUA_INSTALL(library, WaitKey);

    //DXLUA_INSTALL(library, SetGraphMode);
    library["SetGraphMode"] = [](int ScreenSizeX, int ScreenSizeY, int ColorBitDepth, sol::variadic_args va) {
        return SetGraphMode(ScreenSizeX, ScreenSizeY, ColorBitDepth, va.leftover_count() > 0 ? va[0].as<int>() : 60);
    };

    DXLUA_INSTALL(library, ScreenFlip);
    library["ClearDrawScreen"] = [](sol::variadic_args va) {
        return ClearDrawScreen();
    };
    DXLUA_INSTALL(library, SetDrawScreen); 

    library["LoadGraph"] = [](const TCHAR* FileName, sol::variadic_args va) {
        return LoadGraph(FileName, va.leftover_count() > 0 ? va[0].as<bool>() : false);
    };
    library["DrawGraph"] = [](float xf, float yf, int GrHandle, bool TransFlag) {
        return DrawGraph(xf, yf, GrHandle, TransFlag ? TRUE : FALSE);
    };
    library["DrawBox"] = [](int x1, int y1, int x2, int y2, unsigned int Color, bool FillFlag) {
        return DrawBox(x1, y1, x2, y2, Color, FillFlag ? TRUE : FALSE);
    };
    library["DrawString"] = [](int x, int y, const TCHAR* String, unsigned int Color, sol::variadic_args va) {
        return DrawString(x, y, String, Color, va.leftover_count() > 0 ? va[0].as<int>() : 0);
    };
    DXLUA_INSTALL(library, GetColor);

    DXLUA_INSTALL(library, SetFontSize);

    DXLUA_INSTALL(library, DX_SCREEN_FRONT);
    DXLUA_INSTALL(library, DX_SCREEN_BACK);
    DXLUA_INSTALL(library, DX_SCREEN_WORK);
    DXLUA_INSTALL(library, DX_SCREEN_TEMPFRONT);
    DXLUA_INSTALL(library, DX_SCREEN_OTHER);

    DXLUA_INSTALL(library, CheckHitKey);
    DXLUA_INSTALL(library, GetJoypadInputState);

    DXLUA_INSTALL(library, KEY_INPUT_ESCAPE);

    DXLUA_INSTALL(library, DX_INPUT_KEY_PAD1);
    DXLUA_INSTALL(library, DX_INPUT_PAD1);
    DXLUA_INSTALL(library, DX_INPUT_PAD2);
    DXLUA_INSTALL(library, DX_INPUT_PAD3);
    DXLUA_INSTALL(library, DX_INPUT_PAD4);
    DXLUA_INSTALL(library, DX_INPUT_PAD5);
    DXLUA_INSTALL(library, DX_INPUT_PAD6);
    DXLUA_INSTALL(library, DX_INPUT_PAD7);
    DXLUA_INSTALL(library, DX_INPUT_PAD8);
    DXLUA_INSTALL(library, DX_INPUT_PAD9);
    DXLUA_INSTALL(library, DX_INPUT_PAD10);
    DXLUA_INSTALL(library, DX_INPUT_PAD11);
    DXLUA_INSTALL(library, DX_INPUT_PAD12);
    DXLUA_INSTALL(library, DX_INPUT_PAD13);
    DXLUA_INSTALL(library, DX_INPUT_PAD14);
    DXLUA_INSTALL(library, DX_INPUT_PAD15);
    DXLUA_INSTALL(library, DX_INPUT_PAD16);
    DXLUA_INSTALL(library, DX_INPUT_KEY);

    DXLUA_INSTALL(library, TOUCHINPUTPOINT_MAX);

    DXLUA_INSTALL(library, PAD_INPUT_DOWN);
    DXLUA_INSTALL(library, PAD_INPUT_LEFT);
    DXLUA_INSTALL(library, PAD_INPUT_RIGHT);
    DXLUA_INSTALL(library, PAD_INPUT_UP);
    DXLUA_INSTALL(library, PAD_INPUT_A);
    DXLUA_INSTALL(library, PAD_INPUT_B);
    DXLUA_INSTALL(library, PAD_INPUT_C);
    DXLUA_INSTALL(library, PAD_INPUT_X);
    DXLUA_INSTALL(library, PAD_INPUT_Y);
    DXLUA_INSTALL(library, PAD_INPUT_Z);
    DXLUA_INSTALL(library, PAD_INPUT_L);
    DXLUA_INSTALL(library, PAD_INPUT_R);
    DXLUA_INSTALL(library, PAD_INPUT_START);
    DXLUA_INSTALL(library, PAD_INPUT_M);
    DXLUA_INSTALL(library, PAD_INPUT_D);
    DXLUA_INSTALL(library, PAD_INPUT_F);
    DXLUA_INSTALL(library, PAD_INPUT_G);
    DXLUA_INSTALL(library, PAD_INPUT_H);
    DXLUA_INSTALL(library, PAD_INPUT_I);
    DXLUA_INSTALL(library, PAD_INPUT_J);
    DXLUA_INSTALL(library, PAD_INPUT_K);
    DXLUA_INSTALL(library, PAD_INPUT_LL);
    DXLUA_INSTALL(library, PAD_INPUT_N);
    DXLUA_INSTALL(library, PAD_INPUT_O);
    DXLUA_INSTALL(library, PAD_INPUT_P);
    DXLUA_INSTALL(library, PAD_INPUT_RR);
    DXLUA_INSTALL(library, PAD_INPUT_S);
    DXLUA_INSTALL(library, PAD_INPUT_T);
    DXLUA_INSTALL(library, PAD_INPUT_U);
    DXLUA_INSTALL(library, PAD_INPUT_V);
    DXLUA_INSTALL(library, PAD_INPUT_W);
    DXLUA_INSTALL(library, PAD_INPUT_XX);

    DXLUA_INSTALL(library, PAD_INPUT_1);
    DXLUA_INSTALL(library, PAD_INPUT_2);
    DXLUA_INSTALL(library, PAD_INPUT_3);
    DXLUA_INSTALL(library, PAD_INPUT_4);
    DXLUA_INSTALL(library, PAD_INPUT_5);
    DXLUA_INSTALL(library, PAD_INPUT_6);
    DXLUA_INSTALL(library, PAD_INPUT_7);
    DXLUA_INSTALL(library, PAD_INPUT_8);
    DXLUA_INSTALL(library, PAD_INPUT_9);
    DXLUA_INSTALL(library, PAD_INPUT_10);
    DXLUA_INSTALL(library, PAD_INPUT_11);
    DXLUA_INSTALL(library, PAD_INPUT_12);
    DXLUA_INSTALL(library, PAD_INPUT_13);
    DXLUA_INSTALL(library, PAD_INPUT_14);
    DXLUA_INSTALL(library, PAD_INPUT_15);
    DXLUA_INSTALL(library, PAD_INPUT_16);
    DXLUA_INSTALL(library, PAD_INPUT_17);
    DXLUA_INSTALL(library, PAD_INPUT_18);
    DXLUA_INSTALL(library, PAD_INPUT_19);
    DXLUA_INSTALL(library, PAD_INPUT_20);
    DXLUA_INSTALL(library, PAD_INPUT_21);
    DXLUA_INSTALL(library, PAD_INPUT_22);
    DXLUA_INSTALL(library, PAD_INPUT_23);
    DXLUA_INSTALL(library, PAD_INPUT_24);
    DXLUA_INSTALL(library, PAD_INPUT_25);
    DXLUA_INSTALL(library, PAD_INPUT_26);
    DXLUA_INSTALL(library, PAD_INPUT_27);
    DXLUA_INSTALL(library, PAD_INPUT_28);

#undef DXLUA_INSTALL

    return library;
}

} // namespace DxLua
