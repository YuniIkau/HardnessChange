#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//2次元配列マップ
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

namespace  Game_MapChip
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("ゲーム");	//グループ名
	const  string  defName("マップチップ");	//タスク名
	//-------------------------------------------------------------------
	class  Resource
	{
		bool  Initialize();
		bool  Finalize();
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//共有する変数はここに追加する
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();		//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		//変数
		int			arr[100][100];
		int			sizeY, sizeX;
		ML::Box2D	hitBase;
		ML::Box2D	chip[32];
		//メソッド
		//マップ読み込み
		bool Load(const	string&fpath_);
		//当たり判定
		bool CheckHit(const	ML::Box2D& hit_);
		//スクロール停止
		void AjastCameraPos();
	};
}