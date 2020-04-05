-- 移動速度を一定にする(２)

Time = Time or 0

x = x or 0

dx.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function dx.Init()
	-- 描画先画面を裏画面にセット
	dx.SetDrawScreen(dx.DX_SCREEN_BACK)

	-- 四角の描画位置をセット
	x = 0
end

-- ループ
function dx.Update()
	-- DxLua: キー入力で終了
	if dx.CheckHitKeyAll() ~= 0 then return 'exit' end

	-- 現在のカウントを取得する
	Time = dx.GetNowCount()

	-- 画面を消去
	dx.ClearDrawScreen()

	-- 四角の描画位置を移動
	x = x + 8
	if x > 640 then x = 0 end

	-- 四角を描画
	dx.DrawBox(x, 0, x + 32, 32, dx.GetColor(255, 255, 255), true)

	-- 裏画面の内容を表画面に反映させる
	dx.ScreenFlip()

	-- １７ミリ秒(約秒間６０フレームだった時の１フレームあたりの経過時間)
	-- 経過するまでここで待つ
	while dx.GetNowCount() - Time < 17 do end
end
