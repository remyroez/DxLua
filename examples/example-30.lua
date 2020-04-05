-- 回転壁当たり判定プログラム

local PI = (3.14159265358)		-- π
local PI_2 = (3.14159265358)		-- ２π
local BOXSIZE = (128)				-- 箱の大きさ
local BOXX = (320)				-- 箱のＸ座標
local BOXY = (240)				-- 箱のＹ座標
local ROTSPEED = (PI_2 / 180)		-- 箱の回転速度
local SPEED = (8.0)				-- 点の速度
local HITLENGTH = (4.0)				-- 箱の壁との当たり判定の幅

local rot = 0
-- 箱の回転角度
local x, y, sx, sy = 0, 0, 0, 0
-- 箱の中を動く点の座標と速度

-- 二次元ベクトル構造体
-- DxLua: Lua に構造体は無いためテーブルとして返す
function VECT(t)
    t = t or {}
    return {
        x = t.x or 0,
        y = t.y or 0
    }
end

--  HitCheck 関数内の『直線に垂直に交わる点を求める演算』で行っていること、xを求めたい
--　なお　『*』：スカラー倍　　『・』：内積　　『大文字』：ベクトル　　『小文字』：スカラー値
--
-- P1:直線の始点　V1:直線の向き　P2:交差判定をしたい直線の始点(又は終点)
-- ( P1 + x * V1 ) ：P2 から 直線に垂直に交わる点の座標
--
-- ( ( P1 + x * V1 ) - P2 )・V1 = 0
-- ( x * V1 + P1 - P2 )・V1 = 0
-- x * V1・V1 + P1・V1 - P2・V1 = 0  (← V1・V1 = 1 なので x * V1・V1 = x )
-- x = P1・V1 - P2・V1
-- x = V1・( P1 - P2 )
--
-- というわけで、『ここから～ここまで』では x = V1・( P1 - P2 ) をやっています

-- 線と線の交差判定
-- l1x1,l1y1:直線１の始点　　l1x2,l1y2:直線１の終点
-- l2x1,l2y1:直線２の始点　　l2x2,l2y2:直線２の終点
function HitCheck(l1x1, l1y1, l1x2, l1y2, l2x1, l2y1, l2x2, l2y2)
	local p1, v1, p2, v3, t = VECT(), VECT(), VECT(), VECT(), VECT()
	local x, z1, z2, len;

	-- 直線の始点と向きを算出
	p1.x = l1x1
	p1.y = l1y1

	-- 直線の始点
	v1.x = l1x2 - l1x1
	v1.y = l1y2 - l1y1

	-- 向き
	len = math.sqrt(v1.x * v1.x + v1.y * v1.y)

	-- 正規化
	v1.x = v1.x / len
	v1.y = v1.y / len;

	-- 交差判定をしたい線の始点と、始点から直線に垂直に交わる直線上の点の座標を求める

	-- 当たり判定をしたい線の始点
	p2.x = l2x1
	p2.y = l2y1

	-- 直線に垂直に交わる点を求める演算
	-- ここから
	t.x = p2.x - p1.x
	t.y = p2.y - p1.y
	x = v1.x * t.x + v1.y * t.y -- (←この行で内積してます)
	v3.x = p1.x + v1.x * x
	v3.y = p1.y + v1.y * x
	-- ここまで

	-- 始点から求めた直線上の点へのベクトルを算出
	t.x = v3.x - p2.x
	t.y = v3.y - p2.y

	-- 求めた点までの距離が HITLENGTH 以下だったら線に当たった事にする
	if math.sqrt(t.x * t.x + t.y * t.y) < HITLENGTH then return true end

	-- 始点から求めた点へのベクトルと、直線の向きとの外積を求めておく・・・①
	z1 = v1.x * t.y - v1.y * t.x

	-- 交差判定をしたい線の終点と、終点から直線に垂直に交わる直線上の点の座標を求める

	-- 当たり判定をしたい線の終点
	p2.x = l2x2
	p2.y = l2y2

	-- 直線に垂直に交わる点を求める演算
	-- ここから
	t.x = p2.x - p1.x
	t.y = p2.y - p1.y
	x = v1.x * t.x + v1.y * t.y -- (←この行で内積してます)
	v3.x = p1.x + v1.x * x
	v3.y = p1.y + v1.y * x
	-- ここまで

	-- 終点から求めた直線上の点へのベクトルを算出
	t.x = v3.x - p2.x
	t.y = v3.y - p2.y

	-- 求めた点までの距離が HITLENGTH 以下だったら線に当たった事にする
	if math.sqrt(t.x * t.x + t.y * t.y) < HITLENGTH then return true end

	-- 終点から求めた点へのベクトルと、直線の向きとの外積を求めておく・・・②
	z2 = v1.x * t.y - v1.y * t.x

	-- ①と②で求めたベクトルの向きが違う場合は交差判定をしたい線と交差した証拠
	return (z1 >= 0.0 and z2 < 0.0) or (z2 >= 0.0 and z1 < 0.0);
