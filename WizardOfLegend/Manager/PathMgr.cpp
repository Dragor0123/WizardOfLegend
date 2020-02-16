#include "../stdafx.h"
#include "PathMgr.h"

bool CPathMgr::Initialize()
{
	wchar_t strPath[MAX_PATH] = {};

	GetModuleFileName(NULL, strPath, MAX_PATH);

	for (int i = lstrlen(strPath) - 1; i >= 0; --i)
	{
		if (strPath[i] == '/' || strPath[i] == '\\')
		{
			// 제거해야할 문자 개수 = (전체길이) - (현재 인덱스  + 1)
			memset(strPath + (i + 1), 0, sizeof(wchar_t) * (MAX_PATH - (i + 1)));
			break;
		}
	}

	m_mapPath.emplace(ROOT_PATH, strPath);

	// Texture 경로 설정
	if (!Create_Path(BITMAP_PATH, L"Bitmap\\"))
		return false;

	return true;
}

bool CPathMgr::Create_Path(const string & _strKey, const wchar_t * _pPath, const string & _strBaseKey)
{
	const wchar_t* pBasePath = Find_Path(_strBaseKey);
	wstring strPath;

	if (pBasePath)
		strPath = pBasePath;
	else
		return false;

	strPath += _pPath;

	m_mapPath.emplace(_strKey, strPath);
	return true;
}

const wchar_t * CPathMgr::Find_Path(const string & strKey)
{
	auto iter = m_mapPath.find(strKey);
	if (m_mapPath.end() == iter)
		return nullptr;

	return iter->second.c_str();
}

void CPathMgr::Release()
{
	m_mapPath.clear();
}

CPathMgr::CPathMgr()
{
}


CPathMgr::~CPathMgr()
{
	Release();
}
