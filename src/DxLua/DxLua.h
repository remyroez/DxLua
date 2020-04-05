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

// 入力バッファを要求サイズ分確保して返す
char *require_input_buffer(sol::object &library, size_t size);

// キー入力情報の登録
void register_key_input(sol::object &library, int handle, size_t size, bool cancelable, bool ascii_only, bool number_only, bool zenkaku_only = false, bool enable_new_line = false);

// キー入力情報の登録解除
size_t get_key_input_size(sol::object &library, int handle);

// キー入力情報の登録解除
void deregister_key_input(sol::object &library, int handle);

// すべてのキー入力情報の登録解除
void clear_key_inputs(sol::object &library);

// DxLua ライブラリの展開
sol::table open_library(sol::this_state s);

} // namespace DxLua

#endif // DXLUA_H_
