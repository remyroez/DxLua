#include <tuple>

#include <DxLib.h>
#include <lua.hpp>
#include <sol/sol.hpp>

#include "luaapi.h"

namespace DxLua::detail {

void port_system(sol::state_view &lua, sol::table &t) {

	// 初期化終了系関数
	//extern	int			DxLib_Init(void);													// ライブラリ初期化を行う
	//extern	int			DxLib_End(void);														// ライブラリ使用の後始末を行う

	//extern	int			DxLib_GlobalStructInitialize(void);									// ライブラリの内部で使用している構造体をゼロ初期化して、DxLib_Init の前に行った設定を無効化する( DxLib_Init の前でのみ有効 )
	//extern	int			DxLib_IsInit(void);													// ライブラリが初期化されているかどうかを取得する( 戻り値: TRUE=初期化されている  FALSE=されていない )

	// エラー関係関数
	DXLUA_PORT(t, GetLastErrorCode);
	t["GetLastErrorMessage"] = [](sol::variadic_args va) {
		int StringBufferBytes = va_get(va, 0, 0);
		static std::vector<char> StringBuffer;
		if (StringBufferBytes <= 0) {
		} else {
			StringBuffer.reserve(StringBufferBytes);
			if (StringBuffer.size() < StringBufferBytes) {
				StringBuffer.resize(StringBufferBytes);
			}
		}
		int Result = GetLastErrorMessage(StringBuffer.data(), StringBuffer.size());
		return std::tuple(Result, StringBuffer.data());
	};

	// メッセージ処理関数
	DXLUA_PORT(t, ProcessMessage);

	// 設定系関数
	t["SetAlwaysRunFlag"] = [](sol::object Flag) {
		return SetAlwaysRunFlag(Flag.as<bool>() ? TRUE : FALSE);
	};

	// ウエイト系関数
	DXLUA_PORT(t, WaitTimer);

#ifndef DX_NON_INPUT
	DXLUA_PORT(t, WaitKey);
#endif // DX_NON_INPUT

	// カウンタ及び時刻取得系関数
	t["GetNowCount"] = [](sol::variadic_args va) {
		return GetNowCount(va_get(va, 0, false));
	};
	t["GetNowHiPerformanceCount"] = [](sol::variadic_args va) {
		return GetNowHiPerformanceCount(va_get(va, 0, false));
	};
	DXLUA_PORT(t, GetNowSysPerformanceCount);
	DXLUA_PORT(t, GetSysPerformanceFrequency);
	DXLUA_PORT(t, ConvSysPerformanceCountToSeconds);
	DXLUA_PORT(t, ConvSysPerformanceCountToMilliSeconds);
	DXLUA_PORT(t, ConvSysPerformanceCountToMicroSeconds);
	DXLUA_PORT(t, ConvSysPerformanceCountToNanoSeconds);
	DXLUA_PORT(t, ConvSecondsToSysPerformanceCount);
	DXLUA_PORT(t, ConvMilliSecondsToSysPerformanceCount);
	DXLUA_PORT(t, ConvMicroSecondsToSysPerformanceCount);
	DXLUA_PORT(t, ConvNanoSecondsToSysPerformanceCount);
	t["GetDateTime"] = [](sol::variadic_args va) {
		static DATEDATA DateBuf;
		int Result = GetDateTime(&DateBuf);
		return DateBuf;
	};

	// 乱数取得
	DXLUA_PORT(t, GetRand);
	DXLUA_PORT(t, SRand);

	// バッテリー関連
	DXLUA_PORT(t, GetBatteryLifePercent);

	// クリップボード関係
	t["GetClipboardText"] = []() {
		static std::vector<char> DestBuffer(1);
		auto size = GetClipboardText(nullptr);
		if (size > 0) {
			DestBuffer.reserve(size);
			if (DestBuffer.size() < size) {
				DestBuffer.resize(size);
			}
			GetClipboardText(DestBuffer.data());
			DestBuffer[DestBuffer.size() - 1] = '\0';

		} else {
			DestBuffer[0] = '\0';
		}
		return DestBuffer.data();
	};
	t["SetClipboardText"] = [](sol::optional<std::string> maybe_Text) {
		int Result = FALSE;
		if (maybe_Text) {
			auto &Text = maybe_Text.value();
			Result = SetClipboardText(Text.c_str());
		}
		return Result;
	};

	// iniファイル関係
	t["GetPrivateProfileStringDx"] = [](const TCHAR *AppName, const TCHAR *KeyName, const TCHAR *Default, size_t ReturnedStringBufferBytes, const TCHAR *IniFilePath, sol::variadic_args va) {
		int IniFileCharCodeFormat = va_get(va, 0, -1);
		static std::vector<char> ReturnedStringBuffer;
		ReturnedStringBuffer.reserve(ReturnedStringBufferBytes);
		if (ReturnedStringBuffer.size() < ReturnedStringBufferBytes) {
			ReturnedStringBuffer.resize(ReturnedStringBufferBytes);
		}
		int Result = GetPrivateProfileStringDx(AppName, KeyName, Default, ReturnedStringBuffer.data(), ReturnedStringBufferBytes, IniFilePath, IniFileCharCodeFormat);
		return std::tuple(Result, ReturnedStringBuffer.data());
	};
	t["GetPrivateProfileIntDx"] = [](const TCHAR *AppName, const TCHAR *KeyName, int Default, const TCHAR *IniFilePath, sol::variadic_args va) {
		int IniFileCharCodeFormat = va_get(va, 0, -1);
		return GetPrivateProfileIntDx(AppName, KeyName, Default, IniFilePath, IniFileCharCodeFormat);
	};
	//extern	int			GetPrivateProfileStringDxForMem(const TCHAR * AppName, const TCHAR * KeyName, const TCHAR * Default, TCHAR * ReturnedStringBuffer, size_t ReturnedStringBufferBytes, const void *IniFileImage, size_t IniFileImageBytes, int IniFileCharCodeFormat = -1 /* DX_CHARCODEFORMAT_SHIFTJIS 等、-1 でデフォルト */);		// GetPrivateProfileStringDx のメモリから読み込む版
	//extern	int			GetPrivateProfileStringDxForMemWithStrLen(const TCHAR * AppName, size_t AppNameLength, const TCHAR * KeyName, size_t KeyNameLength, const TCHAR * Default, size_t DefaultLength, TCHAR * ReturnedStringBuffer, size_t ReturnedStringBufferBytes, const void *IniFileImage, size_t IniFileImageBytes, int IniFileCharCodeFormat = -1 /* DX_CHARCODEFORMAT_SHIFTJIS 等、-1 でデフォルト */);		// GetPrivateProfileStringDx のメモリから読み込む版
#if 0
	t["GetPrivateProfileStringDxForMem"] = [](
		const TCHAR *AppName,
		const TCHAR *KeyName,
		const TCHAR *Default,
		size_t ReturnedStringBufferBytes,
		const void *IniFileImage,
		size_t IniFileImageBytes,
		sol::variadic_args va
	) {
		int IniFileCharCodeFormat = va_get(va, 0, -1);
		static std::vector<char> ReturnedStringBuffer;
		ReturnedStringBuffer.reserve(ReturnedStringBufferBytes);
		if (ReturnedStringBuffer.size() < ReturnedStringBufferBytes) {
			ReturnedStringBuffer.resize(ReturnedStringBufferBytes);
		}
		int Result = GetPrivateProfileStringDxForMem(AppName, KeyName, Default, ReturnedStringBuffer.data(), ReturnedStringBufferBytes, IniFileImage, IniFileImageBytes, IniFileCharCodeFormat);
		return Result;
	};
#endif
	//extern	int			GetPrivateProfileIntDxForMem(const TCHAR * AppName, const TCHAR * KeyName, int          Default, const void *IniFileImage, size_t IniFileImageBytes, int IniFileCharCodeFormat = -1 /* DX_CHARCODEFORMAT_SHIFTJIS 等、-1 でデフォルト */);		// GetPrivateProfileIntDx のメモリから読み込む版
	//extern	int			GetPrivateProfileIntDxForMemWithStrLen(const TCHAR * AppName, size_t AppNameLength, const TCHAR * KeyName, size_t KeyNameLength, int          Default, const void *IniFileImage, size_t IniFileImageBytes, int IniFileCharCodeFormat = -1 /* DX_CHARCODEFORMAT_SHIFTJIS 等、-1 でデフォルト */);		// GetPrivateProfileIntDx のメモリから読み込む版

#if defined( __APPLE__ ) || defined( __ANDROID__ )

	t["MailApp_Send"] = [](
		sol::optional<std::string> maybe_MailAddr, 
		sol::optional<std::string> maybe_MailCCAddr,
		sol::optional<std::string> maybe_MailBCCAddr,
		sol::optional<std::string> maybe_Subject,
		sol::optional<std::string> maybe_Text
	) {
		const TCHAR *MailAddr = NULL;
		const TCHAR *MailCCAddr = NULL;
		const TCHAR *MailBCCAddr = NULL;
		const TCHAR *Subject = NULL;
		const TCHAR *Text = NULL;

		if (maybe_MailAddr) MailAddr = maybe_MailAddr.value().c_str();
		if (maybe_MailCCAddr) MailCCAddr = maybe_MailCCAddr.value().c_str();
		if (maybe_MailBCCAddr) MailBCCAddr = maybe_MailBCCAddr.value().c_str();
		if (maybe_Subject) Subject = maybe_Subject.value().c_str();
		if (maybe_Text) Text = maybe_Text.value().c_str();

		return MailApp_Send(MailAddr, MailCCAddr, MailBCCAddr, Subject, Text);
	};

#endif // defined( __APPLE__ ) || defined( __ANDROID__ )

}

} // namespace DxLua::detail
