#pragma once

#ifndef __TILEMGR_H__
#define __TILEMGR_H__

#include "Singleton.h"
#include "../Manager/CollisionMgr.h"

class CObj;
class CTileMgr
{
	friend void CCollisionMgr::Collision_Obj_Tile(list<CObj*>& _Dst);
	friend bool CCollisionMgr::Collision_Obj_Tile(CObj * _pObj, float * _fPushedX, float * _fPushedY);

public:
	static CTileMgr* Get_Instance() {
		if (nullptr == m_pInstance)
			m_pInstance = new CTileMgr;
		return m_pInstance;
	}

	static void Destroy_Instance()
	{
		if (nullptr != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	bool Initialize(const string& _strKey);
	void Update(float _fdTime);
	void Late_Update(float _fdTime);
	void Render(HDC _DC, float _fdTime);
	void Release();

	//ASSERT문 던진다.
	int Get_Tile_Col_Max(const string& _strKey) const;
	//ASSERT문 던진다.
	int Get_Tile_Row_Max(const string& _strKey) const;

	// void Collision(float _fdTime);
public:
	void Picking_Tile(POINT& _pt,
		int _iDrawRow, int _iDrawCol, const TILEENUM::OPTION& _eOption);
	void Catching_Tile(POINT& _pt,
		int* _iDrawRow, int* _iDrawCol, TILEENUM::OPTION* _eOption);

	void Save_Tile(const string& _strImageKey);
	void Load_Tile(const string& _strImageKey);

private:
	CTileMgr();
	~CTileMgr();
	CTileMgr(const CTileMgr&) = delete;
	CTileMgr& operator=(const CTileMgr&) = delete;

private:
	vector<CObj*>		m_vecTile;
	static unordered_map<string, wstring> m_mapFile;

	static CTileMgr* m_pInstance;
};

#endif