//-------------------------------------------------------------------
//ポーズ画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game_Pause.h"
#include  "Task_Title_Manager.h"
#include  "DI2018_Ver1_0.h"

namespace  Game_Pause
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		this->corsorNum = 次へ;
		this->uiState = 選択;
		this->hitBase = ML::Box2D(-20, -90, 340, 240);
		for (int i = 0; i < 3; i++)
		{
			drawposY[i] = 250 + ((i + 1) * 200);
			sa[i] = 0;
		}
		this->NextFlag = 0;
		this->desuCnt = 0;
		this->described = false;
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_GN("ゲーム", "ポーズ画面");
		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//★引き継ぎタスクの生成
			if (this->corsorNum != 終了)
			{
				//続ける
				ge->gameState = ge->Game;
				DM::Sound_Play("GameStage", true);
			}
			else
			{
				//タイトルへ
				ge->KillAll_G("ゲーム");
				auto	nextTask = Title_Manager::Object::Create(true);
			}
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		SelectAction();
		SelectMenu();
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//背景
		ML::Box2D drawBack(0, 0, 1056, 1056);
		ML::Box2D srcBack(0, 0, 800, 600);
		DG::Image_Draw("BackGrayImg", drawBack, srcBack, ML::Color(0.4f, 1, 1, 1));
		//ロゴ
		ML::Box2D draw(1056/2-320, 150, 640, 160);
		ML::Box2D src(0, 0, 310, 100);
		DG::Image_Draw("PauseImg", draw, src);
		for (int i = 0; i < 3; i++)
		{
			
			//メニュー
			ML::Box2D drawMenu(ge->screenWidth / 2 - 150, this->drawposY[i], 300, 60);
			ML::Box2D srcMenu(0, 0, 400, 100);
			if (uiState == 選択 && this->corsorNum == i)
			{
				srcMenu.Offset(0, (i + 1) * 100);
				DG::Image_Draw("PauseImg", drawMenu, srcMenu);
			}
			else
			{
				srcMenu.Offset(0, i * 100);
				DG::Image_Draw("PauseMonoImg", drawMenu, srcMenu);
			}
		}
		//カーソル
		for (int i = 0; i < 3; i++)
		{
			if (i == this->corsorNum)
			{
				ML::Box2D drawS = this->hitBase;
				drawS.Offset(ge->screenWidth / 2 - 150, this->drawposY[i]);
				ML::Box2D srcS(0, 0, 300, 250);
				DG::Image_Draw("CameraImg", drawS, srcS);
			}
		}
		//説明
		if (this->corsorNum == 説明 && this->uiState == 決定)
		{
			//操作説明描画
			ML::Box2D drawSe(100, 100, 900 - 56, 900 - 56);
			ML::Box2D srcSe(0, 0, 800, 600);
			DG::Image_Draw("SetumeiImg", drawSe, srcSe);
		}
	}

	//メニューを縦で決定できるようにオーバーライドする
	void Object::SelectAction()
	{
		if (this->described)
		{
			this->uiState = 選択;
			this->described = false;
		}
		auto	in = ge->in1->GetState();
		if (this->uiState == 決定 && this->corsorNum != 説明)
		{
			this->Kill();
		}
		else if (this->uiState == 決定 && this->corsorNum == 説明)
		{
			if (in.B1.down)
			{
				this->corsorNum = 説明;
				this->described = true;
				DM::Sound_Play("ui", false);
			}
		}
	}
	//メニューを縦で選択できるようにオーバーライドする
	void Object::SelectMenu()
	{
		auto in = ge->in1->GetState();
		if (in.LStick.BU.down&&this->corsorNum>次へ)
		{
			this->corsorNum--;
			this->uiState = 選択;
			DM::Sound_Play("ui", false);
		}
		if (in.LStick.BD.down&&this->corsorNum<終了)
		{
			this->corsorNum++;
			this->uiState = 選択;
			DM::Sound_Play("ui", false);
		}
		if (this->uiState == 選択 && in.B1.down)
		{
			this->uiState = 決定;
			DM::Sound_Play("ui", false);
		}
		if (in.ST.down)
		{
			DM::Sound_Play("ui", false);
			this->Kill();
		}
	}
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//リソースクラスの生成
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}