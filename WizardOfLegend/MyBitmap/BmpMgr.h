#pragma once

#ifndef __BMPMGR_H__
#define __BMPMGR_H__

#include "Singleton.h"

class CMyBitmap;

class CBmpMgr : public Singleton<CBmpMgr>
{
	friend class Singleton<CBmpMgr>;

public:
	bool Initialize();
	bool Insert_Bmp(const TCHAR* _pFilePath, const string& _strImageKey);
	CMyBitmap* Find_MyBitmap(const string& _strImageKey);
	HDC Find_Image(const string& _strImageKey);
	void Release();

private:
	CBmpMgr();
	virtual ~CBmpMgr();
	CBmpMgr(const CBmpMgr&) = delete;
	CBmpMgr& operator=(const CBmpMgr&) = delete;

private:
	unordered_map<string, CMyBitmap*> m_mapStrBmp;
};

#endif

