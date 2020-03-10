-- ワイプ１～５
local band = bit.band

local Key = 0
GraphHandle1, GraphHandle2 = GraphHandle1 or -1, GraphHandle2 or -1

-- DxLua: 各ワイプを一つのソースにまとめるためステートマシンの構築
-- 初期ステート
local InitialState = 'Enter'

-- カラーコード
local Yellow = dx.GetColor(0xFF, 0xFF, 0)

-- 共通の更新関数
local UpdateFn = function (self, parent, ...)
    if self[self.State] then
        self[self.State](self, parent, ...)
    else
        parent.State = InitialState
    end
end

-- 共通の待機関数
local WaitFn = function (self, parent, dt)
    dx.DrawString(0, 0, '\n左クリックで再実行\n右クリックで戻る')
    dx.ScreenFlip()
    local mouse = dx.GetMouseInput()
    if band(mouse, dx.MOUSE_INPUT_LEFT) ~= 0 then
        self.State = InitialState
    elseif band(mouse, dx.MOUSE_INPUT_RIGHT) ~= 0 then
        parent.State = InitialState
    end
end

-- 各ワイプのステートマシン
local Wipe1 = { State = InitialState, Update = UpdateFn, Wait = WaitFn }
local Wipe2 = { State = InitialState, Update = UpdateFn, Wait = WaitFn }
local Wipe3 = { State = InitialState, Update = UpdateFn, Wait = WaitFn }
local Wipe4 = { State = InitialState, Update = UpdateFn, Wait = WaitFn }
local Wipe5 = { State = InitialState, Update = UpdateFn, Wait = WaitFn }

-- メインとなるステートマシン
local StateMachine = {
    State = InitialState,
    Wipe1 = Wipe1,
    Wipe2 = Wipe2,
    Wipe3 = Wipe3,
    Wipe4 = Wipe4,
    Wipe5 = Wipe5,
}

-- 画面モードのセット
dx.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function dx.Init()
	-- グラフィックのロード
	GraphHandle1 = dx.LoadGraph("Scene1.jpg")
	GraphHandle2 = dx.LoadGraph("Scene2.jpg")

	-- 描画先を裏画面にします
    dx.SetDrawScreen(dx.DX_SCREEN_BACK)

    dx.ChangeFontType(dx.DX_FONTTYPE_EDGE)
end

-- ループ
function dx.Update(dt)
    StateMachine:Update(dt)
end

-- 開始
function StateMachine:Enter(parent, dt)
    dx.ClearDrawScreen()
    dx.DrawString(0, 0, 'ワイプ選択', Yellow)
    dx.DrawString(0, 0, '\n１～５を入力してください')
    dx.ScreenFlip()
    self.State = 'Select'
end

-- ステートマシン更新
function StateMachine:Update(...)
    -- 現在のステート名のメンバのタイプを取得
    local mode = type(self[self.State])

    if mode == 'function' then
        -- 関数なら実行
        self[self.State](self, ...)
    elseif mode == 'table' then
        -- テーブルなら更新関数があれば実行
        if self[self.State].Update then
            self[self.State]:Update(self, ...)
        end
    end
end

-- ワイプ選択
function StateMachine:Select(dt)
    -- 現在のステート
    local before = self.State

    -- キーボードでワイプ選択
    if dx.CheckHitKey(dx.KEY_INPUT_1) ~= 0 or dx.CheckHitKey(dx.KEY_INPUT_NUMPAD1) ~= 0 then
        self.State = 'Wipe1'
    elseif dx.CheckHitKey(dx.KEY_INPUT_2) ~= 0 or dx.CheckHitKey(dx.KEY_INPUT_NUMPAD2) ~= 0 then
        self.State = 'Wipe2'
    elseif dx.CheckHitKey(dx.KEY_INPUT_3) ~= 0 or dx.CheckHitKey(dx.KEY_INPUT_NUMPAD3) ~= 0 then
        self.State = 'Wipe3'
    elseif dx.CheckHitKey(dx.KEY_INPUT_4) ~= 0 or dx.CheckHitKey(dx.KEY_INPUT_NUMPAD4) ~= 0 then
        self.State = 'Wipe4'
    elseif dx.CheckHitKey(dx.KEY_INPUT_5) ~= 0 or dx.CheckHitKey(dx.KEY_INPUT_NUMPAD5) ~= 0 then
        self.State = 'Wipe5'
    end

    -- ステートが変更されたら、子ステートのステートを初期化
    if self.State ~= before then
        if type(self[self.State]) == 'table' then
            self[self.State].State = InitialState
        end
    end
end

-- 開始
function Wipe1:Enter(parent, dt)
    self.count = 17
    self.State = 'Wipe'
    GraphHandle1, GraphHandle2 = GraphHandle2, GraphHandle1
end

