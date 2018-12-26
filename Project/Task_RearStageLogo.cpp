//-------------------------------------------------------------------
//�X�e�[�W��̃��S
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_RearStageLogo.h"
#include  "Task_RearStageLogoImg.h"
#include  "Task_Title_Manager.h"
#include  "Task_Game_Manager.h"

#include  "ResourceManager.h"

namespace  RearStageLogo
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		//���\�[�X�}�l�[�W���[�̎擾
		auto RM = ge->GetTask_One_GN<ResourceManager::Object>("���\�[�X�}�l�[�W���[", "���\�[�X�}�l�[�W���[");
		//�^�C�g���V�[���Ŏg�p���郊�\�[�X�̓ǂݍ���
		RM->Create(RM->RearStageLogo);
		//BGM�̍Đ��J�n
		DM::Sound_Play("GameOver", true);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		//���\�[�X�}�l�[�W���[�̎擾
		auto RM = ge->GetTask_One_GN<ResourceManager::Object>("���\�[�X�}�l�[�W���[", "���\�[�X�}�l�[�W���[");
		RM->Erase(RM->RearStageLogo);
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
		this->render2D_Priority[1] = 1.0f;
		this->alphaCnt = 0;
		this->desuCnt = 0;
		this->corsorNum = 0;
		this->uiState = �I��;
		this->hitBase = ML::Box2D(0, 0, 200, 40);
		this->hitBase.Offset(0, 0);
		ML::Box2D srcMenu(0, 0, 0, 0);
		if (ge->gameState == ge->�I�[���N���A)
		{
			this->drawPosx[0] = { (LONG)(200) };
			this->drawPosx[1] = { (LONG)(600) };
		}
		else if (ge->gameState == ge->�N���A)
		{
			this->drawPosx[0] = { (LONG)(30) };
			this->drawPosx[1] = { (LONG)(380) };
			this->drawPosx[2] = { (LONG)(730) };
			//�N���A���͂����Ɏ��̃X�e�[�W�֍s����悤��Next�ɃJ�[�\����u��
			this->corsorNum = 2;
		}
		else
		{
			this->drawPosx[0] = { (LONG)(200) };
			this->drawPosx[1] = { (LONG)(600) };
		}
		//���^�X�N�̐���
		auto rsl = RearStageLogoImg::Object::Create(true);
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("LogoRear");

		if (!ge->QuitFlag() && this->nextTaskCreate) 
		{
			//�������p���^�X�N�̐���
			if (this->corsorNum == 2)
			{
				//�N���A�����̃X�e�[�W
				ge->StageNum++;
				auto nextTask = Game_Manager::Object::Create(true);
			}
			//������x�����X�e�[�W�֍s��
			else if (this->corsorNum == 0)
			{
				//�����X�e�[�W
				auto nextTask = Game_Manager::Object::Create(true);
			}
			else
			{
				//�^�C�g��
				auto	nextTask = Title_Manager::Object::Create(true);
			}
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto	in = ge->in1->GetState();
		//��ʕ\������
		if (ge->gameState == ge->�N���A)
		{
			FadeInOut(this->alphaCnt, 0.04f, 1, true);
		}
		else
		{
			FadeInOut(this->alphaCnt, 0.01f, 1, true);
		}

		if (this->uiState == ����)
		{
			if (CntDelayMove(this->desuCnt, 1, 50))
			{
				this->Kill();
			}
		}

		SelectMenu();

		if (in.B1.down&&this->uiState == �I��)
		{
			this->uiState = ����;
			DM::Sound_Play("ui", false);
		}
		if (this->uiState == ����)
		{
			if (CntDelayMove(this->desuCnt, 1, 50))
			{
				this->Kill();
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�Ō�̃X�e�[�W���N���A�����Ȃ�
		if (ge->gameState == ge->�I�[���N���A)
		{
			ML::Box2D draw(0, 0, 1056, 1056);
			ML::Box2D src(0, 0, 800, 600);
			DG::Image_Draw("RearLogoClearImg", draw, src, ML::Color(this->alphaCnt, 1, 1, 1));
			for (int i = 0; i < 2; i++)
			{
				ML::Box2D drawC(this->drawPosx[i], 800, 300, 60);
				ML::Box2D srcC(0, ((i + 1) * 200) - 200, 700, 200);
				DG::Image_Draw("RearMenuImg", drawC, srcC);
			}
		}
		//�Q�[���N���A�Ȃ�
		else if (ge->gameState == ge->�N���A)
		{
			ML::Box2D draw(0, 0, 1056, 1056);
			ML::Box2D src(0, 0, 800, 600);
			DG::Image_Draw("RearLogoClearImg", draw, src, ML::Color(this->alphaCnt, 1, 1, 1));
			for (int i = 0; i < 3; i++)
			{
				ML::Box2D drawC(this->drawPosx[i], 800, 300, 60);
				ML::Box2D srcC(0, ((i + 1) * 200) - 200, 700, 200);
				DG::Image_Draw("RearMenuImg", drawC, srcC);
			}
		}
		//�Q�[���I�[�o�[�Ȃ�
		else
		{
			ML::Box2D draw(0, 0, 1056, 1056);
			ML::Box2D src(0, 0, 800, 600);
			DG::Image_Draw("RearLogoOverImg", draw, src, ML::Color(this->alphaCnt, 1, 1, 1));
			for (int i = 0; i < 2; i++)
			{
				ML::Box2D drawC(this->drawPosx[i], 800, 300, 60);
				ML::Box2D srcC(0, ((i + 1) * 200) - 200, 700, 200);
				DG::Image_Draw("RearMenuImg", drawC, srcC);
			}
		}
		//�J�[�\���̕`��
		ML::Box2D drawC(this->drawPosx[this->corsorNum]-20, 800-120, 340, 340);
		ML::Box2D srcC(0, 0, 300, 250);
		DG::Image_Draw("CameraImg", drawC, srcC);
	}
	void Object::SelectMenu()
	{
		auto in = ge->in1->GetState();
		if (ge->gameState == ge->�I�[���N���A)
		{
			if (in.LStick.BL.down&&this->corsorNum > 0 && this->uiState != ����)
			{
				this->corsorNum--; this->uiState = �I��;
				DM::Sound_Play("ui", false);
			}
			if (in.LStick.BR.down&&this->corsorNum < 1 && this->uiState != ����)
			{
				this->corsorNum++; this->uiState = �I��;
				DM::Sound_Play("ui", false);
			}
		}
		else if (ge->gameState == ge->�N���A)
		{
			if (in.LStick.BL.down&&this->corsorNum > 0 && this->uiState != ����)
			{
				this->corsorNum--;
				this->uiState = �I��;
				DM::Sound_Play("ui", false);
			}
			if (in.LStick.BR.down&&this->corsorNum < 2 && this->uiState != ����)
			{
				this->corsorNum++;
				this->uiState = �I��;
				DM::Sound_Play("ui", false);
			}
		}
		else
		{
			if (in.LStick.BL.down&&this->corsorNum > 0 && this->uiState != ����)
			{
				this->corsorNum--;
				this->uiState = �I��;
				DM::Sound_Play("ui", false);
			}
			if (in.LStick.BR.down&&this->corsorNum < 1 && this->uiState != ����)
			{
				this->corsorNum++;
				this->uiState = �I��;
				DM::Sound_Play("ui", false);
			}
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