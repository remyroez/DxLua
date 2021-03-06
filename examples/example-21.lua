-- チャットプログラム基本
local band, bor, bnot, rshift = bit.band, bit.bor, bit.bnot, bit.rshift

local CHAT_LINENUM = 20 -- チャット中の文字列を表示する行数
local MAX_STRLENGTH = 80 -- チャットで1行で入力できる文字数
local INPUT_LINE = 21 -- チャットで入力領域となる画面上の行位置
local FONT_SIZE = 16 -- フォントのサイズ

InputHandle = InputHandle or 0 -- 入力ハンドル
StringY = StringY or 0 -- 文字列表示領域の次に文字列を表示する時の行位置
NetHandle = NetHandle or 0 -- 接続相手のネットワークハンドル

ScreenString = ScreenString or {} -- 画面に表示中のチャット文字列

StrBuf = StrBuf or ''

-- DxLua: 状態が複雑なのでステートマシンで管理する
local StateMachine = {
	State = 'Select',
	IP = { d1 = 0, d2 = 0, d3 = 0, d4 = 0 },
}

-- 現在の状態に合わせてメソッドを呼び分ける
function StateMachine:ChangeState(state)
	print('ChangeState', self.State, state)
	self.State = state
end

-- 現在の状態に合わせてメソッドを呼び分ける
function StateMachine:Update(...)
	if self[self.State] then
		self[self.State](self, ...)
	end
end

-- どちらのキーが押されるか監視する
function StateMachine:Select()
	if dx.CheckHitKey(dx.KEY_INPUT_Z) ~= 0 then
		Key = 'Z'
		self:ChangeState 'Host'
	elseif dx.CheckHitKey(dx.KEY_INPUT_X) ~= 0 then
		Key = 'X'
		self:ChangeState 'Client'
	end
end

-- 接続を待つ場合
function StateMachine:Host()
	-- 接続待ち状態にする
	dx.PreparationListenNetWork(9850)

	-- 接続があるまで待つ表示
	ScreenStringAdd("接続があるまで待ちます")

	self:ChangeState 'WaitAccess'
end

-- 接続があるまでここでループ
function StateMachine:WaitAccess()
	-- 新しい接続があった場合はそのネットハンドルを保存する
	NetHandle = dx.GetNewAcceptNetWork()

	-- 新しい接続があった場合はループを出る
	if NetHandle ~= -1 then
		-- 接続待ちを解除
		dx.StopListenNetWork()
		self:ChangeState 'PreChat'
	end
end

-- こちらから接続をする場合
function StateMachine:Client()
	-- 接続先のＩＰの入力を促す
	ScreenStringAdd("接続先のＩＰを入力してください")
	ScreenStringAdd("入力は半角で各ＩＰ値は『.』で区切り、隙間は入れないで下さい")

	-- ＩＰの入力を行う
	-- DxLua: 使用方法が C と異なる
	local err, ip = dx.KeyInputSingleCharString(0, INPUT_LINE * FONT_SIZE + 2, 80, false)
	StrBuf = type(ip) == 'string' and ip or ''

	-- 文字列からＩＰを抜き出す
	-- DxLua: 正規表現を使用して一度に取り出す
	self.IP.d1 = -1
	self.IP.d2 = -1
	self.IP.d3 = -1
	self.IP.d4 = -1
	local d1, d2, d3, d4 = (StrBuf:gmatch('([0-9]+)%.([0-9]+)%.([0-9]+)%.([0-9]+)'))()
	if d1 then self.IP.d1 = tonumber(d1) end
	if d2 then self.IP.d2 = tonumber(d2) end
	if d3 then self.IP.d3 = tonumber(d3) end
	if d4 then self.IP.d4 = tonumber(d4) end

	-- もし３つピリオドがなかった場合は入力のし直し
	if self.IP.d1 < 0 or self.IP.d2 < 0 or self.IP.d3 < 0 or self.IP.d4 < 0 then
		ScreenStringAdd("IP値の数が間違っています")
	else
		self:ChangeState 'Connect'
	end
end

-- 接続を試みる
function StateMachine:Connect()
	-- 接続中表示
	ScreenStringAdd("接続中")

	-- 接続を試みる
	NetHandle = dx.ConnectNetWork(self.IP, 9850)

	-- 接続に成功したらループから抜ける
	if NetHandle ~= -1 then
		self:ChangeState 'PreChat'
	else
		-- 接続失敗表示
		ScreenStringAdd("接続は失敗しました")
	end
end

