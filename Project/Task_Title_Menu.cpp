//-------------------------------------------------------------------
//タイトルメニュー表示
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title_Menu.h"
#include  "Task_Game_Manager.h"
#include  "Task_StageLogo.h"
#include  "Task_Title_Manager.h"

namespace  Title_Menu
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
		drawposX[0] = 0;
		for (int i = 0; i < 3; i++)
		{
			drawposX[i] = -40;
			sa[i] = 0;
		}
		this->uiState = 移動中;
		this->hitBase = ML::Box2D(0, 0, 300, 80);
		this->hitBase.Offset(0, 240 - 100);
		this->NextFlag = 0;
		this->desuCnt = 0;
		this->corsorNum = 次へ;
		this->cursor[0] = { 90 };
		this->cursor[1] = { 390 };
		this->cursor[2] = { 690 };
		this->MenuPosX[0] = { 100 };
		this->MenuPosX[1] = { 400 };
		this->MenuPosX[2] = { 700 };
		this->described = false;

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("タイトル");
		//終了
		if (this->corsorNum == 終了)
		{
			ge->KillAll_G("リソースマネージャー");
			ge->quitRequest = true;
		}
		if (!ge->QuitFlag() && this->nextTaskCreate) 
		{
			//★引き継ぎタスクの生成
			auto nextTask = FrontStageLogo::Object::Create(true);
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto in = ge->in1->GetState();
		auto manager = ge->GetTask_One_GN<Title_Manager::Object>("タイトル", "統括");

		if (manager->titleState == manager->プリーズロゴ)
		{
			this->alphaCnt = ColorCntMove(this->alphaCnt, 0.01f, 0.8f, 0);
			if (in.B1.down)
			{
				manager->titleState = manager->メニュー;
				//再生開始
				DM::Sound_Play("ui", false);
			}
			return;
		}
		if (manager->titleState == manager->メニュー)
		{
			
			EasingMoveX();	
			SelectAction();
			SelectMenu();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		auto manager = ge->GetTask_One_GN<Title_Manager::Object>("タイトル", "統括");
		if (manager->titleState == manager->プリーズロゴ)
		{
				ML::Box2D drawP(ge->screenWidth / 2 - 150, 800, 300, 60);
				ML::Box2D srcP(0, 200, 700, 100);
				DG::Image_Draw("TitleFontColorImg", drawP, srcP, ML::Color(this->alphaCnt, 1, 1, 1));
			return;
		}
		if (manager->titleState == manager->メニュー)
		{
			ML::Box2D draw(this->cursor[this->corsorNum], 480, 300, 300);
			ML::Box2D src(0, 0, 300, 250);
			DG::Image_Draw("CameraImg", draw, src);
			for (int i = 0; i < 3; i++)
			{
				ML::Box2D drawMenu((int)this->drawposX[i], 600, 400, 80);
				ML::Box2D srcMenu(0, 0, 700, 100);
				//選択いている文字をカラーにする
				if (uiState == 選択 && this->corsorNum == i)
				{
					srcMenu.Offset(0, 200 + ((i + 1) * 100));
					DG::Image_Draw("TitleFontColorImg", drawMenu, srcMenu);
				}
				else
				{
					srcMenu.Offset(0, 200 + ((i + 1) * 100));
					DG::Image_Draw("TitleFontMonoImg", drawMenu, srcMenu);
				}
				if (this->corsorNum == 説明 && this->uiState == this->決定)
				{
					//操作説明描画
					ML::Box2D drawSe(100, 100, 900 - 56, 900 - 56);
					ML::Box2D srcSe(0, 0, 800, 600);
					DG::Image_Draw("SetumeiImg", drawSe, srcSe, ML::Color(1, 1, 1, 1));
				}
			}
		}
	}
	//緩急をつけて目的地へ近づく
	void Object::EasingMoveX()
	{
		if (this->uiState == 移動中)
		{
			for (int i = 0; i < 3; i++)
			{
				if (this->drawposX[i] < this->MenuPosX[i] + 10 && this->drawposX[i]>this->MenuPosX[i] - 10)
				{
					this->uiState = 選択;
				}
			}
		}
		for (int i = 0; i < 3; i++)
		{
			sa[i] = this->MenuPosX[i] - this->drawposX[i];
			this->drawposX[i] += sa[i] * 0.08f;
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