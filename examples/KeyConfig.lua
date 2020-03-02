
local KeyConfig = doend

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
        KeyConfigInfo = t.KeyConfigInfo or KEYCONFIGINFO(), -- ゲーム中の入力とキーやパッドなどの入力との対応情報
        -- DirectInput のパッドの初期入力状態
        NeutralDirectInputState = t.NeutralDirectInputState or NewArray(MAX_GAMEPAD_NUM, DxLua.DINPUT_JOYSTATE),
        -- DirectInput のパッドの入力状態
        DirectInputState = t.DirectInputState or NewArray(MAX_GAMEPAD_NUM, DxLua.DINPUT_JOYSTATE),
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
	DX_INPUT_PAD1,
	DX_INPUT_PAD2,
	DX_INPUT_PAD3,
	DX_INPUT_PAD4,
	DX_INPUT_PAD5,
	DX_INPUT_PAD6,
	DX_INPUT_PAD7,
	DX_INPUT_PAD8,
	DX_INPUT_PAD9,
	DX_INPUT_PAD10,
	DX_INPUT_PAD11,
	DX_INPUT_PAD12,
	DX_INPUT_PAD13,
	DX_INPUT_PAD14,
	DX_INPUT_PAD15,
	DX_INPUT_PAD16,
}

-- キーコードの情報
local g_KeyInfoTable =
{
	KEYINFO{ KEY_INPUT_BACK, "BACK", }, -- バックスペースキー
	KEYINFO{ KEY_INPUT_TAB, "TAB", }, -- タブキー
	KEYINFO{ KEY_INPUT_RETURN, "ENTER", }, -- エンターキー

	KEYINFO{ KEY_INPUT_LSHIFT, "L SHIFT", }, -- 左シフトキー
	KEYINFO{ KEY_INPUT_RSHIFT, "R SHIFT", }, -- 右シフトキー
	KEYINFO{ KEY_INPUT_LCONTROL, "L CONTROL", }, -- 左コントロールキー
	KEYINFO{ KEY_INPUT_RCONTROL, "R CONTROL", }, -- 右コントロールキー
	KEYINFO{ KEY_INPUT_ESCAPE, "ESCAPE", }, -- エスケープキー
	KEYINFO{ KEY_INPUT_SPACE, "SPACE", }, -- スペースキー
	KEYINFO{ KEY_INPUT_PGUP, "PAGE UP", }, -- ＰａｇｅＵＰキー
	KEYINFO{ KEY_INPUT_PGDN, "PAGE DOWN", }, -- ＰａｇｅＤｏｗｎキー
	KEYINFO{ KEY_INPUT_END, "END", }, -- エンドキー
	KEYINFO{ KEY_INPUT_HOME, "HOME", }, -- ホームキー
	KEYINFO{ KEY_INPUT_LEFT, "LEFT", }, -- 左キー
	KEYINFO{ KEY_INPUT_UP, "UP", }, -- 上キー
	KEYINFO{ KEY_INPUT_RIGHT, "RIGHT", }, -- 右キー
	KEYINFO{ KEY_INPUT_DOWN, "DOWN", }, -- 下キー
	KEYINFO{ KEY_INPUT_INSERT, "INSERT", }, -- インサートキー
	KEYINFO{ KEY_INPUT_DELETE, "DELETE", }, -- デリートキー

	KEYINFO{ KEY_INPUT_MINUS, "-", }, -- －キー
	KEYINFO{ KEY_INPUT_YEN, "\\", }, -- ￥キー
	KEYINFO{ KEY_INPUT_PREVTRACK, "^", }, -- ＾キー
	KEYINFO{ KEY_INPUT_PERIOD, ".", }, -- ．キー
	KEYINFO{ KEY_INPUT_SLASH, "/", }, -- ／キー
	KEYINFO{ KEY_INPUT_LALT, "L ALT", }, -- 左ＡＬＴキー
	KEYINFO{ KEY_INPUT_RALT, "R ALT", }, -- 右ＡＬＴキー
	KEYINFO{ KEY_INPUT_SCROLL, "SCROLL LOCK", }, -- ScrollLockキー
	KEYINFO{ KEY_INPUT_SEMICOLON, ";", }, -- ；キー
	KEYINFO{ KEY_INPUT_COLON, ":", }, -- ：キー
	KEYINFO{ KEY_INPUT_LBRACKET, "[", }, -- ［キー
	KEYINFO{ KEY_INPUT_RBRACKET, "]", }, -- ］キー
	KEYINFO{ KEY_INPUT_AT, "@", }, -- ＠キー
	KEYINFO{ KEY_INPUT_BACKSLASH, "BACK SLASH", }, -- ＼キー
	KEYINFO{ KEY_INPUT_COMMA, ",", }, -- ，キー
	KEYINFO{ KEY_INPUT_KANJI, "KANJI", }, -- 漢字キー
	KEYINFO{ KEY_INPUT_CONVERT, "CONVERT", }, -- 変換キー
	KEYINFO{ KEY_INPUT_NOCONVERT, "NO CONVERT", }, -- 無変換キー
	KEYINFO{ KEY_INPUT_KANA, "KANA", }, -- カナキー
	KEYINFO{ KEY_INPUT_APPS, "APPS", }, -- アプリケーションメニューキー
	KEYINFO{ KEY_INPUT_CAPSLOCK, "CAPS LOCK", }, -- CaspLockキー
	KEYINFO{ KEY_INPUT_SYSRQ, "PRINT SCREEN", }, -- PrintScreenキー
	KEYINFO{ KEY_INPUT_PAUSE, "PAUSE BREAK", }, -- PauseBreakキー
	KEYINFO{ KEY_INPUT_LWIN, "L WIN", }, -- 左Ｗｉｎキー
	KEYINFO{ KEY_INPUT_RWIN, "R WIN", }, -- 右Ｗｉｎキー

	KEYINFO{ KEY_INPUT_NUMLOCK, "NUM LOCK", }, -- テンキー０
	KEYINFO{ KEY_INPUT_NUMPAD0, "NUMPAD 0", }, -- テンキー０
	KEYINFO{ KEY_INPUT_NUMPAD1, "NUMPAD 1", }, -- テンキー１
	KEYINFO{ KEY_INPUT_NUMPAD2, "NUMPAD 2", }, -- テンキー２
	KEYINFO{ KEY_INPUT_NUMPAD3, "NUMPAD 3", }, -- テンキー３
	KEYINFO{ KEY_INPUT_NUMPAD4, "NUMPAD 4", }, -- テンキー４
	KEYINFO{ KEY_INPUT_NUMPAD5, "NUMPAD 5", }, -- テンキー５
	KEYINFO{ KEY_INPUT_NUMPAD6, "NUMPAD 6", }, -- テンキー６
	KEYINFO{ KEY_INPUT_NUMPAD7, "NUMPAD 7", }, -- テンキー７
	KEYINFO{ KEY_INPUT_NUMPAD8, "NUMPAD 8", }, -- テンキー８
	KEYINFO{ KEY_INPUT_NUMPAD9, "NUMPAD 9", }, -- テンキー９
	KEYINFO{ KEY_INPUT_MULTIPLY, "NUMPAD *", }, -- テンキー＊キー
	KEYINFO{ KEY_INPUT_ADD, "NUMPAD +", }, -- テンキー＋キー
	KEYINFO{ KEY_INPUT_SUBTRACT, "NUMPAD -", }, -- テンキー－キー
	KEYINFO{ KEY_INPUT_DECIMAL, "NUMPAD .", }, -- テンキー．キー
	KEYINFO{ KEY_INPUT_DIVIDE, "NUMPAD /", }, -- テンキー／キー
	KEYINFO{ KEY_INPUT_NUMPADENTER, "NUMPAD ENTER", }, -- テンキーのエンターキー

	KEYINFO{ KEY_INPUT_F1, "F1", }, -- Ｆ１キー
	KEYINFO{ KEY_INPUT_F2, "F2", }, -- Ｆ２キー
	KEYINFO{ KEY_INPUT_F3, "F3", }, -- Ｆ３キー
	KEYINFO{ KEY_INPUT_F4, "F4", }, -- Ｆ４キー
	KEYINFO{ KEY_INPUT_F5, "F5", }, -- Ｆ５キー
	KEYINFO{ KEY_INPUT_F6, "F6", }, -- Ｆ６キー
	KEYINFO{ KEY_INPUT_F7, "F7", }, -- Ｆ７キー
	KEYINFO{ KEY_INPUT_F8, "F8", }, -- Ｆ８キー
	KEYINFO{ KEY_INPUT_F9, "F9", }, -- Ｆ９キー
	KEYINFO{ KEY_INPUT_F10, "F10", }, -- Ｆ１０キー
	KEYINFO{ KEY_INPUT_F11, "F11", }, -- Ｆ１１キー
	KEYINFO{ KEY_INPUT_F12, "F12", }, -- Ｆ１２キー

	KEYINFO{ KEY_INPUT_A, "A", }, -- Ａキー
	KEYINFO{ KEY_INPUT_B, "B", }, -- Ｂキー
	KEYINFO{ KEY_INPUT_C, "C", }, -- Ｃキー
	KEYINFO{ KEY_INPUT_D, "D", }, -- Ｄキー
	KEYINFO{ KEY_INPUT_E, "E", }, -- Ｅキー
	KEYINFO{ KEY_INPUT_F, "F", }, -- Ｆキー
	KEYINFO{ KEY_INPUT_G, "G", }, -- Ｇキー
	KEYINFO{ KEY_INPUT_H, "H", }, -- Ｈキー
	KEYINFO{ KEY_INPUT_I, "I", }, -- Ｉキー
	KEYINFO{ KEY_INPUT_J, "J", }, -- Ｊキー
	KEYINFO{ KEY_INPUT_K, "K", }, -- Ｋキー
	KEYINFO{ KEY_INPUT_L, "L", }, -- Ｌキー
	KEYINFO{ KEY_INPUT_M, "M", }, -- Ｍキー
	KEYINFO{ KEY_INPUT_N, "N", }, -- Ｎキー
	KEYINFO{ KEY_INPUT_O, "O", }, -- Ｏキー
	KEYINFO{ KEY_INPUT_P, "P", }, -- Ｐキー
	KEYINFO{ KEY_INPUT_Q, "Q", }, -- Ｑキー
	KEYINFO{ KEY_INPUT_R, "R", }, -- Ｒキー
	KEYINFO{ KEY_INPUT_S, "S", }, -- Ｓキー
	KEYINFO{ KEY_INPUT_T, "T", }, -- Ｔキー
	KEYINFO{ KEY_INPUT_U, "U", }, -- Ｕキー
	KEYINFO{ KEY_INPUT_V, "V", }, -- Ｖキー
	KEYINFO{ KEY_INPUT_W, "W", }, -- Ｗキー
	KEYINFO{ KEY_INPUT_X, "X", }, -- Ｘキー
	KEYINFO{ KEY_INPUT_Y, "Y", }, -- Ｙキー
	KEYINFO{ KEY_INPUT_Z, "Z", }, -- Ｚキー

	KEYINFO{ KEY_INPUT_0, "0", }, -- ０キー
	KEYINFO{ KEY_INPUT_1, "1", }, -- １キー
	KEYINFO{ KEY_INPUT_2, "2", }, -- ２キー
	KEYINFO{ KEY_INPUT_3, "3", }, -- ３キー
	KEYINFO{ KEY_INPUT_4, "4", }, -- ４キー
	KEYINFO{ KEY_INPUT_5, "5", }, -- ５キー
	KEYINFO{ KEY_INPUT_6, "6", }, -- ６キー
	KEYINFO{ KEY_INPUT_7, "7", }, -- ７キー
	KEYINFO{ KEY_INPUT_8, "8", }, -- ８キー
	KEYINFO{ KEY_INPUT_9, "9", }, -- ９キー
}

