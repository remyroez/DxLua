-- １ラインごとの描画処理

local BURE = 5 -- ぶれ度
local PAI = 3.14159 -- 円周率

GraphHandle = GraphHandle or -1 -- グラフィックハンドル
Count = Count or 0 -- フレームカウンタ

-- ＤＸライブラリ初期化処理
function dx.Init()
	-- グラフィックのロード
	GraphHandle = dx.LoadGraph("test1.BMP")

	-- 描画先画面を裏画面にする
	dx.SetDrawScreen(dx.DX_SCREEN_BACK)

	-- フレームカウンタを初期化する
	Count = 0
end

-- ループ
function dx.Update()
	-- 画面全体を描画可能にする
	dx.SetDrawArea(0, 0, 640, 480)

	-- 画面全体を初期化
	dx.ClearDrawScreen()

	-- １ライン単位で描画をする処理
	for i = 0, 480-1 do
		-- 特定ラインだけを描画可能にする
		dx.SetDrawArea(0, i, 640, i + 1)

		-- 画像を描画
		dx.DrawGraph(math.floor(200 - math.sin(PAI / 10.0 * (Count + i)) * BURE), 0, GraphHandle, true)
	end

	-- 裏画面の内容を表画面に反映する
	dx.ScreenFlip()

	-- フレームカウンタをインクリメントする
	Count = Count + 1
end
