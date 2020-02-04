-- マップ表示基本
local MAP_SIZE = 64 -- マップチップ一つのドットサイズ

local MAP_WIDTH = 10 -- マップの幅
local MAP_HEIGHT = 8 -- マップの縦長さ

-- マップのデータ
local MapData =
{
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 0, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 0, 1, 1, 0, 0, 0, 1, 0 },
	{ 0, 1, 1, 1, 1, 0, 0, 0, 1, 0 },
	{ 0, 1, 0, 1, 0, 0, 0, 0, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
}

-- 画面モードのセット
DxLua.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function DxLua.Init()
end

-- ループ
function DxLua.Update()
	-- マップを描く
    for i = 1, MAP_HEIGHT do
		for j = 1, MAP_WIDTH do
            if MapData[i][j] == 0 then
                local x, y = j - 1, i - 1 -- DxLua: Lua の配列のインデックスは 1 からのため調整
				DxLua.DrawBox(x * MAP_SIZE, y * MAP_SIZE,
					x * MAP_SIZE + MAP_SIZE, y * MAP_SIZE + MAP_SIZE,
                    DxLua.GetColor(255, 0, 0), true)
            end
        end
    end

	-- キー入力待ち
    DxLua.WaitKey()

    return 'exit' -- DxLua: ループせずに終了
end
