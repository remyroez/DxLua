-- フェードイン・フェードアウト

local GraphHandle

-- 画面モードのセット
DxLua.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function DxLua.Init()
	-- test1.bmpを読みこむ
	GraphHandle = DxLua.LoadGraph("test1.bmp")

	-- 描画先を裏画面にセット
	DxLua.SetDrawScreen(DxLua.DX_SCREEN_BACK)

	-- 時間待ち
	DxLua.WaitTimer(1000)
end

-- ループ
function DxLua.Update()
	-- フェードイン処理
	for i = 0, 254 do
		-- 描画輝度をセット
		DxLua.SetDrawBright(i, i, i)

		-- グラフィックを描画
		DxLua.DrawGraph(0, 0, GraphHandle, false)
		DxLua.ScreenFlip()
    end

	-- フェードアウト処理
	for i = 0, 254 do
		-- 描画輝度をセット
		DxLua.SetDrawBright(255 - i, 255 - i, 255 - i)

		-- グラフィックを描画
		DxLua.DrawGraph(0, 0, GraphHandle, false)
		DxLua.ScreenFlip()
    end

    return 'exit'
end
