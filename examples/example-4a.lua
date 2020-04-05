-- マップスクロール基本
local band = bit.band

local MAP_SIZE = 64 -- マップチップ一つのドットサイズ

local MAP_WIDTH = 20 -- マップの幅
local MAP_HEIGHT = 16 -- マップの縦長さ

-- マップのデータ
local MapData =
{
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,  1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 0, 1, 1, 1, 0, 0, 0, 0, 0,  1, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 0, 1, 0, 1, 0, 0, 0, 0, 0,  1, 1, 1, 1, 1, 1, 0, 0, 1, 0 },
	{ 0, 1, 1, 0, 1, 0, 0, 1, 1, 0,  0, 0, 0, 0, 0, 1, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 1, 0, 0, 1, 1, 0,  0, 0, 1, 1, 1, 1, 1, 0, 1, 0 },
	{ 0, 1, 0, 0, 1, 0, 0, 1, 1, 0,  0, 0, 1, 0, 0, 1, 1, 0, 1, 0 },
	{ 0, 1, 0, 0, 1, 0, 0, 1, 1, 0,  0, 0, 1, 0, 0, 1, 1, 0, 1, 0 },

	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 0,  0, 1, 1, 0, 0, 1, 1, 0, 1, 0 },
	{ 0, 0, 0, 0, 1, 0, 0, 1, 0, 0,  0, 1, 1, 0, 0, 1, 1, 0, 1, 0 },
	{ 0, 0, 0, 0, 1, 0, 0, 1, 0, 0,  0, 1, 1, 0, 0, 1, 1, 0, 1, 0 },
	{ 0, 0, 0, 1, 1, 0, 0, 1, 0, 0,  0, 1, 1, 0, 0, 1, 1, 0, 1, 0 },
	{ 0, 1, 1, 1, 1, 0, 0, 1, 1, 1,  1, 1, 1, 0, 0, 1, 1, 0, 1, 0 },
	{ 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
}

-- 画面モードのセット
dx.SetGraphMode(640, 480, 16)

-- プレイヤーの位置
local PlayerX, PlayerY

-- マップとプレイヤーの描画関数
function GraphDraw()
	local j, i
	local MapDrawPointX, MapDrawPointY		-- 描画するマップ座標値
	local DrawMapChipNumX, DrawMapChipNumY	-- 描画するマップチップの数

	-- 描画するマップチップの数をセット
	-- DxLua: 数値は double 型のため小数点以下切り捨て
	DrawMapChipNumX = math.floor(640 / MAP_SIZE) + 1
	DrawMapChipNumY = math.floor(480 / MAP_SIZE) + 1

	-- 画面左上に描画するマップ座標をセット
	MapDrawPointX = PlayerX - math.floor(DrawMapChipNumX / 2) - 1
	MapDrawPointY = PlayerY - math.floor(DrawMapChipNumY / 2) - 1

	-- マップを描く
	for i = 1, DrawMapChipNumY do
		for j = 1, DrawMapChipNumX do
			local x, y = j - 1, i - 1 -- DxLua: Lua の配列のインデックスは 1 からのため調整

			-- 画面からはみ出た位置なら描画しない
			if (x + MapDrawPointX < 0 or y + MapDrawPointY < 0 or
				x + MapDrawPointX >= MAP_WIDTH or y + MapDrawPointY >= MAP_HEIGHT) then
				-- DxLua: Lua に continue はない

			elseif MapData[i + MapDrawPointY][j + MapDrawPointX] == 0 then
				-- マップデータが０だったら四角を描画する
				dx.DrawBox(x * MAP_SIZE, y * MAP_SIZE,
					x * MAP_SIZE + MAP_SIZE, y * MAP_SIZE + MAP_SIZE,
					dx.GetColor(255, 0, 0), true)
			end
		end
	end

	-- プレイヤーの描画
	dx.DrawBox((PlayerX - MapDrawPointX - 1) * MAP_SIZE, (PlayerY - MapDrawPointY - 1) * MAP_SIZE,
		(PlayerX - MapDrawPointX) * MAP_SIZE, (PlayerY - MapDrawPointY) * MAP_SIZE,
		dx.GetColor(255, 255, 255), true)
end

local Key
local OldX, OldY -- 移動する前のプレイヤーの位置を保存する変数

-- ＤＸライブラリ初期化処理
function dx.Init()
	-- 描画先画面を裏画面にする
	dx.SetDrawScreen(dx.DX_SCREEN_BACK)

	-- プレイヤーの初期位置をセット
	-- DxLua: Lua の配列のインデックスは 1 からのため調整
	PlayerX = 3
	PlayerY = 3
end

-- ループ
function dx.Update()
	-- 画面を初期化
	dx.ClearDrawScreen()

	-- キー入力を得る
	Key = dx.GetJoypadInputState(dx.DX_INPUT_KEY_PAD1)

	-- 移動する前のプレイヤーの位置を保存
	OldX = PlayerX
	OldY = PlayerY

	-- キー入力に応じてプレイヤーの座標を移動
	if band(Key, dx.PAD_INPUT_LEFT) ~= 0 then PlayerX = PlayerX - 1 end
	if band(Key, dx.PAD_INPUT_RIGHT) ~= 0 then PlayerX = PlayerX + 1 end
	if band(Key, dx.PAD_INPUT_UP) ~= 0 then PlayerY = PlayerY - 1 end
	if band(Key, dx.PAD_INPUT_DOWN) ~= 0 then PlayerY = PlayerY + 1 end

	-- 進入不可能なマップだった場合は移動できない
	if MapData[PlayerY][PlayerX] == 0 then
		PlayerX = OldX
		PlayerY = OldY
	end

	-- マップとプレイヤーを描画
	GraphDraw()

	-- 裏画面の内容を表画面に映す
	dx.ScreenFlip()

	-- ウエイト
	dx.WaitTimer(100)
end
