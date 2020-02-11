-- 移動速度を一定にする(２)

local Time = Time or 0

local x = x or 0

DxLua.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function DxLua.Init()
	-- 描画先画面を裏画面にセット
	DxLua.SetDrawScreen(DxLua.DX_SCREEN_BACK)

	-- 四角の描画位置をセット
	x = 0
end

-- ループ
function DxLua.Update()
	-- DxLua: キー入力で終了
	if DxLua.CheckHitKeyAll() ~= 0 then return 'exit' end

	-- 現在のカウントを取得する
	Time = DxLua.GetNowCount()

	-- 画面を消去
	DxLua.ClearDrawScreen()

	-- 四角の描画位置を移動
	x = x + 8
	if x > 640 then x = 0 end

	-- 四角を描画
	DxLua.DrawBox(x, 0, x + 32, 32, DxLua.GetColor(255, 255, 255), true)

	-- 裏画面の内容を表画面に反映させる
	DxLua.ScreenFlip()

	-- １７ミリ秒(約秒間６０フレームだった時の１フレームあたりの経過時間)
	-- 経過するまでここで待つ
	while DxLua.GetNowCount() - Time < 17 do end
end
