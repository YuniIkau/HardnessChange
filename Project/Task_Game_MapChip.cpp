//-------------------------------------------------------------------
//2次元配列マップ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game_MapChip.h"
#include  "Task_Game_Player.h"
#include  "Task_Game_Camera.h"

namespace  Game_MapChip
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
		this->render2D_Priority[1] = 0.8f;
		//マップのゼロクリア
		for (int y = 0; y < 100; y++)
		{
			for (int x = 0; x < 100; x++)
			{
				this->arr[y][x] = 0;
			}
		}
		this->sizeX = 0;
		this->sizeY = 0;
		this->hitBase = ML::Box2D(0, 0, 0, 0);
		//マップチップ情報の初期化
		for (int c = 0; c < 32; c++)
		{
			int x = (c % 8);
			int y = (c / 8);
			this->chip[c] = ML::Box2D(x * 32, y * 32, 32, 32);
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("フィールド");
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
		for (int y = 0; y <= sizeY; y++)
		{
			for (int x = 0; x <= sizeX; x++)
			{
				ML::Box2D draw(x * ge->kukeisize, y * ge->kukeisize, ge->kukeisize, ge->kukeisize);
				DG::Image_Draw("MapChipImg", draw, this->chip[this->arr[y][x]]);
			}
		}
	}
	//マップファイル読み込み
	bool	Object::Load(const	string& fpath_)
	{
		//ファイルを開く(読み込み)
		ifstream fin(fpath_);
		//読み込み失敗時はすぐにreturn
		if (!fin) { return false; }
		//マップ配列サイズの読み込み
		fin >> this->sizeX >> this->sizeY;
		this->hitBase = ML::Box2D(0, 0, this->sizeX * ge->kukeisize, this->sizeY * ge->kukeisize);
		//マップ配列データの読み込み
		for (int y = 0; y < this->sizeY; y++)
		{
			for (int x = 0; x < this->sizeX; x++)
			{
				fin >> this->arr[y][x];
			}
		}
		fin.close();
		return true;
	}
	//当たり判定
	bool Object::CheckHit(const	ML::Box2D&h_)
	{
		auto cm = ge->GetTask_One_GN<Game_Camera::Object>("ゲーム", "カメラ");
		RECT r = { h_.x,h_.y,h_.x + h_.w,h_.y + h_.h };
		//矩形がマップ外に出ていたら丸め込みを行う
		RECT m = 
		{
		this->hitBase.x,
		this->hitBase.y,
		this->hitBase.x+this->hitBase.w,
		this->hitBase.y+this->hitBase.h
		};
		if (r.left < m.left) { r.left = m.left; }			//左に飛び出た部分をカット
		if (r.top < m.top) { r.top = m.top; }				//上に飛び出た部分をカット
		if (r.right > m.right) { r.right = m.right; }		//右に飛び出た部分をカット
		if (r.bottom > m.bottom) { r.bottom = m.bottom; }	//下に飛び出た部分をカット
		//ループ範囲調整
		int sx, sy, ex, ey;
		sx = r.left / ge->kukeisize;
		sy = r.top / ge->kukeisize;
		ex = (r.right - 1) / ge->kukeisize;
		ey = (r.bottom - 1) / ge->kukeisize;
		auto	in = ge->in1->GetState();
		//範囲外の障害物を探す
		for (int y = sy; y <= ey; y++)
		{
			for (int x = sx; x <= ex; x++)
			{
				//true=侵入不可 false=侵入可能
				if (6 == this->arr[y][x] || 7 == this->arr[y][x]) { ge->gameState = ge->クリア; }
				if (8 == this->arr[y][x]) { return true; }
				if (12 == this->arr[y][x] || 13 == this->arr[y][x] || 14 == this->arr[y][x])
				{
					//スティックの倒す深度の変更
					DI::deadzone = 0.99f;
					if (in.LStick.BD.on)
					{
						return false;
					}
					return true;
				}
				//スティックの倒す深度の変更
				DI::deadzone = 0.24f;
				if (16 <= this->arr[y][x] && this->arr[y][x] <= 31) { return true; }
				if (37 <= this->arr[y][x]) { return true; }
			}
		}
		return false;
	}
	void Object::AjastCameraPos()
	{
		//カメラとマップの範囲を用意
		RECT c =
		{
			ge->camera2D.x,
			ge->camera2D.y,
			ge->camera2D.x + ge->camera2D.w,
			ge->camera2D.y + ge->camera2D.h
		};
		RECT m =
		{
			this->hitBase.x,
			this->hitBase.y,
			this->hitBase.x + this->hitBase.w,
			this->hitBase.y + this->hitBase.h,
		};
		//カメラの位置調整
		if (c.right > m.right) { ge->camera2D.x = m.right - ge->camera2D.w; }
		if (c.bottom > m.bottom) { ge->camera2D.y = m.bottom - ge->camera2D.h; }
		if (c.left < m.left) { ge->camera2D.x = m.left; }
		if (c.top < m.top) { ge->camera2D.y = m.top; }
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