-- キーコンフィグの初期化を行う
function KeyConfig_Initialize()
	local PadNum;

	-- パッドの数を取得する
	PadNum = DxLua.GetJoypadNum();

	-- パッドの数がプログラムが対応している数より多い場合は、対応している数に制限する
	if PadNum > MAX_GAMEPAD_NUM then
		PadNum = MAX_GAMEPAD_NUM;
	end

	-- パッドの初期状態を DirectInput から取得する
	for i = 1, PadNum do
		DxLua.GetJoypadDirectInputState(g_PadIDTable[i], g_KeyConfSys.NeutralDirectInputState[i]);
	end

	-- キー入力の初期状態を取得する
	DxLua.GetHitKeyStateAll(g_KeyConfSys.NeutralKeyInput);
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
			InputState = InputValue;
		end

    elseif SubInfo == 1 then -- -1の場合は InputValue がプラスの場合のみ入力ありとする
		if InputValue < 0 then
			InputState = -InputValue;
		end
    end

    return InputState
end

-- キーコンフィグの入力処理を行う( 毎フレーム実行する )
function KeyConfig_InputProcess(function) do
	int              i;
	KEYCONFIGINFO *KCInfo;
	int              InputState[KEYCONFIG_INPUT_NUM];
	int              PadNum;
	DINPUT_JOYSTATE *DIJState;
	int              ButtonPrevInput;
	int              Total;

	-- パッドの数を取得する
	PadNum = GetJoypadNum();

	-- パッドの数がプログラムが対応している数より多い場合は、対応している数に制限する
	if PadNum > MAX_GAMEPAD_NUM then
		PadNum = MAX_GAMEPAD_NUM;
	end

	-- パッドの入力情報を DirectInput から取得する
	for (i = 0; i < PadNum; i++) do
		GetJoypadDirectInputState(g_PadIDTable[i], &g_KeyConfSys.DirectInputState[i]);
	end

	-- 現在のキーの入力状態を取得する
	GetHitKeyStateAll(g_KeyConfSys.KeyInput);

	-- ゲームで使用する入力情報を構築する
	KCInfo = g_KeyConfSys.KeyConfigInfo;
	for (i = 0; i < KEYCONFIG_INPUT_NUM; i++, KCInfo++) do
		-- 『入力なし』状態にしておく
		InputState[i] = 0;

		-- 対応する DirectInput の情報タイプによって処理を分岐
		DIJState = KCInfo->PadNo < 0 ? NULL : &g_KeyConfSys.DirectInputState[KCInfo->PadNo];
		switch (KCInfo->DirectInputType) do
		case DIRECTINPUT_TYPE_X:	-- 方向入力のＸ軸の場合
			KeyConfig_InputProcess_Assist(&InputState[i], KCInfo->SubInfo[0], DIJState->X);
			break;

		case DIRECTINPUT_TYPE_Y:	-- 方向入力のＹ軸の場合
			KeyConfig_InputProcess_Assist(&InputState[i], KCInfo->SubInfo[0], DIJState->Y);
			break;

		case DIRECTINPUT_TYPE_Z:	-- 方向入力のＺ軸の場合
			KeyConfig_InputProcess_Assist(&InputState[i], KCInfo->SubInfo[0], DIJState->Z);
			break;

		case DIRECTINPUT_TYPE_RX:	-- 方向入力のＸ軸回転の場合
			KeyConfig_InputProcess_Assist(&InputState[i], KCInfo->SubInfo[0], DIJState->Rx);
			break;

		case DIRECTINPUT_TYPE_RY:	-- 方向入力のＹ軸回転の場合
			KeyConfig_InputProcess_Assist(&InputState[i], KCInfo->SubInfo[0], DIJState->Ry);
			break;

		case DIRECTINPUT_TYPE_RZ:	-- 方向入力のＺ軸回転の場合
			KeyConfig_InputProcess_Assist(&InputState[i], KCInfo->SubInfo[0], DIJState->Rz);
			break;

		case DIRECTINPUT_TYPE_POV:	-- 方向コントローラの場合
			-- 設定されている方向に入力されていたら『入力あり』にする
			if DIJState->POV[KCInfo->SubInfo[0]] == KCInfo->SubInfo[1] then
				InputState[i] = DIRECTINPUT_MAX_VALUE;
			end
			break;

		case DIRECTINPUT_TYPE_BUTTON:	-- ボタンの場合
			-- 設定されているボタンが押されていたら『入力あり』にする
			if DIJState->Buttons[KCInfo->SubInfo[0]] == 128 then
				InputState[i] = DIRECTINPUT_MAX_VALUE;
			end
			break;

		case DIRECTINPUT_TYPE_KEY:	-- キーボードのキーの場合
			-- 設定されているキーが押されていたら『入力あり』にする
			if g_KeyConfSys.KeyInput[KCInfo->SubInfo[0]] then
				InputState[i] = DIRECTINPUT_MAX_VALUE;
			end
			break;
		end
	end

	-- １フレーム前の入力状態をとっておく
	ButtonPrevInput = g_KeyConfSys.ButtonInput;

	-- 入力状態の初期化
	g_KeyConfSys.ButtonInput = 0;

	-- 各入力に対応するビットを『入力あり』の場合は立てる
	for (i = 0; i < KEYCONFIG_INPUT_NUM; i++) do
		if InputState[i] > 0 then
			g_KeyConfSys.ButtonInput |= 1 << i;
		end
	end

	-- 今回のフレームで『入力あり』になり始めたビットを算出する
	g_KeyConfSys.ButtonEdgeInput = g_KeyConfSys.ButtonInput & ~ButtonPrevInput;

	-- カメラ操作用の入力状態を算出する
	Total = InputState[KEYCONFIG_INPUT_CAMERA_RIGHT] - InputState[KEYCONFIG_INPUT_CAMERA_LEFT];
	g_KeyConfSys.CameraStickInput.x = Total / (float)DIRECTINPUT_MAX_VALUE;

	Total = InputState[KEYCONFIG_INPUT_CAMERA_DOWN] - InputState[KEYCONFIG_INPUT_CAMERA_UP];
	g_KeyConfSys.CameraStickInput.y = Total / (float)DIRECTINPUT_MAX_VALUE;

	g_KeyConfSys.CameraStickInput.z = 0.0f;
