-- １．迷路を３Ｄで表示
local BLOCK_SIZE = 1000.0		-- ブロックのサイズ

local BLOCK_NUM_X = 16		-- Ｘ方向のブロック数
local BLOCK_NUM_Z = 16		-- Ｚ方向のブロック数

local CAMERA_Y = 500.0		-- カメラの高さ

-- マップ( 1:道  0:壁 )
local Map =
{
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,1,1,1,1,0,1,1,1,0,1,1,0,1,1,0 },
	{ 0,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0 },
	{ 0,1,1,1,1,1,1,0,1,0,0,1,1,1,1,0 },
	{ 0,1,0,1,0,0,0,0,1,0,0,1,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,1,0,0,1,1,1,0,0 },
	{ 0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,0 },
	{ 0,0,0,1,1,1,0,1,0,0,0,1,0,0,1,0 },
	{ 0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0 },
	{ 0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,0 },
	{ 0,0,0,1,1,1,1,1,0,0,0,1,0,0,1,0 },
	{ 0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,0 },
	{ 0,1,1,1,0,1,0,0,0,0,1,0,1,0,1,0 },
	{ 0,1,0,1,1,1,0,0,0,1,1,0,1,0,0,0 },
	{ 0,1,0,1,0,0,0,1,1,1,0,0,1,1,1,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
}

local KabeModel = -1		-- 壁モデル
local x, z = 0, 0		-- 位置
local movx, movz = 0, 0	-- 移動先の座標
local Muki = 0		-- 向き( 0:x軸プラス方向  1:z軸マイナス方向  2:x軸マイナス方向  3:z軸プラス方向 )
local NowInput = 0		-- 現在のフレームの入力
local BackInput = 0		-- 一つ前のフレームの入力
local EdgeInput = 0		-- 入力のエッジ
local FrameNo = 0		-- フレーム番号
CamPos = dx.VECTOR()		-- カメラの座標
CamTarg = dx.VECTOR()	-- カメラの注視点

-- ＤＸライブラリ初期化処理
function dx.Init()
	-- 壁モデルの読みこみ
	KabeModel = dx.MV1LoadModel("Kabe.mqo")

	-- 位置と向きと入力状態の初期化
	x = 2
	z = 2
	Muki = 0
	NowInput = 0

	-- 描画先を裏画面にする
	dx.SetDrawScreen(dx.DX_SCREEN_BACK)
end

-- ループ
function dx.Update()
    -- DxLua: ESC で終了
    if dx.CheckHitKey(dx.KEY_INPUT_ESCAPE) ~= 0 then
        return 'exit'
    end

    -- 画面をクリアする
    dx.ClearDrawScreen()

    -- 入力情報を一つ前のフレームの入力に代入する
    BackInput = NowInput

    -- 現在の入力を取得する
    NowInput = dx.GetJoypadInputState(dx.DX_INPUT_KEY_PAD1)

    -- 現在のフレームで初めて押されたボタンを算出する
    EdgeInput = bit.band(NowInput, bit.bnot(BackInput))

    -- 上が押されたら向いている方向に移動する
    if bit.band(EdgeInput, dx.PAD_INPUT_UP) ~= 0 then
        -- 向きによって移動方向が変わる
        if Muki == 0 then
            -- Ｘ軸プラス方向
            movx = 1
            movz = 0
        elseif Muki == 1 then
            -- Ｚ軸マイナス方向
            movx = 0
            movz = -1
        elseif Muki == 2 then
            -- Ｘ軸マイナス方向
            movx = -1
            movz = 0
        elseif Muki == 3 then
            -- Ｚ軸プラス方向
            movx = 0
            movz = 1
        end

        -- 移動先のマスが道だったら移動する
        if Map[z + movz][x + movx] == 1 then
            x = x + movx
            z = z + movz
        end
    end

    -- 下が押されたら向いている方向と逆方向に移動する
    if bit.band(EdgeInput, dx.PAD_INPUT_DOWN) ~= 0 then
        -- 向きによって移動方向が変わる
        if Muki == 0 then
            -- Ｘ軸プラス方向
            movx = -1
            movz = 0
        elseif Muki == 1 then
            -- Ｚ軸マイナス方向
            movx = 0
            movz = 1
        elseif Muki == 2 then
            -- Ｘ軸マイナス方向
            movx = 1
            movz = 0
        elseif Muki == 3 then
            -- Ｚ軸プラス方向
            movx = 0
            movz = -1
        end

        -- 移動先のマスが道だったら移動する
        if Map[z + movz][x + movx] == 1 then
            x = x + movx
            z = z + movz
        end
    end

    -- 左が押されていたら向いている方向を左に９０度変更する
    if bit.band(EdgeInput, dx.PAD_INPUT_LEFT) ~= 0 then
        if Muki == 0 then
            Muki = 3
        else
            Muki = Muki - 1
        end
    end

    -- 右が押されていたら向いている方向を右に９０度変更する
    if bit.band(EdgeInput, dx.PAD_INPUT_RIGHT) ~= 0 then
        if Muki == 3 then
            Muki = 0
        else
            Muki = Muki + 1
        end
    end

    -- カメラの座標をセット
    CamPos = dx.VECTOR { x * BLOCK_SIZE, CAMERA_Y, z * BLOCK_SIZE }

    -- カメラの注視点をセット
    if Muki == 0 then
        -- Ｘ軸プラス方向
        CamTarg = dx.VECTOR { 1.0, 0.0, 0.0 }
    elseif Muki == 1 then
        -- Ｚ軸マイナス方向
        CamTarg = dx.VECTOR { 0.0, 0.0, -1.0 }
    elseif Muki == 2 then
        -- Ｘ軸マイナス方向
        CamTarg = dx.VECTOR { -1.0, 0.0, 0.0 }
    elseif Muki == 3 then
        -- Ｚ軸プラス方向
        CamTarg = dx.VECTOR { 0.0, 0.0, 1.0 }
    end
    CamTarg = dx.VAdd(CamPos, CamTarg)

    -- カメラの位置と向きをセットする
    dx.SetCameraPositionAndTarget_UpVecY(CamPos, CamTarg)

    -- マップを描画する
    for i = 1, BLOCK_NUM_Z do
        for j = 1, BLOCK_NUM_X do
            -- 道ではないところは描画しない
            if Map[i][j] == 0 then
                -- DxLua: Lua に continue は無い
            else
                -- 壁モデルの座標を変更する
                dx.MV1SetPosition(KabeModel, dx.VECTOR { j * BLOCK_SIZE, 0.0, i * BLOCK_SIZE })

                -- ４方の壁の状態で描画するフレーム番号を変更する
                FrameNo = 0
                if Map[i][j + 1] == 0 then FrameNo = FrameNo + 1 end
                if Map[i][j - 1] == 0 then FrameNo = FrameNo + 2 end
                if Map[i + 1][j] == 0 then FrameNo = FrameNo + 4 end
                if Map[i - 1][j] == 0 then FrameNo = FrameNo + 8 end

                -- 割り出した番号のフレームを描画する
                dx.MV1DrawFrame(KabeModel, FrameNo)
            end
        end
    end

    -- 裏画面の内容を表画面に反映する
    dx.ScreenFlip()
end
