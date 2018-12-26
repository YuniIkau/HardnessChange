//----------------------------------------------------
//スクリーン汎用スーパークラス
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
		this->uiState = 選択;
		this->described = false;
	}
	auto in = ge->in1->GetState();
	//ステージセレクトかゲーム終了に進む
	if (this->uiState == 決定 && this->corsorNum != 説明)
	{
		if (CntDelayMove(this->desuCnt, 1, 50))
		{
			this->Kill();
		}
	}
	//操作説明表示
	else if (this->uiState == 決定 && this->corsorNum == 説明)
	{
		//選択に戻る
		if (in.B1.down)
		{
			DM::Sound_Play("ui", false);
			this->corsorNum = 説明;
			this->described = true;
		}
	}
}

void ScreenBase::SelectMenu()
{
	auto in = ge->in1->GetState();
	if (this->uiState == 移動終わり || this->uiState == 選択)
	{
		if (in.LStick.BL.down&&this->corsorNum > 次へ && this->uiState != 決定)
		{
			this->corsorNum--;
			this->uiState = 選択;
			DM::Sound_Play("ui", false);
		}
		if (in.LStick.BR.down&&this->corsorNum < 終了 && this->uiState != 決定)
		{
			(int)this->corsorNum++;
			this->uiState = 選択;
			DM::Sound_Play("ui", false);
		}
		if (in.B1.down&&this->uiState == 選択)
		{
			this->uiState = 決定;
			DM::Sound_Play("ui", false);
		}
	}
}