-- ベジェ曲線基本

-- 制御点の座標
local P0 = { 100 , 300 }
local P1 = { 250 , 100 }
local P2 = { 400 , 400 }
local P3 = { 550 , 100 }

local DivNum = 300

local u = 0
local P01, P12, P23 = {}, {}, {}
local P02, P13 = {}, {}
local P03 = {}
local x, y = 0, 0
local Counter = 0

-- 画面モードのセット
dx.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function dx.Init()
	-- グラフィックの描画先を裏画面にセット
	dx.SetDrawScreen(dx.DX_SCREEN_BACK)

	-- カウンターを初期化
	Counter = 0
end

-- ループ
function dx.Update()
    -- 画面をクリア
    dx.ClearDrawScreen()

    -- 現在の点の位置を算出
    -- ベジェ曲線の計算をしているところです
    do
        u = (1.0 / DivNum) * Counter

        P01[1] = (1.0 - u) * P0[1] + u * P1[1]
        P01[2] = (1.0 - u) * P0[2] + u * P1[2]
        P12[1] = (1.0 - u) * P1[1] + u * P2[1]
        P12[2] = (1.0 - u) * P1[2] + u * P2[2]
        P23[1] = (1.0 - u) * P2[1] + u * P3[1]
        P23[2] = (1.0 - u) * P2[2] + u * P3[2]

        P02[1] = (1.0 - u) * P01[1] + u * P12[1]
        P02[2] = (1.0 - u) * P01[2] + u * P12[2]
        P13[1] = (1.0 - u) * P12[1] + u * P23[1]
        P13[2] = (1.0 - u) * P12[2] + u * P23[2]

        P03[1] = (1.0 - u) * P02[1] + u * P13[1]
        P03[2] = (1.0 - u) * P02[2] + u * P13[2]

        x = P03[1]
        y = P03[2]
    end

    -- 制御点を線で繋ぐ
    dx.DrawLine(P0[1], P0[2], P1[1], P1[2], dx.GetColor(0, 128, 0))
    dx.DrawLine(P1[1], P1[2], P2[1], P2[2], dx.GetColor(0, 128, 0))
    dx.DrawLine(P2[1], P2[2], P3[1], P3[2], dx.GetColor(0, 128, 0))

    -- 制御点を描画
    dx.DrawCircle(P0[1], P0[2], 2, dx.GetColor(255, 255, 0), true)
    dx.DrawCircle(P3[1], P3[2], 2, dx.GetColor(255, 255, 0), true)

    dx.DrawCircle(P1[1], P1[2], 2, dx.GetColor(255, 0, 0), true)
    dx.DrawCircle(P2[1], P2[2], 2, dx.GetColor(255, 0, 0), true)

    -- 現在の点の位置に四角を描画
    dx.DrawBox(x - 2, y - 2, x + 2, y + 2, dx.GetColor(255, 255, 255), true)

    -- カウンターをインクリメント
    Counter = Counter + 1

    -- もしカウンターが分割数に達していたら０に戻す
    if Counter == DivNum then Counter = 0 end

    -- 画面を表示
    dx.ScreenFlip()
end
