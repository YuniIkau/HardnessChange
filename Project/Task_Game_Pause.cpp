//-------------------------------------------------------------------
//�|�[�Y���
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game_Pause.h"
#include  "Task_Title_Manager.h"
#include  "DI2018_Ver1_0.h"

namespace  Game_Pause
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
		this->corsorNum = ����;
		this->uiState = �I��;
		this->hitBase = ML::Box2D(-20, -90, 340, 240);
		for (int i = 0; i < 3; i++)
		{
			drawposY[i] = 250 + ((i + 1) * 200);
			sa[i] = 0;
		}
		this->NextFlag = 0;
		this->desuCnt = 0;
		this->described = false;
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_GN("�Q�[��", "�|�[�Y���");
		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//�������p���^�X�N�̐���
			if (this->corsorNum != �I��)
			{
				//������
				ge->gameState = ge->Game;
				DM::Sound_Play("GameStage", true);
			}
			else
			{
				//�^�C�g����
				ge->KillAll_G("�Q�[��");
				auto	nextTask = Title_Manager::Object::Create(true);
			}
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		SelectAction();
		SelectMenu();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�w�i
		ML::Box2D drawBack(0, 0, 1056, 1056);
		ML::Box2D srcBack(0, 0, 800, 600);
		DG::Image_Draw("BackGrayImg", drawBack, srcBack, ML::Color(0.4f, 1, 1, 1));
		//���S
		ML::Box2D draw(1056/2-320, 150, 640, 160);
		ML::Box2D src(0, 0, 310, 100);
		DG::Image_Draw("PauseImg", draw, src);
		for (int i = 0; i < 3; i++)
		{
			
			//���j���[
			ML::Box2D drawMenu(ge->screenWidth / 2 - 150, this->drawposY[i], 300, 60);
			ML::Box2D srcMenu(0, 0, 400, 100);
			if (uiState == �I�� && this->corsorNum == i)
			{
				srcMenu.Offset(0, (i + 1) * 100);
				DG::Image_Draw("PauseImg", drawMenu, srcMenu);
			}
			else
			{
				srcMenu.Offset(0, i * 100);
				DG::Image_Draw("PauseMonoImg", drawMenu, srcMenu);
			}
		}
		//�J�[�\��
		for (int i = 0; i < 3; i++)
		{
			if (i == this->corsorNum)
			{
				ML::Box2D drawS = this->hitBase;
				drawS.Offset(ge->screenWidth / 2 - 150, this->drawposY[i]);
				ML::Box2D srcS(0, 0, 300, 250);
				DG::Image_Draw("CameraImg", drawS, srcS);
			}
		}
		//����
		if (this->corsorNum == ���� && this->uiState == ����)
		{
			//��������`��
			ML::Box2D drawSe(100, 100, 900 - 56, 900 - 56);
			ML::Box2D srcSe(0, 0, 800, 600);
			DG::Image_Draw("SetumeiImg", drawSe, srcSe);
		}
	}

	//���j���[���c�Ō���ł���悤�ɃI�[�o�[���C�h����
	void Object::SelectAction()
	{
		if (this->described)
		{
			this->uiState = �I��;
			this->described = false;
		}
		auto	in = ge->in1->GetState();
		if (this->uiState == ���� && this->corsorNum != ����)
		{
			this->Kill();
		}
		else if (this->uiState == ���� && this->corsorNum == ����)
		{
			if (in.B1.down)
			{
				this->corsorNum = ����;
				this->described = true;
				DM::Sound_Play("ui", false);
			}
		}
	}
	//���j���[���c�őI���ł���悤�ɃI�[�o�[���C�h����
	void Object::SelectMenu()
	{
		auto in = ge->in1->GetState();
		if (in.LStick.BU.down&&this->corsorNum>����)
		{
			this->corsorNum--;
			this->uiState = �I��;
			DM::Sound_Play("ui", false);
		}
		if (in.LStick.BD.down&&this->corsorNum<�I��)
		{
			this->corsorNum++;
			this->uiState = �I��;
			DM::Sound_Play("ui", false);
		}
		if (this->uiState == �I�� && in.B1.down)
		{
			this->uiState = ����;
			DM::Sound_Play("ui", false);
		}
		if (in.ST.down)
		{
			DM::Sound_Play("ui", false);
			this->Kill();
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