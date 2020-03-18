-- ３Ｄアクション基本

-- 固定値定義 -------------------------------------------------------------------------------------

-- プレイヤー関係の定義
local PLAYER_PLAY_ANIM_SPEED = 250.0		-- アニメーション速度
local PLAYER_MOVE_SPEED = 30.0		-- 移動速度
local PLAYER_ANIM_BLEND_SPEED = 0.1		-- アニメーションのブレンド率変化速度
local PLAYER_ANGLE_SPEED = 0.2		-- 角度変化速度
local PLAYER_JUMP_POWER = 100.0		-- ジャンプ力
local PLAYER_FALL_UP_POWER = 20.0		-- 足を踏み外した時のジャンプ力
local PLAYER_GRAVITY = 3.0		-- 重力
local PLAYER_MAX_HITCOLL = 2048		-- 処理するコリジョンポリゴンの最大数
local PLAYER_ENUM_DEFAULT_SIZE = 800.0		-- 周囲のポリゴン検出に使用する球の初期サイズ
local PLAYER_HIT_WIDTH = 200.0		-- 当たり判定カプセルの半径
local PLAYER_HIT_HEIGHT = 700.0		-- 当たり判定カプセルの高さ
local PLAYER_HIT_TRYNUM = 16		-- 壁押し出し処理の最大試行回数
local PLAYER_HIT_SLIDE_LENGTH = 5.0		-- 一度の壁押し出し処理でスライドさせる距離
local PLAYER_SHADOW_SIZE = 200.0		-- 影の大きさ
local PLAYER_SHADOW_HEIGHT = 700.0		-- 影が落ちる高さ

-- カメラ関係の定義
local CAMERA_ANGLE_SPEED = 0.05		-- 旋回速度
local CAMERA_PLAYER_TARGET_HEIGHT = 400.0		-- プレイヤー座標からどれだけ高い位置を注視点とするか
local CAMERA_PLAYER_LENGTH = 1600.0		-- プレイヤーとの距離
local CAMERA_COLLISION_SIZE = 50.0		-- カメラの当たり判定サイズ



-- 構造体定義 -------------------------------------------------------------------------------------

-- 入力情報構造体
function PADINPUT(t)
    t = t or {}
    return {
        NowInput = t.NowInput or 0, -- 現在の入力
        EdgeInput = t.EdgeInput or 0, -- 現在のフレームで押されたボタンのみビットが立っている入力値
    }
end

-- プレイヤー情報構造体
function PLAYER(t)
    t = t or {}
    return {
        Position = t.Position or dx.VECTOR{}, -- 座標
        TargetMoveDirection = t.TargetMoveDirection or dx.VECTOR{}, -- モデルが向くべき方向のベクトル
        Angle = t.Angle or 0, -- モデルが向いている方向の角度
        JumpPower = t.JumpPower or 0, -- Ｙ軸方向の速度
        ModelHandle = t.ModelHandle or 0, -- モデルハンドル
        ShadowHandle = t.ShadowHandle or 0, -- 影画像ハンドル
        State = t.State or 0, -- 状態( 0:立ち止まり  1:走り  2:ジャンプ )

        PlayAnim1 = t.PlayAnim1 or -1, -- 再生しているアニメーション１のアタッチ番号( -1:何もアニメーションがアタッチされていない )
        AnimPlayCount1 = t.AnimPlayCount1 or 0, -- 再生しているアニメーション１の再生時間
        PlayAnim2 = t.PlayAnim2 or -1, -- 再生しているアニメーション２のアタッチ番号( -1:何もアニメーションがアタッチされていない )
        AnimPlayCount2 = t.AnimPlayCount2 or 0, -- 再生しているアニメーション２の再生時間
        AnimBlendRate = t.AnimBlendRate or 0, -- 再生しているアニメーション１と２のブレンド率
    }
end

-- ステージ情報構造体
function STAGE(t)
    t = t or {}
    return {
        ModelHandle = t.ModelHandle or 0, -- モデルハンドル
    }
end

-- カメラ情報構造体
function CAMERA(t)
    t = t or {}
    return {
        AngleH = t.AngleH or 0, -- 水平角度
        AngleV = t.AngleV or 0, -- 垂直角度
        Eye = t.Eye or dx.VECTOR{}, -- カメラ座標
        Target = t.Target or dx.VECTOR{}, -- 注視点座標

        SetEye = t.SetEye or dx.VECTOR{},
        SetTarget = t.SetTarget or dx.VECTOR{},
    }
end



-- 実体宣言 ---------------------------------------------------------------------------------------

local inp = PADINPUT{};		-- 入力情報の実体宣言
local pl = PLAYER{};		-- プレイヤー情報の実体宣言
local stg = STAGE{};		-- ステージ情報の実体宣言
local cam = CAMERA{};		-- カメラ情報の実体宣言

-- ＤＸライブラリ初期化処理
function dx.Init()
	-- プレイヤーの初期化
	Player_Initialize();

	-- ステージの初期化
	Stage_Initialize();

	-- カメラの初期化
	Camera_Initialize();

	-- 描画先を裏画面にする
	dx.SetDrawScreen(dx.DX_SCREEN_BACK);
end

-- ループ
function dx.Update()
    -- 画面をクリア
    dx.ClearDrawScreen();

    -- 入力処理
    Input_Process();

    -- プレイヤーの処理
    Player_Process();

    -- カメラの処理
    Camera_Process();

    -- 描画処理
    Render_Process();

    -- 裏画面の内容を表画面に反映
    dx.ScreenFlip();
end

-- ライブラリの後始末
function dx.End()
	-- プレイヤーの後始末
	Player_Terminate();

	-- ステージの後始末
	Stage_Terminate();
end

-- 関数実体 ---------------------------------------------------------------------------------------

