-- モジュール用テーブル
local KeyConfig = {}

-- 入力情報タイプ
KeyConfig.KEYCONFIG_INPUT_LEFT = (0) -- 方向入力左
KeyConfig.KEYCONFIG_INPUT_RIGHT = (1) -- 方向入力右
KeyConfig.KEYCONFIG_INPUT_UP = (2) -- 方向入力上
KeyConfig.KEYCONFIG_INPUT_DOWN = (3) -- 方向入力下
KeyConfig.KEYCONFIG_INPUT_CAMERA_LEFT = (4) -- カメラ用方向入力左
KeyConfig.KEYCONFIG_INPUT_CAMERA_RIGHT = (5) -- カメラ用方向入力右
KeyConfig.KEYCONFIG_INPUT_CAMERA_UP = (6) -- カメラ用方向入力上
KeyConfig.KEYCONFIG_INPUT_CAMERA_DOWN = (7) -- カメラ用方向入力下
KeyConfig.KEYCONFIG_INPUT_ATTACK = (8) -- 攻撃ボタン
KeyConfig.KEYCONFIG_INPUT_JUMP = (9) -- ジャンプボタン
KeyConfig.KEYCONFIG_INPUT_NUM = (10) -- 入力タイプの数

local KEYCONFIG_INPUT_LEFT = KeyConfig.KEYCONFIG_INPUT_LEFT
local KEYCONFIG_INPUT_RIGHT = KeyConfig.KEYCONFIG_INPUT_RIGHT
local KEYCONFIG_INPUT_UP = KeyConfig.KEYCONFIG_INPUT_UP
local KEYCONFIG_INPUT_DOWN = KeyConfig.KEYCONFIG_INPUT_DOWN
local KEYCONFIG_INPUT_CAMERA_LEFT = KeyConfig.KEYCONFIG_INPUT_CAMERA_LEFT
local KEYCONFIG_INPUT_CAMERA_RIGHT = KeyConfig.KEYCONFIG_INPUT_CAMERA_RIGHT
local KEYCONFIG_INPUT_CAMERA_UP = KeyConfig.KEYCONFIG_INPUT_CAMERA_UP
local KEYCONFIG_INPUT_CAMERA_DOWN = KeyConfig.KEYCONFIG_INPUT_CAMERA_DOWN
local KEYCONFIG_INPUT_ATTACK = KeyConfig.KEYCONFIG_INPUT_ATTACK
local KEYCONFIG_INPUT_JUMP = KeyConfig.KEYCONFIG_INPUT_JUMP
local KEYCONFIG_INPUT_NUM = KeyConfig.KEYCONFIG_INPUT_NUM

local MAX_GAMEPAD_NUM = (16) -- チェックするゲームパッドの数
local KEY_STATE_BUFFER_SIZE = (256) -- キーボードの入力状態を取得するためのバッファのサイズ
local DIRECTINPUT_MAX_VALUE = (1000) -- DirectInput から得られる値の最大値

-- DirectInput の入力情報タイプ
local DIRECTINPUT_TYPE_X = (0) -- 方向入力のＸ軸
local DIRECTINPUT_TYPE_Y = (1) -- 方向入力のＹ軸
local DIRECTINPUT_TYPE_Z = (2) -- 方向入力のＺ軸
local DIRECTINPUT_TYPE_RX = (3) -- 方向入力のＸ軸回転
local DIRECTINPUT_TYPE_RY = (4) -- 方向入力のＹ軸回転
local DIRECTINPUT_TYPE_RZ = (5) -- 方向入力のＺ軸回転
local DIRECTINPUT_TYPE_POV = (6) -- 方向コントローラ
local DIRECTINPUT_TYPE_BUTTON = (7) -- ボタン
local DIRECTINPUT_TYPE_KEY = (8) -- キーボードのキー

-- DxLua: 配列を作る
local function NewArray(num, constructor)
    num = num or 0
    constructor = constructor or function () return nil end
    local t = {}
    for i = 1, num do
        table.insert(t, constructor())
    end
    return t
end

-- DxLua: 渡した値の配列を作る
local function NewValueArray(num, value)
    return NewArray(num, function () return value end)
end

-- キーボードの１キー辺りの情報
local function KEYINFO(t)
    t = t or {}
    return {
        KeyInput = t.KeyInput or 0, -- 入力コード( KEY_INPUT_LEFT など )
        Name = t.Name or '' -- キーの名前
    }
end

-- ゲームでの入力とキーやパッドなどの入力との対応情報
local function KEYCONFIGINFO(t)
    t = t or {}
    return {
        PadNo = t.PadNo or 0, -- パッド番号
        DirectInputType = t.DirectInputType or 0, -- DirectInput の入力情報タイプ( DIRECTINPUT_TYPE_X など )
        SubInfo = t.SubInfo or { 0, 0 }, -- サブ情報( DirectInputType によって意味が変わる )
    }
end

