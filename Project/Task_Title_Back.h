#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//^Cg
//-------------------------------------------------------------------
#include "ScreenBase.h"

namespace  Title_Back
{
	//^XNÉèÄéO[v¼ÆÅL¼
	const  string  defGroupName("^Cg");	//O[v¼
	const  string  defName("wi");	//^XN¼
	//-------------------------------------------------------------------
	class  Resource
	{
		bool  Initialize();
		bool  Finalize();
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
	};
	//-------------------------------------------------------------------
	class  Object : public  ScreenBase
	{
	//ÏXsÂ
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//¶¬û øÍtrueÅ^XNVXeÖ©®o^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//uú»v^XN¶¬ÉPñ¾¯s¤
		void  UpDate();		//uÀsvPt[És¤
		void  Render2D_AF();	//u2D`ævPt[És¤
		bool  Finalize();		//uI¹v^XNÁÅÉPñ¾¯s¤
	//ÏXÂ
	public:
	};
}