-- マルチタスク風処理基本
local band, bnot = bit.band, bit.bnot

local FrameTime = 0 -- 前回フレームでの経過時間

-- サウンドノベル風文字列描画プログラム用データ群

-- 文字のサイズ
local MOJI_SIZE = 24

local DrawPointX, DrawPointY = 0, 0 -- 文字列描画の位置
local TimeCounter1 = 0 -- サウンドノベル風文字列描画処理用の時間計測用カウンタ変数
local EndFlag = 0 -- 終了フラグ
local KeyWaitFlag = 0 -- ボタン押し待ちフラグ

local String =
	"　ゲームプログラムとは、いやプログラムとは" ..
	"ある事柄を実現するプログラムの方法を説明されても理解できないことがある。B" ..
	"@　なぜならそのプログラム技法も何かの基本的な技法の組み合わせで出来ているからだ。B" ..
	"@　これはその他の学問も基本がわからないと応用が利かないということと同じ現象で、" ..
	"別に特に珍しいことでもない。B" ..
	"C　しかしゲームプログラムとなると覚えなくてはならない基礎が沢山あり、" ..
	"さらにある程度クオリティの高いソフトを作ろうとすると色々なプログラム技法を" ..
	"習得しなくてはならない。B" ..
	"@　しかもある程度レベルが高くなると自分で技法を編み出すか、技術レベルの高い" ..
	"プログラマーに聞くなどするしか方法がなく大変厄介である。B" ..
	"というかそのせいでゲームプログラムの敷居は高くなっているといえる。BE"

-- マップ移動処理プログラム用データ群
local MAP_SIZE = 64			-- マップチップ一つのドットサイズ

local MAP_WIDTH = 20			-- マップの幅
local MAP_HEIGHT = 16			-- マップの縦長さ

-- マップのデータ
local MapData =
{
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,  1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 0, 1, 1, 1, 0, 0, 0, 0, 0,  1, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 0, 1, 0, 1, 0, 0, 0, 0, 0,  1, 1, 1, 1, 1, 1, 0, 0, 1, 0 },
	{ 0, 1, 1, 0, 1, 0, 0, 1, 1, 0,  0, 0, 0, 0, 0, 1, 0, 0, 1, 0 },
	{ 0, 1, 0, 0, 1, 0, 0, 1, 1, 0,  0, 0, 1, 1, 1, 1, 1, 0, 1, 0 },
	{ 0, 1, 0, 0, 1, 0, 0, 1, 1, 0,  0, 0, 1, 0, 0, 1, 1, 0, 1, 0 },
	{ 0, 1, 0, 0, 1, 0, 0, 1, 1, 0,  0, 0, 1, 0, 0, 1, 1, 0, 1, 0 },

	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 0,  0, 1, 1, 0, 0, 1, 1, 0, 1, 0 },
	{ 0, 0, 0, 0, 1, 0, 0, 1, 0, 0,  0, 1, 1, 0, 0, 1, 1, 0, 1, 0 },
	{ 0, 0, 0, 0, 1, 0, 0, 1, 0, 0,  0, 1, 1, 0, 0, 1, 1, 0, 1, 0 },
	{ 0, 0, 0, 1, 1, 0, 0, 1, 0, 0,  0, 1, 1, 0, 0, 1, 1, 0, 1, 0 },
	{ 0, 1, 1, 1, 1, 0, 0, 1, 1, 1,  1, 1, 1, 0, 0, 1, 1, 0, 1, 0 },
	{ 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
}

-- プレイヤーの位置
local PlayerX, PlayerY = 0, 0

-- マップ移動処理用の時間計測用カウンタ変数
local TimeCounter2 = 0

local OldTime, NowTime = 0, 0

-- DxLua: イテレータ関数
local chars

