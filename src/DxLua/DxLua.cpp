#include <lua.hpp>
#include <sol/sol.hpp>

#include "DxLua.h"

namespace DxLua {

sol::table openDxLua(sol::this_state s)
{
    sol::state_view lua(s);
    sol::table library = lua.create_table();

    return library;
}

} // namespace DxLua