-- 入力処理
function Input_Process()
	local Old;

	-- ひとつ前のフレームの入力を変数にとっておく
	Old = inp.NowInput;

	-- 現在の入力状態を取得
	inp.NowInput = dx.GetJoypadInputState(dx.DX_INPUT_KEY_PAD1);

	-- 今のフレームで新たに押されたボタンのビットだけ立っている値を EdgeInput に代入する
	inp.EdgeInput = bit.band(inp.NowInput, bit.bnot(Old));
end

-- プレイヤーの初期化
function Player_Initialize()
	-- 初期座標は原点
	pl.Position = dx.VGet(0.0, 0.0, 0.0);

	-- 回転値は０
	pl.Angle = 0.0;

	-- ジャンプ力は初期状態では０
	pl.JumpPower = 0.0;

	-- モデルの読み込み
	pl.ModelHandle = dx.MV1LoadModel("DxChara.x");

	-- 影描画用の画像の読み込み
	pl.ShadowHandle = dx.LoadGraph("Shadow.tga");

	-- 初期状態では「立ち止り」状態
	pl.State = 0;

	-- 初期状態でプレイヤーが向くべき方向はＸ軸方向
	pl.TargetMoveDirection = dx.VGet(1.0, 0.0, 0.0);

	-- アニメーションのブレンド率を初期化
	pl.AnimBlendRate = 1.0;

	-- 初期状態ではアニメーションはアタッチされていないにする
	pl.PlayAnim1 = -1;
	pl.PlayAnim2 = -1;

	-- ただ立っているアニメーションを再生
	Player_PlayAnim(4);
end

-- プレイヤーの後始末
function Player_Terminate()
	-- モデルの削除
	dx.MV1DeleteModel(pl.ModelHandle);

	-- 影用画像の削除
	dx.DeleteGraph(pl.ShadowHandle);
end

-- プレイヤーの処理
function Player_Process()
	local UpMoveVec = dx.VECTOR{};		-- 方向ボタン「↑」を入力をしたときのプレイヤーの移動方向ベクトル
	local LeftMoveVec = dx.VECTOR{};		-- 方向ボタン「←」を入力をしたときのプレイヤーの移動方向ベクトル
	local MoveVec = dx.VECTOR{};		-- このフレームの移動ベクトル
	local MoveFlag;			-- 移動したかどうかのフラグ( 1:移動した  0:移動していない )

	-- ルートフレームのＺ軸方向の移動パラメータを無効にする
	do
		local LocalMatrix = dx.MATRIX{};

		-- ユーザー行列を解除する
		dx.MV1ResetFrameUserLocalMatrix(pl.ModelHandle, 2);

		-- 現在のルートフレームの行列を取得する
		LocalMatrix = dx.MV1GetFrameLocalMatrix(pl.ModelHandle, 2);

		-- Ｚ軸方向の平行移動成分を無効にする
		LocalMatrix.m[3][2] = 0.0;

		-- ユーザー行列として平行移動成分を無効にした行列をルートフレームにセットする
		dx.MV1SetFrameUserLocalMatrix(pl.ModelHandle, 2, LocalMatrix);
	end

	-- プレイヤーの移動方向のベクトルを算出
	do
		-- 方向ボタン「↑」を押したときのプレイヤーの移動ベクトルはカメラの視線方向からＹ成分を抜いたもの
		UpMoveVec = dx.VSub(cam.Target, cam.Eye);
		UpMoveVec.y = 0.0;

		-- 方向ボタン「←」を押したときのプレイヤーの移動ベクトルは上を押したときの方向ベクトルとＹ軸のプラス方向のベクトルに垂直な方向
		LeftMoveVec = dx.VCross(UpMoveVec, dx.VGet(0.0, 1.0, 0.0));

		-- 二つのベクトルを正規化( ベクトルの長さを１．０にすること )
		UpMoveVec = dx.VNorm(UpMoveVec);
		LeftMoveVec = dx.VNorm(LeftMoveVec);
	end

	-- このフレームでの移動ベクトルを初期化
	MoveVec = dx.VGet(0.0, 0.0, 0.0);

	-- 移動したかどうかのフラグを初期状態では「移動していない」を表す０にする
	MoveFlag = 0;

	-- パッドの３ボタンと左シフトがどちらも押されていなかったらプレイヤーの移動処理
	if dx.CheckHitKey(dx.KEY_INPUT_LSHIFT) == 0 and (bit.band(inp.NowInput, dx.PAD_INPUT_C) ~= 0) == 0 then
		-- 方向ボタン「←」が入力されたらカメラの見ている方向から見て左方向に移動する
		if bit.band(inp.NowInput, dx.PAD_INPUT_LEFT) ~= 0 then
			-- 移動ベクトルに「←」が入力された時の移動ベクトルを加算する
			MoveVec = dx.VAdd(MoveVec, LeftMoveVec);

			-- 移動したかどうかのフラグを「移動した」にする
			MoveFlag = 1;
		else
			-- 方向ボタン「→」が入力されたらカメラの見ている方向から見て右方向に移動する
			if bit.band(inp.NowInput, dx.PAD_INPUT_RIGHT) ~= 0 then
				-- 移動ベクトルに「←」が入力された時の移動ベクトルを反転したものを加算する
				MoveVec = dx.VAdd(MoveVec, dx.VScale(LeftMoveVec, -1.0));

				-- 移動したかどうかのフラグを「移動した」にする
				MoveFlag = 1;
            end
        end

		-- 方向ボタン「↑」が入力されたらカメラの見ている方向に移動する
		if bit.band(inp.NowInput, dx.PAD_INPUT_UP) ~= 0 then
			-- 移動ベクトルに「↑」が入力された時の移動ベクトルを加算する
			MoveVec = dx.VAdd(MoveVec, UpMoveVec);

			-- 移動したかどうかのフラグを「移動した」にする
			MoveFlag = 1;
		else
			-- 方向ボタン「↓」が入力されたらカメラの方向に移動する
			if bit.band(inp.NowInput, dx.PAD_INPUT_DOWN) ~= 0 then
				-- 移動ベクトルに「↑」が入力された時の移動ベクトルを反転したものを加算する
				MoveVec = dx.VAdd(MoveVec, dx.VScale(UpMoveVec, -1.0));

				-- 移動したかどうかのフラグを「移動した」にする
				MoveFlag = 1;
            end
        end

		-- プレイヤーの状態が「ジャンプ」ではなく、且つボタン１が押されていたらジャンプする
		if pl.State ~= 2 and (bit.band(inp.EdgeInput, dx.PAD_INPUT_A) ~= 0) then
			-- 状態を「ジャンプ」にする
			pl.State = 2;

			-- Ｙ軸方向の速度をセット
			pl.JumpPower = PLAYER_JUMP_POWER;

			-- ジャンプアニメーションの再生
			Player_PlayAnim(2);
		end
	end

	-- 移動ボタンが押されたかどうかで処理を分岐
	if MoveFlag then
		-- 移動ベクトルを正規化したものをプレイヤーが向くべき方向として保存
		pl.TargetMoveDirection = dx.VNorm(MoveVec);

		-- プレイヤーが向くべき方向ベクトルをプレイヤーのスピード倍したものを移動ベクトルとする
		MoveVec = dx.VScale(pl.TargetMoveDirection, PLAYER_MOVE_SPEED);

		-- もし今まで「立ち止まり」状態だったら
		if pl.State == 0 then
			-- 走りアニメーションを再生する
			Player_PlayAnim(1);

			-- 状態を「走り」にする
			pl.State = 1;
		end
	else
		-- このフレームで移動していなくて、且つ状態が「走り」だったら
		if pl.State == 1 then
			-- 立ち止りアニメーションを再生する
			Player_PlayAnim(4);

			-- 状態を「立ち止り」にする
			pl.State = 0;
		end
	end

	-- 状態が「ジャンプ」の場合は
	if pl.State == 2 then
		-- Ｙ軸方向の速度を重力分減算する
		pl.JumpPower = pl.JumpPower - PLAYER_GRAVITY;

		-- もし落下していて且つ再生されているアニメーションが上昇中用のものだった場合は
		if pl.JumpPower < 0.0 and dx.MV1GetAttachAnim(pl.ModelHandle, pl.PlayAnim1) == 2 then
			-- 落下中ようのアニメーションを再生する
			Player_PlayAnim(3);
		end

		-- 移動ベクトルのＹ成分をＹ軸方向の速度にする
		MoveVec.y = pl.JumpPower;
	end

	-- プレイヤーの移動方向にモデルの方向を近づける
	Player_AngleProcess();

	-- 移動ベクトルを元にコリジョンを考慮しつつプレイヤーを移動
	Player_Move(MoveVec);

	-- アニメーション処理
	Player_AnimProcess();
