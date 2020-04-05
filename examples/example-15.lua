-- パーティクル基本
local band, bnot = bit.band, bit.bnot

-- ショットとパーティクルの最大数
local MAX_SHOT = 4
local MAX_SPARK = 800

-- ショットデータ型
-- DxLua: Lua に型定義は無いため関数でテーブルを返す
function SHOT(t)
	t = t or {}
	return {
		Valid = t.Valid or false, -- このデータが使用中か、フラグ
		X = t.X or 0, -- ショットの位置
		Y = t.Y or 0,
	}
end

-- 火花データ型
function SPARK(t)
	t = t or {}
	return {
		Valid = t.Valid or false, -- このデータが使用中か、フラグ
		X = t.X or 0, -- 火花の位置
		Y = t.Y or 0,
		Sx = t.Sx or 0, -- 火花の移動力
		Sy = t.Sy or 0,
		G = t.G or 0, -- 火花の重さ
		Bright = t.Bright or 0 -- 火花の明るさ
	}
end

PlayerX, PlayerY = PlayerX or 0, PlayerY or 0 -- プレイヤーの位置

-- ショットデータ
if not Shot then
	Shot = {}
	for i = 1, MAX_SHOT do
		table.insert(Shot, SHOT())
	end
end

-- 火花データ
if not Spark then
	Spark = {}
	for i = 1, MAX_SPARK do
		table.insert(Spark, SPARK())
	end
end

local Key = 0
local OldKey = 0 -- 前のキー入力状態

dx.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function dx.Init()
	-- 描画先画面を裏画面にセット
	dx.SetDrawScreen(dx.DX_SCREEN_BACK)

	-- プレイヤーの初期位置をセット
	PlayerX = 320
	PlayerY = 400

	-- ショットの存在を初期化する
	-- 火花の存在を初期化する
	-- DxLua: 初期値を false にしてあるため不要
end

-- ループ
function dx.Update()
	-- キー入力取得
	OldKey = Key
	Key = dx.GetJoypadInputState(dx.DX_INPUT_KEY_PAD1)

	if band(Key, dx.PAD_INPUT_RIGHT) ~= 0 then PlayerX = PlayerX + 3 end -- 右を押していたら右に進む
	if band(Key, dx.PAD_INPUT_LEFT) ~= 0 then  PlayerX = PlayerX - 3 end -- 左を押していたら左に進む

	-- ショットの移動処理
	MoveShot()

	-- 火花の移動処理
	MoveSpark()

	-- ショットボタンを押していたらショットを出す
	if band(band(Key, bnot(OldKey)), dx.PAD_INPUT_A) ~= 0 then CreateShot() end

	-- 画面を初期化する
	dx.ClearDrawScreen()

	-- プレイヤーを描画する
	dx.DrawBox(PlayerX, PlayerY,PlayerX + 48, PlayerY + 48, dx.GetColor(255, 0, 0), true)

	-- ショットを描画する
	for j = 1, MAX_SHOT do
		-- ショットデータが有効な時のみ描画
		if Shot[j].Valid then
			dx.DrawBox(Shot[j].X, Shot[j].Y, Shot[j].X + 16, Shot[j].Y + 16,
				dx.GetColor(255, 255, 255), true)
		end
	end

	-- 火花を描画する
	for j = 1, MAX_SPARK do
		-- 火花データが有効な時のみ描画
		if Spark[j].Valid then
			dx.DrawPixel(Spark[j].X / 100, Spark[j].Y / 100,
				dx.GetColor(Spark[j].Bright, Spark[j].Bright, Spark[j].Bright))
		end
	end

	-- 裏画面の内容を表画面に反映させる
	dx.ScreenFlip()
end

-- 火花を出す処理
function CreateSpark(x, y)
	-- 使われていない火花データを探す
	local Index = MAX_SPARK + 1
	for i = 1, MAX_SPARK do
		if not Spark[i].Valid then
			Index = i
			break
		end
	end

	-- もし使われていない火花データがあったら火花を出す
	if Index <= MAX_SPARK then
		-- 火花の位置を設定
		Spark[Index].X = x * 100
		Spark[Index].Y = y * 100

		-- 移動力を設定
		Spark[Index].Sx = dx.GetRand(1000) - 500
		Spark[Index].Sy = -dx.GetRand(500)

		-- 火花の重さをセット
		Spark[Index].G = dx.GetRand(10)

		-- 火花の明るさセット
		Spark[Index].Bright = 255

		-- 火花データを使用中にセット
		Spark[Index].Valid = 1
	end
end

-- 火花移動処理
function MoveSpark()
	-- 火花の移動処理
	for i = 1, MAX_SPARK do
		if not Spark[i].Valid then
			-- 火花データが無効だったらスキップ
			-- DxLua: Lua に continue は無い
		else
			-- 位置を移動力に応じてずらす
			Spark[i].Y = Spark[i].Y + Spark[i].Sy
			Spark[i].X = Spark[i].X + Spark[i].Sx

			-- 移動力を変更
			Spark[i].Sy = Spark[i].Sy + Spark[i].G

			-- 火花の明るさを下げる
			Spark[i].Bright = Spark[i].Bright - 2

			-- 火花の明るさが０以下になったら火花データを無効にする
			if Spark[i].Bright < 0 then Spark[i].Valid = false end
		end
	end
end

-- ショットを撃つ処理
function CreateShot()
	-- 使われていないショットデータを探す
	local Index = MAX_SHOT + 1
	for i = 1, MAX_SHOT do
		if not Shot[i].Valid then
			Index = i
			break
		end
	end

	-- もし使われていないショットデータがあったらショットを出す
	if Index <= MAX_SHOT then
		-- ショットの位置を設定
		Shot[Index].X = PlayerX + 16
		Shot[Index].Y = PlayerY

		-- ショットデータを使用中にセット
		Shot[Index].Valid = 1
	end
end

-- ショットの移動処理
function MoveShot()
	for i = 1, MAX_SHOT do
		if not Shot[i].Valid then
			-- ショットデータが無効だったらスキップ
		else
			-- 位置を上にずらす
			Shot[i].Y = Shot[i].Y - 8

			-- 画面外に出ていたら火花を出したあとショットデータを無効にする
			if Shot[i].Y < 150 then
				-- 火花を出す数をセット
				local R = dx.GetRand(60)
				for j = 1, R do
					-- 火花を生成
					CreateSpark(Shot[i].X + 8, Shot[i].Y + 8)
				end

				-- ショットデータを無効にする
				Shot[i].Valid = false
			end
		end
	end
end
