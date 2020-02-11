#ifndef DXLUA_H_
#define DXLUA_H_

#include <filesystem>

#include <lua.hpp>
#include <sol/sol.hpp>

namespace DxLua {

// 監視対象の追加
void add_watchee(sol::object &library, const std::filesystem::path &watchee);

// 監視対象の追加
void add_watchee(sol::object &library, std::filesystem::path &&watchee);

// 監視対象のクリア
void clear_watchee(sol::object &library);

// ベースパスの設定
void set_base_path(sol::object &library, const std::filesystem::path &path);

// ベースパスを連結して返す
std::filesystem::path append_base_path(const sol::object &library, const std::filesystem::path &path);

// データパスに変換する
inline std::string to_data_path(const sol::object &library, const std::filesystem::path &path) {
	return append_base_path(library, path).string();
}

// データパスに変換する
inline std::string to_data_path(const sol::table &library, const std::filesystem::path &path) {
	return append_base_path(library.as<sol::object>(), path).string();
}

// ベースパスのクリア
void clear_base_path(sol::object &library);

// 監視
bool watch(sol::object &library);

// DxLua ライブラリの展開
sol::table solopen_dxlua(sol::this_state s);

} // namespace DxLua

#endif // DXLUA_H_
