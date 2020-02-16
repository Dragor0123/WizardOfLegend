#pragma once

#ifndef __LINEMGR_H__
#define __LINEMGR_H__

#include "Singleton.h"

class CLine;
class CLineMgr : public Singleton<CLineMgr>
{
	friend class Singleton<CLineMgr>;

public:
	bool Initialize();
	void Render(HDC _DC);
	void Release();

	bool Collision_Line(float _x, float* _y);

private:
	void Load_Line();

private:
	CLineMgr();
	~CLineMgr();
	CLineMgr(const CLineMgr&) = delete;
	CLineMgr& operator=(const CLineMgr&) = delete;

private:
	list<CLine*>	m_listLine;
};

#endif