-- キャラクターに付いていくカメラ
--
-- ＜操作＞
-- 方向キー：キャラクターモデル移動
-- Z,Cキー：カメラの水平角度を変更
-- S,Xキー：カメラの垂直角度を変更

-- 移動速度
local MOVESPEED = 10.0

-- DxChara.x での走りアニメーションの番号
local ANIM_RUN = 1

-- DxChara.x での待機アニメーションの番号
local ANIM_NEUTRAL = 4

-- カメラの回転速度
local CAMERA_ANGLE_SPEED = 3.0

-- カメラの注視点の高さ
local CAMERA_LOOK_AT_HEIGHT = 400.0

-- カメラと注視点の距離
local CAMERA_LOOK_AT_DISTANCE = 2150.0

-- ラインを描く範囲
local LINE_AREA_SIZE = 10000.0

-- ラインの数
local LINE_NUM = 50

local ModelHandle
local AnimTotalTime
local AnimNowTime
local AnimAttachIndex
local RunFlag
local MoveAnimFrameIndex
local Position
local MoveFlag
local Angle
local CameraHAngle
local CameraVAngle
local MoveVector
local SinParam
local CosParam

-- ＤＸライブラリの初期化
function dx.Init()
    -- ３Ｄモデルの読み込み
    ModelHandle = dx.MV1LoadModel("DxChara.x")

    -- カメラの向きを初期化
    CameraHAngle = 0.0
    CameraVAngle = 40.0

    -- 向きを初期化
    Angle = 0.0

    -- 走っているフラグを倒す
    RunFlag = false

    -- 待機アニメーションをアタッチ
    AnimAttachIndex = dx.MV1AttachAnim(ModelHandle, ANIM_NEUTRAL)

    -- 待機アニメーションの総時間を取得しておく
    AnimTotalTime = dx.MV1GetAttachAnimTotalTime(ModelHandle, AnimAttachIndex)

    -- アニメーション再生時間を初期化
    AnimNowTime = 0.0
    dx.MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime)

    -- アニメーションで移動をしているフレームの番号を検索する
    MoveAnimFrameIndex = dx.MV1SearchFrame(ModelHandle, "BasePoint")

    -- アニメーションで移動をしているフレームを無効にする
    dx.MV1SetFrameUserLocalMatrix(ModelHandle, MoveAnimFrameIndex, dx.MV1GetFrameLocalMatrix(ModelHandle, MoveAnimFrameIndex))

    -- ３Ｄモデルの座標を初期化
    Position = dx.VGet(0.0, 0.0, 0.0)

    -- 描画先を裏画面にする
    dx.SetDrawScreen(dx.DX_SCREEN_BACK)

    -- カメラのクリッピング距離を設定
    dx.SetCameraNearFar(100.0, 50000.0)

    -- 背景の色を灰色にする
    dx.SetBackgroundColor(128, 128, 128)
end