end

-- プレイヤーの移動処理
function Player_Move(MoveVector)
	--local i, j, k;						-- 汎用カウンタ変数
	local MoveFlag;						-- 水平方向に移動したかどうかのフラグ( ０:移動していない  １:移動した )
	local HitFlag;						-- ポリゴンに当たったかどうかを記憶しておくのに使う変数( ０:当たっていない  １:当たった )
	local HitDim = dx.MV1_COLL_RESULT_POLY_DIM{};			-- プレイヤーの周囲にあるポリゴンを検出した結果が代入される当たり判定結果構造体
	local KabeNum;						-- 壁ポリゴンと判断されたポリゴンの数
	local YukaNum;						-- 床ポリゴンと判断されたポリゴンの数
	local Kabe = {};	-- 壁ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
	local Yuka = {};	-- 床ポリゴンと判断されたポリゴンの構造体のアドレスを保存しておくためのポインタ配列
	local Poly;				-- ポリゴンの構造体にアクセスするために使用するポインタ( 使わなくても済ませられますがプログラムが長くなるので・・・ )
	local LineRes = dx.HITRESULT_LINE{};				-- 線分とポリゴンとの当たり判定の結果を代入する構造体
	local OldPos = dx.VECTOR{};						-- 移動前の座標
	local NowPos = dx.VECTOR{};						-- 移動後の座標

	-- 移動前の座標を保存
	OldPos = pl.Position;

	-- 移動後の座標を算出
	NowPos = dx.VAdd(pl.Position, MoveVector);

	-- プレイヤーの周囲にあるステージポリゴンを取得する
	-- ( 検出する範囲は移動距離も考慮する )
	HitDim = dx.MV1CollCheck_Sphere(stg.ModelHandle, -1, pl.Position, PLAYER_ENUM_DEFAULT_SIZE + dx.VSize(MoveVector));

	-- x軸かy軸方向に 0.01 以上移動した場合は「移動した」フラグを１にする
	if math.abs(MoveVector.x) > 0.01 or math.abs(MoveVector.z) > 0.01 then
		MoveFlag = 1;
	else
		MoveFlag = 0;
	end

	-- 検出されたポリゴンが壁ポリゴン( ＸＺ平面に垂直なポリゴン )か床ポリゴン( ＸＺ平面に垂直ではないポリゴン )かを判断する
	do
		-- 壁ポリゴンと床ポリゴンの数を初期化する
		KabeNum = 1;
		YukaNum = 1;

		-- 検出されたポリゴンの数だけ繰り返し
		for i = 1, HitDim.HitNum do
			-- ＸＺ平面に垂直かどうかはポリゴンの法線のＹ成分が０に限りなく近いかどうかで判断する
			if HitDim.Dim[i].Normal.y < 0.000001 and HitDim.Dim[i].Normal.y > -0.000001 then
				-- 壁ポリゴンと判断された場合でも、プレイヤーのＹ座標＋１．０ｆより高いポリゴンのみ当たり判定を行う
				if HitDim.Dim[i].Position[1].y > pl.Position.y + 1.0 or
					HitDim.Dim[i].Position[2].y > pl.Position.y + 1.0 or
					HitDim.Dim[i].Position[3].y > pl.Position.y + 1.0 then
					-- ポリゴンの数が列挙できる限界数に達していなかったらポリゴンを配列に追加
					if KabeNum < PLAYER_MAX_HITCOLL then
						-- ポリゴンの構造体のアドレスを壁ポリゴンポインタ配列に保存する
						Kabe[KabeNum] = HitDim.Dim[i];

						-- 壁ポリゴンの数を加算する
						KabeNum = KabeNum + 1
					end
				end
			else
				-- ポリゴンの数が列挙できる限界数に達していなかったらポリゴンを配列に追加
				if YukaNum < PLAYER_MAX_HITCOLL then
					-- ポリゴンの構造体のアドレスを床ポリゴンポインタ配列に保存する
					Yuka[YukaNum] = HitDim.Dim[i];

					-- 床ポリゴンの数を加算する
					YukaNum = YukaNum + 1;
				end
			end
		end
	end

	-- 壁ポリゴンとの当たり判定処理
	if KabeNum ~= 0 then
		-- 壁に当たったかどうかのフラグは初期状態では「当たっていない」にしておく
		HitFlag = 0;

		-- 移動したかどうかで処理を分岐
		if MoveFlag == 1 then
			-- 壁ポリゴンの数だけ繰り返し
			for i = 1, KabeNum do
				-- i番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
				Poly = Kabe[i];

				-- ポリゴンとプレイヤーが当たっていなかったら次のカウントへ
				if dx.HitCheck_Capsule_Triangle(
                    NowPos,
                    dx.VAdd(NowPos, dx.VGet(0.0, PLAYER_HIT_HEIGHT, 0.0)),
                    PLAYER_HIT_WIDTH,
                    Poly.Position[1], Poly.Position[2], Poly.Position[3]
                ) == false then
                    -- continue
                else
                    -- ここにきたらポリゴンとプレイヤーが当たっているということなので、ポリゴンに当たったフラグを立てる
                    HitFlag = 1;

                    -- 壁に当たったら壁に遮られない移動成分分だけ移動する
                    do
                        local SlideVec = dx.VECTOR{};	-- プレイヤーをスライドさせるベクトル

                        -- 進行方向ベクトルと壁ポリゴンの法線ベクトルに垂直なベクトルを算出
                        SlideVec = dx.VCross(MoveVector, Poly.Normal);

                        -- 算出したベクトルと壁ポリゴンの法線ベクトルに垂直なベクトルを算出、これが
                        -- 元の移動成分から壁方向の移動成分を抜いたベクトル
                        SlideVec = dx.VCross(Poly.Normal, SlideVec);

                        -- それを移動前の座標に足したものを新たな座標とする
                        NowPos = dx.VAdd(OldPos, SlideVec);
                    end

                    -- 新たな移動座標で壁ポリゴンと当たっていないかどうかを判定する
                    local jndex = KabeNum
                    for j = 1, KabeNum do
                        -- j番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
                        Poly = Kabe[j];

                        -- 当たっていたらループから抜ける
                        if dx.HitCheck_Capsule_Triangle(
                            NowPos,
                            dx.VAdd(NowPos, dx.VGet(0.0, PLAYER_HIT_HEIGHT, 0.0)),
                            PLAYER_HIT_WIDTH,
                            Poly.Position[1], Poly.Position[2], Poly.Position[3]
                        ) then
                            jndex = j
                            break;
                        end
                    end

                    -- j が KabeNum だった場合はどのポリゴンとも当たらなかったということなので
                    -- 壁に当たったフラグを倒した上でループから抜ける
                    if jndex == KabeNum then
                        HitFlag = 0;
                        break;
                    end
                end
			end
		else
			-- 移動していない場合の処理

			-- 壁ポリゴンの数だけ繰り返し
			for i = 1, KabeNum do
				-- i番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
				Poly = Kabe[i];

				-- ポリゴンに当たっていたら当たったフラグを立てた上でループから抜ける
				if dx.HitCheck_Capsule_Triangle(
                    NowPos,
                    dx.VAdd(NowPos, dx.VGet(0.0, PLAYER_HIT_HEIGHT, 0.0)),
                    PLAYER_HIT_WIDTH,
                    Poly.Position[1], Poly.Position[2], Poly.Position[3]
                ) then
					HitFlag = 1;
					break;
				end
			end
		end

		-- 壁に当たっていたら壁から押し出す処理を行う
		if HitFlag == 1 then
			-- 壁からの押し出し処理を試みる最大数だけ繰り返し
			for k = 1, PLAYER_HIT_TRYNUM do
                -- 壁ポリゴンの数だけ繰り返し
                local index = KabeNum
				for i = 1, KabeNum do
					-- i番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
					Poly = Kabe[i];

					-- プレイヤーと当たっているかを判定
					if dx.HitCheck_Capsule_Triangle(
                        NowPos, dx.VAdd(NowPos, dx.VGet(0.0, PLAYER_HIT_HEIGHT, 0.0)), PLAYER_HIT_WIDTH,
                        Poly.Position[1], Poly.Position[2], Poly.Position[3]
                    ) == false then
                        --continue;
                    else
                        -- 当たっていたら規定距離分プレイヤーを壁の法線方向に移動させる
                        NowPos = dx.VAdd(NowPos, dx.VScale(Poly.Normal, PLAYER_HIT_SLIDE_LENGTH));

                        -- 移動した上で壁ポリゴンと接触しているかどうかを判定
                        local jndex = KabeNum
                        for j = 1, KabeNum do
                            -- 当たっていたらループを抜ける
                            Poly = Kabe[j];
                            if dx.HitCheck_Capsule_Triangle(
                                NowPos, dx.VAdd(NowPos, dx.VGet(0.0, PLAYER_HIT_HEIGHT, 0.0)), PLAYER_HIT_WIDTH,
                                Poly.Position[1], Poly.Position[2], Poly.Position[3]
                            ) == true then
                                jndex = j
                                break;
                            end
                        end

                        -- 全てのポリゴンと当たっていなかったらここでループ終了
                        if jndex == KabeNum then
                            index = i
                            break
                        end
                    end
				end

				-- i が KabeNum ではない場合は全部のポリゴンで押し出しを試みる前に全ての壁ポリゴンと接触しなくなったということなのでループから抜ける
                if index ~= KabeNum then
                    break
                end
			end
		end
	end

	-- 床ポリゴンとの当たり判定
	if YukaNum ~= 0 then
		-- ジャンプ中且つ上昇中の場合は処理を分岐
		if pl.State == 2 and pl.JumpPower > 0.0 then
			local MinY;

			-- 天井に頭をぶつける処理を行う

			-- 一番低い天井にぶつける為の判定用変数を初期化
			MinY = 0.0;

			-- 当たったかどうかのフラグを当たっていないを意味する０にしておく
			HitFlag = 0;

			-- 床ポリゴンの数だけ繰り返し
			for i = 1, YukaNum do
				-- i番目の床ポリゴンのアドレスを床ポリゴンポインタ配列から取得
				Poly = Yuka[i];

				-- 足先から頭の高さまでの間でポリゴンと接触しているかどうかを判定
				LineRes = dx.HitCheck_Line_Triangle(
                    NowPos, dx.VAdd(NowPos, dx.VGet(0.0, PLAYER_HIT_HEIGHT, 0.0)),
                    Poly.Position[1], Poly.Position[2], Poly.Position[3]
                );

                if LineRes.HitFlag == false then
				    -- 接触していなかったら何もしない
                    --continue;
                elseif HitFlag == 1 and MinY < LineRes.Position.y then
				    -- 既にポリゴンに当たっていて、且つ今まで検出した天井ポリゴンより高い場合は何もしない
                    --continue;
                else
                    -- ポリゴンに当たったフラグを立てる
                    HitFlag = 1;

                    -- 接触したＹ座標を保存する
                    MinY = LineRes.Position.y;
                end
			end

			-- 接触したポリゴンがあったかどうかで処理を分岐
			if HitFlag == 1 then
				-- 接触した場合はプレイヤーのＹ座標を接触座標を元に更新
				NowPos.y = MinY - PLAYER_HIT_HEIGHT;

				-- Ｙ軸方向の速度は反転
				pl.JumpPower = -pl.JumpPower;
			end
		else
			local MaxY;

			-- 下降中かジャンプ中ではない場合の処理

			-- 床ポリゴンに当たったかどうかのフラグを倒しておく
			HitFlag = 0;

			-- 一番高い床ポリゴンにぶつける為の判定用変数を初期化
			MaxY = 0.0;

			-- 床ポリゴンの数だけ繰り返し
			for i = 1, YukaNum do
				-- i番目の床ポリゴンのアドレスを床ポリゴンポインタ配列から取得
				Poly = Yuka[i];

				-- ジャンプ中かどうかで処理を分岐
				if pl.State == 2 then
					-- ジャンプ中の場合は頭の先から足先より少し低い位置の間で当たっているかを判定
					LineRes = dx.HitCheck_Line_Triangle(
                        dx.VAdd(NowPos, dx.VGet(0.0, PLAYER_HIT_HEIGHT, 0.0)),
                        dx.VAdd(NowPos, dx.VGet(0.0, -1.0, 0.0)),
                        Poly.Position[1], Poly.Position[2], Poly.Position[3]
                    );
				else
					-- 走っている場合は頭の先からそこそこ低い位置の間で当たっているかを判定( 傾斜で落下状態に移行してしまわない為 )
					LineRes = dx.HitCheck_Line_Triangle(
                        dx.VAdd(NowPos, dx.VGet(0.0, PLAYER_HIT_HEIGHT, 0.0)),
                        dx.VAdd(NowPos, dx.VGet(0.0, -40.0, 0.0)),
                        Poly.Position[1], Poly.Position[2], Poly.Position[3]
                    );
				end

                if (LineRes.HitFlag == false) then
				    -- 当たっていなかったら何もしない
                    -- continue;
                elseif (HitFlag == 1 and MaxY > LineRes.Position.y) then
				    -- 既に当たったポリゴンがあり、且つ今まで検出した床ポリゴンより低い場合は何もしない
                    --continue;
                else
                    -- ポリゴンに当たったフラグを立てる
                    HitFlag = 1;

                    -- 接触したＹ座標を保存する
                    MaxY = LineRes.Position.y;
                end
			end

			-- 床ポリゴンに当たったかどうかで処理を分岐
			if HitFlag == 1 then
				-- 当たった場合

				-- 接触したポリゴンで一番高いＹ座標をプレイヤーのＹ座標にする
				NowPos.y = MaxY;

				-- Ｙ軸方向の移動速度は０に
				pl.JumpPower = 0.0;

				-- もしジャンプ中だった場合は着地状態にする
				if pl.State == 2 then
					-- 移動していたかどうかで着地後の状態と再生するアニメーションを分岐する
					if MoveFlag then
						-- 移動している場合は走り状態に
						Player_PlayAnim(1);
						pl.State = 1;
					else
						-- 移動していない場合は立ち止り状態に
						Player_PlayAnim(4);
						pl.State = 0;
					end

					-- 着地時はアニメーションのブレンドは行わない
					pl.AnimBlendRate = 1.0;
				end
			else
				-- 床コリジョンに当たっていなくて且つジャンプ状態ではなかった場合は
				if pl.State ~= 2 then
					-- ジャンプ中にする
					pl.State = 2;

					-- ちょっとだけジャンプする
					pl.JumpPower = PLAYER_FALL_UP_POWER;

					-- アニメーションは落下中のものにする
					Player_PlayAnim(3);
				end
			end
		end
	end

	-- 新しい座標を保存する
	pl.Position = NowPos;

	-- プレイヤーのモデルの座標を更新する
	dx.MV1SetPosition(pl.ModelHandle, pl.Position);

	-- 検出したプレイヤーの周囲のポリゴン情報を開放する
	dx.MV1CollResultPolyDimTerminate(HitDim);
