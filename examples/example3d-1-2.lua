-- １．迷路を３Ｄで表示
local BLOCK_SIZE = 1000.0		-- ブロックのサイズ

local BLOCK_NUM_X = 16		-- Ｘ方向のブロック数
local BLOCK_NUM_Z = 16		-- Ｚ方向のブロック数

local CAMERA_Y = 500.0		-- カメラの高さ

local MOVE_FRAME = 30		-- 移動や旋回に掛けるフレーム数

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
local Count		-- 汎用カウンタ
local FrameNo = 0		-- フレーム番号
CamPos = dx.VECTOR()		-- カメラの座標
CamDir = dx.VECTOR()		-- カメラの向いている方向
CamTarg = dx.VECTOR()	-- カメラの注視点

-- DxLua: 状態が複雑なのでステートマシンで管理する
local StateMachine = {
    State = 'Wait' -- 状態( 0:入力待ち 1:前進中 2:後退中 3:左旋回中 4:右旋回中 )
}

-- ＤＸライブラリ初期化処理
function dx.Init()
	-- 壁モデルの読みこみ
	KabeModel = dx.MV1LoadModel("Kabe.mqo")

	-- 位置と向きの初期化
	x = 2
	z = 2
    Muki = 0

	-- カメラの座標と向きと注視点をセットする
	CamPos = dx.VGet(x * BLOCK_SIZE, CAMERA_Y, z * BLOCK_SIZE)
	CamDir = dx.VGet(1.0, 0.0, 0.0)
	CamTarg = dx.VAdd(CamPos, CamDir)
	dx.SetCameraPositionAndTarget_UpVecY(CamPos, CamTarg)

	-- 状態の初期化
	StateMachine.State = 'Wait'

	-- 描画先を裏画面にする
	dx.SetDrawScreen(dx.DX_SCREEN_BACK)
end

-- ループ
function dx.Update(...)
    -- DxLua: ESC で終了
    if dx.CheckHitKey(dx.KEY_INPUT_ESCAPE) ~= 0 then
        return 'exit'
    end

    -- 画面をクリアする
    dx.ClearDrawScreen()

    -- 現在の入力を取得する
    NowInput = dx.GetJoypadInputState(dx.DX_INPUT_KEY_PAD1)

    -- 状態によって処理を分岐
    StateMachine:Update(...)

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

-- 現在の状態に合わせてメソッドを呼び分ける
function StateMachine:Update(...)
	if self[self.State] then
		self[self.State](self, ...)
	end
end

-- 0: 入力待ち状態
function StateMachine:Wait(...)
    -- 上が押されたら向いている方向に移動する状態に移行する
    if bit.band(NowInput, dx.PAD_INPUT_UP) ~= 0 then
        -- 向きによって移動方向が変わる
        if Muki == 0 then
            movx = 1 movz = 0		-- Ｘ軸プラス方向
        elseif Muki == 1 then
            movx = 0 movz = -1		-- Ｚ軸マイナス方向
        elseif Muki == 2 then
            movx = -1 movz = 0		-- Ｘ軸マイナス方向
        elseif Muki == 3 then
            movx = 0 movz = 1		-- Ｚ軸プラス方向
        end

        -- 移動先のマスが道だったら移動する
        if Map[z + movz][x + movx] == 1 then
            -- 状態を前進中にする
            self.State = 'Forward'
            Count = 0
        end
    end

    -- 下が押されたら向いている方向と逆方向に移動する
    if bit.band(NowInput, dx.PAD_INPUT_DOWN) ~= 0 then
        -- 向きによって移動方向が変わる
        if Muki == 0 then
            movx = -1 movz = 0		-- Ｘ軸プラス方向
        elseif Muki == 1 then
            movx = 0 movz = 1		-- Ｚ軸マイナス方向
        elseif Muki == 2 then
            movx = 1 movz = 0		-- Ｘ軸マイナス方向
        elseif Muki == 3 then
            movx = 0 movz = -1		-- Ｚ軸プラス方向
        end

        -- 移動先のマスが道だったら移動する
        if Map[z + movz][x + movx] == 1 then
            -- 状態を後退中にする
            self.State = 'Backward'
            Count = 0
        end
    end

    -- 左が押されていたら向いている方向を左に９０度変更する
    if bit.band(NowInput, dx.PAD_INPUT_LEFT) ~= 0 then
        -- 状態を左旋回中にする
        self.State = 'TurnLeft'
        Count = 0
    end

    -- 右が押されていたら向いている方向を右に９０度変更する
    if bit.band(NowInput, dx.PAD_INPUT_RIGHT) ~= 0 then
        -- 状態を右旋回中にする
        self.State = 'TurnRight'
        Count = 0
    end

