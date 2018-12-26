//-------------------------------------------------------------------
//プレイヤ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game_Player.h"
#include  "Task_Game_Camera.h"

namespace  Game_Player
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
		this->hitBase = ML::Box2D(0, 0, 40, 40);
		this->headBase = ML::Box2D(0, -1, 20, 1);
		this->hitFlag = true;
		this->headhitFlag = true;
		this->fallSpeed = 0;//現在の落下上昇速度
		this->jumpPow = -10.5;//初速
		this->PreY = 0;
		ge->gameState = ge->Game;
		this->maxSpeed = 5.0f;		//最大移動速度（横）
		this->addSpeed = 1.0f;		//歩行加速度（地面の影響である程度打ち消される
		this->decSpeed = 0.2f;		//接地状態の時の速度減衰量（摩擦
		this->ang = 0;
		this->angCnt = 0;
		this->desuline = ge->kukeisize * 6;
		this->animFlag = false;
		this->desuCnt = 0;
		//★タスクの生成

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
		this->animFlag = false;
		this->PreY = this->pos.y;
		if (ge->gameState == ge->ポーズ)
		{
			this->pos = this->pos;
			return;
		}
		if (this->playerState == 死)
		{
			this->desuCnt++;
			if (this->desuCnt > 100)
			{
				ge->gameState = ge->ゲームオーバー;
				this->Kill();
			}
			return;
		}
		//減衰処理
		Attenuation();
		HorizontalMove();
		CollisionCheck();
		if (this->playerState == 硬い)
		{
			if (this->DesuCnt < -this->desuline && this->hitFlag == true)
			{
				DM::Sound_Play("Desu", false);
				this->playerState = 死;
			}
		}
		if (this->hitFlag == true)
		{
			DesuCnt = 0;
		}
		Jump();
		//回転するアニメーション
		if (this->animFlag == true)
		{
			this->angCnt++;
			if (angCnt > 3)
			{
				angCnt = 0;
				ang++;
			}
			if (ang >= 360)
			{
				ang = 0;
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->hitBase.OffsetCopy(this->pos);
		ML::Box2D src(0, 0, 100, 100);
		ML::Vec2 genten(20, 20);
		if (this->playerState == 柔らかい)
		{
			src.Offset(100, 0);
		}
		if (this->playerState == 死  )
		{
			ML::Box2D drawCopy(0, 0, 50, 50);
			draw.w += drawCopy.w;
			draw.h += drawCopy.h;
			ML::Box2D srcCopy(300, 0, 150, 150);
			src = srcCopy;
			this->ang = 0;
			genten = ML::Vec2(20, 20);
		}
		DG::Image_Rotation("PlayerImg", (float)this->ang, genten);
		DG::Image_Draw("PlayerImg", draw, src);
	}

	//減衰処理
	void Object::Attenuation()
	{
		if (this->moveVec.x < 0)
		{
			this->animFlag = true;
			this->moveVec.x = min(this->moveVec.x + this->decSpeed, 0);
		}
		else if (this->moveVec.x > 0)
		{
			this->animFlag = true;
			this->moveVec.x = max(this->moveVec.x - this->decSpeed, 0);
		}
	}
	//水平移動
	void Object::HorizontalMove()
	{
		auto in = ge->in1->GetState();
		if (in.LStick.BL.on)
		{
			this->animFlag = true;
			this->addSpeed += 0.2f;
			if (this->addSpeed > this->maxSpeed)
			{
				this->addSpeed = this->maxSpeed;
			}
			this->moveVec.x = -this->addSpeed;
		}
		if (in.LStick.BR.on)
		{
			this->animFlag = true;
			this->addSpeed += 0.2f;
			if (this->addSpeed > this->maxSpeed)
			{
				this->addSpeed = this->maxSpeed;
			}
			this->moveVec.x = this->addSpeed;
		}
		if (!(in.LStick.BR.on || in.LStick.BL.on))
		{
			this->addSpeed = 1.0f;
		}
	}
	//各種の当たり判定
	void Object::CollisionCheck()
	{
		auto cm = ge->GetTask_One_GN<Game_Camera::Object>("ゲーム", "カメラ");
		//足元接触判定
		this->hitFlag = this->CheckFoot();
		//頭上判定
		this->headhitFlag = this->CheckHead();
		ML::Box2D head = this->headBase.OffsetCopy(this->pos);
		if (this->headhitFlag)
		{
			this->fallSpeed = 1;//頭を打ったら落下
		}
		//めり込まない移動
		ML::Vec2  est = this->moveVec;
		this->MapChipCheckMove(est);
		this->hitFlag = this->CheckFoot();
		if (this->hitFlag == true)
		{
			//接地していたら落下速度ゼロ
			this->fallSpeed = 0;
		}


		ML::Box2D hit = this->hitBase.OffsetCopy(this->pos);
		if (true == cm->CmCheckHit(hit))
		{
			this->playerState = 柔らかい;
		}
		else
		{
			this->playerState = 硬い;
		}
	}
	//
	void Object::Jump()
	{
		auto in = ge->in1->GetState();
		if (this->playerState == 柔らかい)
		{
			if (this->hitFlag == true)
			{
				//ジャンプ
				if (in.Trigger.R2.down)
				{
					this->fallSpeed = this->jumpPow;
				}
			}
		}
		//上下移動はfallSpeedの値
		this->moveVec.y = this->fallSpeed;
		if (this->hitFlag == false)
		{
			this->animFlag = true;
			//重力加速0.27* ((g / f) / f)*m 　≒３現実のものと同じにするため3をかける
			this->fallSpeed += ((this->gravity / this->frame) / this->frame)*this->meter * 3;
			if (this->fallSpeed > 10)
			{
				this->fallSpeed = 10;
			}
		}
		if (this->PreY < this->pos.y)
		{
			this->DesuCnt += (int)(this->PreY - this->pos.y);
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