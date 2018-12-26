//-------------------------------------------------------------------
//�^�C�g������
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title_Back.h"
#include  "Task_Title_Logo.h"
#include  "Task_Title_Menu.h"
#include  "Task_Title_Manager.h"

#include  "ResourceManager.h"

namespace  Title_Manager
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		//�V���O���g��
		if (!ge->GetTask_One_GN<ResourceManager::Object>("���\�[�X�}�l�[�W���[", "���\�[�X�}�l�[�W���["))
		{
			//���\�[�X�}�l�[�W���[�̐���
			auto RM = ResourceManager::Object::Create(true);
		}
		auto RM = ge->GetTask_One_GN<ResourceManager::Object>("���\�[�X�}�l�[�W���[", "���\�[�X�}�l�[�W���[");
		//�^�C�g���V�[���Ŏg�p���郊�\�[�X�̓ǂݍ���
		RM->Create(RM->Title);
		//BGM�̍Đ��J�n
		DM::Sound_Play("Title", true);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		auto RM = ge->GetTask_One_GN<ResourceManager::Object>("���\�[�X�}�l�[�W���[", "���\�[�X�}�l�[�W���[");
		RM->Erase(RM->Title);
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
		ge->StageNum = 0;
		this->titleState = �w�i;
		
		//���^�X�N�̐���
		auto Back = Title_Back::Object::Create(true);
		auto Logo = Title_Logo::Object::Create(true);
		auto Menu = Title_Menu::Object::Create(true);
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
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