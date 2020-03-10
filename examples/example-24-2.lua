-- ホーミングレーザー２
local band = bit.band

local MAX_L = 20		-- レーザーの最大数定義
local PI = 3.14159		-- 円周率

local LINE_MAXNUM = 1000				-- 描画する線の最大数

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
        -- レーザーのラインデータ
        Line = t.Line or {},
        -- 表示されているラインの数
        LineNum = t.LineNum or 0,
        -- このデータが使用中かフラグ
        ValidFlag = t.ValidFlag or 0,
    }
end

-- ライン描画用構造体宣言
function LINE(t)
    t = t or {}
    return {
        -- 描くラインの座標
        x = t.x or 0,
        y = t.y or 0,
        -- ラインの向き
        Angle = t.Angle or 0,
        -- 描くラインの色決定用値
        Counter = t.Counter or 0,
    }
end

-- データ定義

local Hx, Hy = 0, 0					-- 砲台の座標
local Hm = 0						-- 砲台の移動方向
local Hsc = 0						-- 砲台のショット間隔カウンタ
local Px, Py = 0, 0					-- 自機の座標

local Ld = {}				-- ホーミングレーザーのデータ
local Lg = 0				-- レーザーのグラフィックハンドル

local Key = 0
local Time = 0

-- 画面モードのセット
dx.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function dx.Init()
	-- 描画先を裏画面にセット
	dx.SetDrawScreen(dx.DX_SCREEN_BACK)

	-- 初期化処理
	do
		-- レーザーグラフィックのロード
        Lg = dx.LoadGraph("Laser.bmp")

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

            -- DxLua: ライン描画データの初期化
            for j = 1, LINE_MAXNUM do
                Ld[i].Line[j] = LINE()
            end
        end
	end

	-- ゲームループ
	Time = dx.GetNowHiPerformanceCount() + 1000000 / 60
end

-- ループ
function dx.Update()
    -- プレイヤーの移動処理
    do
        -- 入力取得
        Key = dx.GetJoypadInputState(dx.DX_INPUT_KEY_PAD1)

        if band(Key, dx.PAD_INPUT_RIGHT) ~= 0 then Px = Px + 5 end	-- 右を押していたら右に進む
        if band(Key, dx.PAD_INPUT_LEFT) ~= 0 then Px = Px - 5 end	-- 左を押していたら左に進む

        if band(Key, dx.PAD_INPUT_UP)   ~= 0 then Py = Py - 5 end	-- 上を押していたら上に進む
        if band(Key, dx.PAD_INPUT_DOWN) ~= 0 then Py = Py + 5 end	-- 下を押していたら下に進む

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

				-- ベクトルb と aの絶対値を求める
				local br = math.sqrt(bx * bx + by * by)
				local ar = math.sqrt(ax * ax + ay * ay)

                -- 外積を利用し向きを照準側に向ける
				Ld[i].Angle = Ld[i].Angle + (PI / 180 * ((bx * ay - by * ax) / (br * ar)) * 5)
            end

            -- 追尾カウンタ加算
            Ld[i].Counter = Ld[i].Counter + 1

            -- 速度を変更する
			Ld[i].sx = math.cos(Ld[i].Angle) * 1000
			Ld[i].sy = math.sin(Ld[i].Angle) * 1000

            -- 移動前のアドレスを保存
            xb = Ld[i].x
            yb = Ld[i].y

            -- 移動する
            Ld[i].x = (Ld[i].x * 100 + Ld[i].sx) / 100
            Ld[i].y = (Ld[i].y * 100 + Ld[i].sy) / 100

            -- 現在の状態をラインデータに変換
            do
                local j = LINE_MAXNUM

                if Ld[i].LineNum ~= LINE_MAXNUM then
                    -- ライン情報をセットする
                    j = Ld[i].LineNum + 1

                    -- 座標のセット
                    Ld[i].Line[j].x = xb
                    Ld[i].Line[j].y = yb

					-- 角度をセット
					do
						local r = math.sqrt(Ld[i].sx * Ld[i].sx + Ld[i].sy * Ld[i].sy)
						Ld[i].Line[j].Angle = math.atan2(Ld[i].sy, Ld[i].sx)
                    end

                    -- 色決定カウンタを初期化
                    Ld[i].Line[j].Counter = 0

                    -- ラインの数を増やす
                    Ld[i].LineNum = Ld[i].LineNum + 1
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
                if Ld[j].ValidFlag == 0 and Ld[j].LineNum == 0 then
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
        local DeleteNum = 0

        -- 画面の初期化
        dx.ClearDrawScreen()

        -- ラインの描画
        for j = 1, MAX_L do
            DeleteNum = 0
            for i = 1 , Ld[j].LineNum - DeleteNum do
                -- もしカウンタが規定値に達していたら普段と１８０度回転したグラフィックを
                -- 描画し、その後ラインデータを無効にする
                if Ld[j].Line[i].Counter == 64 then
                    -- ラインの描画
                    dx.DrawRotaGraph(Ld[j].Line[i].x, Ld[j].Line[i].y,
                        1.0, Ld[j].Line[i].Angle + PI, Lg, true)

                    -- 削除するデータの数を一つ増やす
                    DeleteNum = DeleteNum + 1

                    -- データを詰める
                    -- DxLua: メモリ操作関数は無いので、代わりにテーブル操作を行う
                    table.remove(Ld[j].Line, 1)
                    table.insert(Ld[j].Line, LINE())

                    -- 詰めたので次のデータが今の配列番号 i と同じなる
                    -- ので、の処理
                    i = i - 1

                else
                    -- ラインの描画
                    dx.DrawRotaGraph(Ld[j].Line[i].x, Ld[j].Line[i].y,
                        1.0, Ld[j].Line[i].Angle, Lg, true)

                    -- カウンタを加算する
                    Ld[j].Line[i].Counter = Ld[j].Line[i].Counter + 1
                end
            end
            Ld[j].LineNum = Ld[j].LineNum - DeleteNum
        end

        -- 描画輝度をセット
        dx.SetDrawBright(255, 255, 255)

        -- プレーヤーの描画
        dx.DrawBox(Px, Py, Px + 32, Py + 32, dx.GetColor(255, 255, 255), true)

        -- 砲台の描画
        dx.DrawBox(Hx - 8, Hy - 8, Hx + 8, Hy + 8, dx.GetColor(255, 255, 0), true)

        -- 裏画面の内容を表画面に反映
        dx.ScreenFlip()

        -- 時間待ち
        while (dx.GetNowHiPerformanceCount() < Time) do end
        Time = Time + 1000000 / 60
    end
end
