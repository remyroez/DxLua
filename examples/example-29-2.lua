-- アクションサンプルプログラム( 坂道あり )

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
-- 1 : 通常ブロック
-- 2 : 右方向登り地面ブロック
-- 3 : 右方向下り地面ブロック
-- 4 : 右方向下り天井ブロック
-- 5 : 右方向登り天井ブロック
-- 6 : 上下判定のみブロック
-- 7 : 坂道補助ブロック
local MapData =
{
    { 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1 },
    { 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,2,1 },
    { 1,0,0,1,6, 3,0,0,0,0, 0,0,0,0,0, 0,0,2,7,1 },
    { 1,0,0,4,7, 7,3,0,0,0, 0,0,0,0,0, 0,2,7,7,1 },
    { 1,0,0,0,4, 6,1,0,0,0, 0,0,0,0,0, 1,1,1,1,1 },

    { 1,0,0,0,0, 0,0,0,1,1, 0,0,0,0,0, 0,0,1,0,1 },
    { 1,0,0,0,0, 0,0,0,0,0, 0,0,1,1,0, 0,0,1,0,1 },
    { 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,1 },
    { 1,0,1,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1 },
    { 1,0,0,0,0, 0,0,1,6,3, 0,0,0,0,2, 6,1,0,0,1 },

    { 1,0,0,0,0, 0,0,4,7,1, 0,0,0,2,7, 7,1,0,0,1 },
    { 1,0,0,0,0, 0,0,0,1,1, 0,0,0,1,1, 1,1,0,0,1 },
    { 1,0,0,2,6, 3,0,0,0,0, 0,0,0,1,1, 1,1,0,0,1 },
    { 1,0,2,7,7, 7,3,0,0,0, 0,0,0,0,0, 0,0,0,0,1 },
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
    while dx.GetNowCount() - FrameStartTime < 1000 / 60 do dx.WaitTimer(1) end

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
                local block = MapData[i][j]
                local x, y = j - 1, i - 1
                -- １～７はブロックチップを表しているので１～７のところだけ描画
                if block == 1 or block == 6 or block == 7 then
                    dx.DrawBox(
                        x * CHIP_SIZE, y * CHIP_SIZE,
                        x * CHIP_SIZE + CHIP_SIZE, y * CHIP_SIZE + CHIP_SIZE,
                        dx.GetColor(255, 255, 255), true)

                elseif block == 2 then
                    dx.DrawTriangle(
                        x * CHIP_SIZE, y * CHIP_SIZE + CHIP_SIZE,
                        x * CHIP_SIZE + CHIP_SIZE, y * CHIP_SIZE,
                        x * CHIP_SIZE + CHIP_SIZE, y * CHIP_SIZE + CHIP_SIZE,
                        dx.GetColor(255, 255, 255), true)

                elseif block == 3 then
                    dx.DrawTriangle(
                        x * CHIP_SIZE, y * CHIP_SIZE,
                        x * CHIP_SIZE + CHIP_SIZE, y * CHIP_SIZE + CHIP_SIZE,
                        x * CHIP_SIZE, y * CHIP_SIZE + CHIP_SIZE,
                        dx.GetColor(255, 255, 255), true)

                elseif block == 4 then
                    dx.DrawTriangle(
                        x * CHIP_SIZE, y * CHIP_SIZE,
                        x * CHIP_SIZE + CHIP_SIZE, y * CHIP_SIZE,
                        x * CHIP_SIZE + CHIP_SIZE, y * CHIP_SIZE + CHIP_SIZE,
                        dx.GetColor(255, 255, 255), true)

                elseif block == 5 then
                    dx.DrawTriangle(
                        x * CHIP_SIZE, y * CHIP_SIZE + CHIP_SIZE,
                        x * CHIP_SIZE, y * CHIP_SIZE,
                        x * CHIP_SIZE + CHIP_SIZE, y * CHIP_SIZE,
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
    local MoveXBackup = MoveX
    local MoveYBackup = MoveY

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
        if result == 4 and DownSP < 0.0 then DownSP = DownSP * -1.0 end

        -- 右上のチェック、もしブロックの下辺に当たっていたら落下させる
        result, Dummy, MoveY = MapHitCheck(X + hsize, Y - hsize, Dummy, MoveY)
        if result == 4 and DownSP < 0.0 then DownSP = DownSP * -1.0 end
    end

    -- 次に左右移動成分だけでチェック
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
        MoveX = 0.0
    end

    -- 最後に坂道ブロック、上下判定ブロックを含めてチェック
    do
        -- DxLua: 当たり判定の戻り値を受け取る変数
        local result

        -- 上辺のチェック、もしブロックの上辺に着いていたら落下を止める
        result, MoveX, MoveY = MapHitCheck2(X, Y + hsize, MoveX, MoveY)
        if result == 1 then DownSP = 0.0 end

        -- 下辺のチェック、もしブロックの下辺に着いていたら落下させる
        result, MoveX, MoveY = MapHitCheck2(X, Y - hsize, MoveX, MoveY)
        if result == 2 and DownSP < 0.0 then DownSP = DownSP * -1.0 end

        -- 上下移動成分を加算
        Y = Y + MoveY
        MoveY = 0.0
    end

    -- 接地状態保存
    do
        local ChipL1, ChipC1, ChipR1
        local ChipL2, ChipC2, ChipR2

        -- 足元にブロックがあるか判定
        ChipL1 = GetChipParam2(X - Size * 0.5, Y + Size * 0.5 + 2.0)
        ChipC1 = GetChipParam2(X, Y + Size * 0.5 + 2.0)
        ChipR1 = GetChipParam2(X + Size * 0.5, Y + Size * 0.5 + 2.0)
        if ChipL1 == 1 or ChipC1 ~= 0 or ChipR1 == 1 then
            -- ブロックがあったら接地中にする
            JumpFlag = false
        else
            -- ブロックが無い場合

            -- 前回ジャンプ中であったり、落下方向への速度が無い場合は無条件でジャンプ中にする
            if JumpFlag == true or MoveYBackup <= 0.0 then
                -- ジャンプ中にする
                JumpFlag = true
            else
                -- 前回接地していて、今回接地していなかった場合、下り坂に入ったかを判定する

                -- 足元周辺にブロックがあるか調べる
                ChipL1 = GetChipParam(X - Size * 0.5, Y + Size * 0.5)
                ChipC1 = GetChipParam(X, Y + Size * 0.5)
                ChipR1 = GetChipParam(X + Size * 0.5, Y + Size * 0.5)
                ChipL2 = GetChipParam(X - Size * 0.5, Y + Size * 0.5 + CHIP_SIZE / 2.0)
                ChipC2 = GetChipParam(X, Y + Size * 0.5 + CHIP_SIZE / 2.0)
                ChipR2 = GetChipParam(X + Size * 0.5, Y + Size * 0.5 + CHIP_SIZE / 2.0)
                if ChipL1 == 1 or ChipL1 == 2 or ChipL1 == 3 or ChipL1 == 6 or ChipL1 == 7 or
                    ChipL2 == 1 or ChipL2 == 2 or ChipL2 == 3 or ChipL2 == 6 or ChipL2 == 7 or
                    ChipC1 == 1 or ChipC1 == 2 or ChipC1 == 3 or ChipC1 == 6 or ChipC1 == 7 or
                    ChipC2 == 1 or ChipC2 == 2 or ChipC2 == 3 or ChipC2 == 6 or ChipC2 == 7 or
                    ChipR1 == 1 or ChipR1 == 2 or ChipR1 == 3 or ChipR1 == 6 or ChipR1 == 7 or
                    ChipR2 == 1 or ChipR2 == 2 or ChipR2 == 3 or ChipR2 == 6 or ChipR2 == 7 then
                    local addcy = 0

                    -- 足元周辺にブロックがあった場合は、足元のブロックに無理やり接地させる
                    if ChipC1 ~= 0 or ChipC2 ~= 0 then
                        if ChipC1 == 0 then addcy = 1 end
                    else
                        if ChipL1 ~= 0 or ChipL2 ~= 0 then
                            if ChipL1 == 0 then addcy = 1 end
                        else
                            if ChipR1 == 0 then addcy = 1 end
                        end
                    end

                    -- 接地させるために無理やり足元のブロックまで移動させる
                    MoveY = ((math.floor(math.floor(Y + hsize) / CHIP_SIZE) + 1 + addcy) * CHIP_SIZE - 1.0) - (Y + hsize)
                    local _
                    _, MoveX, MoveY = MapHitCheck2(X, Y + hsize, MoveX, MoveY)
                    Y = Y + MoveY
                    MoveY = 0.0

                    -- 落下速度0で接地中にする
                    DownSP = 0.0
                    JumpFlag = false
                else
                    -- 足場が無かったらジャンプ中にする
                    JumpFlag = true
                end
            end
        end
    end

    -- 終了
    return X, Y, DownSP, JumpFlag
end

-- マップとの当たり判定( 通常ブロックとの判定のみ )
-- ( 戻り値 0:当たらなかった  1:左辺に当たった  2:右辺に当たった
--                            3:上辺に当たった  4:下辺に当たった )
-- 注意：MoveX と MoveY 、どっちか片方が０じゃないとまともに動作しません
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

-- マップとの当たり判定( 坂道ブロック、上下判定ブロックとの判定もあり )
-- ( 戻り値 0:当たらなかった  1:上辺に当たった  2:下辺に当たった )
function MapHitCheck2(X, Y, MoveX, MoveY)
    local afX, afY
    local Chip

    -- 移動量を足す
    afX = X + MoveX
    afY = Y + MoveY

    -- ブロックに当たっているかチェック
    Chip = GetChipParam(afX, afY)
    if Chip ~= 0 then
        local blx, bty, brx, bby
        local blh
        local plx, ply

        -- ブロックの上下左右の座標を算出
        blx = math.floor(afX / CHIP_SIZE) * CHIP_SIZE        -- 左辺の X 座標
        brx = (math.floor(afX / CHIP_SIZE) + 1) * CHIP_SIZE    -- 右辺の X 座標

        bty = math.floor(afY / CHIP_SIZE) * CHIP_SIZE        -- 上辺の Y 座標
        bby = (math.floor(afY / CHIP_SIZE) + 1) * CHIP_SIZE    -- 下辺の Y 座標

        -- プレイヤーの位置がブロックのどの位置にあるか算出
        plx = (afX - blx) / CHIP_SIZE
        ply = (afY - bty) / CHIP_SIZE

        -- 坂道補助ブロックだった場合は上下の坂道ブロックを判定対象にする
        if Chip == 7 then
            local addy

            -- 上下の近いブロックを対象にする
            if ply < 0.5 then
                addy = -CHIP_SIZE
            else
                addy = CHIP_SIZE
            end

            -- ブロック取り直し
            Chip = GetChipParam(afX, afY + addy)

            -- ブロックの上下左右の座標を再算出
            bty = math.floor(math.floor(afY + addy) / CHIP_SIZE) * CHIP_SIZE        -- 上辺の Y 座標
            bby = (math.floor(math.floor(afY + addy) / CHIP_SIZE) + 1) * CHIP_SIZE    -- 下辺の Y 座標

            -- プレイヤーの位置がブロックのどの位置にあるか算出
            ply = (afY - bty) / CHIP_SIZE
        end

        -- ブロックのタイプによって処理を分岐
        if Chip == 2 then    -- 右方向登りブロック
            -- プレイヤーの位置でのブロックの高さを算出
            blh = bby - plx * CHIP_SIZE

            -- ブロックにあたっていた場合
            if afY > blh then
                -- 移動量を補正する
                MoveY = blh - Y - 1.0

                -- 下辺に当たったと返す
                return 1, MoveX, MoveY
            end

        elseif Chip == 3 then    -- 右方向下りブロック
            -- プレイヤーの位置でのブロックの高さを算出
            blh = bty + plx * CHIP_SIZE

            -- ブロックにあたっていた場合
            if afY > blh then
                -- 移動量を補正する
                MoveY = blh - Y - 1.0

                -- 下辺に当たったと返す
                return 1, MoveX, MoveY
            end

        elseif Chip == 4 then    -- 右方向下り天井ブロック
            -- プレイヤーの位置でのブロックの高さを算出
            blh = bty + plx * CHIP_SIZE

            -- ブロックにあたっていた場合
            if afY < blh then
                -- 移動量を補正する
                MoveY = blh - Y + 1.0

                -- 上辺に当たったと返す
                return 2, MoveX, MoveY
            end

        elseif Chip == 5 then    -- 右方向登り天井ブロック
            -- プレイヤーの位置でのブロックの高さを算出
            blh = bby - plx * CHIP_SIZE

            -- ブロックにあたっていた場合
            if afY < blh then
                -- 移動量を補正する
                MoveY = blh - Y + 1.0

                -- 上辺に当たったと返す
                return 2, MoveX, MoveY
            end

        elseif Chip == 1 or Chip == 6 then
            -- 通常ブロック
            -- 上下判定のみブロック
            -- 上辺に当たっていた場合
            if MoveY > 0.0 then
                -- 移動量を補正する
                MoveY = bty - Y - 1.0

                -- 下辺に当たったと返す
                return 1, MoveX, MoveY
            end

            -- 下辺に当たっていた場合
            if MoveY < 0.0 then
                -- 移動量を補正する
                MoveY = bby - Y + 1.0

                -- 上辺に当たったと返す
                return 2, MoveX, MoveY
            end

            -- ここに来たら適当な値を返す
            return 1, MoveX, MoveY
        end
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

-- 坂道ブロックを考慮したマップチップの値を取得する関数
function GetChipParam2(X, Y)
    local Chip

    Chip = GetChipParam(X, Y)

    -- 坂道ブロックではなかったらそのまま値を返す
    if Chip < 2 or Chip > 7 then
        return Chip
    end

    -- 坂道ブロックの場合は坂道を考慮して当たっているか判定
    do
        local blx, bty, brx, bby
        local blh
        local xr, yr

        -- ブロックの上下左右の座標を算出
        blx = math.floor(X / CHIP_SIZE) * CHIP_SIZE        -- 左辺の X 座標
        brx = (math.floor(X / CHIP_SIZE) + 1) * CHIP_SIZE    -- 右辺の X 座標

        bty = math.floor(Y / CHIP_SIZE) * CHIP_SIZE        -- 上辺の Y 座標
        bby = (math.floor(Y / CHIP_SIZE) + 1) * CHIP_SIZE    -- 下辺の Y 座標

        -- プレイヤーの位置がブロックのどの位置にあるか算出
        xr = (X - blx) / CHIP_SIZE
        yr = (Y - bty) / CHIP_SIZE

        -- 坂道補助ブロックだった場合は上下の坂道ブロックを判定対象にする
        if Chip == 7 then
            local addy

            -- 上下の近いブロックを対象にする
            if yr < 0.5 then
                addy = -CHIP_SIZE
            else
                addy = CHIP_SIZE
            end

            -- ブロック取り直し
            Chip = GetChipParam(X, Y + addy)

            -- ブロックの上下左右の座標を再算出
            bty = math.floor(math.floor(Y + addy) / CHIP_SIZE) * CHIP_SIZE        -- 上辺の Y 座標
            bby = (math.floor(math.floor(Y + addy) / CHIP_SIZE) + 1) * CHIP_SIZE    -- 下辺の Y 座標

            -- プレイヤーの位置がブロックのどの位置にあるか算出
            yr = (yr - bty) / CHIP_SIZE
        end

        -- ブロックのタイプによって処理を分岐
        if Chip == 2 then    -- 右方向登りブロック
            -- プレイヤーの位置でのブロックの高さを算出
            blh = bby - xr * CHIP_SIZE

            -- ブロックに当たっているか判定
            if Y > blh then
                return Chip
            end

        elseif Chip == 3 then    -- 右方向下りブロック
            -- プレイヤーの位置でのブロックの高さを算出
            blh = bty + xr * CHIP_SIZE

            -- ブロックに当たっているか判定
            if Y > blh then
                return Chip
            end

        elseif Chip == 4 then    -- 右方向下り天井ブロック
            -- プレイヤーの位置でのブロックの高さを算出
            blh = bty + xr * CHIP_SIZE

            -- ブロックに当たっているか判定
            if Y < blh then
                return Chip
            end

        elseif Chip == 5 then    -- 右方向登り天井ブロック
            -- プレイヤーの位置でのブロックの高さを算出
            blh = bby - xr * CHIP_SIZE

            -- ブロックに当たっているか判定
            if Y < blh then
                return Chip
            end

        elseif Chip == 6 then    -- 上下判定のみブロック
            return Chip
        end
    end

    -- 当たっていなかったらブロックなしの値を返す
    return 0
end