end

-- 1: 前進中状態
function StateMachine:Forward(...)
    -- カウントを進める
    Count = Count + 1

    -- カメラの座標を移動途中の座標にする
    CamPos = dx.VGet(x * BLOCK_SIZE, CAMERA_Y, z * BLOCK_SIZE)
    CamPos = dx.VAdd(CamPos, dx.VScale(CamDir, BLOCK_SIZE * Count / MOVE_FRAME))
    CamTarg = dx.VAdd(CamPos, CamDir)

    -- カウントが移動時間に達したら実座標を移動して入力待ち状態に戻る
    if Count == MOVE_FRAME then
        x = x + movx
        z = z + movz

        self.State = 'Wait'
        Count = 0
    end
end

-- 2: 後退中状態
function StateMachine:Backward(...)
    -- カウントを進める
    Count = Count + 1

    -- カメラの座標を移動途中の座標にする
    CamPos = dx.VGet(x * BLOCK_SIZE, CAMERA_Y, z * BLOCK_SIZE)
    CamPos = dx.VSub(CamPos, dx.VScale(CamDir, BLOCK_SIZE * Count / MOVE_FRAME))
    CamTarg = dx.VAdd(CamPos, CamDir)

    -- カウントが移動時間に達したら実座標を移動して入力待ち状態に戻る
    if Count == MOVE_FRAME then
        x = x + movx
        z = z + movz

        self.State = 'Wait'
        Count = 0
    end
end

-- 3: 左旋回中状態
function StateMachine:TurnLeft(...)
    -- カウントを進める
    Count = Count + 1

    -- 向いている方向を旋回途中の方向にする
    local f = 0
    if Muki == 0 then
        f = 0.0		-- Ｘプラス方向
    elseif Muki == 1 then
        f = -dx.DX_PI_F / 2.0		-- Ｚマイナス方向
    elseif Muki == 2 then
        f = dx.DX_PI_F		-- Ｘマイナス方向
    elseif Muki == 3 then
        f = dx.DX_PI_F / 2.0		-- Ｚプラス方向
    end
    f = f + dx.DX_PI_F / 2.0 * Count / MOVE_FRAME
    CamDir.x = math.cos(f)
    CamDir.z = math.sin(f)
    CamTarg = dx.VAdd(CamPos, CamDir)

    -- カウントが推移時間に達したら実方向を変更して入力待ち状態に戻る
    if Count == MOVE_FRAME then
        if Muki == 0 then
            Muki = 3
        else
            Muki = Muki - 1
        end

        self.State = 'Wait'
        Count = 0
    end
end

-- 4: 右旋回中状態
function StateMachine:TurnRight(...)
    -- カウントを進める
    Count = Count + 1

    -- 向いている方向を旋回途中の方向にする
    local f = 0
    if Muki == 0 then
        f = 0.0		-- Ｘプラス方向
    elseif Muki == 1 then
        f = -dx.DX_PI_F / 2.0		-- Ｚマイナス方向
    elseif Muki == 2 then
        f = dx.DX_PI_F		-- Ｘマイナス方向
    elseif Muki == 3 then
        f = dx.DX_PI_F / 2.0		-- Ｚプラス方向
    end
    f = f - dx.DX_PI_F / 2.0 * Count / MOVE_FRAME
    CamDir.x = math.cos(f)
    CamDir.z = math.sin(f)
    CamTarg = dx.VAdd(CamPos, CamDir)

    -- カウントが推移時間に達したら実方向を変更して入力待ち状態に戻る
    if Count == MOVE_FRAME then
        if Muki == 3 then
            Muki = 0
        else
            Muki = Muki + 1
        end

        self.State = 'Wait'
        Count = 0
    end
end
