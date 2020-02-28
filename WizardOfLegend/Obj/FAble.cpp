#include "../stdafx.h"
#include "FAble.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Manager/KeyMgr.h"
#include "ObjMgr.h"
#include "FButton.h"
#include "../Manager/CtrlOwnerMgr.h"

CFAble::CFAble()
	: m_pFbutton(nullptr)
{
}

CFAble::~CFAble()
{
	Release();
}

bool CFAble::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/UI/fButton.bmp", "fButton"))
		return false;

	return true;
}

void CFAble::Key_Check(float _fdTime)
{
	if (CCtrlOwnerMgr::Get_Instance()->Is_Key_Available(KEYOWN::KS_FABLE))
	{
		if (m_pFbutton && static_cast<CFButton*>(m_pFbutton)->Is_Enable()) {
			if (KEY_DOWN('F'))
				Do_FButton_Action(_fdTime);
		}
	}
}

void CFAble::Do_FButton_Action(float _fdTime)
{
}

void CFAble::Enable_FButton()
{
	if (m_pFbutton)
		static_cast<CFButton*>(m_pFbutton)->Set_Enable(true);
}

void CFAble::Disable_FButton()
{
	if (m_pFbutton)
		static_cast<CFButton*>(m_pFbutton)->Set_Enable(false);
}

int CFAble::Update(float _fdTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CFAble::Late_Update(float _fdTime)
{
}

void CFAble::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{

}

void CFAble::Release()
{
	//SAFE_DELETE(m_pFbutton);
}