end

-- ボタンの入力状態を取得する( 戻り値 : ボタンの入力状態 )
int KeyConfig_GetInput(function) do
	return g_KeyConfSys.ButtonInput;
end

-- ボタンの押された瞬間のみの入力状態を取得する( 戻り値 : ボタンの押された瞬間のみの入力状態 )
int KeyConfig_GetEdgeInput(function) do
	return g_KeyConfSys.ButtonEdgeInput;
end

-- カメラ操作用の入力状態を取得する( 戻り値 : カメラ操作用の入力状態 )
VECTOR KeyConfig_GetCameraInput(function) do
	return g_KeyConfSys.CameraStickInput;
end

-- キーコンフィグ設定をファイルに保存する( 戻り値 : 保存成功：TRUE   保存失敗：FALSE )
int KeyConfig_Save(const char *FilePath) do
	FILE *fp;
	KEYCONFIGINFO *KCInfo;
	int            i;

	-- 対応情報を保存するファイルを開く
	fp = fopen(FilePath, "wt");
	if fp == NULL then
		-- 開けなかったら何もせずに終了
		return FALSE;
	end

	-- ゲームの各入力とキーやパッドなどの入力との対応情報をファイルに書き出す
	KCInfo = g_KeyConfSys.KeyConfigInfo;
	for (i = 0; i < KEYCONFIG_INPUT_NUM; i++, KCInfo++) do
		fprintf(fp, "PadNo = %d  DirectInputType = %d  SubInfo0 = %d  SubInfo1 = %d \n",
			KCInfo->PadNo,
			KCInfo->DirectInputType,
			KCInfo->SubInfo[0],
			KCInfo->SubInfo[1]
		);
	end

	-- ファイルを閉じる
	fclose(fp);

	-- 成功終了
	return TRUE;