dx.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function dx.Init()
	-- マップ移動プログラム関連データの初期化
	do
		-- マップ移動キャラのプレイヤーの初期位置をセット
		PlayerX = 3
		PlayerY = 3
	end

	-- サウンドノベル風文字列描画プログラム関連データの初期化
	do
		-- 描画位置の初期位置セット
		DrawPointX = 0
		DrawPointY = 0

		-- フォントのサイズセット
		dx.SetFontSize(MOJI_SIZE)

		-- 終了フラグを倒す
		EndFlag = 0
	end

	-- 現在時間を初期化
	NowTime = dx.GetNowCount()

    -- DxLua: UTF-8 文字を１文字ずつ返すイテレータ関数を使う
    chars = String:gmatch("[%z\1-\127\194-\244][\128-\191]*")
end

-- ループ
function dx.Update()
	if dx.CheckHitKey(dx.KEY_INPUT_ESCAPE) ~= 0 then
		return 'exit'
	end

	-- マップ移動処理をする(画面左側のみ描画出来るようにする)
	dx.SetDrawArea(0, 0, 320, 480)
	MapShred()

	-- サウンドノベル風文字列描画処理を行う(画面右側のみ描画出きるようにする)
	dx.SetDrawArea(320, 0, 640, 480)
	StringShred()

	-- 時間待ち
	dx.WaitTimer(17)

	-- 今フレームでの経過時間を計測
	OldTime = NowTime
	NowTime = dx.GetNowCount()
	FrameTime = NowTime - OldTime
end

-- マップ移動プログラムの処理関数
function MapShred()
	local Key
	local OldX, OldY	-- 移動する前のプレイヤーの位置を保存する変数

	-- タイムカウンターを経過時間分だけ減らす
	TimeCounter2 = TimeCounter2 - FrameTime

	-- 時間計測用変数が０以上だった場合はキー入力をしない
	if (TimeCounter2 < 0) then
		-- キー入力を得る
		Key = dx.GetJoypadInputState(dx.DX_INPUT_KEY_PAD1)

		-- 移動する前のプレイヤーの位置を保存
		OldX = PlayerX
		OldY = PlayerY

		-- キー入力に応じてプレイヤーの座標を移動
		if band(Key, dx.PAD_INPUT_LEFT) ~= 0 then PlayerX = PlayerX - 1 end
		if band(Key, dx.PAD_INPUT_RIGHT) ~= 0 then PlayerX = PlayerX + 1 end
		if band(Key, dx.PAD_INPUT_UP) ~= 0 then PlayerY = PlayerY - 1 end
		if band(Key, dx.PAD_INPUT_DOWN) ~= 0 then PlayerY = PlayerY + 1 end

		-- 進入不可能なマップだった場合は移動できない
		if (MapData[PlayerY][PlayerX] == 0) then
			PlayerX = OldX
			PlayerY = OldY
		end

		-- マップとプレイヤーの描画
		do
			local MapDrawPointX, MapDrawPointY		-- 描画するマップ座標値
			local DrawMapChipNumX, DrawMapChipNumY	-- 描画するマップチップの数

			-- 画面を初期化
			dx.DrawBox(0, 0, 640, 480, 0, true)

			-- 描画するマップチップの数をセット
			DrawMapChipNumX = math.floor(320 / MAP_SIZE)
			DrawMapChipNumY = math.floor(480 / MAP_SIZE)

			-- 画面左上に描画するマップ座標をセット
			MapDrawPointX = PlayerX - math.floor(DrawMapChipNumX / 2)
			MapDrawPointY = PlayerY - math.floor(DrawMapChipNumY / 2)

			-- マップを描く
			for i = 1, DrawMapChipNumY do
				for j = 1, DrawMapChipNumX do
					-- DxLua: テーブルの添字は１始まりなので調整
					local x, y = j - 1, i - 1

					-- 画面からはみ出た位置なら描画しない
					if x + MapDrawPointX < 1 or y + MapDrawPointY < 1 or
						x + MapDrawPointX > MAP_WIDTH or y + MapDrawPointY > MAP_HEIGHT then
						-- DxLua: Lua に continue は無い
					-- マップデータが０だったら四角を描画する
					elseif MapData[y + MapDrawPointY][x + MapDrawPointX] == 0 then
						dx.DrawBox(x * MAP_SIZE, y * MAP_SIZE,
							x * MAP_SIZE + MAP_SIZE, y * MAP_SIZE + MAP_SIZE,
							dx.GetColor(255, 0, 0), true)
					end
				end
			end

			-- プレイヤーの描画
			dx.DrawBox((PlayerX - MapDrawPointX) * MAP_SIZE, (PlayerY - MapDrawPointY) * MAP_SIZE,
				(PlayerX - MapDrawPointX + 1) * MAP_SIZE, (PlayerY - MapDrawPointY + 1) * MAP_SIZE,
				dx.GetColor(255, 255, 255), true)
		end

		-- 時間計測用変数に待ち時間を代入
		TimeCounter2 = 100
	end
