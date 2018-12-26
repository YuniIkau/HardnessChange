//-------------------------------------------------------------------
//���߃}�X�p�M�~�b�N
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game_TransmissionBlock.h"
#include  "Task_Game_Camera.h"
#include  "Task_Game_Player.h"

namespace  Game_TransmissionBlock
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
		for (int i = 0; i <this->sizeG; i++)
		{
			this->Gflag[i] = true;
			this->Gimmickpos[i] = ML::Vec2(0, 0);
			this->GimmickhitBase[i] = ML::Box2D(0, 0, ge->kukeisize, ge->kukeisize);
		}
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
		for (int i = 0; i < this->sizeG; i++)
		{
			this->GimmickhitBase[i].OffsetCopy((int)this->Gimmickpos[i].x, (int)this->Gimmickpos[i].y);
			auto cm = ge->GetTask_One_GN<Game_Camera::Object>("�Q�[��", "�J����");
			//�J�����ɓ��߃u���b�N�������Ă���Δ��������悤�ɂ���
			if (cm->Object::CmCheckHit(this->GimmickhitBase[i]) != true)
			{
				this->Gflag[i] = true;
			}
			else
			{
				this->Gflag[i] = false;
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		for (int i = 0; i < this->sizeG; i++)
		{
			this->GimmickhitBase[i] = ML::Box2D((int)this->Gimmickpos[i].x, (int)this->Gimmickpos[i].y, ge->kukeisize, ge->kukeisize);
			ML::Box2D draw = this->GimmickhitBase[i];
			ML::Box2D src(0, 0, 256, 256);
			this->alpha[i] = ML::Color(1, 1, 1, 1);
			//�J�����ɓ����Ă��Ȃ��Ƃ���𔼓����ɂ���
			if (this->Gflag[i] == true)
			{
				this->alpha[i] = ML::Color(0.2f, 1, 1, 1);
			}
			draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
			DG::Image_Draw("TransmissionBlock", draw, src, this->alpha[i]);
		}
	}
	//�����蔻��
	bool Object::CheckHit(const	ML::Box2D&h_, int i)
	{
		this->GimmickhitBase[i] = ML::Box2D((int)this->Gimmickpos[i].x, (int)this->Gimmickpos[i].y, ge->kukeisize, ge->kukeisize);
		RECT r = { h_.x,h_.y,h_.x + h_.w,h_.y + h_.h };
		ML::Box2D hit = this->GimmickhitBase[i];
		RECT c = {
			hit.x,
			hit.y,
			hit.x + hit.w,
			hit.y + hit.h,
		};
		if (r.left < c.right)
		{
			if (r.right > c.left)
			{
				if (r.top < c.bottom)
				{
					if (r.bottom > c.top)
					{
						return true;
					}
				}
			}
		}
		return false;
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