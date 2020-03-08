-- 残像処理基本
local band = bit.band

-- 残像データの数
local AFTIMAGENUM = 65

PlayerX, PlayerY = PlayerX or {}, PlayerY or {}

Key = Key or 0

-- 画面モードのセット
DxLua.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function DxLua.Init()
	-- 描画先画面を裏画面にセット
	DxLua.SetDrawScreen(DxLua.DX_SCREEN_BACK)

	-- キャラクターの初期位置をセット
	for i = 1, AFTIMAGENUM do
		PlayerX[i] = 0
		PlayerY[i] = 0
	end
end

-- ループ
function DxLua.Update()
	-- キー入力取得
	Key = DxLua.GetJoypadInputState(DxLua.DX_INPUT_KEY_PAD1)

	-- 上を押していたら上に進む
	if band(Key, DxLua.PAD_INPUT_UP) ~= 0 then PlayerY[1] = PlayerY[1] - 3 end

	-- 下を押していたら下に進む
	if band(Key, DxLua.PAD_INPUT_DOWN) ~= 0 then PlayerY[1] = PlayerY[1] + 3 end

	-- 右を押していたら右に進む
	if band(Key, DxLua.PAD_INPUT_RIGHT) ~= 0 then PlayerX[1] = PlayerX[1] + 3 end

	-- 左を押していたら左に進む
	if band(Key, DxLua.PAD_INPUT_LEFT) ~= 0 then PlayerX[1] = PlayerX[1] - 3 end

	-- 画面を初期化する
	DxLua.ClearDrawScreen()

	-- プレイヤーを描画する
	for i = AFTIMAGENUM, 1, -1 do
		DxLua.DrawBox(PlayerX[i], PlayerY[i], PlayerX[i] + 32, PlayerY[i] + 32,
			DxLua.GetColor(0, 255 - 255 * i / AFTIMAGENUM, 0), true)
	end

	-- 裏画面の内容を表画面に反映させる
	DxLua.ScreenFlip()

	-- 残像データを一つづつずらす
	for i = AFTIMAGENUM, 2, -1 do
		PlayerX[i] = PlayerX[i - 1]
		PlayerY[i] = PlayerY[i - 1]
	end
end
