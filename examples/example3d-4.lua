-- 鏡面効果基本

local SCREEN_W = 640		-- 画面の横幅
local SCREEN_H = 480		-- 画面の縦幅

local MIRROR_SCREEN_W = 640		-- 鏡に映る映像の取得に使用するスクリーンの横解像度
local MIRROR_SCREEN_H = 480		-- 鏡に映る映像の取得に使用するスクリーンの縦解像度
local MIRROR_POINTNUM = 64		-- 鏡の描画に使用する面の頂点分割数
local MIRROR_NUM = 2		-- 鏡の数
local MIRROR_DEBUG_SCALE = 4		-- 鏡のデバッグ表示時に元の何分の１にするかのサイズ

local CAMERA_SPEED = 32.0		-- カメラの速度

local CharaModel		-- キャラクターモデル
local StageModel		-- ステージモデル

local MirrorHandle = { 0, 0 }		-- 鏡に映る映像の取得に使用するスクリーン
local MirrorScreenPosW = {
    { dx.FLOAT4(), dx.FLOAT4(), dx.FLOAT4(), dx.FLOAT4() },
    { dx.FLOAT4(), dx.FLOAT4(), dx.FLOAT4(), dx.FLOAT4() }
}	-- 鏡に映る映像の取得に使用するクリーンの中の鏡の四隅の座標( 同次座標 )

-- 鏡のワールド座標
local MirrorWorldPos =
{
    {
        dx.VECTOR {  2000.0, 2000.0, -4498.0 },
        dx.VECTOR { -2000.0, 2000.0, -4498.0 },
        dx.VECTOR {  2000.0,    0.0, -4498.0 },
        dx.VECTOR { -2000.0,    0.0, -4498.0 },
    },

    {
        dx.VECTOR { -4000.0, 10.0,  4500.0 },
        dx.VECTOR {  4000.0, 10.0,  4500.0 },
        dx.VECTOR { -4000.0, 10.0, -4500.0 },
        dx.VECTOR {  4000.0, 10.0, -4500.0 },
    }
}

-- 鏡の Ambient Color
local MirrorAmbientColor =
{
    dx.COLOR_F { 1.0, 1.0, 1.0, 1.0 },
    dx.COLOR_F { 0.0, 0.0, 0.0, 0.0 },
}

-- 鏡の Diffuse Color
local MirrorDiffuseColor =
{
    { 255, 255, 255, 255 },
    {   0, 255, 255, 255 },
}

-- 鏡のブレンドモードとパラメータ
local MirrorBlendParam =
{
    { dx.DX_BLENDMODE_NOBLEND, 255 },
    { dx.DX_BLENDMODE_ALPHA,   128 },
}

local CameraAngle		-- カメラの水平角度
local CameraEyePosition = dx.VECTOR()	-- カメラの座標
local CameraDirection = dx.VECTOR()	-- カメラの向いている方向
local CameraTargetPosition = dx.VECTOR()	-- カメラの注視点

-- 画面の解像度と色ビット深度を設定
dx.SetGraphMode(SCREEN_W, SCREEN_H, 32)

-- ＤＸライブラリの初期化
function dx.Init()
    -- 鏡処理の初期化
    Mirror_Initialize()

    -- キャラクターモデルの読み込み
    CharaModel = dx.MV1LoadModel("DxChara.x")

    -- ステージモデルの読み込み
    StageModel = dx.MV1LoadModel("ColTestStage.mqo")

    -- キャラクターモデルの設置
    dx.MV1SetPosition(CharaModel, dx.VGet(0.0, 0.0, -3500.0))
    dx.MV1SetRotationXYZ(CharaModel, dx.VGet(0.0, 3.14159 / 2.0, 0.0))
    dx.MV1AttachAnim(CharaModel, 7)

    -- カメラの角度を初期化
    CameraAngle = 325

    -- カメラの座標をセット
    CameraEyePosition = dx.VGet(-2605.0, 670.0, -2561.0)

    -- ライトの向きをセット
    dx.SetLightDirection(dx.VGet(1.0, -1.0, -1.0))
end

