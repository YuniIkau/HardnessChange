//-------------------------------------------------------------------
//透過マス用ギミック
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game_TransmissionBlock.h"
#include  "Task_Game_Camera.h"
#include  "Task_Game_Player.h"

namespace  Game_TransmissionBlock
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
		for (int i = 0; i <this->sizeG; i++)
		{
			this->Gflag[i] = true;
			this->Gimmickpos[i] = ML::Vec2(0, 0);
			this->GimmickhitBase[i] = ML::Box2D(0, 0, ge->kukeisize, ge->kukeisize);
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		for (int i = 0; i < this->sizeG; i++)
		{
			this->GimmickhitBase[i].OffsetCopy((int)this->Gimmickpos[i].x, (int)this->Gimmickpos[i].y);
			auto cm = ge->GetTask_One_GN<Game_Camera::Object>("ゲーム", "カメラ");
			//カメラに透過ブロックが入っていれば判定をするようにする
			if (cm->Object::CmCheckHit(this->GimmickhitBase[i]) != true)
			{
				this->Gflag[i] = true;
			}
			else
			{
				this->Gflag[i] = false;
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		for (int i = 0; i < this->sizeG; i++)
		{
			this->GimmickhitBase[i] = ML::Box2D((int)this->Gimmickpos[i].x, (int)this->Gimmickpos[i].y, ge->kukeisize, ge->kukeisize);
			ML::Box2D draw = this->GimmickhitBase[i];
			ML::Box2D src(0, 0, 256, 256);
			this->alpha[i] = ML::Color(1, 1, 1, 1);
			//カメラに入っていないところを半透明にする
			if (this->Gflag[i] == true)
			{
				this->alpha[i] = ML::Color(0.2f, 1, 1, 1);
			}
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			DG::Image_Draw("TransmissionBlock", draw, src, this->alpha[i]);
		}
	}
	//当たり判定
	bool Object::CheckHit(const	ML::Box2D&h_, int i)
	{
		this->GimmickhitBase[i] = ML::Box2D((int)this->Gimmickpos[i].x, (int)this->Gimmickpos[i].y, ge->kukeisize, ge->kukeisize);
		RECT r = { h_.x,h_.y,h_.x + h_.w,h_.y + h_.h };
		ML::Box2D hit = this->GimmickhitBase[i];
		RECT c = {
			hit.x,
			hit.y,
			hit.x + hit.w,
			hit.y + hit.h,
		};
		if (r.left < c.right)
		{
			if (r.right > c.left)
			{
				if (r.top < c.bottom)
				{
					if (r.bottom > c.top)
					{
						return true;
					}
				}
			}
		}
		return false;
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