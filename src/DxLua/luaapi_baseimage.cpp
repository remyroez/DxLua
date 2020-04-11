#include <tuple>

#include <DxLib.h>
#include <lua.hpp>
#include <sol/sol.hpp>

#include "luaapi.h"

namespace DxLua::detail {

void port_baseimage(sol::state_view &lua, sol::table &t) {

	// 基本イメージデータのロード＋ＤＩＢ関係
	//extern	int			CreateGraphImageOrDIBGraph(const TCHAR * FileName, const void *DataImage, int DataImageSize, int DataImageType /* LOADIMAGE_TYPE_FILE 等 */, int BmpFlag, int ReverseFlag, BASEIMAGE * BaseImage, BITMAPINFO * *BmpInfo, void **GraphData);					// 画像ファイル若しくはメモリ上に展開された画像ファイルイメージを読み込み、基本イメージデータ若しくはＢＭＰデータを構築する
	//extern	int			CreateGraphImageOrDIBGraphWithStrLen(const TCHAR * FileName, size_t FileNameLength, const void *DataImage, int DataImageSize, int DataImageType /* LOADIMAGE_TYPE_FILE 等 */, int BmpFlag, int ReverseFlag, BASEIMAGE * BaseImage, BITMAPINFO * *BmpInfo, void **GraphData);					// 画像ファイル若しくはメモリ上に展開された画像ファイルイメージを読み込み、基本イメージデータ若しくはＢＭＰデータを構築する
	//extern	int			CreateGraphImageType2(STREAMDATA * Src, BASEIMAGE * Dest);																																																						// 画像データから構築したストリーム読み込み用データを使用して基本イメージデータを構築する
	//extern	int			CreateBmpInfo(BITMAPINFO * BmpInfo, int Width, int Height, int Pitch, const void *SrcGrData, void **DestGrData);																																						// 24bitカラー形式のビットマップデータからＢＭＰデータを構築する
	//extern	int			GetImageSize_File(const TCHAR * FileName, int *SizeX, int *SizeY);																																													// 画像ファイルの横ピクセル数と縦ピクセル数を取得する
	//extern	int			GetImageSize_FileWithStrLen(const TCHAR * FileName, size_t FileNameLength, int *SizeX, int *SizeY);																																													// 画像ファイルの横ピクセル数と縦ピクセル数を取得する
	//extern	int			GetImageSize_Mem(const void *FileImage, int FileImageSize, int *SizeX, int *SizeY);																																														// メモリ上に展開された画像ファイルの横ピクセル数と縦ピクセル数を取得する
	//extern	unsigned int GetGraphImageFullColorCode(const BASEIMAGE * GraphImage, int x, int y);																																																			// 基本イメージデータの指定座標のフルカラーコードを取得する
	//extern	int			CreateGraphImage_plus_Alpha(const TCHAR * FileName, const void *RgbImage, int RgbImageSize, int RgbImageType, const void *AlphaImage, int AlphaImageSize, int AlphaImageType, BASEIMAGE * RgbGraphImage, BASEIMAGE * AlphaGraphImage, int ReverseFlag);		// 画像ファイル若しくはメモリ上に展開された画像ファイルイメージを読み込み、基本イメージデータを構築する
	//extern	int			CreateGraphImage_plus_AlphaWithStrLen(const TCHAR * FileName, size_t FileNameLength, const void *RgbImage, int RgbImageSize, int RgbImageType, const void *AlphaImage, int AlphaImageSize, int AlphaImageType, BASEIMAGE * RgbGraphImage, BASEIMAGE * AlphaGraphImage, int ReverseFlag);		// 画像ファイル若しくはメモリ上に展開された画像ファイルイメージを読み込み、基本イメージデータを構築する
	//extern	int			ReverseGraphImage(BASEIMAGE * GraphImage);																																																								// 基本イメージデータを左右反転する

	////extern int		AddUserGraphLoadFunction( int ( *UserLoadFunc )( FILE *fp, BITMAPINFO **BmpInfo, void **GraphData ) ) ;																						// ユーザー定義の画像データ読み込み関数を登録する
	////extern int		AddUserGraphLoadFunction2( int ( *UserLoadFunc )( void *Image, int ImageSize, int ImageType, BITMAPINFO **BmpInfo, void **GraphData ) ) ;													// ユーザー定義の画像データ読み込み関数を登録する
	////extern int		AddUserGraphLoadFunction3( int ( *UserLoadFunc )( void *DataImage, int DataImageSize, int DataImageType, int BmpFlag, BASEIMAGE *BaseImage, BITMAPINFO **BmpInfo, void **GraphData ) ) ;	// ユーザー定義の画像データ読み込み関数Ver3を登録する
	//extern	int			AddUserGraphLoadFunction4(int (*UserLoadFunc)(STREAMDATA * Src, BASEIMAGE * BaseImage)); 																								// ユーザー定義の画像データ読み込み関数Ver4を登録する
	////extern int		SubUserGraphLoadFunction( int ( *UserLoadFunc )( FILE *fp, BITMAPINFO **BmpInfo, void **GraphData ) ) ;																						// ユーザー定義の画像データ読み込み関数を登録から抹消する
	////extern int		SubUserGraphLoadFunction2( int ( *UserLoadFunc )( void *Image, int ImageSize, int ImageType, BITMAPINFO **BmpInfo, void **GraphData ) ) ;													// ユーザー定義の画像データ読み込み関数を登録から抹消する
	////extern int		SubUserGraphLoadFunction3( int ( *UserLoadFunc )( void *DataImage, int DataImageSize, int DataImageType, int BmpFlag, BASEIMAGE *BaseImage, BITMAPINFO **BmpInfo, void **GraphData ) ) ;	// ユーザー定義の画像データ読み込み関数Ver3を登録から抹消する
	//extern	int			SubUserGraphLoadFunction4(int (*UserLoadFunc)(STREAMDATA * Src, BASEIMAGE * BaseImage)); 																								// ユーザー定義の画像データ読み込み関数Ver4を登録から抹消する

	//extern	int			SetUseFastLoadFlag(int Flag);														// 高速読み込みルーチンを使用するかどうかを設定する( TRUE:使用する( デフォルト )  FALSE:使用しない )
	//extern	int			SetGraphDataShavedMode(int ShavedMode /* DX_SHAVEDMODE_NONE 等 */);						// 画像減色時の画像劣化緩和処理モードを設定する( デフォルトでは緩和処理無しの DX_SHAVEDMODE_NONE )
	//extern	int			GetGraphDataShavedMode(void);															// 画像減色時の画像劣化緩和処理モードを取得する
	//extern	int			SetUsePremulAlphaConvertLoad(int UseFlag);													// 画像ファイル読み込み時に乗算済みアルファ画像に変換するかどうかを設定する( TRUE:変換処理を行う  FALSE:変換処理を行わない( デフォルト ) )

	// 基本イメージデータ構造体関係
	//extern	int			CreateBaseImage(const TCHAR * FileName, const void *FileImage, int FileImageSize, int DataType /*=LOADIMAGE_TYPE_FILE*/, BASEIMAGE * BaseImage, int ReverseFlag);			// 画像ファイル若しくはメモリ上に展開された画像ファイルイメージから基本イメージデータを構築する
	//extern	int			CreateBaseImageWithStrLen(const TCHAR * FileName, size_t FileNameLength, const void *FileImage, int FileImageSize, int DataType /*=LOADIMAGE_TYPE_FILE*/, BASEIMAGE * BaseImage, int ReverseFlag);			// 画像ファイル若しくはメモリ上に展開された画像ファイルイメージから基本イメージデータを構築する
	//extern	int			CreateGraphImage(const TCHAR * FileName, const void *DataImage, int DataImageSize, int DataImageType, BASEIMAGE * GraphImage, int ReverseFlag);			// CreateBaseImage の旧名称
	//extern	int			CreateBaseImageToFile(const TCHAR * FileName, BASEIMAGE * BaseImage, int ReverseFlag = FALSE);	// 画像ファイルから基本イメージデータを構築する
	//extern	int			CreateBaseImageToFileWithStrLen(const TCHAR * FileName, size_t FileNameLength, BASEIMAGE * BaseImage, int ReverseFlag = FALSE);	// 画像ファイルから基本イメージデータを構築する
	//extern	int			CreateBaseImageToMem(const void *FileImage, int FileImageSize, BASEIMAGE * BaseImage, int ReverseFlag = FALSE);	// メモリ上に展開された画像ファイルイメージから基本イメージデータを構築する
	//extern	int			CreateARGBF32ColorBaseImage(int SizeX, int SizeY, BASEIMAGE * BaseImage);																							// ＡＲＧＢ各チャンネル 32bit 浮動小数点型 カラーの基本イメージデータを作成する
	//extern	int			CreateARGBF16ColorBaseImage(int SizeX, int SizeY, BASEIMAGE * BaseImage);																							// ＡＲＧＢ各チャンネル 16bit 浮動小数点型 カラーの基本イメージデータを作成する
	//extern	int			CreateARGB8ColorBaseImage(int SizeX, int SizeY, BASEIMAGE * BaseImage);																							// ＡＲＧＢ８カラーの基本イメージデータを作成する
	//extern	int			CreateXRGB8ColorBaseImage(int SizeX, int SizeY, BASEIMAGE * BaseImage);																							// ＸＲＧＢ８カラーの基本イメージデータを作成する
	//extern	int			CreateRGB8ColorBaseImage(int SizeX, int SizeY, BASEIMAGE * BaseImage);																							// ＲＧＢ８カラーの基本イメージデータを作成する
	//extern	int			CreateARGB4ColorBaseImage(int SizeX, int SizeY, BASEIMAGE * BaseImage);																							// ＡＲＧＢ４カラーの基本イメージデータを作成する
	//extern	int			CreateA1R5G5B5ColorBaseImage(int SizeX, int SizeY, BASEIMAGE * BaseImage);																							// Ａ１Ｒ５Ｇ５Ｂ５カラーの基本イメージデータを作成する
	//extern	int			CreateX1R5G5B5ColorBaseImage(int SizeX, int SizeY, BASEIMAGE * BaseImage);																							// Ｘ１Ｒ５Ｇ５Ｂ５カラーの基本イメージデータを作成する
	//extern	int			CreateR5G5B5A1ColorBaseImage(int SizeX, int SizeY, BASEIMAGE * BaseImage);																							// Ｒ５Ｇ５Ｂ５Ａ１カラーの基本イメージデータを作成する
	//extern	int			CreateR5G6B5ColorBaseImage(int SizeX, int SizeY, BASEIMAGE * BaseImage);																							// Ｒ５Ｇ６Ｂ５カラーの基本イメージデータを作成する
	//extern	int			CreatePAL8ColorBaseImage(int SizeX, int SizeY, BASEIMAGE * BaseImage, int UseAlpha = FALSE);																	// パレット８ビットカラーの基本イメージデータを作成する
	//extern	int			CreateColorDataBaseImage(int SizeX, int SizeY, const COLORDATA * ColorData, BASEIMAGE * BaseImage);																							// 指定のカラーフォーマットの基本イメージデータを作成する
	//extern	int			GetBaseImageGraphDataSize(const BASEIMAGE * BaseImage);																																		// 基本イメージデータのイメージサイズを取得する( 単位：byte )
	//extern	int			DerivationBaseImage(const BASEIMAGE * BaseImage, int x1, int y1, int x2, int y2, BASEIMAGE * NewBaseImage);																			// 基本イメージデータの指定の部分だけを使う基本イメージデータの情報を作成する( BaseImage にはフォーマットが DX_BASEIMAGE_FORMAT_NORMAL でミップマップを使用していない画像のみ使用可能 )

	//extern	int			ReleaseBaseImage(BASEIMAGE * BaseImage);																																			// 基本イメージデータの後始末を行う
	//extern	int			ReleaseGraphImage(BASEIMAGE * GraphImage);																																			// ReleaseBaseImage の旧名称

	//extern	int			ConvertNormalFormatBaseImage(BASEIMAGE * BaseImage, int ReleaseOrigGraphData = TRUE);																											// DX_BASEIMAGE_FORMAT_NORMAL 以外の形式のイメージを DX_BASEIMAGE_FORMAT_NORMAL 形式のイメージに変換する
	//extern	int			ConvertPremulAlphaBaseImage(BASEIMAGE * BaseImage);																																			// 通常のαチャンネル付き画像を乗算済みαチャンネル付き画像に変換する( ピクセルフォーマットが ARGB8 以外の場合は ARGB8 に変換されます )
	//extern	int			ConvertInterpAlphaBaseImage(BASEIMAGE * BaseImage);																																			// 乗算済みαチャンネル付き画像を通常のαチャンネル付き画像に変換する( ピクセルフォーマットが ARGB8 以外の場合は ARGB8 に変換されます )

	//extern	int			GetDrawScreenBaseImage(int x1, int y1, int x2, int y2, BASEIMAGE * BaseImage);																											// 描画対象の画面から指定領域を基本イメージデータに転送する
	//extern	int			GetDrawScreenBaseImageDestPos(int x1, int y1, int x2, int y2, BASEIMAGE * BaseImage, int DestX, int DestY);																						// 描画対象の画面から指定領域を基本イメージデータに転送する( 転送先座標指定版 )
	//extern	int			FillBaseImage(BASEIMAGE * BaseImage, int r, int g, int b, int a);																											// 基本イメージデータを指定の色で塗りつぶす
	//extern	int			FillRectBaseImage(BASEIMAGE * BaseImage, int x, int y, int w, int h, int r, int g, int b, int a);																				// 基本イメージデータの指定の領域を指定の色で塗りつぶす
	//extern	int			ClearRectBaseImage(BASEIMAGE * BaseImage, int x, int y, int w, int h);																											// 基本イメージデータの指定の領域を０クリアする
	//extern	int			GetPaletteBaseImage(const BASEIMAGE * BaseImage, int PaletteNo, int *r, int *g, int *b, int *a);																						// 基本イメージデータのパレットを取得する
	//extern	int			SetPaletteBaseImage(BASEIMAGE * BaseImage, int PaletteNo, int  r, int  g, int  b, int  a);																						// 基本イメージデータのパレットをセットする
	//extern	int			SetPixelPalCodeBaseImage(BASEIMAGE * BaseImage, int x, int y, int palNo);																											// 基本イメージデータの指定の座標の色コードを変更する(パレット画像用)
	//extern	int			GetPixelPalCodeBaseImage(const BASEIMAGE * BaseImage, int x, int y);																														// 基本イメージデータの指定の座標の色コードを取得する(パレット画像用)
	//extern	int			SetPixelBaseImage(BASEIMAGE * BaseImage, int x, int y, int    r, int    g, int    b, int    a);																				// 基本イメージデータの指定の座標の色を変更する(各色要素は０～２５５)
	//extern	int			SetPixelBaseImageF(BASEIMAGE * BaseImage, int x, int y, float  r, float  g, float  b, float  a);																				// 基本イメージデータの指定の座標の色を変更する(各色要素は浮動小数点数)
	//extern	int			GetPixelBaseImage(const BASEIMAGE * BaseImage, int x, int y, int *r, int *g, int *b, int *a);																				// 基本イメージデータの指定の座標の色を取得する(各色要素は０～２５５)
	//extern	int			GetPixelBaseImageF(const BASEIMAGE * BaseImage, int x, int y, float *r, float *g, float *b, float *a);																				// 基本イメージデータの指定の座標の色を取得する(各色要素は浮動小数点数)
	//extern	int			DrawLineBaseImage(BASEIMAGE * BaseImage, int x1, int y1, int x2, int y2, int r, int g, int b, int a);																			// 基本イメージデータの指定の座標に線を描画する(各色要素は０～２５５)
	//extern	int			DrawCircleBaseImage(BASEIMAGE * BaseImage, int x, int y, int radius, int r, int g, int b, int a, int FillFlag = TRUE);															// 基本イメージデータの指定の座標に円を描画する(各色要素は０～２５５)
	//extern	int			BltBaseImage(int SrcX, int SrcY, int SrcSizeX, int SrcSizeY, int DestX, int DestY, BASEIMAGE * SrcBaseImage, BASEIMAGE * DestBaseImage);									// 基本イメージデータを別の基本イメージデータに転送する
	//extern	int			BltBaseImage(int DestX, int DestY, BASEIMAGE * SrcBaseImage, BASEIMAGE * DestBaseImage);									// 基本イメージデータを別の基本イメージデータに転送する
	//extern	int			BltBaseImageWithTransColor(int SrcX, int SrcY, int SrcSizeX, int SrcSizeY, int DestX, int DestY, BASEIMAGE * SrcBaseImage, BASEIMAGE * DestBaseImage, int Tr, int Tg, int Tb, int Ta);	// 基本イメージデータを別の基本イメージデータに透過色処理付きで転送する
	//extern	int			BltBaseImageWithAlphaBlend(int SrcX, int SrcY, int SrcSizeX, int SrcSizeY, int DestX, int DestY, BASEIMAGE * SrcBaseImage, BASEIMAGE * DestBaseImage, int Opacity = 255);				// 基本イメージデータを別の基本イメージデータにアルファ値のブレンドを考慮した上で転送する( Opacity は透明度 : 0( 完全透明 ) ～ 255( 完全不透明 ) )( 出力先が ARGB8 形式以外の場合はエラーになります )
	//extern	int			ReverseBaseImageH(BASEIMAGE * BaseImage);																																		// 基本イメージデータを左右反転する
	//extern	int			ReverseBaseImageV(BASEIMAGE * BaseImage);																																		// 基本イメージデータを上下反転する
	//extern	int			ReverseBaseImage(BASEIMAGE * BaseImage);																																		// 基本イメージデータを上下左右反転する
	//extern	int			CheckPixelAlphaBaseImage(const BASEIMAGE * BaseImage);																																		// 基本イメージデータに含まれるピクセルのアルファ値をチェックする( 戻り値   -1:エラー  0:画像にアルファ成分が無い  1:画像にアルファ成分があり、すべて最大(255)値  2:画像にアルファ成分があり、存在するアルファ値は最小(0)と最大(255)もしくは最小(0)のみ　3:画像にアルファ成分があり、最小と最大以外の中間の値がある )  
	//extern	int			GetBaseImageUseMaxPaletteNo(const BASEIMAGE * BaseImage);																																		// 基本イメージデータで使用されているパレット番号の最大値を取得する( パレット画像では無い場合は -1 が返る )

#ifndef DX_NON_JPEGREAD
	//extern	int			ReadJpegExif(const TCHAR * JpegFilePath, BYTE * ExifBuffer_Array, size_t ExifBufferSize);															// JPEGファイルの Exif情報を取得する、ExifBuffer を NULL に渡すと、戻り値の情報のサイズのみ取得できます( 戻り値  -1:エラー  -1以外：Exif情報のサイズ )
	//extern	int			ReadJpegExifWithStrLen(const TCHAR * JpegFilePath, size_t JpegFilePathLength, BYTE * ExifBuffer_Array, size_t ExifBufferSize);															// JPEGファイルの Exif情報を取得する、ExifBuffer を NULL に渡すと、戻り値の情報のサイズのみ取得できます( 戻り値  -1:エラー  -1以外：Exif情報のサイズ )
#endif // DX_NON_JPEGREAD

#ifndef DX_NON_SAVEFUNCTION

	//extern	int			SaveBaseImageToBmp(const TCHAR * FilePath, const BASEIMAGE * BaseImage);																						// 基本イメージデータをＢＭＰ画像として保存する
	//extern	int			SaveBaseImageToBmpWithStrLen(const TCHAR * FilePath, size_t FilePathLength, const BASEIMAGE * BaseImage);																						// 基本イメージデータをＢＭＰ画像として保存する
	//extern	int			SaveBaseImageToDds(const TCHAR * FilePath, const BASEIMAGE * BaseImage);																						// 基本イメージデータをＤＤＳ画像として保存する
	//extern	int			SaveBaseImageToDdsWithStrLen(const TCHAR * FilePath, size_t FilePathLength, const BASEIMAGE * BaseImage);																						// 基本イメージデータをＤＤＳ画像として保存する
#ifndef DX_NON_PNGREAD
	//extern	int			SaveBaseImageToPng(const TCHAR * FilePath, BASEIMAGE * BaseImage, int CompressionLevel);																		// 基本イメージデータをＰＮＧ画像として保存する
	//extern	int			SaveBaseImageToPngWithStrLen(const TCHAR * FilePath, size_t FilePathLength, BASEIMAGE * BaseImage, int CompressionLevel);																		// 基本イメージデータをＰＮＧ画像として保存する
#endif // DX_NON_PNGREAD
#ifndef DX_NON_JPEGREAD
	//extern	int			SaveBaseImageToJpeg(const TCHAR * FilePath, BASEIMAGE * BaseImage, int Quality, int Sample2x1);																	// 基本イメージデータをＪＰＥＧ画像として保存する
	//extern	int			SaveBaseImageToJpegWithStrLen(const TCHAR * FilePath, size_t FilePathLength, BASEIMAGE * BaseImage, int Quality, int Sample2x1);																	// 基本イメージデータをＪＰＥＧ画像として保存する
#endif // DX_NON_JPEGREAD

#endif // DX_NON_SAVEFUNCTION

	// 基本イメージ描画
	//extern	int			DrawBaseImage(int x, int y, BASEIMAGE * BaseImage);																																// 基本イメージデータを描画する

	// カラーマッチングしながらイメージデータ間転送を行う Ver2
	//extern int			GraphColorMatchBltVer2(void *DestGraphData, int DestPitch, const COLORDATA * DestColorData,
	//	const void *SrcGraphData, int SrcPitch, const COLORDATA * SrcColorData,
	//	const void *AlphaMask, int AlphaPitch, const COLORDATA * AlphaColorData,
	//	POINT DestPoint, const RECT * SrcRect, int ReverseFlag,
	//	int TransColorAlphaTestFlag, unsigned int TransColor,
	//	int ImageShavedMode, int AlphaOnlyFlag = FALSE,
	//	int RedIsAlphaFlag = FALSE, int TransColorNoMoveFlag = FALSE,
	//	int Pal8ColorMatch = FALSE);


	// 色情報取得関係
	DXLUA_PORT(t, GetColorF);
	DXLUA_PORT(t, GetColorU8);
	DXLUA_PORT(t, GetColor);
	//extern	int				GetColor2(unsigned int Color, int *Red, int *Green, int *Blue);												// カラー値から赤、緑、青の値を取得する
	//extern	unsigned int	GetColor3(const COLORDATA * ColorData, int Red, int Green, int Blue, int Alpha = 255);							// 指定のピクセルフォーマットに対応したカラー値を得る
	//extern	unsigned int	GetColor4(const COLORDATA * DestColorData, const COLORDATA * SrcColorData, unsigned int SrcColor);				// 指定のカラーフォーマットのカラー値を別のカラーフォーマットのカラー値に変換する
	//extern	int				GetColor5(const COLORDATA * ColorData, unsigned int Color, int *Red, int *Green, int *Blue, int *Alpha = NULL);	// 指定のカラーフォーマットのカラー値を赤、緑、青、アルファの値を取得する
	//extern	int				CreatePaletteColorData(COLORDATA * ColorDataBuf);																			// パレットカラーのカラーフォーマットを構築する
	//extern	int				CreateARGBF32ColorData(COLORDATA * ColorDataBuf);																			// ＡＲＧＢ各チャンネル 32bit 浮動小数点型カラーのカラーフォーマットを構築する
	//extern	int				CreateARGBF16ColorData(COLORDATA * ColorDataBuf);																			// ＡＲＧＢ各チャンネル 16bit 浮動小数点型カラーのカラーフォーマットを構築する
	//extern	int				CreateXRGB8ColorData(COLORDATA * ColorDataBuf);																			// ＸＲＧＢ８カラーのカラーフォーマットを構築する
	//extern	int				CreateARGB8ColorData(COLORDATA * ColorDataBuf);																			// ＡＲＧＢ８カラーのカラーフォーマットを構築する
	//extern	int				CreateARGB4ColorData(COLORDATA * ColorDataBuf);																			// ＡＲＧＢ４カラーのカラーフォーマットを構築する
	//extern	int				CreateA1R5G5B5ColorData(COLORDATA * ColorDataBuf);																			// Ａ１Ｒ５Ｇ５Ｂ５カラーのカラーフォーマットを構築する
	//extern	int				CreateX1R5G5B5ColorData(COLORDATA * ColorDataBuf);																			// Ｘ１Ｒ５Ｇ５Ｂ５カラーのカラーフォーマットを構築する
	//extern	int				CreateR5G5B5A1ColorData(COLORDATA * ColorDataBuf);																			// Ｒ５Ｇ５Ｂ５Ａ１カラーのカラーフォーマットを構築する
	//extern	int				CreateR5G6B5ColorData(COLORDATA * ColorDataBuf);																			// Ｒ５Ｇ６Ｂ５カラーのカラーフォーマットを構築する
	//extern	int				CreateFullColorData(COLORDATA * ColorDataBuf);																			// ２４ビットカラーのカラーフォーマットを構築する
	//extern	int				CreateGrayColorData(COLORDATA * ColorDataBuf);																			// グレースケールのカラーフォーマットを構築する
	//extern	int				CreatePal8ColorData(COLORDATA * ColorDataBuf, int UseAlpha = FALSE);														// パレット２５６色のカラーフォーマットを構築する
	//extern	int				CreateColorData(COLORDATA * ColorDataBuf, int ColorBitDepth,
	//	DWORD RedMask, DWORD GreenMask, DWORD BlueMask, DWORD AlphaMask,
	//	int ChannelNum = 0, int ChannelBitDepth = 0, int FloatTypeFlag = FALSE);								// カラーフォーマットを作成する
	//extern	void			SetColorDataNoneMask(COLORDATA * ColorData);																				// NoneMask 以外の要素を埋めた COLORDATA 構造体の情報を元に NoneMask をセットする
	//extern	int				CmpColorData(const COLORDATA * ColorData1, const COLORDATA * ColorData2);											// 二つのカラーフォーマットが等しいかどうか調べる( 戻り値　TRUE:等しい  FALSE:等しくない )

}

} // namespace DxLua::detail
