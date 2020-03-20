#ifndef DXLUA_LUAAPI_H_
#define DXLUA_LUAAPI_H_

#include <lua.hpp>
#include <sol/sol.hpp>

#define DXLUA_PORT_EX(T, X, Y) T[#X] = Y;
#define DXLUA_PORT(T, X) T[#X] = X;
#define DXLUA_REMOVE(T, X) T[#X] = sol::nil;

namespace DxLua::detail {

void port_define(sol::state_view &lua, sol::table &t);
void port_type(sol::state_view &lua, sol::table &t);
void port_network(sol::state_view &lua, sol::table &t);
void port_keyinput(sol::state_view &lua, sol::table &t);
void port_input(sol::state_view &lua, sol::table &t);
void port_draw(sol::state_view &lua, sol::table &t);
void port_model(sol::state_view &lua, sol::table &t);

} // namespace DxLua::detail

#endif // DXLUA_LUAAPI_H_