-- ワイプ実行
function Wipe1:Wipe(parent, dt)
    -- 画面を初期化
    dx.ClearDrawScreen()

    -- グラフィック１を描画します
    dx.DrawGraph(0, 0, GraphHandle1, false)

    -- グラフィック２を描画します
    for j = 0, math.floor(480 / 16) do
        -- 描画範囲を指定します
        dx.SetDrawArea(0, j * 16, 640, j * 16 + self.count)

        -- グラフィック２を描画します
        dx.DrawGraph(0, 0, GraphHandle2, false)
    end

    dx.SetDrawArea(0, 0, 640, 480)

    -- DxLua: 現在のワイプ
    dx.DrawString(0, 0, 'ワイプ１', Yellow)

    -- 裏画面の内容を表画面に反映させます
    dx.ScreenFlip()

    -- 時間待ち
    dx.WaitTimer(15)

    self.count = self.count - 1

    if self.count < 0 then
        self.State = 'Wait'
    end
end

-- 開始
function Wipe2:Enter(parent, dt)
    self.count = 80
    self.Mode = self.Mode and (self.Mode == 0 and 1 or 0) or 0
    self.State = 'Wipe'
    GraphHandle1, GraphHandle2 = GraphHandle2, GraphHandle1
end

-- ワイプ実行
function Wipe2:Wipe(parent, dt)
    local i = self.count

    -- 画面を初期化
    dx.ClearDrawScreen()

    -- グラフィック１を描画します
    dx.DrawGraph(0, 0, GraphHandle1, false)

    -- グラフィック２を描画します
    for j = 0, math.floor(640 / 16) do
        -- 描画可能領域設定用の値セット
        local k = j + i - 40
        if k > 0 then
            if k > 16 then k = 16 end

            -- 描画可能領域を指定します
            if self.Mode == 0 then
                dx.SetDrawArea(624 - j * 16, 0, 624 - (j * 16 - k), 480)
            else
                dx.SetDrawArea(j * 16, 0, j * 16 - k, 480)
            end

            -- グラフィック２を描画します
            dx.DrawGraph(0, 0, GraphHandle2, false)
        end
    end

    -- 描画可能領域を元に戻します
    dx.SetDrawArea(0, 0, 640, 480)

    -- DxLua: 現在のワイプ
    dx.DrawString(0, 0, 'ワイプ２', Yellow)

    -- 裏画面の内容を表画面に反映させます
    dx.ScreenFlip()

    -- 時間待ち
    dx.WaitTimer(32)

    self.count = self.count - 1

    if self.count < 0 then
        self.State = 'Wait'
    end
end

-- 開始
function Wipe3:Enter(parent, dt)
    self.count = 640 + 256
    self.Mode = self.Mode and (self.Mode == 0 and 1 or 0) or 0
    self.State = 'Wipe'
    GraphHandle1, GraphHandle2 = GraphHandle2, GraphHandle1
end

-- ワイプ実行
function Wipe3:Wipe(parent, dt)
    local i = 640 + 256 - self.count

    -- 画面を初期化
    dx.ClearDrawScreen()

    -- グラフィック１を描画します
    dx.DrawGraph(0, 0, GraphHandle1, false)

    -- グラフィック２を描画します
    for j = 0, 256 do
        -- 描画可能領域設定用の値セット
        local k = j + i - 256

        -- 描画可能領域を指定します
        if k >= 0 then
            if self.Mode == 0 then
                dx.SetDrawArea(k, 0, k + 1, 480)
            else
                dx.SetDrawArea(640 - k, 0, 640 - (k + 1), 480)
            end

            -- アルファブレンド値をセット
            dx.SetDrawBlendMode(dx.DX_BLENDMODE_ALPHA, 255 - j)

            -- グラフィック２を描画します
            dx.DrawGraph(0, 0, GraphHandle2, false)
        end
    end

    -- ブレンドモードを元に戻す
    dx.SetDrawBlendMode(dx.DX_BLENDMODE_NOBLEND, 0)

    -- グラフィック２のアルファブレンド描画以外の部分の描画
    do
        -- 描画領域設定用の値をセット
        local k = i - 256
        if k > 0 then
            if self.Mode == 0 then
                dx.SetDrawArea(0, 0, k, 480)
            else
                dx.SetDrawArea(640 - k, 0, 640, 480)
            end

            dx.DrawGraph(0, 0, GraphHandle2, false)
        end
        -- 描画領域を元に戻す
        dx.SetDrawArea(0, 0, 640, 480)
    end

    -- DxLua: 現在のワイプ
    dx.DrawString(0, 0, 'ワイプ３', Yellow)

    -- 裏画面の内容を表画面に反映させます
    dx.ScreenFlip()

    self.count = self.count - 8

    if self.count < 0 then
        self.State = 'Wait'
    end
end

-- 開始
function Wipe4:Enter(parent, dt)
    if self.Mode == 1 then
        GraphHandle1, GraphHandle2 = GraphHandle2, GraphHandle1
    end
    self.count = 400
    self.Mode = self.Mode and (self.Mode == 0 and 1 or 0) or 0
    self.State = 'Wipe'
