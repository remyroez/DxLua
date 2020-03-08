-- ホーミングレーザー１
local band = bit.band

local MAX_L = 100		-- レーザーの最大数定義
local PI = 3.14159		-- 円周率

local LINE_MAXNUM = 3000				-- 描画する線の最大数

-- データ型宣言

-- レーザー構造体型宣言
-- DxLua: Lua に構造体は無いためテーブルとして返す
function LASER(t)
    t = t or {}
    return {
        -- 現在の座標
        x = t.x or 0,
        y = t.y or 0,
        -- 現在の速度
        sx = t.sx or 0,
        sy = t.sy or 0,
        -- 記録した軌跡の数
        LogNum = t.LogNum or 0,
        -- 進んでいる角度
        Angle = t.Angle or 0,
        -- 追尾をはじめてから経過した時間
        Counter = t.Counter or 0,
        -- このデータが使用中かフラグ
        ValidFlag = t.ValidFlag or 0,
    }
end

-- ライン描画用構造体宣言
function LINE(t)
    t = t or {}
    return {
        -- 描くラインの座標
        x1 = t.x1 or 0,
        y1 = t.y1 or 0,
        x2 = t.x2 or 0,
        y2 = t.y2 or 0,
        -- 描くラインの色決定用値
        Counter = t.Counter or 0,
        -- このデータが使用中かフラグ
        ValidFlag = t.ValidFlag or 0,
    }
end

-- データ定義

local Hx, Hy = 0, 0					-- 砲台の座標
local Hm = 0						-- 砲台の移動方向
local Hsc = 0						-- 砲台のショット間隔カウンタ
local Px, Py = 0, 0					-- 自機の座標

local Ld = {}				-- ホーミングレーザーのデータ
local Line = {}				-- ライン描画用データ

local Key = 0
local Time = 0

-- 画面モードのセット
DxLua.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function DxLua.Init()
	-- 描画先を裏画面にセット
	DxLua.SetDrawScreen(DxLua.DX_SCREEN_BACK)

	-- 初期化処理
	do
		-- 自機の座標セット
		Px = 320
        Py = 200

		-- 砲台の座標セット
		Hx = 320
        Hy = 30

		-- 砲台の移動方向セット
		Hm = 3

		-- 砲台の移動間隔カウンタセット
		Hsc = 30

		-- レーザーデータの初期化
        for i = 1, MAX_L do
            Ld[i] = LASER()
        end

        -- DxLua: ライン描画データの初期化
        for i = 1, LINE_MAXNUM do
            Line[i] = LINE()
        end
	end

	-- ゲームループ
	Time = DxLua.GetNowHiPerformanceCount() + 1000000 / 60
end

