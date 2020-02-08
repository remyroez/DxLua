-- メニュ－処理基本

local SPoint -- 選択カーソルの位置

-- 処理ポインタ配列
-- DxLua: 後で追加
local Method

-- 選択項目の文字列
local String =
{
	"赤い箱を描画する",
	"ランダムにドットを打つ",
	"グラデーションを描く",
}

-- 画面モードのセット
DxLua.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function DxLua.Init()
	-- 選択ポインタの位置初期化
	SPoint = 1

    -- 選択項目の数を取得＆ついでに項目描画
    -- DxLua: イテレータ関数で取得
	SenntakuNum = #String
	for Index, Menu in ipairs(String) do
		DxLua.DrawString(32, (Index - 1) * 32, Menu, DxLua.GetColor(255, 255, 255))
    end
end

-- ループ
function DxLua.Update()
    -- 前のカーソル消去
    DxLua.DrawBox(0, 0, 32, SenntakuNum * 32, 0, true)

    -- カーソル描画
    DxLua.DrawBox(0, (SPoint - 1) * 32, 16, (SPoint - 1) * 32 + 16,
        DxLua.GetColor(255, 255, 0), true)

    -- キー入力
    do
        -- キー入力待ち
        while (DxLua.ProcessMessage() == 0 and DxLua.CheckHitKeyAll() ~= 0) do end
        while (DxLua.ProcessMessage() == 0 and DxLua.CheckHitKeyAll() == 0) do end

        -- キー入力に応じて処理

        -- カーソル移動
        if (DxLua.CheckHitKey(DxLua.KEY_INPUT_UP) ~= 0 and SPoint > 1) then SPoint = SPoint - 1 end
        if (DxLua.CheckHitKey(DxLua.KEY_INPUT_DOWN) ~= 0 and SPoint < SenntakuNum) then SPoint = SPoint + 1 end

        -- 決定キー時処理
        if DxLua.CheckHitKey(DxLua.KEY_INPUT_Z) ~= 0 then
            -- 項目に応じた処理を行う
            Method[SPoint]()

            -- キー入力待ち
            DxLua.WaitKey()

            -- ループから抜ける
            return 'exit'
        end
    end
end

-- 赤い箱を描画する関数
function RedBoxDraw()
	DxLua.DrawBox(0, 0, 640, 480, DxLua.GetColor(255, 0, 0), true)
end

-- ランダムにドットを打つ関数
function RandPsetPixel()
	for i = 1, 1000 do
		DxLua.DrawPixel(DxLua.GetRand(639), DxLua.GetRand(479),
            DxLua.GetColor(DxLua.GetRand(255), DxLua.GetRand(255), DxLua.GetRand(255)))
    end
end

-- グラデーションを描く関数
function GDraw()
	for i = 1, 480 do
		DxLua.DrawLine(0, i - 1, 640, i - 1, DxLua.GetColor(255, 255, 255 * i / 480))
    end
end

-- DxLua: ここで追加
Method =
{
	RedBoxDraw, RandPsetPixel, GDraw
}
