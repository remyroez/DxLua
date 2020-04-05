-- アクションサンプルプログラム

local SCREEN_WIDTH = (640) -- 画面の横幅
local SCREEN_HEIGHT = (480) -- 画面の縦幅
local CHIP_SIZE = (32) -- 一つのチップのサイズ
local MAP_WIDTH = (SCREEN_WIDTH / CHIP_SIZE) -- マップの横幅
local MAP_HEIGHT = (SCREEN_HEIGHT / CHIP_SIZE) -- マップの縦幅

local G = (0.3) -- キャラに掛かる重力加速度
local JUMP_POWER = (9.0) -- キャラのジャンプ力
local SPEED = (5.0) -- キャラの移動スピード
local CHAR_SIZE = (30) -- プレイヤーのサイズ

-- マップデータ
local MapData =
{
    { 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0 },
    { 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1 },
    { 1,0,0,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1 },
    { 1,0,0,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,1 },
    { 1,0,0,1,1, 1,1,0,0,0, 0,0,0,0,0, 0,0,1,0,1 },

    { 1,0,0,0,0, 0,0,0,1,1, 0,0,0,0,0, 0,0,1,0,1 },
    { 1,0,0,0,0, 0,0,0,0,0, 0,0,1,1,0, 0,0,1,0,1 },
    { 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,1 },
    { 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1 },
    { 1,0,0,0,0, 0,0,1,1,0, 0,0,0,0,0, 1,0,0,0,1 },

    { 1,0,0,0,0, 1,1,1,1,1, 0,0,0,0,1, 1,0,0,0,1 },
    { 1,0,0,0,0, 1,1,1,1,1, 0,0,0,1,1, 1,0,0,0,1 },
    { 1,0,0,0,0, 0,0,0,0,0, 0,0,0,1,1, 1,0,0,0,1 },
    { 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1 },
    { 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1 },
}

local PlX, PlY = 0, 0 -- プレイヤーの座標(中心座標)
local PlDownSp = 0 -- プレイヤーの落下速度
local PlJumpFlag = 0 -- プレイヤーがジャンプ中か、のフラグ

local Input, EdgeInput = 0, 0 -- 入力状態
local FrameStartTime = 0 -- ６０ＦＰＳ固定用、時間保存用変数

-- 画面モードのセット
dx.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function dx.Init()
    -- 描画先を裏画面にセット
    dx.SetDrawScreen(dx.DX_SCREEN_BACK)

    -- 垂直同期信号を待たない
    dx.SetWaitVSyncFlag(false)

    -- プレイヤーの座標を初期化
    PlX = 320.0
    PlY = 240.0

    -- プレイヤーの落下速度を初期化
    PlDownSp = 0.0

    -- ジャンプ中フラグを倒す
    PlJumpFlag = false

    -- 入力状態の初期化
    Input = 0
    EdgeInput = 0

    -- ６０ＦＰＳ固定用、時間保存用変数を現在のカウント値にセット
    FrameStartTime = dx.GetNowCount()
end

