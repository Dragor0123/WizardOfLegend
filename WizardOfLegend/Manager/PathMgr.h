#pragma once

#ifndef __PATHMGR_H__
#define __PATHMGR_H__

#include "Singleton.h"
class CPathMgr : public Singleton<CPathMgr>
{
	friend class Singleton<CPathMgr>;

public:
	bool Initialize();
	bool Create_Path(const string& _strKey, const wchar_t* _pPath,
		const string& _strBaseKey = ROOT_PATH);
	const wchar_t* Find_Path(const string& strKey);
	void Release();

private:
	CPathMgr();
	~CPathMgr();
	CPathMgr(const CPathMgr&) = delete;
	CPathMgr& operator=(const CPathMgr&) = delete;

private:
	unordered_map<string, wstring> m_mapPath;
};

#endif