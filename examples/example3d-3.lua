-- ブルーム効果基本

local SCREEN_W = 640				-- 画面の横幅
local SCREEN_H = 480				-- 画面の縦幅
local DOWN_SCALE = 8				-- ガウスフィルタを掛ける画像が画面のサイズの何分の１か
local DOWN_SCALE_SCREEN_W = (SCREEN_W / DOWN_SCALE)	-- ガウスフィルタを掛ける画像の横幅
local DOWN_SCALE_SCREEN_H = (SCREEN_H / DOWN_SCALE)	-- ガウスフィルタを掛ける画像の縦幅

local ColorScreen 	-- 普通の描画結果を書き込むスクリーン
local HighBrightScreen 	-- 普通の描画結果から高輝度部分を抜き出した結果を書き込むスクリーン
local DownScaleScreen 	-- 高輝度部分を縮小した結果を書き込むスクリーン
local GaussScreen 	-- 縮小画像をガウスフィルタでぼかした結果を書き込むスクリーン

local Angle 		-- カメラの水平角度
local StageModel 	-- ステージモデル
local GaussRatio 	-- ガウスフィルタのぼかし度合い

-- 画面の解像度と色ビット深度を設定
dx.SetGraphMode(SCREEN_W, SCREEN_H, 32)

-- ＤＸライブラリの初期化
function dx.Init()
	-- ステージモデルの読み込み
	StageModel = dx.MV1LoadModel("BloomTestStage.mqo")

	-- 通常の描画結果を書き込むスクリーンと、フィルターの処理結果を書き込むスクリーンの作成
	ColorScreen      = dx.MakeScreen(SCREEN_W, SCREEN_H, false)
	HighBrightScreen = dx.MakeScreen(SCREEN_W, SCREEN_H, false)
	DownScaleScreen  = dx.MakeScreen(DOWN_SCALE_SCREEN_W, DOWN_SCALE_SCREEN_H, false)
	GaussScreen      = dx.MakeScreen(DOWN_SCALE_SCREEN_W, DOWN_SCALE_SCREEN_H, false)

	-- ClearDrawScreen 時に画面に塗りつぶす色をセット(真っ白)
	dx.SetBackgroundColor(255, 255, 255)

	-- カメラの角度とガウスフィルタのぼかし度合いを初期化
	Angle = 0
	GaussRatio = 900
end

-- メインループ(何かキーが押されたらループを抜ける)
function dx.Update()
    -- 左右キーが押されたらカメラを回転する
    if dx.CheckHitKey(dx.KEY_INPUT_LEFT) ~= 0 then
        Angle = Angle - 3
        if Angle < 0 then
            Angle = Angle + 360
        end
    end
    if dx.CheckHitKey(dx.KEY_INPUT_RIGHT) ~= 0 then
        Angle = Angle + 3
        if Angle > 360 then
            Angle = Angle - 360
        end
    end

    -- 上下キーが押されたらガウスフィルタのぼかし度合いを変更する
    if dx.CheckHitKey(dx.KEY_INPUT_UP) ~= 0 then
        GaussRatio = GaussRatio + 1
    end
    if dx.CheckHitKey(dx.KEY_INPUT_DOWN) ~= 0 then
        if GaussRatio > 0 then
            GaussRatio = GaussRatio - 1
        end
    end

    -- 通常の描画結果を書き込むスクリーンを描画対象にする
    dx.SetDrawScreen(ColorScreen)

    -- 画面をクリア
    dx.ClearDrawScreen()

    -- カメラの位置と向きを設定
    dx.SetCameraPositionAndAngle(dx.VGet(0.0, 120.0, 0.0), 0.0, Angle / 180.0 * 3.14159, 0.0)

    -- ステージモデルの描画
    dx.MV1DrawModel(StageModel)

    -- 描画結果から高輝度部分のみを抜き出した画像を得る
    dx.GraphFilterBlt(
        ColorScreen,
        HighBrightScreen,
        dx.DX_GRAPH_FILTER_BRIGHT_CLIP,
        dx.DX_CMP_LESS,
        230,
        true,
        dx.GetColor(0, 0, 0),
        255
    )

    -- 高輝度部分を８分の１に縮小した画像を得る
    dx.GraphFilterBlt(HighBrightScreen, DownScaleScreen, dx.DX_GRAPH_FILTER_DOWN_SCALE, DOWN_SCALE)

    -- ８分の１に縮小した画像をガウスフィルタでぼかす
    dx.GraphFilterBlt(DownScaleScreen,  GaussScreen, dx.DX_GRAPH_FILTER_GAUSS, 16, GaussRatio)

    -- 描画対象を裏画面にする
    dx.SetDrawScreen(dx.DX_SCREEN_BACK)

    -- 通常の描画結果を描画する
    dx.DrawGraph(0, 0, ColorScreen, false)


    -- 描画モードをバイリニアフィルタリングにする(拡大したときにドットがぼやけるようにする)
    dx.SetDrawMode(dx.DX_DRAWMODE_BILINEAR)

    -- 描画ブレンドモードを加算にする
    dx.SetDrawBlendMode(dx.DX_BLENDMODE_ADD, 255)

    -- 高輝度部分を縮小してぼかした画像を画面いっぱいに２回描画する(２回描画するのはより明るくみえるようにするため)
    dx.DrawExtendGraph(0, 0, SCREEN_W, SCREEN_H, GaussScreen, false)
    dx.DrawExtendGraph(0, 0, SCREEN_W, SCREEN_H, GaussScreen, false)

    -- 描画ブレンドモードをブレンド無しに戻す
    dx.SetDrawBlendMode(dx.DX_BLENDMODE_NOBLEND, 255)

    -- 描画モードを二アレストに戻す
    dx.SetDrawMode(dx.DX_DRAWMODE_NEAREST)

    -- フィルター処理の途中経過が分かるように画面下部に縮小して描画する
    dx.DrawExtendGraph(180 * 0 + 24, 320, 180 * 0 + 24 + 160, 120 + 320, HighBrightScreen, false)
    dx.DrawExtendGraph(180 * 1 + 24, 320, 180 * 1 + 24 + 160, 120 + 320, DownScaleScreen, false)
    dx.DrawExtendGraph(180 * 2 + 24, 320, 180 * 2 + 24 + 160, 120 + 320, GaussScreen, false)

    -- 現在のガウスフィルタのぼかし度合いを描画する
    dx.DrawFormatString(0, 0, dx.GetColor(255,255,255), "Gauss:%d", GaussRatio)

    -- 裏画面の内容を表画面に反映
    dx.ScreenFlip()
end
