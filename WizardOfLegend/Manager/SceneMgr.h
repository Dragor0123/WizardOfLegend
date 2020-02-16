#pragma once

#ifndef __SCENEMGR_H__
#define __SCENEMGR_H__

#include "Singleton.h"

class CSceneMgr : public Singleton<CSceneMgr>
{
	friend class Singleton<CSceneMgr>;

private:
	CSceneMgr();
	~CSceneMgr();
	CSceneMgr(const CSceneMgr&) = delete;
	CSceneMgr& operator=(const CSceneMgr&) = delete;
};

#endif
