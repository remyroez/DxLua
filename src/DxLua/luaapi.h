#ifndef DXLUA_LUAAPI_H_
#define DXLUA_LUAAPI_H_

#include <lua.hpp>
#include <sol/sol.hpp>

#define DXLUA_PORT(T, X) T[#X] = X;

namespace DxLua::detail {

void portDefine(sol::table &t);

} // namespace DxLua::detail

#endif // DXLUA_LUAAPI_H_
