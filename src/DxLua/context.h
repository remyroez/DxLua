#ifndef DXLUA_CONTEXT_H_
#define DXLUA_CONTEXT_H_

#include <lua.hpp>
#include <sol/sol.hpp>

#define DXLUA_PORT(T, X) T[#X] = X;

namespace DxLua::detail {

class context final {

};

} // namespace DxLua::detail

#endif // DXLUA_CONTEXT_H_
