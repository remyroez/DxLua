#include <DxLib.h>
#include <lua.hpp>
#include <sol/sol.hpp>

#include "luaapi.h"

namespace DxLua::detail {

void port_type(sol::state_view &lua, sol::table &t) {
	// VECTOR
	{
		// ユーザー型定義
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

		// ユーザー型をテーブルとして取得
		sol::table tableVECTOR = lua["VECTOR"];

		// メタテーブルの作成
		sol::table metaVECTOR = tableVECTOR[sol::metatable_key] = lua.create_table();

		// 専用の new 関数を用意
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

		// ユーザー型テーブルの呼び出しで new を呼ぶように対応
		metaVECTOR["__call"] = [](sol::stack_object self, sol::variadic_args va) { return sol::table(self)["new"](va); };

		// ライブラリに追加
		DXLUA_PORT(t, VECTOR);
		DXLUA_PORT_EX(t, FLOAT3, VECTOR);

		// グローバルから削除
		DXLUA_REMOVE(lua, VECTOR);
	}
}

} // namespace DxLua::detail
