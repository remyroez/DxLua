-- コラムス

-- DxLua: LuaJIT(Lua 5.1) にビット演算子は無いため bit ライブラリを利用する
local band, bnot = bit.band, bit.bnot

local BLOCKTYPE_NUM	= 5	-- ブロックの種類の数

local BLOCK_SIZE = 24	-- ブロックのドットサイズ
local WORLD_WIDTH = 8	-- ステージの幅
local WORLD_HEIGHT = 16	-- ステージの高さ

local STAGE_X = 210	-- ステージの左上頂点のＸ座標
local STAGE_Y = 50	-- ステージの左上頂点のＹ座標

local WAIT = 500	-- 黙っててブロックが落ちるまでの時間

-- ステージデータ
local Block = { {} } -- 実際のデータ
local BufferBlock = { {} }	-- 一時状態保存用ブロックデータ

-- アクティブブロックデータ
local ActiveX, ActiveY	-- アクティブブロックの一番下のブロックの位置
local ActiveBlock = {}	-- アクティブなブロックのデータ

-- カウンター
local WaitCounter = 0 -- 下に勝手に落とすまでの時間計測用
local NowTime = 0-- 現在のフレームで経過した時間
local OldTime = 0-- 前のフレームのときにGetNowCount関数で得たタイム

-- ブロックの種類ごとの色データ
local BlockColor =
{
	{ 128, 128, 128 }, { 255, 100, 100 }, { 255, 255, 0 },
	{ 255, 0, 255 }, { 0, 255, 255 }
}

-- 画面モードのセット
DxLua.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function DxLua.Init()
	-- ゲームを初期化
	InitGame()

	-- 描画先画面を裏画面にセット
	DxLua.SetDrawScreen(DxLua.DX_SCREEN_BACK)
end

-- ループ
function DxLua.Update()
	-- キー入力処理
	KeyInput()

	-- 時間経過処理
	TimeFunc()

	-- 画面描画
	ScreenDraw()

	-- 裏画面の内容を表画面に反映させる
	DxLua.ScreenFlip()
end


-- ゲームの初期化
function InitGame()
	-- 前回時間をセット
	OldTime = DxLua.GetNowCount()

	-- アクティブブロックの位置をセット
	ActiveX = math.floor(WORLD_WIDTH / 2)
	ActiveY = 2

	-- アクティブブロックを生成
	CreateNewActiveBlock()

	-- マップブロックの初期化
	for i = 1, WORLD_HEIGHT do
		for j = 1, WORLD_WIDTH do
			if Block[j] == nil then Block[j] = {} end
			Block[j][i] = 0
		end
	end
end

-- 新しいブロックの生成
function CreateNewActiveBlock()
	-- ランダムに３つブロックをセット
	for i = 1, 3 do
		ActiveBlock[i] = DxLua.GetRand(BLOCKTYPE_NUM - 1) + 1
	end
end

local OldKey = 0 -- 前フレームで取得したキー情報

-- キー入力処理
function KeyInput()
	local Key	-- 入力されたキー

	-- キー入力を得る
	Key = DxLua.GetJoypadInputState(DxLua.DX_INPUT_KEY_PAD1)

	-- キー入力に応じて処理をする
	if band(Key, DxLua.PAD_INPUT_DOWN) ~= 0 then MoveActiveBlock(0, 1) end
	if band(band(Key, bnot(OldKey)), DxLua.PAD_INPUT_LEFT) ~= 0 and ActiveX > 1 then MoveActiveBlock(-1, 0) end
	if band(band(Key, bnot(OldKey)), DxLua.PAD_INPUT_RIGHT) ~= 0 and ActiveX < WORLD_WIDTH then MoveActiveBlock(1, 0) end

	if band(band(Key, bnot(OldKey)), DxLua.PAD_INPUT_A) ~= 0 then
		local TempBlock

		-- アクティブブロックの配置を変更する
		TempBlock = ActiveBlock[3]
		ActiveBlock[3] = ActiveBlock[1]
		ActiveBlock[1] = TempBlock

		TempBlock = ActiveBlock[1]
		ActiveBlock[1] = ActiveBlock[2]
		ActiveBlock[2] = TempBlock
	end
	OldKey = Key

	-- 終了
	return 0
end

-- 時間関係処理
function TimeFunc()
	local Time

	-- 現在の時間を得る
	Time = DxLua.GetNowCount()

	-- 今フレームで経過した時間を得る
	NowTime = Time - OldTime

	-- 現在の時間を保存
	OldTime = Time

	-- ウエイトカウンタに経過時間を加算する
	WaitCounter = WaitCounter + NowTime

	-- 一定時間が経過していたら勝手に下にブロックを落す
	if WaitCounter > WAIT then
		MoveActiveBlock(0, 1)

		-- カウンターを０に戻す
		WaitCounter = 0
	end

	-- 終了
	return 0
end