end

-- キーコンフィグ設定をファイルから読み込む( 戻り値 : 読み込み成功：TRUE   読み込み失敗：FALSE )
int KeyConfig_Load(const char *FilePath) do
	FILE *fp;
	KEYCONFIGINFO *KCInfo;
	int            i;

	-- 対応情報を保存したファイルを開く
	fp = fopen(FilePath, "rt");
	if fp == NULL then
		-- 開けなかった場合は何もせずに終了
		return FALSE;
	end

	-- ゲームの各入力とキーやパッドなどの入力との対応情報をファイルから読み込む
	KCInfo = g_KeyConfSys.KeyConfigInfo;
	for (i = 0; i < KEYCONFIG_INPUT_NUM; i++, KCInfo++) do
		fscanf(fp, "PadNo = %d  DirectInputType = %d  SubInfo0 = %d  SubInfo1 = %d \n",
			&KCInfo->PadNo,
			&KCInfo->DirectInputType,
			&KCInfo->SubInfo[0],
			&KCInfo->SubInfo[1]
		);
	end

	-- ファイルを閉じる
	fclose(fp);

	-- 成功終了
	return TRUE;
end

-- キーコンフィグ設定をデフォルトにする
function  KeyConfig_SetDefault(function) do
	int i;

	-- デフォルト設定の情報をコピーする
	for (i = 0; i < KEYCONFIG_INPUT_NUM; i++) do
		g_KeyConfSys.KeyConfigInfo[i] = g_DefaultInputTypeInfo[i];
	end
