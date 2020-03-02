#include "../stdafx.h"
#include "DigitMgr.h"
#include "../Obj/Digit.h"
#include "../Obj/ObjMgr.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Hittable.h"

CDigitMgr::CDigitMgr()
{
}

CDigitMgr::~CDigitMgr()
{
	Release();
}

bool CDigitMgr::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/UI/digitImg_white.bmp", "digit_white"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/UI/digitImg_green.bmp", "digit_green"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/UI/digitImg_yellow.bmp", "digit_yellow"))
		return false;

	return true;
}

void CDigitMgr::Make_Hit_Digit(int _number, CObj * _pObj, DIGIT::COLOR _eCol)
{
	if (!dynamic_cast<CHittable*>(_pObj))
		return;
	int _iDigit = 0;
	if (!m_stackDigit.empty())
		m_stackDigit.swap(stack<int>());

	int iWOffset = Digit_space::iDIG_WIDTH;
	int iHOffset = Digit_space::iDIG_WIDTH + 10;

	float fX = _pObj->Get_PosX() - Digit_space::iDIG_WIDTH;
	float fY = _pObj->Get_PosY() - (_pObj->Get_HitInfo().iCY >> 1);

	string strColor;

	switch (_eCol)
	{
	case DIGIT::DC_WHITE:
		strColor = "digit_white";
		break;
	case DIGIT::DC_YELLOW:
		strColor = "digit_yellow";
		break;
	case DIGIT::DC_GREEN:
		strColor = "digit_green";
		break;
	}

	while (_number != 0)
	{
		_iDigit = _number % 10;
		m_stackDigit.push(_iDigit);
		_number /= 10;
	}

	int iCnt = 0;
	int HitCntOffset = iHOffset * dynamic_cast<CHittable*>(_pObj)->Get_HitDigitCnt();
	while (!m_stackDigit.empty())
	{
		_iDigit = m_stackDigit.top();
		CObj* pDigit = CAbstractFactory<CDigit>::Create(fX + iWOffset * iCnt,
			fY - HitCntOffset, strColor);
		static_cast<CDigit*>(pDigit)->Set_Number(_iDigit);
		CObjMgr::Get_Instance()->Add_Object(OBJID::DIGIT_UI, pDigit);
		++iCnt;
		m_stackDigit.pop();
	}
}

void CDigitMgr::Make_NoMoveDigit(int _number, CObj * _pObj, DIGIT::COLOR _eCol)
{
	int _iDigit = 0;
	if (!m_stackDigit.empty())
		m_stackDigit.swap(stack<int>());

	int iWOffset = Digit_space::iDIG_WIDTH;
	int iHOffset = 0; //Digit_space::iDIG_WIDTH >> 1 + 10;

	float fX = _pObj->Get_PosX() - Digit_space::iDIG_WIDTH;
	float fY = _pObj->Get_PosY() - (_pObj->Get_HitInfo().iCY >> 1) - iHOffset;

	string strColor;

	switch (_eCol)
	{
	case DIGIT::DC_WHITE:
		strColor = "digit_white";
		break;
	case DIGIT::DC_YELLOW:
		strColor = "digit_yellow";
		break;
	case DIGIT::DC_GREEN:
		strColor = "digit_green";
		break;
	}

	while (_number != 0)
	{
		_iDigit = _number % 10;
		m_stackDigit.push(_iDigit);
		_number /= 10;
	}

	int iCnt = 0;
	while (!m_stackDigit.empty())
	{
		_iDigit = m_stackDigit.top();
		CObj* pDigit = CAbstractFactory<CDigit>::Create(fX + iWOffset * iCnt, fY, strColor);
		static_cast<CDigit*>(pDigit)->Set_Number(_iDigit);
		static_cast<CDigit*>(pDigit)->Set_Moving(false);
		CObjMgr::Get_Instance()->Add_Object(OBJID::DIGIT_UI, pDigit);
		++iCnt;
		m_stackDigit.pop();
	}
}

void CDigitMgr::Release()
{
	if (!m_stackDigit.empty())
		m_stackDigit.swap(stack<int>());
}
