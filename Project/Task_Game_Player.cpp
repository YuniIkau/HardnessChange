//-------------------------------------------------------------------
//�v���C��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game_Player.h"
#include  "Task_Game_Camera.h"

namespace  Game_Player
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
		this->hitBase = ML::Box2D(0, 0, 40, 40);
		this->headBase = ML::Box2D(0, -1, 20, 1);
		this->hitFlag = true;
		this->headhitFlag = true;
		this->fallSpeed = 0;//���݂̗����㏸���x
		this->jumpPow = -10.5;//����
		this->PreY = 0;
		ge->gameState = ge->Game;
		this->maxSpeed = 5.0f;		//�ő�ړ����x�i���j
		this->addSpeed = 1.0f;		//���s�����x�i�n�ʂ̉e���ł�����x�ł��������
		this->decSpeed = 0.2f;		//�ڒn��Ԃ̎��̑��x�����ʁi���C
		this->ang = 0;
		this->angCnt = 0;
		this->desuline = ge->kukeisize * 6;
		this->animFlag = false;
		this->desuCnt = 0;
		//���^�X�N�̐���

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
		this->animFlag = false;
		this->PreY = this->pos.y;
		if (ge->gameState == ge->�|�[�Y)
		{
			this->pos = this->pos;
			return;
		}
		if (this->playerState == ��)
		{
			this->desuCnt++;
			if (this->desuCnt > 100)
			{
				ge->gameState = ge->�Q�[���I�[�o�[;
				this->Kill();
			}
			return;
		}
		//��������
		Attenuation();
		HorizontalMove();
		CollisionCheck();
		if (this->playerState == �d��)
		{
			if (this->DesuCnt < -this->desuline && this->hitFlag == true)
			{
				DM::Sound_Play("Desu", false);
				this->playerState = ��;
			}
		}
		if (this->hitFlag == true)
		{
			DesuCnt = 0;
		}
		Jump();
		//��]����A�j���[�V����
		if (this->animFlag == true)
		{
			this->angCnt++;
			if (angCnt > 3)
			{
				angCnt = 0;
				ang++;
			}
			if (ang >= 360)
			{
				ang = 0;
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->hitBase.OffsetCopy(this->pos);
		ML::Box2D src(0, 0, 100, 100);
		ML::Vec2 genten(20, 20);
		if (this->playerState == �_�炩��)
		{
			src.Offset(100, 0);
		}
		if (this->playerState == ��  )
		{
			ML::Box2D drawCopy(0, 0, 50, 50);
			draw.w += drawCopy.w;
			draw.h += drawCopy.h;
			ML::Box2D srcCopy(300, 0, 150, 150);
			src = srcCopy;
			this->ang = 0;
			genten = ML::Vec2(20, 20);
		}
		DG::Image_Rotation("PlayerImg", (float)this->ang, genten);
		DG::Image_Draw("PlayerImg", draw, src);
	}

	//��������
	void Object::Attenuation()
	{
		if (this->moveVec.x < 0)
		{
			this->animFlag = true;
			this->moveVec.x = min(this->moveVec.x + this->decSpeed, 0);
		}
		else if (this->moveVec.x > 0)
		{
			this->animFlag = true;
			this->moveVec.x = max(this->moveVec.x - this->decSpeed, 0);
		}
	}
	//�����ړ�
	void Object::HorizontalMove()
	{
		auto in = ge->in1->GetState();
		if (in.LStick.BL.on)
		{
			this->animFlag = true;
			this->addSpeed += 0.2f;
			if (this->addSpeed > this->maxSpeed)
			{
				this->addSpeed = this->maxSpeed;
			}
			this->moveVec.x = -this->addSpeed;
		}
		if (in.LStick.BR.on)
		{
			this->animFlag = true;
			this->addSpeed += 0.2f;
			if (this->addSpeed > this->maxSpeed)
			{
				this->addSpeed = this->maxSpeed;
			}
			this->moveVec.x = this->addSpeed;
		}
		if (!(in.LStick.BR.on || in.LStick.BL.on))
		{
			this->addSpeed = 1.0f;
		}
	}
	//�e��̓����蔻��
	void Object::CollisionCheck()
	{
		auto cm = ge->GetTask_One_GN<Game_Camera::Object>("�Q�[��", "�J����");
		//�����ڐG����
		this->hitFlag = this->CheckFoot();
		//���㔻��
		this->headhitFlag = this->CheckHead();
		ML::Box2D head = this->headBase.OffsetCopy(this->pos);
		if (this->headhitFlag)
		{
			this->fallSpeed = 1;//����ł����痎��
		}
		//�߂荞�܂Ȃ��ړ�
		ML::Vec2  est = this->moveVec;
		this->MapChipCheckMove(est);
		this->hitFlag = this->CheckFoot();
		if (this->hitFlag == true)
		{
			//�ڒn���Ă����痎�����x�[��
			this->fallSpeed = 0;
		}


		ML::Box2D hit = this->hitBase.OffsetCopy(this->pos);
		if (true == cm->CmCheckHit(hit))
		{
			this->playerState = �_�炩��;
		}
		else
		{
			this->playerState = �d��;
		}
	}
	//
	void Object::Jump()
	{
		auto in = ge->in1->GetState();
		if (this->playerState == �_�炩��)
		{
			if (this->hitFlag == true)
			{
				//�W�����v
				if (in.Trigger.R2.down)
				{
					this->fallSpeed = this->jumpPow;
				}
			}
		}
		//�㉺�ړ���fallSpeed�̒l
		this->moveVec.y = this->fallSpeed;
		if (this->hitFlag == false)
		{
			this->animFlag = true;
			//�d�͉���0.27* ((g / f) / f)*m �@���R�����̂��̂Ɠ����ɂ��邽��3��������
			this->fallSpeed += ((this->gravity / this->frame) / this->frame)*this->meter * 3;
			if (this->fallSpeed > 10)
			{
				this->fallSpeed = 10;
			}
		}
		if (this->PreY < this->pos.y)
		{
			this->DesuCnt += (int)(this->PreY - this->pos.y);
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