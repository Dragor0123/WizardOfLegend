#pragma once

#ifndef __CCTRLOWNERMGR_H__
#define __CCTRLOWNERMGR_H__

#include "Singleton.h"

class CCtrlOwnerMgr : public Singleton<CCtrlOwnerMgr>
{
	friend class Singleton<CCtrlOwnerMgr>;

public:

public:
	bool Initialize();
	void Release();

public:
	bool Is_Key_Available(KEYOWN::KEYSWITCH _idx) const { return m_bKeyOnOffArr[_idx]; }

	void Inventory_On();
	void Inventory_Off();
	void InGameMenu_On();
	void InGameMenu_Off(KEYOWN::KEYSWITCH _idx);
	void TitleMenu_On();
	void TitleMenu_Off();
	void GameObject_On();
	void GameObject_Off();
	void WardInven_Off();
	void WardInven_On();

private:
	void Set_Off(int i) { m_bKeyOnOffArr[i] = false; }
	void Set_On(int i) { m_bKeyOnOffArr[i] = true; }
	void Set_All_Off();

private:
	CCtrlOwnerMgr();
	~CCtrlOwnerMgr();
	CCtrlOwnerMgr(const CCtrlOwnerMgr&) = delete;
	CCtrlOwnerMgr& operator=(const CCtrlOwnerMgr&) = delete;
	bool		m_bKeyOnOffArr[KEYOWN::KS_END];
};

#endif