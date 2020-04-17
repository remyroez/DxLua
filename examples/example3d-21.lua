-- アニメーションによる座標移動　＋　方向入力によるキャラクター移動( カメラ固定 )

-- DxChara.x での走りアニメーションの番号
local ANIM_RUN = 1

-- DxChara.x での待機アニメーションの番号
local ANIM_NEUTRAL = 4

local ModelHandle
local AnimTotalTime
local AnimNowTime
local AnimAttachIndex
local RunFlag
local MoveAnimFrameIndex
local Position
local Input
local MoveFlag
local Angle
local PrevPosition
local NowPosition
local MoveVector
local SinParam
local CosParam

-- ＤＸライブラリの初期化
function dx.Init()
    -- ３Ｄモデルの読み込み
    ModelHandle = dx.MV1LoadModel("DxChara.x")

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

    -- カメラの位置と向きを設定
    dx.SetCameraPositionAndTarget_UpVecY(dx.VGet(0.0, 1200.0, -2000.0), dx.VGet(0.0, 400.0, 0.0))

    -- カメラのクリッピング距離を設定
    dx.SetCameraNearFar(16.0, 5000.0)

    -- 背景の色を灰色にする
    dx.SetBackgroundColor(128, 128, 128)
end

-- メインループ(何かキーが押されたらループを抜ける)
function dx.Update()
    -- 画面のクリア
    dx.ClearDrawScreen()

    -- ゲームパッド＋キーボードの入力を取得
    Input = dx.GetJoypadInputState(dx.DX_INPUT_KEY_PAD1)

    -- 移動しているかどうかのフラグを倒す
    MoveFlag = false

    -- 方向入力の判定
    -- ( 最初に斜め入力の判定を、その後上下左右単体の入力の判定をしています )
    if bit.band(Input, dx.PAD_INPUT_LEFT) ~= 0 and bit.band(Input, dx.PAD_INPUT_UP) ~= 0 then
        Angle = 135.0
        MoveFlag = true
    elseif bit.band(Input, dx.PAD_INPUT_UP) ~= 0 and bit.band(Input, dx.PAD_INPUT_RIGHT) ~= 0 then
        Angle = -135.0
        MoveFlag = true
    elseif bit.band(Input, dx.PAD_INPUT_RIGHT) ~= 0 and bit.band(Input, dx.PAD_INPUT_DOWN) ~= 0 then
        Angle = -45.0
        MoveFlag = true
    elseif bit.band(Input, dx.PAD_INPUT_DOWN) ~= 0 and bit.band(Input, dx.PAD_INPUT_LEFT) ~= 0 then
        Angle = 45.0
        MoveFlag = true
    elseif bit.band(Input, dx.PAD_INPUT_LEFT) ~= 0 then
        Angle = 90.0
        MoveFlag = true
    elseif bit.band(Input, dx.PAD_INPUT_RIGHT) ~= 0 then
        Angle = -90.0
        MoveFlag = true
    elseif bit.band(Input, dx.PAD_INPUT_DOWN) ~= 0 then
        Angle = 0.0
        MoveFlag = true
    elseif bit.band(Input, dx.PAD_INPUT_UP) ~= 0 then
        Angle = 180.0
        MoveFlag = true
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
        dx.MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime)
    end

    -- アニメーション時間を進める前の『アニメーションで移動をしているフレームの座標』を取得しておく
    PrevPosition = dx.MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex)

    -- アニメーション再生時間を進める
    AnimNowTime = AnimNowTime + 200.0

    -- アニメーション再生時間がアニメーションの総時間を越えているかどうかで処理を分岐
    if AnimNowTime >= AnimTotalTime then
        -- 超えている場合は、まず『アニメーション再生時間を進める前の「アニメーションで移動しているフレームの座標」』と、
        -- 『アニメーションの終端での「アニメーションで移動しているフレームの座標」』との差分を移動ベクトルとする
        dx.MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimTotalTime)
        NowPosition = dx.MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex)
        MoveVector = dx.VSub(NowPosition, PrevPosition)

        -- 新しいアニメーション再生時間は、アニメーション再生時間からアニメーション総時間を引いたもの
        AnimNowTime = AnimNowTime - AnimTotalTime

        -- 次に『新しいアニメーション再生時間での「アニメーションで移動をしているフレームの座標」と
        -- 『アニメーション再生時間０での「アニメーションで移動しているフレームの座標」』との差分を移動ベクトルに加算する
        dx.MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, 0.0)
        PrevPosition = dx.MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex)
        dx.MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime)
        NowPosition = dx.MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex)
        MoveVector = dx.VAdd(MoveVector, dx.VSub(NowPosition, PrevPosition))
    else
        -- 新しいアニメーション再生時間をセット
        dx.MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime)

        -- 『新しいアニメーション再生時間での「アニメーションで移動をしているフレームの座標」』と、
        -- 『アニメーション再生時間を進める前の「アニメーションで移動をしているフレームの座標」』との差分を移動ベクトルとする
        NowPosition = dx.MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex)
        MoveVector = dx.VSub(NowPosition, PrevPosition)
    end

    -- キャラクターの向きに合わせて移動ベクトルの角度を回転させて、キャラクターモデルの座標に加算
    SinParam = math.sin(-Angle / 180.0 * dx.DX_PI_F)
    CosParam = math.cos(-Angle / 180.0 * dx.DX_PI_F)
    Position.x = Position.x + (MoveVector.x * CosParam - MoveVector.z * SinParam)
    Position.z = Position.z + (MoveVector.x * SinParam + MoveVector.z * CosParam)

    -- ３Ｄモデルに新しい座標をセット
    dx.MV1SetPosition(ModelHandle, Position)

    -- 新しい向きをセット
    dx.MV1SetRotationXYZ(ModelHandle, dx.VGet(0.0, Angle / 180.0 * dx.DX_PI_F, 0.0))

    -- ３Ｄモデルの描画
    dx.MV1DrawModel(ModelHandle)

    -- 裏画面の内容を表画面に反映
    dx.ScreenFlip()
end
