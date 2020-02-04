-- ジャンプ
local band = bit.band

local PlayerX, PlayerY
local JumpPower
local PlayerGraph
local Key

-- 画面モードのセット
DxLua.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function DxLua.Init()
	-- 描画先画面を裏画面にセット
	DxLua.SetDrawScreen(DxLua.DX_SCREEN_BACK)

	-- グラフィックのロード
    PlayerGraph = DxLua.LoadGraph("Player.bmp")

	-- キャラクターの初期データをセット
	PlayerX = 0
	PlayerY = 0
	JumpPower = 0
end

-- ループ
function DxLua.Update()
    -- キー入力取得
    Key = DxLua.GetJoypadInputState(DxLua.DX_INPUT_KEY_PAD1)

    if band(Key, DxLua.PAD_INPUT_UP) ~= 0 then PlayerY = PlayerY - 3 end	-- 上を押していたら上に進む
    if band(Key, DxLua.PAD_INPUT_DOWN) ~= 0 then PlayerY = PlayerY + 3 end	-- 下を押していたら下に進む
    if band(Key, DxLua.PAD_INPUT_RIGHT) ~= 0 then PlayerX = PlayerX + 3 end	-- 右を押していたら右に進む
    if band(Key, DxLua.PAD_INPUT_LEFT) ~= 0 then PlayerX = PlayerX - 3 end	-- 左を押していたら左に進む

    -- 落下処理
    PlayerY = PlayerY - JumpPower

    -- 落下加速度を加える
    JumpPower = JumpPower - 1

    -- もし地面についていたら止まる
    if PlayerY > 300 then
        PlayerY = 300
        JumpPower = 0
    end

    -- ジャンプボタンを押していて、地面についていたらジャンプ
    if (band(Key, DxLua.PAD_INPUT_A) ~= 0 and PlayerY == 300) then JumpPower = 20 end

    -- 画面を初期化する
    DxLua.ClearDrawScreen()

    -- プレイヤーを描画する
    DxLua.DrawGraph(PlayerX, PlayerY, PlayerGraph, true)

    -- 裏画面の内容を表画面に反映させる
    DxLua.ScreenFlip()
end
