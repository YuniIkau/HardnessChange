#include "DI2018_Ver1_0.h"


namespace DI
{
	float deadzone = 0.24f;//スティック判定の加減
	const  int    analogMax = 32768;
	const  float  digToAnalog = 1.0f / analogMax;
	const  float  digToAnalog_TG = 1.0f / 255;

	Obj::WP Obj::winst;	//	１つしか存在できないインスタンス
	vector<GamePad::WP>			Obj::gpads;
	vector<Mouse::WP>			Obj::mouses;

	class  Pad : public GamePad
	{
	public:
		typedef  shared_ptr<Pad>  SP;
		typedef  weak_ptr<Pad>  WP;

	private:
		XINPUT_VIBRATION vibration;//ぶるぶる情報
		int	leftVibFrame;
		int	rightVibFrame;
		bool connected;//コネクトされているか
		DWORD  pad_ID;

		Pad(void);

		struct KeyParam {
			RGP		gKey;	//ゲームパッドのボタン
			VGP       pKey;	//ボタンの種類（ＰＧ上での名称）
			ButtonState		state;	//キーの状態
		};
		void  Check();
		vector<KeyParam>     buttons;
	private:
		bool  Initialize(
			const DWORD&				id_,//0～3まで
			const KeyDatas_GP&			buttonGP_);

		Err  CreateDevice(DWORD  id_);
		bool checkPush(const VGP  bt_, const DWORD  state_);
	protected:
	public:
		ButtonState  CheckBT(VGP		kk_);
		//~Pad();
		//	更新
		void  B_UpDate();
		//	デバイスを生成する
		static  SP  Create(
			const DWORD&				id_,//0～3まで
			const KeyDatas_GP&			buttonGP_);
		void  SetVibrationL(float  pow_, int  frame_);
		void  SetVibrationR(float  pow_, int  frame_);
	};

	class  KeyBoard : public GamePad
	{
	public:
		typedef  shared_ptr<KeyBoard>  SP;
		typedef  weak_ptr<KeyBoard>  WP;


	private:
		IDirectInputDevice8*  device;
		bool connected;//コネクトされているか
		KeyBoard(void);

		struct KeyParam {
			int					vKey;	//キーボードのバーチャルキー
			VGP			pKey;	//ボタンの種類（ＰＧ上での名称）
			ButtonState			state;	//キーの状態
		};
		void  Check();
		vector<KeyParam>     buttons;
		vector<DWORD>         kbAxisL;		//左スティック対応キー
		vector<DWORD>         kbAxisR;		//右スティック対応キー
		vector<DWORD>         kbTrigger;	//ＬＲトリガー対応キー



	private:
		bool  Initialize(
			const AnalogAxisKB&			axisL_,
			const AnalogAxisKB&			axisR_,
			const AnalogTriggerKB&		trigger_,
			const KeyDatas_KB&			buttonKB_);

		Err  CreateDevice(DWORD  id_);

	protected:

	public:
		ButtonState  CheckBT(VGP		kk_);
		//~KeyBoard();
		//	デバイスを生成する
		static  SP  Create(
			const AnalogAxisKB&			axisL_,
			const AnalogAxisKB&			axisR_,
			const AnalogTriggerKB&		trigger_,
			const KeyDatas_KB&			buttonKB_);

		//	更新
		void  B_UpDate();
	};