-- 入力処理用の情報
local function KEYCONFIGSYSTEMDATA(t)
    t = t or {}
    return {
        KeyConfigInfo = t.KeyConfigInfo or NewArray(KEYCONFIG_INPUT_NUM, KEYCONFIGINFO), -- ゲーム中の入力とキーやパッドなどの入力との対応情報
        -- DirectInput のパッドの初期入力状態
        NeutralDirectInputState = t.NeutralDirectInputState or NewArray(MAX_GAMEPAD_NUM, DxLua.DINPUT_JOYSTATE.new),
        -- DirectInput のパッドの入力状態
        DirectInputState = t.DirectInputState or NewArray(MAX_GAMEPAD_NUM, DxLua.DINPUT_JOYSTATE.new),
        -- キーの初期入力状態
        NeutralKeyInput = t.NeutralKeyInput or NewValueArray(KEY_STATE_BUFFER_SIZE, 0),
        -- キーの入力状態
        KeyInput = t.KeyInput or NewValueArray(KEY_STATE_BUFFER_SIZE, 0),
        ButtonInput = t.ButtonInput or 0, -- ボタンの入力状態
        ButtonEdgeInput = t.ButtonEdgeInput or 0, -- ボタンの押された瞬間のみの入力状態
        CameraStickInput = t.CameraStickInput or DxLua.VECTOR(), -- カメラ操作用の入力状態
    }
end

-- 入力処理用の情報
local g_KeyConfSys = KEYCONFIGSYSTEMDATA()

-- ゲームでの各入力とキーやパッドなどの入力とのデフォルトの対応設定
local g_DefaultInputTypeInfo =
{
	KEYCONFIGINFO{ PadNo = 0, DirectInputType = DIRECTINPUT_TYPE_X,      SubInfo = { -1, 0 }, }, -- KEYCONFIG_INPUT_LEFT
	KEYCONFIGINFO{ PadNo = 0, DirectInputType = DIRECTINPUT_TYPE_X,      SubInfo = {  1, 0 }, }, -- KEYCONFIG_INPUT_RIGHT
	KEYCONFIGINFO{ PadNo = 0, DirectInputType = DIRECTINPUT_TYPE_Y,      SubInfo = { -1, 0 }, }, -- KEYCONFIG_INPUT_UP
	KEYCONFIGINFO{ PadNo = 0, DirectInputType = DIRECTINPUT_TYPE_Y,      SubInfo = {  1, 0 }, }, -- KEYCONFIG_INPUT_DOWN
	KEYCONFIGINFO{ PadNo = 0, DirectInputType = DIRECTINPUT_TYPE_RX,     SubInfo = { -1, 0 }, }, -- KEYCONFIG_INPUT_CAMERA_LEFT
	KEYCONFIGINFO{ PadNo = 0, DirectInputType = DIRECTINPUT_TYPE_RX,     SubInfo = {  1, 0 }, }, -- KEYCONFIG_INPUT_CAMERA_RIGHT
	KEYCONFIGINFO{ PadNo = 0, DirectInputType = DIRECTINPUT_TYPE_RY,     SubInfo = { -1, 0 }, }, -- KEYCONFIG_INPUT_CAMERA_UP
	KEYCONFIGINFO{ PadNo = 0, DirectInputType = DIRECTINPUT_TYPE_RY,     SubInfo = {  1, 0 }, }, -- KEYCONFIG_INPUT_CAMERA_DOWN
	KEYCONFIGINFO{ PadNo = 0, DirectInputType = DIRECTINPUT_TYPE_BUTTON, SubInfo = {  0, 0 }, }, -- KEYCONFIG_INPUT_ATTACK
	KEYCONFIGINFO{ PadNo = 0, DirectInputType = DIRECTINPUT_TYPE_BUTTON, SubInfo = {  2, 0 }, }, -- KEYCONFIG_INPUT_JUMP
}

-- パッドの識別子テーブル
local g_PadIDTable =
{
	DxLua.DX_INPUT_PAD1,
	DxLua.DX_INPUT_PAD2,
	DxLua.DX_INPUT_PAD3,
	DxLua.DX_INPUT_PAD4,
	DxLua.DX_INPUT_PAD5,
	DxLua.DX_INPUT_PAD6,
	DxLua.DX_INPUT_PAD7,
	DxLua.DX_INPUT_PAD8,
	DxLua.DX_INPUT_PAD9,
	DxLua.DX_INPUT_PAD10,
	DxLua.DX_INPUT_PAD11,
	DxLua.DX_INPUT_PAD12,
	DxLua.DX_INPUT_PAD13,
	DxLua.DX_INPUT_PAD14,
	DxLua.DX_INPUT_PAD15,
	DxLua.DX_INPUT_PAD16,
}

