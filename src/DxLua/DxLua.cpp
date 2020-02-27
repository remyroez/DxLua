﻿#include <DxLib.h>
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
sol::table solopen_dxlua(sol::this_state s) {
	// ステート
	sol::state_view lua(s);

	// テーブル
	sol::table library = lua.create_table();

	// 実行関数
	{
		// ハードコーディング
		// TODO: バイナリ化
		auto result = lua.safe_script(R"lua( return function (watch, interval, ...)
    -- 終了関数
    local QuitCode = false
    function DxLua.Quit(code)
        QuitCode = code or 'exit'
    end

    -- ループ
    local BeforeTime = DxLua.GetNowHiPerformanceCount()
    local Watchable = watch ~= 'none';
    local WatchTimer = Watchable and (interval / 1000) or -1

    while (DxLua.ProcessMessage() == 0) do
        -- 経過時間の計測
        local NowTime = DxLua.GetNowHiPerformanceCount()
        local DeltaTime = (NowTime - BeforeTime) / 1000000
        BeforeTime = NowTime;

        -- ファイルの監視
        if WatchTimer >= 0 then
            WatchTimer = WatchTimer - DeltaTime
            if WatchTimer <= 0 then
                WatchTimer = interval / 1000
                if DxLua.__context__:watch() then
                    return watch
                end
            end
        end

        -- 更新
		if DxLua.Update then
			local result = DxLua.Update(DeltaTime) or QuitCode
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

	// 以下、ポーティング

	DXLUA_PORT(library, ProcessMessage);

	DXLUA_PORT(library, WaitTimer);
	DXLUA_PORT(library, WaitKey);

	library["GetNowCount"] = [](sol::variadic_args va) {
		return GetNowCount(va.leftover_count() > 0 ? (va[0].as<bool>() ? TRUE : FALSE) : FALSE);
	};
	library["GetNowHiPerformanceCount"] = [](sol::variadic_args va) {
		return GetNowHiPerformanceCount(va.leftover_count() > 0 ? (va[0].as<bool>() ? TRUE : FALSE) : FALSE);
	};

	DXLUA_PORT(library, GetRand);
	DXLUA_PORT(library, SRand);

	library["SetAlwaysRunFlag"] = [](sol::object Flag) {
		return SetAlwaysRunFlag(Flag.as<bool>() ? TRUE : FALSE);
	};

	library["SetDoubleStartValidFlag"] = [](sol::object Flag) {
		return SetDoubleStartValidFlag(Flag.as<bool>() ? TRUE : FALSE);
	};

	library["SetWaitVSyncFlag"] = [](sol::object Flag) {
		return SetWaitVSyncFlag(Flag.as<bool>() ? TRUE : FALSE);
	};

	//DXLUA_PORT(library, SetGraphMode);
	library["SetGraphMode"] = [](int ScreenSizeX, int ScreenSizeY, int ColorBitDepth, sol::variadic_args va) {
		return SetGraphMode(ScreenSizeX, ScreenSizeY, ColorBitDepth, va.leftover_count() > 0 ? va[0].as<int>() : 60);
	};

	DXLUA_PORT(library, ScreenFlip);
	library["ClearDrawScreen"] = [](sol::variadic_args va) {
		return ClearDrawScreen();
	};
	DXLUA_PORT(library, SetDrawScreen);
	DXLUA_PORT(library, SetDrawArea);
#if 0
	library["GetDrawArea"] = [lua]() {
		RECT rect;
		GetDrawArea(&rect);
		return rect;
	};
#endif
	DXLUA_PORT(library, SetDrawAreaFull);
	DXLUA_PORT(library, SetDraw3DScale);

	library["LoadGraph"] = [library](const TCHAR *FileName, sol::variadic_args va) {
		return LoadGraph(to_data_path(library, FileName).c_str(), va.leftover_count() > 0 ? va[0].as<bool>() : false);
	};
	library["DrawGraph"] = [](float xf, float yf, int GrHandle, bool TransFlag) {
		return DrawGraph(xf, yf, GrHandle, TransFlag ? TRUE : FALSE);
	};
	library["DrawRotaGraph"] = [](int x, int y, double ExRate, double Angle, int GrHandle, bool TransFlag, sol::variadic_args va) {
		int ReverseXFlag = va.leftover_count() > 0 ? (va[0].as<bool>() ? TRUE : FALSE) : FALSE;
		int ReverseYFlag = va.leftover_count() > 1 ? (va[1].as<bool>() ? TRUE : FALSE) : FALSE;
		return DrawRotaGraph(x, y, ExRate, Angle, GrHandle, TransFlag ? TRUE : FALSE, ReverseXFlag, ReverseYFlag);
	};
	library["DrawString"] = [](int x, int y, const TCHAR *String, sol::variadic_args va) {
		unsigned int Color = va.leftover_count() > 0 ? va[0].as<unsigned int>() : 0xFFFFFFFF;
		unsigned int EdgeColor = va.leftover_count() > 1 ? va[1].as<unsigned int>() : 0;
		return DrawString(x, y, String, Color, EdgeColor);
	};

	library["DrawLine"] = [](int x1, int y1, int x2, int y2, sol::variadic_args va) {
		unsigned int Color = va.leftover_count() > 0 ? va[0].as<unsigned int>() : 0xFFFFFFFF;
		int Thickness = va.leftover_count() > 1 ? va[1].as<int>() : 1;
		return DrawLine(x1, y1, x2, y2, Color, va.leftover_count() > 0 ? va[0].as<int>() : 1);
	};
	library["DrawBox"] = [](int x1, int y1, int x2, int y2, sol::variadic_args va) {
		unsigned int Color = va.leftover_count() > 0 ? va[0].as<unsigned int>() : 0xFFFFFFFF;
		int FillFlag = va.leftover_count() > 1 ? (va[1].as<bool>() ? TRUE : FALSE) : TRUE;
		return DrawBox(x1, y1, x2, y2, Color, FillFlag);
	};
	library["DrawCircle"] = [](int x, int y, int r, sol::variadic_args va) {
		unsigned int Color = va.leftover_count() > 0 ? va[0].as<unsigned int>() : 0xFFFFFFFF;
		int FillFlag = va.leftover_count() > 1 ? (va[1].as<bool>() ? TRUE : FALSE) : TRUE;
		int LineThickness = va.leftover_count() > 2 ? va[2].as<int>() : 1;
		return DrawCircle(x, y, r, Color, FillFlag, LineThickness);
	};
	library["DrawTriangle"] = [](int x1, int y1, int x2, int y2, int x3, int y3, sol::variadic_args va) {
		unsigned int Color = va.leftover_count() > 0 ? va[0].as<unsigned int>() : 0xFFFFFFFF;
		int FillFlag = va.leftover_count() > 1 ? (va[1].as<bool>() ? TRUE : FALSE) : TRUE;
		return DrawTriangle(x1, y1, x2, y2, x3, y3, Color, FillFlag);
	};
	DXLUA_PORT(library, DrawPixel);

	DXLUA_PORT(library, SetDrawBlendMode);
	library["GetDrawBlendMode"] = []() {
		int BlendMode = -1;
		int BlendParam = -1;
		auto result = GetDrawBlendMode(&BlendMode, &BlendParam);
		return std::make_tuple(result, BlendMode, BlendParam);
	};

	DXLUA_PORT(library, GetColor);

	DXLUA_PORT(library, SetFontSize);
	DXLUA_PORT(library, SetDrawBright);

	DXLUA_PORT(library, ChangeFontType);
	
	library["GetCharBytes"] = [](int CharCodeFormat, const char *String) {
		return GetCharBytes(CharCodeFormat, String);
	};

	DXLUA_PORT(library, CheckHitKey);
	library["CheckHitKeyAll"] = [](sol::variadic_args va) {
		return CheckHitKeyAll(va.leftover_count() > 0 ? va[0].as<int>() : DX_CHECKINPUT_ALL);
	};
	DXLUA_PORT(library, GetJoypadInputState);
	DXLUA_PORT(library, GetMouseInput);

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
