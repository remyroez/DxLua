#include <tuple>

#include <DxLib.h>
#include <lua.hpp>
#include <sol/sol.hpp>

#include "luaapi.h"

namespace DxLua::detail {

void port_model(sol::state_view &lua, sol::table &t) {
#ifndef DX_NON_MODEL

	// モデルの読み込み・複製関係
	DXLUA_PORT(t, MV1LoadModel);
	//extern	int			MV1LoadModelWithStrLen(const TCHAR * FileName, size_t FileNameLength);					// モデルの読み込み( -1:エラー  0以上:モデルハンドル )
	//extern	int			MV1LoadModelFromMem(const void *FileImage, int FileSize, int (*FileReadFunc)(const TCHAR * FilePath, void **FileImageAddr, int *FileSize, void *FileReadFuncData), int (*FileReleaseFunc)(void *MemoryAddr, void *FileReadFuncData), void *FileReadFuncData = NULL);	// メモリ上のモデルファイルイメージと独自の読み込みルーチンを使用してモデルを読み込む
	DXLUA_PORT(t, MV1DuplicateModel);
	DXLUA_PORT(t, MV1CreateCloneModel);

	DXLUA_PORT(t, MV1DeleteModel);
	DXLUA_PORT(t, MV1InitModel);

	//extern	int			MV1SetLoadModelReMakeNormal(int Flag);														// モデルを読み込む際に法線の再計算を行うかどうかを設定する( TRUE:行う  FALSE:行わない( デフォルト ) )
	//extern	int			MV1SetLoadModelReMakeNormalSmoothingAngle(float SmoothingAngle = 1.562069f);								// モデルを読み込む際に行う法泉の再計算で使用するスムージング角度を設定する( 単位はラジアン )
	//extern	int			MV1SetLoadModelIgnoreScaling(int Flag);														// モデルを読み込む際にスケーリングデータを無視するかどうかを設定する( TRUE:無視する  FALSE:無視しない( デフォルト ) )
	//extern	int			MV1SetLoadModelPositionOptimize(int Flag);														// モデルを読み込む際に座標データの最適化を行うかどうかを設定する( TRUE:行う  FALSE:行わない( デフォルト ) )
	//extern	int			MV1SetLoadModelNotEqNormalSide_AddZeroAreaPolygon(int Flag);														// モデルを読み込む際にポリゴンの辺が接していて、且つ法線の方向が異なる辺に面積０のポリゴンを埋め込むかどうかを設定する( TRUE:埋め込む　FALSE:埋め込まない( デフォルト ) )、( MV1ファイルの読み込みではこの関数の設定は無視され、ポリゴンの埋め込みは実行されません )
	//extern	int			MV1SetLoadModelUsePhysicsMode(int PhysicsMode /* DX_LOADMODEL_PHYSICS_LOADCALC 等 */);			// 読み込むモデルの物理演算モードを設定する
	//extern	int			MV1SetLoadModelPhysicsWorldGravity(float Gravity);													// 読み込むモデルの物理演算に適用する重力パラメータを設定する
	//extern	float		MV1GetLoadModelPhysicsWorldGravity(void);															// 読み込むモデルの物理演算に適用する重力パラメータを取得する
	//extern	int			MV1SetLoadCalcPhysicsWorldGravity(int GravityNo, VECTOR Gravity);									// 読み込むモデルの物理演算モードが事前計算( DX_LOADMODEL_PHYSICS_LOADCALC )だった場合に適用される重力の設定をする
	//extern	VECTOR		MV1GetLoadCalcPhysicsWorldGravity(int GravityNo);													// 読み込むモデルの物理演算モードが事前計算( DX_LOADMODEL_PHYSICS_LOADCALC )だった場合に適用される重力を取得する
	//extern	int			MV1SetLoadModelPhysicsCalcPrecision(int Precision);													// 読み込むモデルの物理演算モードが事前計算( DX_LOADMODEL_PHYSICS_LOADCALC )だった場合に適用される物理演算の時間進行の精度を設定する( 0:60FPS  1:120FPS  2:240FPS  3:480FPS  4:960FPS  5:1920FPS )
	//extern	int			MV1SetLoadModel_PMD_PMX_AnimationFPSMode(int FPSMode /* DX_LOADMODEL_PMD_PMX_ANIMATION_FPSMODE_30 等 */);	// PMD, PMX ファイルを読み込んだ際のアニメーションの FPS モードを設定する
	//extern	int			MV1AddLoadModelDisablePhysicsNameWord(const TCHAR * NameWord);											// 読み込むモデルの物理演算を特定の剛体のみ無効にするための名前のワードを追加する、追加できるワード文字列の最大長は 63 文字、追加できるワードの数は最大 256 個
	//extern	int			MV1AddLoadModelDisablePhysicsNameWordWithStrLen(const TCHAR * NameWord, size_t NameWordLength);					// 読み込むモデルの物理演算を特定の剛体のみ無効にするための名前のワードを追加する、追加できるワード文字列の最大長は 63 文字、追加できるワードの数は最大 256 個
	//extern	int			MV1ResetLoadModelDisablePhysicsNameWord(void);															// MV1AddLoadModelDisablePhysicsNameWord で追加した剛体の無効ワードをリセットして無効ワード無しの初期状態に戻す
	//extern	int			MV1SetLoadModelDisablePhysicsNameWordMode(int DisableNameWordMode /* DX_LOADMODEL_PHYSICS_DISABLENAMEWORD_ALWAYS 等 */);	// MV1AddLoadModelDisablePhysicsNameWord で追加した剛体の無効ワードの適用ルールを変更する
	//extern	int			MV1SetLoadModelAnimFilePath(const TCHAR * FileName);											// 読み込むモデルに適用するアニメーションファイルのパスを設定する、NULLを渡すと設定リセット( 現在は PMD,PMX のみに効果あり )
	//extern	int			MV1SetLoadModelAnimFilePathWithStrLen(const TCHAR * FileName, size_t FileNameLength);					// 読み込むモデルに適用するアニメーションファイルのパスを設定する、NULLを渡すと設定リセット( 現在は PMD,PMX のみに効果あり )
	//extern	int			MV1SetLoadModelUsePackDraw(int Flag);														// 読み込むモデルを同時複数描画に対応させるかどうかを設定する( TRUE:対応させる  FALSE:対応させない( デフォルト ) )、( 「対応させる」にすると描画が高速になる可能性がある代わりに消費VRAMが増えます )
	//extern	int			MV1SetLoadModelTriangleListUseMaxBoneNum(int UseMaxBoneNum);												// 読み込むモデルのひとつのトライアングルリストで使用できる最大ボーン数を設定する( UseMaxBoneNum で指定できる値の範囲は 8 ～ 54、 0 を指定するとデフォルト動作に戻る )

	// モデル保存関係
	//extern	int			MV1SaveModelToMV1File(int MHandle, const TCHAR * FileName, int SaveType = MV1_SAVETYPE_NORMAL, int AnimMHandle = -1, int AnimNameCheck = TRUE, int Normal8BitFlag = 1, int Position16BitFlag = 1, int Weight8BitFlag = 0, int Anim16BitFlag = 1);		// 指定のパスにモデルを保存する( 戻り値  0:成功  -1:メモリ不足  -2:使われていないアニメーションがあった )
	//extern	int			MV1SaveModelToMV1FileWithStrLen(int MHandle, const TCHAR * FileName, size_t FileNameLength, int SaveType = MV1_SAVETYPE_NORMAL, int AnimMHandle = -1, int AnimNameCheck = TRUE, int Normal8BitFlag = 1, int Position16BitFlag = 1, int Weight8BitFlag = 0, int Anim16BitFlag = 1);		// 指定のパスにモデルを保存する( 戻り値  0:成功  -1:メモリ不足  -2:使われていないアニメーションがあった )
#ifndef DX_NON_SAVEFUNCTION
	//extern	int			MV1SaveModelToXFile(int MHandle, const TCHAR * FileName, int SaveType = MV1_SAVETYPE_NORMAL, int AnimMHandle = -1, int AnimNameCheck = TRUE);	// 指定のパスにモデルをＸファイル形式で保存する( 戻り値  0:成功  -1:メモリ不足  -2:使われていないアニメーションがあった )
	//extern	int			MV1SaveModelToXFileWithStrLen(int MHandle, const TCHAR * FileName, size_t FileNameLength, int SaveType = MV1_SAVETYPE_NORMAL, int AnimMHandle = -1, int AnimNameCheck = TRUE);	// 指定のパスにモデルをＸファイル形式で保存する( 戻り値  0:成功  -1:メモリ不足  -2:使われていないアニメーションがあった )
#endif // DX_NON_SAVEFUNCTION

	// モデル描画関係
	DXLUA_PORT(t, MV1DrawModel);
	DXLUA_PORT(t, MV1DrawFrame);
	DXLUA_PORT(t, MV1DrawMesh);
	DXLUA_PORT(t, MV1DrawTriangleList);
	DXLUA_PORT(t, MV1DrawModelDebug);

	// 描画設定関係
	//extern	int			MV1SetUseOrigShader(int UseFlag);														// モデルの描画に SetUseVertexShader, SetUsePixelShader で指定したシェーダーを使用するかどうかを設定する( TRUE:使用する  FALSE:使用しない( デフォルト ) )
	//extern	int			MV1SetSemiTransDrawMode(int DrawMode /* DX_SEMITRANSDRAWMODE_ALWAYS 等 */);				// モデルの半透明要素がある部分についての描画モードを設定する

	// モデル基本制御関係
	//extern	MATRIX		MV1GetLocalWorldMatrix(int MHandle);														// モデルのローカル座標からワールド座標に変換する行列を得る
	//extern	MATRIX_D	MV1GetLocalWorldMatrixD(int MHandle);														// モデルのローカル座標からワールド座標に変換する行列を得る
	DXLUA_PORT(t, MV1SetPosition);
	//extern	int			MV1SetPositionD(int MHandle, VECTOR_D Position);									// モデルの座標をセット
	//extern	VECTOR		MV1GetPosition(int MHandle);														// モデルの座標を取得
	//extern	VECTOR_D	MV1GetPositionD(int MHandle);														// モデルの座標を取得
	//extern	int			MV1SetScale(int MHandle, VECTOR Scale);										// モデルの拡大値をセット
	//extern	VECTOR		MV1GetScale(int MHandle);														// モデルの拡大値を取得
	DXLUA_PORT(t, MV1SetRotationXYZ);
	DXLUA_PORT(t, MV1GetRotationXYZ);
	//extern	int			MV1SetRotationZYAxis(int MHandle, VECTOR ZAxisDirection, VECTOR YAxisDirection, float ZAxisTwistRotate);	// モデルのＺ軸とＹ軸の向きをセットする
	//extern	int			MV1SetRotationYUseDir(int MHandle, VECTOR Direction, float OffsetYAngle);				// モデルのＹ軸の回転値を指定のベクトルの向きを元に設定する、モデルはZ軸のマイナス方向を向いていることを想定するので、そうではない場合は OffsetYAngle で補正する、Ｘ軸回転、Ｚ軸回転は０で固定
	//extern	int			MV1SetRotationMatrix(int MHandle, MATRIX Matrix);										// モデルの回転用行列をセットする
	//extern	MATRIX		MV1GetRotationMatrix(int MHandle);														// モデルの回転用行列を取得する
	//extern	int			MV1SetMatrix(int MHandle, MATRIX   Matrix);									// モデルの変形用行列をセットする
	//extern	int			MV1SetMatrixD(int MHandle, MATRIX_D Matrix);									// モデルの変形用行列をセットする
	//extern	MATRIX		MV1GetMatrix(int MHandle);														// モデルの変形用行列を取得する
	//extern	MATRIX_D	MV1GetMatrixD(int MHandle);														// モデルの変形用行列を取得する
	//extern	int			MV1SetVisible(int MHandle, int VisibleFlag);									// モデルの表示、非表示状態を変更する( TRUE:表示  FALSE:非表示 )
	//extern	int			MV1GetVisible(int MHandle);														// モデルの表示、非表示状態を取得する( TRUE:表示  FALSE:非表示 )
	//extern	int			MV1SetMeshCategoryVisible(int MHandle, int MeshCategory, int VisibleFlag);					// モデルのメッシュの種類( DX_MV1_MESHCATEGORY_NORMAL など )毎の表示、非表示を設定する( TRUE:表示  FALSE:非表示 )
	//extern	int			MV1GetMeshCategoryVisible(int MHandle, int MeshCategory);									// モデルのメッシュの種類( DX_MV1_MESHCATEGORY_NORMAL など )毎の表示、非表示を取得する( TRUE:表示  FALSE:非表示 )
	//extern	int			MV1SetDifColorScale(int MHandle, COLOR_F Scale);										// モデルのディフューズカラーのスケール値を設定する( デフォルト値は 1.0f )
	//extern	COLOR_F		MV1GetDifColorScale(int MHandle);														// モデルのディフューズカラーのスケール値を取得する( デフォルト値は 1.0f )
	//extern	int			MV1SetSpcColorScale(int MHandle, COLOR_F Scale);										// モデルのスペキュラカラーのスケール値を設定する( デフォルト値は 1.0f )
	//extern	COLOR_F		MV1GetSpcColorScale(int MHandle);														// モデルのスペキュラカラーのスケール値を取得する( デフォルト値は 1.0f )
	//extern	int			MV1SetEmiColorScale(int MHandle, COLOR_F Scale);										// モデルのエミッシブカラーのスケール値を設定する( デフォルト値は 1.0f )
	//extern	COLOR_F		MV1GetEmiColorScale(int MHandle);														// モデルのエミッシブカラーのスケール値を取得する( デフォルト値は 1.0f )
	//extern	int			MV1SetAmbColorScale(int MHandle, COLOR_F Scale);										// モデルのアンビエントカラーのスケール値を設定する( デフォルト値は 1.0f )
	//extern	COLOR_F		MV1GetAmbColorScale(int MHandle);														// モデルのアンビエントカラーのスケール値を取得する( デフォルト値は 1.0f )
	//extern	int			MV1GetSemiTransState(int MHandle);														// モデルに半透明要素があるかどうかを取得する( 戻り値 TRUE:ある  FALSE:ない )
	//extern	int			MV1SetOpacityRate(int MHandle, float Rate);											// モデルの不透明度を設定する( 不透明 1.0f ～ 透明 0.0f )
	//extern	float		MV1GetOpacityRate(int MHandle);														// モデルの不透明度を取得する( 不透明 1.0f ～ 透明 0.0f )
	//extern	int			MV1SetUseDrawMulAlphaColor(int MHandle, int Flag);											// モデルを描画する際にRGB値に対してA値を乗算するかどうかを設定する( 描画結果が乗算済みアルファ画像になります )( Flag   TRUE:RGB値に対してA値を乗算する  FALSE:乗算しない(デフォルト) )
	//extern	int			MV1GetUseDrawMulAlphaColor(int MHandle);														// モデルを描画する際にRGB値に対してA値を乗算するかどうかを取得する( 描画結果が乗算済みアルファ画像になります )( 戻り値 TRUE:RGB値に対してA値を乗算する  FALSE:乗算しない(デフォルト) )
	//extern	int			MV1SetUseZBuffer(int MHandle, int Flag);											// モデルを描画する際にＺバッファを使用するかどうかを設定する
	//extern	int			MV1SetWriteZBuffer(int MHandle, int Flag);											// モデルを描画する際にＺバッファに書き込みを行うかどうかを設定する
	//extern	int			MV1SetZBufferCmpType(int MHandle, int CmpType /* DX_CMP_NEVER 等 */);					// モデルの描画時のＺ値の比較モードを設定する
	//extern	int			MV1SetZBias(int MHandle, int Bias);											// モデルの描画時の書き込むＺ値のバイアスを設定する
	//extern	int			MV1SetUseVertDifColor(int MHandle, int UseFlag);										// モデルの含まれるメッシュの頂点ディフューズカラーをマテリアルのディフューズカラーの代わりに使用するかどうかを設定する( TRUE:マテリアルカラーの代わりに使用する  FALSE:マテリアルカラーを使用する )
	//extern	int			MV1SetUseVertSpcColor(int MHandle, int UseFlag);										// モデルに含まれるメッシュの頂点スペキュラカラーをマテリアルのスペキュラカラーの代わりに使用するかどうかを設定する( TRUE:マテリアルカラーの代わりに使用する  FALSE:マテリアルカラーを使用する )
	//extern	int			MV1SetSampleFilterMode(int MHandle, int FilterMode);										// モデルのテクスチャのサンプルフィルターモードを変更する( FilterMode は DX_DRAWMODE_NEAREST 等 )
	//extern	int			MV1SetMaxAnisotropy(int MHandle, int MaxAnisotropy);									// モデルの異方性フィルタリングの最大次数を設定する
	//extern	int			MV1SetWireFrameDrawFlag(int MHandle, int Flag);											// モデルをワイヤーフレームで描画するかどうかを設定する
	//extern	int			MV1RefreshVertColorFromMaterial(int MHandle);														// モデルの頂点カラーを現在設定されているマテリアルのカラーにする
	//extern	int			MV1SetPhysicsWorldGravity(int MHandle, VECTOR Gravity);										// モデルの物理演算の重力を設定する
	//extern	int			MV1PhysicsCalculation(int MHandle, float MillisecondTime);								// モデルの物理演算を指定時間分経過したと仮定して計算する( MillisecondTime で指定する時間の単位はミリ秒 )
	//extern	int			MV1PhysicsResetState(int MHandle);														// モデルの物理演算の状態をリセットする( 位置がワープしたとき用 )
	//extern	int			MV1SetUseShapeFlag(int MHandle, int UseFlag);										// モデルのシェイプ機能を使用するかどうかを設定する( UseFlag  TRUE:使用する( デフォルト )  FALSE:使用しない )
	//extern	int			MV1GetMaterialNumberOrderFlag(int MHandle);														// モデルのマテリアル番号順にメッシュを描画するかどうかのフラグを取得する( TRUE:マテリアル番号順に描画  FALSE:不透明メッシュの後半透明メッシュ )

	// アニメーション関係
	t["MV1AttachAnim"] = [](int MHandle, int AnimIndex, sol::variadic_args va) {
		int AnimSrcMHandle = va_get(va, 0, -1);
		int NameCheck = va_get(va, 1, true);
		return MV1AttachAnim(MHandle, AnimIndex, AnimSrcMHandle, NameCheck);
	};
	DXLUA_PORT(t, MV1DetachAnim);
	DXLUA_PORT(t, MV1SetAttachAnimTime);
	DXLUA_PORT(t, MV1GetAttachAnimTime);
	DXLUA_PORT(t, MV1GetAttachAnimTotalTime);
	t["MV1SetAttachAnimBlendRate"] = [](int MHandle, int AttachIndex, sol::variadic_args va) {
		float Rate = va_get(va, 0, 1.0f);
		return MV1SetAttachAnimBlendRate(MHandle, AttachIndex, Rate);
	};
	DXLUA_PORT(t, MV1GetAttachAnimBlendRate);
	t["MV1SetAttachAnimBlendRateToFrame"] = [](int MHandle, int AttachIndex, int FrameIndex, float Rate, sol::variadic_args va) {
		int SetChild = va_get(va, 0, true);
		return MV1SetAttachAnimBlendRateToFrame(MHandle, AttachIndex, FrameIndex, Rate, SetChild);
	};
	DXLUA_PORT(t, MV1GetAttachAnimBlendRateToFrame);
	DXLUA_PORT(t, MV1GetAttachAnim);
	t["MV1SetAttachAnimUseShapeFlag"] = [](int MHandle, int AttachIndex, sol::variadic_args va) {
		int UseFlag = va_get(va, 0, false);
		return MV1SetAttachAnimUseShapeFlag(MHandle, AttachIndex, UseFlag);
	};
	DXLUA_PORT(t, MV1GetAttachAnimUseShapeFlag);
	DXLUA_PORT(t, MV1GetAttachAnimFrameLocalPosition);
	DXLUA_PORT(t, MV1GetAttachAnimFrameLocalMatrix);

	//extern	int			MV1GetAnimNum(int MHandle);																		// アニメーションの数を取得する
	//extern	const TCHAR *MV1GetAnimName(int MHandle, int AnimIndex);														// 指定番号のアニメーション名を取得する( NULL:エラー )
	//extern	int			MV1SetAnimName(int MHandle, int AnimIndex, const TCHAR *AnimName);		// 指定番号のアニメーション名を変更する
	//extern	int			MV1SetAnimNameWithStrLen(int MHandle, int AnimIndex, const TCHAR *AnimName, size_t AnimNameLength);		// 指定番号のアニメーション名を変更する
	//extern	int			MV1GetAnimIndex(int MHandle, const TCHAR *AnimName);						// 指定名のアニメーション番号を取得する( -1:エラー )
	//extern	int			MV1GetAnimIndexWithStrLen(int MHandle, const TCHAR *AnimName, size_t AnimNameLength);						// 指定名のアニメーション番号を取得する( -1:エラー )
	//extern	float		MV1GetAnimTotalTime(int MHandle, int AnimIndex);														// 指定番号のアニメーションの総時間を得る
	//extern	int			MV1GetAnimTargetFrameNum(int MHandle, int AnimIndex);														// 指定のアニメーションがターゲットとするフレームの数を取得する
	//extern	const TCHAR *MV1GetAnimTargetFrameName(int MHandle, int AnimIndex, int AnimFrameIndex);									// 指定のアニメーションがターゲットとするフレームの名前を取得する
	//extern	int			MV1GetAnimTargetFrame(int MHandle, int AnimIndex, int AnimFrameIndex);									// 指定のアニメーションがターゲットとするフレームの番号を取得する
	//extern	int			MV1GetAnimTargetFrameKeySetNum(int MHandle, int AnimIndex, int AnimFrameIndex);									// 指定のアニメーションがターゲットとするフレーム用のアニメーションキーセットの数を取得する
	//extern	int			MV1GetAnimTargetFrameKeySet(int MHandle, int AnimIndex, int AnimFrameIndex, int Index);						// 指定のアニメーションがターゲットとするフレーム用のアニメーションキーセットキーセットインデックスを取得する

	//extern	int			MV1GetAnimKeySetNum(int MHandle);																		// モデルに含まれるアニメーションキーセットの総数を得る
	//extern	int			MV1GetAnimKeySetType(int MHandle, int AnimKeySetIndex);												// 指定のアニメーションキーセットのタイプを取得する( MV1_ANIMKEY_TYPE_QUATERNION 等 )
	//extern	int			MV1GetAnimKeySetDataType(int MHandle, int AnimKeySetIndex);												// 指定のアニメーションキーセットのデータタイプを取得する( MV1_ANIMKEY_DATATYPE_ROTATE 等 )
	//extern	int			MV1GetAnimKeySetTimeType(int MHandle, int AnimKeySetIndex);												// 指定のアニメーションキーセットのキーの時間データタイプを取得する( MV1_ANIMKEY_TIME_TYPE_ONE 等 )
	//extern	int			MV1GetAnimKeySetDataNum(int MHandle, int AnimKeySetIndex);												// 指定のアニメーションキーセットのキーの数を取得する
	//extern	float		MV1GetAnimKeyDataTime(int MHandle, int AnimKeySetIndex, int Index);										// 指定のアニメーションキーセットのキーの時間を取得する
	//extern	int			MV1GetAnimKeyDataIndexFromTime(int MHandle, int AnimKeySetIndex, float Time);									// 指定のアニメーションキーセットの指定の時間でのキーの番号を取得する
	//extern	FLOAT4		MV1GetAnimKeyDataToQuaternion(int MHandle, int AnimKeySetIndex, int Index);										// 指定のアニメーションキーセットのキーを取得する、キータイプが MV1_ANIMKEY_TYPE_QUATERNION では無かった場合は失敗する
	//extern	FLOAT4		MV1GetAnimKeyDataToQuaternionFromTime(int MHandle, int AnimKeySetIndex, float Time);									// 指定のアニメーションキーセットのキーを取得する、キータイプが MV1_ANIMKEY_TYPE_QUATERNION では無かった場合は失敗する( 時間指定版 )
	//extern	VECTOR		MV1GetAnimKeyDataToVector(int MHandle, int AnimKeySetIndex, int Index);										// 指定のアニメーションキーセットのキーを取得する、キータイプが MV1_ANIMKEY_TYPE_VECTOR では無かった場合は失敗する
	//extern	VECTOR		MV1GetAnimKeyDataToVectorFromTime(int MHandle, int AnimKeySetIndex, float Time);									// 指定のアニメーションキーセットのキーを取得する、キータイプが MV1_ANIMKEY_TYPE_VECTOR では無かった場合は失敗する( 時間指定版 )
	//extern	MATRIX		MV1GetAnimKeyDataToMatrix(int MHandle, int AnimKeySetIndex, int Index);										// 指定のアニメーションキーセットのキーを取得する、キータイプが MV1_ANIMKEY_TYPE_MATRIX4X4C か MV1_ANIMKEY_TYPE_MATRIX3X3 では無かった場合は失敗する
	//extern	MATRIX		MV1GetAnimKeyDataToMatrixFromTime(int MHandle, int AnimKeySetIndex, float Time);									// 指定のアニメーションキーセットのキーを取得する、キータイプが MV1_ANIMKEY_TYPE_MATRIX4X4C か MV1_ANIMKEY_TYPE_MATRIX3X3 では無かった場合は失敗する( 時間指定版 )
	//extern	float		MV1GetAnimKeyDataToFlat(int MHandle, int AnimKeySetIndex, int Index);										// 指定のアニメーションキーセットのキーを取得する、キータイプが MV1_ANIMKEY_TYPE_FLAT では無かった場合は失敗する
	//extern	float		MV1GetAnimKeyDataToFlatFromTime(int MHandle, int AnimKeySetIndex, float Time);									// 指定のアニメーションキーセットのキーを取得する、キータイプが MV1_ANIMKEY_TYPE_FLAT では無かった場合は失敗する( 時間指定版 )
	//extern	float		MV1GetAnimKeyDataToLinear(int MHandle, int AnimKeySetIndex, int Index);										// 指定のアニメーションキーセットのキーを取得する、キータイプが MV1_ANIMKEY_TYPE_LINEAR では無かった場合は失敗する
	//extern	float		MV1GetAnimKeyDataToLinearFromTime(int MHandle, int AnimKeySetIndex, float Time);									// 指定のアニメーションキーセットのキーを取得する、キータイプが MV1_ANIMKEY_TYPE_LINEAR では無かった場合は失敗する( 時間指定版 )

	// マテリアル関係
	//extern	int			MV1GetMaterialNum(int MHandle);															// モデルで使用しているマテリアルの数を取得する
	//extern	const TCHAR *MV1GetMaterialName(int MHandle, int MaterialIndex);										// 指定のマテリアルの名前を取得する
	//extern	int			MV1SetMaterialTypeAll(int MHandle, int Type);							// 全てのマテリアルのタイプを変更する( Type : DX_MATERIAL_TYPE_NORMAL など )
	//extern	int			MV1SetMaterialType(int MHandle, int MaterialIndex, int Type);							// 指定のマテリアルのタイプを変更する( Type : DX_MATERIAL_TYPE_NORMAL など )
	//extern	int			MV1GetMaterialType(int MHandle, int MaterialIndex);										// 指定のマテリアルのタイプを取得する( 戻り値 : DX_MATERIAL_TYPE_NORMAL など )
	//extern	int			MV1SetMaterialTypeParamAll(int MHandle, ...);									// 全てのマテリアルのタイプ別パラメータを変更する( マテリアルタイプ DX_MATERIAL_TYPE_MAT_SPEC_LUMINANCE_TWO_COLOR などで使用 )
	//extern	int			MV1SetMaterialTypeParam(int MHandle, int MaterialIndex, ...);									// 指定のマテリアルのタイプ別パラメータを変更する( マテリアルタイプ DX_MATERIAL_TYPE_MAT_SPEC_LUMINANCE_TWO_COLOR などで使用 )
	////		int			MV1SetMaterialTypeParam(			int MHandle, int MaterialIndex, /* DX_MATERIAL_TYPE_MAT_SPEC_LUMINANCE_UNORM            の場合 */ float MinParam = 正規化の下限値( この値以下が 0.0f になる ), float MaxParam = 正規化の上限値( この値以上が 1.0f になる ) ) ;
	////		int			MV1SetMaterialTypeParam(			int MHandle, int MaterialIndex, /* DX_MATERIAL_TYPE_MAT_SPEC_LUMINANCE_CLIP_UNORM       の場合 */ float MinParam = 正規化の下限値( この値以下が 0.0f になる ), float MaxParam = 正規化の上限値( この値以上が 1.0f になる ), float ClipParam = 閾値( この値未満が 0.0f になる ) ) ;
	////		int			MV1SetMaterialTypeParam(			int MHandle, int MaterialIndex, /* DX_MATERIAL_TYPE_MAT_SPEC_LUMINANCE_CMP_GREATEREQUAL の場合 */ float CmpParam = 比較値( この値以上の場合は 1.0f が、未満の場合は 0.0f が書き込まれる ) ) ;
	////		int			MV1SetMaterialTypeParam(			int MHandle, int MaterialIndex, /* DX_MATERIAL_TYPE_MAT_SPEC_POWER_UNORM                の場合 */ float MinParam = 正規化の下限値( この値以下が 0.0f になる ), float MaxParam = 正規化の上限値( この値以上が 1.0f になる ) ) ;
	////		int			MV1SetMaterialTypeParam(			int MHandle, int MaterialIndex, /* DX_MATERIAL_TYPE_MAT_SPEC_POWER_CLIP_UNORM           の場合 */ float MinParam = 正規化の下限値( この値以下が 0.0f になる ), float MaxParam = 正規化の上限値( この値以上が 1.0f になる ), float ClipParam = 閾値( この値未満が 0.0f になる ) ) ;
	////		int			MV1SetMaterialTypeParam(			int MHandle, int MaterialIndex, /* DX_MATERIAL_TYPE_MAT_SPEC_POWER_CMP_GREATEREQUAL     の場合 */ float CmpParam = 比較値( この値以上の場合は 1.0f が、未満の場合は 0.0f が書き込まれる ) ) ;
	//extern	int			MV1SetMaterialDifColor(int MHandle, int MaterialIndex, COLOR_F Color);						// 指定のマテリアルのディフューズカラーを設定する
	//extern	COLOR_F		MV1GetMaterialDifColor(int MHandle, int MaterialIndex);										// 指定のマテリアルのディフューズカラーを取得する
	//extern	int			MV1SetMaterialSpcColor(int MHandle, int MaterialIndex, COLOR_F Color);						// 指定のマテリアルのスペキュラカラーを設定する
	//extern	COLOR_F		MV1GetMaterialSpcColor(int MHandle, int MaterialIndex);										// 指定のマテリアルのスペキュラカラーを取得する
	//extern	int			MV1SetMaterialEmiColor(int MHandle, int MaterialIndex, COLOR_F Color);						// 指定のマテリアルのエミッシブカラーを設定する
	//extern	COLOR_F		MV1GetMaterialEmiColor(int MHandle, int MaterialIndex);										// 指定のマテリアルのエミッシブカラーを取得する
	//extern	int			MV1SetMaterialAmbColor(int MHandle, int MaterialIndex, COLOR_F Color);						// 指定のマテリアルのアンビエントカラーを設定する
	//extern	COLOR_F		MV1GetMaterialAmbColor(int MHandle, int MaterialIndex);										// 指定のマテリアルのアンビエントカラーを取得する
	//extern	int			MV1SetMaterialSpcPower(int MHandle, int MaterialIndex, float Power);							// 指定のマテリアルのスペキュラの強さを設定する
	//extern	float		MV1GetMaterialSpcPower(int MHandle, int MaterialIndex);										// 指定のマテリアルのスペキュラの強さを取得する
	//extern	int			MV1SetMaterialDifMapTexture(int MHandle, int MaterialIndex, int TexIndex);						// 指定のマテリアルでディフューズマップとして使用するテクスチャを指定する
	//extern	int			MV1GetMaterialDifMapTexture(int MHandle, int MaterialIndex);										// 指定のマテリアルでディフューズマップとして使用されているテクスチャのインデックスを取得する
	//extern	int			MV1SetMaterialSpcMapTexture(int MHandle, int MaterialIndex, int TexIndex);						// 指定のマテリアルでスペキュラマップとして使用するテクスチャを指定する
	//extern	int			MV1GetMaterialSpcMapTexture(int MHandle, int MaterialIndex);										// 指定のマテリアルでスペキュラマップとして使用されているテクスチャのインデックスを取得する
	//extern	int			MV1GetMaterialNormalMapTexture(int MHandle, int MaterialIndex);										// 指定のマテリアルで法線マップとして使用されているテクスチャのインデックスを取得する
	//extern	int			MV1SetMaterialDifGradTexture(int MHandle, int MaterialIndex, int TexIndex);						// 指定のマテリアルでトゥーンレンダリングのディフューズグラデーションマップとして使用するテクスチャを設定する
	//extern	int			MV1GetMaterialDifGradTexture(int MHandle, int MaterialIndex);										// 指定のマテリアルでトゥーンレンダリングのディフューズグラデーションマップとして使用するテクスチャを取得する
	//extern	int			MV1SetMaterialSpcGradTexture(int MHandle, int MaterialIndex, int TexIndex);						// 指定のマテリアルでトゥーンレンダリングのスペキュラグラデーションマップとして使用するテクスチャを設定する
	//extern	int			MV1GetMaterialSpcGradTexture(int MHandle, int MaterialIndex);										// 指定のマテリアルでトゥーンレンダリングのスペキュラグラデーションマップとして使用するテクスチャを取得する
	//extern	int			MV1SetMaterialSphereMapTexture(int MHandle, int MaterialIndex, int TexIndex);						// 指定のマテリアルでトゥーンレンダリングのスフィアマップとして使用するテクスチャを設定する
	//extern	int			MV1GetMaterialSphereMapTexture(int MHandle, int MaterialIndex);										// 指定のマテリアルでトゥーンレンダリングのスフィアマップとして使用するテクスチャを取得する
	//extern	int			MV1SetMaterialDifGradBlendTypeAll(int MHandle, int BlendType);						// 全てのマテリアルのトゥーンレンダリングで使用するディフューズグラデーションマップとディフューズカラーの合成方法を設定する( DX_MATERIAL_BLENDTYPE_ADDITIVE など )
	//extern	int			MV1SetMaterialDifGradBlendType(int MHandle, int MaterialIndex, int BlendType);						// 指定のマテリアルのトゥーンレンダリングで使用するディフューズグラデーションマップとディフューズカラーの合成方法を設定する( DX_MATERIAL_BLENDTYPE_ADDITIVE など )
	//extern	int			MV1GetMaterialDifGradBlendType(int MHandle, int MaterialIndex);										// 指定のマテリアルのトゥーンレンダリングで使用するディフューズグラデーションマップとディフューズカラーの合成方法を取得する( DX_MATERIAL_BLENDTYPE_ADDITIVE など )
	//extern	int			MV1SetMaterialSpcGradBlendTypeAll(int MHandle, int BlendType);						// 全てのマテリアルのトゥーンレンダリングで使用するスペキュラグラデーションマップとスペキュラカラーの合成方法を設定する( DX_MATERIAL_BLENDTYPE_ADDITIVE など )
	//extern	int			MV1SetMaterialSpcGradBlendType(int MHandle, int MaterialIndex, int BlendType);						// 指定のマテリアルのトゥーンレンダリングで使用するスペキュラグラデーションマップとスペキュラカラーの合成方法を設定する( DX_MATERIAL_BLENDTYPE_ADDITIVE など )
	//extern	int			MV1GetMaterialSpcGradBlendType(int MHandle, int MaterialIndex);										// 指定のマテリアルのトゥーンレンダリングで使用するスペキュラグラデーションマップとスペキュラカラーの合成方法を取得する( DX_MATERIAL_BLENDTYPE_ADDITIVE など )
	//extern	int			MV1SetMaterialSphereMapBlendTypeAll(int MHandle, int BlendType);						// 全てのマテリアルのトゥーンレンダリングで使用するスフィアマップの合成方法を設定する( DX_MATERIAL_BLENDTYPE_ADDITIVE など )
	//extern	int			MV1SetMaterialSphereMapBlendType(int MHandle, int MaterialIndex, int BlendType);						// 指定のマテリアルのトゥーンレンダリングで使用するスフィアマップの合成方法を設定する( DX_MATERIAL_BLENDTYPE_ADDITIVE など )
	//extern	int			MV1GetMaterialSphereMapBlendType(int MHandle, int MaterialIndex);										// 指定のマテリアルのトゥーンレンダリングで使用するスフィアマップの合成方法を取得する( DX_MATERIAL_BLENDTYPE_ADDITIVE など )
	//extern	int			MV1SetMaterialOutLineWidthAll(int MHandle, float Width);							// 全てのマテリアルのトゥーンレンダリングで使用する輪郭線の太さを設定する
	//extern	int			MV1SetMaterialOutLineWidth(int MHandle, int MaterialIndex, float Width);							// 指定のマテリアルのトゥーンレンダリングで使用する輪郭線の太さを設定する
	//extern	float		MV1GetMaterialOutLineWidth(int MHandle, int MaterialIndex);										// 指定のマテリアルのトゥーンレンダリングで使用する輪郭線の太さを取得する
	//extern	int			MV1SetMaterialOutLineDotWidthAll(int MHandle, float Width);							// 全てのマテリアルのトゥーンレンダリングで使用する輪郭線のドット単位の太さを設定する
	//extern	int			MV1SetMaterialOutLineDotWidth(int MHandle, int MaterialIndex, float Width);							// 指定のマテリアルのトゥーンレンダリングで使用する輪郭線のドット単位の太さを設定する
	//extern	float		MV1GetMaterialOutLineDotWidth(int MHandle, int MaterialIndex);										// 指定のマテリアルのトゥーンレンダリングで使用する輪郭線のドット単位の太さを取得する
	//extern	int			MV1SetMaterialOutLineColorAll(int MHandle, COLOR_F Color);						// 全てのマテリアルのトゥーンレンダリングで使用する輪郭線の色を設定する
	//extern	int			MV1SetMaterialOutLineColor(int MHandle, int MaterialIndex, COLOR_F Color);						// 指定のマテリアルのトゥーンレンダリングで使用する輪郭線の色を設定する
	//extern	COLOR_F		MV1GetMaterialOutLineColor(int MHandle, int MaterialIndex);										// 指定のマテリアルのトゥーンレンダリングで使用する輪郭線の色を取得する
	//extern	int			MV1SetMaterialDrawBlendModeAll(int MHandle, int BlendMode);						// 全てのマテリアルの描画ブレンドモードを設定する( DX_BLENDMODE_ALPHA 等 )
	//extern	int			MV1SetMaterialDrawBlendMode(int MHandle, int MaterialIndex, int BlendMode);						// 指定のマテリアルの描画ブレンドモードを設定する( DX_BLENDMODE_ALPHA 等 )
	//extern	int			MV1GetMaterialDrawBlendMode(int MHandle, int MaterialIndex);										// 指定のマテリアルの描画ブレンドモードを取得する( DX_BLENDMODE_ALPHA 等 )
	//extern	int			MV1SetMaterialDrawBlendParamAll(int MHandle, int BlendParam);						// 全てのマテリアルの描画ブレンドパラメータを設定する
	//extern	int			MV1SetMaterialDrawBlendParam(int MHandle, int MaterialIndex, int BlendParam);						// 指定のマテリアルの描画ブレンドパラメータを設定する
	//extern	int			MV1GetMaterialDrawBlendParam(int MHandle, int MaterialIndex);										// 指定のマテリアルの描画ブレンドパラメータを設定する
	//extern	int			MV1SetMaterialDrawAlphaTestAll(int MHandle, int Enable, int Mode, int Param);		// 全てのマテリアルの描画時のアルファテストの設定を行う( Enable:αテストを行うかどうか( TRUE:行う  FALSE:行わない( デフォルト ) ) Mode:テストモード( DX_CMP_GREATER等 )  Param:描画アルファ値との比較に使用する値( 0～255 ) )
	//extern	int			MV1SetMaterialDrawAlphaTest(int MHandle, int MaterialIndex, int Enable, int Mode, int Param);		// 指定のマテリアルの描画時のアルファテストの設定を行う( Enable:αテストを行うかどうか( TRUE:行う  FALSE:行わない( デフォルト ) ) Mode:テストモード( DX_CMP_GREATER等 )  Param:描画アルファ値との比較に使用する値( 0～255 ) )
	//extern	int			MV1GetMaterialDrawAlphaTestEnable(int MHandle, int MaterialIndex);										// 指定のマテリアルの描画時のアルファテストを行うかどうかを取得する( 戻り値  TRUE:アルファテストを行う  FALSE:アルファテストを行わない )
	//extern	int			MV1GetMaterialDrawAlphaTestMode(int MHandle, int MaterialIndex);										// 指定のマテリアルの描画時のアルファテストのテストモードを取得する( 戻り値  テストモード( DX_CMP_GREATER等 ) )
	//extern	int			MV1GetMaterialDrawAlphaTestParam(int MHandle, int MaterialIndex);										// 指定のマテリアルの描画時のアルファテストの描画アルファ地との比較に使用する値( 0～255 )を取得する

	// テクスチャ関係
	//extern	int			MV1GetTextureNum(int MHandle);														// テクスチャの数を取得
	//extern	const TCHAR *MV1GetTextureName(int MHandle, int TexIndex);										// テクスチャの名前を取得
	//extern	int			MV1SetTextureColorFilePath(int MHandle, int TexIndex, const TCHAR *FilePath);	// カラーテクスチャのファイルパスを変更する
	//extern	int			MV1SetTextureColorFilePathWithStrLen(int MHandle, int TexIndex, const TCHAR *FilePath, size_t FilePathLength);	// カラーテクスチャのファイルパスを変更する
	//extern	const TCHAR *MV1GetTextureColorFilePath(int MHandle, int TexIndex);										// カラーテクスチャのファイルパスを取得
	//extern	int			MV1SetTextureAlphaFilePath(int MHandle, int TexIndex, const TCHAR *FilePath);	// アルファテクスチャのファイルパスを変更する
	//extern	int			MV1SetTextureAlphaFilePathWithStrLen(int MHandle, int TexIndex, const TCHAR *FilePath, size_t FilePathLength);	// アルファテクスチャのファイルパスを変更する
	//extern	const TCHAR *MV1GetTextureAlphaFilePath(int MHandle, int TexIndex);										// アルファテクスチャのファイルパスを取得
	//extern	int			MV1SetTextureGraphHandle(int MHandle, int TexIndex, int GrHandle, int SemiTransFlag);		// テクスチャで使用するグラフィックハンドルを変更する( GrHandle を -1 にすると解除 )
	//extern	int			MV1GetTextureGraphHandle(int MHandle, int TexIndex);										// テクスチャのグラフィックハンドルを取得する
	//extern	int			MV1SetTextureAddressMode(int MHandle, int TexIndex, int AddrUMode, int AddrVMode);			// テクスチャのアドレスモードを設定する( AddUMode の値は DX_TEXADDRESS_WRAP 等 )
	//extern	int			MV1GetTextureAddressModeU(int MHandle, int TexIndex);										// テクスチャのＵ値のアドレスモードを取得する( 戻り値:DX_TEXADDRESS_WRAP 等 )
	//extern	int			MV1GetTextureAddressModeV(int MHandle, int TexIndex);										// テクスチャのＶ値のアドレスモードを取得する( 戻り値:DX_TEXADDRESS_WRAP 等 )
	//extern	int			MV1GetTextureWidth(int MHandle, int TexIndex);										// テクスチャの幅を取得する
	//extern	int			MV1GetTextureHeight(int MHandle, int TexIndex);										// テクスチャの高さを取得する
	//extern	int			MV1GetTextureSemiTransState(int MHandle, int TexIndex);										// テクスチャに半透明要素があるかどうかを取得する( 戻り値  TRUE:ある  FALSE:ない )
	//extern	int			MV1SetTextureBumpImageFlag(int MHandle, int TexIndex, int Flag);								// テクスチャで使用している画像がバンプマップかどうかを設定する
	//extern	int			MV1GetTextureBumpImageFlag(int MHandle, int TexIndex);										// テクスチャがバンプマップかどうかを取得する( 戻り値  TRUE:バンプマップ  FALSE:違う )
	//extern	int			MV1SetTextureBumpImageNextPixelLength(int MHandle, int TexIndex, float Length);							// バンプマップ画像の場合の隣のピクセルとの距離を設定する
	//extern	float		MV1GetTextureBumpImageNextPixelLength(int MHandle, int TexIndex);										// バンプマップ画像の場合の隣のピクセルとの距離を取得する
	//extern	int			MV1SetTextureSampleFilterMode(int MHandle, int TexIndex, int FilterMode);						// テクスチャのフィルタリングモードを設定する
	//extern	int			MV1GetTextureSampleFilterMode(int MHandle, int TexIndex);										// テクスチャのフィルタリングモードを取得する( 戻り値  DX_DRAWMODE_BILINEAR等 )
	//extern	int			MV1LoadTexture(const TCHAR *FilePath);					// ３Ｄモデルに貼り付けるのに向いた画像の読み込み方式で画像を読み込む( 戻り値  -1:エラー  0以上:グラフィックハンドル )
	//extern	int			MV1LoadTextureWithStrLen(const TCHAR *FilePath, size_t FilePathLength);					// ３Ｄモデルに貼り付けるのに向いた画像の読み込み方式で画像を読み込む( 戻り値  -1:エラー  0以上:グラフィックハンドル )

	// フレーム関係
	//extern	int			MV1GetFrameNum(int MHandle);															// フレームの数を取得する
	//extern	int			MV1SearchFrame(int MHandle, const TCHAR *FrameName);			// フレームの名前からモデル中のフレームのフレームインデックスを取得する( 無かった場合は戻り値が-1 )
	//extern	int			MV1SearchFrameWithStrLen(int MHandle, const TCHAR *FrameName, size_t FrameNameLength);			// フレームの名前からモデル中のフレームのフレームインデックスを取得する( 無かった場合は戻り値が-1 )
	//extern	int			MV1SearchFrameChild(int MHandle, int FrameIndex = -1, const TCHAR *ChildName = NULL);	// フレームの名前から指定のフレームの子フレームのフレームインデックスを取得する( 名前指定版 )( FrameIndex を -1 にすると親を持たないフレームを ChildIndex で指定する )( 無かった場合は戻り値が-1 )
	//extern	int			MV1SearchFrameChildWithStrLen(int MHandle, int FrameIndex = -1, const TCHAR *ChildName = NULL, size_t ChildNameLength = 0);	// フレームの名前から指定のフレームの子フレームのフレームインデックスを取得する( 名前指定版 )( FrameIndex を -1 にすると親を持たないフレームを ChildIndex で指定する )( 無かった場合は戻り値が-1 )
	//extern	const TCHAR *MV1GetFrameName(int MHandle, int FrameIndex);											// 指定のフレームの名前を取得する( エラーの場合は戻り値が NULL )
	//extern	int			MV1GetFrameName2(int MHandle, int FrameIndex, TCHAR *StrBuffer);						// 指定のフレームの名前を取得する( 戻り値   -1:エラー  -1以外:文字列のサイズ )
	//extern	int			MV1GetFrameParent(int MHandle, int FrameIndex);											// 指定のフレームの親フレームのインデックスを得る( 親がいない場合は -2 が返る )
	//extern	int			MV1GetFrameChildNum(int MHandle, int FrameIndex = -1);									// 指定のフレームの子フレームの数を取得する( FrameIndex を -1 にすると親を持たないフレームの数が返ってくる )
	//extern	int			MV1GetFrameChild(int MHandle, int FrameIndex = -1, int ChildIndex = 0);				// 指定のフレームの子フレームのフレームインデックスを取得する( 番号指定版 )( FrameIndex を -1 にすると親を持たないフレームを ChildIndex で指定する )( エラーの場合は戻り値が-1 )
	//extern	VECTOR		MV1GetFramePosition(int MHandle, int FrameIndex);											// 指定のフレームの座標を取得する
	//extern	VECTOR_D	MV1GetFramePositionD(int MHandle, int FrameIndex);											// 指定のフレームの座標を取得する
	//extern	MATRIX		MV1GetFrameBaseLocalMatrix(int MHandle, int FrameIndex);											// 指定のフレームの初期状態での座標変換行列を取得する
	//extern	MATRIX_D	MV1GetFrameBaseLocalMatrixD(int MHandle, int FrameIndex);											// 指定のフレームの初期状態での座標変換行列を取得する
	DXLUA_PORT(t, MV1GetFrameLocalMatrix);
	//extern	MATRIX_D	MV1GetFrameLocalMatrixD(int MHandle, int FrameIndex);											// 指定のフレームの座標変換行列を取得する
	//extern	MATRIX		MV1GetFrameLocalWorldMatrix(int MHandle, int FrameIndex);											// 指定のフレームのローカル座標からワールド座標に変換する行列を得る
	//extern	MATRIX_D	MV1GetFrameLocalWorldMatrixD(int MHandle, int FrameIndex);											// 指定のフレームのローカル座標からワールド座標に変換する行列を得る
	DXLUA_PORT(t, MV1SetFrameUserLocalMatrix);
	//extern	int			MV1SetFrameUserLocalMatrixD(int MHandle, int FrameIndex, MATRIX_D Matrix);						// 指定のフレームの座標変換行列( ローカル行列 )を設定する
	DXLUA_PORT(t, MV1ResetFrameUserLocalMatrix);
	//extern	int			MV1SetFrameUserLocalWorldMatrix(int MHandle, int FrameIndex, MATRIX   Matrix);						// 指定のフレームの座標変換行列( ローカル座標からワールド座標に変換する行列 )を設定する
	//extern	int			MV1SetFrameUserLocalWorldMatrixD(int MHandle, int FrameIndex, MATRIX_D Matrix);						// 指定のフレームの座標変換行列( ローカル座標からワールド座標に変換する行列 )を設定する
	//extern	int			MV1ResetFrameUserLocalWorldMatrix(int MHandle, int FrameIndex);											// 指定のフレームの座標変換行列( ローカル座標からワールド座標に変換する行列 )をデフォルトに戻す
	//extern	VECTOR		MV1GetFrameMaxVertexLocalPosition(int MHandle, int FrameIndex);											// 指定のフレームが持つメッシュ頂点のローカル座標での最大値を得る
	//extern	VECTOR_D	MV1GetFrameMaxVertexLocalPositionD(int MHandle, int FrameIndex);											// 指定のフレームが持つメッシュ頂点のローカル座標での最大値を得る
	//extern	VECTOR		MV1GetFrameMinVertexLocalPosition(int MHandle, int FrameIndex);											// 指定のフレームが持つメッシュ頂点のローカル座標での最小値を得る
	//extern	VECTOR_D	MV1GetFrameMinVertexLocalPositionD(int MHandle, int FrameIndex);											// 指定のフレームが持つメッシュ頂点のローカル座標での最小値を得る
	//extern	VECTOR		MV1GetFrameAvgVertexLocalPosition(int MHandle, int FrameIndex);											// 指定のフレームが持つメッシュ頂点のローカル座標での平均値を得る
	//extern	VECTOR_D	MV1GetFrameAvgVertexLocalPositionD(int MHandle, int FrameIndex);											// 指定のフレームが持つメッシュ頂点のローカル座標での平均値を得る
	//extern	int			MV1GetFrameVertexNum(int MHandle, int FrameIndex);											// 指定のフレームに含まれる頂点の数を取得する
	//extern	int			MV1GetFrameTriangleNum(int MHandle, int FrameIndex);											// 指定のフレームに含まれるポリゴンの数を取得する
	//extern	int			MV1GetFrameMeshNum(int MHandle, int FrameIndex);											// 指定のフレームが持つメッシュの数を取得する
	//extern	int			MV1GetFrameMesh(int MHandle, int FrameIndex, int Index);								// 指定のフレームが持つメッシュのメッシュインデックスを取得する
	//extern	int			MV1SetFrameVisible(int MHandle, int FrameIndex, int VisibleFlag);						// 指定のフレームの表示、非表示状態を変更する( TRUE:表示  FALSE:非表示 )
	//extern	int			MV1GetFrameVisible(int MHandle, int FrameIndex);											// 指定のフレームの表示、非表示状態を取得する( TRUE:表示  FALSE:非表示 )
	//extern	int			MV1SetFrameDifColorScale(int MHandle, int FrameIndex, COLOR_F Scale);							// 指定のフレームのディフューズカラーのスケール値を設定する( デフォルト値は 1.0f )
	//extern	int			MV1SetFrameSpcColorScale(int MHandle, int FrameIndex, COLOR_F Scale);							// 指定のフレームのスペキュラカラーのスケール値を設定する( デフォルト値は 1.0f )
	//extern	int			MV1SetFrameEmiColorScale(int MHandle, int FrameIndex, COLOR_F Scale);							// 指定のフレームのエミッシブカラーのスケール値を設定する( デフォルト値は 1.0f )
	//extern	int			MV1SetFrameAmbColorScale(int MHandle, int FrameIndex, COLOR_F Scale);							// 指定のフレームのアンビエントカラーのスケール値を設定する( デフォルト値は 1.0f )
	//extern	COLOR_F		MV1GetFrameDifColorScale(int MHandle, int FrameIndex);											// 指定のフレームのディフューズカラーのスケール値を取得する( デフォルト値は 1.0f )
	//extern	COLOR_F		MV1GetFrameSpcColorScale(int MHandle, int FrameIndex);											// 指定のフレームのスペキュラカラーのスケール値を取得する( デフォルト値は 1.0f )
	//extern	COLOR_F		MV1GetFrameEmiColorScale(int MHandle, int FrameIndex);											// 指定のフレームのエミッシブカラーのスケール値を取得する( デフォルト値は 1.0f )
	//extern	COLOR_F		MV1GetFrameAmbColorScale(int MHandle, int FrameIndex);											// 指定のフレームのアンビエントカラーのスケール値を取得する( デフォルト値は 1.0f )
	//extern	int			MV1GetFrameSemiTransState(int MHandle, int FrameIndex);											// 指定のフレームに半透明要素があるかどうかを取得する( 戻り値 TRUE:ある  FALSE:ない )
	//extern	int			MV1SetFrameOpacityRate(int MHandle, int FrameIndex, float Rate);								// 指定のフレームの不透明度を設定する( 不透明 1.0f ～ 透明 0.0f )
	//extern	float		MV1GetFrameOpacityRate(int MHandle, int FrameIndex);											// 指定のフレームの不透明度を取得する( 不透明 1.0f ～ 透明 0.0f )
	//extern	int			MV1SetFrameBaseVisible(int MHandle, int FrameIndex, int VisibleFlag);						// 指定のフレームの初期表示状態を設定する( TRUE:表示  FALSE:非表示 )
	//extern	int			MV1GetFrameBaseVisible(int MHandle, int FrameIndex);											// 指定のフレームの初期表示状態を取得する( TRUE:表示  FALSE:非表示 )
	//extern	int			MV1SetFrameTextureAddressTransform(int MHandle, int FrameIndex, float TransU, float TransV, float ScaleU, float ScaleV, float RotCenterU, float RotCenterV, float Rotate);	// 指定のフレームのテクスチャ座標変換パラメータを設定する
	//extern	int			MV1SetFrameTextureAddressTransformMatrix(int MHandle, int FrameIndex, MATRIX Matrix);					// 指定のフレームのテクスチャ座標変換行列をセットする
	//extern	int			MV1ResetFrameTextureAddressTransform(int MHandle, int FrameIndex);										// 指定のフレームのテクスチャ座標変換パラメータをリセットする

	// メッシュ関係
	//extern	int			MV1GetMeshNum(int MHandle);															// モデルに含まれるメッシュの数を取得する
	//extern	int			MV1GetMeshMaterial(int MHandle, int MeshIndex);											// 指定メッシュが使用しているマテリアルのインデックスを取得する
	//extern	int			MV1GetMeshVertexNum(int MHandle, int MeshIndex);											// 指定メッシュに含まれる頂点の数を取得する
	//extern	int			MV1GetMeshTriangleNum(int MHandle, int MeshIndex);											// 指定メッシュに含まれる三角形ポリゴンの数を取得する
	//extern	int			MV1SetMeshVisible(int MHandle, int MeshIndex, int VisibleFlag);							// 指定メッシュの表示、非表示状態を変更する( TRUE:表示  FALSE:非表示 )
	//extern	int			MV1GetMeshVisible(int MHandle, int MeshIndex);											// 指定メッシュの表示、非表示状態を取得する( TRUE:表示  FALSE:非表示 )
	//extern	int			MV1SetMeshDifColorScale(int MHandle, int MeshIndex, COLOR_F Scale);							// 指定のメッシュのディフューズカラーのスケール値を設定する( デフォルト値は 1.0f )
	//extern	int			MV1SetMeshSpcColorScale(int MHandle, int MeshIndex, COLOR_F Scale);							// 指定のメッシュのスペキュラカラーのスケール値を設定する( デフォルト値は 1.0f )
	//extern	int			MV1SetMeshEmiColorScale(int MHandle, int MeshIndex, COLOR_F Scale);							// 指定のメッシュのエミッシブカラーのスケール値を設定する( デフォルト値は 1.0f )
	//extern	int			MV1SetMeshAmbColorScale(int MHandle, int MeshIndex, COLOR_F Scale);							// 指定のメッシュのアンビエントカラーのスケール値を設定する( デフォルト値は 1.0f )
	//extern	COLOR_F		MV1GetMeshDifColorScale(int MHandle, int MeshIndex);											// 指定のメッシュのディフューズカラーのスケール値を取得する( デフォルト値は 1.0f )
	//extern	COLOR_F		MV1GetMeshSpcColorScale(int MHandle, int MeshIndex);											// 指定のメッシュのスペキュラカラーのスケール値を取得する( デフォルト値は 1.0f )
	//extern	COLOR_F		MV1GetMeshEmiColorScale(int MHandle, int MeshIndex);											// 指定のメッシュのエミッシブカラーのスケール値を取得する( デフォルト値は 1.0f )
	//extern	COLOR_F		MV1GetMeshAmbColorScale(int MHandle, int MeshIndex);											// 指定のメッシュのアンビエントカラーのスケール値を取得する( デフォルト値は 1.0f )
	//extern	int			MV1SetMeshOpacityRate(int MHandle, int MeshIndex, float Rate);								// 指定のメッシュの不透明度を設定する( 不透明 1.0f ～ 透明 0.0f )
	//extern	float		MV1GetMeshOpacityRate(int MHandle, int MeshIndex);											// 指定のメッシュの不透明度を取得する( 不透明 1.0f ～ 透明 0.0f )
	//extern	int			MV1SetMeshDrawBlendMode(int MHandle, int MeshIndex, int BlendMode);							// 指定のメッシュの描画ブレンドモードを設定する( DX_BLENDMODE_ALPHA 等 )
	//extern	int			MV1SetMeshDrawBlendParam(int MHandle, int MeshIndex, int BlendParam);							// 指定のメッシュの描画ブレンドパラメータを設定する
	//extern	int			MV1GetMeshDrawBlendMode(int MHandle, int MeshIndex);											// 指定のメッシュの描画ブレンドモードを取得する( DX_BLENDMODE_ALPHA 等 )
	//extern	int			MV1GetMeshDrawBlendParam(int MHandle, int MeshIndex);											// 指定のメッシュの描画ブレンドパラメータを設定する
	//extern	int			MV1SetMeshBaseVisible(int MHandle, int MeshIndex, int VisibleFlag);							// 指定のメッシュの初期表示状態を設定する( TRUE:表示  FALSE:非表示 )
	//extern	int			MV1GetMeshBaseVisible(int MHandle, int MeshIndex);											// 指定のメッシュの初期表示状態を取得する( TRUE:表示  FALSE:非表示 )
	//extern	int			MV1SetMeshBackCulling(int MHandle, int MeshIndex, int CullingFlag);							// 指定のメッシュのバックカリングを行うかどうかを設定する( DX_CULLING_LEFT 等 )
	//extern	int			MV1GetMeshBackCulling(int MHandle, int MeshIndex);											// 指定のメッシュのバックカリングを行うかどうかを取得する( DX_CULLING_LEFT 等 )
	//extern	VECTOR		MV1GetMeshMaxPosition(int MHandle, int MeshIndex);											// 指定のメッシュに含まれるポリゴンの最大ローカル座標を取得する
	//extern	VECTOR		MV1GetMeshMinPosition(int MHandle, int MeshIndex);											// 指定のメッシュに含まれるポリゴンの最小ローカル座標を取得する
	//extern	int			MV1GetMeshTListNum(int MHandle, int MeshIndex);											// 指定のメッシュに含まれるトライアングルリストの数を取得する
	//extern	int			MV1GetMeshTList(int MHandle, int MeshIndex, int Index);								// 指定のメッシュに含まれるトライアングルリストのインデックスを取得する
	//extern	int			MV1GetMeshSemiTransState(int MHandle, int MeshIndex);											// 指定のメッシュに半透明要素があるかどうかを取得する( 戻り値 TRUE:ある  FALSE:ない )
	//extern	int			MV1SetMeshUseVertDifColor(int MHandle, int MeshIndex, int UseFlag);								// 指定のメッシュの頂点ディフューズカラーをマテリアルのディフューズカラーの代わりに使用するかどうかを設定する( TRUE:マテリアルカラーの代わりに使用する  FALSE:マテリアルカラーを使用する )
	//extern	int			MV1SetMeshUseVertSpcColor(int MHandle, int MeshIndex, int UseFlag);								// 指定のメッシュの頂点スペキュラカラーをマテリアルのスペキュラカラーの代わりに使用するかどうかを設定する( TRUE:マテリアルカラーの代わりに使用する  FALSE:マテリアルカラーを使用する )
	//extern	int			MV1GetMeshUseVertDifColor(int MHandle, int MeshIndex);											// 指定のメッシュの頂点ディフューズカラーをマテリアルのディフューズカラーの代わりに使用するかどうかの設定を取得する( 戻り値  TRUE:マテリアルカラーの代わりに使用する  FALSE:マテリアルカラーを使用する )
	//extern	int			MV1GetMeshUseVertSpcColor(int MHandle, int MeshIndex);											// 指定のメッシュの頂点スペキュラカラーをマテリアルのスペキュラカラーの代わりに使用するかどうかの設定を取得する( 戻り値  TRUE:マテリアルカラーの代わりに使用する  FALSE:マテリアルカラーを使用する )
	//extern	int			MV1GetMeshShapeFlag(int MHandle, int MeshIndex);											// 指定のメッシュがシェイプメッシュかどうかを取得する( 戻り値 TRUE:シェイプメッシュ  FALSE:通常メッシュ )

	// シェイプ関係
	//extern	int			MV1GetShapeNum(int MHandle);															// モデルに含まれるシェイプの数を取得する
	//extern	int			MV1SearchShape(int MHandle, const TCHAR *ShapeName);			// シェイプの名前からモデル中のシェイプのシェイプインデックスを取得する( 無かった場合は戻り値が-1 )
	//extern	int			MV1SearchShapeWithStrLen(int MHandle, const TCHAR *ShapeName, size_t ShapeNameLength);			// シェイプの名前からモデル中のシェイプのシェイプインデックスを取得する( 無かった場合は戻り値が-1 )
	//extern	const TCHAR *MV1GetShapeName(int MHandle, int ShapeIndex);											// 指定シェイプの名前を取得する
	//extern	int			MV1GetShapeTargetMeshNum(int MHandle, int ShapeIndex);											// 指定シェイプが対象としているメッシュの数を取得する
	//extern	int			MV1GetShapeTargetMesh(int MHandle, int ShapeIndex, int Index);								// 指定シェイプが対象としているメッシュのメッシュインデックスを取得する
	//extern	int			MV1SetShapeRate(int MHandle, int ShapeIndex, float Rate, int Type = DX_MV1_SHAPERATE_ADD);	// 指定シェイプの有効率を設定する( Rate  0.0f:0% ～ 1.0f:100% )
	//extern	float		MV1GetShapeRate(int MHandle, int ShapeIndex);											// 指定シェイプの有効率を取得する( 戻り値  0.0f:0% ～ 1.0f:100% )
	//extern	float		MV1GetShapeApplyRate(int MHandle, int ShapeIndex);											// 指定シェイプの有効率を取得する( 戻り値  0.0f:0% ～ 1.0f:100% )( MV1SetShapeRate で指定した値がそのまま戻り値となる MV1GetShapeRate と異なりアニメーションのシェイプ情報なども加味した値が戻り値となります )

	// トライアングルリスト関係
	//extern	int			MV1GetTriangleListNum(int MHandle);																		// モデルに含まれるトライアングルリストの数を取得する
	//extern	int			MV1GetTriangleListVertexType(int MHandle, int TListIndex);														// 指定のトライアングルリストの頂点データタイプを取得する( DX_MV1_VERTEX_TYPE_1FRAME 等 )
	//extern	int			MV1GetTriangleListPolygonNum(int MHandle, int TListIndex);														// 指定のトライアングルリストに含まれるポリゴンの数を取得する
	//extern	int			MV1GetTriangleListVertexNum(int MHandle, int TListIndex);														// 指定のトライアングルリストに含まれる頂点データの数を取得する
	//extern	int			MV1GetTriangleListLocalWorldMatrixNum(int MHandle, int TListIndex);														// 指定のトライアングルリストが使用する座標変換行列の数を取得する
	//extern	MATRIX		MV1GetTriangleListLocalWorldMatrix(int MHandle, int TListIndex, int LWMatrixIndex);									// 指定のトライアングルリストが使用する座標変換行列( ローカル→ワールド )を取得する
	//extern	int			MV1GetTriangleListPolygonVertexPosition(int MHandle, int TListIndex, int PolygonIndex, VECTOR *VertexPositionArray = NULL, float *MatrixWeightArray = NULL);	// 指定のトライアングルリストの指定のポリゴンが使用している頂点の座標を取得する( 戻り値  エラー：-1  0以上：ポリゴンが使用している頂点の数 )
	//extern	int			MV1GetTriangleListUseMaterial(int MHandle, int TListIndex);														// 指定のトライアングルリストが使用しているマテリアルのインデックスを取得する

	// コリジョン関係
	t["MV1SetupCollInfo"] = [](int MHandle, sol::variadic_args va) {
		int FrameIndex = va_get(va, 0, -1);
		int XDivNum = va_get(va, 1, 32);
		int YDivNum = va_get(va, 2, 8);
		int ZDivNum = va_get(va, 3, 32);
		int MeshIndex = va_get(va, 4, -1);
		return MV1SetupCollInfo(MHandle, FrameIndex, XDivNum, YDivNum, ZDivNum, MeshIndex);
	};
	t["MV1TerminateCollInfo"] = [](int MHandle, sol::variadic_args va) {
		int FrameIndex = va_get(va, 0, -1);
		int MeshIndex = va_get(va, 1, -1);
		return MV1TerminateCollInfo(MHandle, FrameIndex, MeshIndex);
	};
	t["MV1RefreshCollInfo"] = [](int MHandle, sol::variadic_args va) {
		int FrameIndex = va_get(va, 0, -1);
		int MeshIndex = va_get(va, 1, -1);
		return MV1RefreshCollInfo(MHandle, FrameIndex, MeshIndex);
	};
	t["MV1CollCheck_Line"] = [](int MHandle, int FrameIndex, VECTOR PosStart, VECTOR PosEnd, sol::variadic_args va) {
		int MeshIndex = va_get(va, 0, -1);
		return MV1CollCheck_Line(MHandle, FrameIndex, PosStart, PosEnd, MeshIndex);
	};
	t["MV1CollCheck_LineDim"] = [](int MHandle, int FrameIndex, VECTOR PosStart, VECTOR PosEnd, sol::variadic_args va) {
		int MeshIndex = va_get(va, 0, -1);
		return MV1CollCheck_LineDim(MHandle, FrameIndex, PosStart, PosEnd, MeshIndex);
	};
	t["MV1CollCheck_Sphere"] = [](int MHandle, int FrameIndex, VECTOR CenterPos, float r, sol::variadic_args va) {
		int MeshIndex = va_get(va, 0, -1);
		return MV1CollCheck_Sphere(MHandle, FrameIndex, CenterPos, r, MeshIndex);
	};
	t["MV1CollCheck_Capsule"] = [](int MHandle, int FrameIndex, VECTOR Pos1, VECTOR Pos2, float r, sol::variadic_args va) {
		int MeshIndex = va_get(va, 0, -1);
		return MV1CollCheck_Capsule(MHandle, FrameIndex, Pos1, Pos2, r, MeshIndex);
	};
	t["MV1CollCheck_Triangle"] = [](int MHandle, int FrameIndex, VECTOR Pos1, VECTOR Pos2, VECTOR Pos3, sol::variadic_args va) {
		int MeshIndex = va_get(va, 0, -1);
		return MV1CollCheck_Triangle(MHandle, FrameIndex, Pos1, Pos2, Pos3, MeshIndex);
	};
	DXLUA_PORT(t, MV1CollCheck_GetResultPoly);
	DXLUA_PORT(t, MV1CollResultPolyDimTerminate);

	// 参照用メッシュ関係
	t["MV1SetupReferenceMesh"] = [](int MHandle, int FrameIndex, sol::variadic_args va) {
		int IsTransform = va_get(va, 0, false);
		int IsPositionOnly = va_get(va, 1, false);
		int MeshIndex = va_get(va, 2, -1);
		return MV1SetupReferenceMesh(MHandle, FrameIndex, IsTransform, IsPositionOnly, MeshIndex);
	};
	t["MV1TerminateReferenceMesh"] = [](int MHandle, int FrameIndex, sol::variadic_args va) {
		int IsTransform = va_get(va, 0, false);
		int IsPositionOnly = va_get(va, 1, false);
		int MeshIndex = va_get(va, 2, -1);
		return MV1TerminateReferenceMesh(MHandle, FrameIndex, IsTransform, IsPositionOnly, MeshIndex);
	};
	t["MV1RefreshReferenceMesh"] = [](int MHandle, int FrameIndex, sol::variadic_args va) {
		int IsTransform = va_get(va, 0, false);
		int IsPositionOnly = va_get(va, 1, false);
		int MeshIndex = va_get(va, 2, -1);
		return MV1RefreshReferenceMesh(MHandle, FrameIndex, IsTransform, IsPositionOnly, MeshIndex);
	};
	t["MV1GetReferenceMesh"] = [](int MHandle, int FrameIndex, sol::variadic_args va) {
		int IsTransform = va_get(va, 0, false);
		int IsPositionOnly = va_get(va, 1, false);
		int MeshIndex = va_get(va, 2, -1);
		return MV1GetReferenceMesh(MHandle, FrameIndex, IsTransform, IsPositionOnly, MeshIndex);
	};

#endif // DX_NON_MODEL
}

} // namespace DxLua::detail
