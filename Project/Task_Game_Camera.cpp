//-------------------------------------------------------------------
//カメラ矩形
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game_Camera.h"

namespace  Game_Camera
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
		this->render2D_Priority[1] = 0.5f;
		this->hitBase = ML::Box2D(5, 5, 290, 320);
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		if (ge->gameState == ge->ポーズ)
		{
			return;
		}
		//カメラ移動と画面との判定
		CameraMove();
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, 300, 320);
		draw.Offset(this->pos);
		ML::Box2D src(0, 0, 300, 250);
		DG::Image_Draw("CameraImg", draw, src);
	}
	//カメラとプレイヤ、ギミックとの当たり判定
	bool Object::CmCheckHit(const ML::Box2D&h_)
	{
		RECT r = { h_.x,h_.y,h_.x + h_.w,h_.y + h_.h };
		ML::Box2D hit = this->hitBase.OffsetCopy(this->pos);
		//矩形がカメラ外に出ていたら丸め込みを行う
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

	//カメラ移動と画面との判定
	void Object::CameraMove()
	{
		auto in = ge->in1->GetState();
		POINT est{ 0,0 };
		if (in.RStick.BL.on) { est.x -= 4; }
		if (in.RStick.BR.on) { est.x += 4; }
		if (in.RStick.BU.on) { est.y -= 4; }
		if (in.RStick.BD.on) { est.y += 4; }
		ML::Box2D screenSize(0, 0, ge->screenWidth, ge->screenHeight);
		ML::Box2D thisHitSize = this->hitBase.OffsetCopy(this->pos.x + est.x, this->pos.y + est.y);
		if (screenSize.x > thisHitSize.x)
		{
			return;
		}
		if (screenSize.w < thisHitSize.x + thisHitSize.w)
		{
			return;
		}
		if (screenSize.y > thisHitSize.y)
		{
			return;
		}
		if (screenSize.h < thisHitSize.y + thisHitSize.h)
		{
			return;
		}
		this->pos.x += est.x;
		this->pos.y += est.y;
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