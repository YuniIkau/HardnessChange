//-------------------------------------------------------------------
//2�����z��}�b�v
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game_MapChip.h"
#include  "Task_Game_Player.h"
#include  "Task_Game_Camera.h"

namespace  Game_MapChip
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
		this->render2D_Priority[1] = 0.8f;
		//�}�b�v�̃[���N���A
		for (int y = 0; y < 100; y++)
		{
			for (int x = 0; x < 100; x++)
			{
				this->arr[y][x] = 0;
			}
		}
		this->sizeX = 0;
		this->sizeY = 0;
		this->hitBase = ML::Box2D(0, 0, 0, 0);
		//�}�b�v�`�b�v���̏�����
		for (int c = 0; c < 32; c++)
		{
			int x = (c % 8);
			int y = (c / 8);
			this->chip[c] = ML::Box2D(x * 32, y * 32, 32, 32);
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�t�B�[���h");
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
		for (int y = 0; y <= sizeY; y++)
		{
			for (int x = 0; x <= sizeX; x++)
			{
				ML::Box2D draw(x * ge->kukeisize, y * ge->kukeisize, ge->kukeisize, ge->kukeisize);
				DG::Image_Draw("MapChipImg", draw, this->chip[this->arr[y][x]]);
			}
		}
	}
	//�}�b�v�t�@�C���ǂݍ���
	bool	Object::Load(const	string& fpath_)
	{
		//�t�@�C�����J��(�ǂݍ���)
		ifstream fin(fpath_);
		//�ǂݍ��ݎ��s���͂�����return
		if (!fin) { return false; }
		//�}�b�v�z��T�C�Y�̓ǂݍ���
		fin >> this->sizeX >> this->sizeY;
		this->hitBase = ML::Box2D(0, 0, this->sizeX * ge->kukeisize, this->sizeY * ge->kukeisize);
		//�}�b�v�z��f�[�^�̓ǂݍ���
		for (int y = 0; y < this->sizeY; y++)
		{
			for (int x = 0; x < this->sizeX; x++)
			{
				fin >> this->arr[y][x];
			}
		}
		fin.close();
		return true;
	}
	//�����蔻��
	bool Object::CheckHit(const	ML::Box2D&h_)
	{
		auto cm = ge->GetTask_One_GN<Game_Camera::Object>("�Q�[��", "�J����");
		RECT r = { h_.x,h_.y,h_.x + h_.w,h_.y + h_.h };
		//��`���}�b�v�O�ɏo�Ă�����ۂߍ��݂��s��
		RECT m = 
		{
		this->hitBase.x,
		this->hitBase.y,
		this->hitBase.x+this->hitBase.w,
		this->hitBase.y+this->hitBase.h
		};
		if (r.left < m.left) { r.left = m.left; }			//���ɔ�яo���������J�b�g
		if (r.top < m.top) { r.top = m.top; }				//��ɔ�яo���������J�b�g
		if (r.right > m.right) { r.right = m.right; }		//�E�ɔ�яo���������J�b�g
		if (r.bottom > m.bottom) { r.bottom = m.bottom; }	//���ɔ�яo���������J�b�g
		//���[�v�͈͒���
		int sx, sy, ex, ey;
		sx = r.left / ge->kukeisize;
		sy = r.top / ge->kukeisize;
		ex = (r.right - 1) / ge->kukeisize;
		ey = (r.bottom - 1) / ge->kukeisize;
		auto	in = ge->in1->GetState();
		//�͈͊O�̏�Q����T��
		for (int y = sy; y <= ey; y++)
		{
			for (int x = sx; x <= ex; x++)
			{
				//true=�N���s�� false=�N���\
				if (6 == this->arr[y][x] || 7 == this->arr[y][x]) { ge->gameState = ge->�N���A; }
				if (8 == this->arr[y][x]) { return true; }
				if (12 == this->arr[y][x] || 13 == this->arr[y][x] || 14 == this->arr[y][x])
				{
					//�X�e�B�b�N�̓|���[�x�̕ύX
					DI::deadzone = 0.99f;
					if (in.LStick.BD.on)
					{
						return false;
					}
					return true;
				}
				//�X�e�B�b�N�̓|���[�x�̕ύX
				DI::deadzone = 0.24f;
				if (16 <= this->arr[y][x] && this->arr[y][x] <= 31) { return true; }
				if (37 <= this->arr[y][x]) { return true; }
			}
		}
		return false;
	}
	void Object::AjastCameraPos()
	{
		//�J�����ƃ}�b�v�͈̔͂�p��
		RECT c =
		{
			ge->camera2D.x,
			ge->camera2D.y,
			ge->camera2D.x + ge->camera2D.w,
			ge->camera2D.y + ge->camera2D.h
		};
		RECT m =
		{
			this->hitBase.x,
			this->hitBase.y,
			this->hitBase.x + this->hitBase.w,
			this->hitBase.y + this->hitBase.h,
		};
		//�J�����̈ʒu����
		if (c.right > m.right) { ge->camera2D.x = m.right - ge->camera2D.w; }
		if (c.bottom > m.bottom) { ge->camera2D.y = m.bottom - ge->camera2D.h; }
		if (c.left < m.left) { ge->camera2D.x = m.left; }
		if (c.top < m.top) { ge->camera2D.y = m.top; }
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