end

local bx, by, px, py, f = 0, 0, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, 0;

-- ＤＸライブラリ初期化処理
function dx.Init()
	-- 箱の回転角度を初期化
	rot = 0.0

	-- 点の位置を初期化
	x = 320
	y = 240

	-- 点の速度を初期化
	sx = math.random() * SPEED - SPEED / 2
	sy = math.random() * SPEED - SPEED / 2;
end

-- ループ
function dx.Update()
    if dx.CheckHitKey(dx.KEY_INPUT_ESCAPE) ~= 0 then
        return 'exit'
    end

	-- 画面初期化
	dx.ClearDrawScreen()

	-- 移動前の座標を保存しておく
	bx = x
	by = y

	-- 点の座標の移動
	x = x + sx
	y = y + sy

	-- 箱の回転角度を変更
	rot = rot + ROTSPEED

	-- 一周していたら補正をする
	if rot > PI_2 then rot = rot - PI_2 end

	-- 箱の四隅の座標の初期値をセット
	px[1] = -BOXSIZE
	py[1] = -BOXSIZE
	px[2] = BOXSIZE
	py[2] = -BOXSIZE;
	px[3] = BOXSIZE
	py[3] = BOXSIZE;
	px[4] = -BOXSIZE
	py[4] = BOXSIZE;

	for i = 1, 4 do -- 箱の四隅の座標を現在の回転角度に合わせて回転
		f = px[i] * math.cos(rot) - py[i] * math.sin(rot);
		py[i] = px[i] * math.sin(rot) + py[i] * math.cos(rot);
		px[i] = f;

		-- 回転した後、表示したい座標に移動
		px[i] = px[i] + BOXX
		py[i] = py[i] + BOXY;
	end

	-- 箱の四辺と点との当たり判定(点の移動前と移動後を結ぶ線が箱の四辺と交差したかを判定)
	if HitCheck(px[1], py[1], px[2], py[2], bx, by, x, y) == true or
		HitCheck(px[2], py[2], px[3], py[3], bx, by, x, y) == true or
		HitCheck(px[3], py[3], px[4], py[4], bx, by, x, y) == true or
		HitCheck(px[4], py[4], px[1], py[1], bx, by, x, y) == true then
		-- 当たって(交差して)いたら点の位置を元に戻す
		-- 位置初期化
		x = 320
		y = 240
		-- 速度設定
		sx = math.random() * SPEED - SPEED / 2
		sy = math.random() * SPEED - SPEED / 2;
	end

	-- 箱の四辺を描画
	dx.DrawLine(px[1], py[1], px[2], py[2], dx.GetColor(255, 255, 255));
	dx.DrawLine(px[2], py[2], px[3], py[3], dx.GetColor(255, 255, 255));
	dx.DrawLine(px[3], py[3], px[4], py[4], dx.GetColor(255, 255, 255));
	dx.DrawLine(px[4], py[4], px[1], py[1], dx.GetColor(255, 255, 255));

	-- 点を描画
	dx.DrawPixel(x, y, dx.GetColor(255, 255, 255));

	-- 画面を更新
	dx.ScreenFlip();
end
