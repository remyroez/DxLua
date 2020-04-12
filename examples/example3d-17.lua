-- 走るアニメーションのループ再生

local ModelHandle
local AnimTotalTime
local AnimNowTime
local AnimAttachIndex

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

	-- 描画先を裏画面にする
	dx.SetDrawScreen(dx.DX_SCREEN_BACK)

	-- カメラの位置と向きを設定
	dx.SetCameraPositionAndTarget_UpVecY(dx.VGet(1340.0, 600.0, -420.0), dx.VGet(0.0, 400.0, -420.0))

	-- カメラのクリッピング距離を設定
	dx.SetCameraNearFar(16.0, 3800.0)

	-- 背景の色を灰色にする
	dx.SetBackgroundColor(128, 128, 128)
end

-- メインループ(何かキーが押されたらループを抜ける)
function dx.Update()
    -- 画面のクリア
    dx.ClearDrawScreen()

    -- アニメーション再生時間を進める
    AnimNowTime = AnimNowTime + 100.0

    -- アニメーション再生時間がアニメーションの総時間を越えていたらループさせる
    if AnimNowTime >= AnimTotalTime then
        -- 新しいアニメーション再生時間は、アニメーション再生時間からアニメーション総時間を引いたもの
        AnimNowTime = AnimNowTime - AnimTotalTime
    end

    -- 新しいアニメーション再生時間をセット
    dx.MV1SetAttachAnimTime(ModelHandle, AnimAttachIndex, AnimNowTime)

    -- ３Ｄモデルの描画
    dx.MV1DrawModel(ModelHandle)

    -- 裏画面の内容を表画面に反映
    dx.ScreenFlip()
end