-- ループ
function dx.Update()
    if dx.CheckHitKey(dx.KEY_INPUT_ESCAPE) ~= 0 then
        return 'exit'
    end

    -- 画面のクリア
    dx.ClearDrawScreen()

    -- １/６０秒立つまで待つ
    while dx.GetNowCount() - FrameStartTime < 1000 / 60 do end

    -- 現在のカウント値を保存
    FrameStartTime = dx.GetNowCount()

    -- 入力状態を更新
    do
        local i

        -- パッド１とキーボードから入力を得る
        i = dx.GetJoypadInputState(dx.DX_INPUT_KEY_PAD1)

        -- エッジを取った入力をセット
        EdgeInput = bit.band(i, bit.bnot(Input))

        -- 入力状態の保存
        Input = i
    end

    -- プレイヤーの移動処理
    do
        local MoveX, MoveY

        -- 移動量の初期化
        MoveX = 0.0
        MoveY = 0.0

        -- 左右の移動を見る
        if bit.band(Input, dx.PAD_INPUT_LEFT) ~= 0  then MoveX = MoveX - SPEED end
        if bit.band(Input, dx.PAD_INPUT_RIGHT) ~= 0 then MoveX = MoveX + SPEED end

        -- 地に足が着いている場合のみジャンプボタン(ボタン１ or Ｚキー)を見る
        if PlJumpFlag == false and bit.band(EdgeInput, dx.PAD_INPUT_A) ~= 0 then
            PlDownSp = -JUMP_POWER
            PlJumpFlag = true
        end

        -- 落下処理
        PlDownSp = PlDownSp + G

        -- 落下速度を移動量に加える
        MoveY = PlDownSp

        -- 移動量に基づいてキャラクタの座標を移動
        PlX, PlY, PlDownSp, PlJumpFlag = CharMove(PlX, PlY, PlDownSp, MoveX, MoveY, CHAR_SIZE, PlJumpFlag)
    end

    -- マップの描画
    do
        for i = 1, MAP_HEIGHT do
            for j = 1, MAP_WIDTH do
                -- １は当たり判定チップを表しているので１のところだけ描画
                if MapData[i][j] == 1 then
                    local x, y = j - 1, i - 1
                    dx.DrawBox(x * CHIP_SIZE, y * CHIP_SIZE,
                        x * CHIP_SIZE + CHIP_SIZE, y * CHIP_SIZE + CHIP_SIZE,
                        dx.GetColor(255, 255, 255), true)
                end
            end
        end
    end

    -- キャラクタの描画
    dx.DrawBox(math.floor(PlX - CHAR_SIZE * 0.5), math.floor(PlY - CHAR_SIZE * 0.5),
        math.floor(PlX + CHAR_SIZE * 0.5) + 1, math.floor(PlY + CHAR_SIZE * 0.5) + 1,
        dx.GetColor(255, 0, 0), true)

    -- 画面の更新
    dx.ScreenFlip()
end


-- キャラクタをマップとの当たり判定を考慮しながら移動する
-- DxLua: 元コードでは X, Y, DownSP, JumpFlag がポインタなので返す
function CharMove(X, Y, DownSP, MoveX, MoveY, Size, JumpFlag)
    local Dummy = 0.0
    local hsize

    -- キャラクタの左上、右上、左下、右下部分が当たり判定のある
    -- マップに衝突しているか調べ、衝突していたら補正する

    -- 半分のサイズを算出
    hsize = Size * 0.5

    -- 先ず上下移動成分だけでチェック
    do
        -- DxLua: 当たり判定の戻り値を受け取る変数
        local result

        -- 左下のチェック、もしブロックの上辺に着いていたら落下を止める
        result, Dummy, MoveY = MapHitCheck(X - hsize, Y + hsize, Dummy, MoveY)
        if result == 3 then DownSP = 0.0 end

        -- 右下のチェック、もしブロックの上辺に着いていたら落下を止める
        result, Dummy, MoveY = MapHitCheck(X + hsize, Y + hsize, Dummy, MoveY)
        if result == 3 then DownSP = 0.0 end

        -- 左上のチェック、もしブロックの下辺に当たっていたら落下させる
        result, Dummy, MoveY = MapHitCheck(X - hsize, Y - hsize, Dummy, MoveY)
        if result == 4 then DownSP = DownSP * -1.0 end

        -- 右上のチェック、もしブロックの下辺に当たっていたら落下させる
        result, Dummy, MoveY = MapHitCheck(X + hsize, Y - hsize, Dummy, MoveY)
        if result == 4 then DownSP = DownSP * -1.0 end

        -- 上下移動成分を加算
        Y = Y + MoveY
    end

    -- 後に左右移動成分だけでチェック
    do
        -- DxLua: 結果を捨てるための変数
        local _

        -- 左下のチェック
        _, MoveX, Dummy = MapHitCheck(X - hsize, Y + hsize, MoveX, Dummy)

        -- 右下のチェック
        _, MoveX, Dummy = MapHitCheck(X + hsize, Y + hsize, MoveX, Dummy)

        -- 左上のチェック
        _, MoveX, Dummy = MapHitCheck(X - hsize, Y - hsize, MoveX, Dummy)

        -- 右上のチェック
        _, MoveX, Dummy = MapHitCheck(X + hsize, Y - hsize, MoveX, Dummy)

        -- 左右移動成分を加算
        X = X + MoveX
    end

    -- 接地判定
    do
        -- キャラクタの左下と右下の下に地面があるか調べる
        if GetChipParam(X - Size * 0.5, Y + Size * 0.5 + 1.0) == 0 and
            GetChipParam(X + Size * 0.5, Y + Size * 0.5 + 1.0) == 0 then
            -- 足場が無かったらジャンプ中にする
            JumpFlag = true
        else
            -- 足場が在ったら接地中にする
            JumpFlag = false
        end
    end

    -- 終了
    return X, Y, DownSP, JumpFlag