end

-- プレイヤーの向きを変える処理
function Player_AngleProcess()
	local TargetAngle;			-- 目標角度
	local SaAngle;				-- 目標角度と現在の角度との差

	-- 目標の方向ベクトルから角度値を算出する
	TargetAngle = math.atan2(pl.TargetMoveDirection.x, pl.TargetMoveDirection.z);

	-- 目標の角度と現在の角度との差を割り出す
	do
		-- 最初は単純に引き算
		SaAngle = TargetAngle - pl.Angle;

		-- ある方向からある方向の差が１８０度以上になることは無いので
		-- 差の値が１８０度以上になっていたら修正する
		if SaAngle < -dx.DX_PI_F then
			SaAngle = SaAngle + dx.DX_TWO_PI_F;
		else
			if SaAngle > dx.DX_PI_F then
				SaAngle = SaAngle - dx.DX_TWO_PI_F;
            end
        end
	end

	-- 角度の差が０に近づける
	if SaAngle > 0.0 then
		-- 差がプラスの場合は引く
		SaAngle = SaAngle - PLAYER_ANGLE_SPEED;
		if SaAngle < 0.0 then
			SaAngle = 0.0;
		end
	else
		-- 差がマイナスの場合は足す
		SaAngle = SaAngle + PLAYER_ANGLE_SPEED;
		if SaAngle > 0.0 then
			SaAngle = 0.0;
		end
	end

	-- モデルの角度を更新
	pl.Angle = TargetAngle - SaAngle;
	dx.MV1SetRotationXYZ(pl.ModelHandle, dx.VGet(0.0, pl.Angle + dx.DX_PI_F, 0.0));
