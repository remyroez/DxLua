-- 移動速度を一定にする(１)

ScreenFlipCount, StartTime, FrameTime = ScreenFlipCount or 0, StartTime or 0, FrameTime or 0
x = x or 0

DxLua.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function DxLua.Init()
	-- 描画先画面を裏画面にセット
	DxLua.SetDrawScreen(DxLua.DX_SCREEN_BACK)

	-- 計測中に別のウインドウがアクティブになっても問題が無いように常時実行フラグをセット
	DxLua.SetAlwaysRunFlag(true)

	-- １フレームにかかる時間を計測
	DxLua.ScreenFlip()
	ScreenFlipCount = 0
	StartTime = DxLua.GetNowCount()
	while true do
		-- 画面切り替えを行ってＶＹＳＮＣ待ちをする
		DxLua.ScreenFlip()

		-- １秒経過していたらループから抜ける
		if DxLua.GetNowCount() - StartTime >= 1000 then
			break
		end

		-- ScreenFlip を行った回数をインクリメント
		ScreenFlipCount = ScreenFlipCount + 1
	end

	-- 常時実行フラグを元に戻す
	DxLua.SetAlwaysRunFlag(false)

	-- 計測時間を ScreenFlip を行った回数で割れば
	-- ScreenFlip 一回辺りの時間が取得できます
	FrameTime = 1000 / ScreenFlipCount

	-- 四角の描画位置をセット
	x = 0
end

-- ループ
function DxLua.Update()
	-- 画面を消去
	DxLua.ClearDrawScreen()

	-- 四角の描画位置を移動(１秒間で画面を横切る速さに設定
	x = x + 6400 * FrameTime / 1000
	if x > 640 * 10 then x = 0 end

	-- 四角を描画
	DxLua.DrawBox(x / 10, 0, x / 10 + 32, 32, DxLua.GetColor(255, 255, 255), true)

	-- 裏画面の内容を表画面に反映させる
	DxLua.ScreenFlip()

	if DxLua.CheckHitKeyAll() ~= 0 then return 'exit' end
end
