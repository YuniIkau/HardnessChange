#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�v���C��
//-------------------------------------------------------------------
#include "BChara.h"

namespace  Game_Player
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�Q�[��");	//�O���[�v��
	const  string  defName("�v���C��");	//�^�X�N��
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
	//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		//BChara�Ɏ������Ă��Ȃ��ϐ��E���\�b�h�݂̂����ɏ���
		//�ϐ�
		//�������x
		float       fallSpeed;
		//�W�����v�̏���
		float       jumpPow;
		//���������𒲂ׂ邽�߂̈ړ��O��Y
		float		PreY;
		//�ő呬�x
		float		maxSpeed;
		//�����x
		float		addSpeed;	
		//�����x
		float		decSpeed;	
		//�v���C���̊p�x
		int			ang;
		int			angCnt;
		bool		animFlag;
		//���S���鍂��
		int			desuline;
		int			desuCnt;
		enum PlayerState{ ��, ��, �d��, �_�炩�� };
		PlayerState playerState;
		//���\�b�h
		//��������
		void Attenuation();
		//�����ړ�
		void HorizontalMove();
		//�e��̓����蔻��
		void CollisionCheck();
		//�W�����v����
		void Jump();
	};
}