-- キー入力基本
local band = bit.band

local PlayerX, PlayerY
local PlayerGraph
local Key

-- 画面モードのセット
dx.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function dx.Init()
	-- 描画先画面を裏画面にセット
	dx.SetDrawScreen(dx.DX_SCREEN_BACK)

	-- グラフィックのロード
	PlayerGraph = dx.LoadGraph("Player.bmp")

	-- キャラクターの初期位置をセット
	PlayerX = 0
    PlayerY = 0
end

-- ループ
function dx.Update()
    -- キー入力取得
    Key = dx.GetJoypadInputState(dx.DX_INPUT_KEY_PAD1)

    -- 上を押していたら上に進む
    if band(Key, dx.PAD_INPUT_UP) ~= 0 then PlayerY = PlayerY - 3 end

    -- 下を押していたら下に進む
    if band(Key, dx.PAD_INPUT_DOWN) ~= 0 then PlayerY = PlayerY + 3 end

    -- 右を押していたら右に進む
    if band(Key, dx.PAD_INPUT_RIGHT) ~= 0 then PlayerX = PlayerX + 3 end

    -- 左を押していたら左に進む
    if band(Key, dx.PAD_INPUT_LEFT) ~= 0 then PlayerX = PlayerX - 3 end

    -- 画面を初期化する
    dx.ClearDrawScreen()

    -- プレイヤーを描画する
    dx.DrawGraph(PlayerX, PlayerY, PlayerGraph, true)

    -- 裏画面の内容を表画面に反映させる
    dx.ScreenFlip()
end
