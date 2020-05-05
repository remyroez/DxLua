#ifndef DXLUA_LUAAPI_H_
#define DXLUA_LUAAPI_H_

#include <lua.hpp>
#include <sol/sol.hpp>

#define DXLUA_PORT_EX(T, X, Y) T[#X] = Y;
#define DXLUA_PORT(T, X) T[#X] = X;
#define DXLUA_PORT_DX(T, X) T[#X] = DxLib::X;
#define DXLUA_REMOVE(T, X) T[#X] = sol::nil;

namespace DxLua::detail {

template<typename T>
inline auto va_get(sol::variadic_args &va, int index, const T &default_value) {
	return (va.leftover_count() > index) ? va[index].as<T>() : default_value;
}

template<>
inline auto va_get<bool>(sol::variadic_args &va, int index, const bool &default_value) {
	return ((va.leftover_count() > index) ? va[index].as<bool>() : default_value) ? TRUE : FALSE;
}

void port_define(sol::state_view &lua, sol::table &t);
void port_type(sol::state_view &lua, sol::table &t);
void port_network(sol::state_view &lua, sol::table &t);
void port_keyinput(sol::state_view &lua, sol::table &t);
void port_input(sol::state_view &lua, sol::table &t);
void port_draw(sol::state_view &lua, sol::table &t);
void port_model(sol::state_view &lua, sol::table &t);
void port_math(sol::state_view &lua, sol::table &t);
void port_baseimage(sol::state_view &lua, sol::table &t);
void port_system(sol::state_view &lua, sol::table &t);
void port_log(sol::state_view &lua, sol::table &t);
void port_asyncload(sol::state_view &lua, sol::table &t);
void port_file(sol::state_view &lua, sol::table &t);
void port_inputstring(sol::state_view &lua, sol::table &t);

} // namespace DxLua::detail

#endif // DXLUA_LUAAPI_H_
