#include <DxLib.h>
#include <lua.hpp>
#include <sol/sol.hpp>

#include "luaapi.h"

namespace DxLua::detail {

void port_asyncload(sol::state_view &lua, sol::table &t) {

#ifndef DX_NON_ASYNCLOAD

	// 非同期読み込み関係
	t["SetUseASyncLoadFlag"] = [](sol::variadic_args va) {
		int Flag = va_get(va, 0, false);
		return SetUseASyncLoadFlag(Flag);
	};
	t["GetUseASyncLoadFlag"] = []() {
		return GetUseASyncLoadFlag() ? TRUE : FALSE;
	};
	DXLUA_PORT(t, CheckHandleASyncLoad);
	DXLUA_PORT(t, GetHandleASyncLoadResult);
	DXLUA_PORT(t, SetASyncLoadFinishDeleteFlag);
	DXLUA_PORT(t, GetASyncLoadNum);
	DXLUA_PORT(t, SetASyncLoadThreadNum);

#endif // DX_NON_ASYNCLOAD

}

} // namespace DxLua::detail