	Obj::SP Obj::GetInst()
	{
		if (!winst.expired()) {
			return winst.lock();
		}
		return nullptr;
	}
	Obj::~Obj( )
	{
		//	DIオブジェクトの解放
		ML::SafeRelease(&di);
	}
	Obj::Obj()
		:wnd(0)
		,di(nullptr)
	{
		this->gpads.clear();
		this->mouses.clear();
	}
	void Obj::Set(GamePad::SP s_)
	{
		gpads.push_back(s_);
	}
	void Obj::Set(Mouse::SP s_)
	{
		mouses.push_back(s_);
	}
	//毎フレーム呼び出し
	void Obj::UpDate()
	{
		{
			class cNullCheckGP {
			public:
				// タスクが既に解放されているならtrueを返す
				bool operator()(GamePad::WP t_) const {
					bool rtv = t_.expired();
					return  rtv;
				}
			};
			auto endIt = remove_if(this->gpads.begin(), this->gpads.end(), cNullCheckGP());
			this->gpads.erase(endIt, this->gpads.end());	//既に消失しているものを全てを削除する
			for (auto it = gpads.begin(); it != gpads.end(); ++it) {
				auto  sp = it->lock();
				sp->UpDate();
			}
		}
		class cNullCheckM {
		public:
			// タスクが既に解放されているならtrueを返す
			bool operator()(Mouse::WP t_) const {
				bool rtv = t_.expired();
				return  rtv;
			}
		};
		auto endIt = remove_if(this->mouses.begin(), this->mouses.end(), cNullCheckM());
		this->mouses.erase(endIt, this->mouses.end());	//既に消失しているものを全てを削除する
		for (auto it = mouses.begin(); it != mouses.end(); ++it) {
			auto  sp = it->lock();
			sp->UpDate();
		}
	}
	bool Obj::Initialize(HWND wnd_)
	{
		wnd = wnd_;
		if (FAILED(DirectInput8Create(GetModuleHandle(nullptr),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			(VOID**)&di,
			nullptr)))
		{
			//	デバイス生成失敗
			return false;
		}
		return true;
	}
	//	DIライブラリを生成する
	Obj::SP Obj::Create(HWND		hw_)	//	ウィンドウハンドル
	{
		if (winst.expired()) {
			Obj::SP sp = Obj::SP(new Obj());
			if (sp->Initialize(hw_)) {
				winst = sp;
				return sp;
			}
		}
		return 0;
	}



	//GamePad------------------------------------------------------------------------
	GamePad::GamePad(	)
	{
	}
	GamePad::SP GamePad::CreateKB(
		const AnalogAxisKB&			axisL_,
		const AnalogAxisKB&			axisR_,
		const AnalogTriggerKB&		trigger_,
		const KeyDatas_KB&			buttonKB_)
	{
		return  KeyBoard::Create(axisL_, axisR_, trigger_, buttonKB_);
	}
	GamePad::SP GamePad::CreateGP(
		const DWORD&				id_,//0～3まで
		const KeyDatas_GP&			buttonGP_)
	{
		return  Pad::Create(id_, buttonGP_);
	}
	//	更新
	void GamePad::UpDate()
	{
		this->B_UpDate();
	}
	void  GamePad::SetVibrationL(float  pow_, int  frame_)
	{
		if (this->link != nullptr) {
			link->SetVibrationL(pow_, frame_);
		}
	};
	void  GamePad::SetVibrationR(float  pow_, int  frame_)
	{
		if (this->link != nullptr) {
			link->SetVibrationR(pow_, frame_);
		}
	};

	GamePad::ButtonState  GamePad::UpdateButtonState(bool  nbs_, ButtonState  old_)
	{
		if (nbs_) {
			if (old_ == ButtonState::Err) { old_ = ButtonState::Off; }
			//キー状態(押下系)
			switch (old_) {
			case ButtonState::Off:
			case ButtonState::Up:
				old_ = ButtonState::Down;	break;	//	離していた&離した直後だった
			case ButtonState::On:
			case ButtonState::Down:
				old_ = ButtonState::On;	break;	//	押していた&押した直後だった
			}
		}
		else {
			//キー状態(離系)
			switch (old_) {
			case ButtonState::Off:
			case ButtonState::Up:
				old_ = ButtonState::Off;	break;	//	離していた&離した直後だった
			case ButtonState::On:
			case ButtonState::Down:
				old_ = ButtonState::Up;	break;	//	押していた&押した直後だった
			}
		}
		return old_;
	}
	void  GamePad::SIUpdate(Stick&  si_, float  ax_, float  ay_)
	{
		si_.axis.x = ax_;
		si_.axis.y = ay_;

		//入力量が少ない場合、ニュートラルとする
		if (-deadzone < si_.axis.x  &&  si_.axis.x < deadzone &&
			-deadzone < si_.axis.y  &&  si_.axis.y < deadzone) {
			si_.axis.x = 0;
			si_.axis.y = 0;
		}
		//４方向ボタンのイメージでマッピング
		si_.BL = UpdateButtonState(ax_ < -deadzone, si_.BL);
		si_.BR = UpdateButtonState(ax_ > +deadzone, si_.BR);
		si_.BD = UpdateButtonState(ay_ < -deadzone, si_.BD);
		si_.BU = UpdateButtonState(ay_ > +deadzone, si_.BU);
	}
	void  GamePad::TGUpdate(Trigger&  tg_, float  lt_, float  rt_)
	{
		tg_.volumeL = lt_;
		tg_.volumeR = rt_;
		//入力量が少ない場合、ニュートラルとする
		if (tg_.volumeL < deadzone) {
			tg_.volumeL = 0;
		}
		if (tg_.volumeR < deadzone) {
			tg_.volumeR = 0;
		}
		tg_.L2 = UpdateButtonState(tg_.volumeL != 0, tg_.L2);
		tg_.R2 = UpdateButtonState(tg_.volumeR != 0, tg_.R2);
	}


