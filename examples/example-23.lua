-- ホーミングミサイル１と２
local band = bit.band

local MAX_M	= 100			-- ミサイルの最大数定義
local PI = 3.14159			-- 円周率

-- データ定義

local Hx, Hy = 0, 0				-- 砲台の座標
local Hm = 0					-- 砲台の移動方向
local Hsc = 0					-- 砲台のショット間隔カウンタ
local Px, Py = 0, 0				-- 自機の座標
local Mg = 0				    -- ミサイルのグラフィック
local Mx, My = {}, {}			-- ミサイルの座標
local Msx, Msy = {}, {}			-- ミサイルの速度
local Mv = {}					-- ミサイルデータの使用状態(1:使用中 0:未使用)
local Ma = {}					-- ミサイルの角度
local Mc = {}					-- ミサイルの追尾カウンタ

local Key = 0
local Time = 0
local Mode = 1

-- 画面モードのセット
dx.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function dx.Init()
	-- 描画先を裏画面にセット
	dx.SetDrawScreen(dx.DX_SCREEN_BACK)

	-- 初期化処理
	do
		-- ミサイルのグラフィックロード
		Mg = dx.LoadGraph("MGraph.png")

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

		-- ミサイルデータの初期化
		for i = 1, MAX_M do
            Mv[i] = 0
            -- DxLua: 他のデータも初期化する
            Mx[i] = 0
            My[i] = 0
            Ma[i] = 0
            Mc[i] = 0
            Msx[i] = 0
            Msy[i] = 0
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

        if band(Key, dx.PAD_INPUT_UP) ~= 0 then Py = Py - 5 end	-- 上を押していたら上に進む
        if band(Key, dx.PAD_INPUT_DOWN) ~= 0 then Py = Py + 5 end	-- 下を押していたら下に進む

        -- DxLua: 数字キーでモードを変える
        if dx.CheckHitKey(dx.KEY_INPUT_1) ~= 0 or dx.CheckHitKey(dx.KEY_INPUT_NUMPAD1) ~= 0 then
            Mode = 1
        elseif dx.CheckHitKey(dx.KEY_INPUT_2) ~= 0 or dx.CheckHitKey(dx.KEY_INPUT_NUMPAD2) ~= 0 then
            Mode = 2
        end

        -- 画面外に出ていたら補正
        if Px > 640 - 16 then Px = 640 - 16 end
        if Px < 0 then Px = 0 end

        if Py > 480 - 16 then Py = 480 - 16 end
        if Py < 0 then Py = 0 end
    end

    -- ミサイルの移動処理
    for i = 1, MAX_M do
        -- ミサイルデータが無効だったらスキップ
        if Mv[i] == 0 then
            -- DxLua: Lua に continue は無い
        elseif ((Mx[i] > Px and Mx[i] < Px + 32) or (Px > Mx[i] and Px < Mx[i] + 16)) and
            ((My[i] > Py and My[i] < Py + 32) or (Py > My[i] and Py < My[i] + 16)) then
            -- 照準に当たっていたらミサイルデータを無効にする
            Mv[i] = 0
        elseif (Mc[i] < 100) then
            -- 追尾カウンタが規定値に来ていなければ追尾処理
            local ax, ay, bx, by

            -- bx,by 自分の進んでいる方向 ax,ay 本来進むべき方向
            bx = math.cos(Ma[i])
            by = math.sin(Ma[i])
            ax = (Px + 16) - Mx[i]
            ay = (Py + 16) - My[i]

            -- 外積を利用し向きを照準側に向ける
            -- DxLua: モード別に角度の単位を変更する
            local unit = 8
            if Mode == 1 then
                unit = 8
            elseif Mode == 2 then
				unit = 15
            end
            Ma[i] = Ma[i] + ((ax * by - ay * bx < 0.0) and (math.pi / 180 * unit) or (-math.pi / 180 * unit))
        end

        -- 追尾カウンタ加算
        Mc[i] = Mc[i] + 1

        -- 速度変更
        Msx[i] = Msx[i] + (math.cos(Ma[i]) * 30.0);
        Msy[i] = Msy[i] + (math.sin(Ma[i]) * 30.0);

        -- 移動する
        -- DxLua: モード別に計算式を変える
        if Mode == 1 then
            Mx[i] = Mx[i] + (math.cos(Ma[i]) * 6.0)
            My[i] = My[i] + (math.sin(Ma[i]) * 6.0)
        elseif Mode == 2 then
			Mx[i] = (Mx[i] * 100 + Msx[i]) / 100;
			My[i] = (My[i] * 100 + Msy[i]) / 100;
        end

        -- 画面外に出ていたらミサイルデータを無効にする
        if (Mx[i] < -100 or Mx[i] > 740 or My[i] < -100 or My[i] > 580) then
            Mv[i] = 0
        end
    end

    -- 砲台の移動処理
    do
        Hx = Hx + Hm

        -- 画面端まで来ていたら反転
        if (Hx > 640 - 16 or Hx < 0) then Hm = Hm * -1 end

        -- ショットカウンタを減らす
        Hsc = Hsc - 1

        -- カウンタが０になっていたらミサイル発射
        if (Hsc == 0) then
            local i = MAX_M
            -- 使われていないミサイルデータを探す
            for j = 1, MAX_M do
                if (Mv[j] == 0) then
                    i = j
                    break
                end
            end

            -- もし使われていないミサイルデータがあったらショットを出す
            if (i ~= MAX_M) then
                -- ミサイルの位置を設定
                Mx[i] = Hx + 16
                My[i] = Hy + 16

                -- 速度セット
                Msx[i] = 0
                Msy[i] = 0

                -- 角度をセット
                Ma[i] = PI / 2

                -- 追尾カウンタをセット
                Mc[i] = 0

                -- ショットデータを使用中にセット
                Mv[i] = 1
            end

            -- 発射間隔カウンタ値をセット
            Hsc = 30
        end
    end

    -- 描画処理
    do
        -- 画面の初期化
        dx.ClearDrawScreen()

        -- ミサイルの描画
        for i = 1, MAX_M do
            if (Mv[i] == 0) then
                -- ミサイルデータが有効でない場合は次に移る
            else
                -- ミサイルの描画
                dx.DrawRotaGraph(Mx[i], My[i], 1.0, Ma[i], Mg, true)
            end
        end

        -- プレーヤーの描画
        dx.DrawBox(Px, Py, Px + 32, Py + 32, dx.GetColor(255, 255, 255), true)

        -- 砲台の描画
        dx.DrawBox(Hx - 8, Hy - 8, Hx + 8, Hy + 8, dx.GetColor(255, 255, 0), true)

        -- DxLua: 現在のモード
        dx.DrawString(0, 0, 'モード: ' .. Mode, dx.GetColor(0xFF, 0xFF, 0))
        dx.DrawString(0, 0, '\n１か２を入力してください')

        -- 裏画面の内容を表画面に反映
        dx.ScreenFlip()

        -- 時間待ち
        while (dx.GetNowHiPerformanceCount() < Time) do end
        Time = Time + 1000000 / 60
    end
end