-- キーコードの情報
local g_KeyInfoTable =
{
	KEYINFO{ DxLua.KEY_INPUT_BACK, "BACK", }, -- バックスペースキー
	KEYINFO{ DxLua.KEY_INPUT_TAB, "TAB", }, -- タブキー
	KEYINFO{ DxLua.KEY_INPUT_RETURN, "ENTER", }, -- エンターキー

	KEYINFO{ DxLua.KEY_INPUT_LSHIFT, "L SHIFT", }, -- 左シフトキー
	KEYINFO{ DxLua.KEY_INPUT_RSHIFT, "R SHIFT", }, -- 右シフトキー
	KEYINFO{ DxLua.KEY_INPUT_LCONTROL, "L CONTROL", }, -- 左コントロールキー
	KEYINFO{ DxLua.KEY_INPUT_RCONTROL, "R CONTROL", }, -- 右コントロールキー
	KEYINFO{ DxLua.KEY_INPUT_ESCAPE, "ESCAPE", }, -- エスケープキー
	KEYINFO{ DxLua.KEY_INPUT_SPACE, "SPACE", }, -- スペースキー
	KEYINFO{ DxLua.KEY_INPUT_PGUP, "PAGE UP", }, -- ＰａｇｅＵＰキー
	KEYINFO{ DxLua.KEY_INPUT_PGDN, "PAGE DOWN", }, -- ＰａｇｅＤｏｗｎキー
	KEYINFO{ DxLua.KEY_INPUT_END, "END", }, -- エンドキー
	KEYINFO{ DxLua.KEY_INPUT_HOME, "HOME", }, -- ホームキー
	KEYINFO{ DxLua.KEY_INPUT_LEFT, "LEFT", }, -- 左キー
	KEYINFO{ DxLua.KEY_INPUT_UP, "UP", }, -- 上キー
	KEYINFO{ DxLua.KEY_INPUT_RIGHT, "RIGHT", }, -- 右キー
	KEYINFO{ DxLua.KEY_INPUT_DOWN, "DOWN", }, -- 下キー
	KEYINFO{ DxLua.KEY_INPUT_INSERT, "INSERT", }, -- インサートキー
	KEYINFO{ DxLua.KEY_INPUT_DELETE, "DELETE", }, -- デリートキー

	KEYINFO{ DxLua.KEY_INPUT_MINUS, "-", }, -- －キー
	KEYINFO{ DxLua.KEY_INPUT_YEN, "\\", }, -- ￥キー
	KEYINFO{ DxLua.KEY_INPUT_PREVTRACK, "^", }, -- ＾キー
	KEYINFO{ DxLua.KEY_INPUT_PERIOD, ".", }, -- ．キー
	KEYINFO{ DxLua.KEY_INPUT_SLASH, "/", }, -- ／キー
	KEYINFO{ DxLua.KEY_INPUT_LALT, "L ALT", }, -- 左ＡＬＴキー
	KEYINFO{ DxLua.KEY_INPUT_RALT, "R ALT", }, -- 右ＡＬＴキー
	KEYINFO{ DxLua.KEY_INPUT_SCROLL, "SCROLL LOCK", }, -- ScrollLockキー
	KEYINFO{ DxLua.KEY_INPUT_SEMICOLON, "", }, -- ；キー
	KEYINFO{ DxLua.KEY_INPUT_COLON, ":", }, -- ：キー
	KEYINFO{ DxLua.KEY_INPUT_LBRACKET, "[", }, -- ［キー
	KEYINFO{ DxLua.KEY_INPUT_RBRACKET, "]", }, -- ］キー
	KEYINFO{ DxLua.KEY_INPUT_AT, "@", }, -- ＠キー
	KEYINFO{ DxLua.KEY_INPUT_BACKSLASH, "BACK SLASH", }, -- ＼キー
	KEYINFO{ DxLua.KEY_INPUT_COMMA, ",", }, -- ，キー
	KEYINFO{ DxLua.KEY_INPUT_KANJI, "KANJI", }, -- 漢字キー
	KEYINFO{ DxLua.KEY_INPUT_CONVERT, "CONVERT", }, -- 変換キー
	KEYINFO{ DxLua.KEY_INPUT_NOCONVERT, "NO CONVERT", }, -- 無変換キー
	KEYINFO{ DxLua.KEY_INPUT_KANA, "KANA", }, -- カナキー
	KEYINFO{ DxLua.KEY_INPUT_APPS, "APPS", }, -- アプリケーションメニューキー
	KEYINFO{ DxLua.KEY_INPUT_CAPSLOCK, "CAPS LOCK", }, -- CaspLockキー
	KEYINFO{ DxLua.KEY_INPUT_SYSRQ, "PRINT SCREEN", }, -- PrintScreenキー
	KEYINFO{ DxLua.KEY_INPUT_PAUSE, "PAUSE BREAK", }, -- PauseBreakキー
	KEYINFO{ DxLua.KEY_INPUT_LWIN, "L WIN", }, -- 左Ｗｉｎキー
	KEYINFO{ DxLua.KEY_INPUT_RWIN, "R WIN", }, -- 右Ｗｉｎキー

	KEYINFO{ DxLua.KEY_INPUT_NUMLOCK, "NUM LOCK", }, -- テンキー０
	KEYINFO{ DxLua.KEY_INPUT_NUMPAD0, "NUMPAD 0", }, -- テンキー０
	KEYINFO{ DxLua.KEY_INPUT_NUMPAD1, "NUMPAD 1", }, -- テンキー１
	KEYINFO{ DxLua.KEY_INPUT_NUMPAD2, "NUMPAD 2", }, -- テンキー２
	KEYINFO{ DxLua.KEY_INPUT_NUMPAD3, "NUMPAD 3", }, -- テンキー３
	KEYINFO{ DxLua.KEY_INPUT_NUMPAD4, "NUMPAD 4", }, -- テンキー４
	KEYINFO{ DxLua.KEY_INPUT_NUMPAD5, "NUMPAD 5", }, -- テンキー５
	KEYINFO{ DxLua.KEY_INPUT_NUMPAD6, "NUMPAD 6", }, -- テンキー６
	KEYINFO{ DxLua.KEY_INPUT_NUMPAD7, "NUMPAD 7", }, -- テンキー７
	KEYINFO{ DxLua.KEY_INPUT_NUMPAD8, "NUMPAD 8", }, -- テンキー８
	KEYINFO{ DxLua.KEY_INPUT_NUMPAD9, "NUMPAD 9", }, -- テンキー９
	KEYINFO{ DxLua.KEY_INPUT_MULTIPLY, "NUMPAD *", }, -- テンキー＊キー
	KEYINFO{ DxLua.KEY_INPUT_ADD, "NUMPAD +", }, -- テンキー＋キー
	KEYINFO{ DxLua.KEY_INPUT_SUBTRACT, "NUMPAD -", }, -- テンキー－キー
	KEYINFO{ DxLua.KEY_INPUT_DECIMAL, "NUMPAD .", }, -- テンキー．キー
	KEYINFO{ DxLua.KEY_INPUT_DIVIDE, "NUMPAD /", }, -- テンキー／キー
	KEYINFO{ DxLua.KEY_INPUT_NUMPADENTER, "NUMPAD ENTER", }, -- テンキーのエンターキー

	KEYINFO{ DxLua.KEY_INPUT_F1, "F1", }, -- Ｆ１キー
	KEYINFO{ DxLua.KEY_INPUT_F2, "F2", }, -- Ｆ２キー
	KEYINFO{ DxLua.KEY_INPUT_F3, "F3", }, -- Ｆ３キー
	KEYINFO{ DxLua.KEY_INPUT_F4, "F4", }, -- Ｆ４キー
	KEYINFO{ DxLua.KEY_INPUT_F5, "F5", }, -- Ｆ５キー
	KEYINFO{ DxLua.KEY_INPUT_F6, "F6", }, -- Ｆ６キー
	KEYINFO{ DxLua.KEY_INPUT_F7, "F7", }, -- Ｆ７キー
	KEYINFO{ DxLua.KEY_INPUT_F8, "F8", }, -- Ｆ８キー
	KEYINFO{ DxLua.KEY_INPUT_F9, "F9", }, -- Ｆ９キー
	KEYINFO{ DxLua.KEY_INPUT_F10, "F10", }, -- Ｆ１０キー
	KEYINFO{ DxLua.KEY_INPUT_F11, "F11", }, -- Ｆ１１キー
	KEYINFO{ DxLua.KEY_INPUT_F12, "F12", }, -- Ｆ１２キー

	KEYINFO{ DxLua.KEY_INPUT_A, "A", }, -- Ａキー
	KEYINFO{ DxLua.KEY_INPUT_B, "B", }, -- Ｂキー
	KEYINFO{ DxLua.KEY_INPUT_C, "C", }, -- Ｃキー
	KEYINFO{ DxLua.KEY_INPUT_D, "D", }, -- Ｄキー
	KEYINFO{ DxLua.KEY_INPUT_E, "E", }, -- Ｅキー
	KEYINFO{ DxLua.KEY_INPUT_F, "F", }, -- Ｆキー
	KEYINFO{ DxLua.KEY_INPUT_G, "G", }, -- Ｇキー
	KEYINFO{ DxLua.KEY_INPUT_H, "H", }, -- Ｈキー
	KEYINFO{ DxLua.KEY_INPUT_I, "I", }, -- Ｉキー
	KEYINFO{ DxLua.KEY_INPUT_J, "J", }, -- Ｊキー
	KEYINFO{ DxLua.KEY_INPUT_K, "K", }, -- Ｋキー
	KEYINFO{ DxLua.KEY_INPUT_L, "L", }, -- Ｌキー
	KEYINFO{ DxLua.KEY_INPUT_M, "M", }, -- Ｍキー
	KEYINFO{ DxLua.KEY_INPUT_N, "N", }, -- Ｎキー
	KEYINFO{ DxLua.KEY_INPUT_O, "O", }, -- Ｏキー
	KEYINFO{ DxLua.KEY_INPUT_P, "P", }, -- Ｐキー
	KEYINFO{ DxLua.KEY_INPUT_Q, "Q", }, -- Ｑキー
	KEYINFO{ DxLua.KEY_INPUT_R, "R", }, -- Ｒキー
	KEYINFO{ DxLua.KEY_INPUT_S, "S", }, -- Ｓキー
	KEYINFO{ DxLua.KEY_INPUT_T, "T", }, -- Ｔキー
	KEYINFO{ DxLua.KEY_INPUT_U, "U", }, -- Ｕキー
	KEYINFO{ DxLua.KEY_INPUT_V, "V", }, -- Ｖキー
	KEYINFO{ DxLua.KEY_INPUT_W, "W", }, -- Ｗキー
	KEYINFO{ DxLua.KEY_INPUT_X, "X", }, -- Ｘキー
	KEYINFO{ DxLua.KEY_INPUT_Y, "Y", }, -- Ｙキー
	KEYINFO{ DxLua.KEY_INPUT_Z, "Z", }, -- Ｚキー

	KEYINFO{ DxLua.KEY_INPUT_0, "0", }, -- ０キー
	KEYINFO{ DxLua.KEY_INPUT_1, "1", }, -- １キー
	KEYINFO{ DxLua.KEY_INPUT_2, "2", }, -- ２キー
	KEYINFO{ DxLua.KEY_INPUT_3, "3", }, -- ３キー
	KEYINFO{ DxLua.KEY_INPUT_4, "4", }, -- ４キー
	KEYINFO{ DxLua.KEY_INPUT_5, "5", }, -- ５キー
	KEYINFO{ DxLua.KEY_INPUT_6, "6", }, -- ６キー
	KEYINFO{ DxLua.KEY_INPUT_7, "7", }, -- ７キー
	KEYINFO{ DxLua.KEY_INPUT_8, "8", }, -- ８キー
	KEYINFO{ DxLua.KEY_INPUT_9, "9", }, -- ９キー
}

