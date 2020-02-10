#include <DxLib.h>
#include <lua.hpp>
#include <sol/sol.hpp>

#include "context.h"

namespace DxLua::detail {

// コンテキストのキー
const std::string context::key = "__context__";

// 監視
bool context::watch() {
	bool modified = false;

	for (auto &watchee : _watchees) {
		modified = watchee.watch() || modified;
	}

	return modified;
}

// コンテキストの設定
void context::set(sol::table& t) {
	auto type_key = "__context_type__";
	
	// 型定義
	if (!t[type_key]) {
		auto type = t.new_usertype<context>(
			type_key,
			"watch", &context::watch
		);
	}

	// インスタンス化
	if (!t[detail::context::key]) {
		t[detail::context::key].set(context());
	}
}

// 監視
bool context::watchee::watch() {
	bool modified = false;

	// 最後に書き込まれた時間の取得
	auto lwt = std::filesystem::last_write_time(path);

	// 変更されたかどうか
	if (lwt != last_write_time) {
		last_write_time = lwt;
		modified = true;
	}

	return modified;
}

} // namespace DxLua::detail