	GamePad::SP GamePad::Link(GamePad::SP a, GamePad::SP b)
	{
	//２つのデバイスを統合して使えるようにする
		if(a != 0 && b != 0){	a->Link(b);	}	//キーボード〇　ゲームパッド〇
		else if(a != 0){					}	//キーボード〇　ゲームパッドB2
		else if(b != 0){		a = b;		}	//キーボードB2　ゲームパッド〇
		else{								}	//キーボードB2　ゲームパッドB2
		return a;
	}

	void  GamePad::ConvertSI(StickInfo&  si_, Stick&  s_)
	{
		si_.axis.x = s_.axis.x;
		si_.axis.y = s_.axis.y;

		//角度と長さに変換したアナログスティック入力
		if (si_.axis.x != 0 || si_.axis.y != 0) {//スティックから傾きを検出できる
			//右を0度、下を90度とした角度を求める
			si_.angleDYP = atan2(-si_.axis.y, si_.axis.x);//傾きから角度を算出
			//右を0度、上を90度とした角度を求める
			si_.angleUYP = atan2(si_.axis.y, si_.axis.x);//傾きから角度を算出

			//レバーの傾き量は±0～1に置き換える
			si_.volume = sqrt(si_.axis.x * si_.axis.x + si_.axis.y * si_.axis.y);
			si_.volume = min(si_.volume, 1.0f);
		}
		else {
			si_.volume = 0.0f;
		}
		si_.BL = this->ConvertKS(s_.BL);
		si_.BR = this->ConvertKS(s_.BR);
		si_.BD = this->ConvertKS(s_.BD);
		si_.BU = this->ConvertKS(s_.BU);
	}
	void  GamePad::ConvertTG(TriggerInfo&  ti_, Trigger&  t_)
	{
		ti_.volumeL = t_.volumeL;
		ti_.volumeR = t_.volumeR;
		ti_.L2 = this->ConvertKS(t_.L2);
		ti_.R2 = this->ConvertKS(t_.R2);
	}

	StickInfo  GamePad::StickL()
	{
		StickInfo  me;
		ConvertSI(me, this->ls);
		StickInfo  linkRtv;
		if (link != nullptr) {
			linkRtv = link->StickL();
			if (me.volume < linkRtv.volume) {
				return linkRtv;
			}
		}
		return me;
	}
	StickInfo  GamePad::StickR()
	{
		StickInfo  me;
		ConvertSI(me, this->rs);
		StickInfo  linkRtv;
		if (link != nullptr) {
			linkRtv = link->StickR();
			if (me.volume < linkRtv.volume) {
				return linkRtv;
			}
		}
		return me;
	}
	TriggerInfo  GamePad::TriggerLR()
	{
		TriggerInfo  me;
		ConvertTG(me, this->tg);
		TriggerInfo  linkRtv;
		if (link != nullptr) {
			linkRtv = link->TriggerLR();
			if (me.volumeL < linkRtv.volumeL) {
				me.volumeL = linkRtv.volumeL;
				me.L2 = linkRtv.L2;
			}
			if (me.volumeR < linkRtv.volumeR) {
				me.volumeR = linkRtv.volumeR;
				me.R2 = linkRtv.R2;
			}
		}
		return me;
	}



	ButtonInfo  GamePad::ConvertKS(ButtonState  ks_)
	{
		ButtonInfo  rtv;
		switch (ks_) {
		default:
		case DI::GamePad::ButtonState::Off:
			rtv.SetOff();
			break;
		case DI::GamePad::ButtonState::Down:
			rtv.SetDown();
			break;
		case DI::GamePad::ButtonState::On:
			rtv.SetOn();
			break;
		case DI::GamePad::ButtonState::Up:
			rtv.SetUp();
			break;
		}
		return rtv;
	}
	//２つのキーを統合した時、打倒な状態を返す
	GamePad::ButtonState  GamePad::IntegrationKey(ButtonState  a_, ButtonState  b_)
	{
		using BS = ButtonState;
		BS ret = BS::Off;

		BS  mat[5][5] = {
			//Err       On      Off       Down      Up
			{ BS::Err,  BS::On, BS::Off,  BS::Down, BS::Up}, //Err
			{ BS::On,   BS::On, BS::On,   BS::On,   BS::On}, //On
			{ BS::Off,  BS::On, BS::Off,  BS::Down, BS::Up}, //Off
			{ BS::Down, BS::On, BS::Down, BS::Down, BS::On}, //Down
			{ BS::Up,   BS::On, BS::Up,   BS::On,   BS::Up}, //Up
		};
		return  mat[(BYTE)a_][(BYTE)b_];
	}

