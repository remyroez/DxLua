﻿#include <DxLib.h>
#include <lua.hpp>
#include <sol/sol.hpp>

#include "luaapi.h"

namespace DxLua::detail {

void portDefine(sol::table &t)
{
	// ＤＸライブラリのバージョン
	DXLUA_PORT(t, DXLIB_VERSION)
    t["DXLIB_VERSION_STR"] = DXLIB_VERSION_STR_T;
    t["DXLIB_VERSION_STR_T"] = t["DXLIB_VERSION_STR"];
    t["DXLIB_VERSION_STR_W"] = t["DXLIB_VERSION_STR"];

	// 定義---------------------------------------------------------------------------

	// π
	DXLUA_PORT(t, DX_PI)
	DXLUA_PORT(t, DX_PI_F)
	DXLUA_PORT(t, DX_TWO_PI)
	DXLUA_PORT(t, DX_TWO_PI_F)

	DXLUA_PORT(t, MAX_IMAGE_NUM)
	DXLUA_PORT(t, MAX_2DSURFACE_NUM)
	DXLUA_PORT(t, MAX_3DSURFACE_NUM)
	DXLUA_PORT(t, MAX_IMAGE_DIVNUM)
	DXLUA_PORT(t, MAX_SURFACE_NUM)
	DXLUA_PORT(t, MAX_SHADOWMAP_NUM)
	DXLUA_PORT(t, MAX_SOFTIMAGE_NUM)

	DXLUA_PORT(t, MAX_SOUND_NUM)
	DXLUA_PORT(t, MAX_SOFTSOUND_NUM)
	DXLUA_PORT(t, MAX_MUSIC_NUM)
	DXLUA_PORT(t, MAX_MOVIE_NUM)
	DXLUA_PORT(t, MAX_MASK_NUM)
	DXLUA_PORT(t, MAX_FONT_NUM)
	DXLUA_PORT(t, MAX_INPUT_NUM)
	DXLUA_PORT(t, MAX_SOCKET_NUM)
	DXLUA_PORT(t, MAX_LIGHT_NUM)
	DXLUA_PORT(t, MAX_SHADER_NUM)
	DXLUA_PORT(t, MAX_CONSTANT_BUFFER_NUM)
	DXLUA_PORT(t, MAX_MODEL_BASE_NUM)
	DXLUA_PORT(t, MAX_MODEL_NUM)
	DXLUA_PORT(t, MAX_VERTEX_BUFFER_NUM)
	DXLUA_PORT(t, MAX_INDEX_BUFFER_NUM)
	DXLUA_PORT(t, MAX_FILE_NUM)
	DXLUA_PORT(t, MAX_LIVE2D_CUBISM4_MODEL_NUM)

	DXLUA_PORT(t, MAX_JOYPAD_NUM)

	DXLUA_PORT(t, DEFAULT_SCREEN_SIZE_X)
	DXLUA_PORT(t, DEFAULT_SCREEN_SIZE_Y)
	DXLUA_PORT(t, DEFAULT_COLOR_BITDEPTH)
	DXLUA_PORT(t, DEFAULT_ZBUFFER_BITDEPTH)

	DXLUA_PORT(t, DEFAULT_FOV)
	DXLUA_PORT(t, DEFAULT_TAN_FOV_HALF)
	DXLUA_PORT(t, DEFAULT_NEAR)
	DXLUA_PORT(t, DEFAULT_FAR)

	DXLUA_PORT(t, DX_DEFAULT_FONT_HANDLE)

	DXLUA_PORT(t, DEFAULT_FONT_SIZE)
	DXLUA_PORT(t, DEFAULT_FONT_THINCK)
	DXLUA_PORT(t, DEFAULT_FONT_TYPE)
	DXLUA_PORT(t, DEFAULT_FONT_EDGESIZE)

	DXLUA_PORT(t, MAX_USERIMAGEREAD_FUNCNUM)

	// ＷＩＮＤＯＷＳのバージョンマクロ
	DXLUA_PORT(t, DX_WINDOWSVERSION_31)
	DXLUA_PORT(t, DX_WINDOWSVERSION_95)
	DXLUA_PORT(t, DX_WINDOWSVERSION_98)
	DXLUA_PORT(t, DX_WINDOWSVERSION_ME)
	DXLUA_PORT(t, DX_WINDOWSVERSION_NT31)
	DXLUA_PORT(t, DX_WINDOWSVERSION_NT40)
	DXLUA_PORT(t, DX_WINDOWSVERSION_2000)
	DXLUA_PORT(t, DX_WINDOWSVERSION_XP)
	DXLUA_PORT(t, DX_WINDOWSVERSION_VISTA)
	DXLUA_PORT(t, DX_WINDOWSVERSION_7)
	DXLUA_PORT(t, DX_WINDOWSVERSION_8)
	DXLUA_PORT(t, DX_WINDOWSVERSION_8_1)
	DXLUA_PORT(t, DX_WINDOWSVERSION_10)
	DXLUA_PORT(t, DX_WINDOWSVERSION_NT_TYPE)

	// DirectXのバージョン
	DXLUA_PORT(t, DX_DIRECTXVERSION_NON)
	DXLUA_PORT(t, DX_DIRECTXVERSION_1)
	DXLUA_PORT(t, DX_DIRECTXVERSION_2)
	DXLUA_PORT(t, DX_DIRECTXVERSION_3)
	DXLUA_PORT(t, DX_DIRECTXVERSION_4)
	DXLUA_PORT(t, DX_DIRECTXVERSION_5)
	DXLUA_PORT(t, DX_DIRECTXVERSION_6)
	DXLUA_PORT(t, DX_DIRECTXVERSION_6_1)
	DXLUA_PORT(t, DX_DIRECTXVERSION_7)
	DXLUA_PORT(t, DX_DIRECTXVERSION_8)
	DXLUA_PORT(t, DX_DIRECTXVERSION_8_1)

	// Direct3Dのバージョン
	DXLUA_PORT(t, DX_DIRECT3D_NONE)
	DXLUA_PORT(t, DX_DIRECT3D_9)
	DXLUA_PORT(t, DX_DIRECT3D_9EX)
	DXLUA_PORT(t, DX_DIRECT3D_11)

	// Direct3D11 の Feature Level
	DXLUA_PORT(t, DX_DIRECT3D_11_FEATURE_LEVEL_9_1)
	DXLUA_PORT(t, DX_DIRECT3D_11_FEATURE_LEVEL_9_2)
	DXLUA_PORT(t, DX_DIRECT3D_11_FEATURE_LEVEL_9_3)
	DXLUA_PORT(t, DX_DIRECT3D_11_FEATURE_LEVEL_10_0)
	DXLUA_PORT(t, DX_DIRECT3D_11_FEATURE_LEVEL_10_1)
	DXLUA_PORT(t, DX_DIRECT3D_11_FEATURE_LEVEL_11_0)
	DXLUA_PORT(t, DX_DIRECT3D_11_FEATURE_LEVEL_11_1)

	// 文字セット
	DXLUA_PORT(t, DX_CHARSET_DEFAULT)
	DXLUA_PORT(t, DX_CHARSET_SHFTJIS)
	DXLUA_PORT(t, DX_CHARSET_HANGEUL)
	DXLUA_PORT(t, DX_CHARSET_BIG5)
	DXLUA_PORT(t, DX_CHARSET_GB2312)
	DXLUA_PORT(t, DX_CHARSET_WINDOWS_1252)
	DXLUA_PORT(t, DX_CHARSET_ISO_IEC_8859_15)
	DXLUA_PORT(t, DX_CHARSET_UTF8)
	DXLUA_PORT(t, DX_CHARSET_NUM)

	// 文字コード形式
	DXLUA_PORT(t, DX_CHARCODEFORMAT_SHIFTJIS)
	DXLUA_PORT(t, DX_CHARCODEFORMAT_GB2312)
	DXLUA_PORT(t, DX_CHARCODEFORMAT_UHC)
	DXLUA_PORT(t, DX_CHARCODEFORMAT_BIG5)
	DXLUA_PORT(t, DX_CHARCODEFORMAT_UTF16LE)
	DXLUA_PORT(t, DX_CHARCODEFORMAT_UTF16BE)
	DXLUA_PORT(t, DX_CHARCODEFORMAT_WINDOWS_1252)
	DXLUA_PORT(t, DX_CHARCODEFORMAT_ISO_IEC_8859_15)
	DXLUA_PORT(t, DX_CHARCODEFORMAT_UTF8)
	DXLUA_PORT(t, DX_CHARCODEFORMAT_ASCII)
	DXLUA_PORT(t, DX_CHARCODEFORMAT_UTF32LE)
	DXLUA_PORT(t, DX_CHARCODEFORMAT_UTF32BE)

	// ＭＩＤＩの演奏モード定義
	DXLUA_PORT(t, DX_MIDIMODE_MCI)
	DXLUA_PORT(t, DX_MIDIMODE_DM)
	DXLUA_PORT(t, DX_MIDIMODE_DIRECT_MUSIC_REVERB)
	DXLUA_PORT(t, DX_MIDIMODE_DIRECT_MUSIC_NORMAL)
	DXLUA_PORT(t, DX_MIDIMODE_NUM)

	// 描画モード定義
	DXLUA_PORT(t, DX_DRAWMODE_NEAREST)
	DXLUA_PORT(t, DX_DRAWMODE_BILINEAR)
	DXLUA_PORT(t, DX_DRAWMODE_ANISOTROPIC)
	DXLUA_PORT(t, DX_DRAWMODE_OTHER)
	DXLUA_PORT(t, DX_DRAWMODE_NUM)

	// フォントのタイプ
	DXLUA_PORT(t, DX_FONTTYPE_NORMAL)
	DXLUA_PORT(t, DX_FONTTYPE_EDGE)
	DXLUA_PORT(t, DX_FONTTYPE_ANTIALIASING)
	DXLUA_PORT(t, DX_FONTTYPE_ANTIALIASING_4X4)
	DXLUA_PORT(t, DX_FONTTYPE_ANTIALIASING_8X8)
	DXLUA_PORT(t, DX_FONTTYPE_ANTIALIASING_EDGE)
	DXLUA_PORT(t, DX_FONTTYPE_ANTIALIASING_EDGE_4X4)
	DXLUA_PORT(t, DX_FONTTYPE_ANTIALIASING_EDGE_8X8)

	// フォント画像の階調ビット数
	DXLUA_PORT(t, DX_FONTIMAGE_BIT_1)
	DXLUA_PORT(t, DX_FONTIMAGE_BIT_4)
	DXLUA_PORT(t, DX_FONTIMAGE_BIT_8)

	// 描画ブレンドモード定義
	DXLUA_PORT(t, DX_BLENDMODE_NOBLEND)
	DXLUA_PORT(t, DX_BLENDMODE_ALPHA)
	DXLUA_PORT(t, DX_BLENDMODE_ADD)
	DXLUA_PORT(t, DX_BLENDMODE_SUB)
	DXLUA_PORT(t, DX_BLENDMODE_MUL)
   // (内部処理用)
	DXLUA_PORT(t, DX_BLENDMODE_SUB2)
	//	DXLUA_PORT(t, DX_BLENDMODE_BLINEALPHA)
	DXLUA_PORT(t, DX_BLENDMODE_XOR)
	DXLUA_PORT(t, DX_BLENDMODE_DESTCOLOR)
	DXLUA_PORT(t, DX_BLENDMODE_INVDESTCOLOR)
	DXLUA_PORT(t, DX_BLENDMODE_INVSRC)
	DXLUA_PORT(t, DX_BLENDMODE_MULA)
	DXLUA_PORT(t, DX_BLENDMODE_ALPHA_X4)
	DXLUA_PORT(t, DX_BLENDMODE_ADD_X4)
	DXLUA_PORT(t, DX_BLENDMODE_SRCCOLOR)
	DXLUA_PORT(t, DX_BLENDMODE_HALF_ADD)
	DXLUA_PORT(t, DX_BLENDMODE_SUB1)
	DXLUA_PORT(t, DX_BLENDMODE_PMA_ALPHA)
	DXLUA_PORT(t, DX_BLENDMODE_PMA_ADD)
	DXLUA_PORT(t, DX_BLENDMODE_PMA_SUB)
	DXLUA_PORT(t, DX_BLENDMODE_PMA_INVSRC)
	DXLUA_PORT(t, DX_BLENDMODE_PMA_ALPHA_X4)
	DXLUA_PORT(t, DX_BLENDMODE_PMA_ADD_X4)
	DXLUA_PORT(t, DX_BLENDMODE_LIVE2D_ZERO)
	DXLUA_PORT(t, DX_BLENDMODE_LIVE2D_NORMAL)
	DXLUA_PORT(t, DX_BLENDMODE_LIVE2D_ADD)
	DXLUA_PORT(t, DX_BLENDMODE_LIVE2D_MULT)
	DXLUA_PORT(t, DX_BLENDMODE_LIVE2D_MASK)
	DXLUA_PORT(t, DX_BLENDMODE_NUM)

	// DrawGraphF 等の浮動小数点値で座標を指定する関数における座標タイプ
	DXLUA_PORT(t, DX_DRAWFLOATCOORDTYPE_DIRECT3D9)
	DXLUA_PORT(t, DX_DRAWFLOATCOORDTYPE_DIRECT3D10)

	// 画像合成タイプ
	DXLUA_PORT(t, DX_BLENDGRAPHTYPE_NORMAL)
	DXLUA_PORT(t, DX_BLENDGRAPHTYPE_WIPE)
	DXLUA_PORT(t, DX_BLENDGRAPHTYPE_ALPHA)
	DXLUA_PORT(t, DX_BLENDGRAPHTYPE_NUM)

	// 画像合成座標タイプ
	DXLUA_PORT(t, DX_BLENDGRAPH_POSMODE_DRAWGRAPH)
	DXLUA_PORT(t, DX_BLENDGRAPH_POSMODE_SCREEN)
	DXLUA_PORT(t, DX_BLENDGRAPH_POSMODE_NUM)

	// グラフィックフィルタータイプ
	DXLUA_PORT(t, DX_GRAPH_FILTER_MONO)
	DXLUA_PORT(t, DX_GRAPH_FILTER_GAUSS)
	DXLUA_PORT(t, DX_GRAPH_FILTER_DOWN_SCALE)
	DXLUA_PORT(t, DX_GRAPH_FILTER_BRIGHT_CLIP)
	DXLUA_PORT(t, DX_GRAPH_FILTER_BRIGHT_SCALE)
	DXLUA_PORT(t, DX_GRAPH_FILTER_HSB)
	DXLUA_PORT(t, DX_GRAPH_FILTER_INVERT)
	DXLUA_PORT(t, DX_GRAPH_FILTER_LEVEL)
	DXLUA_PORT(t, DX_GRAPH_FILTER_TWO_COLOR)
	DXLUA_PORT(t, DX_GRAPH_FILTER_GRADIENT_MAP)
	DXLUA_PORT(t, DX_GRAPH_FILTER_PREMUL_ALPHA)
	DXLUA_PORT(t, DX_GRAPH_FILTER_INTERP_ALPHA)
	DXLUA_PORT(t, DX_GRAPH_FILTER_YUV_TO_RGB)
	DXLUA_PORT(t, DX_GRAPH_FILTER_Y2UV1_TO_RGB)
	DXLUA_PORT(t, DX_GRAPH_FILTER_YUV_TO_RGB_RRA)
	DXLUA_PORT(t, DX_GRAPH_FILTER_Y2UV1_TO_RGB_RRA)
	DXLUA_PORT(t, DX_GRAPH_FILTER_BICUBIC_SCALE)
	DXLUA_PORT(t, DX_GRAPH_FILTER_LANCZOS3_SCALE)
	DXLUA_PORT(t, DX_GRAPH_FILTER_PMA_BRIGHT_CLIP)
	DXLUA_PORT(t, DX_GRAPH_FILTER_PMA_BRIGHT_SCALE)
	DXLUA_PORT(t, DX_GRAPH_FILTER_PMA_HSB)
	DXLUA_PORT(t, DX_GRAPH_FILTER_PMA_INVERT)
	DXLUA_PORT(t, DX_GRAPH_FILTER_PMA_LEVEL)
	DXLUA_PORT(t, DX_GRAPH_FILTER_PMA_TWO_COLOR)
	DXLUA_PORT(t, DX_GRAPH_FILTER_PMA_GRADIENT_MAP)
	DXLUA_PORT(t, DX_GRAPH_FILTER_NUM)

	// グラフィックブレンドタイプ
	DXLUA_PORT(t, DX_GRAPH_BLEND_NORMAL)
	DXLUA_PORT(t, DX_GRAPH_BLEND_RGBA_SELECT_MIX)
	DXLUA_PORT(t, DX_GRAPH_BLEND_MULTIPLE)
	DXLUA_PORT(t, DX_GRAPH_BLEND_DIFFERENCE)
	DXLUA_PORT(t, DX_GRAPH_BLEND_ADD)
	DXLUA_PORT(t, DX_GRAPH_BLEND_SCREEN)
	DXLUA_PORT(t, DX_GRAPH_BLEND_OVERLAY)
	DXLUA_PORT(t, DX_GRAPH_BLEND_DODGE)
	DXLUA_PORT(t, DX_GRAPH_BLEND_BURN)
	DXLUA_PORT(t, DX_GRAPH_BLEND_DARKEN)
	DXLUA_PORT(t, DX_GRAPH_BLEND_LIGHTEN)
	DXLUA_PORT(t, DX_GRAPH_BLEND_SOFTLIGHT)
	DXLUA_PORT(t, DX_GRAPH_BLEND_HARDLIGHT)
	DXLUA_PORT(t, DX_GRAPH_BLEND_EXCLUSION)
	DXLUA_PORT(t, DX_GRAPH_BLEND_NORMAL_ALPHACH)
	DXLUA_PORT(t, DX_GRAPH_BLEND_ADD_ALPHACH)
	DXLUA_PORT(t, DX_GRAPH_BLEND_MULTIPLE_A_ONLY)
	DXLUA_PORT(t, DX_GRAPH_BLEND_PMA_NORMAL)
	DXLUA_PORT(t, DX_GRAPH_BLEND_PMA_RGBA_SELECT_MIX)
	DXLUA_PORT(t, DX_GRAPH_BLEND_PMA_MULTIPLE)
	DXLUA_PORT(t, DX_GRAPH_BLEND_PMA_DIFFERENCE)
	DXLUA_PORT(t, DX_GRAPH_BLEND_PMA_ADD)
	DXLUA_PORT(t, DX_GRAPH_BLEND_PMA_SCREEN)
	DXLUA_PORT(t, DX_GRAPH_BLEND_PMA_OVERLAY)
	DXLUA_PORT(t, DX_GRAPH_BLEND_PMA_DODGE)
	DXLUA_PORT(t, DX_GRAPH_BLEND_PMA_BURN)
	DXLUA_PORT(t, DX_GRAPH_BLEND_PMA_DARKEN)
	DXLUA_PORT(t, DX_GRAPH_BLEND_PMA_LIGHTEN)
	DXLUA_PORT(t, DX_GRAPH_BLEND_PMA_SOFTLIGHT)
	DXLUA_PORT(t, DX_GRAPH_BLEND_PMA_HARDLIGHT)
	DXLUA_PORT(t, DX_GRAPH_BLEND_PMA_EXCLUSION)
	DXLUA_PORT(t, DX_GRAPH_BLEND_PMA_NORMAL_ALPHACH)
	DXLUA_PORT(t, DX_GRAPH_BLEND_PMA_ADD_ALPHACH)
	DXLUA_PORT(t, DX_GRAPH_BLEND_PMA_MULTIPLE_A_ONLY)
	DXLUA_PORT(t, DX_GRAPH_BLEND_NUM)

	// DX_GRAPH_BLEND_RGBA_SELECT_MIX 用の色選択用定義
	DXLUA_PORT(t, DX_RGBA_SELECT_SRC_R)
	DXLUA_PORT(t, DX_RGBA_SELECT_SRC_G)
	DXLUA_PORT(t, DX_RGBA_SELECT_SRC_B)
	DXLUA_PORT(t, DX_RGBA_SELECT_SRC_A)
	DXLUA_PORT(t, DX_RGBA_SELECT_BLEND_R)
	DXLUA_PORT(t, DX_RGBA_SELECT_BLEND_G)
	DXLUA_PORT(t, DX_RGBA_SELECT_BLEND_B)
	DXLUA_PORT(t, DX_RGBA_SELECT_BLEND_A)

	// フィルモード
	DXLUA_PORT(t, DX_FILL_WIREFRAME)
	DXLUA_PORT(t, DX_FILL_SOLID)

	// ポリゴンカリングモード
	DXLUA_PORT(t, DX_CULLING_NONE)
	DXLUA_PORT(t, DX_CULLING_LEFT)
	DXLUA_PORT(t, DX_CULLING_RIGHT)
	DXLUA_PORT(t, DX_CULLING_NUM)

	// クリッピング方向
	DXLUA_PORT(t, DX_CAMERACLIP_LEFT)
	DXLUA_PORT(t, DX_CAMERACLIP_RIGHT)
	DXLUA_PORT(t, DX_CAMERACLIP_BOTTOM)
	DXLUA_PORT(t, DX_CAMERACLIP_TOP)
	DXLUA_PORT(t, DX_CAMERACLIP_BACK)
	DXLUA_PORT(t, DX_CAMERACLIP_FRONT)

	// MV1モデルの頂点タイプ
	DXLUA_PORT(t, DX_MV1_VERTEX_TYPE_1FRAME)
	DXLUA_PORT(t, DX_MV1_VERTEX_TYPE_4FRAME)
	DXLUA_PORT(t, DX_MV1_VERTEX_TYPE_8FRAME)
	DXLUA_PORT(t, DX_MV1_VERTEX_TYPE_FREE_FRAME)
	DXLUA_PORT(t, DX_MV1_VERTEX_TYPE_NMAP_1FRAME)
	DXLUA_PORT(t, DX_MV1_VERTEX_TYPE_NMAP_4FRAME)
	DXLUA_PORT(t, DX_MV1_VERTEX_TYPE_NMAP_8FRAME)
	DXLUA_PORT(t, DX_MV1_VERTEX_TYPE_NMAP_FREE_FRAME)
	DXLUA_PORT(t, DX_MV1_VERTEX_TYPE_NUM)

	// メッシュの種類
	DXLUA_PORT(t, DX_MV1_MESHCATEGORY_NORMAL)
	DXLUA_PORT(t, DX_MV1_MESHCATEGORY_OUTLINE)
	DXLUA_PORT(t, DX_MV1_MESHCATEGORY_OUTLINE_ORIG_SHADER)
	DXLUA_PORT(t, DX_MV1_MESHCATEGORY_NUM)

	// シェイプ率の適用タイプ
	DXLUA_PORT(t, DX_MV1_SHAPERATE_ADD)
	DXLUA_PORT(t, DX_MV1_SHAPERATE_OVERWRITE)

	// MV1ファイルの保存タイプ
	DXLUA_PORT(t, MV1_SAVETYPE_MESH)
	DXLUA_PORT(t, MV1_SAVETYPE_ANIM)
	DXLUA_PORT(t, MV1_SAVETYPE_NORMAL)

	// アニメーションキーデータタイプ
	DXLUA_PORT(t, MV1_ANIMKEY_DATATYPE_ROTATE)
	DXLUA_PORT(t, MV1_ANIMKEY_DATATYPE_ROTATE_X)
	DXLUA_PORT(t, MV1_ANIMKEY_DATATYPE_ROTATE_Y)
	DXLUA_PORT(t, MV1_ANIMKEY_DATATYPE_ROTATE_Z)
	DXLUA_PORT(t, MV1_ANIMKEY_DATATYPE_SCALE)
	DXLUA_PORT(t, MV1_ANIMKEY_DATATYPE_SCALE_X)
	DXLUA_PORT(t, MV1_ANIMKEY_DATATYPE_SCALE_Y)
	DXLUA_PORT(t, MV1_ANIMKEY_DATATYPE_SCALE_Z)
	DXLUA_PORT(t, MV1_ANIMKEY_DATATYPE_TRANSLATE)
	DXLUA_PORT(t, MV1_ANIMKEY_DATATYPE_TRANSLATE_X)
	DXLUA_PORT(t, MV1_ANIMKEY_DATATYPE_TRANSLATE_Y)
	DXLUA_PORT(t, MV1_ANIMKEY_DATATYPE_TRANSLATE_Z)
	DXLUA_PORT(t, MV1_ANIMKEY_DATATYPE_MATRIX4X4C)
	DXLUA_PORT(t, MV1_ANIMKEY_DATATYPE_MATRIX3X3)
	DXLUA_PORT(t, MV1_ANIMKEY_DATATYPE_SHAPE)
	DXLUA_PORT(t, MV1_ANIMKEY_DATATYPE_OTHRE)

	// タイムタイプ
	DXLUA_PORT(t, MV1_ANIMKEY_TIME_TYPE_ONE)
	DXLUA_PORT(t, MV1_ANIMKEY_TIME_TYPE_KEY)

	// アニメーションキータイプ
	DXLUA_PORT(t, MV1_ANIMKEY_TYPE_QUATERNION_X)
	DXLUA_PORT(t, MV1_ANIMKEY_TYPE_VECTOR)
	DXLUA_PORT(t, MV1_ANIMKEY_TYPE_MATRIX4X4C)
	DXLUA_PORT(t, MV1_ANIMKEY_TYPE_MATRIX3X3)
	DXLUA_PORT(t, MV1_ANIMKEY_TYPE_FLAT)
	DXLUA_PORT(t, MV1_ANIMKEY_TYPE_LINEAR)
	DXLUA_PORT(t, MV1_ANIMKEY_TYPE_BLEND)
	DXLUA_PORT(t, MV1_ANIMKEY_TYPE_QUATERNION_VMD)

	// 描画先画面指定用定義
	DXLUA_PORT(t, DX_SCREEN_FRONT)
	DXLUA_PORT(t, DX_SCREEN_BACK)
	DXLUA_PORT(t, DX_SCREEN_WORK)
	DXLUA_PORT(t, DX_SCREEN_TEMPFRONT)
	DXLUA_PORT(t, DX_SCREEN_OTHER)

	DXLUA_PORT(t, DX_NONE_GRAPH)

	// グラフィック減色時の画像劣化緩和処理モード
	DXLUA_PORT(t, DX_SHAVEDMODE_NONE)
	DXLUA_PORT(t, DX_SHAVEDMODE_DITHER)
	DXLUA_PORT(t, DX_SHAVEDMODE_DIFFUS)

	// 画像の保存タイプ
	DXLUA_PORT(t, DX_IMAGESAVETYPE_BMP)
	DXLUA_PORT(t, DX_IMAGESAVETYPE_JPEG)
	DXLUA_PORT(t, DX_IMAGESAVETYPE_PNG)
	DXLUA_PORT(t, DX_IMAGESAVETYPE_DDS)

	// サウンド再生形態指定用定義
	DXLUA_PORT(t, DX_PLAYTYPE_LOOPBIT)
	DXLUA_PORT(t, DX_PLAYTYPE_BACKBIT)

	DXLUA_PORT(t, DX_PLAYTYPE_NORMAL)
	DXLUA_PORT(t, DX_PLAYTYPE_BACK)
	DXLUA_PORT(t, DX_PLAYTYPE_LOOP)

	// 動画再生タイプ定義
	DXLUA_PORT(t, DX_MOVIEPLAYTYPE_BCANCEL)
	DXLUA_PORT(t, DX_MOVIEPLAYTYPE_NORMAL)

	// サウンドのタイプ
	DXLUA_PORT(t, DX_SOUNDTYPE_NORMAL)
	DXLUA_PORT(t, DX_SOUNDTYPE_STREAMSTYLE)

	// サウンドデータタイプのマクロ
	DXLUA_PORT(t, DX_SOUNDDATATYPE_MEMNOPRESS)
	DXLUA_PORT(t, DX_SOUNDDATATYPE_MEMNOPRESS_PLUS)
	DXLUA_PORT(t, DX_SOUNDDATATYPE_MEMPRESS)
	DXLUA_PORT(t, DX_SOUNDDATATYPE_FILE)

	// 読み込み処理のタイプ
	DXLUA_PORT(t, DX_READSOUNDFUNCTION_PCM)
	DXLUA_PORT(t, DX_READSOUNDFUNCTION_OGG)
	DXLUA_PORT(t, DX_READSOUNDFUNCTION_OPUS)
	DXLUA_PORT(t, DX_READSOUNDFUNCTION_DEFAULT_NUM)

	// ３Ｄサウンドリバーブエフェクトのプリセット
	DXLUA_PORT(t, DX_REVERB_PRESET_DEFAULT)
	DXLUA_PORT(t, DX_REVERB_PRESET_GENERIC)
	DXLUA_PORT(t, DX_REVERB_PRESET_PADDEDCELL)
	DXLUA_PORT(t, DX_REVERB_PRESET_ROOM)
	DXLUA_PORT(t, DX_REVERB_PRESET_BATHROOM)
	DXLUA_PORT(t, DX_REVERB_PRESET_LIVINGROOM)
	DXLUA_PORT(t, DX_REVERB_PRESET_STONEROOM)
	DXLUA_PORT(t, DX_REVERB_PRESET_AUDITORIUM)
	DXLUA_PORT(t, DX_REVERB_PRESET_CONCERTHALL)
	DXLUA_PORT(t, DX_REVERB_PRESET_CAVE)
	DXLUA_PORT(t, DX_REVERB_PRESET_ARENA)
	DXLUA_PORT(t, DX_REVERB_PRESET_HANGAR)
	DXLUA_PORT(t, DX_REVERB_PRESET_CARPETEDHALLWAY)
	DXLUA_PORT(t, DX_REVERB_PRESET_HALLWAY)
	DXLUA_PORT(t, DX_REVERB_PRESET_STONECORRIDOR)
	DXLUA_PORT(t, DX_REVERB_PRESET_ALLEY)
	DXLUA_PORT(t, DX_REVERB_PRESET_FOREST)
	DXLUA_PORT(t, DX_REVERB_PRESET_CITY)
	DXLUA_PORT(t, DX_REVERB_PRESET_MOUNTAINS)
	DXLUA_PORT(t, DX_REVERB_PRESET_QUARRY)
	DXLUA_PORT(t, DX_REVERB_PRESET_PLAIN)
	DXLUA_PORT(t, DX_REVERB_PRESET_PARKINGLOT)
	DXLUA_PORT(t, DX_REVERB_PRESET_SEWERPIPE)
	DXLUA_PORT(t, DX_REVERB_PRESET_UNDERWATER)
	DXLUA_PORT(t, DX_REVERB_PRESET_SMALLROOM)
	DXLUA_PORT(t, DX_REVERB_PRESET_MEDIUMROOM)
	DXLUA_PORT(t, DX_REVERB_PRESET_LARGEROOM)
	DXLUA_PORT(t, DX_REVERB_PRESET_MEDIUMHALL)
	DXLUA_PORT(t, DX_REVERB_PRESET_LARGEHALL)
	DXLUA_PORT(t, DX_REVERB_PRESET_PLATE)

	DXLUA_PORT(t, DX_REVERB_PRESET_NUM)

	// マスク透過色モード
	DXLUA_PORT(t, DX_MASKTRANS_WHITE)
	DXLUA_PORT(t, DX_MASKTRANS_BLACK)
	DXLUA_PORT(t, DX_MASKTRANS_NONE)

	// マスク画像チャンネル
	DXLUA_PORT(t, DX_MASKGRAPH_CH_A)
	DXLUA_PORT(t, DX_MASKGRAPH_CH_R)
	DXLUA_PORT(t, DX_MASKGRAPH_CH_G)
	DXLUA_PORT(t, DX_MASKGRAPH_CH_B)

	// Ｚバッファ書き込みモード
	DXLUA_PORT(t, DX_ZWRITE_MASK)
	DXLUA_PORT(t, DX_ZWRITE_CLEAR)

	// 比較モード
	DXLUA_PORT(t, DX_CMP_NEVER)
	DXLUA_PORT(t, DX_CMP_LESS)
	DXLUA_PORT(t, DX_CMP_EQUAL)
	DXLUA_PORT(t, DX_CMP_LESSEQUAL)
	DXLUA_PORT(t, DX_CMP_GREATER)
	DXLUA_PORT(t, DX_CMP_NOTEQUAL)
	DXLUA_PORT(t, DX_CMP_GREATEREQUAL)
	DXLUA_PORT(t, DX_CMP_ALWAYS)
	DXLUA_PORT(t, DX_ZCMP_DEFAULT)
	DXLUA_PORT(t, DX_ZCMP_REVERSE)

	// シェーディングモード
	DXLUA_PORT(t, DX_SHADEMODE_FLAT)
	DXLUA_PORT(t, DX_SHADEMODE_GOURAUD)

	// フォグモード
	DXLUA_PORT(t, DX_FOGMODE_NONE)
	DXLUA_PORT(t, DX_FOGMODE_EXP)
	DXLUA_PORT(t, DX_FOGMODE_EXP2)
	DXLUA_PORT(t, DX_FOGMODE_LINEAR)

	// マテリアルタイプ
	DXLUA_PORT(t, DX_MATERIAL_TYPE_NORMAL)
	DXLUA_PORT(t, DX_MATERIAL_TYPE_TOON)
	DXLUA_PORT(t, DX_MATERIAL_TYPE_TOON_2)
	DXLUA_PORT(t, DX_MATERIAL_TYPE_MAT_SPEC_LUMINANCE_UNORM)
	DXLUA_PORT(t, DX_MATERIAL_TYPE_MAT_SPEC_LUMINANCE_CLIP_UNORM)
	DXLUA_PORT(t, DX_MATERIAL_TYPE_MAT_SPEC_LUMINANCE_CMP_GREATEREQUAL)
	DXLUA_PORT(t, DX_MATERIAL_TYPE_MAT_SPEC_POWER_UNORM)
	DXLUA_PORT(t, DX_MATERIAL_TYPE_MAT_SPEC_POWER_CLIP_UNORM)
	DXLUA_PORT(t, DX_MATERIAL_TYPE_MAT_SPEC_POWER_CMP_GREATEREQUAL)
	DXLUA_PORT(t, DX_MATERIAL_TYPE_NUM)

	// マテリアルブレンドタイプ
	DXLUA_PORT(t, DX_MATERIAL_BLENDTYPE_TRANSLUCENT)
	DXLUA_PORT(t, DX_MATERIAL_BLENDTYPE_ADDITIVE)
	DXLUA_PORT(t, DX_MATERIAL_BLENDTYPE_MODULATE)
	DXLUA_PORT(t, DX_MATERIAL_BLENDTYPE_NONE)

	// テクスチャアドレスタイプ
	DXLUA_PORT(t, DX_TEXADDRESS_WRAP)
	DXLUA_PORT(t, DX_TEXADDRESS_MIRROR)
	DXLUA_PORT(t, DX_TEXADDRESS_CLAMP)
	DXLUA_PORT(t, DX_TEXADDRESS_BORDER)
	DXLUA_PORT(t, DX_TEXADDRESS_NUM)

	// シェーダータイプ
	DXLUA_PORT(t, DX_SHADERTYPE_VERTEX)
	DXLUA_PORT(t, DX_SHADERTYPE_PIXEL)
	DXLUA_PORT(t, DX_SHADERTYPE_GEOMETRY)
	DXLUA_PORT(t, DX_SHADERTYPE_COMPUTE)
	DXLUA_PORT(t, DX_SHADERTYPE_DOMAIN)
	DXLUA_PORT(t, DX_SHADERTYPE_HULL)

	// 頂点データタイプ
	DXLUA_PORT(t, DX_VERTEX_TYPE_NORMAL_3D)
	DXLUA_PORT(t, DX_VERTEX_TYPE_SHADER_3D)
	DXLUA_PORT(t, DX_VERTEX_TYPE_NUM)

	// インデックスデータタイプ
	DXLUA_PORT(t, DX_INDEX_TYPE_16BIT)
	DXLUA_PORT(t, DX_INDEX_TYPE_32BIT)

	// モデルファイル読み込み時の物理演算モード
	DXLUA_PORT(t, DX_LOADMODEL_PHYSICS_DISABLE)
	DXLUA_PORT(t, DX_LOADMODEL_PHYSICS_LOADCALC)
	DXLUA_PORT(t, DX_LOADMODEL_PHYSICS_REALTIME)

	// モデルファイル読み込み時の物理演算無効名前ワードのモード
	DXLUA_PORT(t, DX_LOADMODEL_PHYSICS_DISABLENAMEWORD_ALWAYS)
	DXLUA_PORT(t, DX_LOADMODEL_PHYSICS_DISABLENAMEWORD_DISABLEPHYSICSFILEONLY)
	DXLUA_PORT(t, DX_LOADMODEL_PHYSICS_DISABLENAMEWORD_NUM)

	// PMD, PMX ファイル読み込み時のアニメーションの FPS モード( 主に IK 部分の精度に影響します )
	DXLUA_PORT(t, DX_LOADMODEL_PMD_PMX_ANIMATION_FPSMODE_30)
	DXLUA_PORT(t, DX_LOADMODEL_PMD_PMX_ANIMATION_FPSMODE_60)

	// モデルの半透明要素がある箇所に関する描画モード
	DXLUA_PORT(t, DX_SEMITRANSDRAWMODE_ALWAYS)
	DXLUA_PORT(t, DX_SEMITRANSDRAWMODE_SEMITRANS_ONLY)
	DXLUA_PORT(t, DX_SEMITRANSDRAWMODE_NOT_SEMITRANS_ONLY)

	// キューブマップの面番号
	DXLUA_PORT(t, DX_CUBEMAP_FACE_POSITIVE_X)
	DXLUA_PORT(t, DX_CUBEMAP_FACE_NEGATIVE_X)
	DXLUA_PORT(t, DX_CUBEMAP_FACE_POSITIVE_Y)
	DXLUA_PORT(t, DX_CUBEMAP_FACE_NEGATIVE_Y)
	DXLUA_PORT(t, DX_CUBEMAP_FACE_POSITIVE_Z)
	DXLUA_PORT(t, DX_CUBEMAP_FACE_NEGATIVE_Z)

	// ポリゴン描画タイプ
	DXLUA_PORT(t, DX_PRIMTYPE_POINTLIST)
	DXLUA_PORT(t, DX_PRIMTYPE_LINELIST)
	DXLUA_PORT(t, DX_PRIMTYPE_LINESTRIP)
	DXLUA_PORT(t, DX_PRIMTYPE_TRIANGLELIST)
	DXLUA_PORT(t, DX_PRIMTYPE_TRIANGLESTRIP)
	DXLUA_PORT(t, DX_PRIMTYPE_TRIANGLEFAN)
	DXLUA_PORT(t, DX_PRIMTYPE_MIN)
	DXLUA_PORT(t, DX_PRIMTYPE_MAX)

	// ライトタイプ
	DXLUA_PORT(t, DX_LIGHTTYPE_D3DLIGHT_POINT)
	DXLUA_PORT(t, DX_LIGHTTYPE_D3DLIGHT_SPOT)
	DXLUA_PORT(t, DX_LIGHTTYPE_D3DLIGHT_DIRECTIONAL)
	DXLUA_PORT(t, DX_LIGHTTYPE_POINT)
	DXLUA_PORT(t, DX_LIGHTTYPE_SPOT)
	DXLUA_PORT(t, DX_LIGHTTYPE_DIRECTIONAL)

	// グラフィックイメージフォーマットの定義
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_PAL4)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_PAL8)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_PAL4)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_PAL8)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_PAL4)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_PAL8)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_RGB16)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_RGB32)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB16)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB32)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_RGB16)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_RGB32)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_DXT1)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_DXT2)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_DXT3)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_DXT4)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_DXT5)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_BC7_UNORM)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_BC7_UNORM_SRGB)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_PLATFORM0)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_PLATFORM1)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_PLATFORM2)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_PLATFORM3)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_YUV)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_I16)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F16)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F32)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I8)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I16)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F16)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F32)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_TWO_I8)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_TWO_I16)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F16)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F32)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB16)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB32)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ALPHA_RGB32)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_I16)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F16)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F32)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I8)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I16)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F16)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F32)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I8)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I16)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F16)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F32)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_3D_NUM)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_2D)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_R5G6B5)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_X8A8R5G6B5)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_X8R8G8B8)
	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_A8R8G8B8)

	DXLUA_PORT(t, DX_GRAPHICSIMAGE_FORMAT_NUM)

	// 基本イメージのピクセルフォーマット
	DXLUA_PORT(t, DX_BASEIMAGE_FORMAT_NORMAL)
	DXLUA_PORT(t, DX_BASEIMAGE_FORMAT_DXT1)
	DXLUA_PORT(t, DX_BASEIMAGE_FORMAT_DXT2)
	DXLUA_PORT(t, DX_BASEIMAGE_FORMAT_DXT3)
	DXLUA_PORT(t, DX_BASEIMAGE_FORMAT_DXT4)
	DXLUA_PORT(t, DX_BASEIMAGE_FORMAT_DXT5)
	DXLUA_PORT(t, DX_BASEIMAGE_FORMAT_BC7_UNORM)
	DXLUA_PORT(t, DX_BASEIMAGE_FORMAT_BC7_UNORM_SRGB)
	DXLUA_PORT(t, DX_BASEIMAGE_FORMAT_PLATFORM0)
	DXLUA_PORT(t, DX_BASEIMAGE_FORMAT_PLATFORM1)
	DXLUA_PORT(t, DX_BASEIMAGE_FORMAT_PLATFORM2)
	DXLUA_PORT(t, DX_BASEIMAGE_FORMAT_PLATFORM3)
	DXLUA_PORT(t, DX_BASEIMAGE_FORMAT_YUV)

	// ムービーのサーフェスモード
	DXLUA_PORT(t, DX_MOVIESURFACE_NORMAL)
	DXLUA_PORT(t, DX_MOVIESURFACE_OVERLAY)
	DXLUA_PORT(t, DX_MOVIESURFACE_FULLCOLOR)

	// SetUserScreenImage で渡す画面のピクセルフォーマット
	DXLUA_PORT(t, DX_USER_SCREEN_PIXEL_FORMAT_R5G6B5)
	DXLUA_PORT(t, DX_USER_SCREEN_PIXEL_FORMAT_R5G5B5X1)
	DXLUA_PORT(t, DX_USER_SCREEN_PIXEL_FORMAT_X1R5G5B5)
	DXLUA_PORT(t, DX_USER_SCREEN_PIXEL_FORMAT_X8B8G8R8)
	DXLUA_PORT(t, DX_USER_SCREEN_PIXEL_FORMAT_X8R8G8B8)
	DXLUA_PORT(t, DX_USER_SCREEN_PIXEL_FORMAT_NUM)

	// ウインドウの奥行き位置設定タイプ
	DXLUA_PORT(t, DX_WIN_ZTYPE_NORMAL)
	DXLUA_PORT(t, DX_WIN_ZTYPE_BOTTOM)
	DXLUA_PORT(t, DX_WIN_ZTYPE_TOP)
	DXLUA_PORT(t, DX_WIN_ZTYPE_TOPMOST)

	// ツールバーのボタンの状態
	DXLUA_PORT(t, TOOLBUTTON_STATE_ENABLE)
	DXLUA_PORT(t, TOOLBUTTON_STATE_PRESSED)
	DXLUA_PORT(t, TOOLBUTTON_STATE_DISABLE)
	DXLUA_PORT(t, TOOLBUTTON_STATE_PRESSED_DISABLE)
	DXLUA_PORT(t, TOOLBUTTON_STATE_NUM)

	// ツールバーのボタンのタイプ
	DXLUA_PORT(t, TOOLBUTTON_TYPE_NORMAL)
	DXLUA_PORT(t, TOOLBUTTON_TYPE_CHECK)
	DXLUA_PORT(t, TOOLBUTTON_TYPE_GROUP)
	DXLUA_PORT(t, TOOLBUTTON_TYPE_SEP)
	DXLUA_PORT(t, TOOLBUTTON_TYPE_NUM)

	// 親メニューのＩＤ
	DXLUA_PORT(t, MENUITEM_IDTOP)

	// メニューに追加する際のタイプ
	DXLUA_PORT(t, MENUITEM_ADD_CHILD)
	DXLUA_PORT(t, MENUITEM_ADD_INSERT)

	// メニューの横に付くマークタイプ
	DXLUA_PORT(t, MENUITEM_MARK_NONE)
	DXLUA_PORT(t, MENUITEM_MARK_CHECK)
	DXLUA_PORT(t, MENUITEM_MARK_RADIO)

	// 文字変換タイプ定義
	DXLUA_PORT(t, DX_NUMMODE_10)
	DXLUA_PORT(t, DX_NUMMODE_16)
	DXLUA_PORT(t, DX_STRMODE_NOT0)
	DXLUA_PORT(t, DX_STRMODE_USE0)

	// CheckHitKeyAll で調べる入力タイプ
	DXLUA_PORT(t, DX_CHECKINPUT_KEY)
	DXLUA_PORT(t, DX_CHECKINPUT_PAD)
	DXLUA_PORT(t, DX_CHECKINPUT_MOUSE)
	DXLUA_PORT(t, DX_CHECKINPUT_ALL)

	// パッド入力取得パラメータ
	DXLUA_PORT(t, DX_INPUT_KEY_PAD1)
	DXLUA_PORT(t, DX_INPUT_PAD1)
	DXLUA_PORT(t, DX_INPUT_PAD2)
	DXLUA_PORT(t, DX_INPUT_PAD3)
	DXLUA_PORT(t, DX_INPUT_PAD4)
	DXLUA_PORT(t, DX_INPUT_PAD5)
	DXLUA_PORT(t, DX_INPUT_PAD6)
	DXLUA_PORT(t, DX_INPUT_PAD7)
	DXLUA_PORT(t, DX_INPUT_PAD8)
	DXLUA_PORT(t, DX_INPUT_PAD9)
	DXLUA_PORT(t, DX_INPUT_PAD10)
	DXLUA_PORT(t, DX_INPUT_PAD11)
	DXLUA_PORT(t, DX_INPUT_PAD12)
	DXLUA_PORT(t, DX_INPUT_PAD13)
	DXLUA_PORT(t, DX_INPUT_PAD14)
	DXLUA_PORT(t, DX_INPUT_PAD15)
	DXLUA_PORT(t, DX_INPUT_PAD16)
	DXLUA_PORT(t, DX_INPUT_KEY)

	// タッチの同時接触検出対応最大数
	DXLUA_PORT(t, TOUCHINPUTPOINT_MAX)

	// パッド入力定義
	DXLUA_PORT(t, PAD_INPUT_DOWN)
	DXLUA_PORT(t, PAD_INPUT_LEFT)
	DXLUA_PORT(t, PAD_INPUT_RIGHT)
	DXLUA_PORT(t, PAD_INPUT_UP)
	DXLUA_PORT(t, PAD_INPUT_A)
	DXLUA_PORT(t, PAD_INPUT_B)
	DXLUA_PORT(t, PAD_INPUT_C)
	DXLUA_PORT(t, PAD_INPUT_X)
	DXLUA_PORT(t, PAD_INPUT_Y)
	DXLUA_PORT(t, PAD_INPUT_Z)
	DXLUA_PORT(t, PAD_INPUT_L)
	DXLUA_PORT(t, PAD_INPUT_R)
	DXLUA_PORT(t, PAD_INPUT_START)
	DXLUA_PORT(t, PAD_INPUT_M)
	DXLUA_PORT(t, PAD_INPUT_D)
	DXLUA_PORT(t, PAD_INPUT_F)
	DXLUA_PORT(t, PAD_INPUT_G)
	DXLUA_PORT(t, PAD_INPUT_H)
	DXLUA_PORT(t, PAD_INPUT_I)
	DXLUA_PORT(t, PAD_INPUT_J)
	DXLUA_PORT(t, PAD_INPUT_K)
	DXLUA_PORT(t, PAD_INPUT_LL)
	DXLUA_PORT(t, PAD_INPUT_N)
	DXLUA_PORT(t, PAD_INPUT_O)
	DXLUA_PORT(t, PAD_INPUT_P)
	DXLUA_PORT(t, PAD_INPUT_RR)
	DXLUA_PORT(t, PAD_INPUT_S)
	DXLUA_PORT(t, PAD_INPUT_T)
	DXLUA_PORT(t, PAD_INPUT_U)
	DXLUA_PORT(t, PAD_INPUT_V)
	DXLUA_PORT(t, PAD_INPUT_W)
	DXLUA_PORT(t, PAD_INPUT_XX)

	DXLUA_PORT(t, PAD_INPUT_1)
	DXLUA_PORT(t, PAD_INPUT_2)
	DXLUA_PORT(t, PAD_INPUT_3)
	DXLUA_PORT(t, PAD_INPUT_4)
	DXLUA_PORT(t, PAD_INPUT_5)
	DXLUA_PORT(t, PAD_INPUT_6)
	DXLUA_PORT(t, PAD_INPUT_7)
	DXLUA_PORT(t, PAD_INPUT_8)
	DXLUA_PORT(t, PAD_INPUT_9)
	DXLUA_PORT(t, PAD_INPUT_10)
	DXLUA_PORT(t, PAD_INPUT_11)
	DXLUA_PORT(t, PAD_INPUT_12)
	DXLUA_PORT(t, PAD_INPUT_13)
	DXLUA_PORT(t, PAD_INPUT_14)
	DXLUA_PORT(t, PAD_INPUT_15)
	DXLUA_PORT(t, PAD_INPUT_16)
	DXLUA_PORT(t, PAD_INPUT_17)
	DXLUA_PORT(t, PAD_INPUT_18)
	DXLUA_PORT(t, PAD_INPUT_19)
	DXLUA_PORT(t, PAD_INPUT_20)
	DXLUA_PORT(t, PAD_INPUT_21)
	DXLUA_PORT(t, PAD_INPUT_22)
	DXLUA_PORT(t, PAD_INPUT_23)
	DXLUA_PORT(t, PAD_INPUT_24)
	DXLUA_PORT(t, PAD_INPUT_25)
	DXLUA_PORT(t, PAD_INPUT_26)
	DXLUA_PORT(t, PAD_INPUT_27)
	DXLUA_PORT(t, PAD_INPUT_28)

	// XInputボタン入力定義
	DXLUA_PORT(t, XINPUT_BUTTON_DPAD_UP)
	DXLUA_PORT(t, XINPUT_BUTTON_DPAD_DOWN)
	DXLUA_PORT(t, XINPUT_BUTTON_DPAD_LEFT)
	DXLUA_PORT(t, XINPUT_BUTTON_DPAD_RIGHT)
	DXLUA_PORT(t, XINPUT_BUTTON_START)
	DXLUA_PORT(t, XINPUT_BUTTON_BACK)
	DXLUA_PORT(t, XINPUT_BUTTON_LEFT_THUMB)
	DXLUA_PORT(t, XINPUT_BUTTON_RIGHT_THUMB)
	DXLUA_PORT(t, XINPUT_BUTTON_LEFT_SHOULDER)
	DXLUA_PORT(t, XINPUT_BUTTON_RIGHT_SHOULDER)
	DXLUA_PORT(t, XINPUT_BUTTON_A)
	DXLUA_PORT(t, XINPUT_BUTTON_B)
	DXLUA_PORT(t, XINPUT_BUTTON_X)
	DXLUA_PORT(t, XINPUT_BUTTON_Y)

	// マウス入力定義
	DXLUA_PORT(t, MOUSE_INPUT_LEFT)
	DXLUA_PORT(t, MOUSE_INPUT_RIGHT)
	DXLUA_PORT(t, MOUSE_INPUT_MIDDLE)
	DXLUA_PORT(t, MOUSE_INPUT_1)
	DXLUA_PORT(t, MOUSE_INPUT_2)
	DXLUA_PORT(t, MOUSE_INPUT_3)
	DXLUA_PORT(t, MOUSE_INPUT_4)
	DXLUA_PORT(t, MOUSE_INPUT_5)
	DXLUA_PORT(t, MOUSE_INPUT_6)
	DXLUA_PORT(t, MOUSE_INPUT_7)
	DXLUA_PORT(t, MOUSE_INPUT_8)

	// マウスのログ情報タイプ
	DXLUA_PORT(t, MOUSE_INPUT_LOG_DOWN)
	DXLUA_PORT(t, MOUSE_INPUT_LOG_UP)

	// キー定義
	DXLUA_PORT(t, KEY_INPUT_BACK)
	DXLUA_PORT(t, KEY_INPUT_TAB)
	DXLUA_PORT(t, KEY_INPUT_RETURN)

	DXLUA_PORT(t, KEY_INPUT_LSHIFT)
	DXLUA_PORT(t, KEY_INPUT_RSHIFT)
	DXLUA_PORT(t, KEY_INPUT_LCONTROL)
	DXLUA_PORT(t, KEY_INPUT_RCONTROL)
	DXLUA_PORT(t, KEY_INPUT_ESCAPE)
	DXLUA_PORT(t, KEY_INPUT_SPACE)
	DXLUA_PORT(t, KEY_INPUT_PGUP)
	DXLUA_PORT(t, KEY_INPUT_PGDN)
	DXLUA_PORT(t, KEY_INPUT_END)
	DXLUA_PORT(t, KEY_INPUT_HOME)
	DXLUA_PORT(t, KEY_INPUT_LEFT)
	DXLUA_PORT(t, KEY_INPUT_UP)
	DXLUA_PORT(t, KEY_INPUT_RIGHT)
	DXLUA_PORT(t, KEY_INPUT_DOWN)
	DXLUA_PORT(t, KEY_INPUT_INSERT)
	DXLUA_PORT(t, KEY_INPUT_DELETE)

	DXLUA_PORT(t, KEY_INPUT_MINUS)
	DXLUA_PORT(t, KEY_INPUT_YEN)
	DXLUA_PORT(t, KEY_INPUT_PREVTRACK)
	DXLUA_PORT(t, KEY_INPUT_PERIOD)
	DXLUA_PORT(t, KEY_INPUT_SLASH)
	DXLUA_PORT(t, KEY_INPUT_LALT)
	DXLUA_PORT(t, KEY_INPUT_RALT)
	DXLUA_PORT(t, KEY_INPUT_SCROLL)
	DXLUA_PORT(t, KEY_INPUT_SEMICOLON)
	DXLUA_PORT(t, KEY_INPUT_COLON)
	DXLUA_PORT(t, KEY_INPUT_LBRACKET)
	DXLUA_PORT(t, KEY_INPUT_RBRACKET)
	DXLUA_PORT(t, KEY_INPUT_AT)
	DXLUA_PORT(t, KEY_INPUT_BACKSLASH)
	DXLUA_PORT(t, KEY_INPUT_COMMA)
	DXLUA_PORT(t, KEY_INPUT_KANJI)
	DXLUA_PORT(t, KEY_INPUT_CONVERT)
	DXLUA_PORT(t, KEY_INPUT_NOCONVERT)
	DXLUA_PORT(t, KEY_INPUT_KANA)
	DXLUA_PORT(t, KEY_INPUT_APPS)
	DXLUA_PORT(t, KEY_INPUT_CAPSLOCK)
	DXLUA_PORT(t, KEY_INPUT_SYSRQ)
	DXLUA_PORT(t, KEY_INPUT_PAUSE)
	DXLUA_PORT(t, KEY_INPUT_LWIN)
	DXLUA_PORT(t, KEY_INPUT_RWIN)

	DXLUA_PORT(t, KEY_INPUT_NUMLOCK)
	DXLUA_PORT(t, KEY_INPUT_NUMPAD0)
	DXLUA_PORT(t, KEY_INPUT_NUMPAD1)
	DXLUA_PORT(t, KEY_INPUT_NUMPAD2)
	DXLUA_PORT(t, KEY_INPUT_NUMPAD3)
	DXLUA_PORT(t, KEY_INPUT_NUMPAD4)
	DXLUA_PORT(t, KEY_INPUT_NUMPAD5)
	DXLUA_PORT(t, KEY_INPUT_NUMPAD6)
	DXLUA_PORT(t, KEY_INPUT_NUMPAD7)
	DXLUA_PORT(t, KEY_INPUT_NUMPAD8)
	DXLUA_PORT(t, KEY_INPUT_NUMPAD9)
	DXLUA_PORT(t, KEY_INPUT_MULTIPLY)
	DXLUA_PORT(t, KEY_INPUT_ADD)
	DXLUA_PORT(t, KEY_INPUT_SUBTRACT)
	DXLUA_PORT(t, KEY_INPUT_DECIMAL)
	DXLUA_PORT(t, KEY_INPUT_DIVIDE)
	DXLUA_PORT(t, KEY_INPUT_NUMPADENTER)

	DXLUA_PORT(t, KEY_INPUT_F1)
	DXLUA_PORT(t, KEY_INPUT_F2)
	DXLUA_PORT(t, KEY_INPUT_F3)
	DXLUA_PORT(t, KEY_INPUT_F4)
	DXLUA_PORT(t, KEY_INPUT_F5)
	DXLUA_PORT(t, KEY_INPUT_F6)
	DXLUA_PORT(t, KEY_INPUT_F7)
	DXLUA_PORT(t, KEY_INPUT_F8)
	DXLUA_PORT(t, KEY_INPUT_F9)
	DXLUA_PORT(t, KEY_INPUT_F10)
	DXLUA_PORT(t, KEY_INPUT_F11)
	DXLUA_PORT(t, KEY_INPUT_F12)

	DXLUA_PORT(t, KEY_INPUT_A)
	DXLUA_PORT(t, KEY_INPUT_B)
	DXLUA_PORT(t, KEY_INPUT_C)
	DXLUA_PORT(t, KEY_INPUT_D)
	DXLUA_PORT(t, KEY_INPUT_E)
	DXLUA_PORT(t, KEY_INPUT_F)
	DXLUA_PORT(t, KEY_INPUT_G)
	DXLUA_PORT(t, KEY_INPUT_H)
	DXLUA_PORT(t, KEY_INPUT_I)
	DXLUA_PORT(t, KEY_INPUT_J)
	DXLUA_PORT(t, KEY_INPUT_K)
	DXLUA_PORT(t, KEY_INPUT_L)
	DXLUA_PORT(t, KEY_INPUT_M)
	DXLUA_PORT(t, KEY_INPUT_N)
	DXLUA_PORT(t, KEY_INPUT_O)
	DXLUA_PORT(t, KEY_INPUT_P)
	DXLUA_PORT(t, KEY_INPUT_Q)
	DXLUA_PORT(t, KEY_INPUT_R)
	DXLUA_PORT(t, KEY_INPUT_S)
	DXLUA_PORT(t, KEY_INPUT_T)
	DXLUA_PORT(t, KEY_INPUT_U)
	DXLUA_PORT(t, KEY_INPUT_V)
	DXLUA_PORT(t, KEY_INPUT_W)
	DXLUA_PORT(t, KEY_INPUT_X)
	DXLUA_PORT(t, KEY_INPUT_Y)
	DXLUA_PORT(t, KEY_INPUT_Z)

	DXLUA_PORT(t, KEY_INPUT_0)
	DXLUA_PORT(t, KEY_INPUT_1)
	DXLUA_PORT(t, KEY_INPUT_2)
	DXLUA_PORT(t, KEY_INPUT_3)
	DXLUA_PORT(t, KEY_INPUT_4)
	DXLUA_PORT(t, KEY_INPUT_5)
	DXLUA_PORT(t, KEY_INPUT_6)
	DXLUA_PORT(t, KEY_INPUT_7)
	DXLUA_PORT(t, KEY_INPUT_8)
	DXLUA_PORT(t, KEY_INPUT_9)

	// アスキーコントロールキーコード
	DXLUA_PORT(t, CTRL_CODE_BS)
	DXLUA_PORT(t, CTRL_CODE_TAB)
	DXLUA_PORT(t, CTRL_CODE_CR)
	DXLUA_PORT(t, CTRL_CODE_DEL)

	DXLUA_PORT(t, CTRL_CODE_COPY)
	DXLUA_PORT(t, CTRL_CODE_PASTE)
	DXLUA_PORT(t, CTRL_CODE_CUT)
	DXLUA_PORT(t, CTRL_CODE_ALL)

	DXLUA_PORT(t, CTRL_CODE_LEFT)
	DXLUA_PORT(t, CTRL_CODE_RIGHT)
	DXLUA_PORT(t, CTRL_CODE_UP)
	DXLUA_PORT(t, CTRL_CODE_DOWN)

	DXLUA_PORT(t, CTRL_CODE_HOME)
	DXLUA_PORT(t, CTRL_CODE_END)
	DXLUA_PORT(t, CTRL_CODE_PAGE_UP)
	DXLUA_PORT(t, CTRL_CODE_PAGE_DOWN)

	DXLUA_PORT(t, CTRL_CODE_ESC)
	DXLUA_PORT(t, CTRL_CODE_CMP)

	// SetKeyInputStringColor2 に渡す色変更対象を指定するための識別子
	DXLUA_PORT(t, DX_KEYINPSTRCOLOR_NORMAL_STR)
	DXLUA_PORT(t, DX_KEYINPSTRCOLOR_NORMAL_STR_EDGE)
	DXLUA_PORT(t, DX_KEYINPSTRCOLOR_NORMAL_CURSOR)
	DXLUA_PORT(t, DX_KEYINPSTRCOLOR_SELECT_STR)
	DXLUA_PORT(t, DX_KEYINPSTRCOLOR_SELECT_STR_EDGE)
	DXLUA_PORT(t, DX_KEYINPSTRCOLOR_SELECT_STR_BACK)
	DXLUA_PORT(t, DX_KEYINPSTRCOLOR_IME_STR)
	DXLUA_PORT(t, DX_KEYINPSTRCOLOR_IME_STR_EDGE)
	DXLUA_PORT(t, DX_KEYINPSTRCOLOR_IME_STR_BACK)
	DXLUA_PORT(t, DX_KEYINPSTRCOLOR_IME_CURSOR)
	DXLUA_PORT(t, DX_KEYINPSTRCOLOR_IME_LINE)
	DXLUA_PORT(t, DX_KEYINPSTRCOLOR_IME_SELECT_STR)
	DXLUA_PORT(t, DX_KEYINPSTRCOLOR_IME_SELECT_STR_EDGE)
	DXLUA_PORT(t, DX_KEYINPSTRCOLOR_IME_SELECT_STR_BACK)
	DXLUA_PORT(t, DX_KEYINPSTRCOLOR_IME_CONV_WIN_STR)
	DXLUA_PORT(t, DX_KEYINPSTRCOLOR_IME_CONV_WIN_STR_EDGE)
	DXLUA_PORT(t, DX_KEYINPSTRCOLOR_IME_CONV_WIN_SELECT_STR)
	DXLUA_PORT(t, DX_KEYINPSTRCOLOR_IME_CONV_WIN_SELECT_STR_EDGE)
	DXLUA_PORT(t, DX_KEYINPSTRCOLOR_IME_CONV_WIN_SELECT_STR_BACK)
	DXLUA_PORT(t, DX_KEYINPSTRCOLOR_IME_CONV_WIN_EDGE)
	DXLUA_PORT(t, DX_KEYINPSTRCOLOR_IME_CONV_WIN_BACK)
	DXLUA_PORT(t, DX_KEYINPSTRCOLOR_IME_MODE_STR)
	DXLUA_PORT(t, DX_KEYINPSTRCOLOR_IME_MODE_STR_EDGE)
	DXLUA_PORT(t, DX_KEYINPSTRCOLOR_NUM)

	// 文字列入力処理の入力文字数が限界に達している状態で、文字列の末端部分で入力が行われた場合の処理モード
	DXLUA_PORT(t, DX_KEYINPSTR_ENDCHARAMODE_OVERWRITE)
	DXLUA_PORT(t, DX_KEYINPSTR_ENDCHARAMODE_NOTCHANGE)

	// フルスクリーン解像度モード定義
	DXLUA_PORT(t, DX_FSRESOLUTIONMODE_DESKTOP)
	DXLUA_PORT(t, DX_FSRESOLUTIONMODE_NATIVE)
	DXLUA_PORT(t, DX_FSRESOLUTIONMODE_MAXIMUM)

	// フルスクリーン拡大モード定義
	DXLUA_PORT(t, DX_FSSCALINGMODE_BILINEAR)
	DXLUA_PORT(t, DX_FSSCALINGMODE_NEAREST)

	// SetGraphMode 戻り値定義
	DXLUA_PORT(t, DX_CHANGESCREEN_OK)
	DXLUA_PORT(t, DX_CHANGESCREEN_RETURN)
	DXLUA_PORT(t, DX_CHANGESCREEN_DEFAULT)
	DXLUA_PORT(t, DX_CHANGESCREEN_REFRESHNORMAL)

	// ストリームデータ読み込み処理コード簡略化関連
