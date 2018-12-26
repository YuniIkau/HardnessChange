#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------------
//�L�����N�^�ėp�X�[�p�[�N���X
//-------------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

class BChara : public BTask
{
	//�ύX�s����������������������������
public:
	typedef shared_ptr<BChara>		SP;
	typedef weak_ptr<BChara>		WP;
public:
	//�ύX������������������������������
	//�L�����N�^���ʃ����o�ϐ�
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


	//�����o�ϐ��ɍŒ���̏��������s��
	//���������o�ϐ���ǉ�������K�����������ǉ����邱�Ɓ���
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

	//�L�����N�^���ʃ��\�b�h
	//�߂荞�܂Ȃ��ړ�����
	virtual void MapChipCheckMove(ML::Vec2& est_);
	//�����ڐG����
	virtual bool CheckFoot();
	//���㔻��
	virtual bool CheckHead();
	//���߃M�~�b�N�̓ǂݍ���
	virtual bool GimmickLoad(const	string& fpath_);

};