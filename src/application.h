#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <filesystem>
#include <sstream>

#include <lua.hpp>
#include <sol/sol.hpp>

namespace remyroez {

// アプリケーションクラス
class application final {
public:
	// コンストラクタ
	application() = default;

	// デストラクタ
	~application() = default;

	// コピーコンストラクタなどの削除
	application(const application&) = delete;
	application(application&&) = delete;
	application& operator=(const application&) = delete;

	// ムーブ代入
	application& operator=(application&& other) noexcept {
		_state = std::move(other._state);
		_dxLua = std::move(other._dxLua);
		_option = std::move(other._option);
		_attached_console = other._attached_console;
		_alloced_console = other._alloced_console;
		_initialized_engine = other._initialized_engine;
		return *this;
	}

	// 終了コード
	enum class done_code {
		// 未終了
		none,

		// 正常に終了
		exit,

		// リスタート要求
		restart,

		// リロード要求
		reload,

		// エラー発生
		error,
	};

public:
	// コンソールを確保済みかどうか
	bool has_console() const { return _attached_console || _alloced_console; }

public:
	// 設定処理
	bool setup(int argc = 0, const char** argv = nullptr);

	// 設定処理
	bool setup(int argc = 0, char** argv = nullptr) { return setup(argc, const_cast<const char **>(argv)); }

	// 終了処理
	bool teardown();

	// 起動処理
	done_code boot();

protected:
	// 実行処理
	done_code run();

	// スクリプトの初期化関数を呼ぶ
	application::done_code call_init(std::ostringstream& message);

	// スクリプトの実行関数を呼ぶ
	application::done_code call_run(std::ostringstream& message);

	// スクリプトの終了関数を呼ぶ
	application::done_code call_end(std::ostringstream& message);

	// 引数の解析
	bool parse_arguments(int argc, const char** argv);

	// Lua のセットアップ
	bool setup_lua();

	// コンソールのセットアップ
	bool setup_console();

	// コンソールの開放
	bool teardown_console();

	// エンジンのセットアップ
	bool setup_engine();

	// エンジンの初期化処理
	bool initialize_engine();

	// エンジンの終了処理
	bool finalize_engine();

	// スクリプトの読み込み
	bool load_script(std::ostringstream& message);

protected:
	// Lua 用パニック関数
	static void panic(sol::optional<std::string> maybe_msg);

private:
	// Lua ステート
	sol::state _state;

	// DxLua ライブラリオブジェクト
	sol::object _dxLua;

	// オプション
	struct option {
		// ファイル名
		std::filesystem::path filename;

		// パス
		std::filesystem::path path;

		// 引数として指定されたパス
		std::filesystem::path argpath;

		// 引数リスト
		std::vector<std::string> args;

		// コンソールを使用するかどうか
		bool console = true;

		// ウィンドウモードにするかどうか
		bool window = true;

		// バージョン情報を表示するかどうか
		bool version = false;

		// ヘルプを表示するかどうか
		bool help = false;
	} _option;

	// セットアップ済みかどうか
	bool _setupped = false;

	// コンソールをアタッチしたかどうか
	bool _attached_console = false;

	// コンソールを作成したかどうか
	bool _alloced_console = false;

	// エンジンを初期化したかどうか
	bool _initialized_engine = false;

private:
	// デフォルトファイル名
	static const std::filesystem::path default_filename;

	// デフォルトディレクトリ
	static const std::filesystem::path default_directory;

	// デフォルトアーカイブ
	static const std::filesystem::path default_archive;
};

} // namespace remyroez

#endif // APPLICATION_H_