	VGamePad  GamePad::GetState()
	{
		VGamePad  rtv;
		rtv.LStick = this->StickL();
		rtv.RStick = this->StickR();
		rtv.Trigger = this->TriggerLR();
		rtv.B1 = ConvertKS(this->CheckBT(DI::VGP::B1));
		rtv.B2 = ConvertKS(this->CheckBT(DI::VGP::B2));
		rtv.B3 = ConvertKS(this->CheckBT(DI::VGP::B3));
		rtv.B4 = ConvertKS(this->CheckBT(DI::VGP::B4));

		rtv.L1 = ConvertKS(this->CheckBT(DI::VGP::L1));
		rtv.R1 = ConvertKS(this->CheckBT(DI::VGP::R1));

		rtv.ST = ConvertKS(this->CheckBT(DI::VGP::ST));
		rtv.SE = ConvertKS(this->CheckBT(DI::VGP::SE));

		rtv.L3 = ConvertKS(this->CheckBT(DI::VGP::L3));
		rtv.R3 = ConvertKS(this->CheckBT(DI::VGP::R3));

		rtv.HL = ConvertKS(this->CheckBT(DI::VGP::HL));
		rtv.HR = ConvertKS(this->CheckBT(DI::VGP::HR));
		rtv.HU = ConvertKS(this->CheckBT(DI::VGP::HU));
		rtv.HD = ConvertKS(this->CheckBT(DI::VGP::HD));

		rtv.S0 = ConvertKS(this->CheckBT(DI::VGP::S0));
		rtv.S1 = ConvertKS(this->CheckBT(DI::VGP::S1));
		rtv.S2 = ConvertKS(this->CheckBT(DI::VGP::S2));
		rtv.S3 = ConvertKS(this->CheckBT(DI::VGP::S3));
		rtv.S4 = ConvertKS(this->CheckBT(DI::VGP::S4));
		rtv.S5 = ConvertKS(this->CheckBT(DI::VGP::S5));
		rtv.S6 = ConvertKS(this->CheckBT(DI::VGP::S6));
		rtv.S7 = ConvertKS(this->CheckBT(DI::VGP::S7));
		rtv.S8 = ConvertKS(this->CheckBT(DI::VGP::S8));
		rtv.S9 = ConvertKS(this->CheckBT(DI::VGP::S9));
		return rtv;
	}
	//Pad------------------------------------------------------------------------
	Pad::Pad(void)
		:vibration()
		,leftVibFrame(0)
		,rightVibFrame(0)
	{
	}
	bool Pad::Initialize(
		const DWORD&				id_,//0～3まで
		const KeyDatas_GP&			buttonGP_)
	{
		this->pad_ID = id_;
		for(DWORD c = 0;  c < buttonGP_.size(); c++)
		{
			KeyParam w = { buttonGP_[c].gKey, buttonGP_[c].pKey, ButtonState::Off };
			this->buttons.push_back(w);
		}

		return true;
	}
	//	デバイスを生成する
	Pad::SP  Pad::Create(
		const DWORD&				id_,//0～3まで
		const KeyDatas_GP&			buttonGP_)
	{
		auto sp = SP(new Pad());
		if (sp) {
			if (sp->Initialize(id_, buttonGP_))
			{
				if (sp) {
					auto  ob = Obj::GetInst();
					ob->Set(sp);
				}
				return sp;
			}
		}
		return 0;
	}
	//押されたかのチェック
	bool Pad::checkPush(const VGP  bt_, const DWORD  state_)
	{
		int  bt = (int)bt_;
		//                   B1, B2, B3, B4, L1, R1, SE, ST, L3, R3, HL, HR, HU, HD,
		const int  btc[] = { 12, 13, 14, 15,  9, 10,  5,  4,  6,  7,  2,  3,  0,  1 };
		if ((int)bt >= _countof(btc)) { return false; }
		DWORD  bit = 1 << btc[bt];
		return  (state_ & bit) ? true : false;
	}
	GamePad::ButtonState	 Pad::CheckBT(VGP		kk_)
	{
		ButtonState ret = ButtonState::Off;

		//キー配列の中から対象になるキーを検出する
		for (auto it = begin(buttons); it != end(buttons); ++it)
		{
			if (kk_ == it->pKey)		//登録済みのキー一覧から該当するキーを検出した
			{
				ret = IntegrationKey(ret, (ButtonState)it->state);
			}
		}
		if (link != nullptr) {
			ret = IntegrationKey(ret, link->CheckBT(kk_));
		}
		return ret;
	}
	void  Pad::SetVibrationL(float  pow_, int  frame_)
	{
		if (this->link != nullptr) {
			link->SetVibrationL(pow_, frame_);
		}

		this->vibration.wLeftMotorSpeed = static_cast<WORD>(65535.0f * pow_);
		XInputSetState(this->pad_ID, &this->vibration);
		this->leftVibFrame = frame_;
	}
	void  Pad::SetVibrationR(float  pow_, int  frame_)
	{
		if (this->link != nullptr) {
			link->SetVibrationR(pow_, frame_);
		}

		this->vibration.wRightMotorSpeed = static_cast<WORD>(65535.0f * pow_);
		XInputSetState(this->pad_ID, &this->vibration);
		this->rightVibFrame = frame_;
	}

	

