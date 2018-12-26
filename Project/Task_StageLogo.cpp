//-------------------------------------------------------------------
//�X�e�[�W�O�̃��S
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_StageLogo.h"
#include  "Task_Game_Manager.h"

#include  "ResourceManager.h"

namespace  FrontStageLogo
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		//���\�[�X�}�l�[�W���[�̎擾
		auto RM = ge->GetTask_One_GN<ResourceManager::Object>("���\�[�X�}�l�[�W���[", "���\�[�X�}�l�[�W���[");
		//�^�C�g���V�[���Ŏg�p���郊�\�[�X�̓ǂݍ���
		RM->Create(RM->FrontStageLogo);
		//BGM�̍Đ��J�n
		DM::Sound_Play("Select", true);//�Đ��J�n
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		auto RM = ge->GetTask_One_GN<ResourceManager::Object>("���\�[�X�}�l�[�W���[", "���\�[�X�}�l�[�W���[");
		RM->Erase(RM->FrontStageLogo);
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
		this->desuCnt = 0;
		
		this->alphaCnt = 0;
		this->corsorNum = 1;
		this->xbai = (float)ge->screenWidth / 800.0f;
		this->ybai = (float)ge->screenHeight / 600.0f;
		//�������W
		this->cursor[0] = { -500,-500 };
		this->cursor[1] = { (LONG)(15 * this->xbai) ,(LONG)(139 * this->ybai) };
		this->cursor[2] = { (LONG)(208 * this->xbai) ,(LONG)(221 * this->ybai) };
		this->cursor[3] = { (LONG)(426 * this->xbai) ,(LONG)(170 * this->ybai) };
		this->cursor[4] = { (LONG)(634 * this->xbai) ,(LONG)(161 * this->ybai) };
		this->cursor[5] = { (LONG)(11 * this->xbai) ,(LONG)(449 * this->ybai) };
		this->cursor[6] = { (LONG)(249 * this->xbai) ,(LONG)( 461* this->ybai) };
		this->cursor[7] = { (LONG)(439 * this->xbai) ,(LONG)(397 * this->ybai) };
		this->cursor[8] = { (LONG)(633 * this->xbai) ,(LONG)(426 * this->ybai) };
		//���^�X�N�̐���
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���

		ge->KillAll_G("LogoFront");
		if (!ge->QuitFlag() && this->nextTaskCreate) 
		{
			//�������p���^�X�N�̐���
			auto nextTask = Game_Manager::Object::Create(true);
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		//���߂̕\������
		if (!FadeInOut(this->alphaCnt, 0.02f, 1, true))
		{
			return;
		}
		if (this->uiState != ����)
		{
			this->uiState = �I��;
		}

		//�X�e�[�W�̑I��
		auto in = ge->in1->GetState();
		if (in.LStick.BL.down&&this->corsorNum > 1&& this->uiState != ����) 
		{
			this->corsorNum--;
			DM::Sound_Play("ui", false);
		}
		if (in.LStick.BR.down&&this->corsorNum < 8&& this->uiState != ����)
		{
			this->corsorNum++;
			DM::Sound_Play("ui", false);
		}
		if (in.B1.down&&this->uiState == �I��)
		{
			this->uiState = ����;
			DM::Sound_Play("ui", false);
		}
		if (this->uiState == ����)
		{
			if (CntDelayMove(this->desuCnt, 1, 50))
			{
				ge->StageNum = this->corsorNum;
				this->Kill();
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�w�i�̕`��
		ML::Box2D draw(0, 0, ge->screenWidth, ge->screenHeight);
		ML::Box2D src(0, 0, 800, 600);
		DG::Image_Draw("FLogoBackImg", draw, src, ML::Color(this->alphaCnt, 1, 1, 1));

		//�J�[�\���̕`��
		ML::Box2D drawC(this->cursor[this->corsorNum].x, this->cursor[this->corsorNum].y, int(153 * this->xbai), int(134 * this->ybai));
		ML::Box2D srcC(0, 0, 300, 250);
		DG::Image_Draw("CameraImg", drawC, srcC);

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