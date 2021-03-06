-- キーコンフィグ
-- DxLua: KeyConfig.cpp/h のＡＰＩ類をモジュールとして取得
local KeyConfig = require 'KeyConfig'

local KEYCONFIG_FILE_NAME = "KeyConfig.txt" -- キーコンフィグデータのファイル名
local INFO_X = (10) -- 描画開始座標
local INFO_Y = (10)
local INFO_SPACE = (24) -- １行あたりに空けるスペース
local INFO_NAME_X = (140) -- 入力名を描画するＸ座標
local INFO_INPUT_X = (320) -- 入力状態を描画するＸ座標

-- キーコンフィグ対象の項目
local TargetIndex = 1

-- キーコンフィグ各項目の名前
local g_KeyConfigMenuTable =
{
	"左",
	"右",
	"上",
	"下",
	"カメラ左旋回",
	"カメラ右旋回",
	"カメラ上旋回",
	"カメラ下旋回",
	"攻撃",
	"ジャンプ",
}

local DrawY = 0
local InputWait = 0

-- DxLua: 状態が複雑なのでステートマシンで管理する
local StateMachine = {
    State = 'Select',
}

-- 画面モードのセット
dx.SetGraphMode(640, 480, 16)

-- ＤＸライブラリ初期化処理
function dx.Init()
	-- 描画先を裏画面にする
	dx.SetDrawScreen(dx.DX_SCREEN_BACK)
end

-- ループ
function dx.Update(...)
    StateMachine:Update(...)
end

-- 現在の状態に合わせてメソッドを呼び分ける
function StateMachine:Update(...)
	if self[self.State] then
		self[self.State](self, ...)
	end
end

-- 選択画面
function StateMachine:Select()
    -- 画面のクリア
    dx.ClearDrawScreen()

    -- メニュー
    dx.DrawString(0, 0, 'キーコンフィグ選択', dx.GetColor(0xFF, 0xFF, 0))
    dx.DrawString(0, 0, '\n１で設定開始\n２でキーテスト')
    dx.ScreenFlip()

    -- キーボードで選択
    if dx.CheckHitKey(dx.KEY_INPUT_1) ~= 0 or dx.CheckHitKey(dx.KEY_INPUT_NUMPAD1) ~= 0 then
        self.State = 'PreScreen'
    elseif dx.CheckHitKey(dx.KEY_INPUT_2) ~= 0 or dx.CheckHitKey(dx.KEY_INPUT_NUMPAD2) ~= 0 then
        self.State = 'PreTest'
    end
end

-- 共通の前処理
function StateMachine:PreCommon()
	-- キーコンフィグ処理を初期化
	KeyConfig_Initialize()

	-- キーコンフィグファイルを読み込む
	if KeyConfig_Load(KEYCONFIG_FILE_NAME) == false then
		-- コンフィグファイルが読み込めなかったらデフォルト設定にする
		KeyConfig_SetDefault()
    end

	-- 「何か入力があったら何もしない」フラグを倒す
	InputWait = false
end

-- キーコンフィグ画面（前処理）
function StateMachine:PreScreen()
    self:PreCommon()

    TargetIndex = 1
    InputWait = true

    self.State = 'Screen'
end

-- キーコンフィグ画面
function StateMachine:Screen()
    -- 画面のクリア
    dx.ClearDrawScreen()

    -- キーコンフィグの入力処理を行う
    KeyConfig_InputProcess()

    -- 「何か入力があったら何もしない」フラグが立っている場合は処理を分岐
    if InputWait == true then
        -- 何も入力が無かったら「何か入力があったら何もしない」フラグを倒す
        if KeyConfig_CheckInput() == false then
            InputWait = false
        end
    else
        -- キーやパッドの押下状態をチェックして、押下されていたらキーコンフィグ設定に反映させる
        if KeyConfig_UpdateInputTypeInfo(TargetIndex) then
            -- 何かボタンが押されたら次の項目に移動する
            TargetIndex = TargetIndex + 1

            -- 「何か入力があったら何もしない」フラグを立てる
            InputWait = true
        end
    end

    -- 項目の数だけ繰り返し
    DrawY = INFO_Y
    for i = 1, KeyConfig.KEYCONFIG_INPUT_NUM do
        local DrawColor
        local InputString = ''

        -- 変更対象の項目かどうかで描画色を変える
        DrawColor = TargetIndex == i and dx.GetColor(255, 0, 0) or dx.GetColor(255, 255, 255)

        -- 項目名の描画
        dx.DrawString(INFO_X, DrawY, g_KeyConfigMenuTable[i], DrawColor)

        -- 入力に割り当てられている入力名を取得する
        InputString = KeyConfig_GetInputTypeString(i)

        -- 割り当てられている入力名を描画する
        dx.DrawString(INFO_NAME_X, DrawY, InputString, DrawColor)

        -- 項目の描画Y座標を１項目分下に移動する
        DrawY = DrawY + INFO_SPACE
    end

    -- 裏画面の内容を表画面に反映
    dx.ScreenFlip()

    -- 全ての入力の設定が終わったらループを抜ける
    if TargetIndex > KeyConfig.KEYCONFIG_INPUT_NUM then
        -- キーコンフィグ設定を保存する
        KeyConfig_Save(KEYCONFIG_FILE_NAME)

        self.State = 'Select'
    elseif dx.CheckHitKey(dx.KEY_INPUT_ESCAPE) ~= 0 then
        -- DxLua: 戻る
        self.State = 'Select'
    end
end

-- キーコンフィグのテスト（前処理）
function StateMachine:PreTest()
    self:PreCommon()
    self.State = 'Test'
end

-- キーコンフィグのテスト
function StateMachine:Test()
    -- 画面のクリア
    dx.ClearDrawScreen()

    -- キーコンフィグの入力処理を行う
    KeyConfig_InputProcess()

    -- 入力状態を取得する
    Input = KeyConfig_GetInput()

    -- 項目の数だけ繰り返し
    DrawY = INFO_Y
    for i = 1, KeyConfig.KEYCONFIG_INPUT_NUM do
        local InputString = ''

        -- 項目名の描画
        dx.DrawString(INFO_X, DrawY, g_KeyConfigMenuTable[i], dx.GetColor(255, 255, 255))

        -- 入力に割り当てられている入力名を取得する
        InputString = KeyConfig_GetInputTypeString(i)

        -- 割り当てられている入力名を描画する
        dx.DrawString(INFO_NAME_X, DrawY, InputString, dx.GetColor(255, 255, 255))

        -- 入力状態を描画する
        dx.DrawString(INFO_INPUT_X, DrawY, bit.band(Input, bit.lshift(1, i)) ~= 0 and "○" or "＿", dx.GetColor(255, 255, 255))

        -- 項目の描画Y座標を１項目分下に移動する
        DrawY = DrawY + INFO_SPACE
    end

    -- カメラ用方向入力の状態を描画する
    CameraInput = KeyConfig_GetCameraInput()
    dx.DrawFormatString(INFO_X, DrawY, dx.GetColor(255, 255, 255), "カメラ入力  X:%.3f  Y:%.3f", CameraInput.x, CameraInput.y)

    -- 裏画面の内容を表画面に反映
    dx.ScreenFlip()

    if dx.CheckHitKey(dx.KEY_INPUT_ESCAPE) ~= 0 then
        -- DxLua: 戻る
        self.State = 'Select'
    end
end
