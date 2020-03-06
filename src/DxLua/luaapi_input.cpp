#include <tuple>

#include <DxLib.h>
#include <lua.hpp>
#include <sol/sol.hpp>

#include "luaapi.h"

namespace DxLua::detail {

void port_input(sol::state_view &lua, sol::table &t) {
#ifndef DX_NON_INPUT

	DXLUA_PORT(t, CheckHitKey);
	t["CheckHitKeyAll"] = [](sol::variadic_args va) {
		return CheckHitKeyAll(va.leftover_count() > 0 ? va[0].as<int>() : DX_CHECKINPUT_ALL);
	};
	t["GetHitKeyStateAll"] = [](sol::table KeyStateArray) {
		std::array<DX_CHAR, 256> keystates;
		auto result = GetHitKeyStateAll(keystates.data());
		for (int i = 0; i < keystates.size(); ++i) {
			KeyStateArray[i + 1] = (int)keystates[i];
		}
		return result;
	};
	DXLUA_PORT(t, GetJoypadNum);
	DXLUA_PORT(t, GetJoypadButtonNum);
	DXLUA_PORT(t, GetJoypadInputState);
	t["GetJoypadAnalogInput"] = [](int InputType) {
		int X = 0, Y = 0;
		auto result = GetJoypadAnalogInput(&X, &Y, InputType);
		return std::make_tuple(result, X, Y);
	};
	DXLUA_PORT(t, GetJoypadDirectInputState);
	DXLUA_PORT(t, CheckJoypadXInput);
	DXLUA_PORT(t, GetJoypadXInputState);
	DXLUA_PORT(t, GetJoypadXInputState);
	t["SetJoypadInputToKeyInput"] = [](int InputType, int PadInput, int KeyInput1, sol::variadic_args va) {
		int KeyInput2 = va.leftover_count() > 0 ? va[0].as<int>() : -1;
		int KeyInput3 = va.leftover_count() > 1 ? va[1].as<int>() : -1;
		int KeyInput4 = va.leftover_count() > 2 ? va[2].as<int>() : -1;
		return SetJoypadInputToKeyInput(InputType, PadInput, KeyInput1, KeyInput2, KeyInput3, KeyInput4);
	};
	DXLUA_PORT(t, SetJoypadDeadZone);
	DXLUA_PORT(t, GetJoypadDeadZone);
	t["StartJoypadVibration"] = [](int InputType, int Power, int Time, sol::variadic_args va) {
		int EffectIndex = va.leftover_count() > 0 ? va[0].as<int>() : -1;
		return StartJoypadVibration(InputType, Power, Time, EffectIndex);
	};
	t["StopJoypadVibration"] = [](int InputType, sol::variadic_args va) {
		int EffectIndex = va.leftover_count() > 0 ? va[0].as<int>() : -1;
		return StopJoypadVibration(InputType, EffectIndex);
	};
	DXLUA_PORT(t, GetJoypadPOVState);
	DXLUA_PORT(t, ReSetupJoypad);

	t["SetUseJoypadVibrationFlag"] = [](sol::object Flag) {
		return SetUseJoypadVibrationFlag(Flag.as<bool>() ? TRUE : FALSE);
	};
	
	DXLUA_PORT(t, GetMouseInput);

#endif // DX_NON_INPUT
}

} // namespace DxLua::detail