-- キーコンフィグの初期化を行う
function KeyConfig_Initialize()
	local PadNum

	-- パッドの数を取得する
	PadNum = DxLua.GetJoypadNum()

	-- パッドの数がプログラムが対応している数より多い場合は、対応している数に制限する
	if PadNum > MAX_GAMEPAD_NUM then
		PadNum = MAX_GAMEPAD_NUM
	end

	-- パッドの初期状態を DirectInput から取得する
	for i = 1, PadNum do
		DxLua.GetJoypadDirectInputState(g_PadIDTable[i], g_KeyConfSys.NeutralDirectInputState[i])
	end

	-- キー入力の初期状態を取得する
	DxLua.GetHitKeyStateAll(g_KeyConfSys.NeutralKeyInput)
end

-- KeyConfig_InputProcess 用の軸入力タイプの処理を行う補助関数
local function KeyConfig_InputProcess_Assist(
	-- 処理結果を代入する変数のアドレス
	InputState,

	-- 軸入力タイプのサブ情報
	--    (  1:InputValue がプラスの値の場合に入力ありとする
	--      -1:InputValue がマイナスの値の場合に入力ありとする )
	SubInfo,

	-- 軸入力の値
	InputValue
)
	-- サブ情報が 1 か -1 かで処理を分岐
	if SubInfo == 1 then -- 1の場合は InputValue がプラスの場合のみ入力ありとする
		if InputValue > 0 then
			InputState = InputValue
		end

    elseif SubInfo == 1 then -- -1の場合は InputValue がプラスの場合のみ入力ありとする
		if InputValue < 0 then
			InputState = -InputValue
		end
    end

    return InputState
