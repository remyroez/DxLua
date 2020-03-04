#include <DxLib.h>
#include <lua.hpp>
#include <sol/sol.hpp>

#include "luaapi.h"

namespace DxLua::detail {

void port_type(sol::state_view &lua, sol::table &t) {
	// VECTOR
	{
		// ユーザー型定義
		auto VECTOR = t.new_usertype<DxLib::VECTOR>(
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
		sol::table tableVECTOR = t["VECTOR"];

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
		DXLUA_PORT_EX(t, FLOAT3, VECTOR);
	}

	// DINPUT_JOYSTATE
	{
		// ユーザー型定義
		auto DINPUT_JOYSTATE = t.new_usertype<DxLib::DINPUT_JOYSTATE>(
			"DINPUT_JOYSTATE",
			"X", &DxLib::DINPUT_JOYSTATE::X,
			"Y", &DxLib::DINPUT_JOYSTATE::Y,
			"Z", &DxLib::DINPUT_JOYSTATE::Z,
			"Rx", &DxLib::DINPUT_JOYSTATE::Rx,
			"Ry", &DxLib::DINPUT_JOYSTATE::Ry,
			"Rz", &DxLib::DINPUT_JOYSTATE::Rz,
			"Slider", sol::property([](DxLib::DINPUT_JOYSTATE &self) { return std::ref(self.Slider); }),
			"POV", sol::property([](DxLib::DINPUT_JOYSTATE &self) { return std::ref(self.POV); }),
			"Buttons", sol::property([](DxLib::DINPUT_JOYSTATE &self) { return std::ref(self.Buttons); })
		);

		// ユーザー型をテーブルとして取得
		sol::table table = t["DINPUT_JOYSTATE"];

		// メタテーブルの作成
		sol::table metatable = table[sol::metatable_key] = lua.create_table();

		// 専用の new 関数を用意
		table["new"] = [](sol::object arg) {
			// 空インスタンス
			DxLib::DINPUT_JOYSTATE instance;
			memset(&instance, 0, sizeof(instance));

			if (!arg.is<sol::table>()) {
				// テーブルではない

			} else if (sol::table argt = arg.as<sol::table>()) {
				// テーブルからコピー
				instance.X = argt["X"].get_or(0);
				instance.Y = argt["Y"].get_or(0);
				instance.Z = argt["Z"].get_or(0);
				instance.Rx = argt["Rx"].get_or(0);
				instance.Ry = argt["Ry"].get_or(0);
				instance.Rz = argt["Rz"].get_or(0);

				// Slider
				if (sol::object obj = argt["Slider"]; obj.is<sol::table>()) {
					sol::table t = obj;
					for (int i = 0; i < 2; ++i) {
						instance.Slider[i] = t[i + 1].get_or(0);
					}
				}

				// POV
				if (sol::object obj = argt["POV"]; obj.is<sol::table>()) {
					sol::table t = obj;
					for (int i = 0; i < 4; ++i) {
						instance.POV[i] = t[i + 1].get_or(0U);
					}
				}

				// Buttons
				if (sol::object obj = argt["Buttons"]; obj.is<sol::table>()) {
					sol::table t = obj;
					for (int i = 0; i < 32; ++i) {
						instance.Buttons[i] = t[i + 1].get_or(0U);
					}
				}
			}
			return instance;
		};

		// ユーザー型テーブルの呼び出しで new を呼ぶように対応
		metatable["__call"] = [](sol::stack_object self, sol::variadic_args va) { return sol::table(self)["new"](va); };
	}

	// IPDATA
	{
		// ユーザー型定義
		auto IPDATA = t.new_usertype<DxLib::IPDATA>(
			"IPDATA",
			"d1", &DxLib::IPDATA::d1,
			"d2", &DxLib::IPDATA::d2,
			"d3", &DxLib::IPDATA::d3,
			"d4", &DxLib::IPDATA::d4,
			sol::meta_function::to_string, [](const DxLib::IPDATA &ipdata) {
				std::ostringstream ost;
				ost
					<< "{ d1 = " << ipdata.d1
					<< ", d2 = " << ipdata.d2
					<< ", d3 = " << ipdata.d3
					<< ", d4 = " << ipdata.d4
					<< " }";
				return ost.str();
			}
		);
	}

	// IPDATA_IPv6
	{
		// ユーザー型定義
		auto IPDATA_IPv6 = t.new_usertype<DxLib::IPDATA_IPv6>(
			"IPDATA_IPv6",
			"ScopeID", &DxLib::IPDATA_IPv6::ScopeID,
			sol::meta_function::to_string, [](const DxLib::IPDATA_IPv6 &ipdata) {
				std::ostringstream ost;
				ost
					<< "{ ScopeID = " << ipdata.ScopeID
					<< " }";
				return ost.str();
			}
		);
	}
}

} // namespace DxLua::detail
