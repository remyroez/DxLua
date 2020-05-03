#include <DxLib.h>
#include <lua.hpp>
#include <sol/sol.hpp>

#include "luaapi.h"

namespace DxLua::detail {

void port_file(sol::state_view &lua, sol::table &t) {

	// ファイルアクセス関数
	t["FileRead_open"] = [](const TCHAR *FilePath, sol::variadic_args va) {
		int ASync = va_get(va, 0, false);
		return FileRead_open(FilePath, ASync);
	};
	//extern	int			FileRead_open_mem(const void *FileImage, size_t FileImageSize);					// メモリに展開されたファイルを開く
	DXLUA_PORT(t, FileRead_size);
	DXLUA_PORT(t, FileRead_close);
	DXLUA_PORT(t, FileRead_tell);
	DXLUA_PORT(t, FileRead_seek);
	//extern	int			FileRead_read(void *Buffer, int ReadSize, int FileHandle);				// ファイルからデータを読み込む
	DXLUA_PORT(t, FileRead_idle_chk);
	DXLUA_PORT(t, FileRead_eof);
	t["FileRead_set_format"] = [](int FileHandle, sol::variadic_args va) {
		int CharCodeFormat = va_get(va, 0, DX_CHARCODEFORMAT_UTF8);
		return FileRead_set_format(FileHandle, CharCodeFormat);
	};
	//extern	int			FileRead_gets(TCHAR * Buffer, int BufferSize, int FileHandle);				// ファイルから文字列を読み出す
	//extern	TCHAR		FileRead_getc(int FileHandle);												// ファイルから一文字読み出す
	//extern	int			FileRead_scanf(int FileHandle, const TCHAR * Format, ...);					// ファイルから書式化されたデータを読み出す

	//extern	DWORD_PTR	FileRead_createInfo(const TCHAR * ObjectPath);										// ファイル情報ハンドルを作成する( 戻り値  -1:エラー  -1以外:ファイル情報ハンドル )
	//extern	DWORD_PTR	FileRead_createInfo_WithStrLen(const TCHAR * ObjectPath, size_t ObjectPathLength);			// ファイル情報ハンドルを作成する( 戻り値  -1:エラー  -1以外:ファイル情報ハンドル )
	//extern	int			FileRead_getInfoNum(DWORD_PTR FileInfoHandle);									// ファイル情報ハンドル中のファイルの数を取得する
	//extern	int			FileRead_getInfo(int Index, FILEINFO * Buffer, DWORD_PTR FileInfoHandle);		// ファイル情報ハンドル中のファイルの情報を取得する
	//extern	int			FileRead_deleteInfo(DWORD_PTR FileInfoHandle);									// ファイル情報ハンドルを削除する

	//extern	DWORD_PTR	FileRead_findFirst(const TCHAR * FilePath, FILEINFO * Buffer); // 指定のファイル又はフォルダの情報を取得し、ファイル検索ハンドルも作成する( 戻り値: -1=エラー  -1以外=ファイル検索ハンドル )
	//extern	DWORD_PTR	FileRead_findFirst_WithStrLen(const TCHAR * FilePath, size_t FilePathLength, FILEINFO * Buffer); // 指定のファイル又はフォルダの情報を取得し、ファイル検索ハンドルも作成する( 戻り値: -1=エラー  -1以外=ファイル検索ハンドル )
	//extern	int			FileRead_findNext(DWORD_PTR FindHandle, FILEINFO * Buffer);						// 条件の合致する次のファイルの情報を取得する( 戻り値: -1=エラー  0=成功 )
	//extern	int			FileRead_findClose(DWORD_PTR FindHandle);										// ファイル検索ハンドルを閉じる( 戻り値: -1=エラー  0=成功 )

	DXLUA_PORT(t, FileRead_fullyLoad);
	DXLUA_PORT(t, FileRead_fullyLoad_delete);
	DXLUA_PORT(t, FileRead_fullyLoad_getImage);
	DXLUA_PORT(t, FileRead_fullyLoad_getSize);

	// 設定関係関数
	DXLUA_PORT(t, GetStreamFunctionDefault);
	DXLUA_PORT(t, ChangeStreamFunction);
	DXLUA_PORT(t, ChangeStreamFunctionW);

	// 補助関係関数
	//extern int			ConvertFullPath(const TCHAR * Src, TCHAR * Dest, const TCHAR * CurrentDir = NULL);	// フルパスではないパス文字列をフルパスに変換する( CurrentDir はフルパスである必要がある(語尾に『\』があっても無くても良い) )( CurrentDir が NULL の場合は現在のカレントディレクトリを使用する )
	//extern int			ConvertFullPathWithStrLen(const TCHAR * Src, size_t SrcLength, TCHAR * Dest, const TCHAR * CurrentDir = NULL, size_t CurrentDirLength = 0);	// フルパスではないパス文字列をフルパスに変換する( CurrentDir はフルパスである必要がある(語尾に『\』があっても無くても良い) )( CurrentDir が NULL の場合は現在のカレントディレクトリを使用する )
}

} // namespace DxLua::detail
