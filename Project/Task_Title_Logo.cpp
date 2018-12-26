//-------------------------------------------------------------------
//�^�C�g�����S�̕\��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title_Manager.h"
#include  "Task_Title_Logo.h"

namespace  Title_Logo
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		//��ԉ��ɐݒ�
		this->render2D_Priority[1] = 0.1f;
		this->pos.x = -400;
		this->pos.y = 600/4;
		this->draw = ML::Box2D((int)this->pos.x, (int)this->pos.y, 500, 200);
		this->src = ML::Box2D(0, 0, 700, 200);
		this->angle = 0;
		this->pX = 0;
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���̃^�X�N�ł͉������Ȃ�
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		if (this->uiState == �ړ��I���)
		{
			return;
		}
		RotationMove();
		if (this->uiState == ����)
		{
			auto manager = ge->GetTask_One_GN<Title_Manager::Object>("�^�C�g��", "����");
			manager->titleState = manager->�v���[�Y���S;
			this->uiState = �ړ��I���;
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		draw.x = (int)this->pos.x;
		draw.y = (int)this->pos.y;
		DG::Image_Rotation("TitleFontColorImg", angle, ML::Vec2(180, 40));
		DG::Image_Draw("TitleFontColorImg", this->draw, this->src);
	}

	//���ɉ�]���Ȃ���ړ�
	void Object::RotationMove()
	{
		if (this->pos.x >= (ge->screenWidth / 2) - ((this->draw.w / 2) - 30))
		{
			this->pos.x = ((float)(ge->screenWidth / 2) - ((this->draw.w / 2) - 30));
			this->angle = 0;
			this->uiState = ����;
		}
		else {
			this->pX += 12;
			this->angle += ML::ToRadian(15);
			this->pos.x = -400 + cos(angle) + pX;
			this->pos.y = ge->screenHeight / 4 + sin(angle);
		}
	}

	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
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
	//���\�[�X�N���X�̐���
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