end

-- サウンドノベル風文字列描画プログラムの処理関数
function StringShred()
	local OneMojiBuf	-- １文字分一時記憶配列

	-- 時間計測用変数の値を１減らす
	TimeCounter1 = TimeCounter1 - 1

	-- 時間計測用変数が０以上か、終了フラグが１だった場合は処理をしない
	if TimeCounter1 < 0 and EndFlag == 0 then
		local  Moji

		-- ボタン押し待ちフラグがたっていた場合はボタンが押されるまでここで終了
		if KeyWaitFlag == 1 then
			if dx.CheckHitKeyAll() ~= 0 then
				-- ボタンが押されていたら解除
				KeyWaitFlag = 0
			end
		else
			-- 文字の描画
			-- DxLua: イテレータ関数が次の文字を返す
			Moji = chars()
			if not Moji then
				-- DxLua: もう文字がない
			elseif Moji == '@' then	-- 改行文字
				-- 改行処理および参照文字位置を一つ進める
				Kaigyou()

			elseif Moji == 'B' then	-- ボタン押し待ち文字
				-- ボタン押し待ちフラグをたてる
				KeyWaitFlag = 1

			elseif Moji == 'E' then	-- 終了文字
				-- 終了フラグを立てるおよび参照文字位置を一つ進める
				EndFlag = 1

			elseif Moji == 'C' then	-- クリア文字
				-- 画面を初期化して描画文字位置を初期位置に戻すおよび参照文字位置を一つ進める
				dx.DrawBox(0, 0, 640, 480, 0, true)
				DrawPointY = 0
				DrawPointX = 0

			else	-- その他の文字
				-- １文字分抜き出す
				OneMojiBuf = Moji

				-- １文字描画
				dx.DrawString(320 + DrawPointX * MOJI_SIZE, DrawPointY * MOJI_SIZE,
					OneMojiBuf, dx.GetColor(255, 255, 255))

				-- カーソルを一文字分進める
				DrawPointX = DrawPointX + 1

				-- 画面からはみ出たら改行する
				if (DrawPointX * MOJI_SIZE + MOJI_SIZE > 320) then Kaigyou() end
			end
		end

		-- 時間計測用変数の値をセットする
		TimeCounter1 = 2
	end
end

-- 改行関数
function Kaigyou()
	local TempGraph

	-- 描画行位置を一つ下げる
	DrawPointY = DrawPointY + 1

	-- 描画列を最初に戻す
	DrawPointX = 0

	-- もし画面からはみ出るなら画面をスクロールさせる
	if DrawPointY * MOJI_SIZE + MOJI_SIZE > 480 then
		-- テンポラリグラフィックの作成
		TempGraph = dx.MakeGraph(320, 480)

		-- 画面の内容を丸々コピーする
		dx.GetDrawScreenGraph(320, 0, 640, 480, TempGraph)

		-- 一行分上に貼り付ける
		dx.DrawGraph(320, -MOJI_SIZE, TempGraph, false)

		-- 一番下の行の部分を黒で埋める
		dx.DrawBox(320, 480 - MOJI_SIZE, 640, 480, 0, true)

		-- 描画行位置を一つあげる
		DrawPointY = DrawPointY - 1

		-- グラフィックを削除する
		dx.DeleteGraph(TempGraph)
	end
end