end

-- キーコンフィグの入力処理を行う( 毎フレーム実行する )
function KeyConfig_InputProcess()
	local KCInfo = nil
	local InputState = NewValueArray(KEYCONFIG_INPUT_NUM, 0)
	local PadNum = 0
	local DIJState = nil
	local ButtonPrevInput = 0
	local Total = 0

	-- パッドの数を取得する
	PadNum = DxLua.GetJoypadNum()

	-- パッドの数がプログラムが対応している数より多い場合は、対応している数に制限する
	if PadNum > MAX_GAMEPAD_NUM then
		PadNum = MAX_GAMEPAD_NUM
	end

	-- パッドの入力情報を DirectInput から取得する
	for i = 1, PadNum do
		DxLua.GetJoypadDirectInputState(g_PadIDTable[i], g_KeyConfSys.DirectInputState[i])
	end

	-- 現在のキーの入力状態を取得する
	DxLua.GetHitKeyStateAll(g_KeyConfSys.KeyInput)

	-- ゲームで使用する入力情報を構築する
	for i = 1, KEYCONFIG_INPUT_NUM do
        KCInfo = g_KeyConfSys.KeyConfigInfo[i]

		-- 『入力なし』状態にしておく
		InputState[i] = 0

		-- 対応する DirectInput の情報タイプによって処理を分岐
        DIJState = KCInfo.PadNo < 0 and nil or g_KeyConfSys.DirectInputState[KCInfo.PadNo]
        if not DIJState then
            -- パッド情報が取得できなかった

        elseif KCInfo.DirectInputType == DIRECTINPUT_TYPE_X then
		    -- 方向入力のＸ軸の場合
            InputState[i] = KeyConfig_InputProcess_Assist(InputState[i], KCInfo.SubInfo[1], DIJState.X)

        elseif KCInfo.DirectInputType == DIRECTINPUT_TYPE_Y then
            -- 方向入力のＹ軸の場合
			InputState[i] = KeyConfig_InputProcess_Assist(InputState[i], KCInfo.SubInfo[1], DIJState.Y)

        elseif KCInfo.DirectInputType == DIRECTINPUT_TYPE_Z then
            -- 方向入力のＺ軸の場合
			InputState[i] = KeyConfig_InputProcess_Assist(InputState[i], KCInfo.SubInfo[1], DIJState.Z)

        elseif KCInfo.DirectInputType == DIRECTINPUT_TYPE_RX then
            -- 方向入力のＸ軸回転の場合
			InputState[i] = KeyConfig_InputProcess_Assist(InputState[i], KCInfo.SubInfo[1], DIJState.Rx)

        elseif KCInfo.DirectInputType == DIRECTINPUT_TYPE_RY then
            -- 方向入力のＹ軸回転の場合
			InputState[i] = KeyConfig_InputProcess_Assist(InputState[i], KCInfo.SubInfo[1], DIJState.Ry)

        elseif KCInfo.DirectInputType == DIRECTINPUT_TYPE_RZ then
            -- 方向入力のＺ軸回転の場合
			InputState[i] = KeyConfig_InputProcess_Assist(InputState[i], KCInfo.SubInfo[1], DIJState.Rz)

        elseif KCInfo.DirectInputType == DIRECTINPUT_TYPE_POV then
            -- 方向コントローラの場合
			-- 設定されている方向に入力されていたら『入力あり』にする
			if DIJState.POV[KCInfo.SubInfo[1] + 1] == KCInfo.SubInfo[2] then
				InputState[i] = DIRECTINPUT_MAX_VALUE
			end

        elseif KCInfo.DirectInputType == DIRECTINPUT_TYPE_BUTTON then
            -- ボタンの場合
			-- 設定されているボタンが押されていたら『入力あり』にする
			if DIJState.Buttons[KCInfo.SubInfo[1] + 1] == 128 then
				InputState[i] = DIRECTINPUT_MAX_VALUE
			end

        elseif KCInfo.DirectInputType == DIRECTINPUT_TYPE_KEY then
            -- キーボードのキーの場合
			-- 設定されているキーが押されていたら『入力あり』にする
			if g_KeyConfSys.KeyInput[KCInfo.SubInfo[1] + 1] then
				InputState[i] = DIRECTINPUT_MAX_VALUE
			end
		end
	end

	-- １フレーム前の入力状態をとっておく
	ButtonPrevInput = g_KeyConfSys.ButtonInput

	-- 入力状態の初期化
	g_KeyConfSys.ButtonInput = 0

	-- 各入力に対応するビットを『入力あり』の場合は立てる
	for i = 1, KEYCONFIG_INPUT_NUM do
		if InputState[i] > 0 then
			g_KeyConfSys.ButtonInput = bit.bor(g_KeyConfSys.ButtonInput, bit.lshift(1, i))
		end
	end

	-- 今回のフレームで『入力あり』になり始めたビットを算出する
	g_KeyConfSys.ButtonEdgeInput = bit.band(g_KeyConfSys.ButtonInput, bit.bnot(ButtonPrevInput))

	-- カメラ操作用の入力状態を算出する
	Total = InputState[KEYCONFIG_INPUT_CAMERA_RIGHT] - InputState[KEYCONFIG_INPUT_CAMERA_LEFT]
	g_KeyConfSys.CameraStickInput.x = Total / DIRECTINPUT_MAX_VALUE

	Total = InputState[KEYCONFIG_INPUT_CAMERA_DOWN] - InputState[KEYCONFIG_INPUT_CAMERA_UP]
	g_KeyConfSys.CameraStickInput.y = Total / DIRECTINPUT_MAX_VALUE

	g_KeyConfSys.CameraStickInput.z = 0.0
