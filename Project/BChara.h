#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------------
//キャラクタ汎用スーパークラス
//-------------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

class BChara : public BTask
{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆
public:
	typedef shared_ptr<BChara>		SP;
	typedef weak_ptr<BChara>		WP;
public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	//キャラクタ共通メンバ変数
	ML::Vec2	pos;
	ML::Vec2	Gimmickpos[100];
	ML::Box2D	hitBase;
	ML::Box2D	GimmickhitBase[100];
	ML::Box2D   headBase;
	ML::Vec2	moveVec;
	int			moveCnt;
	bool		hitFlag;
	bool        headhitFlag;
	float       gravity;
	int         frame;
	int         meter;
	int			DesuCnt;
	WP			target;
	string GimmickName;
	int sizeX, sizeY, sizeG;


	//メンバ変数に最低限の初期化を行う
	//★★メンバ変数を追加したら必ず初期化も追加すること★★
	BChara()
		:pos(0, 0)
		, hitBase(0, 0, 0, 0)
		, headBase(0, 0, 0, 0)
		, moveVec(0, 0)
		, moveCnt(0)
		, hitFlag(false)
		, headhitFlag(true)
		, gravity(9.8f)
		, frame(60)
		, meter(32)
		, DesuCnt(0)
		, GimmickName("Non")
		, sizeX(0)
		, sizeY(0)
		, sizeG(0)
	{
	}
	virtual ~BChara(){}

	//キャラクタ共通メソッド
	//めり込まない移動処理
	virtual void MapChipCheckMove(ML::Vec2& est_);
	//足元接触判定
	virtual bool CheckFoot();
	//頭上判定
	virtual bool CheckHead();
	//透過ギミックの読み込み
	virtual bool GimmickLoad(const	string& fpath_);

};