-- メインループ(何かキーが押されたらループを抜ける)
function dx.Update()
    -- 左右キーが押されたらカメラを回転する
    if dx.CheckHitKey(dx.KEY_INPUT_LEFT) ~= 0 then
        CameraAngle = CameraAngle + 3
        if CameraAngle > 360 then
            CameraAngle = CameraAngle - 360
        end
    end
    if dx.CheckHitKey(dx.KEY_INPUT_RIGHT) ~= 0 then
        CameraAngle = CameraAngle - 3
        if CameraAngle < 0 then
            CameraAngle = CameraAngle + 360
        end
    end

    -- カメラの向いている方向を算出
    CameraDirection.x = math.cos(CameraAngle * 3.14159 / 180.0)
    CameraDirection.y = 0.0
    CameraDirection.z = math.sin(CameraAngle * 3.14159 / 180.0)

    -- 上下キーが押されたらカメラを前進・後進させる
    if dx.CheckHitKey(dx.KEY_INPUT_UP) ~= 0 then
        CameraEyePosition = dx.VAdd(CameraEyePosition, dx.VScale(CameraDirection, CAMERA_SPEED))
    end
    if dx.CheckHitKey(dx.KEY_INPUT_DOWN) ~= 0 then
        CameraEyePosition = dx.VSub(CameraEyePosition, dx.VScale(CameraDirection, CAMERA_SPEED))
    end

    -- カメラの注視点座標を算出
    CameraTargetPosition = dx.VAdd(CameraEyePosition, CameraDirection)


    -- 鏡に映る映像を描画
    for i = 1, MIRROR_NUM do
        -- 描画先を鏡に映る映像の取得に使用する画像を描画するスクリーンにする
        dx.SetDrawScreen(MirrorHandle[i])

        -- 画面を初期化する
        dx.ClearDrawScreen()

        -- 鏡に映る映像を描画する際に使用するカメラの設定を行う
        Mirror_SetupCamera(i, CameraEyePosition, CameraTargetPosition)

        -- ３Ｄ空間の描画
        World_RenderProcess()
    end


    -- 裏画面を描画対象にする
    dx.SetDrawScreen(dx.DX_SCREEN_BACK)

    -- 画面をクリア
    dx.ClearDrawScreen()

    -- カメラの設定を行う
    dx.SetCameraPositionAndTarget_UpVecY(CameraEyePosition, CameraTargetPosition)

    -- ３Ｄ空間の描画
    World_RenderProcess()

    -- 鏡の描画
    for i = 1, MIRROR_NUM do
        Mirror_Render(i)
    end


    -- 画面左上に鏡のデバッグ表示
    for i = 1, MIRROR_NUM do
        Mirror_DebugRender(i, 20 + 180 * (i - 1), 0)
    end


    -- 裏画面の内容を表画面に反映
    dx.ScreenFlip()
end

-- 鏡の初期化
function Mirror_Initialize()
    -- 鏡に映る映像の取得に使用するスクリーンの作成
    for i = 1, MIRROR_NUM do
        MirrorHandle[i] = dx.MakeScreen(MIRROR_SCREEN_W, MIRROR_SCREEN_H, false)
    end
end

-- 鏡に映る映像を描画するためのカメラの設定を行う
function Mirror_SetupCamera(MirrorNo, CameraEyePos, CameraTargetPos)
    local EyeLength, TargetLength
    local MirrorNormal
    local MirrorWorldPosP
    local MirrorCameraEyePos, MirrorCameraTargetPos

    MirrorWorldPosP = MirrorWorldPos[MirrorNo]

    -- 鏡の面の法線を算出
    MirrorNormal = dx.VNorm(dx.VCross(dx.VSub(MirrorWorldPosP[2], MirrorWorldPosP[1]), dx.VSub(MirrorWorldPosP[3], MirrorWorldPosP[1])))

    -- 鏡の面からカメラの座標までの最短距離、鏡の面からカメラの注視点までの最短距離を算出
    EyeLength = dx.Plane_Point_MinLength(MirrorWorldPosP[1], MirrorNormal, CameraEyePos)
    TargetLength = dx.Plane_Point_MinLength(MirrorWorldPosP[1], MirrorNormal, CameraTargetPos)

    -- 鏡に映る映像を描画する際に使用するカメラの座標とカメラの注視点を算出
    MirrorCameraEyePos = dx.VAdd(CameraEyePos, dx.VScale(MirrorNormal, -EyeLength * 2.0))
    MirrorCameraTargetPos = dx.VAdd(CameraTargetPos, dx.VScale(MirrorNormal, -TargetLength * 2.0))

    -- 計算で得られたカメラの座標とカメラの注視点の座標をカメラの設定する
    dx.SetCameraPositionAndTarget_UpVecY(MirrorCameraEyePos, MirrorCameraTargetPos)

    -- 鏡に映る映像の中での鏡の四隅の座標を算出( 同次座標 )
    for i = 1, 4 do
        MirrorScreenPosW[MirrorNo][i] = dx.ConvWorldPosToScreenPosPlusW(MirrorWorldPosP[i])
    end