-- チャットループ前処理
function StateMachine:PreChat()
	-- 接続成功表示
	ScreenStringAdd("接続しました")

	-- 文字列入力ハンドルを作成する
	InputHandle = dx.MakeKeyInput(80, false, false, false)

	-- 作成した入力ハンドルをアクティブにする
	dx.SetActiveKeyInput(InputHandle)

	self:ChangeState 'Chat'
end

-- チャットループ
function StateMachine:Chat()
	-- 切断確認
	if dx.GetLostNetWork() == NetHandle then
		return 'exit'
	end

	-- 受信した文字列がある場合は受信する
	if dx.GetNetWorkDataLength(NetHandle) > 4 then
		-- 受信した文字列の長さを得る
		local err, buffer = dx.NetWorkRecvToPeek(NetHandle, 4)
		StrLength = dx.Buffer_getInt32(buffer)

		-- 受信するはずの文字列長より受信されている文字数が少ない場合は
		-- 何もせずもどる
		if StrLength + 4 <= dx.GetNetWorkDataLength(NetHandle) then
			-- 文字列の長さを得る
			err, buffer = dx.NetWorkRecv(NetHandle, 4)
			Length = dx.Buffer_getInt32(buffer)

			-- メッセージを受信
			err, buffer = dx.NetWorkRecv(NetHandle, Length)
			local Message = dx.Buffer_getString(buffer)

			-- 画面に表示
			ScreenStringAdd(Message)
		end
	end

	-- 文字列入力
	do
		-- 文字列の入力が終っている場合は送信する
		if dx.CheckKeyInput(InputHandle) == 1 then
			-- 入力された文字列を取得する
			local err, Message = dx.GetKeyInputString(InputHandle)

			-- 入力された文字列の長さを送信する
			-- +1 は終端文字('\0')を含めるため
			StrLength = #Message + 1

			dx.NetWorkSend(NetHandle, StrLength, 4)

			-- 文字列を送信
			dx.NetWorkSend(NetHandle, Message, StrLength)

			-- 自分のとこにも表示する
			ScreenStringAdd(Message)

			-- 入力文字列を初期化する
			dx.SetKeyInputString("", InputHandle)

			-- 再度インプットハンドルをアクティブにする
			dx.SetActiveKeyInput(InputHandle)
		end

		-- 画面に入力中の文字列を描画する
		dx.DrawBox(0, INPUT_LINE * FONT_SIZE + 2, 640, 480, 0, true)
		dx.DrawKeyInputString(0, INPUT_LINE * FONT_SIZE + 2, InputHandle)
		dx.DrawKeyInputModeString(640, 480)
	end

	-- 時間待ち
	dx.WaitTimer(32)
end

dx.SetDoubleStartValidFlag(true)

-- ＤＸライブラリ初期化処理
function dx.Init()
	-- 入力領域と文字出力領域との境界線を引く
	dx.DrawLine(0, CHAT_LINENUM * FONT_SIZE, 640, CHAT_LINENUM * FONT_SIZE, dx.GetColor(255, 255, 255))

	-- 接続を待つか接続をするか入力してもらう
	ScreenStringAdd("接続を待つ場合はＺキーを、接続をする場合はＸキーを押してください")
end

-- ループ
function dx.Update()
	return StateMachine:Update()
end

-- ＤＸライブラリ使用の終了処理
function dx.End()
	-- 切断確認処理
	ScreenStringAdd("切断しました")

	-- 時間待ち
	dx.WaitTimer(3000)
end

-- チャット文字列を追加する
function ScreenStringAdd(AddString)
	-- 格納する行を一つ進める
	StringY = StringY + 1

	-- もし表示領域下いっぱいに来ていた場合はスクロールさせる
	if StringY == CHAT_LINENUM then
		for i = 2, #ScreenString do
			ScreenString[i - 1] = ScreenString[i]
		end

		ScreenString[#ScreenString - 1] = nil

		StringY = StringY - 1
	end

	-- 文字列を格納する
	ScreenString[StringY] = AddString

	-- 画面の内容を描画する
	ScreenStringDraw()

	-- 終了
	return 0
end

-- チャットの現在の状態を画面に表示する
function ScreenStringDraw()
	-- 文字列表示域を黒で塗りつぶす
	dx.DrawBox(0, 0, 640, CHAT_LINENUM * FONT_SIZE, 0, true)

	-- すべてのチャット文字列を描画する
	for i, str in ipairs(ScreenString) do
		dx.DrawString(0, (i - 1) * FONT_SIZE, str, dx.GetColor(255, 255, 255))
	end

	-- 終了
	return 0
end