end

-- ボタンの入力状態を取得する( 戻り値 : ボタンの入力状態 )
function KeyConfig_GetInput()
	return g_KeyConfSys.ButtonInput
end

-- ボタンの押された瞬間のみの入力状態を取得する( 戻り値 : ボタンの押された瞬間のみの入力状態 )
function KeyConfig_GetEdgeInput()
	return g_KeyConfSys.ButtonEdgeInput
end

-- カメラ操作用の入力状態を取得する( 戻り値 : カメラ操作用の入力状態 )
function KeyConfig_GetCameraInput()
	return g_KeyConfSys.CameraStickInput
end

-- キーコンフィグ設定をファイルに保存する( 戻り値 : 保存成功：TRUE   保存失敗：FALSE )
function KeyConfig_Save(FilePath)
	local fp
	local KCInfo = nil

	-- 対応情報を保存するファイルを開く
	fp = io.open(FilePath, "w")
	if not fp then
		-- 開けなかったら何もせずに終了
		return false
	end

	-- ゲームの各入力とキーやパッドなどの入力との対応情報をファイルに書き出す
	for i = 1, KEYCONFIG_INPUT_NUM do
        KCInfo = g_KeyConfSys.KeyConfigInfo[i]
		fp:write(string.format("PadNo = %d  DirectInputType = %d  SubInfo0 = %d  SubInfo1 = %d \n",
			KCInfo.PadNo,
			KCInfo.DirectInputType,
			KCInfo.SubInfo[1],
			KCInfo.SubInfo[2]
        ))
	end

	-- ファイルを閉じる
	fp:close()

	-- 成功終了
	return true
end

-- キーコンフィグ設定をファイルから読み込む( 戻り値 : 読み込み成功：TRUE   読み込み失敗：FALSE )
function KeyConfig_Load(FilePath)
	local fp
	local KCInfo = nil

	-- 対応情報を保存したファイルを開く
	fp = io.open(FilePath, "r")
	if not fp then
		-- 開けなかった場合は何もせずに終了
		return false
	end

	-- ゲームの各入力とキーやパッドなどの入力との対応情報をファイルから読み込む
	for i = 1, KEYCONFIG_INPUT_NUM do
        KCInfo = g_KeyConfSys.KeyConfigInfo[i]

        -- DxLua: fscanf に相当する関数は無いので正規表現で取得する
        -- 一行読み込む
        local line = fp:read()

        -- 正規表現で各数字を文字列として取得
        local a, b, c, d
            = string.match(line, 'PadNo = (%d+)  DirectInputType = (%d+)  SubInfo0 = (%d+)  SubInfo1 = (%d+)')

        -- それぞれ数値に変換して情報に格納
        KCInfo.PadNo, KCInfo.DirectInputType, KCInfo.SubInfo[1], KCInfo.SubInfo[2]
             = tonumber(a or 0), tonumber(b or 0), tonumber(c or 0), tonumber(d or 0)
	end

	-- ファイルを閉じる
	fp:close()

	-- 成功終了
	return true
end

-- キーコンフィグ設定をデフォルトにする
function  KeyConfig_SetDefault()
	-- デフォルト設定の情報をコピーする
	for i = 1, KEYCONFIG_INPUT_NUM do
		g_KeyConfSys.KeyConfigInfo[i] = g_DefaultInputTypeInfo[i]
	end
end

