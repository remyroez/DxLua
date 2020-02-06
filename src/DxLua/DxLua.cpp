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

    DXLUA_INSTALL(library, GetNowCount);
    library["GetNowCount"] = [](sol::variadic_args va) {
        return GetNowCount(va.leftover_count() > 0 ? (va[0].as<bool>() ? TRUE : FALSE) : FALSE);
    };

    DXLUA_INSTALL(library, GetRand);
    DXLUA_INSTALL(library, SRand);

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
    library["DrawString"] = [](int x, int y, const TCHAR* String, unsigned int Color, sol::variadic_args va) {
        return DrawString(x, y, String, Color, va.leftover_count() > 0 ? va[0].as<int>() : 0);
    };

    library["DrawLine"] = [](int x1, int y1, int x2, int y2, unsigned int Color, sol::variadic_args va) {
        return DrawLine(x1, y1, x2, y2, Color, va.leftover_count() > 0 ? va[0].as<int>() : 1);
    };
    library["DrawBox"] = [](int x1, int y1, int x2, int y2, unsigned int Color, bool FillFlag) {
        return DrawBox(x1, y1, x2, y2, Color, FillFlag ? TRUE : FALSE);
    };
    DXLUA_INSTALL(library, DrawPixel);

    DXLUA_INSTALL(library, GetColor);

    DXLUA_INSTALL(library, SetFontSize);
    DXLUA_INSTALL(library, SetDrawBright);

    DXLUA_INSTALL(library, DX_SCREEN_FRONT);
    DXLUA_INSTALL(library, DX_SCREEN_BACK);
    DXLUA_INSTALL(library, DX_SCREEN_WORK);
    DXLUA_INSTALL(library, DX_SCREEN_TEMPFRONT);
    DXLUA_INSTALL(library, DX_SCREEN_OTHER);

    DXLUA_INSTALL(library, CheckHitKey);
    library["CheckHitKeyAll"] = [](sol::variadic_args va) {
        return CheckHitKeyAll(va.leftover_count() > 0 ? va[0].as<int>() : DX_CHECKINPUT_ALL);
    };
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

    DXLUA_INSTALL(library, KEY_INPUT_BACK);
    DXLUA_INSTALL(library, KEY_INPUT_TAB);
    DXLUA_INSTALL(library, KEY_INPUT_RETURN);

    DXLUA_INSTALL(library, KEY_INPUT_LSHIFT);
    DXLUA_INSTALL(library, KEY_INPUT_RSHIFT);
    DXLUA_INSTALL(library, KEY_INPUT_LCONTROL);
    DXLUA_INSTALL(library, KEY_INPUT_RCONTROL);
    DXLUA_INSTALL(library, KEY_INPUT_ESCAPE);
    DXLUA_INSTALL(library, KEY_INPUT_SPACE);
    DXLUA_INSTALL(library, KEY_INPUT_PGUP);
    DXLUA_INSTALL(library, KEY_INPUT_PGDN);
    DXLUA_INSTALL(library, KEY_INPUT_END);
    DXLUA_INSTALL(library, KEY_INPUT_HOME);
    DXLUA_INSTALL(library, KEY_INPUT_LEFT);
    DXLUA_INSTALL(library, KEY_INPUT_UP);
    DXLUA_INSTALL(library, KEY_INPUT_RIGHT);
    DXLUA_INSTALL(library, KEY_INPUT_DOWN);
    DXLUA_INSTALL(library, KEY_INPUT_INSERT);
    DXLUA_INSTALL(library, KEY_INPUT_DELETE);

    DXLUA_INSTALL(library, KEY_INPUT_MINUS);
    DXLUA_INSTALL(library, KEY_INPUT_YEN);
    DXLUA_INSTALL(library, KEY_INPUT_PREVTRACK);
    DXLUA_INSTALL(library, KEY_INPUT_PERIOD);
    DXLUA_INSTALL(library, KEY_INPUT_SLASH);
    DXLUA_INSTALL(library, KEY_INPUT_LALT);
    DXLUA_INSTALL(library, KEY_INPUT_RALT);
    DXLUA_INSTALL(library, KEY_INPUT_SCROLL);
    DXLUA_INSTALL(library, KEY_INPUT_SEMICOLON);
    DXLUA_INSTALL(library, KEY_INPUT_COLON);
    DXLUA_INSTALL(library, KEY_INPUT_LBRACKET);
    DXLUA_INSTALL(library, KEY_INPUT_RBRACKET);
    DXLUA_INSTALL(library, KEY_INPUT_AT);
    DXLUA_INSTALL(library, KEY_INPUT_BACKSLASH);
    DXLUA_INSTALL(library, KEY_INPUT_COMMA);
    DXLUA_INSTALL(library, KEY_INPUT_KANJI);
    DXLUA_INSTALL(library, KEY_INPUT_CONVERT);
    DXLUA_INSTALL(library, KEY_INPUT_NOCONVERT);
    DXLUA_INSTALL(library, KEY_INPUT_KANA);
    DXLUA_INSTALL(library, KEY_INPUT_APPS);
    DXLUA_INSTALL(library, KEY_INPUT_CAPSLOCK);
    DXLUA_INSTALL(library, KEY_INPUT_SYSRQ);
    DXLUA_INSTALL(library, KEY_INPUT_PAUSE);
    DXLUA_INSTALL(library, KEY_INPUT_LWIN);
    DXLUA_INSTALL(library, KEY_INPUT_RWIN);

    DXLUA_INSTALL(library, KEY_INPUT_NUMLOCK);
    DXLUA_INSTALL(library, KEY_INPUT_NUMPAD0);
    DXLUA_INSTALL(library, KEY_INPUT_NUMPAD1);
    DXLUA_INSTALL(library, KEY_INPUT_NUMPAD2);
    DXLUA_INSTALL(library, KEY_INPUT_NUMPAD3);
    DXLUA_INSTALL(library, KEY_INPUT_NUMPAD4);
    DXLUA_INSTALL(library, KEY_INPUT_NUMPAD5);
    DXLUA_INSTALL(library, KEY_INPUT_NUMPAD6);
    DXLUA_INSTALL(library, KEY_INPUT_NUMPAD7);
    DXLUA_INSTALL(library, KEY_INPUT_NUMPAD8);
    DXLUA_INSTALL(library, KEY_INPUT_NUMPAD9);
    DXLUA_INSTALL(library, KEY_INPUT_MULTIPLY);
    DXLUA_INSTALL(library, KEY_INPUT_ADD);
    DXLUA_INSTALL(library, KEY_INPUT_SUBTRACT);
    DXLUA_INSTALL(library, KEY_INPUT_DECIMAL);
    DXLUA_INSTALL(library, KEY_INPUT_DIVIDE);
    DXLUA_INSTALL(library, KEY_INPUT_NUMPADENTER);

    DXLUA_INSTALL(library, KEY_INPUT_F1);
    DXLUA_INSTALL(library, KEY_INPUT_F2);
    DXLUA_INSTALL(library, KEY_INPUT_F3);
    DXLUA_INSTALL(library, KEY_INPUT_F4);
    DXLUA_INSTALL(library, KEY_INPUT_F5);
    DXLUA_INSTALL(library, KEY_INPUT_F6);
    DXLUA_INSTALL(library, KEY_INPUT_F7);
    DXLUA_INSTALL(library, KEY_INPUT_F8);
    DXLUA_INSTALL(library, KEY_INPUT_F9);
    DXLUA_INSTALL(library, KEY_INPUT_F10);
    DXLUA_INSTALL(library, KEY_INPUT_F11);
    DXLUA_INSTALL(library, KEY_INPUT_F12);

    DXLUA_INSTALL(library, KEY_INPUT_A);
    DXLUA_INSTALL(library, KEY_INPUT_B);
    DXLUA_INSTALL(library, KEY_INPUT_C);
    DXLUA_INSTALL(library, KEY_INPUT_D);
    DXLUA_INSTALL(library, KEY_INPUT_E);
    DXLUA_INSTALL(library, KEY_INPUT_F);
    DXLUA_INSTALL(library, KEY_INPUT_G);
    DXLUA_INSTALL(library, KEY_INPUT_H);
    DXLUA_INSTALL(library, KEY_INPUT_I);
    DXLUA_INSTALL(library, KEY_INPUT_J);
    DXLUA_INSTALL(library, KEY_INPUT_K);
    DXLUA_INSTALL(library, KEY_INPUT_L);
    DXLUA_INSTALL(library, KEY_INPUT_M);
    DXLUA_INSTALL(library, KEY_INPUT_N);
    DXLUA_INSTALL(library, KEY_INPUT_O);
    DXLUA_INSTALL(library, KEY_INPUT_P);
    DXLUA_INSTALL(library, KEY_INPUT_Q);
    DXLUA_INSTALL(library, KEY_INPUT_R);
    DXLUA_INSTALL(library, KEY_INPUT_S);
    DXLUA_INSTALL(library, KEY_INPUT_T);
    DXLUA_INSTALL(library, KEY_INPUT_U);
    DXLUA_INSTALL(library, KEY_INPUT_V);
    DXLUA_INSTALL(library, KEY_INPUT_W);
    DXLUA_INSTALL(library, KEY_INPUT_X);
    DXLUA_INSTALL(library, KEY_INPUT_Y);
    DXLUA_INSTALL(library, KEY_INPUT_Z);

    DXLUA_INSTALL(library, KEY_INPUT_0);
    DXLUA_INSTALL(library, KEY_INPUT_1);
    DXLUA_INSTALL(library, KEY_INPUT_2);
    DXLUA_INSTALL(library, KEY_INPUT_3);
    DXLUA_INSTALL(library, KEY_INPUT_4);
    DXLUA_INSTALL(library, KEY_INPUT_5);
    DXLUA_INSTALL(library, KEY_INPUT_6);
    DXLUA_INSTALL(library, KEY_INPUT_7);
    DXLUA_INSTALL(library, KEY_INPUT_8);
    DXLUA_INSTALL(library, KEY_INPUT_9);

#undef DXLUA_INSTALL

    return library;
}

} // namespace DxLua
