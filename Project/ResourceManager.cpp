//-------------------------------------------------------------------
//リソースの管理
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "ResourceManager.h"

namespace  ResourceManager
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
		//高頻度で使用するリソースは最初から使用ずっと読み込んでおく
		DG::Image_Create("CameraImg", "./data/キャラクター/カメラ矩形.png");
		//SEの読み込み
		DM::Sound_CreateSE("ui", "./data/サウンド/UI.wav");
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		DG::Image_Erase("CameraImg");
		ge->KillAll_G("リソースマネージャー");
		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
	}
	//各シーンのリソースの読み込み
	void Object::Create(SceneState sceneState)
	{
		switch (sceneState)
		{
		case Title:
			DG::Image_Create("TitleBackImg", "./data/背景/タイトル背景.png");
			DG::Image_Create("TitleFontColorImg", "./data/文字/タイトル.png");
			DG::Image_Create("TitleFontMonoImg", "./data/文字/タイトルモノクロ.png");
			DG::Image_Create("SetumeiImg", "./data/イメージ/イメージ.png");
			//再生時間が長いモノはStreamを選択（主にBGM）
			DM::Sound_CreateStream("Title", "./data/サウンド/タイトル.wav");
			break;
		case FrontStageLogo:
			DG::Image_Create("FLogoBackImg", "./data/背景/ステージセレクト.png");
			DM::Sound_CreateStream("Select", "./data/サウンド/セレクト.wav");
			break;
		case Game:
			if (ge->StageNum < 4)
			{
				DG::Image_Create("BackImg", "./data/背景/背景1.png");
				DG::Image_Create("BackMonoImg", "./data/背景/背景1モノクロ.png");
			}
			else
			{
				DG::Image_Create("BackImg", "./data/背景/背景2.png");
				DG::Image_Create("BackMonoImg", "./data/背景/背景2モノクロ.png");
			}
			DG::Image_Create("MapChipImg", "./data/マップ/MapChip0.png");
			DG::Image_Create("BackGrayImg", "./data/背景/黒.png");
			DG::Image_Create("TransmissionBlock", "./data/ギミック/透過.png");
			DG::Image_Create("PauseImg", "./data/文字/ポーズ.png");
			DG::Image_Create("PauseMonoImg", "./data/文字/ポーズモノクロ.png");
			DG::Image_Create("PlayerImg", "./data/キャラクター/Player.png");
			DG::Image_Create("SetumeiImg", "./data/イメージ/イメージ.png");
			DM::Sound_CreateStream("GameStage", "./data/サウンド/bgm.wav");
			DM::Sound_CreateSE("Desu", "./data/サウンド/死亡.wav");
			break;
		case RearStageLogo:
			DG::Image_Create("RearLogoClearImg", "./data/背景/ゲームクリア.png");
			DG::Image_Create("RearLogoOverImg", "./data/背景/ゲームオーバー.png");
			DG::Image_Create("RearMenuImg", "./data/文字/終了ロゴ.png");
			DG::Image_Create("RearMenuMonoImg", "./data/文字/終了ロゴモノクロ.png");
			DG::Image_Create("StageLogoImg", "./data/文字/リザルト.png");
			DM::Sound_CreateStream("GameOver", "./data/サウンド/ゲームオーバー.wav");
			break;
		default:
			break;
		}
	}
	//各シーンのリソースの解放
	void Object::Erase(SceneState sceneState)
	{
		switch (sceneState)
		{
		case Title:
			DG::Image_Erase("TitleBackImg");
			DG::Image_Erase("TitleFontColorImg");
			DG::Image_Erase("TitleFontMonoImg");
			DG::Image_Erase("SetumeiImg");
			DM::Sound_Erase("Title");
			break;
		case FrontStageLogo:
			DG::Image_Erase("FLogoBackImg");
			DM::Sound_Erase("Select");
			break;
		case Game:
			if (ge->StageNum < 4)
			{
				DG::Image_Erase("BackImg");
				DG::Image_Erase("BackMonoImg");
			}
			else
			{
				DG::Image_Erase("BackImg");
				DG::Image_Erase("BackMonoImg");
			}
			DG::Image_Erase("MapChipImg");
			DG::Image_Erase("BackGrayImg");
			DG::Image_Erase("TransmissionBlock");
			DG::Image_Erase("PauseImg");
			DG::Image_Erase("PauseMonoImg");
			DG::Image_Erase("PlayerImg");
			DG::Image_Erase("SetumeiImg");
			DM::Sound_Erase("GameStage");
			DM::Sound_Erase("Desu");
			break;
		case RearStageLogo:
			DG::Image_Erase("RearLogoClearImg");
			DG::Image_Erase("RearLogoOverImg");
			DG::Image_Erase("RearMenuImg");
			DG::Image_Erase("RearMenuMonoImg");
			DG::Image_Erase("StageLogoImg");
			DM::Sound_Erase("GameOver");
		default:
			break;
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