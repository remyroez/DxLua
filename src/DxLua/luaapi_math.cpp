#include <tuple>

#include <DxLib.h>
#include <lua.hpp>
#include <sol/sol.hpp>

#include "luaapi.h"

namespace DxLua::detail {

void port_math(sol::state_view &lua, sol::table &t) {

	//DXLUA_PORT(t, MV1LoadModel);

	// 演算ライブラリ
	//extern	int			ConvertMatrixFtoD(MATRIX_D * Out, const MATRIX * In);															// float型要素の行列をdouble型要素の行列に変換する
	//extern	int			ConvertMatrixDtoF(MATRIX * Out, const MATRIX_D * In);															// double型要素の行列をfloat型要素の行列に変換する
	//extern	int			CreateIdentityMatrix(MATRIX * Out);																				// 単位行列を作成する
	//extern	int			CreateIdentityMatrixD(MATRIX_D * Out);																				// 単位行列を作成する
	//extern	int			CreateLookAtMatrix(MATRIX * Out, const VECTOR * Eye, const VECTOR * At, const VECTOR * Up);					// ビュー行列を作成する
	//extern	int			CreateLookAtMatrixD(MATRIX_D * Out, const VECTOR_D * Eye, const VECTOR_D * At, const VECTOR_D * Up);					// ビュー行列を作成する
	//extern	int			CreateLookAtMatrix2(MATRIX * Out, const VECTOR * Eye, double XZAngle, double Oira);								// ビュー行列を作成する(方向を回転値で指定)
	//extern	int			CreateLookAtMatrix2D(MATRIX_D * Out, const VECTOR_D * Eye, double XZAngle, double Oira);								// ビュー行列を作成する(方向を回転値で指定)
	//extern	int			CreateLookAtMatrixRH(MATRIX * Out, const VECTOR * Eye, const VECTOR * At, const VECTOR * Up);					// ビュー行列を作成する(右手座標系用)
	//extern	int			CreateLookAtMatrixRHD(MATRIX_D * Out, const VECTOR_D * Eye, const VECTOR_D * At, const VECTOR_D * Up);					// ビュー行列を作成する(右手座標系用)
	//extern	int			CreateMultiplyMatrix(MATRIX * Out, const MATRIX * In1, const MATRIX * In2);										// 行列の積を求める
	//extern	int			CreateMultiplyMatrixD(MATRIX_D * Out, const MATRIX_D * In1, const MATRIX_D * In2);										// 行列の積を求める
	//extern	int			CreatePerspectiveFovMatrix(MATRIX * Out, float  fov, float  zn, float  zf, float  aspect = -1.0f);						// 射影行列を作成する
	//extern	int			CreatePerspectiveFovMatrixD(MATRIX_D * Out, double fov, double zn, double zf, double aspect = -1.0f);						// 射影行列を作成する
	//extern	int			CreatePerspectiveFovMatrixRH(MATRIX * Out, float  fov, float  zn, float  zf, float  aspect = -1.0f);						// 射影行列を作成する(右手座標系用)
	//extern	int			CreatePerspectiveFovMatrixRHD(MATRIX_D * Out, double fov, double zn, double zf, double aspect = -1.0f);						// 射影行列を作成する(右手座標系用)
	//extern	int			CreateOrthoMatrix(MATRIX * Out, float  size, float  zn, float  zf, float  aspect = -1.0f);						// 正射影行列を作成する
	//extern	int			CreateOrthoMatrixD(MATRIX_D * Out, double size, double zn, double zf, double aspect = -1.0f);						// 正射影行列を作成する
	//extern	int			CreateOrthoMatrixRH(MATRIX * Out, float  size, float  zn, float  zf, float  aspect = -1.0f);						// 正射影行列を作成する(右手座標系用)
	//extern	int			CreateOrthoMatrixRHD(MATRIX_D * Out, double size, double zn, double zf, double aspect = -1.0f);						// 正射影行列を作成する(右手座標系用)
	//extern	int			CreateScalingMatrix(MATRIX * Out, float  sx, float  sy, float  sz);												// スケーリング行列を作成する
	//extern	int			CreateScalingMatrixD(MATRIX_D * Out, double sx, double sy, double sz);												// スケーリング行列を作成する
	//extern	int			CreateRotationXMatrix(MATRIX * Out, float  Angle);																	// Ｘ軸を中心とした回転行列を作成する
	//extern	int			CreateRotationXMatrixD(MATRIX_D * Out, double Angle);																	// Ｘ軸を中心とした回転行列を作成する
	//extern	int			CreateRotationYMatrix(MATRIX * Out, float  Angle);																	// Ｙ軸を中心とした回転行列を作成する
	//extern	int			CreateRotationYMatrixD(MATRIX_D * Out, double Angle);																	// Ｙ軸を中心とした回転行列を作成する
	//extern	int			CreateRotationZMatrix(MATRIX * Out, float  Angle);																	// Ｚ軸を中心とした回転行列を作成する
	//extern	int			CreateRotationZMatrixD(MATRIX_D * Out, double Angle);																	// Ｚ軸を中心とした回転行列を作成する
	//extern	int			CreateTranslationMatrix(MATRIX * Out, float  x, float  y, float  z);													// 平行移動行列を作成する
	//extern	int			CreateTranslationMatrixD(MATRIX_D * Out, double x, double y, double z);													// 平行移動行列を作成する
	//extern	int			CreateTransposeMatrix(MATRIX * Out, const MATRIX * In);															// 転置行列を作成する
	//extern	int			CreateTransposeMatrixD(MATRIX_D * Out, const MATRIX_D * In);															// 転置行列を作成する
	//extern	int			CreateInverseMatrix(MATRIX * Out, const MATRIX * In);															// 逆行列を作成する
	//extern	int			CreateInverseMatrixD(MATRIX_D * Out, const MATRIX_D * In);															// 逆行列を作成する
	//extern	int			CreateViewportMatrix(MATRIX * Out, float  CenterX, float  CenterY, float  Width, float  Height);					// ビューポート行列を作成する
	//extern	int			CreateViewportMatrixD(MATRIX_D * Out, double CenterX, double CenterY, double Width, double Height);					// ビューポート行列を作成する
	//extern	int			CreateRotationXYZMatrix(MATRIX * Out, float  XRot, float  YRot, float  ZRot);											// Ｘ軸回転→Ｙ軸回転→Ｚ軸回転を合成した行列を作成する
	//extern	int			CreateRotationXYZMatrixD(MATRIX_D * Out, double XRot, double YRot, double ZRot);											// Ｘ軸回転→Ｙ軸回転→Ｚ軸回転を合成した行列を作成する
	//extern	int			CreateRotationXZYMatrix(MATRIX * Out, float  XRot, float  YRot, float  ZRot);											// Ｘ軸回転→Ｚ軸回転→Ｙ軸回転を合成した行列を作成する
	//extern	int			CreateRotationXZYMatrixD(MATRIX_D * Out, double XRot, double YRot, double ZRot);											// Ｘ軸回転→Ｚ軸回転→Ｙ軸回転を合成した行列を作成する
	//extern	int			CreateRotationYXZMatrix(MATRIX * Out, float  XRot, float  YRot, float  ZRot);											// Ｙ軸回転→Ｘ軸回転→Ｚ軸回転を合成した行列を作成する
	//extern	int			CreateRotationYXZMatrixD(MATRIX_D * Out, double XRot, double YRot, double ZRot);											// Ｙ軸回転→Ｘ軸回転→Ｚ軸回転を合成した行列を作成する
	//extern	int			CreateRotationYZXMatrix(MATRIX * Out, float  XRot, float  YRot, float  ZRot);											// Ｙ軸回転→Ｚ軸回転→Ｘ軸回転を合成した行列を作成する
	//extern	int			CreateRotationYZXMatrixD(MATRIX_D * Out, double XRot, double YRot, double ZRot);											// Ｙ軸回転→Ｚ軸回転→Ｘ軸回転を合成した行列を作成する
	//extern	int			CreateRotationZXYMatrix(MATRIX * Out, float  XRot, float  YRot, float  ZRot);											// Ｚ軸回転→Ｘ軸回転→Ｙ軸回転を合成した行列を作成する
	//extern	int			CreateRotationZXYMatrixD(MATRIX_D * Out, double XRot, double YRot, double ZRot);											// Ｚ軸回転→Ｘ軸回転→Ｙ軸回転を合成した行列を作成する
	//extern	int			CreateRotationZYXMatrix(MATRIX * Out, float  XRot, float  YRot, float  ZRot);											// Ｚ軸回転→Ｙ軸回転→Ｘ軸回転を合成した行列を作成する
	//extern	int			CreateRotationZYXMatrixD(MATRIX_D * Out, double XRot, double YRot, double ZRot);											// Ｚ軸回転→Ｙ軸回転→Ｘ軸回転を合成した行列を作成する
	//extern	int			GetMatrixXYZRotation(const MATRIX * In, float *OutXRot, float *OutYRot, float *OutZRot);						// 行列からＸＹＺ軸回転の値を取得する( 戻り値　-1:ジンバルロック発生  0:成功 )
	//extern	int			GetMatrixXYZRotationD(const MATRIX_D * In, double *OutXRot, double *OutYRot, double *OutZRot);						// 行列からＸＹＺ軸回転の値を取得する( 戻り値　-1:ジンバルロック発生  0:成功 )
	//extern	int			GetMatrixXZYRotation(const MATRIX * In, float *OutXRot, float *OutYRot, float *OutZRot);						// 行列からＸＺＹ軸回転の値を取得する( 戻り値　-1:ジンバルロック発生  0:成功 )
	//extern	int			GetMatrixXZYRotationD(const MATRIX_D * In, double *OutXRot, double *OutYRot, double *OutZRot);						// 行列からＸＺＹ軸回転の値を取得する( 戻り値　-1:ジンバルロック発生  0:成功 )
	//extern	int			GetMatrixYXZRotation(const MATRIX * In, float *OutXRot, float *OutYRot, float *OutZRot);						// 行列からＹＸＺ軸回転の値を取得する( 戻り値　-1:ジンバルロック発生  0:成功 )
	//extern	int			GetMatrixYXZRotationD(const MATRIX_D * In, double *OutXRot, double *OutYRot, double *OutZRot);						// 行列からＹＸＺ軸回転の値を取得する( 戻り値　-1:ジンバルロック発生  0:成功 )
	//extern	int			GetMatrixYZXRotation(const MATRIX * In, float *OutXRot, float *OutYRot, float *OutZRot);						// 行列からＹＺＸ軸回転の値を取得する( 戻り値　-1:ジンバルロック発生  0:成功 )
	//extern	int			GetMatrixYZXRotationD(const MATRIX_D * In, double *OutXRot, double *OutYRot, double *OutZRot);						// 行列からＹＺＸ軸回転の値を取得する( 戻り値　-1:ジンバルロック発生  0:成功 )
	//extern	int			GetMatrixZXYRotation(const MATRIX * In, float *OutXRot, float *OutYRot, float *OutZRot);						// 行列からＺＸＹ軸回転の値を取得する( 戻り値　-1:ジンバルロック発生  0:成功 )
	//extern	int			GetMatrixZXYRotationD(const MATRIX_D * In, double *OutXRot, double *OutYRot, double *OutZRot);						// 行列からＺＸＹ軸回転の値を取得する( 戻り値　-1:ジンバルロック発生  0:成功 )
	//extern	int			GetMatrixZYXRotation(const MATRIX * In, float *OutXRot, float *OutYRot, float *OutZRot);						// 行列からＺＹＸ軸回転の値を取得する( 戻り値　-1:ジンバルロック発生  0:成功 )
	//extern	int			GetMatrixZYXRotationD(const MATRIX_D * In, double *OutXRot, double *OutYRot, double *OutZRot);						// 行列からＺＹＸ軸回転の値を取得する( 戻り値　-1:ジンバルロック発生  0:成功 )
	//extern	int			VectorConvertFtoD(VECTOR_D * Out, const VECTOR * In);															// float型のベクトルをdouble型のベクトルに変換する
	//extern	int			VectorConvertDtoF(VECTOR * Out, const VECTOR_D * In);															// double型のベクトルをfloat型のベクトルに変換する
	//extern	int			VectorNormalize(VECTOR * Out, const VECTOR * In);															// ベクトルを正規化する
	//extern	int			VectorNormalizeD(VECTOR_D * Out, const VECTOR_D * In);															// ベクトルを正規化する
	//extern	int			VectorScale(VECTOR * Out, const VECTOR * In, float  Scale);												// ベクトルをスカラー倍する
	//extern	int			VectorScaleD(VECTOR_D * Out, const VECTOR_D * In, double Scale);												// ベクトルをスカラー倍する
	//extern	int			VectorMultiply(VECTOR * Out, const VECTOR * In1, const VECTOR * In2);										// ベクトルの掛け算をする
	//extern	int			VectorMultiplyD(VECTOR_D * Out, const VECTOR_D * In1, const VECTOR_D * In2);										// ベクトルの掛け算をする
	//extern	int			VectorSub(VECTOR * Out, const VECTOR * In1, const VECTOR * In2);										// Out = In1 - In2 のベクトル計算をする 
	//extern	int			VectorSubD(VECTOR_D * Out, const VECTOR_D * In1, const VECTOR_D * In2);										// Out = In1 - In2 のベクトル計算をする 
	//extern	int			VectorAdd(VECTOR * Out, const VECTOR * In1, const VECTOR * In2);										// Out = In1 + In2 のベクトル計算をする 
	//extern	int			VectorAddD(VECTOR_D * Out, const VECTOR_D * In1, const VECTOR_D * In2);										// Out = In1 + In2 のベクトル計算をする 
	//extern	int			VectorOuterProduct(VECTOR * Out, const VECTOR * In1, const VECTOR * In2);										// In1とIn2の外積を計算する
	//extern	int			VectorOuterProductD(VECTOR_D * Out, const VECTOR_D * In1, const VECTOR_D * In2);										// In1とIn2の外積を計算する
	//extern	float 		VectorInnerProduct(const VECTOR * In1, const VECTOR * In2);														// In1とIn2の内積を計算する
	//extern	double		VectorInnerProductD(const VECTOR_D * In1, const VECTOR_D * In2);														// In1とIn2の内積を計算する
	//extern	int			VectorRotationX(VECTOR * Out, const VECTOR * In, double Angle);												// ベクトルのＸ軸を軸にした回転を行う
	//extern	int			VectorRotationXD(VECTOR_D * Out, const VECTOR_D * In, double Angle);												// ベクトルのＸ軸を軸にした回転を行う
	//extern	int			VectorRotationY(VECTOR * Out, const VECTOR * In, double Angle);												// ベクトルのＹ軸を軸にした回転を行う
	//extern	int			VectorRotationYD(VECTOR_D * Out, const VECTOR_D * In, double Angle);												// ベクトルのＹ軸を軸にした回転を行う
	//extern	int			VectorRotationZ(VECTOR * Out, const VECTOR * In, double Angle);												// ベクトルのＺ軸を軸にした回転を行う
	//extern	int			VectorRotationZD(VECTOR_D * Out, const VECTOR_D * In, double Angle);												// ベクトルのＺ軸を軸にした回転を行う
	//extern	int			VectorTransform(VECTOR * Out, const VECTOR * InVec, const MATRIX * InMatrix);								// ベクトル行列と4x4正方行列を乗算する( w は 1 と仮定 )
	//extern	int			VectorTransformD(VECTOR_D * Out, const VECTOR_D * InVec, const MATRIX_D * InMatrix);								// ベクトル行列と4x4正方行列を乗算する( w は 1 と仮定 )
	//extern	int			VectorTransformSR(VECTOR * Out, const VECTOR * InVec, const MATRIX * InMatrix);								// ベクトル行列と4x4正方行列の回転部分のみを乗算する
	//extern	int			VectorTransformSRD(VECTOR_D * Out, const VECTOR_D * InVec, const MATRIX_D * InMatrix);								// ベクトル行列と4x4正方行列の回転部分のみを乗算する
	//extern	int			VectorTransform4(VECTOR * Out, float *V4Out, const VECTOR * InVec, const float *V4In, const MATRIX * InMatrix); // ベクトル行列と4x4正方行列を乗算する( w の要素を渡す )
	//extern	int			VectorTransform4D(VECTOR_D * Out, double *V4Out, const VECTOR_D * InVec, const double *V4In, const MATRIX_D * InMatrix); // ベクトル行列と4x4正方行列を乗算する( w の要素を渡す )

	//extern	int			Segment_Segment_Analyse(const VECTOR * SegmentAPos1, const VECTOR * SegmentAPos2, const VECTOR * SegmentBPos1, const VECTOR * SegmentBPos2, SEGMENT_SEGMENT_RESULT * Result);								// 二つの線分の最接近点情報を解析する
	//extern	int			Segment_Segment_AnalyseD(const VECTOR_D * SegmentAPos1, const VECTOR_D * SegmentAPos2, const VECTOR_D * SegmentBPos1, const VECTOR_D * SegmentBPos2, SEGMENT_SEGMENT_RESULT_D * Result);								// 二つの線分の最接近点情報を解析する
	//extern	int			Segment_Point_Analyse(const VECTOR * SegmentPos1, const VECTOR * SegmentPos2, const VECTOR * PointPos, SEGMENT_POINT_RESULT * Result);																	// 線分と点の最接近点情報を解析する
	//extern	int			Segment_Point_AnalyseD(const VECTOR_D * SegmentPos1, const VECTOR_D * SegmentPos2, const VECTOR_D * PointPos, SEGMENT_POINT_RESULT_D * Result);																	// 線分と点の最接近点情報を解析する
	//extern	int			Segment_Triangle_Analyse(const VECTOR * SegmentPos1, const VECTOR * SegmentPos2, const VECTOR * TrianglePos1, const VECTOR * TrianglePos2, const VECTOR * TrianglePos3, SEGMENT_TRIANGLE_RESULT * Result);	// 線分と三角形の最接近点情報を解析する
	//extern	int			Segment_Triangle_AnalyseD(const VECTOR_D * SegmentPos1, const VECTOR_D * SegmentPos2, const VECTOR_D * TrianglePos1, const VECTOR_D * TrianglePos2, const VECTOR_D * TrianglePos3, SEGMENT_TRIANGLE_RESULT_D * Result);	// 線分と三角形の最接近点情報を解析する
	//extern	int			Triangle_Point_Analyse(const VECTOR * TrianglePos1, const VECTOR * TrianglePos2, const VECTOR * TrianglePos3, const VECTOR * PointPos, TRIANGLE_POINT_RESULT * Result);									// 三角形と点の最接近点情報を解析する
	//extern	int			Triangle_Point_AnalyseD(const VECTOR_D * TrianglePos1, const VECTOR_D * TrianglePos2, const VECTOR_D * TrianglePos3, const VECTOR_D * PointPos, TRIANGLE_POINT_RESULT_D * Result);									// 三角形と点の最接近点情報を解析する
	//extern	int			Plane_Point_Analyse(const VECTOR * PlanePos, const VECTOR * PlaneNormal, const VECTOR * PointPos, PLANE_POINT_RESULT * Result);																			// 平面と点の最近点情報を解析する
	//extern	int			Plane_Point_AnalyseD(const VECTOR_D * PlanePos, const VECTOR_D * PlaneNormal, const VECTOR_D * PointPos, PLANE_POINT_RESULT_D * Result);																			// 平面と点の最近点情報を解析する

	//extern	void		TriangleBarycenter(VECTOR   TrianglePos1, VECTOR   TrianglePos2, VECTOR   TrianglePos3, VECTOR   Position, float *TrianglePos1Weight, float *TrianglePos2Weight, float *TrianglePos3Weight);	// 指定の座標から三角形の重心を求める
	//extern	void		TriangleBarycenterD(VECTOR_D TrianglePos1, VECTOR_D TrianglePos2, VECTOR_D TrianglePos3, VECTOR_D Position, double *TrianglePos1Weight, double *TrianglePos2Weight, double *TrianglePos3Weight);	// 指定の座標から三角形の重心を求める

	//extern	float 		Segment_Segment_MinLength(VECTOR   SegmentAPos1, VECTOR   SegmentAPos2, VECTOR   SegmentBPos1, VECTOR   SegmentBPos2);														// 二つの線分の最近点間の距離を得る
	//extern	double		Segment_Segment_MinLengthD(VECTOR_D SegmentAPos1, VECTOR_D SegmentAPos2, VECTOR_D SegmentBPos1, VECTOR_D SegmentBPos2);														// 二つの線分の最近点間の距離を得る
	//extern	float 		Segment_Segment_MinLength_Square(VECTOR   SegmentAPos1, VECTOR   SegmentAPos2, VECTOR   SegmentBPos1, VECTOR   SegmentBPos2);														// 二つの線分の最近点間の距離の二乗を得る
	//extern	double		Segment_Segment_MinLength_SquareD(VECTOR_D SegmentAPos1, VECTOR_D SegmentAPos2, VECTOR_D SegmentBPos1, VECTOR_D SegmentBPos2);														// 二つの線分の最近点間の距離の二乗を得る
	//extern	float 		Segment_Triangle_MinLength(VECTOR   SegmentPos1, VECTOR   SegmentPos2, VECTOR   TrianglePos1, VECTOR   TrianglePos2, VECTOR   TrianglePos3);								// 線分と三角形の最近点間の距離を得る 
	//extern	double		Segment_Triangle_MinLengthD(VECTOR_D SegmentPos1, VECTOR_D SegmentPos2, VECTOR_D TrianglePos1, VECTOR_D TrianglePos2, VECTOR_D TrianglePos3);								// 線分と三角形の最近点間の距離を得る 
	//extern	float 		Segment_Triangle_MinLength_Square(VECTOR   SegmentPos1, VECTOR   SegmentPos2, VECTOR   TrianglePos1, VECTOR   TrianglePos2, VECTOR   TrianglePos3);								// 線分と三角形の最近点間の距離の二乗を得る 
	//extern	double		Segment_Triangle_MinLength_SquareD(VECTOR_D SegmentPos1, VECTOR_D SegmentPos2, VECTOR_D TrianglePos1, VECTOR_D TrianglePos2, VECTOR_D TrianglePos3);								// 線分と三角形の最近点間の距離の二乗を得る 
	//extern	float 		Segment_Point_MinLength(VECTOR   SegmentPos1, VECTOR   SegmentPos2, VECTOR   PointPos);																					// 線分と点の一番近い距離を得る
	//extern	double		Segment_Point_MinLengthD(VECTOR_D SegmentPos1, VECTOR_D SegmentPos2, VECTOR_D PointPos);																					// 線分と点の一番近い距離を得る
	//extern	float 		Segment_Point_MinLength_Square(VECTOR   SegmentPos1, VECTOR   SegmentPos2, VECTOR   PointPos);																					// 線分と点の一番近い距離の二乗を得る
	//extern	double		Segment_Point_MinLength_SquareD(VECTOR_D SegmentPos1, VECTOR_D SegmentPos2, VECTOR_D PointPos);																					// 線分と点の一番近い距離の二乗を得る
	//extern	float 		Triangle_Point_MinLength(VECTOR   TrianglePos1, VECTOR   TrianglePos2, VECTOR   TrianglePos3, VECTOR   PointPos);															// 三角形と点の一番近い距離を得る
	//extern	double		Triangle_Point_MinLengthD(VECTOR_D TrianglePos1, VECTOR_D TrianglePos2, VECTOR_D TrianglePos3, VECTOR_D PointPos);															// 三角形と点の一番近い距離を得る
	//extern	float 		Triangle_Point_MinLength_Square(VECTOR   TrianglePos1, VECTOR   TrianglePos2, VECTOR   TrianglePos3, VECTOR   PointPos);															// 三角形と点の一番近い距離の二乗を得る
	//extern	double		Triangle_Point_MinLength_SquareD(VECTOR_D TrianglePos1, VECTOR_D TrianglePos2, VECTOR_D TrianglePos3, VECTOR_D PointPos);															// 三角形と点の一番近い距離の二乗を得る
	//extern	float 		Triangle_Triangle_MinLength(VECTOR   Triangle1Pos1, VECTOR   Triangle1Pos2, VECTOR   Triangle1Pos3, VECTOR   Triangle2Pos1, VECTOR   Triangle2Pos2, VECTOR   Triangle2Pos3);	// 二つの三角形の最近点間の距離を得る
	//extern	double		Triangle_Triangle_MinLengthD(VECTOR_D Triangle1Pos1, VECTOR_D Triangle1Pos2, VECTOR_D Triangle1Pos3, VECTOR_D Triangle2Pos1, VECTOR_D Triangle2Pos2, VECTOR_D Triangle2Pos3);	// 二つの三角形の最近点間の距離を得る
	//extern	float 		Triangle_Triangle_MinLength_Square(VECTOR   Triangle1Pos1, VECTOR   Triangle1Pos2, VECTOR   Triangle1Pos3, VECTOR   Triangle2Pos1, VECTOR   Triangle2Pos2, VECTOR   Triangle2Pos3);	// 二つの三角形の最近点間の距離の二乗を得る
	//extern	double		Triangle_Triangle_MinLength_SquareD(VECTOR_D Triangle1Pos1, VECTOR_D Triangle1Pos2, VECTOR_D Triangle1Pos3, VECTOR_D Triangle2Pos1, VECTOR_D Triangle2Pos2, VECTOR_D Triangle2Pos3);	// 二つの三角形の最近点間の距離の二乗を得る
	//extern	VECTOR  	Plane_Point_MinLength_Position(VECTOR   PlanePos, VECTOR   PlaneNormal, VECTOR   PointPos);																						// 点に一番近い平面上の座標を得る
	//extern	VECTOR_D	Plane_Point_MinLength_PositionD(VECTOR_D PlanePos, VECTOR_D PlaneNormal, VECTOR_D PointPos);																						// 点に一番近い平面上の座標を得る
	//extern	float 		Plane_Point_MinLength(VECTOR   PlanePos, VECTOR   PlaneNormal, VECTOR   PointPos);																						// 平面と点の一番近い距離を得る
	//extern	double		Plane_Point_MinLengthD(VECTOR_D PlanePos, VECTOR_D PlaneNormal, VECTOR_D PointPos);																						// 平面と点の一番近い距離を得る

	//extern	HITRESULT_LINE   HitCheck_Line_Triangle(VECTOR   LinePos1, VECTOR   LinePos2, VECTOR   TrianglePos1, VECTOR   TrianglePos2, VECTOR   TrianglePos3);										// 三角形と線の当たり判定
	DXLUA_PORT(t, HitCheck_Line_Triangle);
	//extern	HITRESULT_LINE_D HitCheck_Line_TriangleD(VECTOR_D LinePos1, VECTOR_D LinePos2, VECTOR_D TrianglePos1, VECTOR_D TrianglePos2, VECTOR_D TrianglePos3);										// 三角形と線の当たり判定
	//extern	int			HitCheck_Triangle_Triangle(VECTOR   Triangle1Pos1, VECTOR   Triangle1Pos2, VECTOR   Triangle1Pos3, VECTOR   Triangle2Pos1, VECTOR   Triangle2Pos2, VECTOR   Triangle2Pos3);	// 三角形と三角形の当たり判定( TRUE:当たっている  FALSE:当たっていない )
	//extern	int			HitCheck_Triangle_TriangleD(VECTOR_D Triangle1Pos1, VECTOR_D Triangle1Pos2, VECTOR_D Triangle1Pos3, VECTOR_D Triangle2Pos1, VECTOR_D Triangle2Pos2, VECTOR_D Triangle2Pos3);	// 三角形と三角形の当たり判定( TRUE:当たっている  FALSE:当たっていない )
	//extern	int			HitCheck_Triangle_Triangle_2D(VECTOR   Triangle1Pos1, VECTOR   Triangle1Pos2, VECTOR   Triangle1Pos3, VECTOR   Triangle2Pos1, VECTOR   Triangle2Pos2, VECTOR   Triangle2Pos3);	// 三角形と三角形の当たり判定( ２Ｄ版 )( TRUE:当たっている  FALSE:当たっていない )
	//extern	int			HitCheck_Triangle_TriangleD_2D(VECTOR_D Triangle1Pos1, VECTOR_D Triangle1Pos2, VECTOR_D Triangle1Pos3, VECTOR_D Triangle2Pos1, VECTOR_D Triangle2Pos2, VECTOR_D Triangle2Pos3);	// 三角形と三角形の当たり判定( ２Ｄ版 )( TRUE:当たっている  FALSE:当たっていない )
	//extern	HITRESULT_LINE   HitCheck_Line_Cube(VECTOR   LinePos1, VECTOR   LinePos2, VECTOR   CubePos1, VECTOR   CubePos2);																		// 線と箱の当たり判定
	//extern	HITRESULT_LINE_D HitCheck_Line_CubeD(VECTOR_D LinePos1, VECTOR_D LinePos2, VECTOR_D CubePos1, VECTOR_D CubePos2);																		// 線と箱の当たり判定
	//extern	int			HitCheck_Point_Cone(VECTOR   PointPos, VECTOR   ConeTopPos, VECTOR   ConeBottomPos, float  ConeR);																	// 点と円錐の当たり判定( TRUE:当たっている  FALSE:当たっていない )
	//extern	int			HitCheck_Point_ConeD(VECTOR_D PointPos, VECTOR_D ConeTopPos, VECTOR_D ConeBottomPos, double ConeR);																	// 点と円錐の当たり判定( TRUE:当たっている  FALSE:当たっていない )
	//extern	int			HitCheck_Line_Sphere(VECTOR   LinePos1, VECTOR   LinePos2, VECTOR   SphereCenterPos, float  SphereR);																	// 線と球の当たり判定( TRUE:当たっている  FALSE:当たっていない )
	//extern	int			HitCheck_Line_SphereD(VECTOR_D LinePos1, VECTOR_D LinePos2, VECTOR_D SphereCenterPos, double SphereR);																	// 線と球の当たり判定( TRUE:当たっている  FALSE:当たっていない )
	//extern	int			HitCheck_Sphere_Sphere(VECTOR   Sphere1CenterPos, float  Sphere1R, VECTOR   Sphere2CenterPos, float  Sphere2R);															// 球と球の当たり判定( TRUE:当たっている  FALSE:当たっていない )
	//extern	int			HitCheck_Sphere_SphereD(VECTOR_D Sphere1CenterPos, double Sphere1R, VECTOR_D Sphere2CenterPos, double Sphere2R);															// 球と球の当たり判定( TRUE:当たっている  FALSE:当たっていない )
	//extern	int			HitCheck_Sphere_Capsule(VECTOR   SphereCenterPos, float  SphereR, VECTOR   CapPos1, VECTOR   CapPos2, float  CapR);														// 球とカプセルの当たり判定( TRUE:当たっている  FALSE:当たっていない )
	//extern	int			HitCheck_Sphere_CapsuleD(VECTOR_D SphereCenterPos, double SphereR, VECTOR_D CapPos1, VECTOR_D CapPos2, double CapR);														// 球とカプセルの当たり判定( TRUE:当たっている  FALSE:当たっていない )
	//extern	int			HitCheck_Sphere_Triangle(VECTOR   SphereCenterPos, float  SphereR, VECTOR   TrianglePos1, VECTOR   TrianglePos2, VECTOR   TrianglePos3);									// 球と三角形の当たり判定( TRUE:当たっている  FALSE:当たっていない )
	//extern	int			HitCheck_Sphere_TriangleD(VECTOR_D SphereCenterPos, double SphereR, VECTOR_D TrianglePos1, VECTOR_D TrianglePos2, VECTOR_D TrianglePos3);									// 球と三角形の当たり判定( TRUE:当たっている  FALSE:当たっていない )
	//extern	int			HitCheck_Capsule_Capsule(VECTOR   Cap1Pos1, VECTOR   Cap1Pos2, float  Cap1R, VECTOR   Cap2Pos1, VECTOR   Cap2Pos2, float  Cap2R);											// カプセル同士の当たり判定( TRUE:当たっている  FALSE:当たっていない )
	//extern	int			HitCheck_Capsule_CapsuleD(VECTOR_D Cap1Pos1, VECTOR_D Cap1Pos2, double Cap1R, VECTOR_D Cap2Pos1, VECTOR_D Cap2Pos2, double Cap2R);											// カプセル同士の当たり判定( TRUE:当たっている  FALSE:当たっていない )
	//DXLUA_PORT(t, HitCheck_Capsule_Triangle);
	t["HitCheck_Capsule_Triangle"] = [](sol::variadic_args va) {
		auto CapPos1 = va_get(va, 0, VECTOR{ 0, 0, 0 });
		auto CapPos2 = va_get(va, 1, VECTOR{ 0, 0, 0 });
		auto CapR = va_get(va, 2, 0.f);
		auto TrianglePos1 = va_get(va, 3, VECTOR{ 0, 0, 0 });
		auto TrianglePos2 = va_get(va, 4, VECTOR{ 0, 0, 0 });
		auto TrianglePos3 = va_get(va, 5, VECTOR{ 0, 0, 0 });
		return HitCheck_Capsule_Triangle(CapPos1, CapPos2, CapR, TrianglePos1, TrianglePos2, TrianglePos3);
	};
	//extern	int			HitCheck_Capsule_TriangleD(VECTOR_D CapPos1, VECTOR_D CapPos2, double CapR, VECTOR_D TrianglePos1, VECTOR_D TrianglePos2, VECTOR_D TrianglePos3);							// カプセルと三角形の当たり判定( TRUE:当たっている  FALSE:当たっていない )

	// その他
	//extern	int			RectClipping(RECT * Rect, const RECT * ClippuRect);									// 矩形のクリッピング
	//extern	int			RectAdjust(RECT * Rect);															// 矩形の left が right より値が大きい場合などの誤りを補正する
	//extern	int			GetRectSize(const RECT * Rect, int *Width, int *Height);								// 矩形の幅と高さを求める

	// 表記簡略版
	//extern	MATRIX		MGetIdent(void);															// 単位行列を取得する
	//extern	MATRIX_D	MGetIdentD(void);															// 単位行列を取得する
	//extern	MATRIX		MMult(MATRIX   In1, MATRIX   In2);										// 行列の乗算を行う
	//extern	MATRIX_D	MMultD(MATRIX_D In1, MATRIX_D In2);										// 行列の乗算を行う
	//extern	MATRIX		MScale(MATRIX   InM, float  Scale);										// 行列のスケーリングを行う
	//extern	MATRIX_D	MScaleD(MATRIX_D InM, double Scale);										// 行列のスケーリングを行う
	//extern	MATRIX		MAdd(MATRIX   In1, MATRIX   In2);										// 行列の足し算を行う
	//extern	MATRIX_D	MAddD(MATRIX_D In1, MATRIX_D In2);										// 行列の足し算を行う
	//extern	MATRIX		MGetScale(VECTOR   Scale);													// 拡大行列を取得する
	//extern	MATRIX_D	MGetScaleD(VECTOR_D Scale);													// 拡大行列を取得する
	//extern	MATRIX		MGetRotX(float  XAxisRotate);												// Ｘ軸回転行列を取得する
	//extern	MATRIX_D	MGetRotXD(double XAxisRotate);												// Ｘ軸回転行列を取得する
	DXLUA_PORT(t, MGetRotY);
	//extern	MATRIX_D	MGetRotYD(double YAxisRotate);												// Ｙ軸回転行列を取得する
	DXLUA_PORT(t, MGetRotZ);
	//extern	MATRIX_D	MGetRotZD(double ZAxisRotate);												// Ｚ軸回転行列を取得する
	//extern	MATRIX		MGetRotAxis(VECTOR   RotateAxis, float  Rotate);								// 指定軸で指定角度回転する行列を取得する
	//extern	MATRIX_D	MGetRotAxisD(VECTOR_D RotateAxis, double Rotate);								// 指定軸で指定角度回転する行列を取得する
	//extern	MATRIX		MGetRotVec2(VECTOR   In1, VECTOR   In2);										// In1 の向きから In2 の向きへ変換する回転行列を取得する( In2 と In1 が真逆のベクトルの場合は回転軸は不定 )
	//extern	MATRIX_D	MGetRotVec2D(VECTOR_D In1, VECTOR_D In2);										// In1 の向きから In2 の向きへ変換する回転行列を取得する( In2 と In1 が真逆のベクトルの場合は回転軸は不定 )
	//extern	MATRIX		MGetTranslate(VECTOR   Trans);													// 平行移動行列を取得する
	//extern	MATRIX_D	MGetTranslateD(VECTOR_D Trans);													// 平行移動行列を取得する
	//extern	MATRIX		MGetAxis1(VECTOR   XAxis, VECTOR   YAxis, VECTOR   ZAxis, VECTOR   Pos);	// 指定の３軸ローカルのベクトルを基本軸上のベクトルに変換する行列を取得する( x' = XAxis.x * x + YAixs.x * y + ZAxis.z * z + Pos.x   y' = XAxis.y * x + YAixs.y * y + ZAxis.y * z + Pos.y      z' = XAxis.z * x + YAixs.z * y + ZAxis.z * z + Pos.z )
	//extern	MATRIX_D	MGetAxis1D(VECTOR_D XAxis, VECTOR_D YAxis, VECTOR_D ZAxis, VECTOR_D Pos);	// 指定の３軸ローカルのベクトルを基本軸上のベクトルに変換する行列を取得する( x' = XAxis.x * x + YAixs.x * y + ZAxis.z * z + Pos.x   y' = XAxis.y * x + YAixs.y * y + ZAxis.y * z + Pos.y      z' = XAxis.z * x + YAixs.z * y + ZAxis.z * z + Pos.z )
	//extern	MATRIX		MGetAxis2(VECTOR   XAxis, VECTOR   YAxis, VECTOR   ZAxis, VECTOR   Pos);	// 基本軸上のベクトルを指定の３軸上に投影したベクトルに変換する行列を取得する( x' = XAxis.x * ( x - Pos.x ) + XAxis.y * ( x - Pos.x ) + XAxis.z * ( x - Pos.x )    y' = YAxis.x * ( x - Pos.x ) + YAxis.y * ( x - Pos.x ) + YAxis.z * ( x - Pos.x )    z' = ZAxis.x * ( x - Pos.x ) + ZAxis.y * ( x - Pos.x ) + ZAxis.z * ( x - Pos.x ) )
	//extern	MATRIX_D	MGetAxis2D(VECTOR_D XAxis, VECTOR_D YAxis, VECTOR_D ZAxis, VECTOR_D Pos);	// 基本軸上のベクトルを指定の３軸上に投影したベクトルに変換する行列を取得する( x' = XAxis.x * ( x - Pos.x ) + XAxis.y * ( x - Pos.x ) + XAxis.z * ( x - Pos.x )    y' = YAxis.x * ( x - Pos.x ) + YAxis.y * ( x - Pos.x ) + YAxis.z * ( x - Pos.x )    z' = ZAxis.x * ( x - Pos.x ) + ZAxis.y * ( x - Pos.x ) + ZAxis.z * ( x - Pos.x ) )
	//extern	MATRIX		MTranspose(MATRIX   InM);													// 転置行列を取得する
	//extern	MATRIX_D	MTransposeD(MATRIX_D InM);													// 転置行列を取得する
	//extern	MATRIX		MInverse(MATRIX   InM);													// 逆行列を取得する
	//extern	MATRIX_D	MInverseD(MATRIX_D InM);													// 逆行列を取得する
	//extern  VECTOR		MGetSize(MATRIX   InM);													// 拡大行列のＸ軸、Ｙ軸、Ｚ軸の拡大率を取得する
	//extern  VECTOR_D	MGetSizeD(MATRIX_D InM);													// 拡大行列のＸ軸、Ｙ軸、Ｚ軸の拡大率を取得する
	//extern	MATRIX		MGetRotElem(MATRIX   InM);													// 行列の回転成分を取得する
	//extern	MATRIX_D	MGetRotElemD(MATRIX_D InM);													// 行列の回転成分を取得する

	// 行列の平行移動成分を取得する
	//__inline VECTOR MGetTranslateElem(MATRIX &InM);

	// 行列の平行移動成分を取得する
	//__inline VECTOR_D MGetTranslateElemD(MATRIX_D & InM)

	// ベクトル値の変換
	//__inline VECTOR_D VConvFtoD(const VECTOR & In)
	//__inline VECTOR VConvDtoF(const VECTOR_D & In) {

	// ベクトル値の生成
	DXLUA_PORT(t, VGet);
	//__inline VECTOR_D VGetD(double x, double y, double z) {
	//__inline FLOAT2 F2Get(float u, float v) {

	// ４要素ベクトル値の生成
	//__inline FLOAT4 F4Get(float x, float y, float z, float w) {
	//__inline DOUBLE4 D4Get(double x, double y, double z, double w) {

	// ベクトルの加算
	DXLUA_PORT(t, VAdd);
	//__inline VECTOR_D	VAddD(const VECTOR_D & In1, const VECTOR_D & In2) {

	// ベクトルの減算
	DXLUA_PORT(t, VSub);
	//__inline VECTOR_D	VSubD(const VECTOR_D & In1, const VECTOR_D & In2) {
	//__inline FLOAT2		F2Add(const FLOAT2 & In1, const FLOAT2 & In2) {

	// ４要素ベクトルの加算
	//__inline FLOAT4		F4Add(const FLOAT4 & In1, const FLOAT4 & In2) {
	//__inline DOUBLE4	D4Add(const DOUBLE4 & In1, const DOUBLE4 & In2) {
	//__inline FLOAT2		F2Sub(const FLOAT2 & In1, const FLOAT2 & In2) {

	// ４要素ベクトルの減算
	//__inline FLOAT4		F4Sub(const FLOAT4 & In1, const FLOAT4 & In2) {
	//__inline DOUBLE4	D4Sub(const DOUBLE4 & In1, const DOUBLE4 & In2) {

	// ベクトルの内積
	DXLUA_PORT(t, VDot);
	//__inline double		VDotD(const VECTOR_D & In1, const VECTOR_D & In2) {

	// ベクトルの外積
	DXLUA_PORT(t, VCross);
	//__inline VECTOR_D	VCrossD(const VECTOR_D & In1, const VECTOR_D & In2) {

	// ベクトルのスケーリング
	DXLUA_PORT(t, VScale);
	//__inline VECTOR_D	VScaleD(const VECTOR_D & In, double Scale) {
	//__inline FLOAT2		F2Scale(const FLOAT2 & In, float Scale) {

	// ４要素ベクトルのスケーリング
	//__inline FLOAT4		F4Scale(const FLOAT4 & In, float Scale) {
	//__inline DOUBLE4	D4Scale(const DOUBLE4 & In, double Scale) {

	// ベクトルの正規化
	DXLUA_PORT(t, VNorm);
	//extern VECTOR_D		VNormD(VECTOR_D In);

	// ベクトルのサイズ
	DXLUA_PORT(t, VSize);
	//extern double		VSizeD(VECTOR_D In);

	// ベクトルのサイズの２乗
	DXLUA_PORT(t, VSquareSize);
	//__inline double		VSquareSizeD(const VECTOR_D & In) {

	// 行列を使った座標変換
	DXLUA_PORT(t, VTransform);
	//__inline VECTOR_D	VTransformD(const VECTOR_D & InV, const MATRIX_D & InM) {

	// 行列を使った座標変換( スケーリング＋回転成分のみ )
	//__inline VECTOR		VTransformSR(const VECTOR & InV, const MATRIX & InM) {
	//__inline VECTOR_D	VTransformSRD(const VECTOR_D & InV, const MATRIX_D & InM) {

	// 二つのベクトルが成す角のコサイン値を得る
	DXLUA_PORT(t, VCos);
	//extern	double		VCosD(VECTOR_D In1, VECTOR_D In2);

	// 二つのベクトルが成す角の角度を得る( 単位：ラジアン )
	DXLUA_PORT(t, VRad);
	//extern	double		VRadD(VECTOR_D In1, VECTOR_D In2);

	// 2つのクォータニオンの乗算の結果を返す
	//__inline FLOAT4 QTCross(const FLOAT4 & A, const FLOAT4 & B) {
	//__inline DOUBLE4 QTCrossD(const DOUBLE4 & A, const DOUBLE4 & B) {

	// 共役クォータニオンを返す
	//__inline FLOAT4 QTConj(const FLOAT4 & A) {
	//__inline DOUBLE4 QTConjD(const DOUBLE4 & A) {

	// 回転を表すクォータニオンを返す
	//extern FLOAT4  QTRot(VECTOR   Axis, float  Angle);
	//extern DOUBLE4 QTRotD(VECTOR_D Axis, double Angle);

	// 3次元空間上の点を任意の軸の周りに任意の角度だけ回転させる関数
	//extern VECTOR   VRotQ(VECTOR   P, VECTOR   Axis, float  Angle);
	//extern VECTOR_D VRotQD(VECTOR_D P, VECTOR_D Axis, double Angle);
}

} // namespace DxLua::detail
