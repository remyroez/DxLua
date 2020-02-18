#ifndef DXLUA_CONTEXT_H_
#define DXLUA_CONTEXT_H_

#include <vector>
#include <unordered_map>
#include <memory>
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

	// 入力バッファを要求サイズ分確保して返す
	char *require_input_buffer(size_t size) {
		_input_buffer.clear();
		_input_buffer.resize(size);
		return _input_buffer.data();
	}

	// 入力バッファのクリア
	void clear_input_buffer() {
		_input_buffer.clear();
	}

	// キー入力情報の登録
	void register_key_input(int handle, size_t size, bool cancelable, bool ascii_only, bool number_only, bool zenkaku_only = false, bool enable_new_line = false) {
		_key_inputs.emplace(
			handle,
			std::make_unique<key_input>(handle, size, cancelable, ascii_only, number_only, zenkaku_only, enable_new_line)
		);
	}

	// キー入力情報の登録解除
	size_t get_key_input_size(int handle) {
		return (_key_inputs.find(handle) != _key_inputs.end()) ? _key_inputs[handle]->size : 0;
	}

	// キー入力情報の登録解除
	void deregister_key_input(int handle) {
		_key_inputs.erase(handle);
	}

	// すべてのキー入力情報の登録解除
	void clear_key_inputs() {
		_key_inputs.clear();
	}

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

	// 入力バッファ
	std::string _input_buffer;

	// 入力情報
	struct key_input {
		key_input(int _handle, size_t _size, bool _cancelable, bool _ascii_only, bool _number_only, bool _zenkaku_only = false, bool _enable_new_line = false)
		: handle(_handle), size(_size), cancelable(_cancelable), ascii_only(_ascii_only), number_only(_number_only), zenkaku_only(_zenkaku_only), enable_new_line(_enable_new_line) {}

		// ハンドル
		int handle = -1;

		// サイズ
		size_t size = 0;

		// キャンセル可否
		bool cancelable = true;

		// 半角のみ
		bool ascii_only = true;

		// 数字のみ
		bool number_only = true;

		// 全角のみ
		bool zenkaku_only = false;

		// 改行可否
		bool enable_new_line = false;
	};

	// 入力情報マップ
	std::unordered_map<int, std::unique_ptr<key_input>> _key_inputs;
};

} // namespace DxLua::detail

#endif // DXLUA_CONTEXT_H_
