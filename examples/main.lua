-- キー入力基本
local band = bit.band

local PlayerX, PlayerY
local PlayerGraph
local Key

-- 画面モードのセット
DxLua.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function DxLua.Init(...)
    print(...)

	-- 描画先画面を裏画面にセット
	DxLua.SetDrawScreen(DxLua.DX_SCREEN_BACK)

	-- グラフィックのロード
	PlayerGraph = DxLua.LoadGraph("Player.bmp")

	-- キャラクターの初期位置をセット
	PlayerX = 0
    PlayerY = 0
end

-- ループ
function DxLua.Update(DeltaTime)
    -- キー入力取得
    Key = DxLua.GetJoypadInputState(DxLua.DX_INPUT_KEY_PAD1)

    -- 上を押していたら上に進む
    if band(Key, DxLua.PAD_INPUT_UP) ~= 0 then PlayerY = PlayerY - 100 * DeltaTime end

    -- 下を押していたら下に進む
    if band(Key, DxLua.PAD_INPUT_DOWN) ~= 0 then PlayerY = PlayerY + 100 * DeltaTime end

    -- 右を押していたら右に進む
    if band(Key, DxLua.PAD_INPUT_RIGHT) ~= 0 then PlayerX = PlayerX + 100 * DeltaTime end

    -- 左を押していたら左に進む
    if band(Key, DxLua.PAD_INPUT_LEFT) ~= 0 then PlayerX = PlayerX - 100 * DeltaTime end

    if DxLua.CheckHitKey(DxLua.KEY_INPUT_F5) ~= 0 then return 'reload' end

    -- 画面を初期化する
    DxLua.ClearDrawScreen()

    -- プレイヤーを描画する
    DxLua.DrawGraph(PlayerX, PlayerY, PlayerGraph, true)

    -- 裏画面の内容を表画面に反映させる
    DxLua.ScreenFlip()
end
