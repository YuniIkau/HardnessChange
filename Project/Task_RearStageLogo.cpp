//-------------------------------------------------------------------
//ステージ後のロゴ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_RearStageLogo.h"
#include  "Task_RearStageLogoImg.h"
#include  "Task_Title_Manager.h"
#include  "Task_Game_Manager.h"

#include  "ResourceManager.h"

namespace  RearStageLogo
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		//リソースマネージャーの取得
		auto RM = ge->GetTask_One_GN<ResourceManager::Object>("リソースマネージャー", "リソースマネージャー");
		//タイトルシーンで使用するリソースの読み込み
		RM->Create(RM->RearStageLogo);
		//BGMの再生開始
		DM::Sound_Play("GameOver", true);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		//リソースマネージャーの取得
		auto RM = ge->GetTask_One_GN<ResourceManager::Object>("リソースマネージャー", "リソースマネージャー");
		RM->Erase(RM->RearStageLogo);
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
		this->render2D_Priority[1] = 1.0f;
		this->alphaCnt = 0;
		this->desuCnt = 0;
		this->corsorNum = 0;
		this->uiState = 選択;
		this->hitBase = ML::Box2D(0, 0, 200, 40);
		this->hitBase.Offset(0, 0);
		ML::Box2D srcMenu(0, 0, 0, 0);
		if (ge->gameState == ge->オールクリア)
		{
			this->drawPosx[0] = { (LONG)(200) };
			this->drawPosx[1] = { (LONG)(600) };
		}
		else if (ge->gameState == ge->クリア)
		{
			this->drawPosx[0] = { (LONG)(30) };
			this->drawPosx[1] = { (LONG)(380) };
			this->drawPosx[2] = { (LONG)(730) };
			//クリア時はすぐに次のステージへ行けるようにNextにカーソルを置く
			this->corsorNum = 2;
		}
		else
		{
			this->drawPosx[0] = { (LONG)(200) };
			this->drawPosx[1] = { (LONG)(600) };
		}
		//★タスクの生成
		auto rsl = RearStageLogoImg::Object::Create(true);
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("LogoRear");

		if (!ge->QuitFlag() && this->nextTaskCreate) 
		{
			//★引き継ぎタスクの生成
			if (this->corsorNum == 2)
			{
				//クリア時次のステージ
				ge->StageNum++;
				auto nextTask = Game_Manager::Object::Create(true);
			}
			//もう一度同じステージへ行く
			else if (this->corsorNum == 0)
			{
				//同じステージ
				auto nextTask = Game_Manager::Object::Create(true);
			}
			else
			{
				//タイトル
				auto	nextTask = Title_Manager::Object::Create(true);
			}
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto	in = ge->in1->GetState();
		//画面表示処理
		if (ge->gameState == ge->クリア)
		{
			FadeInOut(this->alphaCnt, 0.04f, 1, true);
		}
		else
		{
			FadeInOut(this->alphaCnt, 0.01f, 1, true);
		}

		if (this->uiState == 決定)
		{
			if (CntDelayMove(this->desuCnt, 1, 50))
			{
				this->Kill();
			}
		}

		SelectMenu();

		if (in.B1.down&&this->uiState == 選択)
		{
			this->uiState = 決定;
			DM::Sound_Play("ui", false);
		}
		if (this->uiState == 決定)
		{
			if (CntDelayMove(this->desuCnt, 1, 50))
			{
				this->Kill();
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//最後のステージをクリアしたなら
		if (ge->gameState == ge->オールクリア)
		{
			ML::Box2D draw(0, 0, 1056, 1056);
			ML::Box2D src(0, 0, 800, 600);
			DG::Image_Draw("RearLogoClearImg", draw, src, ML::Color(this->alphaCnt, 1, 1, 1));
			for (int i = 0; i < 2; i++)
			{
				ML::Box2D drawC(this->drawPosx[i], 800, 300, 60);
				ML::Box2D srcC(0, ((i + 1) * 200) - 200, 700, 200);
				DG::Image_Draw("RearMenuImg", drawC, srcC);
			}
		}
		//ゲームクリアなら
		else if (ge->gameState == ge->クリア)
		{
			ML::Box2D draw(0, 0, 1056, 1056);
			ML::Box2D src(0, 0, 800, 600);
			DG::Image_Draw("RearLogoClearImg", draw, src, ML::Color(this->alphaCnt, 1, 1, 1));
			for (int i = 0; i < 3; i++)
			{
				ML::Box2D drawC(this->drawPosx[i], 800, 300, 60);
				ML::Box2D srcC(0, ((i + 1) * 200) - 200, 700, 200);
				DG::Image_Draw("RearMenuImg", drawC, srcC);
			}
		}
		//ゲームオーバーなら
		else
		{
			ML::Box2D draw(0, 0, 1056, 1056);
			ML::Box2D src(0, 0, 800, 600);
			DG::Image_Draw("RearLogoOverImg", draw, src, ML::Color(this->alphaCnt, 1, 1, 1));
			for (int i = 0; i < 2; i++)
			{
				ML::Box2D drawC(this->drawPosx[i], 800, 300, 60);
				ML::Box2D srcC(0, ((i + 1) * 200) - 200, 700, 200);
				DG::Image_Draw("RearMenuImg", drawC, srcC);
			}
		}
		//カーソルの描画
		ML::Box2D drawC(this->drawPosx[this->corsorNum]-20, 800-120, 340, 340);
		ML::Box2D srcC(0, 0, 300, 250);
		DG::Image_Draw("CameraImg", drawC, srcC);
	}
	void Object::SelectMenu()
	{
		auto in = ge->in1->GetState();
		if (ge->gameState == ge->オールクリア)
		{
			if (in.LStick.BL.down&&this->corsorNum > 0 && this->uiState != 決定)
			{
				this->corsorNum--; this->uiState = 選択;
				DM::Sound_Play("ui", false);
			}
			if (in.LStick.BR.down&&this->corsorNum < 1 && this->uiState != 決定)
			{
				this->corsorNum++; this->uiState = 選択;
				DM::Sound_Play("ui", false);
			}
		}
		else if (ge->gameState == ge->クリア)
		{
			if (in.LStick.BL.down&&this->corsorNum > 0 && this->uiState != 決定)
			{
				this->corsorNum--;
				this->uiState = 選択;
				DM::Sound_Play("ui", false);
			}
			if (in.LStick.BR.down&&this->corsorNum < 2 && this->uiState != 決定)
			{
				this->corsorNum++;
				this->uiState = 選択;
				DM::Sound_Play("ui", false);
			}
		}
		else
		{
			if (in.LStick.BL.down&&this->corsorNum > 0 && this->uiState != 決定)
			{
				this->corsorNum--;
				this->uiState = 選択;
				DM::Sound_Play("ui", false);
			}
			if (in.LStick.BR.down&&this->corsorNum < 1 && this->uiState != 決定)
			{
				this->corsorNum++;
				this->uiState = 選択;
				DM::Sound_Play("ui", false);
			}
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