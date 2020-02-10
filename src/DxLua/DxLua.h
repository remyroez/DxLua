#ifndef DXLUA_H_
#define DXLUA_H_

#include <filesystem>

#include <lua.hpp>
#include <sol/sol.hpp>

namespace DxLua {

// 監視対象の追加
void add_watchee(sol::object &lib, const std::filesystem::path& watchee);

// 監視対象の追加
void add_watchee(sol::object &lib, std::filesystem::path&& watchee);

// 監視対象のクリア
void clear_watchee(sol::object &lib);

// 監視
bool watch(sol::object& lib);

// DxLua ライブラリの展開
sol::table solopen_dxlua(sol::this_state s);

} // namespace DxLua

#endif // DXLUA_H_