-- メインループ(何かキーが押されたらループを抜ける)
function dx.Update()
    -- 画面のクリア
    dx.ClearDrawScreen()

    -- ZCSXキーでカメラの操作
    if dx.CheckHitKey(dx.KEY_INPUT_C) == 1 then
        CameraHAngle = CameraHAngle + CAMERA_ANGLE_SPEED
        if CameraHAngle >= 180.0 then
            CameraHAngle = CameraHAngle - 360.0
        end
    end

    if dx.CheckHitKey(dx.KEY_INPUT_Z) == 1 then
        CameraHAngle = CameraHAngle - CAMERA_ANGLE_SPEED
        if CameraHAngle <= -180.0 then
            CameraHAngle = CameraHAngle + 360.0
        end
    end

    if dx.CheckHitKey(dx.KEY_INPUT_S) == 1 then
        CameraVAngle = CameraVAngle + CAMERA_ANGLE_SPEED
        if CameraVAngle >= 80.0 then
            CameraVAngle = 80.0
        end
    end

    if dx.CheckHitKey(dx.KEY_INPUT_X) == 1 then
        CameraVAngle = CameraVAngle - CAMERA_ANGLE_SPEED
        if CameraVAngle <= 0.0 then
            CameraVAngle = 0.0
        end
    end

    -- 移動ベクトルを初期化
    MoveVector = dx.VGet(0.0, 0.0, 0.0)

    -- 移動しているかどうかのフラグを倒す
    MoveFlag = false

    -- 方向入力に従ってキャラクターの移動ベクトルと向きを設定
    if dx.CheckHitKey(dx.KEY_INPUT_LEFT) == 1 then
        Angle = 90.0 - CameraHAngle
        MoveFlag = true
        MoveVector.x = -MOVESPEED
    end

    if dx.CheckHitKey(dx.KEY_INPUT_RIGHT) == 1 then
        Angle = -90.0 - CameraHAngle
        MoveFlag = true
        MoveVector.x = MOVESPEED
    end

    if dx.CheckHitKey(dx.KEY_INPUT_DOWN) == 1 then
        Angle = 0.0 - CameraHAngle
        MoveFlag = true
        MoveVector.z = -MOVESPEED
    end

    if dx.CheckHitKey(dx.KEY_INPUT_UP) == 1 then
        Angle = 180.0 - CameraHAngle
        MoveFlag = true
        MoveVector.z = MOVESPEED
    end

    -- 移動した場合は、カメラの水平角度を考慮した方向に座標を移動する
    if MoveFlag == true then
        TempMoveVector = TempMoveVector or dx.VECTOR()

        -- カメラの角度に合わせて移動ベクトルを回転してから加算
        SinParam = math.sin(CameraHAngle / 180.0 * dx.DX_PI_F)
        CosParam = math.cos(CameraHAngle / 180.0 * dx.DX_PI_F)
        TempMoveVector.x = MoveVector.x * CosParam - MoveVector.z * SinParam
        TempMoveVector.y = 0.0
        TempMoveVector.z = MoveVector.x * SinParam + MoveVector.z * CosParam

        Position = dx.VAdd(Position, TempMoveVector)
    end

    -- 今までと状態が変化した場合はアニメーションを変更する
    if RunFlag ~= MoveFlag then
        -- 走っているかどうかのフラグを保存
        RunFlag = MoveFlag

        -- 今までアタッチしていたアニメーションをデタッチ
        dx.MV1DetachAnim(ModelHandle, AnimAttachIndex)

        -- 新しいアニメーションをアタッチ
        if RunFlag then
            AnimAttachIndex = dx.MV1AttachAnim(ModelHandle, ANIM_RUN)
        else
            AnimAttachIndex = dx.MV1AttachAnim(ModelHandle, ANIM_NEUTRAL)
        end

        -- アニメーションの総時間を取得しておく
        AnimTotalTime = dx.MV1GetAttachAnimTotalTime(ModelHandle, AnimAttachIndex)

        -- アニメーション再生時間を初期化
        AnimNowTime = 0.0
    else
        -- アニメーション再生時間を進める
        AnimNowTime = AnimNowTime + 200.0

        -- アニメーション再生時間がアニメーションの総時間を越えていたらループさせる
        if AnimNowTime >= AnimTotalTime then
            -- 新しいアニメーション再生時間は、アニメーション再生時間からアニメーション総時間を引いたもの
            AnimNowTime = AnimNowTime - AnimTotalTime
        end
    end

    -- 新しいアニメーション再生時間をセット
    dx.MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime)

    -- 新しい向きをセット
    dx.MV1SetRotationXYZ(ModelHandle, dx.VGet(0.0, Angle / 180.0 * dx.DX_PI_F, 0.0))

    -- ３Ｄモデルに新しい座標をセット
    dx.MV1SetPosition(ModelHandle, Position)

    -- カメラの位置と向きを設定
    do
        TempPosition1 = TempPosition1 or dx.VECTOR()
        TempPosition2 = TempPosition2 or dx.VECTOR()
        local CameraPosition
        local CameraLookAtPosition

        -- 注視点はキャラクターモデルの座標から CAMERA_LOOK_AT_HEIGHT 分だけ高い位置
        CameraLookAtPosition = dx.VECTOR(Position)
        CameraLookAtPosition.y = CameraLookAtPosition.y + CAMERA_LOOK_AT_HEIGHT

        -- カメラの位置はカメラの水平角度と垂直角度から算出

        -- 最初に垂直角度を反映した位置を算出
        SinParam = math.sin(CameraVAngle / 180.0 * dx.DX_PI_F)
        CosParam = math.cos(CameraVAngle / 180.0 * dx.DX_PI_F)
        TempPosition1.x = 0.0
        TempPosition1.y = SinParam * CAMERA_LOOK_AT_DISTANCE
        TempPosition1.z = -CosParam * CAMERA_LOOK_AT_DISTANCE

        -- 次に水平角度を反映した位置を算出
        SinParam = math.sin(CameraHAngle / 180.0 * dx.DX_PI_F)
        CosParam = math.cos(CameraHAngle / 180.0 * dx.DX_PI_F)
        TempPosition2.x = CosParam * TempPosition1.x - SinParam * TempPosition1.z
        TempPosition2.y = TempPosition1.y
        TempPosition2.z = SinParam * TempPosition1.x + CosParam * TempPosition1.z

        -- 算出した座標に注視点の位置を加算したものがカメラの位置
        CameraPosition = dx.VAdd(TempPosition2, CameraLookAtPosition)

        -- カメラの設定に反映する
        dx.SetCameraPositionAndTarget_UpVecY(CameraPosition, CameraLookAtPosition)
    end

    -- ３Ｄモデルの描画
    dx.MV1DrawModel(ModelHandle)

    -- 位置関係が分かるように地面にラインを描画する
    do
        local Pos1
        local Pos2

        dx.SetUseZBufferFlag(true)

        Pos1 = dx.VGet(-LINE_AREA_SIZE / 2.0, 0.0, -LINE_AREA_SIZE / 2.0)
        Pos2 = dx.VGet(-LINE_AREA_SIZE / 2.0, 0.0, LINE_AREA_SIZE / 2.0)
        for i = 1, LINE_NUM + 1 do
            dx.DrawLine3D(Pos1, Pos2, dx.GetColor(255, 255, 255))
            Pos1.x = Pos1.x + (LINE_AREA_SIZE / LINE_NUM)
            Pos2.x = Pos2.x + (LINE_AREA_SIZE / LINE_NUM)
        end

        Pos1 = dx.VGet(-LINE_AREA_SIZE / 2.0, 0.0, -LINE_AREA_SIZE / 2.0)
        Pos2 = dx.VGet(LINE_AREA_SIZE / 2.0, 0.0, -LINE_AREA_SIZE / 2.0)
        for i = 1, LINE_NUM do
            dx.DrawLine3D(Pos1, Pos2, dx.GetColor(255, 255, 255))
            Pos1.z = Pos1.z + (LINE_AREA_SIZE / LINE_NUM)
            Pos2.z = Pos2.z + (LINE_AREA_SIZE / LINE_NUM)
        end

        dx.SetUseZBufferFlag(false)
    end

    -- 裏画面の内容を表画面に反映
    dx.ScreenFlip()
end