end

-- UpdateInputTypeInfo 用の軸入力タイプの処理を行う補助関数
local function UpdateInputTypeInfo_Assist(
	KEYCONFIGINFO *KCInfo,					-- キーやパッドなどの入力情報
	int *ValidMaxDInput,			-- MaxInput に有効な値が入っているかどうかを保存している変数のアドレス
	int *MaxDInput,				-- 有効な入力値の最大値を保存している変数のアドレス
	int            PadNo,					-- パッド番号
	int            DirectInputType,			-- DirectInput の入力情報タイプ
	int            DirectInputValue,		-- DirectInput の入力値
	int            NeutralDirectInputValue	-- 初期状態の DirectInput の入力値
) do
	-- 初期状態と値が同じ場合は何もしない
	if DirectInputValue == NeutralDirectInputValue then
		return;
	end

	-- DirectInput の入力値がマイナスかプラスかで処理を分岐
	if DirectInputValue < 0 then
		-- MaxDirectInputValue にまだ有効な値が入っていないか
		-- 又は MaxDirectInputValue の値よりも DirectInputValue の値が大きいかをチェック
		if *ValidMaxDInput == FALSE || *MaxDInput < -DirectInputValue then
			-- 情報を保存する
			KCInfo->PadNo = PadNo;
			KCInfo->DirectInputType = DirectInputType;
			KCInfo->SubInfo[0] = -1;
			KCInfo->SubInfo[1] = 0;

			*ValidMaxDInput = TRUE;
			*MaxDInput = -DirectInputValue;
		end
	end else
		if DirectInputValue > 0 then
			-- MaxDirectInputValue にまだ有効な値が入っていないか
			-- 又は MaxDirectInputValue の値よりも DirectInputValue の値が大きいかをチェック
			if *ValidMaxDInput == FALSE || *MaxDInput < DirectInputValue then
				-- 情報を保存する
				KCInfo->PadNo = PadNo;
				KCInfo->DirectInputType = DirectInputType;
				KCInfo->SubInfo[0] = 1;
				KCInfo->SubInfo[1] = 0;

				*ValidMaxDInput = TRUE;
				*MaxDInput = DirectInputValue;
			end
		end
