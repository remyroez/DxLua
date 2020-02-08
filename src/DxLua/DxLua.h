#ifndef DXLUA_H_
#define DXLUA_H_

#include <lua.hpp>
#include <sol/sol.hpp>

#include <filesystem>
#include <sstream>

namespace DxLua {

// DxLua ライブラリの展開
sol::table openDxLua(sol::this_state s);

} // namespace DxLua

#endif // DXLUA_H_
