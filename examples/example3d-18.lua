-- 方向入力に従ってキャラクターを移動させる

-- 移動速度
local MOVESPEED = 10.0

-- DxChara.x での走りアニメーションの番号
local ANIM_RUN = 1

-- DxChara.x での待機アニメーションの番号
local ANIM_NEUTRAL = 4

-- 斜め入力用の各軸の倍率( sin( 45°) )
local VECTOR_SCALING = 0.70710678118

local ModelHandle;
local AnimTotalTime;
local AnimNowTime;
local AnimAttachIndex;
local RunFlag;
local MoveAnimFrameIndex;
local Position;
local Input;
local MoveFlag;
local Angle;

-- ＤＸライブラリの初期化
function dx.Init()
    -- ３Ｄモデルの読み込み
    ModelHandle = dx.MV1LoadModel("DxChara.x");

    -- 向きを初期化
    Angle = 0.0;

    -- 走っているフラグを倒す
    RunFlag = false;

    -- 待機アニメーションをアタッチ
    AnimAttachIndex = dx.MV1AttachAnim(ModelHandle, ANIM_NEUTRAL);

    -- 待機アニメーションの総時間を取得しておく
    AnimTotalTime = dx.MV1GetAttachAnimTotalTime(ModelHandle, AnimAttachIndex);

    -- アニメーション再生時間を初期化
    AnimNowTime = 0.0;
    dx.MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime);

    -- アニメーションで移動をしているフレームの番号を検索する
    MoveAnimFrameIndex = dx.MV1SearchFrame(ModelHandle, "BasePoint");

    -- アニメーションで移動をしているフレームを無効にする
    dx.MV1SetFrameUserLocalMatrix(ModelHandle, MoveAnimFrameIndex, dx.MGetIdent());

    -- ３Ｄモデルの座標を初期化
    Position = dx.VGet(0.0, 0.0, 0.0);

    -- 描画先を裏画面にする
    dx.SetDrawScreen(dx.DX_SCREEN_BACK);

    -- カメラの位置と向きを設定
    dx.SetCameraPositionAndTarget_UpVecY(dx.VGet(0.0, 1200.0, -2000.0), dx.VGet(0.0, 400.0, 0.0));

    -- カメラのクリッピング距離を設定
    dx.SetCameraNearFar(16.0, 5000.0);

    -- 背景の色を灰色にする
    dx.SetBackgroundColor(128, 128, 128);
end

-- メインループ(何かキーが押されたらループを抜ける)
function dx.Update()
    -- 画面のクリア
    dx.ClearDrawScreen();

    -- ゲームパッド＋キーボードの入力を取得
    Input = dx.GetJoypadInputState(dx.DX_INPUT_KEY_PAD1);

    -- 移動しているかどうかのフラグを倒す
    MoveFlag = false;

    -- 方向入力に従ってキャラクターを移動
    -- ( 最初に斜め入力の判定を、その後上下左右単体の入力の判定をしています )
    if bit.band(Input, dx.PAD_INPUT_LEFT) ~= 0 and bit.band(Input, dx.PAD_INPUT_UP) ~= 0 then
        Angle = 135.0;
        MoveFlag = true;
        Position.x = Position.x - (MOVESPEED * VECTOR_SCALING)
        Position.z = Position.z + (MOVESPEED * VECTOR_SCALING)
    elseif bit.band(Input, dx.PAD_INPUT_UP) ~= 0 and bit.band(Input, dx.PAD_INPUT_RIGHT) ~= 0 then
        Angle = -135.0;
        MoveFlag = true;
        Position.x = Position.x + (MOVESPEED * VECTOR_SCALING)
        Position.z = Position.z + (MOVESPEED * VECTOR_SCALING)
    elseif bit.band(Input, dx.PAD_INPUT_RIGHT) ~= 0 and bit.band(Input, dx.PAD_INPUT_DOWN) ~= 0 then
        Angle = -45.0;
        MoveFlag = true;
        Position.x = Position.x + (MOVESPEED * VECTOR_SCALING)
        Position.z = Position.z - (MOVESPEED * VECTOR_SCALING)
    elseif bit.band(Input, dx.PAD_INPUT_DOWN) ~= 0 and bit.band(Input, dx.PAD_INPUT_LEFT) ~= 0 then
        Angle = 45.0;
        MoveFlag = true;
        Position.x = Position.x - (MOVESPEED * VECTOR_SCALING)
        Position.z = Position.z - (MOVESPEED * VECTOR_SCALING)
    elseif bit.band(Input, dx.PAD_INPUT_LEFT) ~= 0 then
        Angle = 90.0;
        MoveFlag = true;
        Position.x = Position.x - MOVESPEED
    elseif bit.band(Input, dx.PAD_INPUT_RIGHT) ~= 0 then
        Angle = -90.0;
        MoveFlag = true;
        Position.x = Position.x + MOVESPEED
    elseif bit.band(Input, dx.PAD_INPUT_DOWN) ~= 0 then
        Angle = 0.0;
        MoveFlag = true;
        Position.z = Position.z - MOVESPEED
    elseif bit.band(Input, dx.PAD_INPUT_UP) ~= 0 then
        Angle = 180.0;
        MoveFlag = true;
        Position.z = Position.z + MOVESPEED
    end

    -- 今までと状態が変化した場合はアニメーションを変更する
    if RunFlag ~= MoveFlag then
        -- 走っているかどうかのフラグを保存
        RunFlag = MoveFlag;

        -- 今までアタッチしていたアニメーションをデタッチ
        dx.MV1DetachAnim(ModelHandle, AnimAttachIndex);

        -- 新しいアニメーションをアタッチ
        if RunFlag then
            AnimAttachIndex = dx.MV1AttachAnim(ModelHandle, ANIM_RUN);
        else
            AnimAttachIndex = dx.MV1AttachAnim(ModelHandle, ANIM_NEUTRAL);
        end

        -- アニメーションの総時間を取得しておく
        AnimTotalTime = dx.MV1GetAttachAnimTotalTime(ModelHandle, AnimAttachIndex);

        -- アニメーション再生時間を初期化
        AnimNowTime = 0.0;
    else
        -- アニメーション再生時間を進める
        AnimNowTime = AnimNowTime + (200.0)

        -- アニメーション再生時間がアニメーションの総時間を越えていたらループさせる
        if AnimNowTime >= AnimTotalTime then
            -- 新しいアニメーション再生時間は、アニメーション再生時間からアニメーション総時間を引いたもの
            AnimNowTime = AnimNowTime - AnimTotalTime
        end
    end

    -- 新しいアニメーション再生時間をセット
    dx.MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime);

    -- 新しい向きをセット
    dx.MV1SetRotationXYZ(ModelHandle, dx.VGet(0.0, Angle / 180.0 * dx.DX_PI_F, 0.0));

    -- ３Ｄモデルに新しい座標をセット
    dx.MV1SetPosition(ModelHandle, Position);

    -- ３Ｄモデルの描画
    dx.MV1DrawModel(ModelHandle);

    -- 裏画面の内容を表画面に反映
    dx.ScreenFlip();
end
