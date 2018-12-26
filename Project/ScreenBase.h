#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------------
//タイトル汎用スーパークラス
//-------------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

class ScreenBase : public BTask
{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆
public:
	typedef shared_ptr<ScreenBase>		SP;
	typedef weak_ptr<ScreenBase>		WP;
public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	//キャラクタ共通メンバ変数
	ML::Vec2	pos;
	ML::Box2D	hitBase;
	enum UiState 
	{
		無,
		移動中,
		選択,
		決定,
		移動終わり
	};
	UiState uiState;
	float alphaCnt;
	bool alphaFlag;
	bool described;
	int desuCnt;
	//インクリメントなどを行う状態遷移がしたいためあえてintで使用する
	enum
	{
		次へ,
		説明,
		終了
	};
	int corsorNum;

	//メンバ変数に最低限の初期化を行う
	//★★メンバ変数を追加したら必ず初期化も追加すること★★
	ScreenBase()
		:pos(0, 0)
		, hitBase(0, 0, 0, 0)
		, uiState(無)
		, alphaCnt(0)
		, alphaFlag(false)
		, described(false)
		, corsorNum(次へ)
		, desuCnt(0)
	{
	}
	virtual ~ScreenBase() {}

	//メソッド
	//カウント系
	/*
	最大まで行くと最小を初期値に代入する
	使用するもの 使用するカウント 加算値 カウント最大 カウント最小
	*/
	virtual void CntLoopMove(int cnt,int add,int max,int min);
	//minの値がない場合0を代入する
	virtual void CntLoopMove(int cnt, int add, int max);
	/*
	指定したカウント分遅延させる
	使用するもの 使用するカウント・加算値・カウント最大
	*/
	virtual bool CntDelayMove(int& cnt, int add, int max);

	//カラー系
	/*
	カラー(a,R,G,B)を加減して表示する
	使用するもの 使用するカウント 加算値 カウント最大orカウント最小 フェードインかフェードアウトの判定(true=フェードイン,false=フェードアウト)
	*/
	virtual bool FadeInOut(float& cnt, float add, float limit, bool inOut);
	/*
	カラー(a,R,G,B)の上げ下げを繰り返す
	使用するもの 使用するカウント 加算値 カウント最大 カウント最小 フェードインかフェードアウトの判定(true=フェードイン,false=フェードアウト)
	*/
	virtual float ColorCntMove(float cnt, float add, float max, float min);

	//メニュー選択の選択部分(必要に応じてオーバーライドしている)
	virtual void SelectMenu();
	//メニュー選択の決定部分(必要に応じてオーバーライドしている)
	virtual void SelectAction();
};