#include <tuple>

#include <DxLib.h>
#include <lua.hpp>
#include <sol/sol.hpp>

#include "luaapi.h"

namespace DxLua::detail {

void port_inputstring(sol::state_view &lua, sol::table &t) {

#ifndef DX_NON_INPUTSTRING

	// 文字コードバッファ操作関係
	DXLUA_PORT(t, StockInputChar);
	DXLUA_PORT(t, ClearInputCharBuf);
	t["GetInputChar"] = [](sol::object DeleteFlag) {
		return (unsigned int)GetInputChar(DeleteFlag.as<bool>() ? TRUE : FALSE);
	};
	t["GetInputCharWait"] = [](sol::object DeleteFlag) {
		return (unsigned int)GetInputCharWait(DeleteFlag.as<bool>() ? TRUE : FALSE);
	};

	t["GetOneChar"] = [](sol::variadic_args va) {
		TCHAR CharBuffer = '\0';
		int DeleteFlag = va_get(va, 0, false);
		int Result = GetOneChar(&CharBuffer, DeleteFlag);
		return std::tuple(Result, (unsigned int)CharBuffer);
	};
	t["GetOneCharWait"] = [](sol::variadic_args va) {
		TCHAR CharBuffer = '\0';
		int DeleteFlag = va_get(va, 0, false);
		int Result = GetOneCharWait(&CharBuffer, DeleteFlag);
		return std::tuple(Result, (unsigned int)CharBuffer);
	};
	DXLUA_PORT(t, GetCtrlCodeCmp);

#endif // DX_NON_INPUTSTRING

#ifndef DX_NON_KEYEX

	//extern	int			DrawIMEInputString(int x, int y, int SelectStringNum, int DrawCandidateList = TRUE);	// 画面上に入力中の文字列を描画する
	//extern	int			SetUseIMEFlag(int UseFlag);							// ＩＭＥを使用するかどうかを設定する
	//extern	int			GetUseIMEFlag(void);								// ＩＭＥを使用するかどうかの設定を取得する
	//extern	int			SetInputStringMaxLengthIMESync(int Flag);							// ＩＭＥで入力できる最大文字数を MakeKeyInput の設定に合わせるかどうかをセットする( TRUE:あわせる  FALSE:あわせない(デフォルト) )
	//extern	int			SetIMEInputStringMaxLength(int Length);							// ＩＭＥで一度に入力できる最大文字数を設定する( 0:制限なし  1以上:指定の文字数で制限 )

#endif // DX_NON_KEYEX

	//extern	int			GetStringPoint(const TCHAR * String, int Point);		// 全角文字、半角文字入り乱れる中から指定の文字数での半角文字数を得る
	//extern	int			GetStringPointWithStrLen(const TCHAR * String, size_t StringLength, int Point);		// 全角文字、半角文字入り乱れる中から指定の文字数での半角文字数を得る
	//extern	int			GetStringPoint2(const TCHAR * String, int Point);		// 全角文字、半角文字入り乱れる中から指定の半角文字数での文字数を得る
	//extern	int			GetStringPoint2WithStrLen(const TCHAR * String, size_t StringLength, int Point);		// 全角文字、半角文字入り乱れる中から指定の半角文字数での文字数を得る
	//extern	int			GetStringLength(const TCHAR * String);										// 全角文字、半角文字入り乱れる中から文字数を取得する

#ifndef DX_NON_FONT
	//extern	int			DrawObtainsString(int x, int y, int AddY, const TCHAR * String, unsigned int StrColor, unsigned int StrEdgeColor = 0, int FontHandle = -1, unsigned int SelectBackColor = 0xffffffff, unsigned int SelectStrColor = 0, unsigned int SelectStrEdgeColor = 0xffffffff, int SelectStart = -1, int SelectEnd = -1);		// 描画可能領域に収まるように改行しながら文字列を描画
	//extern	int			DrawObtainsNString(int x, int y, int AddY, const TCHAR * String, size_t StringLength, unsigned int StrColor, unsigned int StrEdgeColor = 0, int FontHandle = -1, unsigned int SelectBackColor = 0xffffffff, unsigned int SelectStrColor = 0, unsigned int SelectStrEdgeColor = 0xffffffff, int SelectStart = -1, int SelectEnd = -1);		// 描画可能領域に収まるように改行しながら文字列を描画
	//extern	int			DrawObtainsString_CharClip(int x, int y, int AddY, const TCHAR * String, unsigned int StrColor, unsigned int StrEdgeColor = 0, int FontHandle = -1, unsigned int SelectBackColor = 0xffffffff, unsigned int SelectStrColor = 0, unsigned int SelectStrEdgeColor = 0xffffffff, int SelectStart = -1, int SelectEnd = -1);		// 描画可能領域に収まるように改行しながら文字列を描画( クリップが文字単位 )
	//extern	int			DrawObtainsNString_CharClip(int x, int y, int AddY, const TCHAR * String, size_t StringLength, unsigned int StrColor, unsigned int StrEdgeColor = 0, int FontHandle = -1, unsigned int SelectBackColor = 0xffffffff, unsigned int SelectStrColor = 0, unsigned int SelectStrEdgeColor = 0xffffffff, int SelectStart = -1, int SelectEnd = -1);		// 描画可能領域に収まるように改行しながら文字列を描画( クリップが文字単位 )
	//extern	int			GetObtainsStringCharPosition(int x, int y, int AddY, const TCHAR * String, int StrLen, int *PosX, int *PosY, int FontHandle = -1);		// 描画可能領域に収まるように改行しながら文字列を描画した場合の文字列の末端の座標を取得する
	//extern	int			GetObtainsStringCharPosition_CharClip(int x, int y, int AddY, const TCHAR * String, int StrLen, int *PosX, int *PosY, int FontHandle = -1);		// 描画可能領域に収まるように改行しながら文字列を描画した場合の文字列の末端の座標を取得する( クリップが文字単位 )
#endif // DX_NON_FONT
	//extern	int			DrawObtainsBox(int x1, int y1, int x2, int y2, int AddY, unsigned int Color, int FillFlag);																																										// 描画可能領域に収まるように補正を加えながら矩形を描画

#ifndef DX_NON_KEYEX

	//extern	int			InputStringToCustom(int x, int y, size_t BufLength, TCHAR * StrBuffer, int CancelValidFlag, int SingleCharOnlyFlag, int NumCharOnlyFlag, int DoubleCharOnlyFlag = FALSE, int EnableNewLineFlag = FALSE, int DisplayCandidateList = TRUE);		// 文字列の入力取得

	//extern	int			KeyInputString(int x, int y, size_t CharMaxLength, TCHAR * StrBuffer, int CancelValidFlag);													// 文字列の入力取得
	//extern	int			KeyInputSingleCharString(int x, int y, size_t CharMaxLength, TCHAR * StrBuffer, int CancelValidFlag);													// 半角文字列のみの入力取得
	//extern	int			KeyInputNumber(int x, int y, int MaxNum, int MinNum, int CancelValidFlag);																	// 数値の入力取得

	//extern	int			GetIMEInputModeStr(TCHAR * GetBuffer);																											// IMEの入力モード文字列を取得する
	//extern	const IMEINPUTDATA *GetIMEInputData(void);																														// IMEで入力中の文字列の情報を取得する
	//extern	int			SetKeyInputStringColor(ULONGLONG NmlStr, ULONGLONG NmlCur, ULONGLONG IMEStrBack, ULONGLONG IMECur, ULONGLONG IMELine, ULONGLONG IMESelectStr, ULONGLONG IMEModeStr, ULONGLONG NmlStrE = 0, ULONGLONG IMESelectStrE = 0, ULONGLONG IMEModeStrE = 0, ULONGLONG IMESelectWinE = ULL_PARAM(0xffffffffffffffff), ULONGLONG IMESelectWinF = ULL_PARAM(0xffffffffffffffff), ULONGLONG SelectStrBackColor = ULL_PARAM(0xffffffffffffffff), ULONGLONG SelectStrColor = ULL_PARAM(0xffffffffffffffff), ULONGLONG SelectStrEdgeColor = ULL_PARAM(0xffffffffffffffff), ULONGLONG IMEStr = ULL_PARAM(0xffffffffffffffff), ULONGLONG IMEStrE = ULL_PARAM(0xffffffffffffffff));	// ( SetKeyInputStringColor2 の旧関数 )InputString関数使用時の文字の各色を変更する
	//extern	int			SetKeyInputStringColor2(int TargetColor /* DX_KEYINPSTRCOLOR_NORMAL_STR 等 */, unsigned int Color);													// InputString関数使用時の文字の各色を変更する
	//extern	int			ResetKeyInputStringColor2(int TargetColor /* DX_KEYINPSTRCOLOR_NORMAL_STR 等 */);																		// SetKeyInputStringColor2 で設定した色をデフォルトに戻す
	//extern	int			SetKeyInputStringFont(int FontHandle);																												// キー入力文字列描画関連で使用するフォントのハンドルを変更する(-1でデフォルトのフォントハンドル)
	//extern	int			SetKeyInputStringEndCharaMode(int EndCharaMode /* DX_KEYINPSTR_ENDCHARAMODE_OVERWRITE 等 */);																// キー入力文字列処理の入力文字数が限界に達している状態で、文字列の末端部分で入力が行われた場合の処理モードを変更する
	//extern	int			DrawKeyInputModeString(int x, int y);																												// 入力モード文字列を描画する

	//extern	int			InitKeyInput(void);																														// キー入力データ初期化
	//extern	int			MakeKeyInput(size_t MaxStrLength, int CancelValidFlag, int SingleCharOnlyFlag, int NumCharOnlyFlag, int DoubleCharOnlyFlag = FALSE, int EnableNewLineFlag = FALSE);			// 新しいキー入力ハンドルの作成
	//extern	int			DeleteKeyInput(int InputHandle);																												// キー入力ハンドルの削除
	//extern	int			SetActiveKeyInput(int InputHandle);																												// 指定のキー入力ハンドルをアクティブにする( -1 を指定するとアクティブなキー入力ハンドルが無い状態になります )
	//extern	int			GetActiveKeyInput(void);																														// 現在アクティブになっているキー入力ハンドルを取得する
	//extern	int			CheckKeyInput(int InputHandle);																												// キー入力ハンドルの入力が終了しているか取得する
	//extern	int			ReStartKeyInput(int InputHandle);																												// 入力が完了したキー入力ハンドルを再度編集状態に戻す
	//extern	int			ProcessActKeyInput(void);																														// キー入力ハンドル処理関数
	//extern	int			DrawKeyInputString(int x, int y, int InputHandle, int DrawCandidateList = TRUE);																// キー入力ハンドルの入力中情報の描画
	//extern	int			SetKeyInputDrawArea(int x1, int y1, int x2, int y2, int InputHandle);																				// キー入力ハンドルの入力中文字列を描画する際の描画範囲を設定する

	//extern	int			SetKeyInputSelectArea(int  SelectStart, int  SelectEnd, int InputHandle);																			// キー入力ハンドルの指定の領域を選択状態にする( SelectStart と SelectEnd に -1 を指定すると選択状態が解除されます )
	//extern	int			GetKeyInputSelectArea(int *SelectStart, int *SelectEnd, int InputHandle);																			// キー入力ハンドルの選択領域を取得する
	//extern	int			SetKeyInputDrawStartPos(int DrawStartPos, int InputHandle);																							// キー入力ハンドルの描画開始文字位置を設定する
	//extern	int			GetKeyInputDrawStartPos(int InputHandle);																												// キー入力ハンドルの描画開始文字位置を取得する
	//extern	int			SetKeyInputCursorBrinkTime(int Time);																													// キー入力ハンドルのキー入力時のカーソルの点滅する早さをセットする
	//extern	int			SetKeyInputCursorBrinkFlag(int Flag);																													// キー入力ハンドルのキー入力時のカーソルを点滅させるかどうかをセットする
	//extern	int			SetKeyInputString(const TCHAR * String, int InputHandle);																	// キー入力ハンドルに指定の文字列をセットする
	//extern	int			SetKeyInputStringWithStrLen(const TCHAR * String, size_t StringLength, int InputHandle);																	// キー入力ハンドルに指定の文字列をセットする
	//extern	int			SetKeyInputNumber(int   Number, int InputHandle);																	// キー入力ハンドルに指定の数値を文字に置き換えてセットする
	//extern	int			SetKeyInputNumberToFloat(float Number, int InputHandle);																	// キー入力ハンドルに指定の浮動小数点値を文字に置き換えてセットする
	//extern	int			GetKeyInputString(TCHAR * StrBuffer, int InputHandle);																	// キー入力ハンドルの入力中の文字列を取得する
	//extern	int			GetKeyInputNumber(int InputHandle);																												// キー入力ハンドルの入力中の文字列を整数値として取得する
	//extern	float		GetKeyInputNumberToFloat(int InputHandle);																												// キー入力ハンドルの入力中の文字列を浮動小数点値として取得する
	//extern	int			SetKeyInputCursorPosition(int Position, int InputHandle);																						// キー入力ハンドルの現在のカーソル位置を設定する
	//extern	int			GetKeyInputCursorPosition(int InputHandle);																												// キー入力ハンドルの現在のカーソル位置を取得する

#endif // DX_NON_KEYEX

}

} // namespace DxLua::detail
