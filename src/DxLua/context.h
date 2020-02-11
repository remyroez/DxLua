#ifndef DXLUA_CONTEXT_H_
#define DXLUA_CONTEXT_H_

#include <vector>
#include <string>
#include <filesystem>

#include <lua.hpp>
#include <sol/sol.hpp>

// コンテキスト取得マクロ
#define DXLUA_CONTEXT(X) (detail::context::get(X))

namespace DxLua::detail {

class context final {
public:
	// コンストラクタ
	context() = default;

	// デストラクタ
	~context() = default;

	// コピーコンストラクタなどの削除
	context(const context &) = delete;
	context &operator=(const context &) = delete;

	// ムーブコンストラクタ
	context(context &&other) noexcept
		: _watchees(std::move(other._watchees)), _base_path(std::move(other._base_path)) {}

	// ムーブ代入
	context &operator=(context &&other) noexcept {
		_watchees = std::move(other._watchees);
		_base_path = std::move(other._base_path);
		return *this;
	}

public:
	// 監視対象の追加
	void add_watchee(const std::filesystem::path &watchee) {
		if (std::filesystem::exists(watchee)) {
			_watchees.emplace_back(watchee);
			_watchees.back().watch();
		}
	}

	// 監視対象の追加
	void add_watchee(std::filesystem::path &&watchee) {
		if (std::filesystem::exists(watchee)) {
			_watchees.emplace_back(watchee);
			_watchees.back().watch();
		}
	}

	// 監視対象のクリア
	void clear_watchee() { _watchees.clear(); }

	// ベースパスの設定
	void set_base_path(const std::filesystem::path &path) {
		_base_path = path;
	}

	// ベースパスを連結して返す
	std::filesystem::path append_base_path(const std::filesystem::path &path) const {
		return _base_path.empty() ? path : (_base_path / path);
	}

	// ベースパスのクリア
	void clear_base_path() { _base_path.clear(); }

public: // Lua API
	// 監視
	bool watch();

public:
	// コンテキストの取得
	static context &get(sol::table &t) { return t[context::key].get<context>(); }

	// コンテキストの取得
	static context &get(sol::object &o) { return const_cast<context &>(context::get(o.as<sol::table>())); }

	// コンテキストの取得
	static const context &get(const sol::table &t) { return t[context::key].get<context>(); }

	// コンテキストの取得
	static const context &get(const sol::object &o) { return context::get(o.as<sol::table>()); }

	// コンテキストの設定
	static void set(sol::table &t);

	// コンテキストのキー
	static const std::string key;

private:
	// 監視対象型
	struct watchee {
		watchee(std::filesystem::path watchee_path) : path(watchee_path) {}

		// 監視
		bool watch();

		// パス
		std::filesystem::path path;

		// 最後に変更された時間
		std::filesystem::file_time_type last_write_time;
	};

	// 監視対象
	std::vector<watchee> _watchees;

	// ベースパス
	std::filesystem::path _base_path;
};

} // namespace DxLua::detail

#endif // DXLUA_CONTEXT_H_
