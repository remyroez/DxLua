-- 座標の回転基本

local PI = 3.14159 -- 円周率

local x, y, xd, yd = 0, 0, 0, 0

-- 画面モードのセット
DxLua.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function DxLua.Init()
	-- グラフィックの描画先を裏画面にセット
	DxLua.SetDrawScreen(DxLua.DX_SCREEN_BACK)

	-- 最初の座標を設定
	x = 100
	y = 0
end

-- ループ
function DxLua.Update()
    -- 画面をクリア
    DxLua.ClearDrawScreen()

    -- 座標を3度回転してやる
    xd = x * math.cos(PI / 180 * 3) - y * math.sin(PI / 180 * 3)
    yd = x * math.sin(PI / 180 * 3) + y * math.cos(PI / 180 * 3)

    -- 回転後の座標を保存
    x = xd
    y = yd

    -- 円を描画(画面の原点は画面の一番左上にあるので、無理やり画面の
    -- 中心に原点があるかのように座標を移動させる)
    DxLua.DrawCircle(x + 320, y + 240, 16, DxLua.GetColor(255, 0, 0), true)

    -- 画面を表示
    DxLua.ScreenFlip()
end
