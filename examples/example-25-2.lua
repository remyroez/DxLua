-- その２ 敵の弾をかわす
local band = bit.band

local RECTIME = 5 -- 動作を保存する最大時間(秒)
local SPEED = 5 -- 自機のスピード

local MAX_SHOT = 80	-- 弾の最大数
local SHOT_SPEED = 5 -- 弾の速度

-- 動作データ保存用データ構造体
-- DxLua: Lua には構造体は無いのでテーブルとして返す
function RECDATA(t)
    t = t or {}
    return {
        InputKey = t.InputKey or 0,
    }
end

-- 弾データ構造体
function SHOT(t)
    t = t or {}
    return {
        -- 位置
        x = t.x or 0,
        y = t.y or 0,
        -- 速度
        sx = t.sx or 0,
        sy = t.sy or 0,
        -- データ使用中かフラグ
        ValidFlag = t.ValidFlag or 0
    }
end

-- 乱数の初期値
local InitRand = 0

-- 弾のデータ
local Shot = {}

-- 自機の中心座標
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
dx.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function dx.Init()
	-- VSYNC待ちをしない
    dx.SetWaitVSyncFlag(false)

    StateMachine:Change('Select')
end

-- ループ
function dx.Update(...)
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
    dx.ClearDrawScreen()
    dx.DrawString(0, 0, "動作を保存する場合は十字キーの左を、再生する場合は右を押してください", dx.GetColor(255, 255, 255))
    dx.ScreenFlip()

    -- 入力があるまで待つ
    Mode = -1
end

-- 最初に動作保存か再生処理か選択してもらう
function StateMachine:Select(...)
    -- パッドの状態を得る
    Key = dx.GetJoypadInputState(dx.DX_INPUT_KEY_PAD1)

    -- 右が押された場合は再生モードにセット
    if band(Key, dx.PAD_INPUT_RIGHT) ~= 0 then
        Mode = MODE_REPLAY
        self:Change('Replay')
    end

    -- 左が押された場合は動作保存モードにセット
    if band(Key, dx.PAD_INPUT_LEFT) ~= 0 then
        Mode = MODE_REC
        self:Change('Rec')
    end
end

-- 共通の前処理
function StateMachine:PreRecReplay()
    -- 共通動作の自機位置のセット処理
    x = 320
    y = 400

    -- DxLua: Shot テーブルの初期化
    Shot = {}
    for i = 1, MAX_SHOT do
        Shot[i] = SHOT()
    end
end

-- 共通の前処理の後処理
function StateMachine:PostPreRecReplay()
	-- 乱数の初期化値をセット
    dx.SRand(InitRand)

	-- 画面の初期化＆描画先を裏画面にセット
	dx.ClearDrawScreen()
	dx.SetDrawScreen(dx.DX_SCREEN_BACK)

	-- 現在のタイムカウントを保存
	Time = dx.GetNowHiPerformanceCount()

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

	-- 乱数の初期化値を現在のタイムカウントから得る
	InitRand = dx.GetNowCount()

    self:PostPreRecReplay()
end

-- 動作保存モードの処理
function StateMachine:Rec()
    -- 画面の初期化
    dx.ClearDrawScreen()

    -- 普通にキーの入力状態を得る
    Key = dx.GetJoypadInputState(dx.DX_INPUT_KEY_PAD1)

    self:UpdatePlayer()
    self:UpdateShot()

    -- メッセージの描画
    dx.DrawString(0, 0, "動作を保存中です", dx.GetColor(255, 255, 255))

    self:Present()

    -- 動作保存モードの処理
    do
        -- キーの状態を保存
		-- int 型は 4バイトでもったいないので 使っている下８ビットだけ保存
        RecData[RecFrame].InputKey = band(Key, 0xFF)

        -- 保存した数を増やす
        RecFrame = RecFrame + 1

        -- 保存した数が規定数に達していたら(つまりもう保存できないほど保存したら)
        -- ループから抜ける
        if RecFrame > 60 * RECTIME then
            RecFrame = 60 * RECTIME
            Loop = 0
        end
    end

    -- DxLua: ループが終了したら次のステートへ
    if Loop == 0 then
        self:Change('RecAfter')
    end
end

-- バイトの書き込み
local function write_byte(fp, byte)
    -- ファイルが未指定
    if not fp then return end

    -- 値を 8 bit に丸める
    byte = bit.band(byte or 0, 0xFF)

    -- 1 byte 書き込み
    fp:write(string.char(byte))
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