end

-- 現在押下されているキーやパッドの入力情報をチェックして、入力があった場合は情報更新する( キーコンフィグ画面用 )
--     戻り値 : 入力があったかどうか( TRUE:入力があった  FALSE:入力は無かった )
int KeyConfig_UpdateInputTypeInfo(int UpdateInputType) do
	int              i;
	int              j;
	int              ValidMaxDInput;
	int              MaxDInput;
	int              PadNum;
	DINPUT_JOYSTATE *DIJState;
	DINPUT_JOYSTATE *NDIJState;
	KEYCONFIGINFO *KCInfo = &g_KeyConfSys.KeyConfigInfo[UpdateInputType];
	int              Result;

	-- 戻り値は『入力が無かった』にしておく
	Result = FALSE;

	-- 接続されているパッドの数を取得する
	PadNum = GetJoypadNum();

	-- パッドの数がプログラムが対応している数より多い場合は、対応している数に制限する
	if PadNum > MAX_GAMEPAD_NUM then
		PadNum = MAX_GAMEPAD_NUM;
	end

	-- 『MaxDInput に有効な値が入っているかどうか』のフラグを倒し、
	-- MaxDInput の値も初期化しておく
	ValidMaxDInput = FALSE;
	MaxDInput = 0;

	-- 全てのパッドの入力状態をチェック
	DIJState = g_KeyConfSys.DirectInputState;
	NDIJState = g_KeyConfSys.NeutralDirectInputState;
	for (i = 0; i < PadNum; i++, DIJState++, NDIJState++) do
		-- 方向入力状態をチェック
		UpdateInputTypeInfo_Assist(KCInfo, &ValidMaxDInput, &MaxDInput, i, DIRECTINPUT_TYPE_X, DIJState->X, NDIJState->X);
		UpdateInputTypeInfo_Assist(KCInfo, &ValidMaxDInput, &MaxDInput, i, DIRECTINPUT_TYPE_Y, DIJState->Y, NDIJState->Y);
		UpdateInputTypeInfo_Assist(KCInfo, &ValidMaxDInput, &MaxDInput, i, DIRECTINPUT_TYPE_Z, DIJState->Z, NDIJState->Z);
		UpdateInputTypeInfo_Assist(KCInfo, &ValidMaxDInput, &MaxDInput, i, DIRECTINPUT_TYPE_RX, DIJState->Rx, NDIJState->Rx);
		UpdateInputTypeInfo_Assist(KCInfo, &ValidMaxDInput, &MaxDInput, i, DIRECTINPUT_TYPE_RY, DIJState->Ry, NDIJState->Ry);
		UpdateInputTypeInfo_Assist(KCInfo, &ValidMaxDInput, &MaxDInput, i, DIRECTINPUT_TYPE_RZ, DIJState->Rz, NDIJState->Rz);

		-- 方向コントローラの入力状態をチェック
		for (j = 0; j < 4; j++) do
			-- POV が有効な値( -1 以外の値 )で、且つ初期状態と異なるかをチェック
			if (DIJState->POV[j] != -1 &&
				DIJState->POV[j] != NDIJState->POV[j]) do
				-- 情報を保存する
				KCInfo->PadNo = i;
				KCInfo->DirectInputType = DIRECTINPUT_TYPE_POV;
				KCInfo->SubInfo[0] = j;
				KCInfo->SubInfo[1] = DIJState->POV[j];

				-- 戻り値を『入力があった』にする
				Result = TRUE;
			end
		end

		-- ボタンの入力状態をチェック
		for (j = 0; j < 32; j++) do
			-- ボタンが押されていて、且つ初期状態と異なるかをチェック
			if (DIJState->Buttons[j] == 128 &&
				DIJState->Buttons[j] != NDIJState->Buttons[j]) do
				-- 情報を保存する
				KCInfo->PadNo = i;
				KCInfo->DirectInputType = DIRECTINPUT_TYPE_BUTTON;
				KCInfo->SubInfo[0] = j;
				KCInfo->SubInfo[1] = 0;

				-- 戻り値を『入力があった』にする
				Result = TRUE;
			end
		end
	end

	-- ValidMaxDirectInputValue が true の場合は、有効な入力があったということなので戻り値を『入力があった』にする
	if ValidMaxDInput then
		Result = TRUE;
	end

	-- キーの入力状態をチェック
	for (i = 0; i < KEY_STATE_BUFFER_SIZE; i++) do
		-- 入力が初期状態と異なるかをチェック
		if g_KeyConfSys.KeyInput[i] != g_KeyConfSys.NeutralKeyInput[i] then
			-- 情報を保存する
			KCInfo->PadNo = -1;
			KCInfo->DirectInputType = DIRECTINPUT_TYPE_KEY;
			KCInfo->SubInfo[0] = i;

			-- 戻り値を『入力があった』にする
			Result = TRUE;
		end
	end

	-- 入力があったかどうかを返す
	return Result;