-- UpdateInputTypeInfo 用の軸入力タイプの処理を行う補助関数
local function UpdateInputTypeInfo_Assist(
	KCInfo,					-- キーやパッドなどの入力情報
	ValidMaxDInput,			-- MaxInput に有効な値が入っているかどうかを保存している変数のアドレス
	MaxDInput,				-- 有効な入力値の最大値を保存している変数のアドレス
	PadNo,					-- パッド番号
	DirectInputType,			-- DirectInput の入力情報タイプ
	DirectInputValue,		-- DirectInput の入力値
	NeutralDirectInputValue	-- 初期状態の DirectInput の入力値
)
	-- 初期状態と値が同じ場合は何もしない
	if DirectInputValue == NeutralDirectInputValue then
		return ValidMaxDInput, MaxDInput
	end

	-- DirectInput の入力値がマイナスかプラスかで処理を分岐
	if DirectInputValue < 0 then
		-- MaxDirectInputValue にまだ有効な値が入っていないか
		-- 又は MaxDirectInputValue の値よりも DirectInputValue の値が大きいかをチェック
		if ValidMaxDInput == false or MaxDInput < -DirectInputValue then
			-- 情報を保存する
			KCInfo.PadNo = PadNo
			KCInfo.DirectInputType = DirectInputType
			KCInfo.SubInfo[1] = -1
			KCInfo.SubInfo[2] = 0

			ValidMaxDInput = true
			MaxDInput = -DirectInputValue
		end
	else
		if DirectInputValue > 0 then
			-- MaxDirectInputValue にまだ有効な値が入っていないか
			-- 又は MaxDirectInputValue の値よりも DirectInputValue の値が大きいかをチェック
			if ValidMaxDInput == false or MaxDInput < DirectInputValue then
				-- 情報を保存する
				KCInfo.PadNo = PadNo
				KCInfo.DirectInputType = DirectInputType
				KCInfo.SubInfo[1] = 1
				KCInfo.SubInfo[2] = 0

				ValidMaxDInput = true
				MaxDInput = DirectInputValue
			end
        end
    end

    return ValidMaxDInput, MaxDInput
end

-- 現在押下されているキーやパッドの入力情報をチェックして、入力があった場合は情報更新する( キーコンフィグ画面用 )
--     戻り値 : 入力があったかどうか( true:入力があった  false:入力は無かった )
function KeyConfig_UpdateInputTypeInfo(UpdateInputType)
	local ValidMaxDInput
	local MaxDInput
	local PadNum
	local DIJState
	local NDIJState
	local KCInfo = g_KeyConfSys.KeyConfigInfo[UpdateInputType]
	local Result

	-- 戻り値は『入力が無かった』にしておく
	Result = false

	-- 接続されているパッドの数を取得する
	PadNum = DxLua.GetJoypadNum()

	-- パッドの数がプログラムが対応している数より多い場合は、対応している数に制限する
	if PadNum > MAX_GAMEPAD_NUM then
		PadNum = MAX_GAMEPAD_NUM
	end

	-- 『MaxDInput に有効な値が入っているかどうか』のフラグを倒し、
	-- MaxDInput の値も初期化しておく
	ValidMaxDInput = false
	MaxDInput = 0

	-- 全てのパッドの入力状態をチェック
	for i = 1, PadNum do
        DIJState = g_KeyConfSys.DirectInputState[i]
        NDIJState = g_KeyConfSys.NeutralDirectInputState[i]

		-- 方向入力状態をチェック
		ValidMaxDInput, MaxDInput = UpdateInputTypeInfo_Assist(KCInfo, ValidMaxDInput, MaxDInput, i, DIRECTINPUT_TYPE_X, DIJState.X, NDIJState.X)
		ValidMaxDInput, MaxDInput = UpdateInputTypeInfo_Assist(KCInfo, ValidMaxDInput, MaxDInput, i, DIRECTINPUT_TYPE_Y, DIJState.Y, NDIJState.Y)
		ValidMaxDInput, MaxDInput = UpdateInputTypeInfo_Assist(KCInfo, ValidMaxDInput, MaxDInput, i, DIRECTINPUT_TYPE_Z, DIJState.Z, NDIJState.Z)
		ValidMaxDInput, MaxDInput = UpdateInputTypeInfo_Assist(KCInfo, ValidMaxDInput, MaxDInput, i, DIRECTINPUT_TYPE_RX, DIJState.Rx, NDIJState.Rx)
		ValidMaxDInput, MaxDInput = UpdateInputTypeInfo_Assist(KCInfo, ValidMaxDInput, MaxDInput, i, DIRECTINPUT_TYPE_RY, DIJState.Ry, NDIJState.Ry)
		ValidMaxDInput, MaxDInput = UpdateInputTypeInfo_Assist(KCInfo, ValidMaxDInput, MaxDInput, i, DIRECTINPUT_TYPE_RZ, DIJState.Rz, NDIJState.Rz)

		-- 方向コントローラの入力状態をチェック
		for j = 1, 4 do
			-- POV が有効な値( -1 以外の値 )で、且つ初期状態と異なるかをチェック
			if DIJState.POV[j] ~= -1 and
				DIJState.POV[j] ~= NDIJState.POV[j] then
				-- 情報を保存する
				KCInfo.PadNo = i
				KCInfo.DirectInputType = DIRECTINPUT_TYPE_POV
				KCInfo.SubInfo[1] = j - 1
				KCInfo.SubInfo[2] = DIJState.POV[j]

				-- 戻り値を『入力があった』にする
				Result = true
			end
		end

		-- ボタンの入力状態をチェック
		for j = 1, 32 do
			-- ボタンが押されていて、且つ初期状態と異なるかをチェック
			if DIJState.Buttons[j] == 128 and
				DIJState.Buttons[j] ~= NDIJState.Buttons[j] then
				-- 情報を保存する
				KCInfo.PadNo = i
				KCInfo.DirectInputType = DIRECTINPUT_TYPE_BUTTON
				KCInfo.SubInfo[1] = j - 1
				KCInfo.SubInfo[2] = 0

				-- 戻り値を『入力があった』にする
				Result = true
			end
		end
	end

	-- ValidMaxDirectInputValue が true の場合は、有効な入力があったということなので戻り値を『入力があった』にする
	if ValidMaxDInput then
		Result = true
	end

	-- キーの入力状態をチェック
	for i = 1, KEY_STATE_BUFFER_SIZE do
		-- 入力が初期状態と異なるかをチェック
		if g_KeyConfSys.KeyInput[i] ~= g_KeyConfSys.NeutralKeyInput[i] then
			-- 情報を保存する
			KCInfo.PadNo = -1
			KCInfo.DirectInputType = DIRECTINPUT_TYPE_KEY
			KCInfo.SubInfo[1] = i - 1

			-- 戻り値を『入力があった』にする
			Result = true
		end
	end

	-- 入力があったかどうかを返す
	return Result