-- バイトの読み込み
local function read_byte(fp)
    -- ファイルが未指定
    if not fp then return end

    local byte = 0

    -- 1 byte 読み込み
    byte = string.byte(fp:read(1) or '\0')

    return byte
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
        local fp = io.open("MoveRec2.dat", "wb")

        if not fp then
            -- 画面に終了メッセージを出す
            dx.ClearDrawScreen()
            dx.DrawString(0, 0, "動作の保存に失敗しました", dx.GetColor(255, 0, 0))
            dx.DrawString(0, 0, "\n何かキーを入力すると最初に戻ります")
            dx.ScreenFlip()

            self:Change('Done')
            return
        end

        -- 乱数の初期化値を書き出す
        write_word(fp, InitRand)

        -- 次に保存したフレーム数を書き出す
        write_word(fp, RecFrame)

        -- 次に保存した動作データを書き出す
        for i, data in ipairs(RecData) do
            write_byte(fp, data.InputKey)
        end

        -- ファイルを閉じる
        fp:close()
    end

    -- 画面に終了メッセージを出す
    dx.ClearDrawScreen()
    dx.DrawString(0, 0, "動作の保存が終了しました", dx.GetColor(255, 255, 255))
    dx.DrawString(0, 0, "\n何かキーを入力すると最初に戻ります")
    dx.ScreenFlip()

    self:Change('Done')
end

-- 動作再生時の初期化処理
function StateMachine:PreReplay()
    self:PreRecReplay()

    -- 動作保存データファイルを開く
    local fp = io.open("MoveRec2.dat", "rb")

    -- もし開けなかったら終了
    if not fp then
        -- メッセージを描画
        dx.ClearDrawScreen()
        dx.DrawString(0, 0, "動作保存ファイルがありません", dx.GetColor(255, 255, 255))
        dx.DrawString(0, 0, "\n何かキーを入力すると最初に戻ります")
        dx.ScreenFlip()

        dx.WaitKey()

        -- ＤＸライブラリを終了
        -- DxLua: 最初に戻す
        self:Change('Select')
        return
    end

    -- 乱数の初期化値を読みこむ
    InitRand = read_word(fp)

    -- 保存したフレーム数読み込む
    RecFrame = read_word(fp)

    -- 保存されているフレーム数分の動作保存データを読みこめるだけのメモリ領域を確保
    -- 動作保存データを読み込む
    RecData = {}
    for i = 1, RecFrame do
        RecData[i] = RECDATA { InputKey = read_byte(fp) }
    end

    -- 再生が終ったフレーム数を初期化
    RePlayFrame = 1

    self:PostPreRecReplay()
end

-- 動作再生時の処理
function StateMachine:Replay()
    -- 画面の初期化
    dx.ClearDrawScreen()

    -- 動作再生時の処理
    do
        -- 保存したキー入力状態を代入
        Key = RecData[RePlayFrame].InputKey
    end

    self:UpdatePlayer()
    self:UpdateShot()

    -- メッセージの描画
    dx.DrawString(0, 0, "動作を再生中です", dx.GetColor(255, 255, 255))

    self:Present()

    -- 動作再生時の処理
    do
        -- 再生したフレーム数を増やす
        RePlayFrame = RePlayFrame + 1

        -- もしすべて再生し終わっていたらループから抜ける
        if RePlayFrame > RecFrame then
            RePlayFrame = RecFrame
            Loop = 0
        end
    end

    -- DxLua: ループが終了したら次のステートへ
    if Loop == 0 then
        self:Change('ReplayAfter')
    end
end

-- 動作再生時の処理
function StateMachine:ReplayAfter()
    -- 画面に終了メッセージを出す
    dx.ClearDrawScreen()
    dx.DrawString(0, 0, "動作の再生が終了しました", dx.GetColor(255, 255, 255))
    dx.DrawString(0, 0, "\n何かキーを入力すると最初に戻ります")
    dx.ScreenFlip()

    self:Change('Done')
end

-- 終了時の処理
function StateMachine:Done()
	-- 動作保存データ保存用に確保したメモリ領域を開放する
	RecData = {}

	-- キー入力待ち
    dx.WaitKey()

    self:Change('Select')
end

