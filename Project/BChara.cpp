//----------------------------------------------------
//�L�����N�^�ėp�X�[�p�[�N���X
//----------------------------------------------------
#include "BChara.h"
#include "MyPG.h"
#include "Task_Game_MapChip.h"
#include "Task_Game_Camera.h"
#include "Task_Game_Player.h"
#include "Task_Game_TransmissionBlock.h"

//----------------------------------------------------
//�߂荞�܂Ȃ��ړ�����
void BChara::MapChipCheckMove(ML::Vec2& e_)
{
	//�}�b�v�����݂��邩���ׂĂ���A�N�Z�X
	auto	map = ge->GetTask_One_GN<Game_MapChip::Object>("�Q�[��", "�}�b�v�`�b�v");
	//��̃^�X�N����f�[�^��ǂݍ��ݕ����̓��߃u���b�N���������邽�ߌ��o��Group�ł͂Ȃ�One
	auto tB = ge->GetTask_One_GN<Game_TransmissionBlock::Object>("�Q�[��", "���߃u���b�N");
	//�}�b�v���Ȃ���Δ��肵�Ȃ�(�ł��Ȃ�)
	if (nullptr == map) { return; }
	ML::Vec2 moveAdd;
	//�����ɑ΂���ړ�
	while (e_.x != 0)
	{
		float preX = this->pos.x;
		if (e_.x >= 1)
		{
			this->pos.x += 1;	e_.x -= 1;
		}
		else if (e_.x <= -1)
		{
			this->pos.x -= 1;	e_.x += 1;
		}
		else
		{
			this->pos.x += e_.x;	e_.x = 0;
		}
		ML::Box2D hit = this->hitBase.OffsetCopy(this->pos);
		if (true == map->CheckHit(hit))
		{
			this->pos.x = preX;	//�ړ����L�����Z��
			break;
		}
		for (int i = 0; i < tB->sizeG; i++)
		{
			if (true == tB->CheckHit(hit, i) && tB->Gflag[i] == false)
			{
				this->pos.x = preX;	//�ړ����L�����Z��
				break;
			}
		}
	}
	//�c���ɑ΂���ړ�
	while (e_.y != 0)
	{
		float preY = this->pos.y;
		if (e_.y >= 1)
		{
			this->pos.y += 1;	e_.y -= 1;
		}
		else if (e_.y <= -1)
		{
			this->pos.y -= 1;	e_.y += 1;
		}
		else
		{
			this->pos.y += e_.y;	e_.y = 0;
		}
		ML::Box2D hit = this->hitBase.OffsetCopy(this->pos);
		if (true == map->CheckHit(hit))
		{
			this->pos.y = preY;	//�ړ����L�����Z��
			break;
		}
		for (int i = 0; i < tB->sizeG; i++)
		{
			if (true == tB->CheckHit(hit, i) && tB->Gflag[i] == false)
			{
				this->pos.y = preY;	//�ړ����L�����Z��
				break;
			}
		}
	}
}

//----------------------------------------------------
//�����ڐG����
bool BChara::CheckFoot()
{
	//�����蔻�����ɂ��đ�����`�𐶐�
	ML::Box2D foot(this->hitBase.x,
		this->hitBase.y + this->hitBase.h,
		this->hitBase.w,
		1);
	foot.Offset(this->pos);

	auto	map = ge->GetTask_One_GN<Game_MapChip::Object>("�Q�[��", "�}�b�v�`�b�v");
	if (nullptr == map) { return false; }//�}�b�v���Ȃ���Δ��肵�Ȃ�(�ł��Ȃ�)
	//�}�b�v�ƐڐG����
	//�M�~�b�N�ƐڐG����
	auto tG = ge->GetTask_One_GN<Game_TransmissionBlock::Object>("�Q�[��", "���߃u���b�N");//////////////////////////////////////////////////////////////
	for (int i = 0; i < tG->sizeG; i++)
	{
		if (tG->Gflag[i] == false)
		{
			if (map->CheckHit(foot) == true || tG->CheckHit(foot, i) == true)
			{
				return true;
			}
		}
	}
	return map->CheckHit(foot);
}
//���㔻��
bool BChara::CheckHead()
{
	//�����蔻�����ɂ��ē����`�𐶐�
	ML::Box2D head(this->hitBase.x,
		this->hitBase.y-1,
		this->hitBase.w,
		1);
	head.Offset(this->pos);

	auto	map = ge->GetTask_One_GN<Game_MapChip::Object>("�Q�[��", "�}�b�v�`�b�v");
	if (nullptr == map) { return false; }//�}�b�v���Ȃ���Δ��肵�Ȃ�(�ł��Ȃ�)
	//�}�b�v�ƐڐG����
	//�M�~�b�N�ƐڐG����
	auto tB = ge->GetTask_One_GN<Game_TransmissionBlock::Object>("�Q�[��", "���߃u���b�N");////////////////////////////////////////////////////////////////
	for (int i = 0; i < tB->sizeG; i++)
	{
		if (tB->Gflag[i] == false)
		{
			return map->CheckHit(head) + tB->CheckHit(head, i);
		}
		else
		{
			return map->CheckHit(head);
		}
	}
	return false;
}
//���߃M�~�b�N�̓ǂݍ���
bool	BChara::GimmickLoad(const	string& fpath_)
{
	//�t�@�C�����J��(�ǂݍ���)
	ifstream fin(fpath_);
	//�ǂݍ��ݎ��s
	if (!fin) { return false; }	

	//�`�b�v�t�@�C�����̓ǂݍ��݂ƁA�摜�̃��[�h
	string	chipFileName, chipFilePath;
	fin >> chipFileName;
	chipFilePath = "./data/�M�~�b�N/" + chipFileName;
	DG::Image_Create(this->GimmickName, chipFilePath);

	//�}�b�v�z��T�C�Y�̓ǂݍ���
	fin >> this->sizeG;
	//�}�b�v�z��f�[�^�̓ǂݍ���
	for (int i = 0; i < this->sizeG; i++)
	{
			fin >> this->Gimmickpos[i].x >> this->Gimmickpos[i].y;
			this->Gimmickpos[i].x = this->Gimmickpos[i].x * ge->kukeisize;
			this->Gimmickpos[i].y = this->Gimmickpos[i].y * ge->kukeisize;
	}
	fin.close();
	return true;
}