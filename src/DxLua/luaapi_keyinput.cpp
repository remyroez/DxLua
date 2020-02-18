#include <tuple>

#include <DxLib.h>
#include <lua.hpp>
#include <sol/sol.hpp>

#include "luaapi.h"
#include "DxLua.h"

namespace DxLua::detail {

void port_keyinput(sol::state_view &lua, sol::table &t) {
#ifndef DX_NON_KEYEX

	t["DrawIMEInputString"] = [](int x, int y, int SelectStringNum, sol::variadic_args va) {
		return DrawIMEInputString(x, y, SelectStringNum, va.leftover_count() > 0 ? (va[0].as<bool>() ? TRUE : FALSE) : TRUE);
	};
	t["SetUseIMEFlag"] = [](sol::object UseFlag) {
		return SetUseIMEFlag(UseFlag.as<bool>() ? TRUE : FALSE);
	};
	DXLUA_PORT(t, GetUseIMEFlag);
	t["SetInputStringMaxLengthIMESync"] = [](sol::object Flag) {
		return SetInputStringMaxLengthIMESync(Flag.as<bool>() ? TRUE : FALSE);
	};
	DXLUA_PORT(t, SetIMEInputStringMaxLength);

	t["KeyInputString"] = [t](int x, int y, size_t CharMaxLength, bool CancelValidFlag) {
		auto *buffer = DxLua::require_input_buffer(t.as<sol::object>(), CharMaxLength + 1);
		auto result = KeyInputString(x, y, CharMaxLength, buffer, CancelValidFlag ? TRUE : FALSE);
		return std::make_tuple(result, std::string(buffer));
	};
	t["KeyInputSingleCharString"] = [t](int x, int y, size_t CharMaxLength, bool CancelValidFlag) {
		auto *buffer = DxLua::require_input_buffer(t.as<sol::object>(), CharMaxLength + 1);
		auto result = KeyInputSingleCharString(x, y, CharMaxLength, buffer, CancelValidFlag ? TRUE : FALSE);
		return std::make_tuple(result, std::string(buffer));
	};
	t["KeyInputNumber"] = [t](int x, int y, int MaxNum, int MinNum, bool CancelValidFlag) {
		return KeyInputNumber(x, y, MaxNum, MinNum, CancelValidFlag ? TRUE : FALSE);
	};

	t["InitKeyInput"] = [t]() {
		DxLua::clear_key_inputs(t.as<sol::object>());
		return InitKeyInput();
	};
	t["MakeKeyInput"] = [t](size_t MaxStrLength, bool CancelValidFlag, bool SingleCharOnlyFlag, bool NumCharOnlyFlag, sol::variadic_args va) {
		bool DoubleCharOnlyFlag = va.leftover_count() > 0 ? va[0].as<bool>() : false;
		bool EnableNewLineFlag = va.leftover_count() > 1 ? va[1].as<bool>() : false;
		auto handle = MakeKeyInput(
			MaxStrLength,
			CancelValidFlag ? TRUE : FALSE,
			SingleCharOnlyFlag ? TRUE : FALSE,
			NumCharOnlyFlag ? TRUE : FALSE,
			DoubleCharOnlyFlag ? TRUE : FALSE,
			EnableNewLineFlag ? TRUE : FALSE
		);
		if (handle >= 0) {
			DxLua::register_key_input(
				t.as<sol::object>(),
				handle,
				MaxStrLength,
				CancelValidFlag,
				SingleCharOnlyFlag,
				NumCharOnlyFlag,
				DoubleCharOnlyFlag,
				EnableNewLineFlag
			);
		}
		return handle;
	};
	t["DeleteKeyInput"] = [t](int InputHandle) {
		DxLua::deregister_key_input(t.as<sol::object>(), InputHandle);
		return DeleteKeyInput(InputHandle);
	};
	DXLUA_PORT(t, SetActiveKeyInput);
	DXLUA_PORT(t, GetActiveKeyInput);
	DXLUA_PORT(t, CheckKeyInput);
	DXLUA_PORT(t, ReStartKeyInput);
	DXLUA_PORT(t, ProcessActKeyInput);
	t["DrawKeyInputString"] = [](int x, int y, int InputHandle, sol::variadic_args va) {
		return DrawKeyInputString(x, y, InputHandle, va.leftover_count() > 0 ? (va[0].as<bool>() ? TRUE : FALSE) : TRUE);
	};
	DXLUA_PORT(t, SetKeyInputDrawArea);

	DXLUA_PORT(t, DrawKeyInputModeString);

	DXLUA_PORT(t, SetKeyInputSelectArea);
	t["GetKeyInputSelectArea"] = [](int InputHandle) {
		int SelectStart = 0, SelectEnd = 0;
		auto result = GetKeyInputSelectArea(&SelectStart, &SelectEnd, InputHandle);
		return std::make_tuple(result, SelectStart, SelectEnd);
	};
	DXLUA_PORT(t, SetKeyInputDrawStartPos);
	DXLUA_PORT(t, GetKeyInputDrawStartPos);
	DXLUA_PORT(t, SetKeyInputCursorBrinkTime);
	DXLUA_PORT(t, SetKeyInputCursorBrinkFlag);
	t["SetKeyInputCursorBrinkFlag"] = [](sol::object Flag) {
		return SetKeyInputCursorBrinkFlag(Flag.as<bool>() ? TRUE : FALSE);
	};
	t["SetKeyInputString"] = [t](std::string String, int InputHandle) {
		int result = -1;
		auto size = DxLua::get_key_input_size(t.as<sol::object>(), InputHandle);
		if (size > 0) {
			String.resize(size);
			String.shrink_to_fit();
			result = SetKeyInputString(String.data(), InputHandle);
		}
		return result;
	};
	DXLUA_PORT(t, SetKeyInputNumber);
	DXLUA_PORT(t, SetKeyInputNumberToFloat);
	t["GetKeyInputString"] = [t](int InputHandle) {
		int result = -1;
		auto size = DxLua::get_key_input_size(t.as<sol::object>(), InputHandle);
		std::string buffer;
		if (size > 0) {
			buffer.resize(size + 1);
			result = GetKeyInputString(buffer.data(), InputHandle);
			buffer.shrink_to_fit();
		}
		return std::make_tuple(result, buffer);
	};
	DXLUA_PORT(t, GetKeyInputNumber);
	DXLUA_PORT(t, GetKeyInputNumberToFloat);
	DXLUA_PORT(t, SetKeyInputCursorPosition);
	DXLUA_PORT(t, GetKeyInputCursorPosition);
#if 0
	int			InputStringToCustom(int x, int y, size_t BufLength, TCHAR * StrBuffer, int CancelValidFlag, int SingleCharOnlyFlag, int NumCharOnlyFlag, int DoubleCharOnlyFlag = FALSE, int EnableNewLineFlag = FALSE, int DisplayCandidateList = TRUE);		// 文字列の入力取得

	int			GetIMEInputModeStr(TCHAR * GetBuffer);																											// IMEの入力モード文字列を取得する
	const IMEINPUTDATA *GetIMEInputData(void);																														// IMEで入力中の文字列の情報を取得する
	int			SetKeyInputStringColor(ULONGLONG NmlStr, ULONGLONG NmlCur, ULONGLONG IMEStrBack, ULONGLONG IMECur, ULONGLONG IMELine, ULONGLONG IMESelectStr, ULONGLONG IMEModeStr, ULONGLONG NmlStrE = 0, ULONGLONG IMESelectStrE = 0, ULONGLONG IMEModeStrE = 0, ULONGLONG IMESelectWinE = ULL_PARAM(0xffffffffffffffff), ULONGLONG IMESelectWinF = ULL_PARAM(0xffffffffffffffff), ULONGLONG SelectStrBackColor = ULL_PARAM(0xffffffffffffffff), ULONGLONG SelectStrColor = ULL_PARAM(0xffffffffffffffff), ULONGLONG SelectStrEdgeColor = ULL_PARAM(0xffffffffffffffff), ULONGLONG IMEStr = ULL_PARAM(0xffffffffffffffff), ULONGLONG IMEStrE = ULL_PARAM(0xffffffffffffffff));	// ( SetKeyInputStringColor2 の旧関数 )InputString関数使用時の文字の各色を変更する
	int			SetKeyInputStringColor2(int TargetColor /* DX_KEYINPSTRCOLOR_NORMAL_STR 等 */, unsigned int Color);													// InputString関数使用時の文字の各色を変更する
	int			ResetKeyInputStringColor2(int TargetColor /* DX_KEYINPSTRCOLOR_NORMAL_STR 等 */);																		// SetKeyInputStringColor2 で設定した色をデフォルトに戻す
	int			SetKeyInputStringFont(int FontHandle);																												// キー入力文字列描画関連で使用するフォントのハンドルを変更する(-1でデフォルトのフォントハンドル)
	int			SetKeyInputStringEndCharaMode(int EndCharaMode /* DX_KEYINPSTR_ENDCHARAMODE_OVERWRITE 等 */);																// キー入力文字列処理の入力文字数が限界に達している状態で、文字列の末端部分で入力が行われた場合の処理モードを変更する
#endif
#endif // DX_NON_KEYEX
}

} // namespace DxLua::detail
