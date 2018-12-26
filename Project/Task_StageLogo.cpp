//-------------------------------------------------------------------
//ステージ前のロゴ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_StageLogo.h"
#include  "Task_Game_Manager.h"

#include  "ResourceManager.h"

namespace  FrontStageLogo
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		//リソースマネージャーの取得
		auto RM = ge->GetTask_One_GN<ResourceManager::Object>("リソースマネージャー", "リソースマネージャー");
		//タイトルシーンで使用するリソースの読み込み
		RM->Create(RM->FrontStageLogo);
		//BGMの再生開始
		DM::Sound_Play("Select", true);//再生開始
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		auto RM = ge->GetTask_One_GN<ResourceManager::Object>("リソースマネージャー", "リソースマネージャー");
		RM->Erase(RM->FrontStageLogo);
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
		this->desuCnt = 0;
		
		this->alphaCnt = 0;
		this->corsorNum = 1;
		this->xbai = (float)ge->screenWidth / 800.0f;
		this->ybai = (float)ge->screenHeight / 600.0f;
		//初期座標
		this->cursor[0] = { -500,-500 };
		this->cursor[1] = { (LONG)(15 * this->xbai) ,(LONG)(139 * this->ybai) };
		this->cursor[2] = { (LONG)(208 * this->xbai) ,(LONG)(221 * this->ybai) };
		this->cursor[3] = { (LONG)(426 * this->xbai) ,(LONG)(170 * this->ybai) };
		this->cursor[4] = { (LONG)(634 * this->xbai) ,(LONG)(161 * this->ybai) };
		this->cursor[5] = { (LONG)(11 * this->xbai) ,(LONG)(449 * this->ybai) };
		this->cursor[6] = { (LONG)(249 * this->xbai) ,(LONG)( 461* this->ybai) };
		this->cursor[7] = { (LONG)(439 * this->xbai) ,(LONG)(397 * this->ybai) };
		this->cursor[8] = { (LONG)(633 * this->xbai) ,(LONG)(426 * this->ybai) };
		//★タスクの生成
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放

		ge->KillAll_G("LogoFront");
		if (!ge->QuitFlag() && this->nextTaskCreate) 
		{
			//★引き継ぎタスクの生成
			auto nextTask = Game_Manager::Object::Create(true);
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		//初めの表示部分
		if (!FadeInOut(this->alphaCnt, 0.02f, 1, true))
		{
			return;
		}
		if (this->uiState != 決定)
		{
			this->uiState = 選択;
		}

		//ステージの選択
		auto in = ge->in1->GetState();
		if (in.LStick.BL.down&&this->corsorNum > 1&& this->uiState != 決定) 
		{
			this->corsorNum--;
			DM::Sound_Play("ui", false);
		}
		if (in.LStick.BR.down&&this->corsorNum < 8&& this->uiState != 決定)
		{
			this->corsorNum++;
			DM::Sound_Play("ui", false);
		}
		if (in.B1.down&&this->uiState == 選択)
		{
			this->uiState = 決定;
			DM::Sound_Play("ui", false);
		}
		if (this->uiState == 決定)
		{
			if (CntDelayMove(this->desuCnt, 1, 50))
			{
				ge->StageNum = this->corsorNum;
				this->Kill();
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//背景の描画
		ML::Box2D draw(0, 0, ge->screenWidth, ge->screenHeight);
		ML::Box2D src(0, 0, 800, 600);
		DG::Image_Draw("FLogoBackImg", draw, src, ML::Color(this->alphaCnt, 1, 1, 1));

		//カーソルの描画
		ML::Box2D drawC(this->cursor[this->corsorNum].x, this->cursor[this->corsorNum].y, int(153 * this->xbai), int(134 * this->ybai));
		ML::Box2D srcC(0, 0, 300, 250);
		DG::Image_Draw("CameraImg", drawC, srcC);

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