#include <tuple>

#include <DxLib.h>
#include <lua.hpp>
#include <sol/sol.hpp>

#include "DxLua.h"
#include "luaapi.h"

#ifdef min
#undef min
#endif

namespace {

template <typename F, class... Args>
int forward_GraphFilter(int FilterType, sol::variadic_args va, F f, Args&&... args) {
	using namespace DxLua::detail;

	int Result = FALSE;

	switch (FilterType) {
	case DX_GRAPH_FILTER_MONO:
		{
			int Cb = va_get(va, 0, 8);
			int Cr = va_get(va, 1, 0);
			Result = f(std::forward<Args>(args)..., Cb, Cr);
			break;
		}
	case DX_GRAPH_FILTER_GAUSS:
		{
			int PixelWidth = va_get(va, 0, 8);
			int Param = va_get(va, 1, 0);
			Result = f(std::forward<Args>(args)..., PixelWidth, Param);
			break;
		}
	case DX_GRAPH_FILTER_DOWN_SCALE:
		{
			int DivNum = va_get(va, 0, 2);
			Result = f(std::forward<Args>(args)..., DivNum);
			break;
		}
	case DX_GRAPH_FILTER_BRIGHT_CLIP:
		{
			int CmpType = va_get(va, 0, DX_CMP_LESS);
			int CmpParam = va_get(va, 1, 0);
			int ClipFillFlag = va_get(va, 2, false);
			unsigned int ClipFillColor = va_get(va, 3, 0xFFFFFFFFUL);
			int ClipFillAlpha = va_get(va, 4, 255);
			Result = f(
				std::forward<Args>(args)...,
				CmpType, CmpParam, ClipFillFlag, ClipFillColor, ClipFillAlpha
			);
			break;
		}
	case DX_GRAPH_FILTER_BRIGHT_SCALE:
		{
			int MinBright = va_get(va, 0, 0);
			int MaxBright = va_get(va, 1, 0);
			Result = f(std::forward<Args>(args)..., MinBright, MaxBright);
			break;
		}
	case DX_GRAPH_FILTER_HSB:
		{
			int HueType = va_get(va, 0, 0);
			int Hue = va_get(va, 1, 0);
			int Saturation = va_get(va, 2, 0);
			int Bright = va_get(va, 3, 0);
			Result = f(std::forward<Args>(args)..., HueType, Hue, Saturation, Bright);
			break;
		}
	case DX_GRAPH_FILTER_INVERT:
		{
			Result = f(std::forward<Args>(args)...);
			break;
		}
	case DX_GRAPH_FILTER_LEVEL:
		{
			int Min = va_get(va, 0, 0);
			int Max = va_get(va, 1, 0);
			int Gamma = va_get(va, 2, 0);
			int AfterMin = va_get(va, 3, 0);
			int AfterMax = va_get(va, 4, 0);
			Result = f(std::forward<Args>(args)..., Min, Max, Gamma, AfterMin, AfterMax);
			break;
		}
	case DX_GRAPH_FILTER_TWO_COLOR:
		{
			int Threshold = va_get(va, 0, 0);
			unsigned int LowColor = va_get(va, 1, 0xFFFFFFFFUL);
			int LowAlpha = va_get(va, 2, 0);
			unsigned int HighColor = va_get(va, 3, 0xFFFFFFFFUL);
			int HighAlpha = va_get(va, 4, 0);
			Result = f(std::forward<Args>(args)..., Threshold, LowColor, LowAlpha, HighColor, HighAlpha);
			break;
		}
	case DX_GRAPH_FILTER_GRADIENT_MAP:
		{
			int MapGrHandle = va_get(va, 0, -1);
			int Reverse = va_get(va, 1, false);
			Result = f(std::forward<Args>(args)..., MapGrHandle, Reverse);
			break;
		}
	case DX_GRAPH_FILTER_PREMUL_ALPHA:
		{
			Result = f(std::forward<Args>(args)...);
			break;
		}
	case DX_GRAPH_FILTER_INTERP_ALPHA:
		{
			Result = f(std::forward<Args>(args)...);
			break;
		}
	case DX_GRAPH_FILTER_YUV_TO_RGB:
		{
			Result = f(std::forward<Args>(args)...);
			break;
		}
	case DX_GRAPH_FILTER_Y2UV1_TO_RGB:
		{
			int UVGrHandle = va_get(va, 0, -1);
			Result = f(std::forward<Args>(args)..., UVGrHandle);
			break;
		}
	case DX_GRAPH_FILTER_YUV_TO_RGB_RRA:
		{
			Result = f(std::forward<Args>(args)...);
			break;
		}
	case DX_GRAPH_FILTER_Y2UV1_TO_RGB_RRA:
		{
			int UVGrHandle = va_get(va, 0, -1);
			Result = f(std::forward<Args>(args)..., UVGrHandle);
			break;
		}
	case DX_GRAPH_FILTER_BICUBIC_SCALE:
		{
			int DestSizeX = va_get(va, 0, 0);
			int DestSizeY = va_get(va, 0, 0);
			Result = f(std::forward<Args>(args)..., DestSizeX, DestSizeY);
			break;
		}
	case DX_GRAPH_FILTER_LANCZOS3_SCALE:
		{
			int DestSizeX = va_get(va, 0, 0);
			int DestSizeY = va_get(va, 0, 0);
			Result = f(std::forward<Args>(args)..., DestSizeX, DestSizeY);
			break;
		}
	}
	return Result;
}

template <class T>
auto &table_to_array(sol::table table) {
	static std::vector<T> array;
	static T zero{};

	auto size = table.size();
	array.reserve(size);
	if (array.size() < size) {
		array.resize(size);
	}

	for (auto i = 0; i < size; ++i) {
		array[i] = table[i + 1].get_or(zero);
	}

	return array;
}

} // namespace

