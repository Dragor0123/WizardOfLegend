#pragma once

#ifndef __TILEMGR_H__
#define __TILEMGR_H__

#include "Singleton.h"

class CObj;
class CTileMgr : public Singleton<CTileMgr>
{
	friend class Singleton<CTileMgr>;

public:
	bool Initialize();
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

	void Save_Tile();
	void Load_Tile();

private:
	CTileMgr();
	~CTileMgr();
	CTileMgr(const CTileMgr&) = delete;
	CTileMgr& operator=(const CTileMgr&) = delete;

private:
	vector<CObj*>		m_vecTile;
};

#endif