#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//プレイヤ
//-------------------------------------------------------------------
#include "BChara.h"

namespace  Game_Player
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("ゲーム");	//グループ名
	const  string  defName("プレイヤ");	//タスク名
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
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
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
		//BCharaに持たせていない変数・メソッドのみここに書く
		//変数
		//落下速度
		float       fallSpeed;
		//ジャンプの初速
		float       jumpPow;
		//落下距離を調べるための移動前のY
		float		PreY;
		//最大速度
		float		maxSpeed;
		//加速度
		float		addSpeed;	
		//減速度
		float		decSpeed;	
		//プレイヤの角度
		int			ang;
		int			angCnt;
		bool		animFlag;
		//死亡する高さ
		int			desuline;
		int			desuCnt;
		enum PlayerState{ 普, 死, 硬い, 柔らかい };
		PlayerState playerState;
		//メソッド
		//減衰処理
		void Attenuation();
		//水平移動
		void HorizontalMove();
		//各種の当たり判定
		void CollisionCheck();
		//ジャンプ処理
		void Jump();
	};
}