end

-- プレイヤーに新たなアニメーションを再生する
function Player_PlayAnim(PlayAnim)
	-- 再生中のモーション２が有効だったらデタッチする
	if pl.PlayAnim2 ~= -1 then
		dx.MV1DetachAnim(pl.ModelHandle, pl.PlayAnim2);
		pl.PlayAnim2 = -1;
	end

	-- 今まで再生中のモーション１だったものの情報を２に移動する
	pl.PlayAnim2 = pl.PlayAnim1;
	pl.AnimPlayCount2 = pl.AnimPlayCount1;

	-- 新たに指定のモーションをモデルにアタッチして、アタッチ番号を保存する
	pl.PlayAnim1 = dx.MV1AttachAnim(pl.ModelHandle, PlayAnim);
	pl.AnimPlayCount1 = 0.0;

	-- ブレンド率は再生中のモーション２が有効ではない場合は１．０ｆ( 再生中のモーション１が１００％の状態 )にする
	pl.AnimBlendRate = pl.PlayAnim2 == -1 and 1.0 or 0.0;
end

-- プレイヤーのアニメーション処理
function Player_AnimProcess()
	local AnimTotalTime;		-- 再生しているアニメーションの総時間

	-- ブレンド率が１以下の場合は１に近づける
	if pl.AnimBlendRate < 1.0 then
		pl.AnimBlendRate = pl.AnimBlendRate + PLAYER_ANIM_BLEND_SPEED;
		if pl.AnimBlendRate > 1.0 then
			pl.AnimBlendRate = 1.0;
		end
	end

	-- 再生しているアニメーション１の処理
	if pl.PlayAnim1 ~= -1 then
		-- アニメーションの総時間を取得
		AnimTotalTime = dx.MV1GetAttachAnimTotalTime(pl.ModelHandle, pl.PlayAnim1);

		-- 再生時間を進める
		pl.AnimPlayCount1 = pl.AnimPlayCount1 + PLAYER_PLAY_ANIM_SPEED;

		-- 再生時間が総時間に到達していたら再生時間をループさせる
		if pl.AnimPlayCount1 >= AnimTotalTime then
			pl.AnimPlayCount1 = math.fmod(pl.AnimPlayCount1, AnimTotalTime);
		end

		-- 変更した再生時間をモデルに反映させる
		dx.MV1SetAttachAnimTime(pl.ModelHandle, pl.PlayAnim1, pl.AnimPlayCount1);

		-- アニメーション１のモデルに対する反映率をセット
		dx.MV1SetAttachAnimBlendRate(pl.ModelHandle, pl.PlayAnim1, pl.AnimBlendRate);
	end

	-- 再生しているアニメーション２の処理
	if pl.PlayAnim2 ~= -1 then
		-- アニメーションの総時間を取得
		AnimTotalTime = dx.MV1GetAttachAnimTotalTime(pl.ModelHandle, pl.PlayAnim2);

		-- 再生時間を進める
		pl.AnimPlayCount2 = pl.AnimPlayCount2 + PLAYER_PLAY_ANIM_SPEED;

		-- 再生時間が総時間に到達していたら再生時間をループさせる
		if pl.AnimPlayCount2 > AnimTotalTime then
			pl.AnimPlayCount2 = math.fmod(pl.AnimPlayCount2, AnimTotalTime);
		end

		-- 変更した再生時間をモデルに反映させる
		dx.MV1SetAttachAnimTime(pl.ModelHandle, pl.PlayAnim2, pl.AnimPlayCount2);

		-- アニメーション２のモデルに対する反映率をセット
		dx.MV1SetAttachAnimBlendRate(pl.ModelHandle, pl.PlayAnim2, 1.0 - pl.AnimBlendRate);
	end