end

-- ワイプ実行
function Wipe4:Wipe(parent, dt)
    local i = 400 - self.count

    -- 画面を初期化
    dx.ClearDrawScreen()

    -- グラフィック１を描画します
    dx.DrawGraph(0, 0, GraphHandle1, false)

    -- 描画したグラフィックの上に反転円を描きます
    DrawReversalCircle(320, 240, self.Mode == 0 and i or 399 - i, 0)

    -- DxLua: 現在のワイプ
    dx.DrawString(0, 0, 'ワイプ４', Yellow)

    -- 裏画面の内容を表画面に反映させます
    dx.ScreenFlip()

    self.count = self.count - 8

    if self.count < 0 then
        self.State = 'Wait'
    end
end

-- 開始
function Wipe5:Enter(parent, dt)
    self.count = -160
    self.Mode = self.Mode and (self.Mode == 0 and 1 or 0) or 0
    self.State = 'Wipe'
    GraphHandle1, GraphHandle2 = GraphHandle2, GraphHandle1
end

-- ワイプ実行
function Wipe5:Wipe(parent, dt)
    local i = self.count

    -- 画面を初期化
    dx.ClearDrawScreen()

    -- グラフィック１を描画します
    dx.DrawGraph(0, 0, GraphHandle1, false)

    -- 描画したグラフィックの上に円を描きます
    dx.DrawCircle(320, 240, i + 100, 0);

    -- その上からグラフィック２描きます
    if 0 < i then
        -- 直後に描いた円の中に描画可能領域をセット
        dx.SetDrawArea(320 - i, 240 - i, 320 + i, 240 + i);

        -- グラフィック２を描画
        dx.DrawGraph(0, 0, GraphHandle2, false);

        -- 反転円を描画
        DrawReversalCircle(320, 240, i, 0);

        -- 描画可能領域を元に戻す
        dx.SetDrawArea(0, 0, 640, 480);
    end

    -- DxLua: 現在のワイプ
    dx.DrawString(0, 0, 'ワイプ５', Yellow)

    -- 裏画面の内容を表画面に反映させます
    dx.ScreenFlip()

    self.count = self.count + 4

    if self.count > 500 then
        self.State = 'Wait'
    end
end

-- 反転円の描画
function DrawReversalCircle(x, y, r, Color)
	-- 円反転描画領域の外側を描画
	dx.DrawBox(0, 0, 640, y - r, Color, true)
	dx.DrawBox(0, y - r, x - r, 480, Color, true)
	dx.DrawBox(x - r, y + r + 1, 640, 480, Color, true)
	dx.DrawBox(x + r, y - r, 640, y + r + 1, Color, true)

	-- 描画処理
	do
		local Dx, Dy, F, j
		local x1, x2, y1

		-- 初期値セット
        Dx = r
        Dy = 0
        F = -2 * r + 3

		j = 0
		-- 描画開始
		do
			-- まず最初に座標データを進める
			if (F >= 0) then
                x2 = Dy + x
                x1 = -Dy + x
                y1 = Dx + y
				dx.DrawLine(0, y1, x1, y1, Color)
				dx.DrawLine(x2, y1, 640, y1, Color)
                x2 = Dy + x
                x1 = -Dy + x
                y1 = -Dx + y
				dx.DrawLine(0, y1, x1, y1, Color)
				dx.DrawLine(x2, y1, 640, y1, Color)

				Dx = Dx - 1
				F = F - 4 * Dx
            end

			Dy = Dy + 1
			F = F + 4 * Dy + 2

			-- 描き切るまでループ
			while Dx >= Dy do
				-- ラインを描く
                x2 = Dx + x
                x1 = -Dx + x
                y1 = Dy + y
				dx.DrawLine(0, y1, x1, y1, Color)
				dx.DrawLine(x2, y1, 640, y1, Color)
                x2 = Dx + x
                x1 = -Dx + x
                y1 = -Dy + y
				dx.DrawLine(0, y1, x1, y1, Color)
				dx.DrawLine(x2, y1, 640, y1, Color)

				-- 座標データを進める
				if F >= 0 then
                    x2 = Dy + x
                    x1 = -Dy + x
                    y1 = Dx + y
					dx.DrawLine(0, y1, x1, y1, Color)
					dx.DrawLine(x2, y1, 640, y1, Color)
                    x2 = Dy + x
                    x1 = -Dy + x
                    y1 = -Dx + y
					dx.DrawLine(0, y1, x1, y1, Color)
					dx.DrawLine(x2, y1, 640, y1, Color)

					Dx = Dx - 1
					F = F - 4 * Dx
                end

				Dy = Dy + 1
				F = F + (4 * Dy + 2)
			end
		end
    end

	-- 終了
	return 0
end
