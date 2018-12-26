//-------------------------------------------------------------------
//�J������`
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game_Camera.h"

namespace  Game_Camera
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
		this->render2D_Priority[1] = 0.5f;
		this->hitBase = ML::Box2D(5, 5, 290, 320);
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		if (ge->gameState == ge->�|�[�Y)
		{
			return;
		}
		//�J�����ړ��Ɖ�ʂƂ̔���
		CameraMove();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, 300, 320);
		draw.Offset(this->pos);
		ML::Box2D src(0, 0, 300, 250);
		DG::Image_Draw("CameraImg", draw, src);
	}
	//�J�����ƃv���C���A�M�~�b�N�Ƃ̓����蔻��
	bool Object::CmCheckHit(const ML::Box2D&h_)
	{
		RECT r = { h_.x,h_.y,h_.x + h_.w,h_.y + h_.h };
		ML::Box2D hit = this->hitBase.OffsetCopy(this->pos);
		//��`���J�����O�ɏo�Ă�����ۂߍ��݂��s��
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

	//�J�����ړ��Ɖ�ʂƂ̔���
	void Object::CameraMove()
	{
		auto in = ge->in1->GetState();
		POINT est{ 0,0 };
		if (in.RStick.BL.on) { est.x -= 4; }
		if (in.RStick.BR.on) { est.x += 4; }
		if (in.RStick.BU.on) { est.y -= 4; }
		if (in.RStick.BD.on) { est.y += 4; }
		ML::Box2D screenSize(0, 0, ge->screenWidth, ge->screenHeight);
		ML::Box2D thisHitSize = this->hitBase.OffsetCopy(this->pos.x + est.x, this->pos.y + est.y);
		if (screenSize.x > thisHitSize.x)
		{
			return;
		}
		if (screenSize.w < thisHitSize.x + thisHitSize.w)
		{
			return;
		}
		if (screenSize.y > thisHitSize.y)
		{
			return;
		}
		if (screenSize.h < thisHitSize.y + thisHitSize.h)
		{
			return;
		}
		this->pos.x += est.x;
		this->pos.y += est.y;
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