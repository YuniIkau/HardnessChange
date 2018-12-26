//----------------------------------------------------
//�X�N���[���ėp�X�[�p�[�N���X
//----------------------------------------------------
#include "ScreenBase.h"
#include "MyPG.h"
//----------------------------------------------------
void ScreenBase::CntLoopMove(int cnt, int add, int max, int min)
{
	cnt += add;
	if (cnt > max)
	{
		cnt = min;
	}
}
void ScreenBase::CntLoopMove(int cnt, int add, int max)
{
	cnt += add;
	if (cnt > max)
	{
		cnt = 0;
	}
}
bool ScreenBase::CntDelayMove(int& cnt, int add, int max)
{
	cnt += add;
	if (cnt > max)
	{
		cnt = 0;
		return true;
	}
	return false;
}

bool ScreenBase::FadeInOut(float& cnt, float add, float limit, bool inOut)
{
	if (inOut == true)
	{
		cnt += add;
		if (cnt > limit)
		{
			cnt = limit;
			return true;
		}
	}
	else if (inOut == false)
	{
		cnt -= add;
		if (cnt < limit)
		{
			cnt = limit;
			return true;
		}
	}
	return false;
}

float ScreenBase::ColorCntMove(float cnt, float add, float max, float min)
{
	if (cnt < min)
	{
		this->alphaFlag = true;
	}
	else if (cnt > max)
	{
		this->alphaFlag = false;
	}
	cnt = this->alphaFlag ? cnt + add : cnt - add;
	return cnt;
}

void ScreenBase::SelectAction()
{
	if (this->described)
	{
		this->uiState = �I��;
		this->described = false;
	}
	auto in = ge->in1->GetState();
	//�X�e�[�W�Z���N�g���Q�[���I���ɐi��
	if (this->uiState == ���� && this->corsorNum != ����)
	{
		if (CntDelayMove(this->desuCnt, 1, 50))
		{
			this->Kill();
		}
	}
	//��������\��
	else if (this->uiState == ���� && this->corsorNum == ����)
	{
		//�I���ɖ߂�
		if (in.B1.down)
		{
			DM::Sound_Play("ui", false);
			this->corsorNum = ����;
			this->described = true;
		}
	}
}

void ScreenBase::SelectMenu()
{
	auto in = ge->in1->GetState();
	if (this->uiState == �ړ��I��� || this->uiState == �I��)
	{
		if (in.LStick.BL.down&&this->corsorNum > ���� && this->uiState != ����)
		{
			this->corsorNum--;
			this->uiState = �I��;
			DM::Sound_Play("ui", false);
		}
		if (in.LStick.BR.down&&this->corsorNum < �I�� && this->uiState != ����)
		{
			(int)this->corsorNum++;
			this->uiState = �I��;
			DM::Sound_Play("ui", false);
		}
		if (in.B1.down&&this->uiState == �I��)
		{
			this->uiState = ����;
			DM::Sound_Play("ui", false);
		}
	}
}