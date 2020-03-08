-- シューティングの敵のショットの基本バリエーション

-- マクロ定義部 ==============================================

-- 精度を上げるための下駄ビット数
-- DxLua: Lua の数値は double 型で十分精度があるので使用しない
--local SHFTNUM = 15

-- 敵のショットの最大数
local MAX_ENEMYSHOT = 2000

-- π
local PI = 3.14159 -- 円周率

-- 構造体定義部 ==============================================

-- 敵のショットのデータ構造体型
-- DxLua: Lua に構造体は無いためテーブルとして定義
function ENEMYSHOT(t)
    t = t or {}
    return {
        x = t.x or 0,
        y = t.y or 0,
        sx = t.sx or 0,
        sy = t.sy or 0,
        Size = t.Size or 0,
        ValidFlag = t.ValidFlag or false,
    }
end

-- 敵のデータ構造体型
function ENEMY(t)
    t = t or {}
    return {
        x = t.x or 0,
        y = t.y or 0,
        Counter = t.Counter or 0,
        Counter2 = t.Counter2 or 0,
        Angle = t.Angle or 0,
    }
end

-- プレイヤーのデータ構造体型
function PLAYER(t)
    t = t or {}
    return {
        x = t.x or 0,
        y = t.y or 0,
    }
end

-- データ宣言部 ===============================================

-- 敵のデータ
local Enemy = ENEMY()

-- 敵のショットデータ
local EnemyShot = {}
for i = 1, MAX_ENEMYSHOT do
    EnemyShot[i] = ENEMYSHOT()
end

-- 敵のショットの数
local EnemyShotNum = 0

-- プレイヤーデータ
local Player = PLAYER()

-- 敵のショットパターンナンバー
local ShotMode = 1

-- 敵のショット処理関数のポインタ配列
local EnemyShred = {}

-- プログラム部 ===============================================

-- 画面モードのセット
DxLua.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function DxLua.Init()
	-- 裏画面を使用
	DxLua.SetDrawScreen(DxLua.DX_SCREEN_BACK)

	-- 弾の数の初期化
	EnemyShotNum = 0

	-- 敵のデータの初期化
	Enemy.x = 320
	Enemy.y = 80
	Enemy.Counter = 0
	Enemy.Counter2 = 0
	Enemy.Angle = 0.0

	-- 自機の位置の初期化
	Player.x = 320
	Player.y = 400

	-- 敵のショットタイプを初期化
	ShotMode = 1
end