#if 0 // TODO: 関数として登録
	DXLUA_PORT(t, STTELL)
	DXLUA_PORT(t, STSEEK)
	DXLUA_PORT(t, STREAD)
	DXLUA_PORT(t, STWRITE)
	DXLUA_PORT(t, STEOF)
	DXLUA_PORT(t, STCLOSE)
#endif
	// ストリームデータ制御のシークタイプ定義
	DXLUA_PORT(t, STREAM_SEEKTYPE_SET)
	DXLUA_PORT(t, STREAM_SEEKTYPE_END)
	DXLUA_PORT(t, STREAM_SEEKTYPE_CUR)

	// グラフィックロード時のイメージタイプ
	DXLUA_PORT(t, LOADIMAGE_TYPE_FILE)
	DXLUA_PORT(t, LOADIMAGE_TYPE_MEM)
	DXLUA_PORT(t, LOADIMAGE_TYPE_NONE)

#ifndef DX_NON_NETWORK

	// HTTP エラー
	DXLUA_PORT(t, HTTP_ERR_SERVER)
	DXLUA_PORT(t, HTTP_ERR_NOTFOUND)
	DXLUA_PORT(t, HTTP_ERR_MEMORY)
	DXLUA_PORT(t, HTTP_ERR_LOST)
	DXLUA_PORT(t, HTTP_ERR_NONE)

	// HTTP 処理の結果
	DXLUA_PORT(t, HTTP_RES_COMPLETE)
	DXLUA_PORT(t, HTTP_RES_STOP)
	DXLUA_PORT(t, HTTP_RES_ERROR)
	DXLUA_PORT(t, HTTP_RES_NOW)

#endif // DX_NON_NETWORK
}

} // namespace DxLua::detail