end

-- 鏡の描画
function Mirror_Render(MirrorNo)
    local Vert = {}
    local Index = {}
    local Material = dx.MATERIALPARAM()
    local HUnitPos
    local VUnitPos = {}
    local HPos
    local VPos = {}
    local HUnitUV
    local VUnitUV = {}
    local HUV
    local VUV = {}
    local MirrorNormal
    local DiffuseColor
    local SpecularColor
    local TextureW, TextureH
    local MirrorWorldPosP
    local k

    MirrorWorldPosP = MirrorWorldPos[MirrorNo]

    -- 鏡の描画に使用するマテリアルのセットアップ
    Material.Ambient = MirrorAmbientColor[MirrorNo]
    Material.Diffuse = dx.GetColorF(0.0, 0.0, 0.0, 0.0)
    Material.Emissive = dx.GetColorF(0.0, 0.0, 0.0, 0.0)
    Material.Specular = dx.GetColorF(0.0, 0.0, 0.0, 0.0)
    Material.Power = 1.0
    dx.SetMaterialParam(Material)

    -- 鏡の面の法線を算出
    MirrorNormal = dx.VNorm(dx.VCross(dx.VSub(MirrorWorldPosP[2], MirrorWorldPosP[1]), dx.VSub(MirrorWorldPosP[3], MirrorWorldPosP[1])))

    -- 鏡に映る映像を書き込んだ画像のテクスチャのサイズを取得
    _, TextureW, TextureH = dx.GetGraphTextureSize(MirrorHandle[MirrorNo])

    -- 鏡の描画に使用する頂点のセットアップ
    VUnitPos[1] = dx.VScale(dx.VSub(MirrorWorldPosP[3], MirrorWorldPosP[1]), 1.0 / (MIRROR_POINTNUM - 1))
    VUnitPos[2] = dx.VScale(dx.VSub(MirrorWorldPosP[4], MirrorWorldPosP[2]), 1.0 / (MIRROR_POINTNUM - 1))
    VUnitUV[1] = dx.F4Scale(dx.F4Sub(MirrorScreenPosW[MirrorNo][3], MirrorScreenPosW[MirrorNo][1]), 1.0 / (MIRROR_POINTNUM - 1))
    VUnitUV[2] = dx.F4Scale(dx.F4Sub(MirrorScreenPosW[MirrorNo][4], MirrorScreenPosW[MirrorNo][2]), 1.0 / (MIRROR_POINTNUM - 1))
    DiffuseColor = dx.GetColorU8(MirrorDiffuseColor[MirrorNo][1], MirrorDiffuseColor[MirrorNo][2], MirrorDiffuseColor[MirrorNo][3], MirrorDiffuseColor[MirrorNo][4])
    SpecularColor = dx.GetColorU8(0, 0, 0, 0)
    VPos[1] = MirrorWorldPosP[1]
    VPos[2] = MirrorWorldPosP[2]
    VUV[1] = MirrorScreenPosW[MirrorNo][1]
    VUV[2] = MirrorScreenPosW[MirrorNo][2]
    k = 1
    for i = 1, MIRROR_POINTNUM do
        HUnitPos = dx.VScale(dx.VSub(VPos[2], VPos[1]), 1.0 / (MIRROR_POINTNUM - 1))
        HPos = VPos[1]
        HUnitUV = dx.F4Scale(dx.F4Sub(VUV[2], VUV[1]), 1.0 / (MIRROR_POINTNUM - 1))
        HUV = VUV[1]
        for j = 1, MIRROR_POINTNUM do
            Vert[k] = dx.VERTEX3D()
            Vert[k].pos = HPos
            Vert[k].norm = MirrorNormal
            Vert[k].dif = DiffuseColor
            Vert[k].spc = SpecularColor
            Vert[k].u = HUV.x / (HUV.w * TextureW)
            Vert[k].v = HUV.y / (HUV.w * TextureH)
            Vert[k].su = 0.0
            Vert[k].sv = 0.0
            HUV = dx.F4Add(HUV, HUnitUV)
            HPos = dx.VAdd(HPos, HUnitPos)
            k = k + 1
        end
        VUV[1] = dx.F4Add(VUV[1], VUnitUV[1])
        VUV[2] = dx.F4Add(VUV[2], VUnitUV[2])
        VPos[1] = dx.VAdd(VPos[1], VUnitPos[1])
        VPos[2] = dx.VAdd(VPos[2], VUnitPos[2])
    end

    -- 鏡の描画に使用する頂点インデックスをセットアップ
    k = 1
    for i = 1, MIRROR_POINTNUM - 1 do
        for j = 1, MIRROR_POINTNUM - 1 do
            Index[k + 0] = (i + 0 - 1) * MIRROR_POINTNUM + j + 0 - 1
            Index[k + 1] = (i + 0 - 1) * MIRROR_POINTNUM + j + 1 - 1
            Index[k + 2] = (i + 1 - 1) * MIRROR_POINTNUM + j + 0 - 1
            Index[k + 3] = (i + 1 - 1) * MIRROR_POINTNUM + j + 1 - 1
            Index[k + 4] = Index[k + 2]
            Index[k + 5] = Index[k + 1]
            k = k + 6
        end
    end

    -- 描画モードをバイリニアフィルタリングに設定
    dx.SetDrawMode(dx.DX_DRAWMODE_BILINEAR)

    -- 描画ブレンドモードを変更
    dx.SetDrawBlendMode(MirrorBlendParam[MirrorNo][1], MirrorBlendParam[MirrorNo][2])

    -- 描画にＺバッファを使用する
    dx.SetUseZBuffer3D(true)

    -- Ｚバッファに書き込みを行う
    dx.SetWriteZBuffer3D(true)

    -- 鏡を描画
    dx.DrawPolygonIndexed3D(Vert, MIRROR_POINTNUM * MIRROR_POINTNUM, Index, (MIRROR_POINTNUM - 1) * (MIRROR_POINTNUM - 1) * 2, MirrorHandle[MirrorNo], false)

    -- Ｚバッファに書き込みを行う設定を元に戻す
    dx.SetWriteZBuffer3D(false)

    -- Ｚバッファを使用する設定を元に戻す
    dx.SetUseZBuffer3D(false)

    -- 描画ブレンドモードを元に戻す
    dx.SetDrawBlendMode(dx.DX_BLENDMODE_NOBLEND, 255)

    -- 描画モードを元に戻す
    dx.SetDrawMode(dx.DX_DRAWMODE_NEAREST)
