#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------------
//�^�C�g���ėp�X�[�p�[�N���X
//-------------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

class ScreenBase : public BTask
{
	//�ύX�s����������������������������
public:
	typedef shared_ptr<ScreenBase>		SP;
	typedef weak_ptr<ScreenBase>		WP;
public:
	//�ύX������������������������������
	//�L�����N�^���ʃ����o�ϐ�
	ML::Vec2	pos;
	ML::Box2D	hitBase;
	enum UiState 
	{
		��,
		�ړ���,
		�I��,
		����,
		�ړ��I���
	};
	UiState uiState;
	float alphaCnt;
	bool alphaFlag;
	bool described;
	int desuCnt;
	//�C���N�������g�Ȃǂ��s����ԑJ�ڂ����������߂�����int�Ŏg�p����
	enum
	{
		����,
		����,
		�I��
	};
	int corsorNum;

	//�����o�ϐ��ɍŒ���̏��������s��
	//���������o�ϐ���ǉ�������K�����������ǉ����邱�Ɓ���
	ScreenBase()
		:pos(0, 0)
		, hitBase(0, 0, 0, 0)
		, uiState(��)
		, alphaCnt(0)
		, alphaFlag(false)
		, described(false)
		, corsorNum(����)
		, desuCnt(0)
	{
	}
	virtual ~ScreenBase() {}

	//���\�b�h
	//�J�E���g�n
	/*
	�ő�܂ōs���ƍŏ��������l�ɑ������
	�g�p������� �g�p����J�E���g ���Z�l �J�E���g�ő� �J�E���g�ŏ�
	*/
	virtual void CntLoopMove(int cnt,int add,int max,int min);
	//min�̒l���Ȃ��ꍇ0��������
	virtual void CntLoopMove(int cnt, int add, int max);
	/*
	�w�肵���J�E���g���x��������
	�g�p������� �g�p����J�E���g�E���Z�l�E�J�E���g�ő�
	*/
	virtual bool CntDelayMove(int& cnt, int add, int max);

	//�J���[�n
	/*
	�J���[(a,R,G,B)���������ĕ\������
	�g�p������� �g�p����J�E���g ���Z�l �J�E���g�ő�or�J�E���g�ŏ� �t�F�[�h�C�����t�F�[�h�A�E�g�̔���(true=�t�F�[�h�C��,false=�t�F�[�h�A�E�g)
	*/
	virtual bool FadeInOut(float& cnt, float add, float limit, bool inOut);
	/*
	�J���[(a,R,G,B)�̏グ�������J��Ԃ�
	�g�p������� �g�p����J�E���g ���Z�l �J�E���g�ő� �J�E���g�ŏ� �t�F�[�h�C�����t�F�[�h�A�E�g�̔���(true=�t�F�[�h�C��,false=�t�F�[�h�A�E�g)
	*/
	virtual float ColorCntMove(float cnt, float add, float max, float min);

	//���j���[�I���̑I�𕔕�(�K�v�ɉ����ăI�[�o�[���C�h���Ă���)
	virtual void SelectMenu();
	//���j���[�I���̌��蕔��(�K�v�ɉ����ăI�[�o�[���C�h���Ă���)
	virtual void SelectAction();
};