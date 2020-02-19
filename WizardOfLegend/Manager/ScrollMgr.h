#pragma once

#ifndef __SCROLLMGR_H__
#define __SCROLLMGR_H__

#include "Singleton.h"

class CScrollMgr : public Singleton<CScrollMgr>
{
	friend class Singleton<CScrollMgr>;
public:
	inline float Get_ScrollX() const { return m_fScrollX; }
	inline float Get_ScrollY() const { return m_fScrollY; }

	inline void Set_ScrollX(float _x) { m_fScrollX += _x; }
	inline void Set_ScrollY(float _y) { m_fScrollY += _y; }

	bool Initialize() { return true; }

public:
	void Scroll_Lock();

private:
	CScrollMgr();
	~CScrollMgr();
	CScrollMgr(const CScrollMgr&) = delete;
	CScrollMgr& operator=(const CScrollMgr&) = delete;

private:
	float	m_fScrollX;
	float	m_fScrollY;
};

#endif 