end

-- パッドかキーの入力があるかどうかを取得する( キーコンフィグ画面用 )
--     戻り値 : 入力があったかどうか( true:入力があった  false:入力は無かった )
function KeyConfig_CheckInput()
	local PadNum

	-- 接続されているパッドの数を取得する
	PadNum = DxLua.GetJoypadNum()

	-- パッドの数がプログラムが対応している数より多い場合は、対応している数に制限する
	if PadNum > MAX_GAMEPAD_NUM then
		PadNum = MAX_GAMEPAD_NUM
	end

	-- 全てのパッドの入力状態をチェック
	for i = 1, PadNum do
        -- 初期状態と情報が異なる場合は何かしら入力があるということなので、TRUE を返す
        if g_KeyConfSys.DirectInputState[i] ~= g_KeyConfSys.NeutralDirectInputState[i] then
			return true
		end
	end

	-- キーの入力状態が初期状態と異なる場合はキーの入力があるということなので true を返す
	if g_KeyConfSys.NeutralKeyInput ~= g_KeyConfSys.KeyInput then
		return true
	end

	-- ここに来たということは何も入力が無かったということなので false を返す
	return false
end

-- 指定の入力情報タイプに割り当てられているパッドやキーの入力の名前を取得する( キーコンフィグ画面用 )
function KeyConfig_GetInputTypeString(InputType, InputString)
	local KCInfo = g_KeyConfSys.KeyConfigInfo[InputType]
    local String

    InputString = ''

	-- キーボード入力かどうかで処理を分岐
	if KCInfo.DirectInputType == DIRECTINPUT_TYPE_KEY then
		-- キーコードの情報テーブルに無いキーコードである場合に備えて、最初は UNKNOWN を代入しておく
		InputString = "Key UNKNOWN"

        -- 一致するキーコード情報をテーブルから探す
        for KInfo in ipairs(g_KeyInfoTable) do
			if KInfo.KeyInput == KCInfo.SubInfo[1] then
				InputString = "Key " .. KInfo.Name
				break
			end
		end
	else
		-- パッドの入力名は先頭にパッドの番号を付ける
		String = "Pad No." .. KCInfo.PadNo .. ' '

		-- 対応しているパッドの DirectInput の情報タイプによって分岐
        if KCInfo.DirectInputType == DIRECTINPUT_TYPE_X then
            -- X軸入力の場合はプラスの場合は RIGHT、マイナスの場合は LEFT を付ける
			InputString = String .. (KCInfo.SubInfo[1] > 0 and "RIGHT" or "LEFT")

        elseif KCInfo.DirectInputType == DIRECTINPUT_TYPE_Y then
            -- Y軸入力の場合はプラスの場合は DOWN、マイナスの場合は UP を付ける
			InputString = String .. (KCInfo.SubInfo[1] > 0 and "DOWN" or "UP")

        elseif KCInfo.DirectInputType == DIRECTINPUT_TYPE_Z then
            -- Z軸入力の場合はプラスの場合は +、マイナスの場合は - を付ける
			InputString = String .. (KCInfo.SubInfo[1] > 0 and "+" or "-") .. 'Z'

        elseif KCInfo.DirectInputType == DIRECTINPUT_TYPE_RX then
            -- X軸回転入力の場合はプラスの場合は +、マイナスの場合は - を付ける
			InputString = String .. (KCInfo.SubInfo[1] > 0 and "+" or "-") .. 'Rx'

        elseif KCInfo.DirectInputType == DIRECTINPUT_TYPE_RY then
            -- Y軸回転入力の場合はプラスの場合は +、マイナスの場合は - を付ける
			InputString = String .. (KCInfo.SubInfo[1] > 0 and "+" or "-") .. 'Ry'

        elseif KCInfo.DirectInputType == DIRECTINPUT_TYPE_RZ then
            -- Z軸回転入力の場合はプラスの場合は +、マイナスの場合は - を付ける
			InputString = String .. (KCInfo.SubInfo[1] > 0 and "+" or "-") .. 'Rz'

        elseif KCInfo.DirectInputType == DIRECTINPUT_TYPE_POV then
            -- 方向コントローラ入力の場合は POV であることと、POVの情報番号と入力角度を付ける
			InputString = String .. 'POV ' .. KCInfo.SubInfo[1] .. ' Angle ' .. math.floor(KCInfo.SubInfo[2] / 100)

        elseif KCInfo.DirectInputType == DIRECTINPUT_TYPE_BUTTON then
            -- ボタン入力の場合は BUTTON とボタン番号を付ける
			InputString = String .. ' BUTTON ' .. KCInfo.SubInfo[1]
		end
    end

    return InputString
end

return KeyConfig