-- アクティブブロックの移動
function MoveActiveBlock(MoveX, MoveY)
	local NewX, NewY

	-- 移動後の座標をセットする
	NewX = MoveX + ActiveX
	NewY = MoveY + ActiveY

	-- 左右移動の処理
	if MoveX ~= 0 and NewX >= 1 and NewX <= WORLD_WIDTH then
		-- 各３つのブロックが画面上のブロックに当たっていないか調べる
		if CheckHitActiveBlock(NewX, NewY) == -1 then
			-- あたっていたら左右移動を無効にする
			MoveX = 0
		end
	end

	-- 上下移動の処理
	if MoveY ~= 0 then
		-- 画面の一番下のブロック位置まで来ていたらブロックを固定させる
		if NewY >= WORLD_HEIGHT + 1 then
			LockActiveBlock(ActiveX, ActiveY)

			-- 移動を無効にする
			MoveY = 0

		elseif CheckHitActiveBlock(NewX, NewY) == - 1 then
			-- 各３つのブロックが画面上のブロックに当たっていないか調べる
			-- あたっていたらブロックを固定する
			LockActiveBlock(ActiveX, ActiveY)

			-- 移動を無効にする
			MoveY = 0
		end
	end

	-- 座標を移動する
	ActiveX = ActiveX + MoveX
	ActiveY = ActiveY + MoveY

	-- 終了
	return 0
end

-- アクティブブロックが画面上のブロックに当たっていないか調べる
function CheckHitActiveBlock(x, y)
	-- ３つあるブロックがそれぞれ画面上のブロックに当たっていないか調べる
	for i = 1, 3 do
		if (y - i < 1) then
			-- DxLua: Lua に continue はない
		elseif Block[x][y - i] ~= 0 then
			-- 当たっていたらここで終了
			return -1
		end
	end

	-- 当たっていない場合だけ０を返す
	return 0
end

-- アクティブブロックを固定する
-- 及び次のブロックを出す
-- もし次のブロックを出すスペースがなかったらゲームオーバー
function LockActiveBlock(x, y)
	for i = 1, 3 do
		-- 画面上から外れている場合はセットしない
		if y - i < 1 then
			-- DxLua: Lua に continue はない
		else
			-- ブロックの固定
			Block[x][y - i] = ActiveBlock[i]
		end
	end

	-- 消せるブロックがある場合は消す
	CheckEliminatBlock()

	-- 新しいブロックをセット
	do
		-- アクティブブロックの位置をセット
		ActiveX = math.floor(WORLD_WIDTH / 2)
		ActiveY = 3

		-- 新しいアクティブブロックを生成
		CreateNewActiveBlock()

		-- 次に出そうとしているスペースがなかったらゲームオーバー処理
		for i = 1, 3 do
			if Block[ActiveX][ActiveY - i + 1] ~= 0 then GameOver() end
		end
	end

	-- カウンタリセット
	WaitCounter = 0

	-- 時間待ち
	DxLua.WaitTimer(200)

	-- 終了
	return 0
end

-- ゲームオーバー処理
function GameOver()
	-- 画面中心にゲームオーバーを表示
	ScreenDraw()
	DxLua.DrawString(300, 220, "GameOver", DxLua.GetColor(255, 255, 255))
	DxLua.ScreenFlip()

	-- キー入力待ち
	DxLua.WaitKey()

	-- ＤＸライブラリ終了
	--DxLua.DxLib_End()

	-- ソフト終了
	--exit(-1)
end

-- 消えるブロックがあるか調べてあったら消す処理をする
function CheckEliminatBlock()
	local ClsFlag = false -- 一つでも消せたブロックがあったか、のフラグ

	repeat
		-- 一時使用用ブロックデータを初期化
		InitBufferBlock()

		-- 消せたブロックがあるか、フラグを倒す
		ClsFlag = false

		-- 各ブロックが消えるか調べる
		for i = 1, WORLD_HEIGHT do
			for j = 1, WORLD_WIDTH do
				-- もしブロックがない場合は次に移る
				if Block[j][i] == 0 then
					--
				else
					-- ブロックが消えるかどうか調べて調査結果をバッファに保存する
					BufferBlock[j][i] = CheckEliminatBlockToOne(j, i)
				end
			end
		end

		-- 消えると判断されたブロックを消す
		for i = 1, WORLD_HEIGHT do
			for j = 1, WORLD_WIDTH do
				if BufferBlock[j][i] == 1 then
					ClsFlag = true
					Block[j][i] = 0
				end
			end
		end

		-- 空きを積める
		for i = WORLD_HEIGHT - 1, 0, -1 do
			for j = 1, WORLD_WIDTH do
				if Block[j][i] ~= 0 then
					local index = 1
					for k = i + 1, WORLD_HEIGHT do
						index = k
						if Block[ j ][ k ] then
							break
						end
					end
					index = index - 1

					if index ~= i then
						Block[j][index] = Block[j][i]
						Block[j][i] = 0
					end
				end
			end
		end
	until not ClsFlag	-- 消せたブロックがあった場合再度チェック

	-- 終了
	return 0
