#include <DxLib.h>
#include <lua.hpp>
#include <sol/sol.hpp>

#include "DxLua.h"
#include "luaapi.h"
#include "context.h"

namespace DxLua {

// 監視対象の追加
void add_watchee(sol::object &library, const std::filesystem::path &watchee) {
	DXLUA_CONTEXT(library).add_watchee(watchee);
}

// 監視対象の追加
void add_watchee(sol::object &library, std::filesystem::path &&watchee) {
	DXLUA_CONTEXT(library).add_watchee(watchee);
}

// 監視対象のクリア
void clear_watchee(sol::object &library) {
	DXLUA_CONTEXT(library).clear_watchee();
}

// ベースパスの設定
void set_base_path(sol::object &library, const std::filesystem::path &path) {
	DXLUA_CONTEXT(library).set_base_path(path);
}

// ベースパスを連結して返す
std::filesystem::path append_base_path(const sol::object &library, const std::filesystem::path &path) {
	return DXLUA_CONTEXT(library).append_base_path(path);
}

// ベースパスのクリア
void clear_base_path(sol::object &library) {
	DXLUA_CONTEXT(library).clear_base_path();
}

// 監視
bool watch(sol::object &library) {
	return DXLUA_CONTEXT(library).watch();
}

// 入力バッファを要求サイズ分確保して返す
char *require_input_buffer(sol::object &library, size_t size) {
	return DXLUA_CONTEXT(library).require_input_buffer(size);
}

// キー入力情報の登録
void register_key_input(sol::object &library, int handle, size_t size, bool cancelable, bool ascii_only, bool number_only, bool zenkaku_only, bool enable_new_line) {
	return DXLUA_CONTEXT(library).register_key_input(handle, size, cancelable, ascii_only, number_only, zenkaku_only, enable_new_line);
}

// キー入力情報の登録解除
size_t get_key_input_size(sol::object &library, int handle) {
	return DXLUA_CONTEXT(library).get_key_input_size(handle);
}

// キー入力情報の登録解除
void deregister_key_input(sol::object &library, int handle) {
	DXLUA_CONTEXT(library).deregister_key_input(handle);
}

// すべてのキー入力情報の登録解除
void clear_key_inputs(sol::object &library) {
	DXLUA_CONTEXT(library).clear_key_inputs();
}

// DxLua ライブラリの展開
sol::table open_library(sol::this_state s) {
	// ステート
	sol::state_view lua(s);

	// テーブル
	sol::table library = lua.create_table();

	// 実行関数
	{
		// ハードコーディング
		// TODO: バイナリ化
		auto result = lua.safe_script(R"lua( return function (watch, interval, ...)
	local dx = require 'dx'

    -- 終了関数
    local QuitCode = false
    function dx.Quit(code)
        QuitCode = code or 'exit'
    end

    -- ループ
    local BeforeTime = dx.GetNowHiPerformanceCount()
    local Watchable = watch ~= 'none';
    local WatchTimer = Watchable and (interval / 1000) or -1

    while (dx.ProcessMessage() == 0) do
        -- 経過時間の計測
        local NowTime = dx.GetNowHiPerformanceCount()
        local DeltaTime = (NowTime - BeforeTime) / 1000000
        BeforeTime = NowTime;

        -- ファイルの監視
        if WatchTimer >= 0 then
            WatchTimer = WatchTimer - DeltaTime
            if WatchTimer <= 0 then
                WatchTimer = interval / 1000
                if dx.__context__:watch() then
                    return watch
                end
            end
        end

        -- 更新
		if dx.Update then
			local result = dx.Update(DeltaTime) or QuitCode
			if result then
				return result -- エラーが起きたら直ちに終了
			end
        end
    end

    return 'exit'
end)lua"
);
		if (!result.valid()) {
			// チャンク作成失敗
			sol::error err = result;
			std::cerr << err.what() << std::endl;

		} else {
			// チャンク作成成功
			if (sol::object fn = result.get<sol::function>(); fn.is<sol::function>()) {
				library["Run"] = fn;
			}
		}
	}

	// コンテキストのインスタンス化
	detail::context::set(library);

	// 小分けにした各ポート関数を呼ぶ
	detail::port_define(lua, library);
	detail::port_type(lua, library);
	detail::port_network(lua, library);
	detail::port_keyinput(lua, library);
	detail::port_input(lua, library);
	detail::port_draw(lua, library);

	// 以下、ポーティング

	DXLUA_PORT(library, ProcessMessage);

	DXLUA_PORT(library, WaitTimer);

#ifndef DX_NON_INPUT
	DXLUA_PORT(library, WaitKey);
#endif // DX_NON_INPUT