-- ループ
function DxLua.Update()
    -- プレイヤーの移動処理
    do
        -- 入力取得
        Key = DxLua.GetJoypadInputState(DxLua.DX_INPUT_KEY_PAD1)

        if band(Key, DxLua.PAD_INPUT_RIGHT) ~= 0 then Px = Px + 5 end	-- 右を押していたら右に進む
        if band(Key, DxLua.PAD_INPUT_LEFT) ~= 0 then Px = Px - 5 end	-- 左を押していたら左に進む

        if band(Key, DxLua.PAD_INPUT_UP)   ~= 0 then Py = Py - 5 end	-- 上を押していたら上に進む
        if band(Key, DxLua.PAD_INPUT_DOWN) ~= 0 then Py = Py + 5 end	-- 下を押していたら下に進む

        -- 画面外に出ていたら補正
        if Px > 640 - 16 then Px = 640 - 16 end
        if Px < 0 then Px = 0 end

        if Py > 480 - 16 then Py = 480 - 16 end
        if Py < 0 then Py = 0 end
    end

    -- レーザーの移動処理
    for i = 1, MAX_L do
        local xb, yb

        -- レーザーデータが無効だったらスキップ
        if Ld[i].ValidFlag == 0 then
            -- DxLua: Lua に continue は無い
        elseif (Ld[i].x > Px and Ld[i].x < Px + 32) and
            (Ld[i].y > Py and Ld[i].y < Py + 32) then
            -- 照準に当たっていたらレーザーデータを無効にする
            Ld[i].ValidFlag = 0
        else
            -- 追尾カウンタが規定値に来ていなければ追尾処理
            if Ld[i].Counter < 200 then
                local ax, ay, bx, by

                -- bx,by 自分の進んでいる方向 ax,ay 本来進むべき方向
                bx = math.cos(Ld[i].Angle)
                by = math.sin(Ld[i].Angle)
                ax = Px - Ld[i].x + 16
                ay = Py - Ld[i].y + 16

                -- 外積を利用し向きを照準側に向ける
                Ld[i].Angle = Ld[i].Angle + ((ax * by - ay * bx < 0.0) and (PI / 180 * 15) or (-PI / 180 * 15))
            end

            -- 追尾カウンタ加算
            Ld[i].Counter = Ld[i].Counter + 1

            -- 速度を変更する
            Ld[i].sx = Ld[i].sx + (math.cos(Ld[i].Angle) * 30)
            Ld[i].sy = Ld[i].sy + (math.sin(Ld[i].Angle) * 30)

            -- 移動前のアドレスを保存
            xb = Ld[i].x
            yb = Ld[i].y

            -- 移動する
            Ld[i].x = (Ld[i].x * 100 + Ld[i].sx) / 100
            Ld[i].y = (Ld[i].y * 100 + Ld[i].sy) / 100

            -- 現在の状態をラインデータに変換
            do
                local j = LINE_MAXNUM

                -- 使われていないラインデータを探す
                for k = 1, LINE_MAXNUM do
                    if Line[k].ValidFlag == 0 then
                        j = k
                        break
                    end
                end

                -- もし空のデータがあった場合のみラインデータ追加
                if j ~= LINE_MAXNUM then
                    -- ライン情報をセットする

                    -- 座標のセット
                    Line[j].x1 = xb
                    Line[j].y1 = yb
                    Line[j].x2 = Ld[i].x
                    Line[j].y2 = Ld[i].y

                    -- 色決定カウンタを初期化
                    Line[j].Counter = 0

                    -- データを使用中にセット
                    Line[j].ValidFlag = 1
                end
            end

            -- 画面外に出ていたらレーザーデータを無効にする
            if Ld[i].x < -100 or Ld[i].x > 740 or
                Ld[i].y < -100 or Ld[i].y > 580 then
                Ld[i].ValidFlag = 0
            end
        end
    end

    -- 砲台の移動処理
    do
        Hx = Hx + Hm

        -- 画面端まで来ていたら反転
        if (Hx > 640 - 16 or Hx < 0) then
            Hm = Hm * -1
        end

        -- ショットカウンタを減らす
        Hsc = Hsc - 1

        -- カウンタが０になっていたらレーザー発射
        if Hsc == 0 then
            local i = MAX_L
            -- 使われていないレーザーデータを探す
            for j = 1, MAX_L do
                if Ld[j].ValidFlag == 0 then
                    i = j
                    break
                end
            end

            -- もし使われていないレーザーデータがあったらショットを出す
            if i ~= MAX_L then
                -- レーザーの位置を設定
                Ld[i].x = Hx + 16
                Ld[i].y = Hy + 16

                -- レーザーの速度を設定
                Ld[i].sx = 0
                Ld[i].sy = 0

                -- 角度をセット
                Ld[i].Angle = PI / 2

                -- 追尾カウンタをセット
                Ld[i].Counter = 0

                -- レーザーデータを使用中にセット
                Ld[i].ValidFlag = 1
            end

            -- 発射間隔カウンタ値をセット
            Hsc = 30
        end
    end

    -- 描画処理
    do
        -- 画面の初期化
        DxLua.ClearDrawScreen()

        -- 描画ブレンドモードを加算半透明にセット
        DxLua.SetDrawBlendMode(DxLua.DX_BLENDMODE_ADD, 255)

        -- ラインの描画
        for i = 1, LINE_MAXNUM do
            if (Line[i].ValidFlag == 0) then
                -- ラインデータが有効でない場合は次に移る
            else
                -- ラインの描画
                DxLua.DrawLine(Line[i].x1, Line[i].y1,
                    Line[i].x2, Line[i].y2,
                    DxLua.GetColor(0, 255 - Line[i].Counter * 4, 0))

                -- カウンタを加算する
                Line[i].Counter = Line[i].Counter + 1

                -- もし規定値に達していたらラインデータを無効にする
                if Line[i].Counter == 64 then
                    Line[i].ValidFlag = 0
                end
            end
        end

        -- 描画ブレンドモードを通常描画モードにセット
        DxLua.SetDrawBlendMode(DxLua.DX_BLENDMODE_NOBLEND, 255)

        -- プレーヤーの描画
        DxLua.DrawBox(Px, Py, Px + 32, Py + 32, DxLua.GetColor(255, 255, 255), true)

        -- 砲台の描画
        DxLua.DrawBox(Hx - 8, Hy - 8, Hx + 8, Hy + 8, DxLua.GetColor(255, 255, 0), true)

        -- 裏画面の内容を表画面に反映
        DxLua.ScreenFlip()

        -- 時間待ち
        while (DxLua.GetNowHiPerformanceCount() < Time) do end
        Time = Time + 1000000 / 60
    end
end
