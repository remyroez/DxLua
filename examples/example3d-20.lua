-- 走るアニメーションのフレーム( ボーン )「BasePoint」の移動値を使用してキャラクターモデルの座標移動を行うプログラム

local ModelHandle
local AnimTotalTime
local AnimNowTime
local AnimAttachIndex
local MoveAnimFrameIndex
local Position
local PrevPosition
local NowPosition

-- ＤＸライブラリの初期化
function dx.Init()
    -- ３Ｄモデルの読み込み
    ModelHandle = dx.MV1LoadModel("DxChara.x")

    -- 走りアニメーションをアタッチ
    AnimAttachIndex = dx.MV1AttachAnim(ModelHandle, 1)

    -- 走りアニメーションの総時間を取得しておく
    AnimTotalTime = dx.MV1GetAttachAnimTotalTime(ModelHandle, AnimAttachIndex)

    -- アニメーション再生時間を初期化
    AnimNowTime = 0.0
    dx.MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime)

    -- アニメーションで移動をしているフレームの番号を検索する
    MoveAnimFrameIndex = dx.MV1SearchFrame(ModelHandle, "BasePoint")

    -- アニメーションで移動をしているフレームを無効にする
    dx.MV1SetFrameUserLocalMatrix(ModelHandle, MoveAnimFrameIndex, dx.MGetIdent())

    -- ３Ｄモデルの座標を初期化
    Position = dx.VGet(0.0, 0.0, 0.0)

    -- 描画先を裏画面にする
    dx.SetDrawScreen(dx.DX_SCREEN_BACK)

    -- カメラの位置と向きを設定
    dx.SetCameraPositionAndTarget_UpVecY(dx.VGet(2400.0, 1000.0, -1650.0), dx.VGet(0.0, 280.0, -1650.0))

    -- カメラのクリッピング距離を設定
    dx.SetCameraNearFar(16.0, 3800.0)

    -- 背景の色を灰色にする
    dx.SetBackgroundColor(128, 128, 128)
end

-- メインループ(何かキーが押されたらループを抜ける)
function dx.Update()
    -- 画面のクリア
    dx.ClearDrawScreen()

    -- アニメーション時間を進める前の『アニメーションで移動をしているフレームの座標』を取得しておく
    PrevPosition = dx.MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex)

    -- アニメーション再生時間を進める
    AnimNowTime = AnimNowTime + 100.0

    -- アニメーション再生時間がアニメーションの総時間を越えているかどうかで処理を分岐
    if (AnimNowTime >= AnimTotalTime) then
        -- 超えている場合は、まず『アニメーション再生時間を進める前の「アニメーションで移動しているフレームの座標」』と、
        -- 『アニメーションの終端での「アニメーションで移動しているフレームの座標」』との差分をモデルの座標に加算する
        dx.MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimTotalTime)
        NowPosition = dx.MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex)
        Position = dx.VAdd(Position, dx.VSub(NowPosition, PrevPosition))

        -- 新しいアニメーション再生時間は、アニメーション再生時間からアニメーション総時間を引いたもの
        AnimNowTime = AnimNowTime - AnimTotalTime

        -- 次に『新しいアニメーション再生時間での「アニメーションで移動をしているフレームの座標」と
        -- 『アニメーション再生時間０での「アニメーションで移動しているフレームの座標」』との差分をモデルの座標に加算する
        dx.MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, 0.0)
        PrevPosition = dx.MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex)
        dx.MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime)
        NowPosition = dx.MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex)
        Position = dx.VAdd(Position, dx.VSub(NowPosition, PrevPosition))
    else
        -- 新しいアニメーション再生時間をセット
        dx.MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime)

        -- 『新しいアニメーション再生時間での「アニメーションで移動をしているフレームの座標」』と、
        -- 『アニメーション再生時間を進める前の「アニメーションで移動をしているフレームの座標」』との差分をモデルの座標に加算する
        NowPosition = dx.MV1GetAttachAnimFrameLocalPosition(ModelHandle, AnimAttachIndex, MoveAnimFrameIndex)
        Position = dx.VAdd(Position, dx.VSub(NowPosition, PrevPosition))
    end

    -- ３Ｄモデルに新しい座標をセット
    dx.MV1SetPosition(ModelHandle, Position)

    -- ３Ｄモデルの描画
    dx.MV1DrawModel(ModelHandle)

    -- 裏画面の内容を表画面に反映
    dx.ScreenFlip()
end
