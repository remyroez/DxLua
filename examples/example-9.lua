-- フェードイン・フェードアウト

local GraphHandle

-- 画面モードのセット
dx.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function dx.Init()
	-- test1.bmpを読みこむ
	GraphHandle = dx.LoadGraph("test1.bmp")

	-- 描画先を裏画面にセット
	dx.SetDrawScreen(dx.DX_SCREEN_BACK)

	-- 時間待ち
	dx.WaitTimer(1000)
end

-- ループ
function dx.Update()
	-- フェードイン処理
	for i = 0, 254 do
		-- 描画輝度をセット
		dx.SetDrawBright(i, i, i)

		-- グラフィックを描画
		dx.DrawGraph(0, 0, GraphHandle, false)
		dx.ScreenFlip()
    end

	-- フェードアウト処理
	for i = 0, 254 do
		-- 描画輝度をセット
		dx.SetDrawBright(255 - i, 255 - i, 255 - i)

		-- グラフィックを描画
		dx.DrawGraph(0, 0, GraphHandle, false)
		dx.ScreenFlip()
    end

    return 'exit'
end
