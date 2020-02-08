-- シューティング基本
local band, bnot = bit.band, bit.bnot

-- ショットの最大数
local MAX_SHOT = 4

local PlayerX, PlayerY -- プレイヤーの位置

local ShotValid = {} -- ショットが存在するか、フラグ
local ShotX, ShotY = {}, {} -- ショットの位置

-- 画面モードのセット
DxLua.SetGraphMode(640, 480, 16)

local Key = 0
local OldKey -- 前のキー入力状態

-- ＤＸライブラリ初期化処理
function DxLua.Init()
	-- 描画先画面を裏画面にセット
	DxLua.SetDrawScreen(DxLua.DX_SCREEN_BACK)

	-- プレイヤーの初期位置をセット
	PlayerX = 320
	PlayerY = 400

	-- ショットの存在を初期化する
	for i = 1, MAX_SHOT do
		ShotValid[i] = 0
	end
end

-- ループ
function DxLua.Update()
	-- キー入力取得
	OldKey = Key
	Key = DxLua.GetJoypadInputState(DxLua.DX_INPUT_KEY_PAD1)

	if band(Key, DxLua.PAD_INPUT_RIGHT) ~= 0 then PlayerX = PlayerX + 3 end -- 右を押していたら右に進む
	if band(Key, DxLua.PAD_INPUT_LEFT) ~= 0 then PlayerX = PlayerX - 3 end -- 左を押していたら左に進む

	-- ショットの移動処理
	for j = 1, MAX_SHOT do
		-- ショットデータが無効だったらスキップ
		if (ShotValid[j] == 0) then
			-- DxLua: Lua に continue はない
		else
			-- 位置を上にずらす
			ShotY[j] = ShotY[j] - 8

			-- 画面外に出ていたらショットデータを無効にする
			if (ShotY[j] < -32) then ShotValid[j] = 0 end
		end
	end

	-- ショットボタンを押していたらショットを出す
	-- 一つ前のループでショットボタンを押していたらショットは出さない
	if band(band(Key, bnot(OldKey)), DxLua.PAD_INPUT_A) ~= 0 then
		-- 使われていないショットデータを探す
		local k = -1
		for j = 1, MAX_SHOT do
			if ShotValid[j] == 0 then
				k = j
				break
			end
		end

		-- もし使われていないショットデータがあったらショットを出す
		if k >= 0 then
			-- ショットの位置を設定
			ShotX[k] = PlayerX + 16
			ShotY[k] = PlayerY

			-- ショットデータを使用中にセット
			ShotValid[k] = 1
		end
	end

	-- 画面を初期化する
	DxLua.ClearDrawScreen()

	-- プレイヤーを描画する
	DxLua.DrawBox(PlayerX, PlayerY, PlayerX + 48, PlayerY + 48, DxLua.GetColor(255, 0, 0), true)

	-- ショットを描画する
	for j = 1, MAX_SHOT do
		-- ショットデータが有効な時のみ描画
		if ShotValid[j] == 1 then
			DxLua.DrawBox(ShotX[j], ShotY[j], ShotX[j] + 16, ShotY[j] + 16,
				DxLua.GetColor(255, 255, 255), true)
		end
	end

	-- 裏画面の内容を表画面に反映させる
	DxLua.ScreenFlip()
end