-- ループ
function DxLua.Update()
    if DxLua.CheckHitKey(DxLua.KEY_INPUT_ESCAPE) ~= 0 then
        return 'exit'
    end

    -- 画面の初期化
    DxLua.ClearDrawScreen()

    -- 自機の処理
    do
        local Input

        -- 入力取得
        Input = DxLua.GetJoypadInputState(DxLua.DX_INPUT_KEY_PAD1)

        -- 自機移動
        if bit.band(Input, DxLua.PAD_INPUT_LEFT) ~= 0 and (Player.x > 10) then Player.x = Player.x - 2 end
        if bit.band(Input, DxLua.PAD_INPUT_RIGHT) ~= 0 and (Player.x < 630) then Player.x = Player.x + 2 end
        if bit.band(Input, DxLua.PAD_INPUT_UP) ~= 0 and (Player.y > 10) then Player.y = Player.y - 2 end
        if bit.band(Input, DxLua.PAD_INPUT_DOWN) ~= 0 and (Player.y < 470) then Player.y = Player.y + 2 end

        -- 自機の描画
        do
            local x, y

            x = Player.x
            y = Player.y
            DxLua.DrawBox(x - 5, y - 5, x + 5, y + 5, DxLua.GetColor(255, 255, 0), false)
        end
    end

    -- 弾の処理
    do
        local Con, Num

        -- 弾の数だけ移動処理を繰り返す
        Con = 0
        Num = EnemyShotNum
        for i = 1, MAX_ENEMYSHOT do
            -- 弾のデータが有効な場合は処理
            if EnemyShot[i].ValidFlag == true then
                -- 移動処理
                EnemyShot[i].x = EnemyShot[i].x + EnemyShot[i].sx
                EnemyShot[i].y = EnemyShot[i].y + EnemyShot[i].sy

                -- 画面外に出たら弾情報を消去する
                if (EnemyShot[i].x < -20) or
                    (EnemyShot[i].x > 660) or
                    (EnemyShot[i].y < -20) or
                    (EnemyShot[i].y > 500) then
                    -- データの有効フラグを倒す
                    EnemyShot[i].ValidFlag = false

                    -- 弾の数を減らす
                    EnemyShotNum = EnemyShotNum - 1
                end

                -- 弾の描画
                do
                    local x, y

                    x = EnemyShot[i].x
                    y = EnemyShot[i].y

                    DxLua.DrawCircle(x, y, EnemyShot[i].Size, DxLua.GetColor(255, 255, 255), false)
                end

                -- 処理した弾の数をインクリメント
                Con = Con + 1

                -- 処理した弾の数が、存在している弾の数と同じになった場合はループを抜ける
                if Num < Con then break end
            end
        end
    end

    -- 敵の処理
    do
        -- 敵のショット処理
        EnemyShred[ShotMode]()

        -- 敵の描画
        do
            local x, y

            x = Enemy.x
            y = Enemy.y

            DxLua.DrawCircle(x, y, 10, DxLua.GetColor(255, 255, 255), false)
        end
    end

    -- 敵のショットの切り替え処理
    do
        local C

        -- 入力された文字を取得
        C = DxLua.GetInputChar(true)

        -- Ｃキーが押されたら敵のショットモードを変更する
        if C == string.byte('C') or C == string.byte('c') then
            Enemy.Counter2 = 0
            Enemy.Counter = 0
            Enemy.Angle = 0.0

            ShotMode = ShotMode + 1
            if ShotMode > 5 then ShotMode = 1 end
        end
	end
	
	-- DxLua: ショット名を表示する
	DxLua.DrawString(0, 0, 'ショット' .. ShotMode .. ': ' .. ShotNames[ShotMode], DxLua.GetColor(0xFF, 0xFF, 0))
	DxLua.DrawString(0, 0, '\nC で切り替えます')

    -- 画面の更新
    DxLua.ScreenFlip()
end

-- ショットの追加関数
function ShotAdd(x, y, Size, Angle, Speed)
	local i = MAX_ENEMYSHOT + 1

	-- 使われていないショットを検索
	for j = 1, MAX_ENEMYSHOT do
        if EnemyShot[j].ValidFlag == false then
            i = j
            break
        end
    end
	if i > MAX_ENEMYSHOT then return -1 end

	-- 新しいショットのデータを初期化
	do
		-- 座標セット
		EnemyShot[i].x = x
		EnemyShot[i].y = y

		-- サイズセット
		EnemyShot[i].Size = Size

		-- 飛んで行く方向とスピードから、Ｘ軸方向への移動速度とＹ軸方向への移動速度を得る
		EnemyShot[i].sx = math.cos(Angle) * Speed
		EnemyShot[i].sy = math.sin(Angle) * Speed

		-- ショットデータの有効フラグを立てる
		EnemyShot[i].ValidFlag = true

		-- ショットの数を増やす
		EnemyShotNum = EnemyShotNum + 1
	end

	-- 終了
	return 0
end

-- 回転ショット
function ShotType5()
	-- カウンタが２になったらショット処理を行う
	if Enemy.Counter == 2 then
		-- ショットの方向を変更
		Enemy.Angle = Enemy.Angle + 0.2

		-- 速度の違う五つのショットを発射
		for i = 1, 5 do
			ShotAdd(Enemy.x, Enemy.y, 6, Enemy.Angle, (3) + ((i - 1) * 8000))
		end

		-- カウンタを初期化する
		Enemy.Counter = 0
	else
		Enemy.Counter = Enemy.Counter + 1
	end

	-- 終了
	return 0
end

