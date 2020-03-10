-- セーブ・ロード基本
local band, bnot = bit.band, bit.bnot

-- ボタン押しカウンター
local Counter = 0
local HiScore = 0

-- 画面モードのセット
dx.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function dx.Init()
	-- ハイスコアをロード
	HiScoreLoad()
end

-- ループ
function dx.Update()
	-- 連射計測処理
	Rennsya()

	-- 時間待ち
	dx.WaitTimer(1000)

	-- キー入力待ち
	dx.WaitKey()
end

-- 連射計測関数
function Rennsya()
	local Color	-- 文字の色コード
	local StartTime	-- 連射スタート時間
	local OldKey	-- 前回のキー入力状態
	local Key	-- 現在のキー入力状態
	local OldTime	-- 前フレームの経過時間
	local NowTime	-- 現在の経過時間

	-- 白色コードを取得
	Color = dx.GetColor(255, 255, 255)

	-- 画面構成を創る
	do
		-- 画面初期化
		dx.ClearDrawScreen()

		-- メッセージ表示
		dx.DrawString(0, 0, "ジョイパッドのＡボタンか、Ｚキーを連射してください", Color)

		-- カウント値表示
		dx.DrawString(10, 120, "COUNT", Color)
		DrawNum(100, 120, 0)

		-- ハイスコア表示
		dx.DrawString(100, 30, "HI SCORE", Color)
		DrawNum(200, 30, HiScore)

		-- 残り秒数の表示
		dx.DrawString(10, 80, "TIME", Color)
		DrawNum(100, 80, 10)
	end

	-- キー入力待ち
	dx.WaitKey()

	StartTime = dx.GetNowCount()	-- 連射開始時の時間を保存
	Counter = 0			-- 連射カウンターを初期化
	OldKey = 0			-- 前回のキー入力を初期化
	NowTime = 0			-- 現在の経過時間初期化
	OldTime = 0			-- 前回の経過時間を初期化

	-- １０秒経つまで連射計測処理
	while dx.ProcessMessage() == 0 and NowTime < 10000 do
		-- 今回の経過タイムを取得
		NowTime = dx.GetNowCount() - StartTime

		-- キー入力を取得
		Key = dx.GetJoypadInputState(dx.DX_INPUT_KEY_PAD1)

		-- もし前回押していない状態で今回押していたらカウンタをインクリメントする
		if band(band(Key, bnot(OldKey)), dx.PAD_INPUT_A) ~= 0 then
			Counter = Counter + 1

			-- カウンタ値を表示
			DrawNum(100, 120, Counter)
		end

		-- 前回の残り秒数と今回の残り秒数が違った場合は残り秒数を更新
		if math.floor(NowTime / 1000) ~= math.floor(OldTime / 1000) then
			DrawNum(100, 80, 10 - math.floor(NowTime / 1000))

			-- 今回のタイムを保存
			OldTime = NowTime
		end

		-- 今回のキー入力状態を保存
		OldKey = Key
	end

	-- もしハイスコア以上の値が出ていたらハイスコア変数にカウント値を代入
	if HiScore < Counter then
		HiScore = Counter

		-- ハイスコアのセーブ
		HiScoreSave()

		-- ハイスコア表示
		DrawNum(200, 30, HiScore)
	end
end

-- ハイスコアのセーブ
function HiScoreSave()
	-- ハイスコアセーブ用ファイルを開く
	-- (２番目の引数の"wb"の'w'は「書きこみ」の意味
	-- 'b'はバイナリの意味(逆はテキスト))
	local fp, err = io.open("HiScore.dat", "wb")

	-- オープンできなかったらここで終了
	if not fp then
		print(err)
		return
	end

	-- ハイスコアデータの書き出し
	fp:write(HiScore)

	-- ファイルを閉じる
	fp:close()
end

-- ハイスコアのロード
function HiScoreLoad()
	-- ハイスコアセーブ用ファイルを開く
	-- (２番目の引数の"rb"の'r'は「読み込み」の意味
	-- 'b'はバイナリの意味(逆はテキスト))
	local fp = io.open("HiScore.dat", "rb")

	-- オープンできなかったらファイルが無いとみなし
	-- 標準ハイスコアの５０をセット
	if not fp then
		HiScore = 50
	else
		-- ハイスコアデータの読み込み
		HiScore = fp:read("*n")

		-- ファイルを閉じる
		fp:close()
	end
end

-- 数値を画面に表示する関数
function DrawNum(x, y, Num)
	local StrBuf

	-- 数値を１０進数文字列に変換
	StrBuf = tostring(Num)

	-- 画面に描画
	dx.DrawBox(x, y, x + #StrBuf * 20, y + 20, 0, true)
	dx.DrawString(x, y, StrBuf, dx.GetColor(255, 255, 255))
end