-- プレイヤー更新
function StateMachine:UpdatePlayer()
    -- 自機を移動させる
    if band(Key, dx.PAD_INPUT_LEFT) ~= 0 then x = x - SPEED end
    if band(Key, dx.PAD_INPUT_RIGHT) ~= 0 then x = x + SPEED end
    if band(Key, dx.PAD_INPUT_UP) ~= 0 then y = y - SPEED end
    if band(Key, dx.PAD_INPUT_DOWN) ~= 0 then y = y + SPEED end

    -- 画面外に出た場合の補正
    if x > 625 then x = 620 end
    if x < 15 then x = 0 end
    if y > 465 then y = 460 end
    if y < 15 then y = 0 end

    -- 自機の描画
    dx.DrawBox(x - 15, y - 15, x + 15, y + 15, dx.GetColor(255, 255, 0), true)
end

-- 弾更新
function StateMachine:UpdateShot()
    -- 弾の移動処理を行う
    for i = 1, MAX_SHOT do
        -- 使用中フラグが立っている場合処理
        if Shot[i].ValidFlag == 1 then
            -- 弾の移動
            Shot[i].y = Shot[i].y + (Shot[i].sy / 1000)
            Shot[i].x = Shot[i].x + (Shot[i].sx / 1000)

            -- ショットを描画
            dx.DrawCircle(Shot[i].x, Shot[i].y, 5, dx.GetColor(255, 0, 0), true)

            -- 画面外に出たらショットを無効にする
            if Shot[i].y > 500 or Shot[i].y < -30 or
                Shot[i].x > 660 or Shot[i].x < -30 then
                Shot[i].ValidFlag = 0
            end
        end
    end

    -- 弾と自機の当たり判定を行う
    for i = 1, MAX_SHOT do
        local r, xl, yl = 0, 0, 0

        if Shot[i].ValidFlag == 1 then
            -- 自機の中心位置と弾の中心位置との距離の２乗を求める
            -- (三平方の定理)
            xl = Shot[i].x - x
            yl = Shot[i].y - y
            r = xl * xl + yl * yl

            -- 距離の二乗が１５ドットの二乗より少なかったら当たったと判定
            if r < 15 * 15 then
                Loop = 0
            end
        end
    end

    -- 弾の追加処理、ある一定の確率で出現
    if dx.GetRand(5) == 0 then
        local i = MAX_SHOT

        -- 空いているデータを探す
        for j = 1, MAX_SHOT do
            -- 使用中フラグが倒れていたらそれに決定
            if Shot[j].ValidFlag == 0 then
                i = j
                break
            end
        end

        -- 空いているデータがあった場合のみ弾追加
        if i ~= MAX_SHOT then
            -- 使用中フラグを立てる
            Shot[i].ValidFlag = 1

            -- 位置をセット
            Shot[i].x = dx.GetRand(639)
            Shot[i].y = 0

            -- 自機に向かっていく弾かただ下に落ちるだけの弾か分岐
            if dx.GetRand(1) == 0 then
                -- ただ下に移動する弾の場合の処理

                -- 下向きに速度を与える
                Shot[i].sx = 0
                Shot[i].sy = SHOT_SPEED * 1000
            else
                local xx, yy, r = 0, 0, 0

                -- 自機に向かっていく弾の場合の処理

                -- 弾の位置から自機の中心位置までのＸ軸Ｙ軸ごとの距離を算出
                xx = x - Shot[i].x
                yy = y - Shot[i].y

                -- 三平方の定理で直線距離を算出
                r = math.sqrt(xx * xx + yy * yy)

                -- 精度を１０００倍にした後算出したＸ軸Ｙ軸ごとの距離を直線距離で割る
                xx = (xx * 1000) / r
                yy = (yy * 1000) / r

                -- 割られたＸ軸Ｙ軸ごとの距離に弾のスピードを掛けてやったものを
                -- 弾のスピードにセット
                Shot[i].sx = xx * SHOT_SPEED
                Shot[i].sy = yy * SHOT_SPEED
            end
        end
    end
end

-- バッファの入れ替えと待機
function StateMachine:Present()
    -- 裏画面の状態を表画面に反映させる
    dx.ScreenFlip()

    -- ６０分の１秒経つまで待つ
    while (dx.ProcessMessage() == 0 and dx.GetNowHiPerformanceCount() - Time < 1000000 / 60) do end
    Time = dx.GetNowHiPerformanceCount()
end