-- ばら撒きショット
function ShotType4()
	-- カウンタ２の値によって処理を分岐
	if Enemy.Counter2 == 0 then
		-- 待ち処理

		-- カウンタをインクリメント
		Enemy.Counter = Enemy.Counter + 1

		-- カウンタが６０になったらカウンタ２の値を増やして、ショット処理に移る
		if Enemy.Counter > 60 then
			Enemy.Counter2 = Enemy.Counter2 + 1
			Enemy.Counter = 0

			-- このときの自機への方向を保存
			Enemy.Angle = math.atan2(Player.y - Enemy.y, Player.x - Enemy.x)
		end

	elseif Enemy.Counter2 == 1 then
		--ショット処理

		-- カウンタが２の倍数の時のみショットする
		if Enemy.Counter % 2 == 0 then
			local Angle

			-- 飛ばす方向にぶれをつける
			Angle = Enemy.Angle + (PI / 3600 * (DxLua.GetRand(800) - 400))

			-- ショット
			ShotAdd(Enemy.x, Enemy.y, 5, Angle, 3)
		end

		-- カウンタをインクリメント、カウントが５０になったら待ちに戻る
		Enemy.Counter = Enemy.Counter + 1
		if Enemy.Counter == 50 then
			Enemy.Counter2 = 0
			Enemy.Counter = 0
		end
	end

	-- 終了
	return 0
end

-- 一点手中時間差ショット
function ShotType3()
	-- カウンタ２の値によって処理を分岐
	if Enemy.Counter2 == 0 then
		-- 待ち処理

		-- カウンタをインクリメント
		Enemy.Counter = Enemy.Counter + 1

		-- カウンタが６０になったらカウンタ２の値を増やして、ショット処理に移る
		if Enemy.Counter > 60 then
			Enemy.Counter2 = Enemy.Counter2 + 1
			Enemy.Counter = 0

			-- このときの自機への方向を保存
			Enemy.Angle = math.atan2(Player.y - Enemy.y, Player.x - Enemy.x)
		end

	elseif Enemy.Counter2 == 1 then
		-- ショット処理

		-- カウンタが５の倍数の時のみショットする
		if Enemy.Counter % 5 == 0 then
			ShotAdd(Enemy.x, Enemy.y, 4, Enemy.Angle, (1) + (Enemy.Counter) / 15)
		end

		-- カウンタをインクリメント、カウントが５０になったら待ちに戻る
		Enemy.Counter = Enemy.Counter + 1
		if Enemy.Counter == 50 then
			Enemy.Counter2 = 0
			Enemy.Counter = 0
		end
	end

	-- 終了
	return 0
end

-- 全方向ショットの関数
function ShotType2()
	-- カウンタが１０になったらショットする
	if Enemy.Counter == 10 then
		local Angle, d

		-- 敵から見た自機への方向を取得
		Angle = math.atan2(Player.y - Enemy.y, Player.x - Enemy.x)

		-- ワイドショット処理、少しずつ角度を変えて一周分ショット
		d = Angle - PI
		for i = 1, 20 do
			ShotAdd(Enemy.x, Enemy.y, 4, d, 4)
			d = d + (2 * PI / 20)
		end

		-- カウンタを初期化
		Enemy.Counter = 0
	else
		-- カウンタをインクリメント
		Enemy.Counter = Enemy.Counter + 1
	end

	-- 終了
	return 0
end

-- ３ＷＡＹショットの関数
function ShotType1()
	-- カウンタが４０になったらショットする
	if Enemy.Counter == 40 then
		local Angle, d

		-- 敵から見た自機への方向を取得
		Angle = math.atan2(Player.y - Enemy.y, Player.x - Enemy.x)

		-- ３ＷＡＹショット処理
		d = Angle - (PI / 3) / 2
        for i = 1, 3 do
			ShotAdd(Enemy.x, Enemy.y, 4, d, 2)
			d = d + ((PI / 3) / 2)
		end

		-- カウンタを初期化
		Enemy.Counter = 0
	else
		-- カウンタをインクリメント
		Enemy.Counter = Enemy.Counter + 1
	end

	-- 終了
	return 0
end

-- DxLua: ここで関数テーブルを用意
EnemyShred =
{
	ShotType1,
	ShotType2,
	ShotType3,
	ShotType4,
	ShotType5,
}

-- DxLua: ショット名テーブル
ShotNames = {
	'３ＷＡＹショット',
	'全方向ショット',
	'一点手中時間差ショット',
	'ばら撒きショット',
	'回転ショット',
}