end

-- 鏡のデバッグ描画
function Mirror_DebugRender(MirrorNo, DrawX, DrawY)
    local HUnitUV
    local VUnitUV = {}
    local HUV
    local VUV = {}
    local x
    local y

    -- 指定の座標に鏡の映像の取得に使用したスクリーンを描画
    dx.DrawExtendGraph(DrawX, DrawY, DrawX + MIRROR_SCREEN_W / MIRROR_DEBUG_SCALE, DrawY + MIRROR_SCREEN_H / MIRROR_DEBUG_SCALE, MirrorHandle[MirrorNo], false)

    -- 鏡の映像の取得に使用したスクリーンの中の鏡の部分に点を描画
    VUnitUV[1] = dx.F4Scale(dx.F4Sub(MirrorScreenPosW[MirrorNo][3], MirrorScreenPosW[MirrorNo][1]), 1.0 / (MIRROR_POINTNUM - 1))
    VUnitUV[2] = dx.F4Scale(dx.F4Sub(MirrorScreenPosW[MirrorNo][4], MirrorScreenPosW[MirrorNo][2]), 1.0 / (MIRROR_POINTNUM - 1))
    VUV[1] = MirrorScreenPosW[MirrorNo][1]
    VUV[2] = MirrorScreenPosW[MirrorNo][2]
    for i = 1, MIRROR_POINTNUM do
        HUnitUV = dx.F4Scale(dx.F4Sub(VUV[2], VUV[1]), 1.0 / (MIRROR_POINTNUM - 1))
        HUV = VUV[1]
        for j = 1, MIRROR_POINTNUM do
            x = math.floor(HUV.x / HUV.w / MIRROR_DEBUG_SCALE)
            y = math.floor(HUV.y / HUV.w / MIRROR_DEBUG_SCALE)

            if x > 0 and x < MIRROR_SCREEN_W / MIRROR_DEBUG_SCALE and
                y > 0 and y < MIRROR_SCREEN_H / MIRROR_DEBUG_SCALE then
                    dx.DrawPixel(DrawX + x, DrawY + y, dx.GetColor(0, 255, 0))
            end

            HUV = dx.F4Add(HUV, HUnitUV)
        end
        VUV[1] = dx.F4Add(VUV[1], VUnitUV[1])
        VUV[2] = dx.F4Add(VUV[2], VUnitUV[2])
    end
end

-- ３Ｄ空間の描画
function World_RenderProcess()
    -- ステージモデルの描画
    dx.MV1DrawModel(StageModel)

    -- キャラクターモデルの描画
    dx.MV1DrawModel(CharaModel)
end
