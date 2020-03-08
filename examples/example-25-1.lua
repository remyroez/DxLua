-- その１ とりあえず保存
local band = bit.band

local RECTIME = 5		-- 動作を保存する最大時間(秒)
local SPEED = 10		-- 自機のスピード

-- 動作データ保存用データ構造体
-- DxLua: Lua には構造体は無いのでテーブルとして返す
function RECDATA(t)
    t = t or {}
    return {
        InputKey = t.InputKey or 0,
    }
end

-- 自機の座標
local x, y = 0, 0

-- 保存したフレーム数
local RecFrame = 0

-- 再生したフレーム数
local RePlayFrame = 0

-- 動作データ保存用メモリ領域のポインタ
local RecData = {}

-- 処理モード
local Mode = 0

-- モードごとのマクロ値を定義
local MODE_REC = 0
local MODE_REPLAY = 1

local Key, Loop = 0, 0
local Time = 0

-- DxLua: 状態が複雑なのでステートマシンで管理する
local StateMachine = {
    State = '',
}

-- 画面モードのセット
DxLua.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function DxLua.Init()
	-- VSYNC待ちをしない
    DxLua.SetWaitVSyncFlag(false)

    StateMachine:Change('Select')
end

-- ループ
function DxLua.Update(...)
    return StateMachine:Update(...)
end

-- ステートの変更
function StateMachine:Change(newState)
    self.State = newState or 'Error'

    -- 前処理として Pre(ステート) という名前の関数があれば呼ぶ
    if self['Pre' .. self.State] then
        self['Pre' .. self.State](self)
    end
end

-- ステートマシン更新
function StateMachine:Update(...)
    if self[self.State] then
        return self[self.State](self, ...)
    end
end

-- 最初に動作保存か再生処理か選択してもらう（前処理）
function StateMachine:PreSelect()
    -- メッセージの描画
    DxLua.ClearDrawScreen()
    DxLua.DrawString(0, 0, "動作を保存する場合は十字キーの左を、再生する場合は右を押してください", DxLua.GetColor(255, 255, 255))
    DxLua.ScreenFlip()

    -- 入力があるまで待つ
    Mode = -1
end

-- 最初に動作保存か再生処理か選択してもらう
function StateMachine:Select(...)
    -- パッドの状態を得る
    Key = DxLua.GetJoypadInputState(DxLua.DX_INPUT_KEY_PAD1)

    -- 右が押された場合は再生モードにセット
    if band(Key, DxLua.PAD_INPUT_RIGHT) ~= 0 then
        Mode = MODE_REPLAY
        self:Change('Replay')
    end

    -- 左が押された場合は動作保存モードにセット
    if band(Key, DxLua.PAD_INPUT_LEFT) ~= 0 then
        Mode = MODE_REC
        self:Change('Rec')
    end
end

-- 共通の前処理
function StateMachine:PreRecReplay()
    -- 共通動作の自機位置のセット処理
    x = 300
    y = 220
end

-- 共通の前処理の後処理
function StateMachine:PostPreRecReplay()
	-- 画面の初期化＆描画先を裏画面にセット
	DxLua.ClearDrawScreen()
	DxLua.SetDrawScreen(DxLua.DX_SCREEN_BACK)

	-- 現在のタイムカウントを保存
	Time = DxLua.GetNowHiPerformanceCount()

	-- 動作保存処理開始
	Loop = 1
end

-- 動作保存時の初期化処理
function StateMachine:PreRec()
    self:PreRecReplay()

    -- とりあえず秒間６０フレームの計算でデータ領域を確保
    -- DxLua: テーブル配列を作る
    RecData = {}
    for i = 1, 60 * RECTIME do
        RecData[i] = RECDATA()
    end

    -- 保存データ数の初期化
    RecFrame = 1

    self:PostPreRecReplay()
end

-- 動作保存モードの処理
function StateMachine:Rec()
    -- 画面の初期化
    DxLua.ClearDrawScreen()

    -- 普通にキーの入力状態を得る
    Key = DxLua.GetJoypadInputState(DxLua.DX_INPUT_KEY_PAD1)

    self:UpdatePlayer()

    -- メッセージの描画
    DxLua.DrawString(0, 0, "動作を保存中です", DxLua.GetColor(255, 255, 255))

    self:Present()

    -- 動作保存モードの処理
    do
        -- キーの状態を保存
        RecData[RecFrame].InputKey = Key

        -- 保存した数を増やす
        RecFrame = RecFrame + 1

        -- 保存した数が規定数に達していたら(つまりもう保存できないほど保存したら)
        -- ループから抜ける
        if RecFrame > 60 * RECTIME then
            RecFrame = 60 * RECTIME
            Loop = 0
            self:Change('RecAfter')
        end
    end
end

-- ワードの書き込み
local function write_word(fp, word)
    -- ファイルが未指定
    if not fp then return end

    -- ワードを 32 bit に丸める
    word = bit.band(word or 0, 0xFFFFFFFF)

    -- 1 byte ずつ書き込み
    for i = 1, 4 do
        local byte = bit.band(bit.rshift(word, (i - 1) * 8), 0xFF)
        fp:write(string.char(byte))
    end
end

-- ワードの読み込み
local function read_word(fp)
    -- ファイルが未指定
    if not fp then return end

    local word = 0

    -- 1 byte ずつ読み込み
    for i = 1, 4 do
        local byte = string.byte(fp:read(1) or '\0')
        word = bit.bor(word, bit.lshift(byte, (i - 1) * 8))
    end

    return word