end

-- プレイヤーの影を描画
function Player_ShadowRender()
	local HitResDim = dx.MV1_COLL_RESULT_POLY_DIM;
	local HitRes;
	local Vertex = { dx.VERTEX3D{}, dx.VERTEX3D{}, dx.VERTEX3D{} }
	local SlideVec = dx.VECTOR{};

	-- ライティングを無効にする
	dx.SetUseLighting(false);

	-- Ｚバッファを有効にする
	dx.SetUseZBuffer3D(true);

	-- テクスチャアドレスモードを CLAMP にする( テクスチャの端より先は端のドットが延々続く )
	dx.SetTextureAddressMode(dx.DX_TEXADDRESS_CLAMP);

	-- プレイヤーの直下に存在する地面のポリゴンを取得
	HitResDim = dx.MV1CollCheck_Capsule(
        stg.ModelHandle,
        -1,
        pl.Position,
        dx.VAdd(pl.Position, dx.VGet(0.0, -PLAYER_SHADOW_HEIGHT, 0.0)),
        PLAYER_SHADOW_SIZE
    );

	-- 頂点データで変化が無い部分をセット
	Vertex[0].dif = dx.GetColorU8(255, 255, 255, 255);
	Vertex[0].spc = dx.GetColorU8(0, 0, 0, 0);
	Vertex[0].su = 0.0;
	Vertex[0].sv = 0.0;
	Vertex[1] = Vertex[0];
	Vertex[2] = Vertex[0];

	-- 球の直下に存在するポリゴンの数だけ繰り返し
	HitRes = HitResDim.Dim;
    for i = 1, HitResDim.HitNum do
        HitRes = HitRes + 1

		-- ポリゴンの座標は地面ポリゴンの座標
		Vertex[0].pos = HitRes.Position[0];
		Vertex[1].pos = HitRes.Position[1];
		Vertex[2].pos = HitRes.Position[2];

		-- ちょっと持ち上げて重ならないようにする
		SlideVec = dx.VScale(HitRes.Normal, 0.5);
		Vertex[0].pos = dx.VAdd(Vertex[0].pos, SlideVec);
		Vertex[1].pos = dx.VAdd(Vertex[1].pos, SlideVec);
		Vertex[2].pos = dx.VAdd(Vertex[2].pos, SlideVec);

		-- ポリゴンの不透明度を設定する
		Vertex[0].dif.a = 0;
		Vertex[1].dif.a = 0;
		Vertex[2].dif.a = 0;
		if (HitRes.Position[0].y > pl.Position.y - PLAYER_SHADOW_HEIGHT) then
            Vertex[0].dif.a = 128 * (1.0 - math.abs(HitRes.Position[0].y - pl.Position.y) / PLAYER_SHADOW_HEIGHT);
        end

		if (HitRes.Position[1].y > pl.Position.y - PLAYER_SHADOW_HEIGHT) then
            Vertex[1].dif.a = 128 * (1.0 - math.abs(HitRes.Position[1].y - pl.Position.y) / PLAYER_SHADOW_HEIGHT);
        end

		if (HitRes.Position[2].y > pl.Position.y - PLAYER_SHADOW_HEIGHT) then
            Vertex[2].dif.a = 128 * (1.0 - math.abs(HitRes.Position[2].y - pl.Position.y) / PLAYER_SHADOW_HEIGHT);
        end

		-- ＵＶ値は地面ポリゴンとプレイヤーの相対座標から割り出す
		Vertex[0].u = (HitRes.Position[0].x - pl.Position.x) / (PLAYER_SHADOW_SIZE * 2.0) + 0.5;
		Vertex[0].v = (HitRes.Position[0].z - pl.Position.z) / (PLAYER_SHADOW_SIZE * 2.0) + 0.5;
		Vertex[1].u = (HitRes.Position[1].x - pl.Position.x) / (PLAYER_SHADOW_SIZE * 2.0) + 0.5;
		Vertex[1].v = (HitRes.Position[1].z - pl.Position.z) / (PLAYER_SHADOW_SIZE * 2.0) + 0.5;
		Vertex[2].u = (HitRes.Position[2].x - pl.Position.x) / (PLAYER_SHADOW_SIZE * 2.0) + 0.5;
		Vertex[2].v = (HitRes.Position[2].z - pl.Position.z) / (PLAYER_SHADOW_SIZE * 2.0) + 0.5;

		-- 影ポリゴンを描画
		dx.DrawPolygon3D(Vertex, 1, pl.ShadowHandle, true);
	end

	-- 検出した地面ポリゴン情報の後始末
	dx.MV1CollResultPolyDimTerminate(HitResDim);

	-- ライティングを有効にする
	dx.SetUseLighting(true);

	-- Ｚバッファを無効にする
	dx.SetUseZBuffer3D(false);