	library["GetNowCount"] = [](sol::variadic_args va) {
		return GetNowCount(va.leftover_count() > 0 ? (va[0].as<bool>() ? TRUE : FALSE) : FALSE);
	};
	library["GetNowHiPerformanceCount"] = [](sol::variadic_args va) {
		return GetNowHiPerformanceCount(va.leftover_count() > 0 ? (va[0].as<bool>() ? TRUE : FALSE) : FALSE);
	};

	DXLUA_PORT(library, GetRand);
	DXLUA_PORT(library, SRand);

	DXLUA_PORT(library, VGet);
	DXLUA_PORT(library, VAdd);
	DXLUA_PORT(library, VSub);
	DXLUA_PORT(library, VDot);
	DXLUA_PORT(library, VCross);
	DXLUA_PORT(library, VScale);
	DXLUA_PORT(library, VNorm);
	DXLUA_PORT(library, VSize);
	DXLUA_PORT(library, VSquareSize);
	DXLUA_PORT(library, VCos);
	DXLUA_PORT(library, VRad);

	DXLUA_PORT(library, MV1LoadModel);
	DXLUA_PORT(library, MV1SetPosition);
	DXLUA_PORT(library, MV1DrawFrame);

	DXLUA_PORT(library, SetCameraPositionAndTarget_UpVecY);

	library["SetAlwaysRunFlag"] = [](sol::object Flag) {
		return SetAlwaysRunFlag(Flag.as<bool>() ? TRUE : FALSE);
	};

	library["SetDoubleStartValidFlag"] = [](sol::object Flag) {
		return SetDoubleStartValidFlag(Flag.as<bool>() ? TRUE : FALSE);
	};

	DXLUA_PORT(library, SetFontSize);
	DXLUA_PORT(library, SetDrawBright);

	DXLUA_PORT(library, ChangeFontType);

	library["DrawString"] = [](int x, int y, const TCHAR *String, sol::variadic_args va) {
		unsigned int Color = va.leftover_count() > 0 ? va[0].as<unsigned int>() : 0xFFFFFFFF;
		unsigned int EdgeColor = va.leftover_count() > 1 ? va[1].as<unsigned int>() : 0;
		return DrawString(x, y, String, Color, EdgeColor);
	};
	library["DrawFormatString"] = [lua, library](int x, int y, unsigned int Color, const TCHAR *String, sol::variadic_args va) {
		int Result = -1;
		if (sol::function fn = lua["string"]["format"]; fn.valid()) {
			if (sol::object result = fn.call(String, va); result.valid()) {
				Result = DrawFormatString(x, y, Color, result.as<const char *>());
			}
		}
		return Result;
	};
	DXLUA_PORT(library, GetColor);

	library["GetCharBytes"] = [](int CharCodeFormat, const char *String) {
		return GetCharBytes(CharCodeFormat, String);
	};

	library["GetInputChar"] = [](sol::object DeleteFlag) {
		return (unsigned int)GetInputChar(DeleteFlag.as<bool>() ? TRUE : FALSE);
	};
	library["GetInputCharWait"] = [](sol::object DeleteFlag) {
		return (unsigned int)GetInputCharWait(DeleteFlag.as<bool>() ? TRUE : FALSE);
	};

	library["Buffer_getInt32"] = [](sol::table table, sol::variadic_args va) {
		int result = 0;
		ptrdiff_t index = va.leftover_count() > 0 ? va[0].as<ptrdiff_t>() : 1;
		if (index < 1) {
			// インデックスが１未満

		} else if (table.size() < (index + sizeof(result) - 1)) {
			// テーブルのサイズがインデックスから４未満

		} else {
			result |= table.get_or(index + 0, 0) & UCHAR_MAX;
			result |= (table.get_or(index + 1, 0) & UCHAR_MAX) << 8;
			result |= (table.get_or(index + 2, 0) & UCHAR_MAX) << 16;
			result |= (table.get_or(index + 3, 0) & UCHAR_MAX) << 24;
		}

		return result;
	};

	library["Buffer_getString"] = [](sol::table table, sol::variadic_args va) {
		std::string result = "";
		ptrdiff_t index = va.leftover_count() > 0 ? va[0].as<ptrdiff_t>() : 1;

		if (index < 1) {
			// インデックスが１未満

		} else if (table.size() < index) {
			// テーブルのサイズがインデックス初期値未満

		} else {
			for (auto i = index; i <= table.size(); ++i) {
				result.push_back(static_cast<char>(table.get_or(i, 0)));
			}
		}

		return std::string(result.c_str());
	};

	return library;
}

} // namespace DxLua
