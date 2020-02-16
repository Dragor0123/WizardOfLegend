#pragma once
#include "Singleton.h"

class CResourcesMgr : public Singleton<CResourcesMgr>
{
	friend class Singleton<CResourcesMgr>;

private:
	CResourcesMgr();
	~CResourcesMgr();
	CResourcesMgr(const CResourcesMgr&) = delete;
	CResourcesMgr& operator=(const CResourcesMgr&) = delete;
};

