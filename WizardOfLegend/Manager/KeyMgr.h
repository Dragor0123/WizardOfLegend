#pragma once

#ifndef __KEYMGR_H__
#define __KEYMGR_H__

#include "Singleton.h"
#define VK_MAX 0xFF

class CKeyMgr : public Singleton<CKeyMgr>
{
	friend class Singleton<CKeyMgr>;

public:
	static bool Key_Pressing(BYTE _key);
	static bool Key_Down(BYTE _key);
	static bool Key_Up(BYTE _key);
	void Key_Update();

private:
	CKeyMgr();
	~CKeyMgr();
	CKeyMgr(const CKeyMgr&) = delete;
	CKeyMgr& operator=(const CKeyMgr&) = delete;

private:
	static bool m_bKeyState[VK_MAX];
};

#define KEY_DOWN(key) CKeyMgr::Get_Instance()->Key_Down(key)
#define KEY_UP(key) CKeyMgr::Get_Instance()->Key_Up(key)
#define KEY_PRESSING(key) CKeyMgr::Get_Instance()->Key_Down(key)


#endif