end

-- 動作保存モードの処理
function StateMachine:RecAfter()
    -- 動作データをディスクに保存する
    do
        -- ファイルを開く
        local fp = io.open("MoveRec.dat", "wb")

        if not fp then
            -- 画面に終了メッセージを出す
            DxLua.ClearDrawScreen()
            DxLua.DrawString(0, 0, "動作の保存に失敗しました", DxLua.GetColor(255, 0, 0))
            DxLua.DrawString(0, 0, "\n何かキーを入力すると最初に戻ります")
            DxLua.ScreenFlip()

            self:Change('Done')
            return
        end

        -- 最初に保存したフレーム数を書き出す
        write_word(fp, RecFrame)

        -- 次に保存した動作データを書き出す
        for i, data in ipairs(RecData) do
            write_word(fp, data.InputKey)
        end

        -- ファイルを閉じる
        fp:close()
    end

    -- 画面に終了メッセージを出す
    DxLua.ClearDrawScreen()
    DxLua.DrawString(0, 0, "動作の保存が終了しました", DxLua.GetColor(255, 255, 255))
    DxLua.DrawString(0, 0, "\n何かキーを入力すると最初に戻ります")
    DxLua.ScreenFlip()

    self:Change('Done')
end

-- 動作再生時の初期化処理
function StateMachine:PreReplay()
    self:PreRecReplay()

    -- 動作保存データファイルを開く
    local fp = io.open("MoveRec.dat", "rb")

    -- もし開けなかったら終了
    if not fp then
        -- メッセージを描画
        DxLua.ClearDrawScreen()
        DxLua.DrawString(0, 0, "動作保存ファイルがありません", DxLua.GetColor(255, 255, 255))
        DxLua.DrawString(0, 0, "\n何かキーを入力すると最初に戻ります")
        DxLua.ScreenFlip()

        DxLua.WaitKey()

        -- ＤＸライブラリを終了
        -- DxLua: 最初に戻す
        self:Change('Select')
        return
    end

    -- 保存したフレーム数読み込む
    RecFrame = read_word(fp)

    -- 保存されているフレーム数分の動作保存データを読みこめるだけのメモリ領域を確保
    -- 動作保存データを読み込む
    RecData = {}
    for i = 1, RecFrame do
        RecData[i] = RECDATA { InputKey = read_word(fp) }
    end

    -- 再生が終ったフレーム数を初期化
    RePlayFrame = 1

    self:PostPreRecReplay()
end

-- 動作再生時の処理
function StateMachine:Replay()
    -- 画面の初期化
    DxLua.ClearDrawScreen()

    -- 動作再生時の処理
    do
        -- 保存したキー入力状態を代入
        Key = RecData[RePlayFrame].InputKey
    end

    self:UpdatePlayer()

    -- メッセージの描画
    DxLua.DrawString(0, 0, "動作を再生中です", DxLua.GetColor(255, 255, 255))

    self:Present()

    -- 動作再生時の処理
    do
        -- 再生したフレーム数を増やす
        RePlayFrame = RePlayFrame + 1

        -- もしすべて再生し終わっていたらループから抜ける
        if RePlayFrame > RecFrame then
            RePlayFrame = RecFrame
            Loop = 0
            self:Change('ReplayAfter')
        end
    end
end

-- 動作再生時の処理
function StateMachine:ReplayAfter()
    -- 画面に終了メッセージを出す
    DxLua.ClearDrawScreen()
    DxLua.DrawString(0, 0, "動作の再生が終了しました", DxLua.GetColor(255, 255, 255))
    DxLua.DrawString(0, 0, "\n何かキーを入力すると最初に戻ります")
    DxLua.ScreenFlip()

    self:Change('Done')
end

-- 終了時の処理
function StateMachine:Done()
	-- 動作保存データ保存用に確保したメモリ領域を開放する
	RecData = {}

	-- キー入力待ち
    DxLua.WaitKey()

    self:Change('Select')
end

-- プレイヤー更新
function StateMachine:UpdatePlayer()
    -- 自機を移動させる
    if band(Key, DxLua.PAD_INPUT_LEFT) ~= 0 then x = x - SPEED end
    if band(Key, DxLua.PAD_INPUT_RIGHT) ~= 0 then x = x + SPEED end
    if band(Key, DxLua.PAD_INPUT_UP) ~= 0 then y = y - SPEED end
    if band(Key, DxLua.PAD_INPUT_DOWN) ~= 0 then y = y + SPEED end

    -- 画面外に出た場合の補正
    if x > 620 then x = 620 end
    if x < 0 then x = 0 end
    if y > 460 then y = 460 end
    if y < 0 then y = 0 end

    -- 自機の描画
    DxLua.DrawBox(x, y, x + 30, y + 30, DxLua.GetColor(255, 255, 0), true)
end

-- バッファの入れ替えと待機
function StateMachine:Present()
    -- 裏画面の状態を表画面に反映させる
    DxLua.ScreenFlip()

    -- ６０分の１秒経つまで待つ
    while (DxLua.ProcessMessage() == 0 and DxLua.GetNowHiPerformanceCount() - Time < 1000000 / 60) do end
    Time = DxLua.GetNowHiPerformanceCount()
end