end

-- 一時使用用ブロックデータの初期化
function InitBufferBlock()
	for i = 1, WORLD_HEIGHT do
		for j = 1, WORLD_WIDTH do
			if not BufferBlock[j] then BufferBlock[j] = {} end
			BufferBlock[j][i] = 0
		end
	end
end

-- 特定ブロックが消えるか探索
function CheckEliminatBlockToOne(x, y)
	--[[
	local CheckBlock
	local BlockNum

	-- チェックするブロックの種類を保存
	CheckBlock = Block[x][y]

	-- 左右にどれだけつながっているか調べる
	for(i = 0 x + i >= 0 and Block[ x + i ][ y ] == CheckBlock i --){}
	i ++
	for(BlockNum = 0 x + i < WORLD_WIDTH and Block[ x + i ][ y ] == CheckBlock BlockNum ++, i ++){}

	-- ３つ以上つながっていたらここで終了
	if(BlockNum >= 3) return 1


	-- 上下にどれだけつながっているか調べる
	for(i = 0 y + i >= 0 and Block[ x ][ y + i ] == CheckBlock i --){}
	i ++
	for(BlockNum = 0 y + i < WORLD_HEIGHT and Block[ x ][ y + i ] == CheckBlock BlockNum ++, i ++){}

	-- ３つ以上つながっていたらここで終了
	if(BlockNum >= 3) return 1


	-- 左上から右下にかけて繋がっている数を調べる
	for(i = 0 y + i >= 0 and x + i >= 0 and Block[ x + i ][ y + i ] == CheckBlock i --){}
	i ++
	for(BlockNum = 0 x + i < WORLD_WIDTH and y + i < WORLD_HEIGHT and Block[ x + i ][ y + i ] == CheckBlock BlockNum ++, i ++){}

	-- ３つ以上つながっていたらここで終了
	if(BlockNum >= 3) return 1


	-- 右上から左下にかけて繋がっている数を調べる
	for(i = 0 y + i >= 0 and  x - i < WORLD_WIDTH and Block[ x - i ][ y + i ] == CheckBlock i --){}
	i ++
	for(BlockNum = 0 x - i >= 0 and y + i < WORLD_HEIGHT and Block[ x - i ][ y + i ] == CheckBlock BlockNum ++, i ++){}

	-- ３つ以上つながっていたらここで終了
	if(BlockNum >= 3) return 1

	--]]
	-- ここまで来ていたら消えない
	return 0
end

-- 画面描画処理関数
function ScreenDraw()
	-- 画面を初期化
	DxLua.ClearDrawScreen()

	-- 枠を描画
	DxLua.DrawBox(STAGE_X - 24, STAGE_Y, STAGE_X,
		STAGE_Y + WORLD_HEIGHT * BLOCK_SIZE,
		DxLua.GetColor(255, 0, 0), true)

	DxLua.DrawBox(STAGE_X + WORLD_WIDTH * BLOCK_SIZE, STAGE_Y,
		STAGE_X + 24 + WORLD_WIDTH * BLOCK_SIZE, STAGE_Y + WORLD_HEIGHT * BLOCK_SIZE,
		DxLua.GetColor(255, 0, 0), true)

	DxLua.DrawBox(STAGE_X - 24, STAGE_Y + WORLD_HEIGHT * BLOCK_SIZE,
		STAGE_X + 24 + WORLD_WIDTH * BLOCK_SIZE, STAGE_Y + WORLD_HEIGHT * BLOCK_SIZE + 24,
		DxLua.GetColor(255, 0, 0), true)

	-- ブロックを描画
	for i = 1, WORLD_HEIGHT do
		for j = 1, WORLD_WIDTH do
			if Block[j][i] ~= 0 then
				local index = Block[j][i]
				local jx, iy = j - 1, i
				DxLua.DrawBox(STAGE_X + jx * BLOCK_SIZE, STAGE_Y + BLOCK_SIZE * iy,
					STAGE_X + jx * BLOCK_SIZE + BLOCK_SIZE, STAGE_Y + iy * BLOCK_SIZE + BLOCK_SIZE,
					DxLua.GetColor(BlockColor[index][1],BlockColor[index][2],BlockColor[index][3]), true)
			end
		end
	end

	-- アクティブブロックを描画
	for i = 1, 3 do
		if i + ActiveY < 1 then
			break
		end
		local index = ActiveBlock[i]
		local ax, ay = ActiveX - 1, ActiveY
		DxLua.DrawBox(STAGE_X + ax * BLOCK_SIZE, STAGE_Y + (ay - i) * BLOCK_SIZE,
				STAGE_X + ax * BLOCK_SIZE + BLOCK_SIZE, STAGE_Y + (ay - i) * BLOCK_SIZE + BLOCK_SIZE,
				DxLua.GetColor(BlockColor[index][1],BlockColor[index][2],BlockColor[index][3]), true)
	end

	-- 終了
	return 0
end