namespace DxLua::detail {

void port_draw(sol::state_view &lua, sol::table &library) {
#ifndef DX_NOTUSE_DRAWFUNCTION

	// グラフィックハンドル作成関係関数
	//extern	int			MakeGraph(int SizeX, int SizeY, int NotUse3DFlag = FALSE);							// 指定サイズのグラフィックハンドルを作成する
	library["MakeScreen"] = [library](int SizeX, int SizeY, sol::variadic_args va) {
		int UseAlphaChannel = va_get(va, 0, false);
		return MakeScreen(SizeX, SizeY, UseAlphaChannel);
	};
	//extern	int			DerivationGraph(int   SrcX, int   SrcY, int   Width, int   Height, int SrcGraphHandle);	// 指定のグラフィックハンドルの指定部分だけを抜き出して新たなグラフィックハンドルを作成する
	//extern	int			DerivationGraphF(float SrcX, float SrcY, float Width, float Height, int SrcGraphHandle);	// 指定のグラフィックハンドルの指定部分だけを抜き出して新たなグラフィックハンドルを作成する( float版 )
	library["DeleteGraph"] = [library](int GrHandle, sol::variadic_args va) {
		int LogOutFlag = va_get(va, 0, false);
		return DeleteGraph(GrHandle, LogOutFlag);
	};
	//extern	int			DeleteSharingGraph(int GrHandle);															// 指定のグラフィックハンドルと、同じグラフィックハンドルから派生しているグラフィックハンドル( DerivationGraph で派生したハンドル、LoadDivGraph 読み込んで作成された複数のハンドル )を一度に削除する
	//extern	int			GetGraphNum(void);																	// 有効なグラフィックハンドルの数を取得する
	//extern	int			FillGraph(int GrHandle, int Red, int Green, int Blue, int Alpha = 255);				// グラフィックハンドルを指定の色で塗りつぶす
	//extern	int			FillRectGraph(int GrHandle, int x, int y, int Width, int Height, int Red, int Green, int Blue, int Alpha = 255);	// グラフィックハンドルの指定の範囲を指定の色で塗りつぶす
	//extern	int			SetGraphLostFlag(int GrHandle, int *LostFlag);												// 指定のグラフィックハンドルが削除された際に 1 にする変数のアドレスを設定する
	//extern	int			InitGraph(int LogOutFlag = FALSE);													// すべてのグラフィックハンドルを削除する
	//extern	int			ReloadFileGraphAll(void);																	// ファイルから画像を読み込んだ全てのグラフィックハンドルについて、再度ファイルから画像を読み込む

	// シャドウマップハンドル関係関数
	//extern	int			MakeShadowMap(int SizeX, int SizeY);													// シャドウマップハンドルを作成する
	//extern	int			DeleteShadowMap(int SmHandle);															// シャドウマップハンドルを削除する
	//extern	int			SetShadowMapLightDirection(int SmHandle, VECTOR Direction);											// シャドウマップが想定するライトの方向を設定する
	//extern	int			ShadowMap_DrawSetup(int SmHandle);															// シャドウマップへの描画の準備を行う
	//extern	int			ShadowMap_DrawEnd(void);																	// シャドウマップへの描画を終了する
	//extern	int			SetUseShadowMap(int SmSlotIndex, int SmHandle);											// 描画で使用するシャドウマップを指定する、有効なスロットは０～２、SmHandle に -1 を渡すと指定のスロットのシャドウマップを解除
	//extern	int			SetShadowMapDrawArea(int SmHandle, VECTOR MinPosition, VECTOR MaxPosition);					// シャドウマップに描画する際の範囲を設定する( この関数で描画範囲を設定しない場合は視錐台を拡大した範囲が描画範囲となる )
	//extern	int			ResetShadowMapDrawArea(int SmHandle);															// SetShadowMapDrawArea の設定を解除する
	//extern	int			SetShadowMapAdjustDepth(int SmHandle, float Depth);												// シャドウマップを使用した描画時の補正深度を設定する
	//extern	int			GetShadowMapViewProjectionMatrix(int SmHandle, MATRIX * MatrixBuffer);										// シャドウマップ作成時や適用時に使用するビュー行列と射影行列を乗算した行列を取得する
	//extern	int			TestDrawShadowMap(int SmHandle, int x1, int y1, int x2, int y2);							// シャドウマップを画面にテスト描画する

	// グラフィックハンドルへの画像転送関数
	//extern	int			BltBmpToGraph(const COLORDATA * BmpColorData, HBITMAP RgbBmp, HBITMAP AlphaBmp, int CopyPointX, int CopyPointY, int  GrHandle);					// ＢＭＰの内容をグラフィックハンドルに転送
	//extern	int			BltBmpToDivGraph(const COLORDATA * BmpColorData, HBITMAP RgbBmp, HBITMAP AlphaBmp, int AllNum, int XNum, int YNum, int Width, int Height, const int *GrHandle, int ReverseFlag);	// ＢＭＰの内容を分割作成したグラフィックハンドルたちに転送
	//extern	int			BltBmpOrGraphImageToGraph(const COLORDATA * BmpColorData, HBITMAP RgbBmp, HBITMAP AlphaBmp, int BmpFlag, const BASEIMAGE * RgbBaseImage, const BASEIMAGE * AlphaBaseImage, int CopyPointX, int CopyPointY, int  GrHandle);					// ＢＭＰ か BASEIMAGE をグラフィックハンドルに転送
	//extern	int			BltBmpOrGraphImageToGraph2(const COLORDATA * BmpColorData, HBITMAP RgbBmp, HBITMAP AlphaBmp, int BmpFlag, const BASEIMAGE * RgbBaseImage, const BASEIMAGE * AlphaBaseImage, const RECT * SrcRect, int DestX, int DestY, int  GrHandle);					// ＢＭＰ か BASEIMAGE の指定の領域をグラフィックハンドルに転送
	//extern	int			BltBmpOrGraphImageToDivGraph(const COLORDATA * BmpColorData, HBITMAP RgbBmp, HBITMAP AlphaBmp, int BmpFlag, const BASEIMAGE * RgbBaseImage, const BASEIMAGE * AlphaBaseImage, int AllNum, int XNum, int YNum, int   Width, int   Height, const int *GrHandle, int ReverseFlag);	// ＢＭＰ か BASEIMAGE を分割作成したグラフィックハンドルたちに転送
	//extern	int			BltBmpOrGraphImageToDivGraphF(const COLORDATA * BmpColorData, HBITMAP RgbBmp, HBITMAP AlphaBmp, int BmpFlag, const BASEIMAGE * RgbBaseImage, const BASEIMAGE * AlphaBaseImage, int AllNum, int XNum, int YNum, float Width, float Height, const int *GrHandle, int ReverseFlag);	// ＢＭＰ か BASEIMAGE を分割作成したグラフィックハンドルたちに転送( float型 )

	// 画像からグラフィックハンドルを作成する関数
	//extern	int			LoadBmpToGraph(const TCHAR * FileName, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL);										// 画像ファイルからグラフィックハンドルを作成する
	//extern	int			LoadBmpToGraphWithStrLen(const TCHAR * FileName, size_t FileNameLength, int TextureFlag, int ReverseFlag, int SurfaceMode = DX_MOVIESURFACE_NORMAL);										// 画像ファイルからグラフィックハンドルを作成する
	library["LoadGraph"] = [library](const TCHAR *FileName, sol::variadic_args va) {
		int NotUse3DFlag = va_get(va, 0, false);
		return LoadGraph(DxLua::to_data_path(library, FileName).c_str(), NotUse3DFlag);
	};
	//extern	int			LoadGraphWithStrLen(const TCHAR * FileName, size_t FileNameLength, int NotUse3DFlag = FALSE);																							// 画像ファイルからグラフィックハンドルを作成する
	//extern	int			LoadReverseGraph(const TCHAR * FileName, int NotUse3DFlag = FALSE);																							// 画像ファイルを反転したものでグラフィックハンドルを作成する
	//extern	int			LoadReverseGraphWithStrLen(const TCHAR * FileName, size_t FileNameLength, int NotUse3DFlag = FALSE);																							// 画像ファイルを反転したものでグラフィックハンドルを作成する
	//extern	int			LoadDivGraph(const TCHAR * FileName, int AllNum, int XNum, int YNum, int   XSize, int   YSize, int *HandleArray, int NotUse3DFlag = FALSE);				// 画像ファイルを分割してグラフィックハンドルを作成する
	//extern	int			LoadDivGraphWithStrLen(const TCHAR * FileName, size_t FileNameLength, int AllNum, int XNum, int YNum, int   XSize, int   YSize, int *HandleArray, int NotUse3DFlag = FALSE);				// 画像ファイルを分割してグラフィックハンドルを作成する
	//extern	int			LoadDivGraphF(const TCHAR * FileName, int AllNum, int XNum, int YNum, float XSize, float YSize, int *HandleArray, int NotUse3DFlag = FALSE);				// 画像ファイルを分割してグラフィックハンドルを作成する
	//extern	int			LoadDivGraphFWithStrLen(const TCHAR * FileName, size_t FileNameLength, int AllNum, int XNum, int YNum, float XSize, float YSize, int *HandleArray, int NotUse3DFlag = FALSE);				// 画像ファイルを分割してグラフィックハンドルを作成する
	//extern	int			LoadDivBmpToGraph(const TCHAR * FileName, int AllNum, int XNum, int YNum, int   SizeX, int   SizeY, int *HandleArray, int TextureFlag, int ReverseFlag);		// 画像ファイルを分割してグラフィックハンドルを作成する
	//extern	int			LoadDivBmpToGraphWithStrLen(const TCHAR * FileName, size_t FileNameLength, int AllNum, int XNum, int YNum, int   SizeX, int   SizeY, int *HandleArray, int TextureFlag, int ReverseFlag);		// 画像ファイルを分割してグラフィックハンドルを作成する
	//extern	int			LoadDivBmpToGraphF(const TCHAR * FileName, int AllNum, int XNum, int YNum, float SizeX, float SizeY, int *HandleArray, int TextureFlag, int ReverseFlag);		// 画像ファイルを分割してグラフィックハンドルを作成する
	//extern	int			LoadDivBmpToGraphFWithStrLen(const TCHAR * FileName, size_t FileNameLength, int AllNum, int XNum, int YNum, float SizeX, float SizeY, int *HandleArray, int TextureFlag, int ReverseFlag);		// 画像ファイルを分割してグラフィックハンドルを作成する
	//extern	int			LoadReverseDivGraph(const TCHAR * FileName, int AllNum, int XNum, int YNum, int   XSize, int   YSize, int *HandleArray, int NotUse3DFlag = FALSE);				// 画像ファイルを反転したものを分割してグラフィックハンドルを作成する
	//extern	int			LoadReverseDivGraphWithStrLen(const TCHAR * FileName, size_t FileNameLength, int AllNum, int XNum, int YNum, int   XSize, int   YSize, int *HandleArray, int NotUse3DFlag = FALSE);				// 画像ファイルを反転したものを分割してグラフィックハンドルを作成する
	//extern	int			LoadReverseDivGraphF(const TCHAR * FileName, int AllNum, int XNum, int YNum, float XSize, float YSize, int *HandleArray, int NotUse3DFlag = FALSE);				// 画像ファイルを反転したものを分割してグラフィックハンドルを作成する
	//extern	int			LoadReverseDivGraphFWithStrLen(const TCHAR * FileName, size_t FileNameLength, int AllNum, int XNum, int YNum, float XSize, float YSize, int *HandleArray, int NotUse3DFlag = FALSE);				// 画像ファイルを反転したものを分割してグラフィックハンドルを作成する
	//extern	int			LoadBlendGraph(const TCHAR * FileName);																																			// 画像ファイルからブレンド用グラフィックハンドルを作成する
	//extern	int			LoadBlendGraphWithStrLen(const TCHAR * FileName, size_t FileNameLength);																													// 画像ファイルからブレンド用グラフィックハンドルを作成する

	//extern	int			CreateGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, const void *AlphaFileImage = NULL, int AlphaFileImageSize = 0, int TextureFlag = TRUE, int ReverseFlag = FALSE);																			// メモリ上の画像イメージからグラフィックハンドルを作成する
	//extern	int			ReCreateGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, int GrHandle, const void *AlphaFileImage = NULL, int AlphaFileImageSize = 0, int TextureFlag = TRUE, int ReverseFlag = FALSE);																			// メモリ上の画像イメージから既存のグラフィックハンドルにデータを転送する
	//extern	int			CreateDivGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, int AllNum, int XNum, int YNum, int   SizeX, int   SizeY, int *HandleArray, int TextureFlag = TRUE, int ReverseFlag = FALSE, const void *AlphaFileImage = NULL, int AlphaFileImageSize = 0);			// メモリ上の画像イメージから分割グラフィックハンドルを作成する
	//extern	int			CreateDivGraphFFromMem(const void *RGBFileImage, int RGBFileImageSize, int AllNum, int XNum, int YNum, float SizeX, float SizeY, int *HandleArray, int TextureFlag = TRUE, int ReverseFlag = FALSE, const void *AlphaFileImage = NULL, int AlphaFileImageSize = 0);			// メモリ上の画像イメージから分割グラフィックハンドルを作成する( float版 )
	//extern	int			ReCreateDivGraphFromMem(const void *RGBFileImage, int RGBFileImageSize, int AllNum, int XNum, int YNum, int   SizeX, int   SizeY, const int *HandleArray, int TextureFlag = TRUE, int ReverseFlag = FALSE, const void *AlphaFileImage = NULL, int AlphaFileImageSize = 0);			// メモリ上の画像イメージから既存の分割グラフィックハンドルにデータを転送する
	//extern	int			ReCreateDivGraphFFromMem(const void *RGBFileImage, int RGBFileImageSize, int AllNum, int XNum, int YNum, float SizeX, float SizeY, const int *HandleArray, int TextureFlag = TRUE, int ReverseFlag = FALSE, const void *AlphaFileImage = NULL, int AlphaFileImageSize = 0);			// メモリ上の画像イメージから既存の分割グラフィックハンドルにデータを転送する( float版 )
	//extern	int			CreateGraphFromBmp(const BITMAPINFO * RGBBmpInfo, const void *RGBBmpImage, const BITMAPINFO * AlphaBmpInfo = NULL, const void *AlphaBmpImage = NULL, int TextureFlag = TRUE, int ReverseFlag = FALSE);																			// ビットマップデータからグラフィックハンドルを作成する
	//extern	int			ReCreateGraphFromBmp(const BITMAPINFO * RGBBmpInfo, const void *RGBBmpImage, int GrHandle, const BITMAPINFO * AlphaBmpInfo = NULL, const void *AlphaBmpImage = NULL, int TextureFlag = TRUE, int ReverseFlag = FALSE);																			// ビットマップデータから既存のグラフィックハンドルにデータを転送する
	//extern	int			CreateDivGraphFromBmp(const BITMAPINFO * RGBBmpInfo, const void *RGBBmpImage, int AllNum, int XNum, int YNum, int   SizeX, int   SizeY, int *HandleArray, int TextureFlag = TRUE, int ReverseFlag = FALSE, const BITMAPINFO * AlphaBmpInfo = NULL, const void *AlphaBmpImage = NULL);	// ビットマップデータから分割グラフィックハンドルを作成する
	//extern	int			CreateDivGraphFFromBmp(const BITMAPINFO * RGBBmpInfo, const void *RGBBmpImage, int AllNum, int XNum, int YNum, float SizeX, float SizeY, int *HandleArray, int TextureFlag = TRUE, int ReverseFlag = FALSE, const BITMAPINFO * AlphaBmpInfo = NULL, const void *AlphaBmpImage = NULL);	// ビットマップデータから分割グラフィックハンドルを作成する( float版 )
	//extern	int			ReCreateDivGraphFromBmp(const BITMAPINFO * RGBBmpInfo, const void *RGBBmpImage, int AllNum, int XNum, int YNum, int   SizeX, int   SizeY, const int *HandleArray, int TextureFlag = TRUE, int ReverseFlag = FALSE, const BITMAPINFO * AlphaBmpInfo = NULL, const void *AlphaBmpImage = NULL);	// ビットマップデータから既存の分割グラフィックハンドルにデータを転送する
	//extern	int			ReCreateDivGraphFFromBmp(const BITMAPINFO * RGBBmpInfo, const void *RGBBmpImage, int AllNum, int XNum, int YNum, float SizeX, float SizeY, const int *HandleArray, int TextureFlag = TRUE, int ReverseFlag = FALSE, const BITMAPINFO * AlphaBmpInfo = NULL, const void *AlphaBmpImage = NULL);	// ビットマップデータから既存の分割グラフィックハンドルにデータを転送する( float版 )
	//extern	int			CreateDXGraph(const BASEIMAGE * RgbBaseImage, const BASEIMAGE * AlphaBaseImage, int TextureFlag);																									// 基本イメージデータからサイズを割り出し、それに合ったグラフィックハンドルを作成する
	//extern	int			CreateGraphFromGraphImage(const BASEIMAGE * RgbBaseImage, int TextureFlag = TRUE, int ReverseFlag = FALSE);																// 基本イメージデータからグラフィックハンドルを作成する
	//extern	int			CreateGraphFromGraphImage(const BASEIMAGE * RgbBaseImage, const BASEIMAGE * AlphaBaseImage, int TextureFlag = TRUE, int ReverseFlag = FALSE);																// 基本イメージデータからグラフィックハンドルを作成する
	//extern	int			ReCreateGraphFromGraphImage(const BASEIMAGE * RgbBaseImage, int GrHandle, int TextureFlag = TRUE, int ReverseFlag = FALSE);																// 基本イメージデータから既存のグラフィックハンドルにデータを転送する
	//extern	int			ReCreateGraphFromGraphImage(const BASEIMAGE * RgbBaseImage, const BASEIMAGE * AlphaBaseImage, int GrHandle, int TextureFlag = TRUE, int ReverseFlag = FALSE);																// 基本イメージデータから既存のグラフィックハンドルにデータを転送する
	//extern	int			CreateDivGraphFromGraphImage(BASEIMAGE * RgbBaseImage, int AllNum, int XNum, int YNum, int   SizeX, int   SizeY, int *HandleArray, int TextureFlag = TRUE, int ReverseFlag = FALSE);																// 基本イメージデータから分割グラフィックハンドルを作成する
	//extern	int			CreateDivGraphFFromGraphImage(BASEIMAGE * RgbBaseImage, int AllNum, int XNum, int YNum, float SizeX, float SizeY, int *HandleArray, int TextureFlag = TRUE, int ReverseFlag = FALSE);																// 基本イメージデータから分割グラフィックハンドルを作成する( float版 )
	//extern	int			CreateDivGraphFromGraphImage(BASEIMAGE * RgbBaseImage, const BASEIMAGE * AlphaBaseImage, int AllNum, int XNum, int YNum, int   SizeX, int   SizeY, int *HandleArray, int TextureFlag = TRUE, int ReverseFlag = FALSE);																// 基本イメージデータから分割グラフィックハンドルを作成する
	//extern	int			CreateDivGraphFFromGraphImage(BASEIMAGE * RgbBaseImage, const BASEIMAGE * AlphaBaseImage, int AllNum, int XNum, int YNum, float SizeX, float SizeY, int *HandleArray, int TextureFlag = TRUE, int ReverseFlag = FALSE);																// 基本イメージデータから分割グラフィックハンドルを作成する( float版 )
	//extern	int			ReCreateDivGraphFromGraphImage(BASEIMAGE * RgbBaseImage, int AllNum, int XNum, int YNum, int   SizeX, int   SizeY, const int *HandleArray, int TextureFlag = TRUE, int ReverseFlag = FALSE);																// 基本イメージデータから既存の分割グラフィックハンドルにデータを転送する
	//extern	int			ReCreateDivGraphFFromGraphImage(BASEIMAGE * RgbBaseImage, int AllNum, int XNum, int YNum, float SizeX, float SizeY, const int *HandleArray, int TextureFlag = TRUE, int ReverseFlag = FALSE);																// 基本イメージデータから既存の分割グラフィックハンドルにデータを転送する( float版 )
	//extern	int			ReCreateDivGraphFromGraphImage(BASEIMAGE * RgbBaseImage, const BASEIMAGE * AlphaBaseImage, int AllNum, int XNum, int YNum, int   SizeX, int   SizeY, const int *HandleArray, int TextureFlag = TRUE, int ReverseFlag = FALSE);																// 基本イメージデータから既存の分割グラフィックハンドルにデータを転送する
	//extern	int			ReCreateDivGraphFFromGraphImage(BASEIMAGE * RgbBaseImage, const BASEIMAGE * AlphaBaseImage, int AllNum, int XNum, int YNum, float SizeX, float SizeY, const int *HandleArray, int TextureFlag = TRUE, int ReverseFlag = FALSE);																// 基本イメージデータから既存の分割グラフィックハンドルにデータを転送する( float版 )
	//extern	int			CreateGraph(int Width, int Height, int Pitch, const void *RGBImage, const void *AlphaImage = NULL, int GrHandle = -1);																																						// メモリ上のビットマップイメージからグラフィックハンドルを作成する
	//extern	int			CreateDivGraph(int Width, int Height, int Pitch, const void *RGBImage, int AllNum, int XNum, int YNum, int   SizeX, int   SizeY, int *HandleArray, const void *AlphaImage = NULL);																								// メモリ上のビットマップイメージから分割グラフィックハンドルを作成する
	//extern	int			CreateDivGraphF(int Width, int Height, int Pitch, const void *RGBImage, int AllNum, int XNum, int YNum, float SizeX, float SizeY, int *HandleArray, const void *AlphaImage = NULL);																								// メモリ上のビットマップイメージから分割グラフィックハンドルを作成する( float版 )
	//extern	int			ReCreateGraph(int Width, int Height, int Pitch, const void *RGBImage, int GrHandle, const void *AlphaImage = NULL);																																								// メモリ上のビットマップイメージからグラフィックハンドルを再作成する
#ifndef DX_NON_SOFTIMAGE
	//extern	int			CreateBlendGraphFromSoftImage(int SIHandle);																														// ソフトウエアで扱うイメージからブレンド用画像グラフィックハンドルを作成する( -1:エラー  -1以外:ブレンド用グラフィックハンドル )
	//extern	int			CreateGraphFromSoftImage(int SIHandle);																														// ソフトウエアで扱うイメージからグラフィックハンドルを作成する( -1:エラー  -1以外:グラフィックハンドル )
	//extern	int			CreateGraphFromRectSoftImage(int SIHandle, int x, int y, int SizeX, int SizeY);																					// ソフトウエアで扱うイメージの指定の領域を使ってグラフィックハンドルを作成する( -1:エラー  -1以外:グラフィックハンドル )
	//extern	int			ReCreateGraphFromSoftImage(int SIHandle, int GrHandle);																											// ソフトウエアで扱うイメージから既存のグラフィックハンドルに画像データを転送する
	//extern	int			ReCreateGraphFromRectSoftImage(int SIHandle, int x, int y, int SizeX, int SizeY, int GrHandle);																		// ソフトウエアで扱うイメージから既存のグラフィックハンドルに画像データを転送する
	//extern	int			CreateDivGraphFromSoftImage(int SIHandle, int AllNum, int XNum, int YNum, int   SizeX, int   SizeY, int *HandleArray);											// ソフトウエアで扱うイメージから分割グラフィックハンドルを作成する
	//extern	int			CreateDivGraphFFromSoftImage(int SIHandle, int AllNum, int XNum, int YNum, float SizeX, float SizeY, int *HandleArray);											// ソフトウエアで扱うイメージから分割グラフィックハンドルを作成する( float版 )
	//extern	int			ReCreateDivGraphFromSoftImage(int SIHandle, int AllNum, int XNum, int YNum, int   SizeX, int   SizeY, const int *HandleArray);										// ソフトウエアで扱うイメージから既存の分割グラフィックハンドルにデータを転送する
	//extern	int			ReCreateDivGraphFFromSoftImage(int SIHandle, int AllNum, int XNum, int YNum, float SizeX, float SizeY, const int *HandleArray);										// ソフトウエアで扱うイメージから既存の分割グラフィックハンドルにデータを転送する( float版 )
#endif // DX_NON_SOFTIMAGE
	//extern	int			CreateGraphFromBaseImage(const BASEIMAGE * BaseImage);																											// 基本イメージデータからグラフィックハンドルを作成する
	//extern	int			CreateGraphFromRectBaseImage(const BASEIMAGE * BaseImage, int x, int y, int SizeX, int SizeY);																		// 基本イメージデータの指定の領域を使ってグラフィックハンドルを作成する
	//extern	int			ReCreateGraphFromBaseImage(const BASEIMAGE * BaseImage, int GrHandle);														// 基本イメージデータから既存のグラフィックハンドルに画像データを転送する
	//extern	int			ReCreateGraphFromRectBaseImage(const BASEIMAGE * BaseImage, int x, int y, int SizeX, int SizeY, int GrHandle);														// 基本イメージデータの指定の領域を使って既存のグラフィックハンドルに画像データを転送する
	//extern	int			CreateDivGraphFromBaseImage(BASEIMAGE * BaseImage, int AllNum, int XNum, int YNum, int   SizeX, int   SizeY, int *HandleArray);								// 基本イメージデータから分割グラフィックハンドルを作成する
	//extern	int			CreateDivGraphFFromBaseImage(BASEIMAGE * BaseImage, int AllNum, int XNum, int YNum, float SizeX, float SizeY, int *HandleArray);								// 基本イメージデータから分割グラフィックハンドルを作成する( float版 )
	//extern	int			ReCreateDivGraphFromBaseImage(BASEIMAGE * BaseImage, int AllNum, int XNum, int YNum, int   SizeX, int   SizeY, const int *HandleArray);						// 基本イメージデータから既存の分割グラフィックハンドルにデータを転送する
	//extern	int			ReCreateDivGraphFFromBaseImage(BASEIMAGE * BaseImage, int AllNum, int XNum, int YNum, float SizeX, float SizeY, const int *HandleArray);						// 基本イメージデータから既存の分割グラフィックハンドルにデータを転送する( float版 )
	//extern	int			ReloadGraph(const TCHAR * FileName, int GrHandle, int ReverseFlag = FALSE);																		// 画像ファイルからグラフィックハンドルへ画像データを転送する
	//extern	int			ReloadGraphWithStrLen(const TCHAR * FileName, size_t FileNameLength, int GrHandle, int ReverseFlag = FALSE);																		// 画像ファイルからグラフィックハンドルへ画像データを転送する
	//extern	int			ReloadDivGraph(const TCHAR * FileName, int AllNum, int XNum, int YNum, int   XSize, int   YSize, const int *HandleArray, int ReverseFlag = FALSE);	// 画像ファイルからグラフィックハンドルたちへ画像データを分割転送する
	//extern	int			ReloadDivGraphWithStrLen(const TCHAR * FileName, size_t FileNameLength, int AllNum, int XNum, int YNum, int   XSize, int   YSize, const int *HandleArray, int ReverseFlag = FALSE);	// 画像ファイルからグラフィックハンドルたちへ画像データを分割転送する
	//extern	int			ReloadDivGraphF(const TCHAR * FileName, int AllNum, int XNum, int YNum, float XSize, float YSize, const int *HandleArray, int ReverseFlag = FALSE);	// 画像ファイルからグラフィックハンドルたちへ画像データを分割転送する( float版 )
	//extern	int			ReloadDivGraphFWithStrLen(const TCHAR * FileName, size_t FileNameLength, int AllNum, int XNum, int YNum, float XSize, float YSize, const int *HandleArray, int ReverseFlag = FALSE);	// 画像ファイルからグラフィックハンドルたちへ画像データを分割転送する( float版 )
	//extern	int			ReloadReverseGraph(const TCHAR * FileName, int GrHandle);																			// ReloadGraph の画像反転処理追加版
	//extern	int			ReloadReverseGraphWithStrLen(const TCHAR * FileName, size_t FileNameLength, int GrHandle);																			// ReloadGraph の画像反転処理追加版
	//extern	int			ReloadReverseDivGraph(const TCHAR * FileName, int AllNum, int XNum, int YNum, int   XSize, int   YSize, const int *HandleArray);		// ReloadDivGraph の画像反転処理追加版
	//extern	int			ReloadReverseDivGraphWithStrLen(const TCHAR * FileName, size_t FileNameLength, int AllNum, int XNum, int YNum, int   XSize, int   YSize, const int *HandleArray);		// ReloadDivGraph の画像反転処理追加版
	//extern	int			ReloadReverseDivGraphF(const TCHAR * FileName, int AllNum, int XNum, int YNum, float XSize, float YSize, const int *HandleArray);		// ReloadDivGraph の画像反転処理追加版( float版 )
	//extern	int			ReloadReverseDivGraphFWithStrLen(const TCHAR * FileName, size_t FileNameLength, int AllNum, int XNum, int YNum, float XSize, float YSize, const int *HandleArray);		// ReloadDivGraph の画像反転処理追加版( float版 )

	// グラフィックハンドル作成時設定係関数
	//extern	int			SetGraphColorBitDepth(int ColorBitDepth);							// SetCreateGraphColorBitDepth の旧名称
	//extern 	int			GetGraphColorBitDepth(void);										// GetCreateGraphColorBitDepth の旧名称
	//extern	int			SetCreateGraphColorBitDepth(int BitDepth);								// 作成するグラフィックハンドルの色深度を設定する
	//extern	int			GetCreateGraphColorBitDepth(void);										// 作成するグラフィックハンドルの色深度を取得する
	//extern	int			SetCreateGraphChannelBitDepth(int BitDepth);								// 作成するグラフィックハンドルの１チャンネル辺りのビット深度を設定する
	//extern	int			GetCreateGraphChannelBitDepth(void);										// 作成するグラフィックハンドルの１チャンネル辺りのビット深度を取得する
	//extern	int			SetDrawValidGraphCreateFlag(int Flag);									// SetDrawScreen に引数として渡せる( 描画対象として使用できる )グラフィックハンドルを作成するかどうかを設定する( TRUE:描画可能グラフィックハンドルを作成する  FLASE:通常のグラフィックハンドルを作成する( デフォルト ) )
	//extern	int			GetDrawValidGraphCreateFlag(void);										// SetDrawScreen に引数として渡せる( 描画対象として使用できる )グラフィックハンドルを作成するかどうかを設定を取得する
	//extern	int			SetDrawValidFlagOf3DGraph(int Flag);									// SetDrawValidGraphCreateFlag の旧名称
	//extern	int			SetLeftUpColorIsTransColorFlag(int Flag);									// 画像ファイルからグラフィックハンドルを作成する際に画像左上の色を透過色として扱うかどうかを設定する( TRUE:透過色として扱う 　FALSE:透過色として扱わない( デフォルト ) )
	//extern	int			SetUsePaletteGraphFlag(int Flag);									// 読み込む画像がパレット画像の場合、パレット画像として使用できる場合はパレット画像として使用するかどうかを設定する( TRUE:パレット画像として使用できる場合はパレット画像として使用する( デフォルト )  FALSE:パレット画像として使用できる場合もパレット画像としては使用しない( 通常タイプの画像に変換して使用する ) )
	//extern	int			SetUseBlendGraphCreateFlag(int Flag);									// ブレンド処理用画像を作成するかどうか( 要は画像の赤成分をα成分として扱うかどうか )の設定を行う( TRUE:ブレンド画像として読み込む  FALSE:通常画像として読み込む( デフォルト ) )
	//extern	int			GetUseBlendGraphCreateFlag(void);										// ブレンド処理用画像を作成するかどうか( 要は画像の赤成分をα成分として扱うかどうか )の設定を取得する
	//extern	int			SetUseAlphaTestGraphCreateFlag(int Flag);									// アルファテストを使用するグラフィックハンドルを作成するかどうかを設定する( TRUE:アルファテストを使用する( デフォルト )  FALSE:アルファテストを使用しない )
	//extern	int			GetUseAlphaTestGraphCreateFlag(void);										// アルファテストを使用するグラフィックハンドルを作成するかどうかを取得する
	//extern	int			SetUseAlphaTestFlag(int Flag);									// SetUseAlphaTestGraphCreateFlag の旧名称
	//extern	int			GetUseAlphaTestFlag(void);										// GetUseAlphaTestGraphCreateFlag の旧名称
	//extern	int			SetCubeMapTextureCreateFlag(int Flag);									// キューブマップテクスチャを作成するかどうかのフラグを設定する
	//extern	int			GetCubeMapTextureCreateFlag(void);										// キューブマップテクスチャを作成するかどうかのフラグを取得する
	//extern	int			SetUseNoBlendModeParam(int Flag);									// SetDrawBlendMode 関数の第一引数に DX_BLENDMODE_NOBLEND を代入した際に、デフォルトでは第二引数は内部で２５５を指定したことになるが、その自動２５５化をしないかどうかを設定する( TRUE:しない(第二引数の値が使用される)   FALSE:する(第二引数の値は無視されて 255 が常に使用される)(デフォルト) )αチャンネル付き画像に対して描画を行う場合のみ意味がある関数
	//extern	int			SetDrawValidAlphaChannelGraphCreateFlag(int Flag);									// SetDrawScreen の引数として渡せる( 描画対象として使用できる )αチャンネル付きグラフィックハンドルを作成するかどうかを設定する( SetDrawValidGraphCreateFlag 関数で描画対象として使用できるグラフィックハンドルを作成するように設定されていないと効果ありません )( TRUE:αチャンネル付き   FALSE:αチャンネルなし( デフォルト ) )
	//extern	int			GetDrawValidAlphaChannelGraphCreateFlag(void);										// SetDrawScreen の引数として渡せる( 描画対象として使用できる )αチャンネル付きグラフィックハンドルを作成するかどうかを取得する
	//extern	int			SetDrawValidFloatTypeGraphCreateFlag(int Flag);									// SetDrawScreen の引数として渡せる( 描画対象として使用できる )ピクセルフォーマットが浮動小数点型のグラフィックハンドルを作成するかどうかを設定する( SetDrawValidGraphCreateFlag 関数で描画対象として使用できるグラフィックハンドルを作成するように設定されていないと効果ありません )、グラフィックスデバイスが浮動小数点型のピクセルフォーマットに対応していない場合はグラフィックハンドルの作成に失敗する( TRUE:浮動小数点型　　FALSE:整数型( デフォルト ) )
	//extern	int			GetDrawValidFloatTypeGraphCreateFlag(void);										// SetDrawScreen の引数として渡せる( 描画対象として使用できる )ピクセルフォーマットが浮動小数点型のグラフィックハンドルを作成するかどうかを取得する
	//extern	int			SetDrawValidGraphCreateZBufferFlag(int Flag);									// SetDrawScreen の引数として渡せる( 描画対象として使用できる )グラフィックハンドルを作成する際に専用のＺバッファも作成するかどうかを設定する( TRUE:専用のＺバッファを作成する( デフォルト )  FALSE:専用のＺバッファは作成しない )
	//extern	int			GetDrawValidGraphCreateZBufferFlag(void);										// SetDrawScreen の引数として渡せる( 描画対象として使用できる )グラフィックハンドルを作成する際に専用のＺバッファも作成するかどうかを取得する
	//extern	int			SetCreateDrawValidGraphZBufferBitDepth(int BitDepth);								// SetDrawScreen の引数として渡せる( 描画対象として使用できる )グラフィックハンドルに適用するＺバッファのビット深度を設定する( BitDepth:ビット深度( 指定可能な値は 16, 24, 32 の何れか( SetDrawValidGraphCreateFlag 関数で描画対象として使用できるグラフィックハンドルを作成するように設定されていないと効果ありません )
	//extern	int			GetCreateDrawValidGraphZBufferBitDepth(void);										// SetDrawScreen の引数として渡せる( 描画対象として使用できる )グラフィックハンドルに適用するＺバッファのビット深度を取得する
	//extern	int			SetCreateDrawValidGraphMipLevels(int MipLevels);								// SetDrawScreen の引数として渡せる( 描画対象として使用できる )グラフィックハンドルに適用するMipMapのレベルを設定する
	//extern	int			GetCreateDrawValidGraphMipLevels(void);										// SetDrawScreen の引数として渡せる( 描画対象として使用できる )グラフィックハンドルに適用するMipMapのレベルを取得する
	//extern	int			SetCreateDrawValidGraphChannelNum(int ChannelNum);								// SetDrawScreen の引数として渡せる( 描画対象として使用できる )グラフィックハンドルに適用する色のチャンネル数を設定する( ChannelNum:チャンネル数( 指定可能な値は 1, 2, 4 の何れか( SetDrawValidGraphCreateFlag 関数で描画対象として使用できるグラフィックハンドルを作成するように設定されていないと効果ありません )
	//extern	int			GetCreateDrawValidGraphChannelNum(void);										// SetDrawScreen の引数として渡せる( 描画対象として使用できる )グラフィックハンドルに適用する色のチャンネル数を取得する
	//extern	int			SetCreateDrawValidGraphMultiSample(int Samples, int Quality);					// SetDrawScreen の引数として渡せる( 描画対象として使用できる )グラフィックハンドルに適用するマルチサンプリング( アンチエイリアシング )設定を行う( Samples:マルチサンプル処理に使用するドット数( 多いほど重くなります )  Quality:マルチサンプル処理の品質 )
	//extern	int			SetDrawValidMultiSample(int Samples, int Quality);					// SetCreateDrawValidGraphMultiSample の旧名称
	//extern	int			GetMultiSampleQuality(int Samples);									// 指定のマルチサンプル数で使用できる最大クオリティ値を取得する( 戻り値がマイナスの場合は引数のサンプル数が使用できないことを示します )
	//extern	int			SetUseTransColor(int Flag);									// 透過色機能を使用するかどうかを設定する( TRUE:使用する( デフォルト )  FALSE:使用しない )
	//extern	int			SetUseTransColorGraphCreateFlag(int Flag);									// 透過色機能を使用することを前提とした画像データの読み込み処理を行うかどうかを設定する( TRUE にすると SetDrawMode( DX_DRAWMODE_BILINEAR ); をした状態で DrawGraphF 等の浮動小数点型座標を受け取る関数で小数点以下の値を指定した場合に発生する描画結果の不自然を緩和する効果があります ( デフォルトは FALSE ) )
	//extern 	int			SetUseGraphAlphaChannel(int Flag);									// SetUseAlphaChannelGraphCreateFlag の旧名称
	//extern 	int			GetUseGraphAlphaChannel(void);										// GetUseAlphaChannelGraphCreateFlag の旧名称
	//extern 	int			SetUseAlphaChannelGraphCreateFlag(int Flag);									// αチャンネル付きグラフィックハンドルを作成するかどうかを設定する( TRUE:αチャンネル付き   FALSE:αチャンネル無し )
	//extern 	int			GetUseAlphaChannelGraphCreateFlag(void);										// αチャンネル付きグラフィックハンドルを作成するかどうかを取得する( TRUE:αチャンネル付き   FALSE:αチャンネル無し )
	//extern	int			SetUseNotManageTextureFlag(int Flag);									// Direct3D の管理テクスチャ機能を使用するグラフィックハンドルを作成するかどうかを設定する( TRUE:管理機能を使用する( デフォルト )  FALSE:管理機能を使用しない )、管理機能を使用するとグラフィックスデバイスのＶＲＡＭ容量以上の画像を扱うことができる代わりにシステムメモリの使用量が増えます
	//extern	int			GetUseNotManageTextureFlag(void);										// Direct3D の管理テクスチャ機能を使用するグラフィックハンドルを作成するかどうかを取得する
	//extern	int			SetUsePlatformTextureFormat(int PlatformTextureFormat);					// 作成するグラフィックハンドルで使用する環境依存のテクスチャフォーマットを指定する( Direct3D9環境なら DX_TEXTUREFORMAT_DIRECT3D9_R8G8B8 など、0 を渡すと解除 )
	//extern	int			GetUsePlatformTextureFormat(void);										// 作成するグラフィックハンドルで使用する環境依存のテクスチャフォーマットを取得する
	//extern	int			SetTransColor(int Red, int Green, int Blue);				// 作成するグラフィックハンドルに適用する透過色を設定する( Red,Green,Blue:透過色を光の３原色で表したもの( 各色０～２５５ ) )
	//extern	int			GetTransColor(int *Red, int *Green, int *Blue);				// 作成するグラフィックハンドルに適用する透過色を取得する
	//extern	int			SetUseDivGraphFlag(int Flag);									// ２のｎ乗ではないサイズの画像を複数のテクスチャを使用してＶＲＡＭの無駄を省くかどうかを設定する( TRUE:複数のテクスチャを使用する   FALSE:なるべく一枚のテクスチャで済ます( デフォルト ) )、複数のテクスチャを使用する場合はＶＲＡＭ容量の節約ができる代わりに速度の低下やバイリニアフィルタリング描画時にテクスチャとテクスチャの境目が良く見るとわかる等の弊害があります
	//extern	int			SetUseAlphaImageLoadFlag(int Flag);									// LoadGraph などの際にファイル名の末尾に _a が付いたアルファチャンネル用の画像ファイルを追加で読み込む処理を行うかどうかを設定する( TRUE:行う( デフォルト )  FALSE:行わない )
	//extern	int			SetUseMaxTextureSize(int Size);									// 使用するテクスチャーの最大サイズを設定する( デフォルトではグラフィックスデバイスが対応している最大テクスチャーサイズ、引数に 0 を渡すとデフォルト設定になります )
	//extern	int			SetUseGraphBaseDataBackup(int Flag);									// グラフィックハンドルを作成する際に使用した画像データのバックアップをして Direct3DDevice のデバイスロスト時に使用するかどうかを設定する( TRUE:バックアップをする( デフォルト )  FALSE:バックアップをしない )、バックアップをしないとメモリの節約になりますが、復帰に掛かる時間が長くなり、メモリ上のファイルイメージからグラフィックハンドルを作成した場合は自動復帰ができないなどの弊害があります
	//extern	int			GetUseGraphBaseDataBackup(void);										// グラフィックハンドルを作成する際に使用した画像データのバックアップをして Direct3DDevice のデバイスロスト時に使用するかどうかを取得する
	//extern	int			SetUseSystemMemGraphCreateFlag(int Flag);									// ( 現在効果なし )グラフィックハンドルが持つ画像データをシステムメモリ上に作成するかどうかを設定する( TRUE:システムメモリ上に作成  FALSE:ＶＲＡＭ上に作成( デフォルト ) )
	//extern	int			GetUseSystemMemGraphCreateFlag(void);										// ( 現在効果なし )グラフィックハンドルが持つ画像データをシステムメモリ上に作成するかどうかを取得する

	// 画像情報関係関数
	//extern	const unsigned int *GetFullColorImage(int GrHandle);																// 指定のグラフィックハンドルのＡＲＧＢ８イメージを取得する( 現在動画ファイルをグラフィックハンドルで読み込んだ場合のみ使用可能 )

	//extern	int			GraphLock(int GrHandle, int *PitchBuf, void **DataPointBuf, COLORDATA **ColorDataPP = NULL, int WriteOnly = FALSE);	// グラフィックメモリ領域のロック
	//extern	int			GraphUnLock(int GrHandle);																							// グラフィックメモリ領域のロック解除

	//extern	int			SetUseGraphZBuffer(int GrHandle, int UseFlag, int BitDepth = -1);										// グラフィックハンドル専用のＺバッファを持つかどうかを設定する( GrHandle:対象となるグラフィックハンドル( 描画対象として使用可能なグラフィックハンドルのみ有効 )  UseFlag:専用のＺバッファを持つかどうか( TRUE:持つ( デフォルト )  FALSE:持たない )  BitDepth:ビット深度( 16 or 24 or 32 ) )
	//extern	int			CopyGraphZBufferImage(int DestGrHandle, int SrcGrHandle);													// グラフィックハンドルのＺバッファの状態を別のグラフィックハンドルのＺバッファにコピーする( DestGrHandle も SrcGrHandle もＺバッファを持っている描画対象にできるグラフィックハンドルで、サイズが同じであり、且つマルチサンプリング( アンチエイリアス )設定が無いことが条件 )

	//extern	int			SetDeviceLostDeleteGraphFlag(int GrHandle, int DeleteFlag);														// グラフィックスデバイスのデバイスロスト発生時に指定のグラフィックハンドルを削除するかどうかを設定する( TRUE:デバイスロスト時に削除する  FALSE:デバイスロストが発生しても削除しない )

	//extern	int			GetGraphSize(int GrHandle, int *SizeXBuf, int *SizeYBuf);										// グラフィックハンドルが持つ画像のサイズを得る
	//extern	int			GetGraphSizeF(int GrHandle, float *SizeXBuf, float *SizeYBuf);										// グラフィックハンドルが持つ画像のサイズを得る( float型 )
	library["GetGraphTextureSize"] = [](int GrHandle) {
		int SizeXBuf = 0, SizeYBuf = 0;
		auto result = GetGraphTextureSize(GrHandle, &SizeXBuf, &SizeYBuf);
		return std::tuple(result, SizeXBuf, SizeYBuf);
	};
	//extern	int			GetGraphUseBaseGraphArea(int GrHandle, int *UseX, int *UseY, int *UseSizeX, int *UseSizeY);			// LoadDivGraph や DerivationGraph で元画像の一部分を使用している場合に、指定のグラフィックハンドルが使用している元画像の範囲を取得する
	//extern	int			GetGraphMipmapCount(int GrHandle);																		// グラフィックハンドルが持つテクスチャのミップマップレベル数を取得する
	//extern	int			GetGraphFilePath(int GrHandle, TCHAR *FilePathBuffer);													// グラフィックハンドルが画像ファイルから読み込まれていた場合、その画像のファイルパスを取得する
	//extern	int			CheckDrawValidGraph(int GrHandle);																		// 指定のグラフィックハンドルが描画対象にできる( SetDrawScreen の引数に渡せる )グラフィックハンドルかどうかを取得する( 戻り値　TRUE:描画対象にできるグラフィックハンドル　FALSE:描画対象にできないグラフィックハンドル )

	//extern	const COLORDATA *GetTexColorData(int AlphaCh, int AlphaTest, int ColorBitDepth, int DrawValid = FALSE);				// カラーデータを得る
	//extern	const COLORDATA *GetTexColorData(const IMAGEFORMATDESC *Format);														// フォーマットに基づいたカラーデータを得る
	//extern	const COLORDATA *GetTexColorData(int FormatIndex);																		// 指定のフォーマットインデックスのカラーデータを得る
	//extern	int			GetMaxGraphTextureSize(int *SizeX, int *SizeY);																// グラフィックスデバイスが対応している最大テクスチャサイズを取得する
	//extern	int			GetValidRestoreShredPoint(void);																				// グラフィックハンドルの画像を復元する関数が登録されているかどうかを取得する( TRUE:登録されている  FALSE:登録されていない )
	//extern	int			GetCreateGraphColorData(COLORDATA *ColorData, IMAGEFORMATDESC *Format);										// ( 現在効果なし )これから新たにグラフィックを作成する場合に使用するカラー情報を取得する

	// 画像パレット操作関係関数( ソフトウエア画像のみ使用可能 )
	//extern	int			GetGraphPalette(int GrHandle, int ColorIndex, int *Red, int *Green, int *Blue);						// グラフィックハンドルのパレットを取得する( ソフトウエアレンダリングモードで、且つパレット画像の場合のみ使用可能 )
	//extern  int			GetGraphOriginalPalette(int GrHandle, int ColorIndex, int *Red, int *Green, int *Blue);						// グラフィックハンドルの SetGraphPalette で変更する前のパレットを取得する( ソフトウエアレンダリングモードで、且つパレット画像の場合のみ使用可能 )
	//extern	int			SetGraphPalette(int GrHandle, int ColorIndex, unsigned int Color);									// グラフィックハンドルのパレットを変更する( ソフトウエアレンダリングモードで、且つパレット画像の場合のみ使用可能 )
	//extern	int			ResetGraphPalette(int GrHandle);																		// SetGraphPalette で変更したパレットを全て元に戻す( ソフトウエアレンダリングモードで、且つパレット画像の場合のみ使用可能 )

	// 図形描画関数
	library["DrawLine"] = [](int x1, int y1, int x2, int y2, sol::variadic_args va) {
		unsigned int Color = va_get(va, 0, 0xFFFFFFFFUL);
		int Thickness = va_get(va, 1, 1);
		return DrawLine(x1, y1, x2, y2, Color, Thickness);
	};
	//extern	int			DrawLineAA(float x1, float y1, float x2, float y2, unsigned int Color, float Thickness = 1.0f);							// 線を描画する( アンチエイリアス付き )
	library["DrawBox"] = [](int x1, int y1, int x2, int y2, sol::variadic_args va) {
		unsigned int Color = va_get(va, 0, 0xFFFFFFFFUL);
		int FillFlag = va_get(va, 1, true);
		return DrawBox(x1, y1, x2, y2, Color, FillFlag);
	};
	//extern	int			DrawBoxAA(float x1, float y1, float x2, float y2, unsigned int Color, int FillFlag, float LineThickness = 1.0f);			// 四角形を描画する( アンチエイリアス付き )
	//extern	int			DrawFillBox(int   x1, int   y1, int   x2, int   y2, unsigned int Color);													// 中身を塗りつぶす四角形を描画する
	//extern	int			DrawLineBox(int   x1, int   y1, int   x2, int   y2, unsigned int Color);													// 枠だけの四角形の描画 する
	library["DrawCircle"] = [](int x, int y, int r, sol::variadic_args va) {
		unsigned int Color = va_get(va, 0, 0xFFFFFFFFUL);
		int FillFlag = va_get(va, 1, true);
		int LineThickness = va_get(va, 2, 1);
		return DrawCircle(x, y, r, Color, FillFlag, LineThickness);
	};
	//extern	int			DrawCircleAA(float x, float y, float r, int posnum, unsigned int Color, int FillFlag = TRUE, float LineThickness = 1.0f);	// 円を描画する( アンチエイリアス付き )
	//extern	int			DrawOval(int   x, int   y, int   rx, int   ry, unsigned int Color, int FillFlag, int   LineThickness = 1);	// 楕円を描画する
	//extern	int			DrawOvalAA(float x, float y, float rx, float ry, int posnum, unsigned int Color, int FillFlag, float LineThickness = 1.0f);	// 楕円を描画する( アンチエイリアス付き )
	//extern	int			DrawOval_Rect(int   x1, int   y1, int   x2, int   y2, unsigned int Color, int FillFlag);										// 指定の矩形に収まる円( 楕円 )を描画する
	library["DrawTriangle"] = [](int x1, int y1, int x2, int y2, int x3, int y3, sol::variadic_args va) {
		unsigned int Color = va_get(va, 0, 0xFFFFFFFFUL);
		int FillFlag = va_get(va, 1, true);
		return DrawTriangle(x1, y1, x2, y2, x3, y3, Color, FillFlag);
	};
	//extern	int			DrawTriangleAA(float x1, float y1, float x2, float y2, float x3, float y3, unsigned int Color, int FillFlag, float LineThickness = 1.0f);			// 三角形を描画する( アンチエイリアス付き )
	//extern	int			DrawQuadrangle(int   x1, int   y1, int   x2, int   y2, int   x3, int   y3, int   x4, int   y4, unsigned int Color, int FillFlag);										// 四角形を描画する
	//extern	int			DrawQuadrangleAA(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, unsigned int Color, int FillFlag, float LineThickness = 1.0f);			// 四角形を描画する( アンチエイリアス付き )
	//extern	int			DrawRoundRect(int   x1, int   y1, int   x2, int   y2, int   rx, int   ry, unsigned int Color, int FillFlag);										// 角の丸い四角形を描画する
	//extern	int			DrawRoundRectAA(float x1, float y1, float x2, float y2, float rx, float ry, int posnum, unsigned int Color, int FillFlag, float LineThickness = 1.0f);			// 角の丸い四角形を描画する( アンチエイリアス付き )
	//extern	int			BeginAADraw(void);																																					// DrawTriangleAA などのアンチエイリアス付き図形描画の準備を行う
	//extern	int			EndAADraw(void);																																					// DrawTriangleAA などのアンチエイリアス付き図形描画の後始末を行う
	library["DrawPixel"] = [](int x, int y, sol::variadic_args va) {
		unsigned int Color = va_get(va, 0, 0xFFFFFFFFUL);
		return DrawPixel(x, y, Color);
	};

	//extern	int			Paint(int x, int y, unsigned int FillColor, ULONGLONG BoundaryColor = ULL_PARAM(0xffffffffffffffff));								// 指定点から境界色があるところまで塗りつぶす(境界色を -1 にすると指定点の色の領域を塗りつぶす)

	//extern 	int			DrawPixelSet(const POINTDATA *PointDataArray, int Num);																					// 点の集合を描画する
	//extern	int			DrawLineSet(const LINEDATA *LineDataArray, int Num);																					// 線の集合を描画する

	//extern	int			DrawPixel3D(VECTOR   Pos, unsigned int Color);							// ３Ｄの点を描画する
	//extern	int			DrawPixel3DD(VECTOR_D Pos, unsigned int Color);							// ３Ｄの点を描画する
	//extern	int			DrawLine3D(VECTOR   Pos1, VECTOR   Pos2, unsigned int Color);							// ３Ｄの線分を描画する
	//extern	int			DrawLine3DD(VECTOR_D Pos1, VECTOR_D Pos2, unsigned int Color);							// ３Ｄの線分を描画する
	//extern	int			DrawTriangle3D(VECTOR   Pos1, VECTOR   Pos2, VECTOR   Pos3, unsigned int Color, int FillFlag);				// ３Ｄの三角形を描画する
	//extern	int			DrawTriangle3DD(VECTOR_D Pos1, VECTOR_D Pos2, VECTOR_D Pos3, unsigned int Color, int FillFlag);				// ３Ｄの三角形を描画する
	//extern	int			DrawCube3D(VECTOR   Pos1, VECTOR   Pos2, unsigned int DifColor, unsigned int SpcColor, int FillFlag);		// ３Ｄの立方体を描画する
	//extern	int			DrawCube3DD(VECTOR_D Pos1, VECTOR_D Pos2, unsigned int DifColor, unsigned int SpcColor, int FillFlag);		// ３Ｄの立方体を描画する
	//extern	int			DrawSphere3D(VECTOR   CenterPos, float  r, int DivNum, unsigned int DifColor, unsigned int SpcColor, int FillFlag);		// ３Ｄの球体を描画する
	//extern	int			DrawSphere3DD(VECTOR_D CenterPos, double r, int DivNum, unsigned int DifColor, unsigned int SpcColor, int FillFlag);		// ３Ｄの球体を描画する
	//extern	int			DrawCapsule3D(VECTOR   Pos1, VECTOR   Pos2, float  r, int DivNum, unsigned int DifColor, unsigned int SpcColor, int FillFlag);		// ３Ｄのカプセルを描画する
	//extern	int			DrawCapsule3DD(VECTOR_D Pos1, VECTOR_D Pos2, double r, int DivNum, unsigned int DifColor, unsigned int SpcColor, int FillFlag);		// ３Ｄのカプセルを描画する
	//extern	int			DrawCone3D(VECTOR   TopPos, VECTOR   BottomPos, float  r, int DivNum, unsigned int DifColor, unsigned int SpcColor, int FillFlag);		// ３Ｄの円錐を描画する
	//extern	int			DrawCone3DD(VECTOR_D TopPos, VECTOR_D BottomPos, double r, int DivNum, unsigned int DifColor, unsigned int SpcColor, int FillFlag);		// ３Ｄの円錐を描画する

	// 画像描画関数
	//extern	int			LoadGraphScreen(int x, int y, const TCHAR *GraphName, int TransFlag);										// 画像ファイルを読みこんで画面に描画する
	//extern	int			LoadGraphScreenWithStrLen(int x, int y, const TCHAR *GraphName, size_t GraphNameLength, int TransFlag);										// 画像ファイルを読みこんで画面に描画する

	library["DrawGraph"] = [](float xf, float yf, int GrHandle, bool TransFlag) {
		return DrawGraph(xf, yf, GrHandle, TransFlag ? TRUE : FALSE);
	};
	library["DrawExtendGraph"] = [](int x1, int y1, int x2, int y2, int GrHandle, bool TransFlag) {
		return DrawExtendGraph(x1, y1, x2, y2, GrHandle, TransFlag ? TRUE : FALSE);
	};
	library["DrawRotaGraph"] = [](int x, int y, double ExRate, double Angle, int GrHandle, bool TransFlag, sol::variadic_args va) {
		int ReverseXFlag = va_get(va, 0, false);
		int ReverseYFlag = va_get(va, 1, false);
		return DrawRotaGraph(x, y, ExRate, Angle, GrHandle, TransFlag ? TRUE : FALSE, ReverseXFlag, ReverseYFlag);
	};
	//extern	int			DrawRotaGraph2(int x, int y, int cx, int cy, double ExtRate, double Angle, int GrHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);	// 画像の回転描画２( 回転中心指定付き )
	//extern	int			DrawRotaGraph3(int x, int y, int cx, int cy, double ExtRateX, double ExtRateY, double Angle, int GrHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE); 	// 画像の回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )
	//extern	int			DrawRotaGraphFast(int x, int y, float  ExRate, float  Angle, int GrHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);	// 画像の回転描画( 高速版、座標計算のアルゴリズムが簡略化されています、描画結果に不都合が無ければこちらの方が高速です )
	//extern	int			DrawRotaGraphFast2(int x, int y, int cx, int cy, float  ExtRate, float  Angle, int GrHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);	// 画像の回転描画２( 回転中心指定付き )( 高速版、座標計算のアルゴリズムが簡略化されています、描画結果に不都合が無ければこちらの方が高速です )
	//extern	int			DrawRotaGraphFast3(int x, int y, int cx, int cy, float  ExtRateX, float  ExtRateY, float  Angle, int GrHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE); 	// 画像の回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )( 高速版、座標計算のアルゴリズムが簡略化されています、描画結果に不都合が無ければこちらの方が高速です )
	//extern	int			DrawModiGraph(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int GrHandle, int TransFlag);															// 画像の自由変形描画
	//extern	int			DrawTurnGraph(int x, int y, int GrHandle, int TransFlag);															// 画像の左右反転描画
	//extern	int			DrawReverseGraph(int x, int y, int GrHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);	// 画像の反転描画

	//extern	int			DrawGraphF(float xf, float yf, int GrHandle, int TransFlag);															// 画像の描画( 座標指定が float 版 )
	//extern	int			DrawExtendGraphF(float x1f, float y1f, float x2f, float y2, int GrHandle, int TransFlag);															// 画像の拡大描画( 座標指定が float 版 )
	//extern	int			DrawRotaGraphF(float xf, float yf, double ExRate, double Angle, int GrHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);	// 画像の回転描画( 座標指定が float 版 )
	//extern	int			DrawRotaGraph2F(float xf, float yf, float cxf, float cyf, double ExtRate, double Angle, int GrHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);	// 画像の回転描画２( 回転中心指定付き )( 座標指定が float 版 )
	//extern	int			DrawRotaGraph3F(float xf, float yf, float cxf, float cyf, double ExtRateX, double ExtRateY, double Angle, int GrHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE); 	// 画像の回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )( 座標指定が float 版 )
	//extern	int			DrawRotaGraphFastF(float xf, float yf, float  ExRate, float  Angle, int GrHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);	// 画像の回転描画( 座標指定が float 版 )( 高速版、座標計算のアルゴリズムが簡略化されています、描画結果に不都合が無ければこちらの方が高速です )
	//extern	int			DrawRotaGraphFast2F(float xf, float yf, float cxf, float cyf, float  ExtRate, float  Angle, int GrHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);	// 画像の回転描画２( 回転中心指定付き )( 座標指定が float 版 )( 高速版、座標計算のアルゴリズムが簡略化されています、描画結果に不都合が無ければこちらの方が高速です )
	//extern	int			DrawRotaGraphFast3F(float xf, float yf, float cxf, float cyf, float  ExtRateX, float  ExtRateY, float  Angle, int GrHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE); 	// 画像の回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )( 座標指定が float 版 )( 高速版、座標計算のアルゴリズムが簡略化されています、描画結果に不都合が無ければこちらの方が高速です )
	//extern	int			DrawModiGraphF(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int GrHandle, int TransFlag);															// 画像の自由変形描画( 座標指定が float 版 )
	//extern	int			DrawTurnGraphF(float xf, float yf, int GrHandle, int TransFlag);															// 画像の左右反転描画( 座標指定が float 版 )
	//extern	int			DrawReverseGraphF(float xf, float yf, int GrHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);	// 画像の反転描画( 座標指定が float 版 )

	//extern	int			DrawChipMap(int Sx, int Sy, int XNum, int YNum, const int *MapData, int ChipTypeNum, int MapDataPitch, const int *ChipGrHandle, int TransFlag);																											// チップ画像を使った２Ｄマップ描画
	//extern	int			DrawChipMap(int MapWidth, int MapHeight, const int *MapData, int ChipTypeNum, const int *ChipGrHandle, int TransFlag, int MapDrawPointX, int MapDrawPointY, int MapDrawWidth, int MapDrawHeight, int ScreenX, int ScreenY);	// チップ画像を使った２Ｄマップ描画
	//extern	int			DrawTile(int x1, int y1, int x2, int y2, int Tx, int Ty, double ExtRate, double Angle, int GrHandle, int TransFlag);																																	// 画像を指定領域にタイル状に描画する

	//extern	int			DrawRectGraph(int DestX, int DestY, int SrcX, int SrcY, int    Width, int    Height, int GraphHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);				// 画像の指定矩形部分のみを等倍描画
	//extern	int			DrawRectExtendGraph(int DestX1, int DestY1, int DestX2, int DestY2, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int GraphHandle, int TransFlag);																	// 画像の指定矩形部分のみを拡大描画
	//extern	int			DrawRectRotaGraph(int x, int y, int SrcX, int SrcY, int Width, int Height, double ExtRate, double Angle, int GraphHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);				// 画像の指定矩形部分のみを回転描画
	//extern	int			DrawRectRotaGraph2(int x, int y, int SrcX, int SrcY, int Width, int Height, int cx, int cy, double ExtRate, double Angle, int GraphHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);				// 画像の指定矩形部分のみを回転描画２( 回転中心指定付き )
	//extern	int			DrawRectRotaGraph3(int x, int y, int SrcX, int SrcY, int Width, int Height, int cx, int cy, double ExtRateX, double ExtRateY, double Angle, int GraphHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);				// 画像の指定矩形部分のみを回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )
	//extern	int			DrawRectRotaGraphFast(int x, int y, int SrcX, int SrcY, int Width, int Height, float ExtRate, float Angle, int GraphHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);				// 画像の指定矩形部分のみを回転描画( 高速版、座標計算のアルゴリズムが簡略化されています、描画結果に不都合が無ければこちらの方が高速です )
	//extern	int			DrawRectRotaGraphFast2(int x, int y, int SrcX, int SrcY, int Width, int Height, int cx, int cy, float ExtRate, float Angle, int GraphHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);				// 画像の指定矩形部分のみを回転描画２( 回転中心指定付き )( 高速版、座標計算のアルゴリズムが簡略化されています、描画結果に不都合が無ければこちらの方が高速です )
	//extern	int			DrawRectRotaGraphFast3(int x, int y, int SrcX, int SrcY, int Width, int Height, int cx, int cy, float ExtRateX, float ExtRateY, float Angle, int GraphHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);				// 画像の指定矩形部分のみを回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )( 高速版、座標計算のアルゴリズムが簡略化されています、描画結果に不都合が無ければこちらの方が高速です )
	//extern	int			DrawRectModiGraph(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int SrcX, int SrcY, int Width, int Height, int GraphHandle, int TransFlag);																	// 画像の指定矩形部分のみを自由変形描画

	//extern	int			DrawRectGraphF(float DestX, float DestY, int SrcX, int SrcY, int    Width, int    Height, int GraphHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);	// 画像の指定矩形部分のみを等倍描画( 座標指定が float 版 )
	//extern	int			DrawRectExtendGraphF(float DestX1, float DestY1, float DestX2, float DestY2, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int GraphHandle, int TransFlag);															// 画像の指定矩形部分のみを拡大描画( 座標指定が float 版 )
	//extern	int			DrawRectRotaGraphF(float x, float y, int SrcX, int SrcY, int Width, int Height, double ExtRate, double Angle, int GraphHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);	// 画像の指定矩形部分のみを回転描画( 座標指定が float 版 )
	//extern	int			DrawRectRotaGraph2F(float x, float y, int SrcX, int SrcY, int Width, int Height, float cxf, float cyf, double ExtRate, double Angle, int GraphHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);	// 画像の指定矩形部分のみを回転描画２( 回転中心指定付き )( 座標指定が float 版 )
	//extern	int			DrawRectRotaGraph3F(float x, float y, int SrcX, int SrcY, int Width, int Height, float cxf, float cyf, double ExtRateX, double ExtRateY, double Angle, int GraphHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);	// 画像の指定矩形部分のみを回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )( 座標指定が float 版 )
	//extern	int			DrawRectRotaGraphFastF(float x, float y, int SrcX, int SrcY, int Width, int Height, float ExtRate, float  Angle, int GraphHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);	// 画像の指定矩形部分のみを回転描画( 座標指定が float 版 )( 高速版、座標計算のアルゴリズムが簡略化されています、描画結果に不都合が無ければこちらの方が高速です )
	//extern	int			DrawRectRotaGraphFast2F(float x, float y, int SrcX, int SrcY, int Width, int Height, float cxf, float cyf, float ExtRate, float  Angle, int GraphHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);	// 画像の指定矩形部分のみを回転描画２( 回転中心指定付き )( 座標指定が float 版 )( 高速版、座標計算のアルゴリズムが簡略化されています、描画結果に不都合が無ければこちらの方が高速です )
	//extern	int			DrawRectRotaGraphFast3F(float x, float y, int SrcX, int SrcY, int Width, int Height, float cxf, float cyf, float ExtRateX, float  ExtRateY, float  Angle, int GraphHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);	// 画像の指定矩形部分のみを回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )( 座標指定が float 版 )( 高速版、座標計算のアルゴリズムが簡略化されています、描画結果に不都合が無ければこちらの方が高速です )
	//extern	int			DrawRectModiGraphF(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int SrcX, int SrcY, int Width, int Height, int GraphHandle, int TransFlag);															// 画像の指定矩形部分のみを自由変形描画( 座標指定が float 版 )

	//extern	int			DrawBlendGraph(int x, int y, int GrHandle, int TransFlag, int BlendGraph, int BorderParam, int BorderRange);									// ブレンド画像と合成して画像を等倍描画する
	//extern	int			DrawBlendGraphPos(int x, int y, int GrHandle, int TransFlag, int bx, int by, int BlendGraph, int BorderParam, int BorderRange);									// ブレンド画像と合成して画像を等倍描画する( ブレンド画像の起点座標を指定する引数付き )

	//extern	int			DrawCircleGauge(int CenterX, int CenterY, double Percent, int GrHandle, double StartPercent = 0.0, double Scale = 1.0, int ReverseX = FALSE, int ReverseY = FALSE);										// 円グラフ的な描画を行う( GrHandle の画像の上下左右の端は透過色にしておく必要があります )

	//extern	int			DrawGraphToZBuffer(int X, int Y, int GrHandle, int WriteZMode /* DX_ZWRITE_MASK 等 */);														// Ｚバッファに対して画像の等倍描画
	//extern	int			DrawTurnGraphToZBuffer(int x, int y, int GrHandle, int WriteZMode /* DX_ZWRITE_MASK 等 */);														// Ｚバッファに対して画像の左右反転描画
	//extern	int			DrawReverseGraphToZBuffer(int x, int y, int GrHandle, int WriteZMode /* DX_ZWRITE_MASK 等 */, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);	// Ｚバッファに対して画像の反転描画
	//extern	int			DrawExtendGraphToZBuffer(int x1, int y1, int x2, int y2, int GrHandle, int WriteZMode /* DX_ZWRITE_MASK 等 */);														// Ｚバッファに対して画像の拡大描画
	//extern	int			DrawRotaGraphToZBuffer(int x, int y, double ExRate, double Angle, int GrHandle, int WriteZMode /* DX_ZWRITE_MASK 等 */, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);	// Ｚバッファに対して画像の回転描画
	//extern	int			DrawRotaGraph2ToZBuffer(int x, int y, int cx, int cy, double ExtRate, double Angle, int GrHandle, int WriteZMode /* DX_ZWRITE_MASK 等 */, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);	// Ｚバッファに対して画像の回転描画２( 回転中心指定付き )
	//extern	int			DrawRotaGraph3ToZBuffer(int x, int y, int cx, int cy, double ExtRateX, double ExtRateY, double Angle, int GrHandle, int WriteZMode /* DX_ZWRITE_MASK 等 */, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE); 	// Ｚバッファに対して画像の回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )
	//extern	int			DrawRotaGraphFastToZBuffer(int x, int y, float ExRate, float Angle, int GrHandle, int WriteZMode /* DX_ZWRITE_MASK 等 */, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);	// Ｚバッファに対して画像の回転描画( 高速版、座標計算のアルゴリズムが簡略化されています、描画結果に不都合が無ければこちらの方が高速です )
	//extern	int			DrawRotaGraphFast2ToZBuffer(int x, int y, int cx, int cy, float ExtRate, float Angle, int GrHandle, int WriteZMode /* DX_ZWRITE_MASK 等 */, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);	// Ｚバッファに対して画像の回転描画２( 回転中心指定付き )( 高速版、座標計算のアルゴリズムが簡略化されています、描画結果に不都合が無ければこちらの方が高速です )
	//extern	int			DrawRotaGraphFast3ToZBuffer(int x, int y, int cx, int cy, float ExtRateX, float ExtRateY, float Angle, int GrHandle, int WriteZMode /* DX_ZWRITE_MASK 等 */, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE); 	// Ｚバッファに対して画像の回転描画３( 回転中心指定付き＋縦横拡大率別指定版 )( 高速版、座標計算のアルゴリズムが簡略化されています、描画結果に不都合が無ければこちらの方が高速です )
	//extern	int			DrawModiGraphToZBuffer(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int GrHandle, int WriteZMode /* DX_ZWRITE_MASK 等 */);														// Ｚバッファに対して画像の自由変形描画
	//extern	int			DrawBoxToZBuffer(int x1, int y1, int x2, int y2, int FillFlag, int WriteZMode /* DX_ZWRITE_MASK 等 */);														// Ｚバッファに対して矩形の描画
	//extern	int			DrawCircleToZBuffer(int x, int y, int r, int FillFlag, int WriteZMode /* DX_ZWRITE_MASK 等 */);														// Ｚバッファに対して円の描画
	//extern	int			DrawTriangleToZBuffer(int x1, int y1, int x2, int y2, int x3, int y3, int FillFlag, int WriteZMode /* DX_ZWRITE_MASK 等 */);														// Ｚバッファに対して三角形を描画する
	//extern	int			DrawQuadrangleToZBuffer(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int FillFlag, int WriteZMode /* DX_ZWRITE_MASK 等 */);														// Ｚバッファに対して四角形を描画する
	//extern	int			DrawRoundRectToZBuffer(int x1, int y1, int x2, int y2, int rx, int ry, int FillFlag, int WriteZMode /* DX_ZWRITE_MASK 等 */);														// Ｚバッファに対して角の丸い四角形を描画する

	//extern	int			DrawPolygon(const VERTEX *VertexArray, int PolygonNum, int GrHandle, int TransFlag, int UVScaling = FALSE);		// ２Ｄポリゴンを描画する( Vertex:三角形を形成する頂点配列の先頭アドレス( 頂点の数はポリゴンの数×３ )  PolygonNum:描画するポリゴンの数  GrHandle:使用するグラフィックハンドル  TransFlag:透過色処理を行うかどうか( TRUE:行う  FALSE:行わない )  UVScaling:基本FALSEでOK )
	//extern	int			DrawPolygon2D(const VERTEX2D *VertexArray, int PolygonNum, int GrHandle, int TransFlag);							// ２Ｄポリゴンを描画する
	library["DrawPolygon3D"] = [](sol::table VertexArrayTable, int PolygonNum, int GrHandle, sol::variadic_args va) {
		auto &VertexArray = table_to_array<VERTEX3D>(VertexArrayTable);
		int TransFlag = va_get(va, 0, false);
		return DrawPolygon3D(VertexArray.data(), PolygonNum, GrHandle, TransFlag);
	};
	//extern	int			DrawPolygonIndexed2D(const VERTEX2D *VertexArray, int VertexNum, const unsigned short *IndexArray, int PolygonNum, int GrHandle, int TransFlag);							// ２Ｄポリゴンを描画する( 頂点インデックスを使用 )
	library["DrawPolygonIndexed3D"] = [](sol::table VertexArrayTable, int VertexNum, sol::table IndexArrayTable, int PolygonNum, int GrHandle, sol::variadic_args va) {
		auto &VertexArray = table_to_array<VERTEX3D>(VertexArrayTable);
		auto &IndexArray = table_to_array<unsigned short>(IndexArrayTable);
		int TransFlag = va_get(va, 0, false);
		return DrawPolygonIndexed3D(VertexArray.data(), VertexNum, IndexArray.data(), PolygonNum, GrHandle, TransFlag);
	};
	//extern	int			DrawPolygonIndexed3DBase(const VERTEX_3D *VertexArray, int VertexNum, const unsigned short *IndexArray, int IndexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST 等 */, int GrHandle, int TransFlag);							// ３Ｄポリゴンを描画する( 頂点インデックスを使用 )( 旧バージョン用 )
	//extern	int			DrawPolygon3DBase(const VERTEX_3D *VertexArray, int VertexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST 等 */, int GrHandle, int TransFlag);							// ３Ｄポリゴンを描画する( 旧バージョン用 )

	//extern	int			DrawPolygonBase(const VERTEX *VertexArray, int VertexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST 等 */, int GrHandle, int TransFlag, int UVScaling = FALSE);		// ２Ｄプリミティブを描画する
	//extern	int			DrawPrimitive2D(const VERTEX2D *VertexArray, int VertexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST 等 */, int GrHandle, int TransFlag);							// ２Ｄプリミティブを描画する
	//extern	int			DrawPrimitive3D(const VERTEX3D *VertexArray, int VertexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST 等 */, int GrHandle, int TransFlag);							// ３Ｄプリミティブを描画する
	//extern	int			DrawPrimitiveIndexed2D(const VERTEX2D *VertexArray, int VertexNum, const unsigned short *IndexArray, int IndexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST 等 */, int GrHandle, int TransFlag);							// ２Ｄプリミティブを描画する(頂点インデックス使用)
	//extern	int			DrawPrimitiveIndexed3D(const VERTEX3D *VertexArray, int VertexNum, const unsigned short *IndexArray, int IndexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST 等 */, int GrHandle, int TransFlag);							// ３Ｄプリミティブを描画する(頂点インデックス使用)

	//extern	int			DrawPolygon3D_UseVertexBuffer(int VertexBufHandle, int GrHandle, int TransFlag);		// 頂点バッファを使用して３Ｄポリゴンを描画する
	//extern	int			DrawPrimitive3D_UseVertexBuffer(int VertexBufHandle, int PrimitiveType  /* DX_PRIMTYPE_TRIANGLELIST 等 */, int GrHandle, int TransFlag);		// 頂点バッファを使用して３Ｄプリミティブを描画する
	//extern	int			DrawPrimitive3D_UseVertexBuffer2(int VertexBufHandle, int PrimitiveType  /* DX_PRIMTYPE_TRIANGLELIST 等 */, int StartVertex, int UseVertexNum, int GrHandle, int TransFlag);		// 頂点バッファを使用して３Ｄプリミティブを描画する
	//extern	int			DrawPolygonIndexed3D_UseVertexBuffer(int VertexBufHandle, int IndexBufHandle, int GrHandle, int TransFlag);		// 頂点バッファとインデックスバッファを使用して３Ｄポリゴンを描画する
	//extern	int			DrawPrimitiveIndexed3D_UseVertexBuffer(int VertexBufHandle, int IndexBufHandle, int PrimitiveType  /* DX_PRIMTYPE_TRIANGLELIST 等 */, int GrHandle, int TransFlag);		// 頂点バッファとインデックスバッファを使用して３Ｄプリミティブを描画する
	//extern	int			DrawPrimitiveIndexed3D_UseVertexBuffer2(int VertexBufHandle, int IndexBufHandle, int PrimitiveType  /* DX_PRIMTYPE_TRIANGLELIST 等 */, int BaseVertex, int StartVertex, int UseVertexNum, int StartIndex, int UseIndexNum, int GrHandle, int TransFlag);		// 頂点バッファとインデックスバッファを使用して３Ｄプリミティブを描画する

	//extern	int			DrawGraph3D(float x, float y, float z, int GrHandle, int TransFlag);																// 画像の３Ｄ描画
	//extern	int			DrawExtendGraph3D(float x, float y, float z, double ExRateX, double ExRateY, int GrHandle, int TransFlag);																// 画像の拡大３Ｄ描画
	//extern	int			DrawRotaGraph3D(float x, float y, float z, double ExRate, double Angle, int GrHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);		// 画像の回転３Ｄ描画
	//extern	int			DrawRota2Graph3D(float x, float y, float z, float cx, float cy, double ExtRateX, double ExtRateY, double Angle, int GrHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);		// 画像の回転３Ｄ描画(回転中心指定型)
	//extern	int			DrawModiBillboard3D(VECTOR Pos, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int GrHandle, int TransFlag);																// 画像の自由変形３Ｄ描画
	//extern	int			DrawBillboard3D(VECTOR Pos, float cx, float cy, float Size, float Angle, int GrHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);		// ３Ｄ空間上に画像を描画

	// 描画設定関係関数
	DXLUA_PORT(library, SetDrawMode);
	DXLUA_PORT(library, GetDrawMode);
	DXLUA_PORT(library, SetDrawBlendMode);
	library["GetDrawBlendMode"] = []() {
		int BlendMode = -1;
		int BlendParam = -1;
		auto result = GetDrawBlendMode(&BlendMode, &BlendParam);
		return std::make_tuple(result, BlendMode, BlendParam);
	};
	//extern	int			SetDrawAlphaTest(int TestMode, int TestParam);									// 描画時のアルファテストの設定を行う( TestMode:テストモード( DX_CMP_GREATER等 -1でデフォルト動作に戻す )  TestParam:描画アルファ値との比較に使用する値( 0～255 ) )
	//extern	int			GetDrawAlphaTest(int *TestMode, int *TestParam);								// 描画時のアルファテストの設定を取得する( TestMode:テストモード( DX_CMP_GREATER等 -1でデフォルト動作に戻す )  TestParam:描画アルファ値との比較に使用する値( 0～255 ) )
	//extern	int			SetBlendGraph(int BlendGraph, int BorderParam, int BorderRange);			// ( SetBlendGraphParam の BlendType = DX_BLENDGRAPHTYPE_WIPE の処理を行う旧関数 )描画処理時に描画する画像とブレンドするαチャンネル付き画像をセットする( BlendGraph を -1 でブレンド機能を無効 )
	//extern	int			SetBlendGraphParam(int BlendGraph, int BlendType, ...);							// 描画処理時に描画する画像とブレンドする画像のブレンド設定を行う、BlendGraph を -1 にすれば設定を解除、その場合 BlendType とその後ろのパラメータは無視される
	////		int			SetBlendGraphParam(					int BlendGraph, int BlendType = DX_BLENDGRAPHTYPE_NORMAL, int Ratio = ( 0( ブレンド率０％ )～255( ブレンド率１００％ ) ) ) ;
	////		int			SetBlendGraphParam(					int BlendGraph, int BlendType = DX_BLENDGRAPHTYPE_WIPE, int BorderParam = 境界位置(０～２５５), int BorderRange = 境界幅(指定できる値は１、６４、１２８、２５５の４つ) ) ;
	////		int			SetBlendGraphParam(					int BlendGraph, int BlendType = DX_BLENDGRAPHTYPE_ALPHA ) ;
	//extern	int			SetBlendGraphPosition(int x, int y);												// ブレンド画像の起点座標をセットする
	//extern	int			SetBlendGraphPositionMode(int BlendGraphPositionMode /* DX_BLENDGRAPH_POSMODE_DRAWGRAPH など */);	// ブレンド画像の適応座標モードを設定する
	//extern	int			SetDrawBright(int RedBright, int GreenBright, int BlueBright);				// 描画輝度を設定する
	//extern	int			GetDrawBright(int *Red, int *Green, int *Blue);								// 描画輝度を取得する
	//extern	int			SetWriteAlphaChannelFlag(int Flag);													// 描画先のアルファチャンネルの内容を書き換えるかを設定する( FALSE:書き換えない  TRUE:書き換える( デフォルト ) )
	//extern	int			GetWriteAlphaChannelFlag(void);														// 描画先のアルファチャンネルの内容を書き換えるかを取得する( FALSE:書き換えない  TRUE:書き換える( デフォルト ) )
	//extern	int			CheckSeparateAlphaBlendEnable(void);														// 描画先のアルファチャンネルの内容を書き換えないことができるかどうかを取得する( TRUE:書き換えないことができる  FALSE:書き換えないことができない )
	//extern	int			SetIgnoreDrawGraphColor(int EnableFlag);												// 描画する画像のＲＧＢ成分を無視するかどうかを指定する( EnableFlag:この機能を使うかどうか( TRUE:使う  FALSE:使わない( デフォルト ) ) )
	//extern	int			SetMaxAnisotropy(int MaxAnisotropy);											// 最大異方性値を設定する
	//extern	int			SetUseLarge3DPositionSupport(int UseFlag);													// ３Ｄ処理で使用する座標値が 10000000.0f などの大きな値になっても描画の崩れを小さく抑える処理を使用するかどうかを設定する、DxLib_Init の呼び出し前でのみ使用可能( TRUE:描画の崩れを抑える処理を使用する( CPU負荷が上がります )　　FALSE:描画の崩れを抑える処理は使用しない( デフォルト ) )
	//
	library["SetUseZBufferFlag"] = [](sol::variadic_args va) {
		int Flag = va_get(va, 0, false);
		return SetUseZBufferFlag(Flag);
	};
	library["SetWriteZBufferFlag"] = [](sol::variadic_args va) {
		int Flag = va_get(va, 0, false);
		return SetWriteZBufferFlag(Flag);
	};
	DXLUA_PORT(library, SetZBufferCmpType);
	DXLUA_PORT(library, SetZBias);
	library["SetUseZBuffer3D"] = [](sol::variadic_args va) {
		int Flag = va_get(va, 0, false);
		return SetUseZBuffer3D(Flag);
	};
	library["SetWriteZBuffer3D"] = [](sol::variadic_args va) {
		int Flag = va_get(va, 0, false);
		return SetWriteZBuffer3D(Flag);
	};
	DXLUA_PORT(library, SetZBufferCmpType3D);
	DXLUA_PORT(library, SetZBias3D);
	DXLUA_PORT(library, SetDrawZ);

	DXLUA_PORT(library, SetDrawArea);
	DXLUA_PORT(library, GetDrawArea);
	DXLUA_PORT(library, SetDrawAreaFull);
	DXLUA_PORT(library, SetDraw3DScale);

	//extern	int			SetRestoreShredPoint(void (*ShredPoint)(void));								// SetRestoreGraphCallback の旧名
	//extern	int			SetRestoreGraphCallback(void (*Callback)(void));									// グラフィックハンドル復元関数を登録する
	//extern	int			RunRestoreShred(void);														// グラフィック復元関数を実行する
	//extern	int			SetGraphicsDeviceRestoreCallbackFunction(void (*Callback)(void *Data), void *CallbackData);	// グラフィックスデバイスがロストから復帰した際に呼ばれるコールバック関数を設定する
	//extern	int			SetGraphicsDeviceLostCallbackFunction(void (*Callback)(void *Data), void *CallbackData);	// グラフィックスデバイスがロストから復帰する前に呼ばれるコールバック関数を設定する

	//extern	int			SetTransformTo2D(const MATRIX *Matrix);										// ２Ｄ描画に使用される変換行列を設定する
	//extern	int			SetTransformTo2DD(const MATRIX_D *Matrix);										// ２Ｄ描画に使用される変換行列を設定する
	//extern	int			ResetTransformTo2D(void);														// ２Ｄ描画用に使用する変換行列の設定を初期状態に戻す
	//extern	int			SetTransformToWorld(const MATRIX *Matrix);										// ローカル座標からワールド座標に変換するための行列を設定する
	//extern	int			SetTransformToWorldD(const MATRIX_D *Matrix);										// ローカル座標からワールド座標に変換するための行列を設定する
	//extern	int			GetTransformToWorldMatrix(MATRIX *MatBuf);										// ローカル座標からワールド座標に変換するための行列を取得する
	//extern	int			GetTransformToWorldMatrixD(MATRIX_D *MatBuf);										// ローカル座標からワールド座標に変換するための行列を取得する
	//extern	int			SetTransformToView(const MATRIX *Matrix);										// ワールド座標からビュー座標に変換するための行列を設定する
	//extern	int			SetTransformToViewD(const MATRIX_D *Matrix);										// ワールド座標からビュー座標に変換するための行列を設定する
	//extern	int			GetTransformToViewMatrix(MATRIX *MatBuf);										// ワールド座標からビュー座標に変換するための行列を取得する
	//extern	int			GetTransformToViewMatrixD(MATRIX_D *MatBuf);										// ワールド座標からビュー座標に変換するための行列を取得する
	//extern	int			SetTransformToProjection(const MATRIX *Matrix);										// ビュー座標からプロジェクション座標に変換するための行列を設定する
	//extern	int			SetTransformToProjectionD(const MATRIX_D *Matrix);										// ビュー座標からプロジェクション座標に変換するための行列を設定する
	//extern	int			GetTransformToProjectionMatrix(MATRIX *MatBuf);										// ビュー座標からプロジェクション座標に変換するための行列を取得する
	//extern	int			GetTransformToProjectionMatrixD(MATRIX_D *MatBuf);										// ビュー座標からプロジェクション座標に変換するための行列を取得する
	//extern	int			SetTransformToViewport(const MATRIX *Matrix);										// ビューポート行列を設定する
	//extern	int			SetTransformToViewportD(const MATRIX_D *Matrix);										// ビューポート行列を設定する
	//extern	int			GetTransformToViewportMatrix(MATRIX *MatBuf);											// ビューポート行列を取得する
	//extern	int			GetTransformToViewportMatrixD(MATRIX_D *MatBuf);											// ビューポート行列を取得する
	//extern	int			GetTransformToAPIViewportMatrix(MATRIX *MatBuf);											// Direct3Dで自動適用されるビューポート行列を取得する
	//extern	int			GetTransformToAPIViewportMatrixD(MATRIX_D *MatBuf);											// Direct3Dで自動適用されるビューポート行列を取得する
	//extern	int			SetDefTransformMatrix(void);														// デフォルトの変換行列を設定する
	//extern	int			GetTransformPosition(VECTOR *LocalPos, float *x, float *y);					// ローカル座標からスクリーン座標を取得する
	//extern	int			GetTransformPositionD(VECTOR_D *LocalPos, double *x, double *y);					// ローカル座標からスクリーン座標を取得する
	//extern	float		GetBillboardPixelSize(VECTOR   WorldPos, float  WorldSize);							// ワールド空間上のビルボードのサイズからスクリーンに投影した場合のピクセル単位のサイズを取得する
	//extern	double		GetBillboardPixelSizeD(VECTOR_D WorldPos, double WorldSize);							// ワールド空間上のビルボードのサイズからスクリーンに投影した場合のピクセル単位のサイズを取得する
	DXLUA_PORT_DX(library, ConvWorldPosToViewPos);
	//extern	VECTOR_D	ConvWorldPosToViewPosD(VECTOR_D WorldPos);											// ワールド座標をビュー座標に変換する
	DXLUA_PORT_DX(library, ConvWorldPosToScreenPos);
	//extern	VECTOR_D	ConvWorldPosToScreenPosD(VECTOR_D WorldPos);											// ワールド座標をスクリーン座標に変換する
	DXLUA_PORT_DX(library, ConvWorldPosToScreenPosPlusW);
	//extern	DOUBLE4		ConvWorldPosToScreenPosPlusWD(VECTOR_D WorldPos);											// ワールド座標をスクリーン座標に変換する、最後のＸＹＺ座標をＷで割る前の値を得る
	DXLUA_PORT_DX(library, ConvScreenPosToWorldPos);
	//extern	VECTOR_D	ConvScreenPosToWorldPosD(VECTOR_D ScreenPos);											// スクリーン座標をワールド座標に変換する
	DXLUA_PORT_DX(library, ConvScreenPosToWorldPos_ZLinear);
	//extern	VECTOR_D	ConvScreenPosToWorldPos_ZLinearD(VECTOR_D ScreenPos);											// スクリーン座標をワールド座標に変換する( Z座標が線形 )

	DXLUA_PORT_DX(library, SetUseBackCulling);
	DXLUA_PORT_DX(library, GetUseBackCulling);

	library["SetTextureAddressMode"] = [](int Mode, sol::variadic_args va) {
		int Stage = va_get(va, 0, -1);
		return SetTextureAddressMode(Mode, Stage);
	};
	//extern	int			SetTextureAddressModeUV(int ModeU, int ModeV, int Stage = -1);						// テクスチャアドレスモードを設定する( U と V を別々に設定する )
	//extern	int			SetTextureAddressTransform(float TransU, float TransV, float ScaleU, float ScaleV, float RotCenterU, float RotCenterV, float Rotate);	// テクスチャ座標変換パラメータを設定する
	//extern	int			SetTextureAddressTransformMatrix(MATRIX Matrix);												// テクスチャ座標変換行列を設定する
	//extern	int			ResetTextureAddressTransform(void);														// テクスチャ座標変換設定をリセットする

	//extern	int			SetFogEnable(int Flag);													// フォグを有効にするかどうかを設定する( TRUE:有効  FALSE:無効 )
	//extern	int			GetFogEnable(void);														// フォグが有効かどうかを取得する( TRUE:有効  FALSE:無効 )
	//extern	int			SetFogMode(int Mode /* DX_FOGMODE_NONE 等 */);							// フォグモードを設定する
	//extern	int			GetFogMode(void);														// フォグモードを取得する
	//extern	int			SetFogColor(int  r, int  g, int  b);										// フォグカラーを設定する
	//extern	int			GetFogColor(int *r, int *g, int *b);										// フォグカラーを取得する
	//extern	int			SetFogStartEnd(float  start, float  end);									// フォグが始まる距離と終了する距離を設定する( 0.0f ～ 1.0f )
	//extern	int			GetFogStartEnd(float *start, float *end);									// フォグが始まる距離と終了する距離を取得する( 0.0f ～ 1.0f )
	//extern	int			SetFogDensity(float density);												// フォグの密度を設定する( 0.0f ～ 1.0f )
	//extern	float		GetFogDensity(void);														// フォグの密度を取得する( 0.0f ～ 1.0f )

	// 画面関係関数
	DXLUA_PORT_DX(library, GetPixel);
	DXLUA_PORT(library, GetPixelF);
	library["SetBackgroundColor"] = [](int Red, int Green, int Blue, sol::variadic_args va) {
		int Alpha = va_get(va, 0, 0);
		return SetBackgroundColor(Red, Green, Blue, Alpha);
	};
	library["GetBackgroundColor"] = []() {
		int Red, Green, Blue, Alpha;
		auto Result = GetBackgroundColor(&Red, &Green, &Blue, &Alpha);
		return std::tuple(Result, Red, Green, Blue, Alpha);
	};
	//extern	int				GetDrawScreenGraph(int x1, int y1, int x2, int y2, int GrHandle, int UseClientFlag = TRUE);	// 描画先の画面から指定領域の画像情報をグラフィックハンドルに転送する
	//extern	int				BltDrawValidGraph(int TargetDrawValidGrHandle, int x1, int y1, int x2, int y2, int DestX, int DestY, int DestGrHandle);							// SetDrawScreen で描画対象にできるグラフィックハンドルから指定領域の画像情報を別のグラフィックハンドルに転送する
	DXLUA_PORT(library, ScreenFlip);
	DXLUA_PORT(library, ScreenCopy);
	DXLUA_PORT(library, WaitVSync);
	library["ClearDrawScreen"] = [](sol::variadic_args va) {
		// TODO: ClearDrawScreen(const RECT * ClearRect = NULL);
		return ClearDrawScreen();
	};
	//extern	int				ClearDrawScreenZBuffer(const RECT *ClearRect = NULL);												// 画面のＺバッファをクリアする
	//extern	int				ClsDrawScreen(void);																		// ClearDrawScreenの旧名称
	DXLUA_PORT(library, SetDrawScreen);
	DXLUA_PORT(library, GetDrawScreen);
	//extern	int				GetActiveGraph(void);																		// GetDrawScreen の旧名称
	//extern	int				SetUseSetDrawScreenSettingReset(int UseFlag);																	// SetDrawScreen を実行した際にカメラや描画範囲の設定をリセットするかを設定する( UseFlag  TRUE:リセットする( デフォルト )  FALSE:リセットしない )
	//extern	int				GetUseSetDrawScreenSettingReset(void);																		// SetDrawScreen を実行した際にカメラや描画範囲の設定をリセットするかを取得する
	//extern	int				SetDrawZBuffer(int DrawScreen);																// 描画先Ｚバッファのセット( DrawScreen 付属のＺバッファを描画先Ｚバッファにする、DrawScreen を -1 にするとデフォルトの描画先Ｚバッファに戻る )
	library["SetGraphMode"] = [](int ScreenSizeX, int ScreenSizeY, int ColorBitDepth, sol::variadic_args va) {
		int RefreshRate = va_get(va, 0, 60);
		return SetGraphMode(ScreenSizeX, ScreenSizeY, ColorBitDepth, RefreshRate);
	};
	//extern	int				SetUserScreenImage(void *Image, int PixelFormat /* DX_USER_SCREEN_PIXEL_FORMAT_R5G6B5 等 */);	// 画面のメモリイメージをセットする( DxLib_Init の前で呼ぶ必要がある( DxLib_Init の前に一度でも呼んでいれば、DxLib_Init 後は Image のアドレスのみの変更目的で呼ぶことは可能 )、PixelFormat に DX_USER_SCREEN_PIXEL_FORMAT_R5G6B5 又は DX_USER_SCREEN_PIXEL_FORMAT_X8R8G8B8 の二つ以外を指定した場合はＤＸライブラリの描画関数は一切使用できなくなります )
	//extern	int				SetFullScreenResolutionMode(int ResolutionMode /* DX_FSRESOLUTIONMODE_NATIVE 等 */);						// フルスクリーン解像度モードを設定する
	//extern	int				GetFullScreenResolutionMode(int *ResolutionMode, int *UseResolutionMode);									// フルスクリーン解像度モードを取得する( UseResolutionMode は実際に使用されている解像度モード( 例えば DX_FSRESOLUTIONMODE_NATIVE を指定していてもモニタが指定の解像度に対応していない場合は UseResolutionMode が DX_FSRESOLUTIONMODE_DESKTOP や DX_FSRESOLUTIONMODE_MAXIMUM になります ) )
	//extern	int				SetFullScreenScalingMode(int ScalingMode /* DX_FSSCALINGMODE_NEAREST 等 */, int FitScaling = FALSE);	// フルスクリーンモード時の画面拡大モードを設定する
	//extern	int				SetEmulation320x240(int Flag);																	// ６４０ｘ４８０の画面で３２０ｘ２４０の画面解像度にするかどうかを設定する、６４０ｘ４８０以外の解像度では無効( TRUE:有効  FALSE:無効 )
	//extern	int				SetZBufferSize(int ZBufferSizeX, int ZBufferSizeY);											// 画面用のＺバッファのサイズを設定する
	//extern	int				SetZBufferBitDepth(int BitDepth);																// 画面用のＺバッファのビット深度を設定する( 16 or 24 or 32 )
	library["SetWaitVSyncFlag"] = [](sol::object Flag) {
		return SetWaitVSyncFlag(Flag.as<bool>() ? TRUE : FALSE);
	};
	//extern	int				GetWaitVSyncFlag(void);																		// ScreenFlip 実行時にＶＳＹＮＣ待ちをするかどうかを取得する
	//extern	int				SetFullSceneAntiAliasingMode(int Samples, int Quality);													// 画面のフルスクリーンアンチエイリアスモードの設定を行う( DxLib_Init の前でのみ使用可能 )
	//extern	int				SetGraphDisplayArea(int x1, int y1, int x2, int y2);												// ScreenFlip 時に表画面全体に転送する裏画面の領域を設定する( DxLib_Init の前でのみ使用可能 )
	//extern	int				SetChangeScreenModeGraphicsSystemResetFlag(int Flag);																	// 画面モード変更時( とウインドウモード変更時 )にグラフィックスシステムの設定やグラフィックハンドルをリセットするかどうかを設定する( TRUE:リセットする( デフォルト )  FALSE:リセットしない )
	//extern	int				GetScreenState(int *SizeX, int *SizeY, int *ColorBitDepth);									// 現在の画面の解像度とカラービット数を得る 
	//extern	int				GetDrawScreenSize(int *XBuf, int *YBuf);														// 描画先のサイズを取得する
	//extern	int				GetScreenBitDepth(void);																		// 画面のカラービット数を取得する
	//extern	int				GetColorBitDepth(void);																		// GetScreenBitDepth の旧名称
	//extern	int				GetChangeDisplayFlag(void);																		// 画面モードが変更されているかどうかを取得する
	//extern	int				GetVideoMemorySize(int *AllSize, int *FreeSize);													// ( 現在正常に動作しません )ビデオメモリの容量を得る
	//extern	int				GetRefreshRate(void);																		// 現在の画面のリフレッシュレートを取得する
	//extern	int				GetDisplayNum(void);																		// ディスプレイの数を取得
	//extern	int				GetDisplayInfo(int DisplayIndex, int *DesktopRectX, int *DesktopRectY, int *DesktopSizeX, int *DesktopSizeY, int *IsPrimary);	// ディスプレイのデスクトップ上での矩形位置を取得する
	//extern	int				GetDisplayModeNum(int DisplayIndex = 0);														// 変更可能なディスプレイモードの数を取得する
	//extern	DISPLAYMODEDATA	GetDisplayMode(int ModeIndex, int DisplayIndex = 0);											// 変更可能なディスプレイモードの情報を取得する( ModeIndex は 0 ～ GetDisplayModeNum の戻り値-1 )
	//extern	int				GetDisplayMaxResolution(int *SizeX, int *SizeY, int DisplayIndex = 0);								// ディスプレイの最大解像度を取得する
	//extern	const COLORDATA *GetDispColorData(void);																		// ディスプレイのカラーデータアドレスを取得する
	//extern	int				GetMultiDrawScreenNum(void);																		// 同時に描画を行うことができる画面の数を取得する
	//extern	int				GetDrawFloatCoordType(void);																		// DrawGraphF 等の浮動小数点値で座標を指定する関数における座標タイプを取得する( 戻り値 : DX_DRAWFLOATCOORDTYPE_DIRECT3D9 など )

	// その他設定関係関数
	//extern	int			SetUseNormalDrawShader(int Flag);									// 通常描画にプログラマブルシェーダーを使用するかどうかを設定する( TRUE:使用する( デフォルト )  FALSE:使用しない )
	//extern	int			SetUseSoftwareRenderModeFlag(int Flag);									// ソフトウエアレンダリングモードを使用するかどうかを設定する( TRUE:使用する  FALSE:使用しない( デフォルト ) )( DxLib_Init の前に呼ぶ必要があります )
	//extern	int			SetNotUse3DFlag(int Flag);									// ( 同効果のSetUseSoftwareRenderModeFlag を使用して下さい )３Ｄ機能を使わないかどうかを設定する
	//extern	int			SetUse3DFlag(int Flag);									// ( 同効果のSetUseSoftwareRenderModeFlag を使用して下さい )３Ｄ機能を使うかどうかを設定する
	//extern	int			GetUse3DFlag(void);										// 描画に３Ｄ機能を使うかどうかを取得する
	//extern	int			SetScreenMemToVramFlag(int Flag);									// ( 同効果のSetUseSoftwareRenderModeFlag を使用して下さい )画面のピクセルデータをＶＲＡＭに置くかどうかを設定する
	//extern	int			GetScreenMemToSystemMemFlag(void);										// 画面のピクセルデータがシステムメモリ上に存在するかを取得する

	//extern	int			SetWindowDrawRect(const RECT *DrawRect);						// 通常使用しない
	//extern	int			RestoreGraphSystem(void);										// ＤＸライブラリのグラフィックス処理関連の復帰処理を行う
	//extern	int			SetUseHardwareVertexProcessing(int Flag);									// ハードウエアの頂点演算処理機能を使用するかどうかを設定する( TRUE:使用する( デフォルト )  FALSE:使用しない )( DxLib_Init の前に呼ぶ必要があります )
	//extern	int			SetUsePixelLighting(int Flag);									// ピクセル単位でライティングを行うかどうかを設定する、要 ShaderModel 3.0( TRUE:ピクセル単位のライティングを行う  FALSE:頂点単位のライティングを行う( デフォルト ) )
	//extern	int			SetUseOldDrawModiGraphCodeFlag(int Flag);									// 古いバージョンの DrawModiGraph 関数のコードを使用するかどうかを設定する
	//extern	int			SetUseVramFlag(int Flag);									// ( 現在効果なし )ＶＲＡＭを使用するかのフラグをセットする
	//extern	int			GetUseVramFlag(void);										// ( 現在効果なし )２Ｄグラフィックサーフェス作成時にシステムメモリーを使用するかのフラグ取得
	//extern	int			SetBasicBlendFlag(int Flag);									// ( 現在効果なし )簡略化ブレンド処理を行うか否かのフラグをセットする
	//extern	int			SetUseBasicGraphDraw3DDeviceMethodFlag(int Flag);									// ( 現在効果なし )単純図形の描画に３Ｄデバイスの機能を使用するかどうかを設定する
	//extern	int			SetUseDisplayIndex(int Index);									// ＤＸライブラリのウインドウを表示するディスプレイデバイスを設定する( -1 を指定するとマウスカーソルがあるディスプレイデバイスにＤＸライブラリのウインドウを表示する )
	//extern	int			RenderVertex(void);										// 頂点バッファに溜まった頂点データを描画する( 特殊用途 )

	// 描画パフォーマンス関係関数
	//extern	int			GetDrawCallCount(void);										// 前々回の ScreenFlip 呼び出しから、前回の ScreenFlip 呼び出しまでの間に行われた描画コールの回数を取得する
	//extern	float		GetFPS(void);										// フレームレート( １秒間に呼ばれる ScreenFlip の回数 )を取得する

#ifndef DX_NON_SAVEFUNCTION

	// 描画先画面保存関数
	//extern	int			SaveDrawScreen(int x1, int y1, int x2, int y2, const TCHAR *FileName, int SaveType = DX_IMAGESAVETYPE_BMP, int Jpeg_Quality = 80, int Jpeg_Sample2x1 = TRUE, int Png_CompressionLevel = -1);		// 現在描画対象になっている画面をファイルで保存する
	//extern	int			SaveDrawScreenWithStrLen(int x1, int y1, int x2, int y2, const TCHAR *FileName, size_t FileNameLength, int SaveType = DX_IMAGESAVETYPE_BMP, int Jpeg_Quality = 80, int Jpeg_Sample2x1 = TRUE, int Png_CompressionLevel = -1);		// 現在描画対象になっている画面をファイルで保存する
	//extern	int			SaveDrawScreenToBMP(int x1, int y1, int x2, int y2, const TCHAR *FileName);																																// 現在描画対象になっている画面をＢＭＰ形式で保存する
	//extern	int			SaveDrawScreenToBMPWithStrLen(int x1, int y1, int x2, int y2, const TCHAR *FileName, size_t FileNameLength);																																// 現在描画対象になっている画面をＢＭＰ形式で保存する
	//extern	int			SaveDrawScreenToDDS(int x1, int y1, int x2, int y2, const TCHAR *FileName);																																// 現在描画対象になっている画面をＤＤＳ形式で保存する
	//extern	int			SaveDrawScreenToDDSWithStrLen(int x1, int y1, int x2, int y2, const TCHAR *FileName, size_t FileNameLength);																																// 現在描画対象になっている画面をＤＤＳ形式で保存する
	//extern	int			SaveDrawScreenToJPEG(int x1, int y1, int x2, int y2, const TCHAR *FileName, int Quality = 80, int Sample2x1 = TRUE);																						// 現在描画対象になっている画面をＪＰＥＧ形式で保存する Quality = 画質、値が大きいほど低圧縮高画質,0～100 
	//extern	int			SaveDrawScreenToJPEGWithStrLen(int x1, int y1, int x2, int y2, const TCHAR *FileName, size_t FileNameLength, int Quality = 80, int Sample2x1 = TRUE);																						// 現在描画対象になっている画面をＪＰＥＧ形式で保存する Quality = 画質、値が大きいほど低圧縮高画質,0～100 
	//extern	int			SaveDrawScreenToPNG(int x1, int y1, int x2, int y2, const TCHAR *FileName, int CompressionLevel = -1);																										// 現在描画対象になっている画面をＰＮＧ形式で保存する CompressionLevel = 圧縮率、値が大きいほど高圧縮率高負荷、０は無圧縮,0～9
	//extern	int			SaveDrawScreenToPNGWithStrLen(int x1, int y1, int x2, int y2, const TCHAR *FileName, size_t FileNameLength, int CompressionLevel = -1);																										// 現在描画対象になっている画面をＰＮＧ形式で保存する CompressionLevel = 圧縮率、値が大きいほど高圧縮率高負荷、０は無圧縮,0～9

	// 描画対象にできるグラフィックハンドル保存関数
	//extern	int			SaveDrawValidGraph(int GrHandle, int x1, int y1, int x2, int y2, const TCHAR *FileName, int SaveType = DX_IMAGESAVETYPE_BMP, int Jpeg_Quality = 80, int Jpeg_Sample2x1 = TRUE, int Png_CompressionLevel = -1);		// 描画対象にできるグラフィックハンドルをファイルで保存する
	//extern	int			SaveDrawValidGraphWithStrLen(int GrHandle, int x1, int y1, int x2, int y2, const TCHAR *FileName, size_t FileNameLength, int SaveType = DX_IMAGESAVETYPE_BMP, int Jpeg_Quality = 80, int Jpeg_Sample2x1 = TRUE, int Png_CompressionLevel = -1);		// 描画対象にできるグラフィックハンドルをファイルで保存する
	//extern	int			SaveDrawValidGraphToBMP(int GrHandle, int x1, int y1, int x2, int y2, const TCHAR *FileName);																																// 描画対象にできるグラフィックハンドルをＢＭＰ形式で保存する
	//extern	int			SaveDrawValidGraphToBMPWithStrLen(int GrHandle, int x1, int y1, int x2, int y2, const TCHAR *FileName, size_t FileNameLength);																																// 描画対象にできるグラフィックハンドルをＢＭＰ形式で保存する
	//extern	int			SaveDrawValidGraphToDDS(int GrHandle, int x1, int y1, int x2, int y2, const TCHAR *FileName);																																// 描画対象にできるグラフィックハンドルをＤＤＳ形式で保存する
	//extern	int			SaveDrawValidGraphToDDSWithStrLen(int GrHandle, int x1, int y1, int x2, int y2, const TCHAR *FileName, size_t FileNameLength);																																// 描画対象にできるグラフィックハンドルをＤＤＳ形式で保存する
	//extern	int			SaveDrawValidGraphToJPEG(int GrHandle, int x1, int y1, int x2, int y2, const TCHAR *FileName, int Quality = 80, int Sample2x1 = TRUE);																						// 描画対象にできるグラフィックハンドルをＪＰＥＧ形式で保存する Quality = 画質、値が大きいほど低圧縮高画質,0～100 
	//extern	int			SaveDrawValidGraphToJPEGWithStrLen(int GrHandle, int x1, int y1, int x2, int y2, const TCHAR *FileName, size_t FileNameLength, int Quality = 80, int Sample2x1 = TRUE);																						// 描画対象にできるグラフィックハンドルをＪＰＥＧ形式で保存する Quality = 画質、値が大きいほど低圧縮高画質,0～100 
	//extern	int			SaveDrawValidGraphToPNG(int GrHandle, int x1, int y1, int x2, int y2, const TCHAR *FileName, int CompressionLevel = -1);																									// 描画対象にできるグラフィックハンドルをＰＮＧ形式で保存する CompressionLevel = 圧縮率、値が大きいほど高圧縮率高負荷、０は無圧縮,0～9
	//extern	int			SaveDrawValidGraphToPNGWithStrLen(int GrHandle, int x1, int y1, int x2, int y2, const TCHAR *FileName, size_t FileNameLength, int CompressionLevel = -1);																									// 描画対象にできるグラフィックハンドルをＰＮＧ形式で保存する CompressionLevel = 圧縮率、値が大きいほど高圧縮率高負荷、０は無圧縮,0～9

#endif // DX_NON_SAVEFUNCTION

	// 頂点バッファ関係関数
	//extern	int			CreateVertexBuffer(int VertexNum, int VertexType /* DX_VERTEX_TYPE_NORMAL_3D 等 */);						// 頂点バッファを作成する( -1:エラー  0以上:頂点バッファハンドル )
	//extern	int			DeleteVertexBuffer(int VertexBufHandle);																	// 頂点バッファを削除する
	//extern	int			InitVertexBuffer(void);																				// すべての頂点バッファを削除する
	//extern	int			SetVertexBufferData(int SetIndex, const void *VertexArray, int VertexNum, int VertexBufHandle);			// 頂点バッファに頂点データを転送する
	//extern	void *GetBufferVertexBuffer(int VertexBufHandle);																	// 頂点バッファハンドルの頂点バッファのアドレスを取得する
	//extern	int			UpdateVertexBuffer(int VertexBufHandle, int UpdateStartIndex, int UpdateVertexNum);						// 頂点バッファハンドルの頂点バッファへの変更を適用する( GetBufferVertexBuffer で取得したバッファへの変更を反映する )
	//extern	int			CreateIndexBuffer(int IndexNum, int IndexType /* DX_INDEX_TYPE_16BIT 等 */);							// インデックスバッファを作成する( -1:エラー　0以上：インデックスバッファハンドル )
	//extern	int			DeleteIndexBuffer(int IndexBufHandle);																	// インデックスバッファを削除する
	//extern	int			InitIndexBuffer(void);																				// すべてのインデックスバッファを削除する
	//extern	int			SetIndexBufferData(int SetIndex, const void *IndexArray, int IndexNum, int IndexBufHandle);				// インデックスバッファにインデックスデータを転送する
	//extern	void *GetBufferIndexBuffer(int IndexBufHandle);																	// インデックスバッファハンドルのインデックスバッファのアドレスを取得する
	//extern	int			UpdateIndexBuffer(int IndexBufHandle, int UpdateStartIndex, int UpdateIndexNum);						// インデックスバッファハンドルのインデックスバッファへの変更を適用する( GetBufferIndexBuffer で取得したバッファへの変更を反映する )
	//extern	int			GetMaxPrimitiveCount(void);																				// グラフィックスデバイスが対応している一度に描画できるプリミティブの最大数を取得する
	//extern	int			GetMaxVertexIndex(void);																				// グラフィックスデバイスが対応している一度に使用することのできる最大頂点数を取得する

	// シェーダー関係関数
	//extern	int			GetValidShaderVersion(void);												// 使用できるシェーダーのバージョンを取得する( 0=使えない  200=シェーダーモデル２．０が使用可能  300=シェーダーモデル３．０が使用可能 )

	//extern	int			LoadVertexShader(const TCHAR *FileName);		// 頂点シェーダーバイナリをファイルから読み込み頂点シェーダーハンドルを作成する( 戻り値 -1:エラー  -1以外:シェーダーハンドル )
	//extern	int			LoadVertexShaderWithStrLen(const TCHAR *FileName, size_t FileNameLength);		// 頂点シェーダーバイナリをファイルから読み込み頂点シェーダーハンドルを作成する( 戻り値 -1:エラー  -1以外:シェーダーハンドル )
	//extern	int			LoadGeometryShader(const TCHAR *FileName);		// ジオメトリシェーダーバイナリをファイルから読み込みジオメトリシェーダーハンドルを作成する( 戻り値 -1:エラー  -1以外:シェーダーハンドル )
	//extern	int			LoadGeometryShaderWithStrLen(const TCHAR *FileName, size_t FileNameLength);		// ジオメトリシェーダーバイナリをファイルから読み込みジオメトリシェーダーハンドルを作成する( 戻り値 -1:エラー  -1以外:シェーダーハンドル )
	//extern	int			LoadPixelShader(const TCHAR *FileName);		// ピクセルシェーダーバイナリをファイルから読み込みピクセルシェーダーハンドルを作成する( 戻り値 -1:エラー  -1以外:シェーダーハンドル )
	//extern	int			LoadPixelShaderWithStrLen(const TCHAR *FileName, size_t FileNameLength);		// ピクセルシェーダーバイナリをファイルから読み込みピクセルシェーダーハンドルを作成する( 戻り値 -1:エラー  -1以外:シェーダーハンドル )
	//extern	int			LoadVertexShaderFromMem(const void *ImageAddress, int ImageSize);				// メモリに読み込まれた頂点シェーダーバイナリから頂点シェーダーハンドルを作成する( 戻り値 -1:エラー  -1以外:シェーダーハンドル )
	//extern	int			LoadGeometryShaderFromMem(const void *ImageAddress, int ImageSize);				// メモリに読み込まれたジオメトリシェーダーバイナリからジオメトリシェーダーハンドルを作成する( 戻り値 -1:エラー  -1以外:シェーダーハンドル )
	//extern	int			LoadPixelShaderFromMem(const void *ImageAddress, int ImageSize);				// メモリに読み込まれたピクセルシェーダーバイナリからピクセルシェーダーハンドルを作成する( 戻り値 -1:エラー  -1以外:シェーダーハンドル )
	//extern	int			DeleteShader(int ShaderHandle);									// シェーダーハンドルの削除( 頂点シェーダー・ピクセルシェーダー共通 )
	//extern	int			InitShader(void);												// シェーダーハンドルを全て削除する( 頂点シェーダーハンドル・ピクセルシェーダーハンドルどちらもすべて削除 )

	//extern	int			GetConstIndexToShader(const TCHAR *ConstantName, int ShaderHandle);	// 指定の名前を持つ定数が使用するシェーダー定数の番号を取得する
	//extern	int			GetConstIndexToShaderWithStrLen(const TCHAR *ConstantName, size_t ConstantNameLength, int ShaderHandle);	// 指定の名前を持つ定数が使用するシェーダー定数の番号を取得する
	//extern	int			GetConstCountToShader(const TCHAR *ConstantName, int ShaderHandle);	// 指定の名前を持つ定数が使用するシェーダー定数の数を取得する
	//extern	int			GetConstCountToShaderWithStrLen(const TCHAR *ConstantName, size_t ConstantNameLength, int ShaderHandle);	// 指定の名前を持つ定数が使用するシェーダー定数の数を取得する
	//extern	const FLOAT4 *GetConstDefaultParamFToShader(const TCHAR *ConstantName, int ShaderHandle);	// 指定の名前を持つ浮動小数点数定数のデフォルトパラメータが格納されているメモリアドレスを取得する
	//extern	const FLOAT4 *GetConstDefaultParamFToShaderWithStrLen(const TCHAR *ConstantName, size_t ConstantNameLength, int ShaderHandle);	// 指定の名前を持つ浮動小数点数定数のデフォルトパラメータが格納されているメモリアドレスを取得する
	//extern	int			SetVSConstSF(int ConstantIndex, float  Param);						// 頂点シェーダーの float 型定数を設定する
	//extern	int			SetVSConstF(int ConstantIndex, FLOAT4 Param);						// 頂点シェーダーの float 型定数を設定する
	//extern	int			SetVSConstFMtx(int ConstantIndex, MATRIX Param);						// 頂点シェーダーの float 型定数に行列を設定する
	//extern	int			SetVSConstFMtxT(int ConstantIndex, MATRIX Param);						// 頂点シェーダーの float 型定数に転置した行列を設定する
	//extern	int			SetVSConstSI(int ConstantIndex, int    Param);						// 頂点シェーダーの int   型定数を設定する
	//extern	int			SetVSConstI(int ConstantIndex, INT4   Param);						// 頂点シェーダーの int   型定数を設定する
	//extern	int			SetVSConstB(int ConstantIndex, BOOL   Param);						// 頂点シェーダーの BOOL  型定数を設定する
	//extern	int			SetVSConstSFArray(int ConstantIndex, const float *ParamArray, int ParamNum);	// 頂点シェーダーの float 型定数を設定する( 配列を使って連番インデックスに一度に設定 )
	//extern	int			SetVSConstFArray(int ConstantIndex, const FLOAT4 *ParamArray, int ParamNum);	// 頂点シェーダーの float 型定数を設定する( 配列を使って連番インデックスに一度に設定 )
	//extern	int			SetVSConstFMtxArray(int ConstantIndex, const MATRIX *ParamArray, int ParamNum);	// 頂点シェーダーの float 型定数に行列を設定する( 配列を使って連番インデックスに一度に設定 )
	//extern	int			SetVSConstFMtxTArray(int ConstantIndex, const MATRIX *ParamArray, int ParamNum);	// 頂点シェーダーの float 型定数に転置した行列を設定する( 配列を使って連番インデックスに一度に設定 )
	//extern	int			SetVSConstSIArray(int ConstantIndex, const int *ParamArray, int ParamNum);	// 頂点シェーダーの int   型定数を設定する( 配列を使って連番インデックスに一度に設定 )
	//extern	int			SetVSConstIArray(int ConstantIndex, const INT4 *ParamArray, int ParamNum);	// 頂点シェーダーの int   型定数を設定する( 配列を使って連番インデックスに一度に設定 )
	//extern	int			SetVSConstBArray(int ConstantIndex, const BOOL *ParamArray, int ParamNum);	// 頂点シェーダーの BOOL  型定数を設定する( 配列を使って連番インデックスに一度に設定 )
	//extern	int			ResetVSConstF(int ConstantIndex, int ParamNum);							// 頂点シェーダーの float 型定数の設定をリセットする
	//extern	int			ResetVSConstI(int ConstantIndex, int ParamNum);							// 頂点シェーダーの int   型定数の設定をリセットする
	//extern	int			ResetVSConstB(int ConstantIndex, int ParamNum);							// 頂点シェーダーの BOOL  型定数の設定をリセットする

	//extern	int			SetPSConstSF(int ConstantIndex, float  Param);						// ピクセルシェーダーの float 型定数を設定する
	//extern	int			SetPSConstF(int ConstantIndex, FLOAT4 Param);						// ピクセルシェーダーの float 型定数を設定する
	//extern	int			SetPSConstFMtx(int ConstantIndex, MATRIX Param);						// ピクセルシェーダーの float 型定数に行列を設定する
	//extern	int			SetPSConstFMtxT(int ConstantIndex, MATRIX Param);						// ピクセルシェーダーの float 型定数に転置した行列を設定する
	//extern	int			SetPSConstSI(int ConstantIndex, int    Param);						// ピクセルシェーダーの int   型定数を設定する
	//extern	int			SetPSConstI(int ConstantIndex, INT4   Param);						// ピクセルシェーダーの int   型定数を設定する
	//extern	int			SetPSConstB(int ConstantIndex, BOOL   Param);						// ピクセルシェーダーの BOOL  型定数を設定する
	//extern	int			SetPSConstSFArray(int ConstantIndex, const float *ParamArray, int ParamNum);	// ピクセルシェーダーの float 型定数を設定する( 配列を使って連番インデックスに一度に設定 )
	//extern	int			SetPSConstFArray(int ConstantIndex, const FLOAT4 *ParamArray, int ParamNum);	// ピクセルシェーダーの float 型定数を設定する( 配列を使って連番インデックスに一度に設定 )
	//extern	int			SetPSConstFMtxArray(int ConstantIndex, const MATRIX *ParamArray, int ParamNum);	// ピクセルシェーダーの float 型定数に行列を設定する( 配列を使って連番インデックスに一度に設定 )
	//extern	int			SetPSConstFMtxTArray(int ConstantIndex, const MATRIX *ParamArray, int ParamNum);	// ピクセルシェーダーの float 型定数に転置した行列を設定する( 配列を使って連番インデックスに一度に設定 )
	//extern	int			SetPSConstSIArray(int ConstantIndex, const int *ParamArray, int ParamNum);	// ピクセルシェーダーの int   型定数を設定する( 配列を使って連番インデックスに一度に設定 )
	//extern	int			SetPSConstIArray(int ConstantIndex, const INT4 *ParamArray, int ParamNum);	// ピクセルシェーダーの int   型定数を設定する( 配列を使って連番インデックスに一度に設定 )
	//extern	int			SetPSConstBArray(int ConstantIndex, const BOOL *ParamArray, int ParamNum);	// ピクセルシェーダーの BOOL  型定数を設定する( 配列を使って連番インデックスに一度に設定 )
	//extern	int			ResetPSConstF(int ConstantIndex, int ParamNum);							// ピクセルシェーダーの float 型定数の設定をリセットする
	//extern	int			ResetPSConstI(int ConstantIndex, int ParamNum);							// ピクセルシェーダーの int   型定数の設定をリセットする
	//extern	int			ResetPSConstB(int ConstantIndex, int ParamNum);							// ピクセルシェーダーの BOOL  型定数の設定をリセットする

	//extern	int			SetRenderTargetToShader(int TargetIndex, int DrawScreen, int SurfaceIndex = 0, int MipLevel = 0);	// シェーダーを使用した描画での描画先を設定する( DrawScreen に -1 を渡すと無効化 )
	//extern	int			SetUseTextureToShader(int StageIndex, int GraphHandle);						// シェーダーを使用した描画で使用するグラフィックハンドルを設定する
	//extern	int			SetUseVertexShader(int ShaderHandle);										// シェーダーを使用した描画に使用する頂点シェーダーを設定する( -1を渡すと解除 )
	//extern	int			SetUseGeometryShader(int ShaderHandle);										// シェーダーを使用した描画に使用するジオメトリシェーダーを設定する( -1を渡すと解除 )
	//extern	int			SetUsePixelShader(int ShaderHandle);										// シェーダーを使用した描画に使用するピクセルシェーダーを設定する( -1を渡すと解除 )

	//extern	int			CalcPolygonBinormalAndTangentsToShader(VERTEX3DSHADER *VertexArray, int PolygonNum);														// ポリゴンの頂点の接線と従法線をＵＶ座標から計算してセットする
	//extern	int			CalcPolygonIndexedBinormalAndTangentsToShader(VERTEX3DSHADER *VertexArray, int VertexNum, const unsigned short *IndexArray, int PolygonNum);		// ポリゴンの頂点の接線と従法線をＵＶ座標から計算してセットする( 頂点インデックスを使用する )

	//extern	int			DrawBillboard3DToShader(VECTOR Pos, float cx, float cy, float Size, float Angle, int GrHandle, int TransFlag, int ReverseXFlag = FALSE, int ReverseYFlag = FALSE);							// シェーダーを使ってビルボードを描画する
	//extern	int			DrawPolygon2DToShader(const VERTEX2DSHADER *VertexArray, int PolygonNum);																											// シェーダーを使って２Ｄポリゴンを描画する
	//extern	int			DrawPolygon3DToShader(const VERTEX3DSHADER *VertexArray, int PolygonNum);																											// シェーダーを使って３Ｄポリゴンを描画する
	//extern	int			DrawPolygonIndexed2DToShader(const VERTEX2DSHADER *VertexArray, int VertexNum, const unsigned short *IndexArray, int PolygonNum);															// シェーダーを使って２Ｄポリゴンを描画する( 頂点インデックスを使用する )
	//extern	int			DrawPolygonIndexed3DToShader(const VERTEX3DSHADER *VertexArray, int VertexNum, const unsigned short *IndexArray, int PolygonNum);															// シェーダーを使って３Ｄポリゴンを描画する( 頂点インデックスを使用する )
	//extern	int			DrawPrimitive2DToShader(const VERTEX2DSHADER *VertexArray, int VertexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST 等 */);		// シェーダーを使って２Ｄプリミティブを描画する
	//extern	int			DrawPrimitive3DToShader(const VERTEX3DSHADER *VertexArray, int VertexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST 等 */);		// シェーダーを使って３Ｄプリミティブを描画する
	//extern	int			DrawPrimitiveIndexed2DToShader(const VERTEX2DSHADER *VertexArray, int VertexNum, const unsigned short *IndexArray, int IndexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST 等 */);		// シェーダーを使って２Ｄプリミティブを描画する( 頂点インデックスを使用する )
	//extern	int			DrawPrimitiveIndexed3DToShader(const VERTEX3DSHADER *VertexArray, int VertexNum, const unsigned short *IndexArray, int IndexNum, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST 等 */);		// シェーダーを使って３Ｄプリミティブを描画する( 頂点インデックスを使用する )
	//extern	int			DrawPolygon3DToShader_UseVertexBuffer(int VertexBufHandle);																														// シェーダーを使って３Ｄポリゴンを描画する( 頂点バッファ使用版 )
	//extern	int			DrawPolygonIndexed3DToShader_UseVertexBuffer(int VertexBufHandle, int IndexBufHandle);																									// シェーダーを使って３Ｄポリゴンを描画する( 頂点バッファとインデックスバッファ使用版 )
	//extern	int			DrawPrimitive3DToShader_UseVertexBuffer(int VertexBufHandle, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST 等 */);												// シェーダーを使って３Ｄプリミティブを描画する( 頂点バッファ使用版 )
	//extern	int			DrawPrimitive3DToShader_UseVertexBuffer2(int VertexBufHandle, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST 等 */, int StartVertex, int UseVertexNum);			// シェーダーを使って３Ｄプリミティブを描画する( 頂点バッファ使用版 )
	//extern	int			DrawPrimitiveIndexed3DToShader_UseVertexBuffer(int VertexBufHandle, int IndexBufHandle, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST 等 */);												// シェーダーを使って３Ｄプリミティブを描画する( 頂点バッファとインデックスバッファ使用版 )
	//extern	int			DrawPrimitiveIndexed3DToShader_UseVertexBuffer2(int VertexBufHandle, int IndexBufHandle, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST 等 */, int BaseVertex, int StartVertex, int UseVertexNum, int StartIndex, int UseIndexNum);	// シェーダーを使って３Ｄプリミティブを描画する( 頂点バッファとインデックスバッファ使用版 )

	// シェーダー用定数バッファ関係関数
	//extern	int			InitShaderConstantBuffer(void);																					// 全てのシェーダー用定数バッファハンドルを削除する
	//extern	int			CreateShaderConstantBuffer(int BufferSize);																			// シェーダー用定数バッファハンドルを初期化する
	//extern	int			DeleteShaderConstantBuffer(int SConstBufHandle);																		// シェーダー用定数バッファハンドルを削除する
	//extern	void *GetBufferShaderConstantBuffer(int SConstBufHandle);																		// シェーダー用定数バッファハンドルの定数バッファのアドレスを取得する
	//extern	int			UpdateShaderConstantBuffer(int SConstBufHandle);																		// シェーダー用定数バッファハンドルの定数バッファへの変更を適用する
	//extern	int			SetShaderConstantBuffer(int SConstBufHandle, int TargetShader /* DX_SHADERTYPE_VERTEX など */, int Slot);		// シェーダー用定数バッファハンドルの定数バッファを指定のシェーダーの指定のスロットにセットする

	// フィルター関係関数
#ifndef DX_NON_FILTER
	library["GraphFilter"] = [](int GrHandle, int FilterType, sol::variadic_args va) {
		return ::forward_GraphFilter(FilterType, va, GraphFilter, GrHandle, FilterType);
	};
	library["GraphFilterBlt"] = [](int SrcGrHandle, int DestGrHandle, int FilterType, sol::variadic_args va) {
		return ::forward_GraphFilter(FilterType, va, GraphFilterBlt, SrcGrHandle, DestGrHandle, FilterType);
	};
	library["GraphFilterRectBlt"] = [](int SrcGrHandle, int DestGrHandle, int SrcX1, int SrcY1, int SrcX2, int SrcY2, int DestX, int DestY, int FilterType, sol::variadic_args va) {
		return ::forward_GraphFilter(FilterType, va, GraphFilterRectBlt, SrcGrHandle, DestGrHandle, SrcX1, SrcY1, SrcX2, SrcY2, DestX, DestY, FilterType);
	};
	DXLUA_PORT(library, GraphBlend);
	DXLUA_PORT(library, GraphBlendBlt);
	DXLUA_PORT(library, GraphBlendRectBlt);
#endif // DX_NON_FILTER

	// ムービーグラフィック関係関数
#ifndef DX_NON_MOVIE
	//extern	int			PlayMovie(const TCHAR * FileName, int ExRate, int PlayType);	// 動画ファイルの再生
	//extern	int			PlayMovieWithStrLen(const TCHAR * FileName, size_t FileNameLength, int ExRate, int PlayType);	// 動画ファイルの再生
	//extern	int			GetMovieImageSize_File(const TCHAR * FileName, int *SizeX, int *SizeY);	// 動画ファイルの横ピクセル数と縦ピクセル数を取得する
	//extern	int			GetMovieImageSize_File_WithStrLen(const TCHAR * FileName, size_t FileNameLength, int *SizeX, int *SizeY);	// 動画ファイルの横ピクセル数と縦ピクセル数を取得する
	//extern	int			GetMovieImageSize_Mem(const void *FileImage, int FileImageSize, int *SizeX, int *SizeY);		// メモリ上に展開された動画ファイルの横ピクセル数と縦ピクセル数を取得する
	//extern	int			OpenMovieToGraph(const TCHAR * FileName, int FullColor = TRUE);		// 動画ファイルを開く
	//extern	int			OpenMovieToGraphWithStrLen(const TCHAR * FileName, size_t FileNameLength, int FullColor = TRUE);		// 動画ファイルを開く
	//extern 	int			PlayMovieToGraph(int GraphHandle, int PlayType = DX_PLAYTYPE_BACK, int SysPlay = 0);		// 動画ファイルを使用するグラフィックハンドルの動画ファイルの再生を開始する
	//extern 	int			PauseMovieToGraph(int GraphHandle, int SysPause = 0);										// 動画ファイルを使用するグラフィックハンドルの動画ファイルの再生を停止する
	//extern	int			AddMovieFrameToGraph(int GraphHandle, unsigned int FrameNum);									// 動画ファイルの再生フレームを進める、戻すことは出来ない( 動画ファイルが停止状態で、且つ Ogg Theora のみ有効 )
	//extern	int			SeekMovieToGraph(int GraphHandle, int Time);												// 動画ファイルの再生位置を設定する(ミリ秒単位)
	//extern	int			SetPlaySpeedRateMovieToGraph(int GraphHandle, double SpeedRate);										// 動画ファイルの再生速度を設定する( 1.0 = 等倍速  2.0 = ２倍速 )、一部のファイルフォーマットのみで有効な機能です
	//extern 	int			GetMovieStateToGraph(int GraphHandle);															// 動画ファイルの再生状態を得る
	//extern	int			SetMovieVolumeToGraph(int Volume, int GraphHandle);												// 動画ファイルの音量を設定する(0～10000)
	//extern	int			ChangeMovieVolumeToGraph(int Volume, int GraphHandle);												// 動画ファイルの音量を設定する(0～255)
	//extern	const BASEIMAGE *GetMovieBaseImageToGraph(int GraphHandle, int *ImageUpdateFlag = NULL, int ImageUpdateFlagSetOnly = FALSE);	// 動画ファイルの基本イメージデータを取得する( ImageUpdateFlag に int 型変数のアドレスを渡すと、イメージが更新された場合は 1 が、更新されていない場合は 0 が格納されます、 ImageUpdateFlagSetOnly を TRUE にすると戻り値の BASEIMAGE は有効な画像データではなくなりますが、BASEIMAGE の更新処理が行われませんので、ImageUpdateFlag を利用して画像が更新されたかどうかだけをチェックしたい場合は TRUE にしてください )
	//extern	int			GetMovieTotalFrameToGraph(int GraphHandle);															// 動画ファイルの総フレーム数を得る( Ogg Theora でのみ有効 )
	//extern	int			TellMovieToGraph(int GraphHandle);															// 動画ファイルの再生位置を取得する(ミリ秒単位)
	//extern	int			TellMovieToGraphToFrame(int GraphHandle);															// 動画ファイルの再生位置を取得する(フレーム単位)
	//extern	int			SeekMovieToGraphToFrame(int GraphHandle, int Frame);												// 動画ファイルの再生位置を設定する(フレーム単位)
	//extern	LONGLONG	GetOneFrameTimeMovieToGraph(int GraphHandle);															// 動画ファイルの１フレームあたりの時間を取得する(戻り値：１フレームの時間(単位:マイクロ秒))
	//extern	int			GetLastUpdateTimeMovieToGraph(int GraphHandle);															// 動画ファイルのイメージを最後に更新した時間を得る(ミリ秒単位)
	//extern	int			SetMovieRightImageAlphaFlag(int Flag);																// 読み込む動画ファイル映像の右半分の赤成分をα情報として扱うかどうかをセットする( TRUE:α情報として扱う  FALSE:α情報として扱わない( デフォルト ) )
	//extern	int			SetMovieColorA8R8G8B8Flag(int Flag);																// 読み込む動画ファイルが32bitカラーだった場合、A8R8G8B8 形式として扱うかどうかをセットする、32bitカラーではない動画ファイルに対しては無効( Flag  TRUE:A8R8G8B8として扱う  FALSE:X8R8G8B8として扱う( デフォルト ) )
	//extern	int			SetMovieUseYUVFormatSurfaceFlag(int Flag);																// ＹＵＶフォーマットのサーフェスが使用できる場合はＹＵＶフォーマットのサーフェスを使用するかどうかを設定する( TRUE:使用する( デフォルト ) FALSE:ＲＧＢフォーマットのサーフェスを使用する )
#endif // DX_NON_MOVIE

	// カメラ関係関数
	//extern	int			SetCameraNearFar(float  Near, float  Far);												// カメラの Nearクリップ面と Farクリップ面の距離を設定する
	//extern	int			SetCameraNearFarD(double Near, double Far);												// カメラの Nearクリップ面と Farクリップ面の距離を設定する
	//extern	int			SetCameraPositionAndTarget_UpVecY(VECTOR    Position, VECTOR   Target);									// カメラの視点、注視点、アップベクトルを設定する( アップベクトルはＹ軸方向から導き出す )
	//extern	int			SetCameraPositionAndTarget_UpVecYD(VECTOR_D  Position, VECTOR_D Target);									// カメラの視点、注視点、アップベクトルを設定する( アップベクトルはＹ軸方向から導き出す )
	//extern	int			SetCameraPositionAndTargetAndUpVec(VECTOR    Position, VECTOR   TargetPosition, VECTOR   UpVector);		// カメラの視点、注視点、アップベクトルを設定する
	//extern	int			SetCameraPositionAndTargetAndUpVecD(VECTOR_D Position, VECTOR_D TargetPosition, VECTOR_D UpVector);		// カメラの視点、注視点、アップベクトルを設定する
	//extern	int			SetCameraPositionAndAngle(VECTOR   Position, float  VRotate, float  HRotate, float  TRotate);	// カメラの視点、注視点、アップベクトルを設定する( 注視点とアップベクトルは垂直回転角度、水平回転角度、捻り回転角度から導き出す )
	DXLUA_PORT(library, SetCameraPositionAndAngle);
	//extern	int			SetCameraPositionAndAngleD(VECTOR_D Position, double VRotate, double HRotate, double TRotate);	// カメラの視点、注視点、アップベクトルを設定する( 注視点とアップベクトルは垂直回転角度、水平回転角度、捻り回転角度から導き出す )
	//extern	int			SetCameraViewMatrix(MATRIX   ViewMatrix);													// ビュー行列を直接設定する
	//extern	int			SetCameraViewMatrixD(MATRIX_D ViewMatrix);													// ビュー行列を直接設定する
	//extern	int			SetCameraScreenCenter(float x, float y);													// 画面上におけるカメラが見ている映像の中心の座標を設定する
	//extern	int			SetCameraScreenCenterD(double x, double y);													// 画面上におけるカメラが見ている映像の中心の座標を設定する

	//extern	int			SetupCamera_Perspective(float  Fov);															// 遠近法カメラをセットアップする
	//extern	int			SetupCamera_PerspectiveD(double Fov);															// 遠近法カメラをセットアップする
	//extern	int			SetupCamera_Ortho(float  Size);															// 正射影カメラをセットアップする
	//extern	int			SetupCamera_OrthoD(double Size);															// 正射影カメラをセットアップする
	//extern	int			SetupCamera_ProjectionMatrix(MATRIX   ProjectionMatrix);											// 射影行列を直接設定する
	//extern	int			SetupCamera_ProjectionMatrixD(MATRIX_D ProjectionMatrix);											// 射影行列を直接設定する
	//extern	int			SetCameraDotAspect(float  DotAspect);													// カメラのドットアスペクト比を設定する
	//extern	int			SetCameraDotAspectD(double DotAspect);													// カメラのドットアスペクト比を設定する

	//extern	int			CheckCameraViewClip(VECTOR   CheckPos);													// 指定の座標がカメラの視界に入っているかどうかを判定する( 戻り値 TRUE:視界に入っていない  FALSE:視界に入っている )
	//extern	int			CheckCameraViewClipD(VECTOR_D CheckPos);													// 指定の座標がカメラの視界に入っているかどうかを判定する( 戻り値 TRUE:視界に入っていない  FALSE:視界に入っている )
	//extern	int			CheckCameraViewClip_Dir(VECTOR   CheckPos);													// 指定の座標がカメラの視界に入っているかどうかを判定する、戻り値で外れている方向も知ることができる( 戻り値 0:視界に入っている  0以外:視界に入っていない( DX_CAMERACLIP_LEFT や DX_CAMERACLIP_RIGHT が or 演算で混合されたもの、and 演算で方向を確認できる ) )
	//extern	int			CheckCameraViewClip_DirD(VECTOR_D CheckPos);													// 指定の座標がカメラの視界に入っているかどうかを判定する、戻り値で外れている方向も知ることができる( 戻り値 0:視界に入っている  0以外:視界に入っていない( DX_CAMERACLIP_LEFT や DX_CAMERACLIP_RIGHT が or 演算で混合されたもの、and 演算で方向を確認できる ) )
	//extern	int			CheckCameraViewClip_Box(VECTOR   BoxPos1, VECTOR   BoxPos2);									// 二つの座標で表されるボックスがカメラの視界に入っているかどうかを判定する( 戻り値 TRUE:視界に入っていない  FALSE:視界に入っている )
	//extern	int			CheckCameraViewClip_BoxD(VECTOR_D BoxPos1, VECTOR_D BoxPos2);									// 二つの座標で表されるボックスがカメラの視界に入っているかどうかを判定する( 戻り値 TRUE:視界に入っていない  FALSE:視界に入っている )

	//extern	float		GetCameraNear(void);																// カメラの Near クリップ面の距離を取得する
	//extern	double		GetCameraNearD(void);																// カメラの Near クリップ面の距離を取得する
	//extern	float		GetCameraFar(void);																// カメラの Far クリップ面の距離を取得する
	//extern	double		GetCameraFarD(void);																// カメラの Far クリップ面の距離を取得する

	//extern	VECTOR		GetCameraPosition(void);																// カメラの位置を取得する
	//extern	VECTOR_D	GetCameraPositionD(void);																// カメラの位置を取得する
	//extern	VECTOR		GetCameraTarget(void);																// カメラの注視点を取得する
	//extern	VECTOR_D	GetCameraTargetD(void);																// カメラの注視点を取得する
	//extern	VECTOR		GetCameraUpVector(void);																// カメラの正面方向に垂直な上方向のベクトルを取得する
	//extern	VECTOR_D	GetCameraUpVectorD(void);																// カメラの正面方向に垂直な上方向のベクトルを取得する
	//extern	VECTOR		GetCameraDownVector(void);																// カメラの正面方向に垂直な下方向のベクトルを取得する
	//extern	VECTOR_D	GetCameraDownVectorD(void);																// カメラの正面方向に垂直な下方向のベクトルを取得する
	//extern	VECTOR		GetCameraRightVector(void);																// カメラの正面方向に垂直な右方向のベクトルを取得する
	//extern	VECTOR_D	GetCameraRightVectorD(void);																// カメラの正面方向に垂直な右方向のベクトルを取得する
	//extern	VECTOR		GetCameraLeftVector(void);																// カメラの正面方向に垂直な左方向のベクトルを取得する
	//extern	VECTOR_D	GetCameraLeftVectorD(void);																// カメラの正面方向に垂直な左方向のベクトルを取得する
	//extern	VECTOR		GetCameraFrontVector(void);																// カメラの正面方向のベクトルを取得する
	//extern	VECTOR_D	GetCameraFrontVectorD(void);																// カメラの正面方向のベクトルを取得する
	//extern	VECTOR		GetCameraBackVector(void);																// カメラの後ろ方向のベクトルを取得する
	//extern	VECTOR_D	GetCameraBackVectorD(void);																// カメラの後ろ方向のベクトルを取得する
	//extern	float		GetCameraAngleHRotate(void);																// カメラの水平方向の向きを取得する
	//extern	double		GetCameraAngleHRotateD(void);																// カメラの水平方向の向きを取得する
	//extern	float		GetCameraAngleVRotate(void);																// カメラの垂直方向の向きを取得する
	//extern	double		GetCameraAngleVRotateD(void);																// カメラの垂直方向の向きを取得する
	//extern	float		GetCameraAngleTRotate(void);																// カメラの向きの捻り角度を取得する
	//extern	double		GetCameraAngleTRotateD(void);																// カメラの向きの捻り角度を取得する

	//extern	MATRIX		GetCameraViewMatrix(void);																// ビュー行列を取得する
	//extern	MATRIX_D	GetCameraViewMatrixD(void);																// ビュー行列を取得する
	//extern	MATRIX		GetCameraBillboardMatrix(void);																// ビルボード行列を取得する
	//extern	MATRIX_D	GetCameraBillboardMatrixD(void);																// ビルボード行列を取得する
	//extern	int			GetCameraScreenCenter(float *x, float *y);												// 画面上におけるカメラが見ている映像の中心の座標を取得する
	//extern	int			GetCameraScreenCenterD(double *x, double *y);												// 画面上におけるカメラが見ている映像の中心の座標を取得する
	//extern	float		GetCameraFov(void);																// カメラの視野角を取得する
	//extern	double		GetCameraFovD(void);																// カメラの視野角を取得する
	//extern	float		GetCameraSize(void);																// カメラの視野サイズを取得する
	//extern	double		GetCameraSizeD(void);																// カメラの視野サイズを取得する
	//extern	MATRIX		GetCameraProjectionMatrix(void);																// 射影行列を取得する
	//extern	MATRIX_D	GetCameraProjectionMatrixD(void);																// 射影行列を取得する
	//extern	float		GetCameraDotAspect(void);																// カメラのドットアスペクト比を得る
	//extern	double		GetCameraDotAspectD(void);																// カメラのドットアスペクト比を得る
	//extern	MATRIX		GetCameraViewportMatrix(void);																// ビューポート行列を取得する
	//extern	MATRIX_D	GetCameraViewportMatrixD(void);																// ビューポート行列を取得する
	//extern	MATRIX		GetCameraAPIViewportMatrix(void);																// Direct3Dで自動適用されるビューポート行列を取得する
	//extern	MATRIX_D	GetCameraAPIViewportMatrixD(void);																// Direct3Dで自動適用されるビューポート行列を取得する

	// ライト関係関数

	// ライト関係関数
	library["SetUseLighting"] = [](sol::variadic_args va) {
		int Flag = va_get(va, 0, false);
		return SetUseLighting(Flag);
	};
	//extern	int			SetMaterialUseVertDifColor(int UseFlag);																	// ３Ｄ描画のライティング計算で頂点カラーのディフューズカラーを使用するかどうかを設定する
	//extern	int			SetMaterialUseVertSpcColor(int UseFlag);																	// ３Ｄ描画のライティング計算で頂点カラーのスペキュラカラーを使用するかどうかを設定する
	DXLUA_PORT(library, SetMaterialParam);
	//extern	int			SetUseSpecular(int UseFlag);																	// ３Ｄ描画にスペキュラを使用するかどうかを設定する
	DXLUA_PORT(library, SetGlobalAmbientLight);

	//extern	int			ChangeLightTypeDir(VECTOR Direction);															// デフォルトライトのタイプをディレクショナルライトにする
	//extern	int			ChangeLightTypeSpot(VECTOR Position, VECTOR Direction, float OutAngle, float InAngle, float Range, float Atten0, float Atten1, float Atten2);	// デフォルトライトのタイプをスポットライトにする
	//extern	int			ChangeLightTypePoint(VECTOR Position, float Range, float Atten0, float Atten1, float Atten2);		// デフォルトライトのタイプをポイントライトにする
	DXLUA_PORT(library, GetLightType);
	//extern	int			SetLightEnable(int EnableFlag);																// デフォルトライトを使用するかどうかを設定する
	//extern	int			GetLightEnable(void);																		// デフォルトライトを使用するかどうかを取得する( 戻り値　TRUE:有効  FALSE:無効 )
	DXLUA_PORT(library, SetLightDifColor);
	DXLUA_PORT(library, GetLightDifColor);
	DXLUA_PORT(library, SetLightSpcColor);
	DXLUA_PORT(library, GetLightSpcColor);
	DXLUA_PORT(library, SetLightAmbColor);
	DXLUA_PORT(library, GetLightAmbColor);
	DXLUA_PORT(library, SetLightDirection);
	DXLUA_PORT(library, GetLightDirection);
	DXLUA_PORT(library, SetLightPosition);
	DXLUA_PORT(library, GetLightPosition);
	//extern	int			SetLightRangeAtten(float Range, float Atten0, float Atten1, float Atten2);						// デフォルトライトの距離減衰パラメータを設定する( 有効距離、距離減衰係数０、１、２ )
	//extern	int			GetLightRangeAtten(float *Range, float *Atten0, float *Atten1, float *Atten2);					// デフォルトライトの距離減衰パラメータを取得する( 有効距離、距離減衰係数０、１、２ )
	//extern	int			SetLightAngle(float OutAngle, float InAngle);												// デフォルトライトのスポットライトのパラメータを設定する( 外部コーン角度、内部コーン角度 )
	//extern	int			GetLightAngle(float *OutAngle, float *InAngle);												// デフォルトライトのスポットライトのパラメータを取得する( 外部コーン角度、内部コーン角度 )
	//extern	int			SetLightUseShadowMap(int SmSlotIndex, int UseFlag);												// デフォルトライトに SetUseShadowMap で指定したシャドウマップを適用するかどうかを設定する( SmSlotIndex:シャドウマップスロット( SetUseShadowMap の第一引数に設定する値 ) UseFlag:適用にするかどうかのフラグ( TRUE:適用する( デフォルト )  FALSE:適用しない ) )

	//extern	int			CreateDirLightHandle(VECTOR Direction);															// ディレクショナルライトハンドルを作成する
	//extern	int			CreateSpotLightHandle(VECTOR Position, VECTOR Direction, float OutAngle, float InAngle, float Range, float Atten0, float Atten1, float Atten2);	// スポットライトハンドルを作成する
	//extern	int			CreatePointLightHandle(VECTOR Position, float Range, float Atten0, float Atten1, float Atten2);		// ポイントライトハンドルを作成する
	//extern	int			DeleteLightHandle(int LHandle);																	// ライトハンドルを削除する
	//extern	int			DeleteLightHandleAll(void);																		// ライトハンドルを全て削除する
	//extern	int			SetLightTypeHandle(int LHandle, int LightType);													// ライトハンドルのライトのタイプを変更する( DX_LIGHTTYPE_DIRECTIONAL 等 )
	//extern	int			SetLightEnableHandle(int LHandle, int EnableFlag);													// ライトハンドルのライト効果の有効、無効を設定する( TRUE:有効  FALSE:無効 )
	//extern	int			SetLightDifColorHandle(int LHandle, COLOR_F Color);													// ライトハンドルのライトのディフューズカラーを設定する
	//extern	int			SetLightSpcColorHandle(int LHandle, COLOR_F Color);													// ライトハンドルのライトのスペキュラカラーを設定する
	//extern	int			SetLightAmbColorHandle(int LHandle, COLOR_F Color);													// ライトハンドルのライトのアンビエントカラーを設定する
	//extern	int			SetLightDirectionHandle(int LHandle, VECTOR Direction);												// ライトハンドルのライトの方向を設定する
	//extern	int			SetLightPositionHandle(int LHandle, VECTOR Position);												// ライトハンドルのライトの位置を設定する
	//extern	int			SetLightRangeAttenHandle(int LHandle, float Range, float Atten0, float Atten1, float Atten2);			// ライトハンドルのライトの距離減衰パラメータを設定する( 有効距離、距離減衰係数０、１、２ )
	//extern	int			SetLightAngleHandle(int LHandle, float OutAngle, float InAngle);									// ライトハンドルのライトのスポットライトのパラメータを設定する( 外部コーン角度、内部コーン角度 )
	//extern	int			SetLightUseShadowMapHandle(int LHandle, int SmSlotIndex, int UseFlag);									// ライトハンドルのライトに SetUseShadowMap で指定したシャドウマップを適用するかどうかを設定する( SmSlotIndex:シャドウマップスロット( SetUseShadowMap の第一引数に設定する値 ) UseFlag:適用にするかどうかのフラグ( TRUE:適用する( デフォルト )  FALSE:適用しない ) )
	//extern	int			GetLightTypeHandle(int LHandle);																	// ライトハンドルのライトのタイプを取得する( 戻り値は DX_LIGHTTYPE_DIRECTIONAL 等 )
	//extern	int			GetLightEnableHandle(int LHandle);																	// ライトハンドルのライト効果の有効、無効を取得する( TRUE:有効  FALSE:無効 )
	//extern	COLOR_F		GetLightDifColorHandle(int LHandle);																	// ライトハンドルのライトのディフューズカラーを取得する
	//extern	COLOR_F		GetLightSpcColorHandle(int LHandle);																	// ライトハンドルのライトのスペキュラカラーを取得する
	//extern	COLOR_F		GetLightAmbColorHandle(int LHandle);																	// ライトハンドルのライトのアンビエントカラーを取得する
	//extern	VECTOR		GetLightDirectionHandle(int LHandle);																	// ライトハンドルのライトの方向を取得する
	//extern	VECTOR		GetLightPositionHandle(int LHandle);																	// ライトハンドルのライトの位置を取得する
	//extern	int			GetLightRangeAttenHandle(int LHandle, float *Range, float *Atten0, float *Atten1, float *Atten2);		// ライトハンドルのライトの距離減衰パラメータを取得する( 有効距離、距離減衰係数０、１、２ )
	//extern	int			GetLightAngleHandle(int LHandle, float *OutAngle, float *InAngle);								// ライトハンドルのライトのスポットライトのパラメータを取得する( 外部コーン角度、内部コーン角度 )

	//extern	int			GetEnableLightHandleNum(void);																		// 有効になっているライトハンドルの数を取得する
	//extern	int			GetEnableLightHandle(int Index);																	// 有効になっているライトハンドルを取得する

	// 色情報取得用関数
	//extern	int			GetTexFormatIndex(const IMAGEFORMATDESC *Format);												// テクスチャフォーマットのインデックスを得る

	// DxMask.cpp 関数プロトタイプ宣言
#ifndef DX_NON_MASK

	// マスク関係 
	//extern	int			CreateMaskScreen(void);																						// マスクスクリーンを作成する
	//extern	int			DeleteMaskScreen(void);																						// マスクスクリーンを削除する
	//extern	int			DrawMaskToDirectData(int x, int y, int Width, int Height, const void *MaskData, int TransMode);					// マスクスクリーンにメモリ上のビットマップデータを転送する( MaskData は 1byte で 1dot 表す１ラインあたり Width byte のデータ配列の先頭アドレス )
	//extern	int			DrawFillMaskToDirectData(int x1, int y1, int x2, int y2, int Width, int Height, const void *MaskData);				// マスクスクリーンにメモリ上のビットマップデータで指定矩形内を塗りつぶすように転送する

	//extern	int			SetUseMaskScreenFlag(int ValidFlag);																				// マスクスクリーンを使用するかどうかを設定する( ValidFlag:使用するかどうか( TRUE:使用する  FALSE:使用しない( デフォルト ) )
	//extern	int			GetUseMaskScreenFlag(void);																						// マスクスクリーンを使用するかどうかを取得する
	//extern	int			FillMaskScreen(int Flag);																					// マスクスクリーンを指定の色で塗りつぶす
	//extern	int			SetMaskScreenGraph(int GraphHandle);																				// マスクスクリーンとして使用するグラフィックのハンドルを設定する、-1を渡すと解除( 引数で渡すグラフィックハンドルは MakeScreen で作成した「アルファチャンネル付きの描画対象にできるグラフィックハンドル」である必要があります( アルファチャンネルがマスクに使用されます ) )
	//extern	int			SetMaskScreenGraphUseChannel(int UseChannel /* DX_MASKGRAPH_CH_A 等 */);													// マスクスクリーンとして使用するグラフィックの、どのチャンネルをマスクとして使用するかを設定する( デフォルトは DX_MASKGRAPH_CH_A、 尚、DX_MASKGRAPH_CH_A以外を使用する場合はグラフィックスデバイスがシェーダーモデル2.0以降に対応している必要があります )

	//extern	int			InitMask(void);																						// マスクハンドルをすべて削除する
	//extern	int			MakeMask(int Width, int Height);																		// マスクハンドルを作成する
	//extern	int			GetMaskSize(int *WidthBuf, int *HeightBuf, int MaskHandle);												// マスクハンドルが持つマスクイメージのサイズを取得する
	//extern	int			GetMaskBaseImageInfo(BASEIMAGE *BaseImage, int MaskHandle);														// マスクハンドルが持つマスクイメージのBASEIMAGEデータを取得する( フォーマットが変更されるような操作をすると正常に動作しなくなります、あと、ReleaseBaseImage は実行する必要はありません )
	//extern	int			SetDataToMask(int Width, int Height, const void *MaskData, int MaskHandle);									// マスクハンドルにビットマップイメージを転送する
	//extern	int			DeleteMask(int MaskHandle);																				// マスクハンドルを削除する
	//extern	int			GraphImageBltToMask(const BASEIMAGE *BaseImage, int ImageX, int ImageY, int MaskHandle);							// マスクハンドルにBASEIMAGEデータを転送する
	//extern	int			LoadMask(const TCHAR *FileName);																			// 画像ファイルを読み込みマスクハンドルを作成する
	//extern	int			LoadMaskWithStrLen(const TCHAR *FileName, size_t FileNameLength);																			// 画像ファイルを読み込みマスクハンドルを作成する
	//extern	int			LoadDivMask(const TCHAR *FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleArray);	// 画像ファイルを分割読み込みしてマスクハンドルを作成する
	//extern	int			LoadDivMaskWithStrLen(const TCHAR *FileName, size_t FileNameLength, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleArray);	// 画像ファイルを分割読み込みしてマスクハンドルを作成する
	//extern	int			CreateMaskFromMem(const void *FileImage, int FileImageSize);																				// メモリ上にある画像ファイルイメージを読み込みマスクハンドルを作成する
	//extern	int			CreateDivMaskFromMem(const void *FileImage, int FileImageSize, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleArray);		// メモリ上にある画像ファイルイメージを分割読み込みしてマスクハンドルを作成する
	//extern	int			DrawMask(int x, int y, int MaskHandle, int TransMode);													// マスクスクリーンにマスクハンドルの持つマスクイメージを書き込む
#ifndef DX_NON_FONT
	//extern	int			DrawFormatStringMask(int x, int y, int Flag, const TCHAR *FormatString, ...);						// 書式指定ありの文字列をマスクスクリーンに描画する
	//extern	int			DrawFormatStringMaskToHandle(int x, int y, int Flag, int FontHandle, const TCHAR *FormatString, ...);						// 書式指定ありの文字列をマスクスクリーンに描画する( フォントハンドル指定版 )( SetFontCacheToTextureFlag( FALSE ) ; にして作成したフォントハンドルのみ使用可能 )
	//extern	int			DrawStringMask(int x, int y, int Flag, const TCHAR *String);									// 文字列をマスクスクリーンに描画する
	//extern	int			DrawNStringMask(int x, int y, int Flag, const TCHAR *String, size_t StringLength);			// 文字列をマスクスクリーンに描画する
	//extern	int			DrawStringMaskToHandle(int x, int y, int Flag, int FontHandle, const TCHAR *String);									// 文字列をマスクスクリーンに描画する( フォントハンドル指定版 )( SetFontCacheToTextureFlag( FALSE ) ; にして作成したフォントハンドルのみ使用可能 )
	//extern	int			DrawNStringMaskToHandle(int x, int y, int Flag, int FontHandle, const TCHAR *String, size_t StringLength);			// 文字列をマスクスクリーンに描画する( フォントハンドル指定版 )( SetFontCacheToTextureFlag( FALSE ) ; にして作成したフォントハンドルのみ使用可能 )
#endif // DX_NON_FONT
	//extern	int			DrawFillMask(int x1, int y1, int x2, int y2, int MaskHandle);												// マスクハンドルが持つマスクイメージをマスクスクリーンいっぱいに描画する( タイル状に並べる )
	//extern	int			SetMaskReverseEffectFlag(int ReverseFlag);																				// マスクイメージ内の数値に対する効果を逆転させる( 旧バージョンとの互換性の為の機能 )

	//extern 	int			GetMaskScreenData(int x1, int y1, int x2, int y2, int MaskHandle);												// マスクスクリーンの指定矩形部分をマスクハンドルに転送する
	//extern	int			GetMaskUseFlag(void);																						// マスクスクリーンを使用する設定になっているかどうかを取得する

#endif // DX_NON_MASK

#endif // DX_NOTUSE_DRAWFUNCTION
}

} // namespace DxLua::detail
