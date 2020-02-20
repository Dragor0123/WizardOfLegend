#pragma once

#ifndef __ARCANAMGR_H__
#define __ARCANAMGR_H__

#include "Singleton.h"

class CArcanaMgr : public Singleton<CArcanaMgr>
{
	friend class Singleton<CArcanaMgr>;

private:
	CArcanaMgr();
	~CArcanaMgr();
	CArcanaMgr(const CArcanaMgr&) = delete;
	CArcanaMgr& operator=(const CArcanaMgr&) = delete;

private:
};

#endif