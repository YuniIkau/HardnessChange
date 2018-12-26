//----------------------------------------------------
//キャラクタ汎用スーパークラス
//----------------------------------------------------
#include "BChara.h"
#include "MyPG.h"
#include "Task_Game_MapChip.h"
#include "Task_Game_Camera.h"
#include "Task_Game_Player.h"
#include "Task_Game_TransmissionBlock.h"

//----------------------------------------------------
//めり込まない移動処理
void BChara::MapChipCheckMove(ML::Vec2& e_)
{
	//マップが存在するか調べてからアクセス
	auto	map = ge->GetTask_One_GN<Game_MapChip::Object>("ゲーム", "マップチップ");
	//一つのタスクからデータを読み込み複数の透過ブロックを処理するため検出はGroupではなくOne
	auto tB = ge->GetTask_One_GN<Game_TransmissionBlock::Object>("ゲーム", "透過ブロック");
	//マップがなければ判定しない(できない)
	if (nullptr == map) { return; }
	ML::Vec2 moveAdd;
	//横軸に対する移動
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
			this->pos.x = preX;	//移動をキャンセル
			break;
		}
		for (int i = 0; i < tB->sizeG; i++)
		{
			if (true == tB->CheckHit(hit, i) && tB->Gflag[i] == false)
			{
				this->pos.x = preX;	//移動をキャンセル
				break;
			}
		}
	}
	//縦軸に対する移動
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
			this->pos.y = preY;	//移動をキャンセル
			break;
		}
		for (int i = 0; i < tB->sizeG; i++)
		{
			if (true == tB->CheckHit(hit, i) && tB->Gflag[i] == false)
			{
				this->pos.y = preY;	//移動をキャンセル
				break;
			}
		}
	}
}

//----------------------------------------------------
//足元接触判定
bool BChara::CheckFoot()
{
	//当たり判定を基にして足元矩形を生成
	ML::Box2D foot(this->hitBase.x,
		this->hitBase.y + this->hitBase.h,
		this->hitBase.w,
		1);
	foot.Offset(this->pos);

	auto	map = ge->GetTask_One_GN<Game_MapChip::Object>("ゲーム", "マップチップ");
	if (nullptr == map) { return false; }//マップがなければ判定しない(できない)
	//マップと接触判定
	//ギミックと接触判定
	auto tG = ge->GetTask_One_GN<Game_TransmissionBlock::Object>("ゲーム", "透過ブロック");//////////////////////////////////////////////////////////////
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
//頭上判定
bool BChara::CheckHead()
{
	//当たり判定を基にして頭上矩形を生成
	ML::Box2D head(this->hitBase.x,
		this->hitBase.y-1,
		this->hitBase.w,
		1);
	head.Offset(this->pos);

	auto	map = ge->GetTask_One_GN<Game_MapChip::Object>("ゲーム", "マップチップ");
	if (nullptr == map) { return false; }//マップがなければ判定しない(できない)
	//マップと接触判定
	//ギミックと接触判定
	auto tB = ge->GetTask_One_GN<Game_TransmissionBlock::Object>("ゲーム", "透過ブロック");////////////////////////////////////////////////////////////////
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
//透過ギミックの読み込み
bool	BChara::GimmickLoad(const	string& fpath_)
{
	//ファイルを開く(読み込み)
	ifstream fin(fpath_);
	//読み込み失敗
	if (!fin) { return false; }	

	//チップファイル名の読み込みと、画像のロード
	string	chipFileName, chipFilePath;
	fin >> chipFileName;
	chipFilePath = "./data/ギミック/" + chipFileName;
	DG::Image_Create(this->GimmickName, chipFilePath);

	//マップ配列サイズの読み込み
	fin >> this->sizeG;
	//マップ配列データの読み込み
	for (int i = 0; i < this->sizeG; i++)
	{
			fin >> this->Gimmickpos[i].x >> this->Gimmickpos[i].y;
			this->Gimmickpos[i].x = this->Gimmickpos[i].x * ge->kukeisize;
			this->Gimmickpos[i].y = this->Gimmickpos[i].y * ge->kukeisize;
	}
	fin.close();
	return true;
}