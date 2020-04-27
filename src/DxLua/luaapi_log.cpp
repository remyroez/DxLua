#include <tuple>

#include <DxLib.h>
#include <lua.hpp>
#include <sol/sol.hpp>

#include "luaapi.h"

namespace DxLua::detail {

void port_log(sol::state_view &lua, sol::table &t) {

#ifndef DX_NON_LOG

	// ログファイル関数
	DXLUA_PORT(t, LogFileAdd);
	DXLUA_PORT(t, LogFileFmtAdd);
	DXLUA_PORT(t, LogFileTabAdd);
	DXLUA_PORT(t, LogFileTabSub);
	t["SetUseTimeStampFlag"] = [](sol::variadic_args va) {
		int UseFlag = va_get(va, 0, false);
		return SetUseTimeStampFlag(UseFlag);
	};

	// ログ出力設定関数
	t["SetOutApplicationLogValidFlag"] = [](sol::variadic_args va) {
		int Flag = va_get(va, 0, false);
		return SetOutApplicationLogValidFlag(Flag);
	};
	DXLUA_PORT(t, SetApplicationLogFileName);
	DXLUA_PORT(t, SetApplicationLogSaveDirectory);
	t["SetUseDateNameLogFile"] = [](sol::variadic_args va) {
		int Flag = va_get(va, 0, false);
		return SetUseDateNameLogFile(Flag);
	};

#ifndef DX_NON_PRINTF_DX

	// ログ出力機能関数
	t["SetLogDrawOutFlag"] = [](sol::variadic_args va) {
		int DrawFlag = va_get(va, 0, false);
		return SetLogDrawOutFlag(DrawFlag);
	};
	t["GetLogDrawFlag"] = []() {
		return GetLogDrawFlag() ? TRUE : FALSE;
	};
	DXLUA_PORT(t, SetLogFontSize);
	DXLUA_PORT(t, SetLogFontHandle);
	DXLUA_PORT(t, SetLogDrawArea);

	// 簡易画面出力関数
	//extern 	int			printfDx(const TCHAR * FormatString, ...);								// printf と同じ引数で画面に文字列を表示するための関数
	//extern	int			putsDx(const TCHAR * String, int NewLine = TRUE);						// puts と同じ引数で画面に文字列を表示するための関数
	//extern	int			putsDxWithStrLen(const TCHAR * String, size_t StringLength, int NewLine = TRUE);	// puts と同じ引数で画面に文字列を表示するための関数
	//extern	int			clsDx(void);															// printfDx の結果をリセットするための関数

#endif // DX_NON_PRINTF_DX

#endif // DX_NON_LOG

}

} // namespace DxLua::detail
