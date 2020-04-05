-- 関数ポインタ

-- DxLua: LuaJIT(Lua 5.1) にビット演算子は無いため bit ライブラリを利用する
local band, bnot = bit.band, bit.bnot

local PI = math.pi -- 円周率 DxLua: Lua 標準ライブラリからコピー

local MPoint -- 使用している関数ナンバー

-- 描画処理ポインタ配列
-- DxLua: 後で追加
local Method

local MethodNum -- 使用している描画処理関数ナンバー
local Count -- 描画処理用カウンタ
local OldKey = 0 -- 前フレームのキー入力情報

-- 画面モードのセット
dx.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function dx.Init()
    -- 使用関数ナンバーを初期化
    -- DxLua: Lua の添字は 1 から
	MPoint = 1

    -- 描画処理関数の数の取得
    -- DxLua: Lua には要素数を返す構文がある
    MethodNum = #Method

	-- 描画先画面を裏画面にする
	dx.SetDrawScreen(dx.DX_SCREEN_BACK)

	-- カウント初期化
	Count = 0
end

-- ループ
function dx.Update()
    -- キー入力処理
    do
        local Key

        -- もしＺキーを押していたら処理項目変更
        Key = dx.GetJoypadInputState(dx.DX_INPUT_KEY_PAD1)

        if band(band(Key, bnot(OldKey)), dx.PAD_INPUT_A) ~= 0 then
            MPoint = MPoint + 1
            if MPoint > MethodNum then MPoint = 1 end
        end

        OldKey = Key
    end

    -- 画面をクリア
    dx.ClearDrawScreen()

    -- 描画関数を使用
    Method[MPoint](Count)

    -- 裏画面の内容を表画面に反映
    dx.ScreenFlip()

    -- カウントを増やす
    Count = Count + 1
end

-- 描画処理１
function Draw1(Pal)
	local y, x

	-- 描画位置を算出
	y = (math.sin(PI / 360 * Pal) * 240) + 240
	x = Pal % (640 + 64) - 32

	dx.DrawBox(x, y, x + 16, y + 16, dx.GetColor(255, 255, 0), true)
end

-- 描画処理２
function Draw2(Pal)
	local y, x

	-- 描画位置を算出
	y = 240
	x = Pal % (640 + 64) - 32

	dx.DrawBox(x, y, x + 16, y + 16, dx.GetColor(255, 255, 0), true)
end

-- 描画処理３
function Draw3(Pal)
	local y, x

	-- 描画位置を算出
	y = Pal % (480 + 64) - 32
	x = Pal % (640 + 64) - 32

	dx.DrawBox(x, y, x + 16, y + 16, dx.GetColor(255, 255, 0), true)
end

-- DxLua: ここで追加
Method =
{
	Draw1, Draw2, Draw3
}
