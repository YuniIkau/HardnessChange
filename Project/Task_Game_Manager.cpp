//-------------------------------------------------------------------
//ゲームシーン統括
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game_Manager.h"
#include  "Task_Game_MapChip.h"
#include  "Task_Game_Player.h"
#include  "Task_Game_Camera.h"
#include  "Task_Game_BackImg.h"
#include  "Task_Game_TransmissionBlock.h"
#include  "Task_RearStageLogo.h"
#include  "Task_Game_Pause.h"

#include  "ResourceManager.h"

namespace  Game_Manager
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		//リソースマネージャーの取得
		auto RM = ge->GetTask_One_GN<ResourceManager::Object>("リソースマネージャー", "リソースマネージャー");
		//タイトルシーンで使用するリソースの読み込み
		RM->Create(RM->Game);
		//BGMの再生開始
		DM::Sound_Play("GameStage", true);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		auto RM = ge->GetTask_One_GN<ResourceManager::Object>("リソースマネージャー", "リソースマネージャー");
		RM->Erase(RM->Game);
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
		//一つのマスの一辺の長さ
		ge->kukeisize = 48;
		ge->camera2D = ML::Box2D(0, 0, ge->screenWidth, ge->screenHeight);	

		//★タスクの生成
		//背景
		auto b = Game_BackImg::Object::Create(true);
		//マップ
		auto m = Game_MapChip::Object::Create(true);
		string Num = std::to_string(ge->StageNum);
		string pas = "./data/マップ/map" + Num + ".txt";
		m->Load(pas);
		//プレイヤ
		auto pl = Game_Player::Object::Create(true);
		pl->pos.x = 60;
		pl->pos.y = 900;
		//カメラ
		auto cm = Game_Camera::Object::Create(true);
		cm->pos.x = pl->pos.x;
		cm->pos.y = pl->pos.y-300;
		//透過マス
		auto tB = Game_TransmissionBlock::Object::Create(true);
		string tpas = "./data/ギミック/透過配置" + Num + ".txt";
		tB->GimmickLoad(tpas);
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("ゲーム");

		if (ge->StageNum == 8 && ge->gameState == ge->クリア)
		{
			ge->gameState = ge->オールクリア;
		}
		if (!ge->QuitFlag() && this->nextTaskCreate) 
		{
			//★引き継ぎタスクの生成
			if (ge->gameState != ge->ポーズ)
			{
				//リザルトのシーンへ移動
				auto nextTask = RearStageLogo::Object::Create(true);
			}
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto in = ge->in1->GetState();
		
		switch (ge->gameState)
		{
		case ge->クリア:
		case ge->ゲームオーバー:
			this->Kill();
		default:
				break;
		}
		//ポーズ画面に入るか
		if (in.ST.down&&ge->gameState != ge->ポーズ)
		{
			ge->gameState = ge->ポーズ;
			DM::Sound_Stop("GameStage");
			DM::Sound_Play("ui", false);
			//ポーズタスク表示
			auto P = Game_Pause::Object::Create(true);
		}
		//ポーズ画面から戻るか
		else if(in.ST.down&&ge->gameState == ge->ポーズ)
		{
			DM::Sound_Play("GameStage", true);
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
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