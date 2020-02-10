#ifndef DXLUA_H_
#define DXLUA_H_

#include <lua.hpp>
#include <sol/sol.hpp>

namespace DxLua {

// DxLua ライブラリの展開
sol::table solopen_dxlua(sol::this_state s);

} // namespace DxLua

#endif // DXLUA_H_