	void  Pad::B_UpDate()
	{
		XINPUT_STATE	state;//ボタン情報

		//ボタンの状態を調べるとともに、パッド抜けをチェック
		if (XInputGetState(this->pad_ID, &state) != ERROR_SUCCESS)
		{
			connected = false;
			return;
		}
		DWORD  btstate = state.Gamepad.wButtons;

		//ボタンへの対応
		bool keyFlag;
		for (auto it = begin(buttons); it != end(buttons); ++it)
		{
			//キーコードに対応したデータ変更処理を行う
			keyFlag = false;
			//POV対応
			if (     it->gKey == RGP::HSL) { keyFlag = this->checkPush(VGP::HL, btstate); }
			else if (it->gKey == RGP::HSR) { keyFlag = this->checkPush(VGP::HR, btstate); }
			else if (it->gKey == RGP::HSU) { keyFlag = this->checkPush(VGP::HU, btstate); }
			else if (it->gKey == RGP::HSD) { keyFlag = this->checkPush(VGP::HD, btstate); }
			//ボタン００～１４
			else if (it->gKey >= RGP::B01 && it->gKey <= RGP::B16) {
				keyFlag = this->checkPush(static_cast<VGP>(it->gKey), btstate);
			}
			//未対応のキーが設定されている
			else { continue; }
			it->state = UpdateButtonState(keyFlag, it->state);
		}

		//最大値合わせ
		if (state.Gamepad.sThumbLX == -32768) { state.Gamepad.sThumbLX = -32767; }
		if (state.Gamepad.sThumbLY == -32768) { state.Gamepad.sThumbLY = -32767; }


		//アナログスティック対応
		SIUpdate(this->ls, state.Gamepad.sThumbLX * digToAnalog, state.Gamepad.sThumbLY  * digToAnalog);
		SIUpdate(this->rs, state.Gamepad.sThumbRX * digToAnalog, state.Gamepad.sThumbRY  * digToAnalog);
		TGUpdate(this->tg, 
			state.Gamepad.bLeftTrigger * digToAnalog_TG, 
			state.Gamepad.bRightTrigger * digToAnalog_TG);

		//バイブレーション停止
		if (this->leftVibFrame > 0) {
			this->leftVibFrame--;
			if (this->leftVibFrame == 0) {
				this->vibration.wLeftMotorSpeed = 0;
				XInputSetState(this->pad_ID, &this->vibration);
			}
		}
		if (this->rightVibFrame > 0) {
			this->rightVibFrame--;
			if (this->rightVibFrame == 0) {
				this->vibration.wRightMotorSpeed = 0;
				XInputSetState(this->pad_ID, &this->vibration);
			}
		}
	}
	//KeyBoard------------------------------------------------------------------------
	KeyBoard::KeyBoard(void)
	{

	}
	//	デバイスを生成する
	KeyBoard::SP  KeyBoard::Create(
		const AnalogAxisKB&			axisL_,
		const AnalogAxisKB&			axisR_,
		const AnalogTriggerKB&		trigger_,
		const KeyDatas_KB&			buttonKB_)
	{
		auto sp = SP(new KeyBoard());
		if (sp) {
			if (sp->Initialize(axisL_, axisR_, trigger_, buttonKB_))
			{
				if (sp) {
					auto  ob = Obj::GetInst();
					ob->Set(sp);
				}
				return sp;
			}
		}
		return 0;
	}
	static GUID					a_guid;
	static int					a_id;
	BOOL CALLBACK diEnumDevice(const DIDEVICEINSTANCE* inDevice,
		void* inContext)
	{
		//デバイス番号
		DWORD* num = (DWORD*)inContext;
		//指定された番号のデバイスに到達
		if ((*num) == a_id)
		{
			a_guid = inDevice->guidInstance;	//ＧＵＩＤを保存
			(*num)++;
			return DIENUM_STOP;					//デバイスの列挙を終了する
		}
		else
		{
			(*num)++;
			return DIENUM_CONTINUE;			//次のデバイスのチェックに移る
		}
	}

