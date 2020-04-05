-- サウンドノベル風文字列描画、テキストバッファ使用バージョン
-- サウンドノベル風文字列描画、半角文字対応＋テキストバッファ使用バージョン
local band, bnot = bit.band, bit.bnot

-- 文字のサイズ
local MOJI_SIZE = 24

-- 仮想テキストバッファの横サイズ縦サイズ
local STRBUF_WIDTH = 48
local STRBUF_HEIGHT = 20

StringBuf = StringBuf or {}	-- 仮想テキストバッファ
CursorX, CursorY = 0, 1 -- 仮想画面上での文字表示カーソルの位置
EndFlag = 0 -- 終了フラグ
KeyWaitFlag = 0 -- ボタン押し待ちフラグ
Count = 0 -- フレームカウンタ

local String =
	"　ｹﾞｰﾑﾌﾟﾛｸﾞﾗﾑを習得するための一番の近道はとにかく沢山ﾌﾟﾛｸﾞﾗﾑを組む" ..
	"ことである。B" ..
	"@　ﾌﾟﾛｸﾞﾗﾑの参考書にはゲームのﾌﾟﾛｸﾞﾗﾑの方法なんて何も書かれていない、B" ..
	"変数、B配列、B関数、Bﾙｰﾌﾟ、B条件分岐…Bこれらすべての説明はゲームで何に使うか" ..
	"なんてどこにも書いていない、Bせいぜい住所録を題材にした例がある程度である。B" ..
	"C　ﾌﾟﾛｸﾞﾗﾑは習うより慣れろなのでﾌﾟﾛｸﾞﾗﾑを組むに当たって少しでも知識が" ..
	"つけば後はそこからは掘り下げ、広げていけば良いわけで、Bﾌﾟﾛｸﾞﾗﾑの参考書を" ..
	"読んでいて少しでも何か出来るような気がしたらそこでとにかくﾌﾟﾛｸﾞﾗﾑ" ..
	"を打ってみることが大事である。E"

-- DxLua: イテレータ関数
local chars

OneMojiBuf = '' -- １文字分一時記憶配列

dx.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function dx.Init()
	-- 描画位置の初期位置セット
	CursorX = 0
	CursorY = 1

	-- フォントのサイズセット
	dx.SetFontSize(MOJI_SIZE)

	-- 描画先を裏画面にセット
	dx.SetDrawScreen(dx.DX_SCREEN_BACK)

	-- フレームカウンタ初期化
	Count = 0

    -- DxLua: UTF-8 文字を１文字ずつ返すイテレータ関数を使う
    chars = String:gmatch("[%z\1-\127\194-\244][\128-\191]*")
end

-- ループ
function dx.Update()
	if dx.CheckHitKey(dx.KEY_INPUT_ESCAPE) ~= 0 then
		return 'exit'
	end

	-- サウンドノベル風文字列描画処理を行う
	-- ただし終了フラグが１だった場合は処理をしない
	if (EndFlag == 0) then
		local  Moji

		-- ボタン押し待ちフラグがたっていた場合はボタンが押されるまでここで終了
		if (KeyWaitFlag == 1) then
			if (dx.ProcessMessage() == 0 and dx.CheckHitKeyAll() ~= 0) then
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

				-- ボタンが離されるまで待つ
				while (dx.ProcessMessage() == 0 and dx.CheckHitKeyAll() ~= 0) do end

				-- ボタン押し待ちフラグをたてる
				KeyWaitFlag = 1

			elseif Moji == 'E' then	-- 終了文字

				-- 終了フラグを立てるおよび参照文字位置を一つ進める
				EndFlag = 1

			elseif Moji == 'C' then	-- クリア文字

				-- 仮想テキストバッファを初期化して描画文字位置を初期位置に戻すおよび参照文字位置を一つ進める
				StringBuf = {}

				CursorY = 1
				CursorX = 0

			else	-- その他の文字

				-- １文字のバイト数を取得
				-- DxLua: 半角文字なら幅１，全角文字なら２とする
				local Width = 1
				if #Moji == 1 then
					-- ASCII の範囲
					Width = 1
				elseif Moji:match('\239\189[\165-\191]') then
					-- 半角カナ（前半）
					Width = 1
				elseif Moji:match('\239\190[\128-\159]') then
					-- 半角カナ（後半）
					Width = 1
				else
					-- その他は全角と見做す
					Width = 2
				end

				-- １文字分抜き出す
				OneMojiBuf = Moji

				-- １文字テキストバッファに代入
				-- DxLua: 現在の行に追加
				if not StringBuf[CursorY] then
					StringBuf[CursorY] = ''
				end
				StringBuf[CursorY] = StringBuf[CursorY] .. OneMojiBuf

				-- カーソルを一文字分進める
				CursorX = CursorX + Width

				-- テキストバッファ横幅からはみ出たら改行する
				if CursorX >= STRBUF_WIDTH then Kaigyou() end
			end
		end
	end

	-- 画面のクリア
	dx.ClearDrawScreen()

	-- 背景エフェクトの描画
	do
		local Color

		Color = math.floor(math.sin(Count / 100.0) * 80.0 + 125.0)
		dx.DrawBox(0, 0, 640, 480, dx.GetColor(Color, 0, 0), true)
		Count = Count + 1
	end

	-- テキストバッファの描画
	for i, line in ipairs(StringBuf) do
		dx.DrawString(8, (i - 1) * MOJI_SIZE, line, dx.GetColor(255, 255, 255))
	end

	-- 裏画面の内容を表画面に反映させる
	dx.ScreenFlip()
end


-- 改行関数
function Kaigyou()
	-- 描画行位置を一つ下げる
	CursorY = CursorY + 1

	-- 描画列を最初に戻す
	CursorX = 0

	-- もしテキストバッファ縦幅からはみ出るならテキストバッファを縦スクロールさせる
	if CursorY >= STRBUF_HEIGHT then
		for i = 2, STRBUF_HEIGHT do
			StringBuf[i - 1] = StringBuf[i]
		end

		-- 描画行位置を一つあげる
		CursorY = CursorY - 1
	end
end
