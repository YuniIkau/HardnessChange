//-------------------------------------------------------------------
//�^�C�g�����j���[�\��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title_Menu.h"
#include  "Task_Game_Manager.h"
#include  "Task_StageLogo.h"
#include  "Task_Title_Manager.h"

namespace  Title_Menu
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
		drawposX[0] = 0;
		for (int i = 0; i < 3; i++)
		{
			drawposX[i] = -40;
			sa[i] = 0;
		}
		this->uiState = �ړ���;
		this->hitBase = ML::Box2D(0, 0, 300, 80);
		this->hitBase.Offset(0, 240 - 100);
		this->NextFlag = 0;
		this->desuCnt = 0;
		this->corsorNum = ����;
		this->cursor[0] = { 90 };
		this->cursor[1] = { 390 };
		this->cursor[2] = { 690 };
		this->MenuPosX[0] = { 100 };
		this->MenuPosX[1] = { 400 };
		this->MenuPosX[2] = { 700 };
		this->described = false;

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�^�C�g��");
		//�I��
		if (this->corsorNum == �I��)
		{
			ge->KillAll_G("���\�[�X�}�l�[�W���[");
			ge->quitRequest = true;
		}
		if (!ge->QuitFlag() && this->nextTaskCreate) 
		{
			//�������p���^�X�N�̐���
			auto nextTask = FrontStageLogo::Object::Create(true);
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto in = ge->in1->GetState();
		auto manager = ge->GetTask_One_GN<Title_Manager::Object>("�^�C�g��", "����");

		if (manager->titleState == manager->�v���[�Y���S)
		{
			this->alphaCnt = ColorCntMove(this->alphaCnt, 0.01f, 0.8f, 0);
			if (in.B1.down)
			{
				manager->titleState = manager->���j���[;
				//�Đ��J�n
				DM::Sound_Play("ui", false);
			}
			return;
		}
		if (manager->titleState == manager->���j���[)
		{
			
			EasingMoveX();	
			SelectAction();
			SelectMenu();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		auto manager = ge->GetTask_One_GN<Title_Manager::Object>("�^�C�g��", "����");
		if (manager->titleState == manager->�v���[�Y���S)
		{
				ML::Box2D drawP(ge->screenWidth / 2 - 150, 800, 300, 60);
				ML::Box2D srcP(0, 200, 700, 100);
				DG::Image_Draw("TitleFontColorImg", drawP, srcP, ML::Color(this->alphaCnt, 1, 1, 1));
			return;
		}
		if (manager->titleState == manager->���j���[)
		{
			ML::Box2D draw(this->cursor[this->corsorNum], 480, 300, 300);
			ML::Box2D src(0, 0, 300, 250);
			DG::Image_Draw("CameraImg", draw, src);
			for (int i = 0; i < 3; i++)
			{
				ML::Box2D drawMenu((int)this->drawposX[i], 600, 400, 80);
				ML::Box2D srcMenu(0, 0, 700, 100);
				//�I�����Ă��镶�����J���[�ɂ���
				if (uiState == �I�� && this->corsorNum == i)
				{
					srcMenu.Offset(0, 200 + ((i + 1) * 100));
					DG::Image_Draw("TitleFontColorImg", drawMenu, srcMenu);
				}
				else
				{
					srcMenu.Offset(0, 200 + ((i + 1) * 100));
					DG::Image_Draw("TitleFontMonoImg", drawMenu, srcMenu);
				}
				if (this->corsorNum == ���� && this->uiState == this->����)
				{
					//��������`��
					ML::Box2D drawSe(100, 100, 900 - 56, 900 - 56);
					ML::Box2D srcSe(0, 0, 800, 600);
					DG::Image_Draw("SetumeiImg", drawSe, srcSe, ML::Color(1, 1, 1, 1));
				}
			}
		}
	}
	//�ɋ}�����ĖړI�n�֋߂Â�
	void Object::EasingMoveX()
	{
		if (this->uiState == �ړ���)
		{
			for (int i = 0; i < 3; i++)
			{
				if (this->drawposX[i] < this->MenuPosX[i] + 10 && this->drawposX[i]>this->MenuPosX[i] - 10)
				{
					this->uiState = �I��;
				}
			}
		}
		for (int i = 0; i < 3; i++)
		{
			sa[i] = this->MenuPosX[i] - this->drawposX[i];
			this->drawposX[i] += sa[i] * 0.08f;
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