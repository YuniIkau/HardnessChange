//-------------------------------------------------------------------
//タイトルロゴの表示
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title_Manager.h"
#include  "Task_Title_Logo.h"

namespace  Title_Logo
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
		//一番奥に設定
		this->render2D_Priority[1] = 0.1f;
		this->pos.x = -400;
		this->pos.y = 600/4;
		this->draw = ML::Box2D((int)this->pos.x, (int)this->pos.y, 500, 200);
		this->src = ML::Box2D(0, 0, 700, 200);
		this->angle = 0;
		this->pX = 0;
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//このタスクでは何もしない
		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		if (this->uiState == 移動終わり)
		{
			return;
		}
		RotationMove();
		if (this->uiState == 決定)
		{
			auto manager = ge->GetTask_One_GN<Title_Manager::Object>("タイトル", "統括");
			manager->titleState = manager->プリーズロゴ;
			this->uiState = 移動終わり;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		draw.x = (int)this->pos.x;
		draw.y = (int)this->pos.y;
		DG::Image_Rotation("TitleFontColorImg", angle, ML::Vec2(180, 40));
		DG::Image_Draw("TitleFontColorImg", this->draw, this->src);
	}

	//横に回転しながら移動
	void Object::RotationMove()
	{
		if (this->pos.x >= (ge->screenWidth / 2) - ((this->draw.w / 2) - 30))
		{
			this->pos.x = ((float)(ge->screenWidth / 2) - ((this->draw.w / 2) - 30));
			this->angle = 0;
			this->uiState = 決定;
		}
		else {
			this->pX += 12;
			this->angle += ML::ToRadian(15);
			this->pos.x = -400 + cos(angle) + pX;
			this->pos.y = ge->screenHeight / 4 + sin(angle);
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