end

-- パッドかキーの入力があるかどうかを取得する( キーコンフィグ画面用 )
--     戻り値 : 入力があったかどうか( TRUE:入力があった  FALSE:入力は無かった )
int KeyConfig_CheckInput(function) do
	int              i;
	int              PadNum;

	-- 接続されているパッドの数を取得する
	PadNum = GetJoypadNum();

	-- パッドの数がプログラムが対応している数より多い場合は、対応している数に制限する
	if PadNum > MAX_GAMEPAD_NUM then
		PadNum = MAX_GAMEPAD_NUM;
	end

	-- 全てのパッドの入力状態をチェック
	for (i = 0; i < PadNum; i++) do
		-- 初期状態と情報が異なる場合は何かしら入力があるということなので、TRUE を返す
		if (memcmp(&g_KeyConfSys.DirectInputState[i],
			&g_KeyConfSys.NeutralDirectInputState[i], sizeof(DINPUT_JOYSTATE)) != 0) do
			return TRUE;
		end
	end

	-- キーの入力状態が初期状態と異なる場合はキーの入力があるということなので TRUE を返す
	if memcmp(g_KeyConfSys.NeutralKeyInput, g_KeyConfSys.KeyInput, sizeof(g_KeyConfSys.NeutralKeyInput)) != 0 then
		return TRUE;
	end

	-- ここに来たということは何も入力が無かったということなので FALSE を返す
	return FALSE;