end

-- マップとの当たり判定( 戻り値 0:当たらなかった  1:左辺に当たった  2:右辺に当たった
--                                                3:上辺に当たった  4:下辺に当たった
-- 注意：MoveX と MoveY 、どっちか片方が０じゃないとまともに動作しません(爆)
-- DxLua: 元コードでは MoveX, MoveY がポインタなので返す
function MapHitCheck(X, Y, MoveX, MoveY)
    local afX, afY

    -- 移動量を足す
    afX = X + MoveX
    afY = Y + MoveY

    -- 当たり判定のあるブロックに当たっているかチェック
    if GetChipParam(afX, afY) == 1 then
        local blx, bty, brx, bby

        -- 当たっていたら壁から離す処理を行う

        -- ブロックの上下左右の座標を算出
        blx = math.floor(afX / CHIP_SIZE) * CHIP_SIZE        -- 左辺の X 座標
        brx = math.floor(afX / CHIP_SIZE + 1) * CHIP_SIZE    -- 右辺の X 座標

        bty = math.floor(afY / CHIP_SIZE) * CHIP_SIZE        -- 上辺の Y 座標
        bby = math.floor(afY / CHIP_SIZE + 1) * CHIP_SIZE    -- 下辺の Y 座標

        -- 上辺に当たっていた場合
        if MoveY > 0.0 then
            -- 移動量を補正する
            MoveY = bty - Y - 1.0

            -- 上辺に当たったと返す
            return 3, MoveX, MoveY
        end

        -- 下辺に当たっていた場合
        if MoveY < 0.0 then
            -- 移動量を補正する
            MoveY = bby - Y + 1.0

            -- 下辺に当たったと返す
            return 4, MoveX, MoveY
        end

        -- 左辺に当たっていた場合
        if MoveX > 0.0 then
            -- 移動量を補正する
            MoveX = blx - X - 1.0

            -- 左辺に当たったと返す
            return 1, MoveX, MoveY
        end

        -- 右辺に当たっていた場合
        if MoveX < 0.0 then
            -- 移動量を補正する
            MoveX = brx - X + 1.0

            -- 右辺に当たったと返す
            return 2, MoveX, MoveY
        end

        -- ここに来たら適当な値を返す
        return 4, MoveX, MoveY
    end

    -- どこにも当たらなかったと返す
    return 0, MoveX, MoveY
end

-- マップチップの値を取得する関数
function GetChipParam(X, Y)
    local x, y

    -- 整数値へ変換
    x = math.floor(X / CHIP_SIZE)
    y = math.floor(Y / CHIP_SIZE)

    -- マップからはみ出ていたら 0 を返す
    if x >= MAP_WIDTH or y >= MAP_HEIGHT or x < 0 or y < 0 then return 0 end

    -- 指定の座標に該当するマップの情報を返す
    return MapData[y + 1][x + 1]
end
