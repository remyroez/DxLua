#include <DxLib.h>
#include <lua.hpp>
#include <sol/sol.hpp>

#include "DxLua.h"
#include "luaapi.h"
#include "context.h"

namespace DxLua {

sol::table openDxLua(sol::this_state s)
{
    // ステート
    sol::state_view lua(s);

    // テーブル
    // TODO: DxLua ユーザータイプ
    sol::table library = lua.create_table();

    // 実行関数
    {
        // ハードコーディング
        // TODO: バイナリ化
        auto result = lua.safe_script(R"lua( return function (...)
    -- 終了関数
    local QuitCode = false
    function DxLua.Quit(code)
        QuitCode = code or 'exit'
    end

    -- ループ
    if DxLua.Update then
        local BeforeTime = DxLua.GetNowHiPerformanceCount()
        while (DxLua.ProcessMessage() == 0) do
            local NowTime = DxLua.GetNowHiPerformanceCount()
            local DeltaTime = (NowTime - BeforeTime) / 1000000
            BeforeTime = NowTime;
            local result = DxLua.Update(DeltaTime) or QuitCode
            if result then
                return result -- エラーが起きたら直ちに終了
            end
        end
    end

    return 'exit'
end)lua"
        );
        if (!result.valid()) {
            // チャンク作成失敗
            sol::error err = result;
            std::cerr << err.what() << std::endl;

        } else {
            // チャンク作成成功
            if (sol::object fn = result.get<sol::function>(); fn.is<sol::function>()) {
                library["Run"] = fn;
            }
        }
    }

    auto VECTOR = lua.new_usertype<DxLib::VECTOR>(
        "VECTOR",
        "x", &DxLib::VECTOR::x,
        "y", &DxLib::VECTOR::y,
        "z", &DxLib::VECTOR::z,
        sol::meta_function::to_string, [](const DxLib::VECTOR &v) {
            std::ostringstream ost;
            ost << "{ x = " << v.x << ", y = " << v.y << ", z = " << v.z << " }";
            return ost.str();
        }
    );
    sol::table tableVECTOR = lua["VECTOR"];
    sol::table metaVECTOR = tableVECTOR[sol::metatable_key] = lua.create_table();

    tableVECTOR["new"] = [](sol::variadic_args va) {
        if (va.leftover_count() > 0 && va[0].is<sol::table>()) {
            sol::table t = va[0].as<sol::table>();
            if (t[1].get_type() == sol::type::number) {
                return DxLib::VECTOR{
                    t[1].get_or<float>(0),
                    t[2].get_or<float>(0),
                    t[3].get_or<float>(0)
                };

            } else {
                return DxLib::VECTOR{
                    t["x"].get_or<float>(0),
                    t["y"].get_or<float>(0),
                    t["z"].get_or<float>(0)
                };
            }

        } else {
            return DxLib::VECTOR{
                va.leftover_count() > 0 && va[0].is<float>() ? va[0].as<float>() : 0,
                va.leftover_count() > 1 && va[0].is<float>() ? va[1].as<float>() : 0,
                va.leftover_count() > 2 && va[0].is<float>() ? va[2].as<float>() : 0
            };
        }
    };
    metaVECTOR["__call"] = [](sol::stack_object self, sol::variadic_args va) { return sol::table(self)["new"](va); };
    DXLUA_PORT(library, VECTOR);
    lua["VECTOR"] = sol::nil;

    // 小分けにした各ポート関数を呼ぶ
    detail::portDefine(library);

    // 以下、ポーティング

    DXLUA_PORT(library, ProcessMessage);

    DXLUA_PORT(library, WaitTimer);
    DXLUA_PORT(library, WaitKey);

    library["GetNowCount"] = [](sol::variadic_args va) {
        return GetNowCount(va.leftover_count() > 0 ? (va[0].as<bool>() ? TRUE : FALSE) : FALSE);
    };
    library["GetNowHiPerformanceCount"] = [](sol::variadic_args va) {
        return GetNowHiPerformanceCount(va.leftover_count() > 0 ? (va[0].as<bool>() ? TRUE : FALSE) : FALSE);
    };

    DXLUA_PORT(library, GetRand);
    DXLUA_PORT(library, SRand);

    //DXLUA_PORT(library, SetGraphMode);
    library["SetGraphMode"] = [](int ScreenSizeX, int ScreenSizeY, int ColorBitDepth, sol::variadic_args va) {
        return SetGraphMode(ScreenSizeX, ScreenSizeY, ColorBitDepth, va.leftover_count() > 0 ? va[0].as<int>() : 60);
    };

    DXLUA_PORT(library, ScreenFlip);
    library["ClearDrawScreen"] = [](sol::variadic_args va) {
        return ClearDrawScreen();
    };
    DXLUA_PORT(library, SetDrawScreen);

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
    DXLUA_PORT(library, DrawPixel);

    DXLUA_PORT(library, GetColor);

    DXLUA_PORT(library, SetFontSize);
    DXLUA_PORT(library, SetDrawBright);

    DXLUA_PORT(library, CheckHitKey);
    library["CheckHitKeyAll"] = [](sol::variadic_args va) {
        return CheckHitKeyAll(va.leftover_count() > 0 ? va[0].as<int>() : DX_CHECKINPUT_ALL);
    };
    DXLUA_PORT(library, GetJoypadInputState);

    return library;
}

} // namespace DxLua