end

-- 指定の入力情報タイプに割り当てられているパッドやキーの入力の名前を取得する( キーコンフィグ画面用 )
function KeyConfig_GetInputTypeString(int InputType, char *InputString) do
	KEYCONFIGINFO *KCInfo = &g_KeyConfSys.KeyConfigInfo[InputType];
	KEYINFO *KInfo;
	char           String[256];

	-- キーボード入力かどうかで処理を分岐
	if KCInfo->DirectInputType == DIRECTINPUT_TYPE_KEY then
		-- キーコードの情報テーブルに無いキーコードである場合に備えて、最初は UNKNOWN を代入しておく
		strcpy(InputString, "Key UNKNOWN");

		-- 一致するキーコード情報をテーブルから探す
		for (KInfo = g_KeyInfoTable; KInfo->Name != NULL; KInfo++) do
			if KInfo->KeyInput == KCInfo->SubInfo[0] then
				sprintf(InputString, "Key %s", KInfo->Name);
				break;
			end
		end
	end else do
		-- パッドの入力名は先頭にパッドの番号を付ける
		sprintf(String, "Pad No.%d", KCInfo->PadNo);

		-- 対応しているパッドの DirectInput の情報タイプによって分岐
		switch (KCInfo->DirectInputType) do
		case DIRECTINPUT_TYPE_X:		-- X軸入力の場合はプラスの場合は RIGHT、マイナスの場合は LEFT を付ける
			sprintf(InputString, "%s %s", String, KCInfo->SubInfo[0] > 0 ? "RIGHT" : "LEFT");
			break;

		case DIRECTINPUT_TYPE_Y:		-- Y軸入力の場合はプラスの場合は DOWN、マイナスの場合は UP を付ける
			sprintf(InputString, "%s %s", String, KCInfo->SubInfo[0] > 0 ? "DOWN" : "UP");
			break;

		case DIRECTINPUT_TYPE_Z:		-- Z軸入力の場合はプラスの場合は +、マイナスの場合は - を付ける
			sprintf(InputString, "%s %sZ", String, KCInfo->SubInfo[0] > 0 ? "+" : "-");
			break;

		case DIRECTINPUT_TYPE_RX:		-- X軸回転入力の場合はプラスの場合は +、マイナスの場合は - を付ける
			sprintf(InputString, "%s %sRx", String, KCInfo->SubInfo[0] > 0 ? "+" : "-");
			break;

		case DIRECTINPUT_TYPE_RY:		-- Y軸回転入力の場合はプラスの場合は +、マイナスの場合は - を付ける
			sprintf(InputString, "%s %sRy", String, KCInfo->SubInfo[0] > 0 ? "+" : "-");
			break;

		case DIRECTINPUT_TYPE_RZ:		-- Z軸回転入力の場合はプラスの場合は +、マイナスの場合は - を付ける
			sprintf(InputString, "%s %sRz", String, KCInfo->SubInfo[0] > 0 ? "+" : "-");
			break;

		case DIRECTINPUT_TYPE_POV:		-- 方向コントローラ入力の場合は POV であることと、POVの情報番号と入力角度を付ける
			sprintf(InputString, "%s POV %d Angle %d°", String, KCInfo->SubInfo[0], KCInfo->SubInfo[1] / 100);
			break;

		case DIRECTINPUT_TYPE_BUTTON:	-- ボタン入力の場合は BUTTON とボタン番号を付ける
			sprintf(InputString, "%s BUTTON %d", String, KCInfo->SubInfo[0]);
			break;
		end
	end
end

return KeyConfig
