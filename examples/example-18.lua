-- 格ゲーコマンド入力基本
local band, bnot, bor = bit.band, bit.bnot, bit.bor

local INPUT_MAX_BUF = 256 -- 入力データの最大記憶数
local INPUT_VALID_TIME = 500 -- 入力データが無効になるまでの時間
local INPUT_DOUJI_TIME = 10 -- 同時押しの範囲となる時間
local SKILL_DRAWTIME = 1000 -- 技名を表示している時間

-- メインデータ
NowTime = NowTime or 0 -- 現在の時間

DrawTime = DrawTime or 0 -- 技名ののこり表示時間
DrawSkillName = DrawSkillName or "" -- 表示する技名

OldInputData = OldInputData or 0 -- ひとつ前のキー入力データ
InputBuf = InputBuf or {} -- 入力データのログ
InputTime = InputTime or 0 -- 入力が行われた時間
InputNum = InputNum or 1 -- 入力されたコマンドの数

-- 技名データ
local SkillName =
{
	"波動拳",
	"昇竜拳",
	"昇竜拳",
}

-- 技データ
local SkillCommand =
{
	-- 波動拳
	{
		DxLua.PAD_INPUT_DOWN,
		bor(DxLua.PAD_INPUT_RIGHT, DxLua.PAD_INPUT_DOWN),
		DxLua.PAD_INPUT_RIGHT,
	},

	-- 昇竜拳
	{
		DxLua.PAD_INPUT_RIGHT,
		bor(DxLua.PAD_INPUT_RIGHT, DxLua.PAD_INPUT_DOWN),
		DxLua.PAD_INPUT_DOWN,
		bor(DxLua.PAD_INPUT_RIGHT, DxLua.PAD_INPUT_DOWN),
	},
	{
		DxLua.PAD_INPUT_RIGHT,
		DxLua.PAD_INPUT_DOWN,
		bor(DxLua.PAD_INPUT_RIGHT, DxLua.PAD_INPUT_DOWN),
	},
}

OldTime = OldTime or 0 -- 前フレーム時の時間

DxLua.SetGraphMode(640, 480, 16) -- 画面設定

-- ＤＸライブラリ初期化処理
function DxLua.Init()
	-- 描画先を裏画面に設定
	DxLua.SetDrawScreen(DxLua.DX_SCREEN_BACK)

	-- 時間の取得
	NowTime = DxLua.GetNowCount()
end

-- ループ
function DxLua.Update()
	if band(DxLua.GetJoypadInputState(DxLua.DX_INPUT_KEY_PAD1), DxLua.PAD_INPUT_START) ~= 0 then
		return 'exit'
	end

	-- コマンド入力処理の実行
	CommandInput()

	-- 画面を初期化
	DxLua.ClearDrawScreen()

	-- 技名の表示
	if DrawTime > 0 then
		DxLua.DrawString(200, 200, DrawSkillName, 0xffffff)

		-- 残り表示時間を減らす
		DrawTime = DrawTime - (NowTime - OldTime)
	end

	-- DxLua: デバッグ用
	for i, command in ipairs(InputBuf) do
		DxLua.DrawString(200 + i * 20, 300, tostring(command), 0xffffff)
	end

	-- 裏画面の内容を表画面に反映させる
	DxLua.ScreenFlip()

	OldTime = NowTime		-- 前フレームの時間を保存
	NowTime = DxLua.GetNowCount()	-- 現在の時間を取得
end

-- キー入力処理を行う
function CommandInput()
	local InputData, OldCursorData, CursorData

	-- 現在の入力状態を取得
	InputData = DxLua.GetJoypadInputState(DxLua.DX_INPUT_KEY_PAD1)

	-- カーソルキー入力のみのデータにする
	CursorData = InputData
	CursorData = band(
		CursorData,
		bor(DxLua.PAD_INPUT_LEFT, DxLua.PAD_INPUT_RIGHT, DxLua.PAD_INPUT_UP, DxLua.PAD_INPUT_DOWN)
	)

	-- 前回のキー入力からカーソルキー入力のみを抽出
	OldCursorData = OldInputData
	OldCursorData = band(
		OldCursorData,
		bor(DxLua.PAD_INPUT_LEFT, DxLua.PAD_INPUT_RIGHT, DxLua.PAD_INPUT_UP, DxLua.PAD_INPUT_DOWN)
	)

	-- 前フレームとキー入力に違いがあり、更にカーソルキーを押していた場合のみ記録する
	if CursorData ~= OldCursorData and CursorData ~= 0 then
		-- もし以前とのキー入力時間との差が同時押し判定時間内だった場合は以前のキー入力データに加算する
		if (InputNum > 1) and (NowTime - InputTime < INPUT_DOUJI_TIME) then
			InputBuf[InputNum - 1] = bor(InputBuf[InputNum - 1], CursorData)
		else
			-- バッファにキー入力状態を保存する
			InputBuf[InputNum] = CursorData

			-- 入力時間を残す
			InputTime = NowTime

			-- 入力コマンドの数を増やす
			InputNum = InputNum + 1
		end
	end

	-- 何もしていない時間が規定以上経過していたら入力データをクリアする
	if (InputTime ~= 0 and NowTime - InputTime > INPUT_VALID_TIME) then
		InitInputData()
	end

	-- もしＺボタンが押されていたら対応した技を検索する
	if band(band(InputData, bnot(CursorData), bnot(OldInputData)), DxLua.PAD_INPUT_A) ~= 0 then
		local SkillNum

		-- 対応している技を取得
		SkillNum = SkillCheck()

		-- もし何の技も出していなければなにもしない
		if SkillNum ~= -1 then
			-- 技の名前をセット
			DrawSkillName = SkillName[SkillNum]

			-- 表示時間をセット
			DrawTime = SKILL_DRAWTIME
		end

		-- 入力データを初期化
		InitInputData()
	end

	-- 今フレームのキー入力状態を保存
	OldInputData = InputData
end

-- インプットデータを初期化する
function InitInputData()
	local i

	-- キー入力データをクリア
	InputBuf = {}

	-- 入力時間もクリア
	InputTime = 0

	-- 入力されたコマンドの数を初期化
	InputNum = 1
end


-- 入力したコマンドに対応した技を検索する
function SkillCheck()
	local Skill, MaxConbo, i, j

	-- 出す技を探す
	MaxConbo = 0
	-- DxLua: イテレータ関数を利用して分かりやすく変更
	for i, skill in ipairs(SkillCommand) do
		-- コマンドデータと入力したデータが等しい数を調べる
		local index = -1
		for j, command in ipairs(skill) do
			if command ~= InputBuf[j] then
				break
			else
				index = j
			end
		end

		-- コマンドデータの終端まで同じだった場合は
		-- その技のコマンド数と技ナンバーを保存する
		-- なお以前にもっと多くのコマンドを要す技と適合していたら記憶しない
		if index == #skill and MaxConbo < index then
			MaxConbo = index
			Skill = i
		end
	end

	-- 合った技が無ければ－１を返す
	if MaxConbo == 0 then return -1 end

	-- 選ばれた技ナンバーを返す
	return Skill
end
