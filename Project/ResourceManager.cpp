//-------------------------------------------------------------------
//���\�[�X�̊Ǘ�
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "ResourceManager.h"

namespace  ResourceManager
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
		//���p�x�Ŏg�p���郊�\�[�X�͍ŏ�����g�p�����Ɠǂݍ���ł���
		DG::Image_Create("CameraImg", "./data/�L�����N�^�[/�J������`.png");
		//SE�̓ǂݍ���
		DM::Sound_CreateSE("ui", "./data/�T�E���h/UI.wav");
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		DG::Image_Erase("CameraImg");
		ge->KillAll_G("���\�[�X�}�l�[�W���[");
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
	//�e�V�[���̃��\�[�X�̓ǂݍ���
	void Object::Create(SceneState sceneState)
	{
		switch (sceneState)
		{
		case Title:
			DG::Image_Create("TitleBackImg", "./data/�w�i/�^�C�g���w�i.png");
			DG::Image_Create("TitleFontColorImg", "./data/����/�^�C�g��.png");
			DG::Image_Create("TitleFontMonoImg", "./data/����/�^�C�g�����m�N��.png");
			DG::Image_Create("SetumeiImg", "./data/�C���[�W/�C���[�W.png");
			//�Đ����Ԃ��������m��Stream��I���i���BGM�j
			DM::Sound_CreateStream("Title", "./data/�T�E���h/�^�C�g��.wav");
			break;
		case FrontStageLogo:
			DG::Image_Create("FLogoBackImg", "./data/�w�i/�X�e�[�W�Z���N�g.png");
			DM::Sound_CreateStream("Select", "./data/�T�E���h/�Z���N�g.wav");
			break;
		case Game:
			if (ge->StageNum < 4)
			{
				DG::Image_Create("BackImg", "./data/�w�i/�w�i1.png");
				DG::Image_Create("BackMonoImg", "./data/�w�i/�w�i1���m�N��.png");
			}
			else
			{
				DG::Image_Create("BackImg", "./data/�w�i/�w�i2.png");
				DG::Image_Create("BackMonoImg", "./data/�w�i/�w�i2���m�N��.png");
			}
			DG::Image_Create("MapChipImg", "./data/�}�b�v/MapChip0.png");
			DG::Image_Create("BackGrayImg", "./data/�w�i/��.png");
			DG::Image_Create("TransmissionBlock", "./data/�M�~�b�N/����.png");
			DG::Image_Create("PauseImg", "./data/����/�|�[�Y.png");
			DG::Image_Create("PauseMonoImg", "./data/����/�|�[�Y���m�N��.png");
			DG::Image_Create("PlayerImg", "./data/�L�����N�^�[/Player.png");
			DG::Image_Create("SetumeiImg", "./data/�C���[�W/�C���[�W.png");
			DM::Sound_CreateStream("GameStage", "./data/�T�E���h/bgm.wav");
			DM::Sound_CreateSE("Desu", "./data/�T�E���h/���S.wav");
			break;
		case RearStageLogo:
			DG::Image_Create("RearLogoClearImg", "./data/�w�i/�Q�[���N���A.png");
			DG::Image_Create("RearLogoOverImg", "./data/�w�i/�Q�[���I�[�o�[.png");
			DG::Image_Create("RearMenuImg", "./data/����/�I�����S.png");
			DG::Image_Create("RearMenuMonoImg", "./data/����/�I�����S���m�N��.png");
			DG::Image_Create("StageLogoImg", "./data/����/���U���g.png");
			DM::Sound_CreateStream("GameOver", "./data/�T�E���h/�Q�[���I�[�o�[.wav");
			break;
		default:
			break;
		}
	}
	//�e�V�[���̃��\�[�X�̉��
	void Object::Erase(SceneState sceneState)
	{
		switch (sceneState)
		{
		case Title:
			DG::Image_Erase("TitleBackImg");
			DG::Image_Erase("TitleFontColorImg");
			DG::Image_Erase("TitleFontMonoImg");
			DG::Image_Erase("SetumeiImg");
			DM::Sound_Erase("Title");
			break;
		case FrontStageLogo:
			DG::Image_Erase("FLogoBackImg");
			DM::Sound_Erase("Select");
			break;
		case Game:
			if (ge->StageNum < 4)
			{
				DG::Image_Erase("BackImg");
				DG::Image_Erase("BackMonoImg");
			}
			else
			{
				DG::Image_Erase("BackImg");
				DG::Image_Erase("BackMonoImg");
			}
			DG::Image_Erase("MapChipImg");
			DG::Image_Erase("BackGrayImg");
			DG::Image_Erase("TransmissionBlock");
			DG::Image_Erase("PauseImg");
			DG::Image_Erase("PauseMonoImg");
			DG::Image_Erase("PlayerImg");
			DG::Image_Erase("SetumeiImg");
			DM::Sound_Erase("GameStage");
			DM::Sound_Erase("Desu");
			break;
		case RearStageLogo:
			DG::Image_Erase("RearLogoClearImg");
			DG::Image_Erase("RearLogoOverImg");
			DG::Image_Erase("RearMenuImg");
			DG::Image_Erase("RearMenuMonoImg");
			DG::Image_Erase("StageLogoImg");
			DM::Sound_Erase("GameOver");
		default:
			break;
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