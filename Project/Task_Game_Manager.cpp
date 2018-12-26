//-------------------------------------------------------------------
//�Q�[���V�[������
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game_Manager.h"
#include  "Task_Game_MapChip.h"
#include  "Task_Game_Player.h"
#include  "Task_Game_Camera.h"
#include  "Task_Game_BackImg.h"
#include  "Task_Game_TransmissionBlock.h"
#include  "Task_RearStageLogo.h"
#include  "Task_Game_Pause.h"

#include  "ResourceManager.h"

namespace  Game_Manager
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		//���\�[�X�}�l�[�W���[�̎擾
		auto RM = ge->GetTask_One_GN<ResourceManager::Object>("���\�[�X�}�l�[�W���[", "���\�[�X�}�l�[�W���[");
		//�^�C�g���V�[���Ŏg�p���郊�\�[�X�̓ǂݍ���
		RM->Create(RM->Game);
		//BGM�̍Đ��J�n
		DM::Sound_Play("GameStage", true);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		auto RM = ge->GetTask_One_GN<ResourceManager::Object>("���\�[�X�}�l�[�W���[", "���\�[�X�}�l�[�W���[");
		RM->Erase(RM->Game);
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
		//��̃}�X�̈�ӂ̒���
		ge->kukeisize = 48;
		ge->camera2D = ML::Box2D(0, 0, ge->screenWidth, ge->screenHeight);	

		//���^�X�N�̐���
		//�w�i
		auto b = Game_BackImg::Object::Create(true);
		//�}�b�v
		auto m = Game_MapChip::Object::Create(true);
		string Num = std::to_string(ge->StageNum);
		string pas = "./data/�}�b�v/map" + Num + ".txt";
		m->Load(pas);
		//�v���C��
		auto pl = Game_Player::Object::Create(true);
		pl->pos.x = 60;
		pl->pos.y = 900;
		//�J����
		auto cm = Game_Camera::Object::Create(true);
		cm->pos.x = pl->pos.x;
		cm->pos.y = pl->pos.y-300;
		//���߃}�X
		auto tB = Game_TransmissionBlock::Object::Create(true);
		string tpas = "./data/�M�~�b�N/���ߔz�u" + Num + ".txt";
		tB->GimmickLoad(tpas);
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�Q�[��");

		if (ge->StageNum == 8 && ge->gameState == ge->�N���A)
		{
			ge->gameState = ge->�I�[���N���A;
		}
		if (!ge->QuitFlag() && this->nextTaskCreate) 
		{
			//�������p���^�X�N�̐���
			if (ge->gameState != ge->�|�[�Y)
			{
				//���U���g�̃V�[���ֈړ�
				auto nextTask = RearStageLogo::Object::Create(true);
			}
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto in = ge->in1->GetState();
		
		switch (ge->gameState)
		{
		case ge->�N���A:
		case ge->�Q�[���I�[�o�[:
			this->Kill();
		default:
				break;
		}
		//�|�[�Y��ʂɓ��邩
		if (in.ST.down&&ge->gameState != ge->�|�[�Y)
		{
			ge->gameState = ge->�|�[�Y;
			DM::Sound_Stop("GameStage");
			DM::Sound_Play("ui", false);
			//�|�[�Y�^�X�N�\��
			auto P = Game_Pause::Object::Create(true);
		}
		//�|�[�Y��ʂ���߂邩
		else if(in.ST.down&&ge->gameState == ge->�|�[�Y)
		{
			DM::Sound_Play("GameStage", true);
		}
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