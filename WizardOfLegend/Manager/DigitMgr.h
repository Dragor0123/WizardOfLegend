#pragma once

#ifndef __DIGITMGR_H__
#define __DIGITMGR_H__

#include "Singleton.h"

class CObj;
class CDigitMgr : public Singleton<CDigitMgr>
{
	friend class Singleton<CDigitMgr>;
public:
	void Make_Hit_Digit(int _number, CObj* _pObj = nullptr, DIGIT::COLOR _eCol = DIGIT::DC_WHITE);
	void Make_NoMoveDigit(int _number, CObj* _pObj = nullptr, DIGIT::COLOR _eCol = DIGIT::DC_WHITE);

	bool Initialize();
	void Release();

private:
	CDigitMgr();
	~CDigitMgr();
	CDigitMgr(const CDigitMgr&) = delete;
	CDigitMgr& operator=(const CDigitMgr&) = delete;

private:
	stack<int> m_stackDigit;
};

#define MAKE_DIGIT(_number, _pObj, _eCol) CDigitMgr::Get_Instance()->Make_Hit_Digit(_number, _pObj, _eCol);

#endif