end

-- ステージの初期化処理
function Stage_Initialize()
	-- ステージモデルの読み込み
	stg.ModelHandle = dx.MV1LoadModel("ColTestStage.mqo");

	-- モデル全体のコリジョン情報のセットアップ
	dx.MV1SetupCollInfo(stg.ModelHandle, -1);
end

-- ステージの後始末処理
function Stage_Terminate()
	-- ステージモデルの後始末
	dx.MV1DeleteModel(stg.ModelHandle);
end

-- カメラの初期化処理
function Camera_Initialize()
	-- カメラの初期水平角度は１８０度
	cam.AngleH = dx.DX_PI_F;

	-- 垂直角度は０度
	cam.AngleV = 0.0;
end

-- カメラの処理
function Camera_Process()
	-- パッドの３ボタンか、シフトキーが押されている場合のみ角度変更操作を行う
	if dx.CheckHitKey(dx.KEY_INPUT_LSHIFT) or (bit.band(inp.NowInput, dx.PAD_INPUT_C) ~= 0) then
		-- 「←」ボタンが押されていたら水平角度をマイナスする
		if bit.band(inp.NowInput, dx.PAD_INPUT_LEFT) ~= 0 then
			cam.AngleH = cam.AngleH - CAMERA_ANGLE_SPEED;

			-- －１８０度以下になったら角度値が大きくなりすぎないように３６０度を足す
			if cam.AngleH < -dx.DX_PI_F then
				cam.AngleH = cam.AngleH + dx.DX_TWO_PI_F;
			end
		end

		-- 「→」ボタンが押されていたら水平角度をプラスする
		if bit.band(inp.NowInput, dx.PAD_INPUT_RIGHT) ~= 0 then
			cam.AngleH = cam.AngleH + CAMERA_ANGLE_SPEED;

			-- １８０度以上になったら角度値が大きくなりすぎないように３６０度を引く
			if cam.AngleH > dx.DX_PI_F then
				cam.AngleH = cam.AngleH - dx.DX_TWO_PI_F;
			end
		end

		-- 「↑」ボタンが押されていたら垂直角度をマイナスする
		if bit.band(inp.NowInput, dx.PAD_INPUT_UP) ~= 0 then
			cam.AngleV = cam.AngleV - CAMERA_ANGLE_SPEED;

			-- ある一定角度以下にはならないようにする
			if cam.AngleV < -dx.DX_PI_F / 2.0 + 0.6 then
				cam.AngleV = -dx.DX_PI_F / 2.0 + 0.6;
			end
		end

		-- 「↓」ボタンが押されていたら垂直角度をプラスする
		if bit.band(inp.NowInput, dx.PAD_INPUT_DOWN) ~= 0 then
			cam.AngleV = cam.AngleV + CAMERA_ANGLE_SPEED;

			-- ある一定角度以上にはならないようにする
			if cam.AngleV > dx.DX_PI_F / 2.0 - 0.6 then
				cam.AngleV = dx.DX_PI_F / 2.0 - 0.6;
			end
		end
	end

	-- カメラの注視点はプレイヤー座標から規定値分高い座標
	cam.Target = dx.VAdd(pl.Position, dx.VGet(0.0, CAMERA_PLAYER_TARGET_HEIGHT, 0.0));

	-- カメラの座標を決定する
	do
		local RotZ = dx.MATRIX{}, RotY;
		local Camera_Player_Length;
		local HRes = dx.MV1_COLL_RESULT_POLY_DIM{};
		local HitNum;

		-- 水平方向の回転はＹ軸回転
		RotY = dx.MGetRotY(cam.AngleH);

		-- 垂直方向の回転はＺ軸回転 )
		RotZ = dx.MGetRotZ(cam.AngleV);

		-- カメラからプレイヤーまでの初期距離をセット
		Camera_Player_Length = CAMERA_PLAYER_LENGTH;

		-- カメラの座標を算出
		-- Ｘ軸にカメラとプレイヤーとの距離分だけ伸びたベクトルを
		-- 垂直方向回転( Ｚ軸回転 )させたあと水平方向回転( Ｙ軸回転 )して更に
		-- 注視点の座標を足したものがカメラの座標
		cam.Eye = dx.VAdd(dx.VTransform(dx.VTransform(dx.VGet(-Camera_Player_Length, 0.0, 0.0), RotZ), RotY), cam.Target);

		-- 注視点からカメラの座標までの間にステージのポリゴンがあるか調べる
		HRes = dx.MV1CollCheck_Capsule(stg.ModelHandle, -1, cam.Target, cam.Eye, CAMERA_COLLISION_SIZE);
		HitNum = HRes.HitNum;
		dx.MV1CollResultPolyDimTerminate(HRes);
		if HitNum ~= 0 then
			local NotHitLength;
			local HitLength;
			local TestLength;
			local TestPosition = dx.VECTOR{};

			-- あったら無い位置までプレイヤーに近づく

			-- ポリゴンに当たらない距離をセット
			NotHitLength = 0.0;

			-- ポリゴンに当たる距離をセット
			HitLength = Camera_Player_Length;
			repeat
				-- 当たるかどうかテストする距離をセット( 当たらない距離と当たる距離の中間 )
				TestLength = NotHitLength + (HitLength - NotHitLength) / 2.0;

				-- テスト用のカメラ座標を算出
				TestPosition = dx.VAdd(dx.VTransform(dx.VTransform(dx.VGet(-TestLength, 0.0, 0.0), RotZ), RotY), cam.Target);

				-- 新しい座標で壁に当たるかテスト
				HRes = dx.MV1CollCheck_Capsule(stg.ModelHandle, -1, cam.Target, TestPosition, CAMERA_COLLISION_SIZE);
				HitNum = HRes.HitNum;
				dx.MV1CollResultPolyDimTerminate(HRes);
				if HitNum ~= 0 then
					-- 当たったら当たる距離を TestLength に変更する
					HitLength = TestLength;
				else
					-- 当たらなかったら当たらない距離を TestLength に変更する
					NotHitLength = TestLength;
				end

				-- HitLength と NoHitLength が十分に近づいていなかったらループ
			until ((HitLength - NotHitLength) > 0.1);

			-- カメラの座標をセット
			cam.Eye = TestPosition;
		end
	end

	-- カメラの情報をライブラリのカメラに反映させる
	cam.SetEye = dx.VAdd(cam.SetEye, dx.VScale(dx.VSub(cam.Eye, cam.SetEye), 0.2));
	cam.SetTarget = dx.VAdd(cam.SetTarget, dx.VScale(dx.VSub(cam.Target, cam.SetTarget), 0.2));
	dx.SetCameraPositionAndTarget_UpVecY(cam.SetEye, cam.SetTarget);
end

-- 描画処理
function Render_Process()
	-- ステージモデルの描画
	dx.MV1DrawModel(stg.ModelHandle);

	-- プレイヤーモデルの描画
	dx.MV1DrawModel(pl.ModelHandle);

	-- プレイヤーの影の描画
	Player_ShadowRender();
end
