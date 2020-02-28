#pragma once

#ifndef __OBJMGR_H__
#define __OBJMGR_H__

#include "Singleton.h"

class CObj;

class CObjMgr : public Singleton<CObjMgr>
{
	friend class Singleton<CObjMgr>;

public:
	bool Initialize();
	void Update(float _fdTime);
	void Late_Update(float _fdTime);
	void Collision(float _fdTime);
	void Render(HDC _DC, float _fdTime);
	void Release();

public:
	void Add_Object(OBJID::ID _eID, CObj* _pObj);
	CObj* Get_Target(CObj* _pObj, OBJID::ID _eID);
	const list<CObj*>& Get_listObj(OBJID::ID _eId) const { return m_listObj[_eId]; }
	void Delete_ID(OBJID::ID _eID);
	void Delete_A_Obj(OBJID::ID _eID, CObj* _pObj);

private:

private:
	CObjMgr();
	~CObjMgr();
	CObjMgr(const CObjMgr&) = delete;
	CObjMgr& operator=(const CObjMgr&) = delete;

private:
	list<CObj*>			m_listObj[OBJID::END];
};

#endif
