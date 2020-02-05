-- サウンドノベル風文字列描画基本

-- 文字のサイズ
local MOJI_SIZE = 24

local DrawPointX, DrawPointY -- 文字列描画の位置

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
		TempGraph = DxLua.MakeGraph(640, 480)

		-- 画面の内容を丸々コピーする
		DxLua.GetDrawScreenGraph(0, 0, 640, 480, TempGraph)

		-- 一行分上に貼り付ける
		DxLua.DrawGraph(0, -MOJI_SIZE, TempGraph, false)

		-- 一番下の行の部分を黒で埋める
		DxLua.DrawBox(0, 480 - MOJI_SIZE, 640, 480, 0, true)

		-- 描画行位置を一つあげる
		DrawPointY = DrawPointY - 1

		-- グラフィックを削除する
		DxLua.DeleteGraph(TempGraph)
    end

	-- 終了
	return 0
end

local EndFlag -- 終了フラグ

-- DxLua: イテレータ関数
local chars

-- 画面モードのセット
DxLua.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function DxLua.Init()
	-- 描画位置の初期位置セット
	DrawPointX = 0
	DrawPointY = 0

	-- フォントのサイズセット
	DxLua.SetFontSize(MOJI_SIZE)

	-- 終了フラグを倒す
    EndFlag = 0

    -- DxLua: UTF-8 文字を１文字ずつ返すイテレータ関数を使う
    chars = String:gmatch("[%z\1-\127\194-\244][\128-\191]*")
end

-- ループ
function DxLua.Update()
    -- 文字の描画
    local Moji = chars()

    -- DxLua: 次の文字が無ければ nil が返るので終了する
    if not Moji then
        DxLua.WaitKey()
        return 'exit'
    end

    -- DxLua: Lua に switch はない
    if Moji == '@' then -- 改行文字
        -- 改行処理および参照文字位置を一つ進める
        Kaigyou()

    elseif Moji == 'B' then -- ボタン押し待ち文字
        -- ボタン押し待ちおよび参照文字位置を一つ進める
        DxLua.WaitKey()

    elseif Moji == 'E' then -- 終了文字
        -- 終了フラグを立てるおよび参照文字位置を一つ進める
        EndFlag = 1

    elseif Moji == 'C' then	-- クリア文字
        -- 画面を初期化して描画文字位置を初期位置に戻すおよび参照文字位置を一つ進める
        DxLua.ClearDrawScreen()
        DrawPointY = 0
        DrawPointX = 0

    else -- その他の文字
        -- １文字描画
        DxLua.DrawString(DrawPointX * MOJI_SIZE , DrawPointY * MOJI_SIZE,
            Moji, DxLua.GetColor(255 , 255 , 255))

        -- カーソルを一文字分進める
        DrawPointX = DrawPointX + 1

        -- 少し待つ
        DxLua.WaitTimer(10)

        -- 画面からはみ出たら改行する
        if DrawPointX * MOJI_SIZE + MOJI_SIZE > 640 then Kaigyou() end
    end

    -- 終了フラグが１だったら終了する
    if EndFlag == 1 then return 'exit' end
end