	bool KeyBoard::Initialize(
		const AnalogAxisKB&			axisL_,
		const AnalogAxisKB&			axisR_,
		const AnalogTriggerKB&		trigger_,
		const KeyDatas_KB&			buttonKB_)
	{
		Obj::SP dii = Obj::GetInst();
		if (dii == 0) { return false; }

		struct {
			DWORD				dType;
			LPCDIDATAFORMAT		dFormat;
			DWORD				cLevel;

		}kind_KeyBoard = { DI8DEVCLASS_KEYBOARD, &c_dfDIKeyboard,  DISCL_NONEXCLUSIVE | DISCL_FOREGROUND };
		
		//デバイスの検出を行う----------------------------------------------------------
		//指定デバイス検出（グローバル変数　a_guid　および　a_id　に注意）
		DWORD deviceCount = 0;
		a_id = 0;
		if (FAILED(dii->DInput().EnumDevices(
			kind_KeyBoard.dType, diEnumDevice,
			&deviceCount, DIEDFL_ATTACHEDONLY))) {
			return false;
		}
		//入力デバイスを生成する
		if (FAILED(dii->DInput().CreateDevice(a_guid, &device, nullptr))) { return false; }
		//入力デバイスのフォーマットを設定する
		if (FAILED(device->SetDataFormat(kind_KeyBoard.dFormat))) { return false; }
		//入力デバイスの協調レベルを設定する
		if (FAILED(device->SetCooperativeLevel(dii->Wnd(), kind_KeyBoard.cLevel))) { return false; }

		DIPROPDWORD dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = 8;//同時に判定できるキーの数になるのかな？
		if (FAILED(device->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
		{
			return false;
		}

		//----------------------------------------------------------
		for (DWORD c = 0; c < buttonKB_.size(); c++)
		{
			KeyParam w = { buttonKB_[c].vKey, buttonKB_[c].pKey, ButtonState::Off };
			this->buttons.push_back(w);
		}
		//キー登録
		kbAxisL.push_back(axisL_.vKeyL);
		kbAxisL.push_back(axisL_.vKeyR);
		kbAxisL.push_back(axisL_.vKeyU);
		kbAxisL.push_back(axisL_.vKeyD);

		kbAxisR.push_back(axisR_.vKeyL);
		kbAxisR.push_back(axisR_.vKeyR);
		kbAxisR.push_back(axisR_.vKeyU);
		kbAxisR.push_back(axisR_.vKeyD);

		kbTrigger.push_back(trigger_.vKeyL);
		kbTrigger.push_back(trigger_.vKeyR);
		return true;
	}
	GamePad::ButtonState	 KeyBoard::CheckBT(VGP		kk_)
	{
		ButtonState ret = ButtonState::Off;

		//キー配列の中から対象になるキーを検出する
		for (auto it = begin(buttons); it != end(buttons); ++it)
		{
			if (kk_ == it->pKey)		//登録済みのキー一覧から該当するキーを検出した
			{
				ret = IntegrationKey(ret, (ButtonState)it->state);
			}
		}
		if (link != nullptr) {
			ret = IntegrationKey(ret, link->CheckBT(kk_));
		}
		return ret;
	}
	void  KeyBoard::B_UpDate()
	{
		BYTE keyStatus[256] = { 0 };
		//デバイスが使用可能か確認する
		int acquireCnt = 0;
		BOOL brFlag = FALSE;
		if (FAILED(device->Poll()))
		{
			do
			{
				if (SUCCEEDED(device->Acquire())) { brFlag = TRUE; }
				if (++acquireCnt > 30) { return; }
			} while (brFlag == FALSE);
		}
		//	キーボード情報取得
		if (FAILED(device->GetDeviceState(sizeof(BYTE) * 256, &keyStatus))) { return; }
		//	方向入力に登録されたキーへの対応
		float  ax_L = 0, ay_L = 0;
		if (kbAxisL.size() == 4) {
			if (keyStatus[kbAxisL[0]] != 0) { ax_L -= 1; }
			if (keyStatus[kbAxisL[1]] != 0) { ax_L += 1; }
			if (keyStatus[kbAxisL[2]] != 0) { ay_L += 1; }
			if (keyStatus[kbAxisL[3]] != 0) { ay_L -= 1; }
		}
		float  ax_R = 0, ay_R = 0;
		if (kbAxisR.size() == 4) {
			if (keyStatus[kbAxisR[0]] != 0) { ax_R -= 1; }
			if (keyStatus[kbAxisR[1]] != 0) { ax_R += 1; }
			if (keyStatus[kbAxisR[2]] != 0) { ay_R += 1; }
			if (keyStatus[kbAxisR[3]] != 0) { ay_R -= 1; }
		}

		//LRスティック情報更新
		SIUpdate(ls, ax_L, ay_L);
		SIUpdate(rs, ax_R, ay_R);
		//　ボタンとして登録されたキーへの対応
		for (auto it = begin(buttons); it != end(buttons); ++it) {
			it->state = UpdateButtonState(keyStatus[it->vKey] != 0, it->state);
		}
	}

	//Mouse------------------------------------------------------------------------
	Err  Mouse::init(float scaleX_, float scaleY_)
	{
		Obj::SP dii = Obj::GetInst();
		if (dii == 0){ return Err::NOTDEVICE; }

		//--------------------------------
		//DirectMouse
		if (FAILED(dii->DInput().CreateDevice(GUID_SysMouse, &mouse, nullptr))){ return Err::CREATEDEVICE; }
		if (FAILED(mouse->SetDataFormat(&c_dfDIMouse2))){ return Err::DATAFORMAT; } // マウス用のデータ・フォーマットを設定
		mouse->SetCooperativeLevel(dii->Wnd(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);//レベル設定
		// デバイスの設定    
		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.dwData = DIPROPAXISMODE_REL; // 相対値モードDIPROPAXISMODE_RELで設定（絶対値はDIPROPAXISMODE_ABS）    
		mouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);

		// 入力制御開始    
		mouse->Acquire();
		//--------------------------------
		scaleX = scaleX_;
		scaleY = scaleY_;

		return Err::NOTHING;
	}

	Mouse::SP Mouse::Create(float scaleX_, float scaleY_)
	{
		auto sp = SP(new Mouse());
		if (sp){
			if (sp->init(scaleX_, scaleY_) != Err::NOTHING){
				MessageBox(nullptr, _T("マウスがありませんよ！"), nullptr, MB_OK);
				return nullptr;
			}
			else{
				auto  ob = Obj::GetInst();
				ob->Set(sp);
			}
		}
		return sp;
	}
	Mouse::Mouse()
	{
	}

	Mouse::~Mouse()
	{
		if (mouse != NULL)
		{
			mouse->Unacquire();
			ML::SafeRelease(&mouse);
		}
	}

	void Mouse::UpDate()
	{
		//--------------------------------
		//add koyanagi
		// 値の更新
		ZeroMemory(&mouseState, sizeof(DIMOUSESTATE2));
		if (FAILED(mouse->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState))){
			mouse->Acquire();
		}
		//スクリーン座標取得
		GetCursorPos(&this->pos_Screen);
		//クライアント座標算出
		this->pos = this->pos_Screen;
		Obj::SP dii = Obj::GetInst();
		ScreenToClient(dii->Wnd(), &this->pos);
		this->pos.x = (LONG)(this->pos.x / scaleX);
		this->pos.y = (LONG)(this->pos.y / scaleY);
		//ホイール
		mouseWheel = mouseState.lZ;
		//ボタン系
		for (int i = 0; i < (int)MB::kMax; i++)
		{
			if ((mouseState.rgbButtons[i] & 0x80) >> 7){
				if (mouseButton[i] == ButtonState::Off || mouseButton[i] == ButtonState::Up)  mouseButton[i] = ButtonState::Down;
				else  mouseButton[i] = ButtonState::On;
			}
			else{
				if (mouseButton[i] == ButtonState::On || mouseButton[i] == ButtonState::Down)  mouseButton[i] = ButtonState::Up;
				else  mouseButton[i] = ButtonState::Off;
			}
		}
		//--------------------------------
	}
	ButtonInfo  Mouse::ConvertKS(DI::Mouse::ButtonState  ks_)
	{
		ButtonInfo  rtv;
		switch (ks_) {
		default:
		case DI::Mouse::ButtonState::Off:
			rtv.SetOff();
			break;
		case DI::Mouse::ButtonState::Down:
			rtv.SetDown();
			break;
		case DI::Mouse::ButtonState::On:
			rtv.SetOn();
			break;
		case DI::Mouse::ButtonState::Up:
			rtv.SetUp();
			break;
		}
		return rtv;
	}
	//
	VMouse  Mouse::GetState()
	{
		VMouse  rtv;
		rtv.pos = this->pos;
		rtv.pos_Screen = this->pos_Screen;
		rtv.wheel = this->mouseWheel;
		rtv.LB = ConvertKS(this->CheckBT(Mouse::MB::LB));
		rtv.RB = ConvertKS(this->CheckBT(Mouse::MB::RB));
		rtv.CB = ConvertKS(this->CheckBT(Mouse::MB::CB));

		rtv.B1 = ConvertKS(this->CheckBT(Mouse::MB::RESERVED1));
		rtv.B2 = ConvertKS(this->CheckBT(Mouse::MB::RESERVED2));
		rtv.B3 = ConvertKS(this->CheckBT(Mouse::MB::RESERVED3));
		rtv.B4 = ConvertKS(this->CheckBT(Mouse::MB::RESERVED4));
		rtv.B5 = ConvertKS(this->CheckBT(Mouse::MB::RESERVED5));
		return  rtv;
	}
	Mouse::ButtonState Mouse::CheckBT(Mouse::MB  mb_)
	{
		return mouseButton[(int)mb_];
	}

	//-----------------------------------------------------------------------
	//GamePad系
	//-----------------------------------------------------------------------
	map<string, DI::GamePad::SP>	GamePadManager::arr;
	bool GamePadManager::notObject;
	void  GamePadManager::Set(const string& name_, DI::GamePad::SP sp_)
	{
		arr[name_] = sp_;
	}
	//-----------------------------------------------------------------------
	DI::GamePad::SP  GamePadManager::Get(const string& name_)
	{
		auto it = arr.find(name_);
		if (it == arr.end()) {
			if (notObject == false) {
				MessageBox(nullptr, name_.c_str(), _T("GamePadManager 対象が存在しません"), MB_OK);
			}
			notObject = true;
			return nullptr;
		}
		return it->second;
	}
	//-----------------------------------------------------------------------
	void  GamePadManager::Clear(const string& name_)
	{
		auto it = arr.find(name_);
		if (it == arr.end()) {
			return;
		}
		arr.erase(it);
	}
	//-----------------------------------------------------------------------
	void GamePadManager::AllClear()
	{
		arr.clear();
	}
	//-----------------------------------------------------------------------
	//Mouse系
	//-----------------------------------------------------------------------
	map<string, DI::Mouse::SP>	MouseManager::arr;
	bool MouseManager::notObject;
	void  MouseManager::Set(const string& name_, DI::Mouse::SP sp_)
	{
		arr[name_] = sp_;
	}
	//-----------------------------------------------------------------------
	DI::Mouse::SP  MouseManager::Get(const string& name_)
	{
		auto it = arr.find(name_);
		if (it == arr.end()) {
			if (notObject == false) {
				MessageBox(nullptr, name_.c_str(), _T("MouseManager 対象が存在しません"), MB_OK);
			}
			notObject = true;
			return nullptr;
		}
		return it->second;
	}
	//-----------------------------------------------------------------------
	void  MouseManager::Clear(const string& name_)
	{
		auto it = arr.find(name_);
		if (it == arr.end()) {
			return;
		}
		arr.erase(it);
	}
	//-----------------------------------------------------------------------
	void MouseManager::AllClear()
	{
		arr.clear();
	}
	//---------------------------------------------------------------------------
}

