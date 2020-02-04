-- マップスクロール基本（滑らか）
local band = bit.band

local MAP_SIZE = 64 -- マップチップ一つのドットサイズ

local MAP_WIDTH = 20 -- マップの幅
local MAP_HEIGHT = 16 -- マップの縦長さ

local MOVE_FRAME = 32 -- 移動にかけるフレーム数

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
DxLua.SetGraphMode(640, 480, 16)

-- プレイヤーの位置
local PlayerX, PlayerY

-- 移動しているかどうかのフラグ( 0:停止中  1:移動中 )
local Move

-- 各方向に移動する量
local MoveX, MoveY

-- 移動し始めてから何フレーム経過したかを保持する変数
local MoveCounter

-- マップとプレイヤーの描画関数
function GraphDraw(ScrollX, ScrollY)
	local j, i
	local MapDrawPointX, MapDrawPointY		-- 描画するマップ座標値
	local DrawMapChipNumX, DrawMapChipNumY	-- 描画するマップチップの数

	-- 描画するマップチップの数をセット
	-- DxLua: 数値は double 型のため小数点以下切り捨て
	DrawMapChipNumX = math.floor(640 / MAP_SIZE) + 2
	DrawMapChipNumY = math.floor(480 / MAP_SIZE) + 2

	-- 画面左上に描画するマップ座標をセット
	MapDrawPointX = PlayerX - math.floor(DrawMapChipNumX / 2 - 1) - 1
	MapDrawPointY = PlayerY - math.floor(DrawMapChipNumY / 2 - 1) - 1

	-- マップを描く
	for i = 0, DrawMapChipNumY do
		for j = 0, DrawMapChipNumX do
			local x, y = j - 1, i - 1 -- DxLua: Lua の配列のインデックスは 1 からのため調整

			-- 画面からはみ出た位置なら描画しない
			if (x + MapDrawPointX < 0 or y + MapDrawPointY < 0 or
				x + MapDrawPointX >= MAP_WIDTH or y + MapDrawPointY >= MAP_HEIGHT) then
				-- DxLua: Lua に continue はない

			elseif MapData[i + MapDrawPointY][j + MapDrawPointX] == 0 then
				-- マップデータが０だったら四角を描画する
				DxLua.DrawBox(x * MAP_SIZE + ScrollX, y * MAP_SIZE + ScrollY,
					x * MAP_SIZE + MAP_SIZE + ScrollX, y * MAP_SIZE + MAP_SIZE + ScrollY,
					DxLua.GetColor(255, 0, 0), true)
			end
		end
	end

	-- プレイヤーの描画
	DxLua.DrawBox((PlayerX - MapDrawPointX - 1) * MAP_SIZE, (PlayerY - MapDrawPointY - 1) * MAP_SIZE,
		(PlayerX - MapDrawPointX) * MAP_SIZE, (PlayerY - MapDrawPointY) * MAP_SIZE,
		DxLua.GetColor(255, 255, 255), true)
end

local Key
local ScrollX, ScrollY

-- ＤＸライブラリ初期化処理
function DxLua.Init()
	-- 描画先画面を裏画面にする
	DxLua.SetDrawScreen(DxLua.DX_SCREEN_BACK)

	-- プレイヤーの初期位置をセット
	-- DxLua: Lua の配列のインデックスは 1 からのため調整
	PlayerX = 3
	PlayerY = 3

	-- 最初は停止中(0)にしておく
	Move = 0
end

-- ループ
function DxLua.Update()
	-- 画面を初期化
	DxLua.ClearDrawScreen()

	-- 移動中ではない場合キー入力を受け付ける
	if Move == 0 then
		-- キー入力を得る
		Key = DxLua.GetJoypadInputState(DxLua.DX_INPUT_KEY_PAD1)

		-- 移動する前のプレイヤーの位置を保存
		OldX = PlayerX
		OldY = PlayerY

		-- キー入力に応じてプレイヤーの座標を移動
		if band(Key, DxLua.PAD_INPUT_LEFT) ~= 0 then
			Move = 1
			MoveX = -1
			MoveY = 0
		end
		if band(Key, DxLua.PAD_INPUT_RIGHT) ~= 0 then
			Move = 1
			MoveX = 1
			MoveY = 0
		end
		if band(Key, DxLua.PAD_INPUT_UP) ~= 0 then
			Move = 1
			MoveX = 0
			MoveY = -1
		end
		if band(Key, DxLua.PAD_INPUT_DOWN) ~= 0 then
			Move = 1
			MoveX = 0
			MoveY = 1
		end

		-- 進入不可能なマップだった場合は移動できない
		if Move == 1 then
			if MapData[PlayerY + MoveY][PlayerX + MoveX] == 0 then
				Move = 0
			else
				MoveCounter = 0
			end
		end

		-- 停止中は画面のスクロールは行わない
		ScrollX = 0
		ScrollY = 0
	end

	-- 移動中の場合は移動処理を行う
	if Move == 1 then
		MoveCounter = MoveCounter + 1

		-- 移動処理が終了したら停止中にする
		if MoveCounter == MOVE_FRAME then
			Move = 0

			-- プレイヤーの位置を変更する
			PlayerX = PlayerX + MoveX
			PlayerY = PlayerY + MoveY

			-- 停止中は画面のスクロールは行わない
			ScrollX = 0
			ScrollY = 0
		else
			-- 経過時間からスクロール量を算出する
			ScrollX = -math.floor( MoveX * MAP_SIZE * MoveCounter / MOVE_FRAME )
			ScrollY = -math.floor( MoveY * MAP_SIZE * MoveCounter / MOVE_FRAME )
		end
	end

	-- マップとプレイヤーを描画
	GraphDraw(ScrollX, ScrollY)

	-- 裏画面の内容を表画面に映す
	DxLua.ScreenFlip()
end
