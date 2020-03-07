#include "../stdafx.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "Player.h"
#include "Mouse.h"
#include "../Manager/ScrollMgr.h"
#include "../Manager/CollisionMgr.h"
#include "../Manager/SceneMgr.h"

CObjMgr::CObjMgr()
{
}


CObjMgr::~CObjMgr()
{
	Release();
}

bool CObjMgr::Initialize()
{
	//Add_Object(OBJID::MOUSE, CAbstractFactory<CMouse>::Create());
	return true;
}

void CObjMgr::Update(float _fdTime)
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		auto iter = m_listObj[i].begin();
		for (; iter != m_listObj[i].end(); )
		{
			int iEvent = (*iter)->Update(_fdTime);

			if (OBJ_DEAD == iEvent)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_listObj[i].erase(iter);
			}
			else
				++iter;
		}
	}
	/*
	UNDERDECO, TELECIR,
	SUMMONCARD, PLAYER,
	MONSTER, BOSS,
	P_RECTBULLET, P_CIRBULLET,
	M_RECTBULLET, M_CIRBULLET,
	P_SHIELD, M_SHIELD,
	NPC, CARD, OBSTACLE,
	ORB, PORTAL, GOODS,
	EFFECT,
	MENU_UI, STAGE_UI,
	INVENTORY, MOUSE,
	*/

	for (int i = OBJID::PLAYER; i < OBJID::NPC; ++i) {
		CCollisionMgr::Collision_Obj_Tile(m_listObj[i]);
	}
}

void CObjMgr::Late_Update(float _fdTime)
{
	for (int i = 0; i < OBJID::END; ++i)
	{
		for (auto& pObj : m_listObj[i])
		{
			pObj->Late_Update(_fdTime);
			if (m_listObj[i].empty())
				break;

			GROUPID::ID	 eID = pObj->Get_Group_ID();
			m_listRender[eID].emplace_back(pObj);
		}
	}
}

void CObjMgr::Collision(float _fdTime)
{
	CCollisionMgr::Collision_Rect(m_listObj[OBJID::PLAYER], m_listObj[OBJID::TELECIR]);
	CCollisionMgr::Collision_Rect(m_listObj[OBJID::PLAYER], m_listObj[OBJID::NPC]);
	CCollisionMgr::Collision_Rect(m_listObj[OBJID::PLAYER], m_listObj[OBJID::CARD]);
	CCollisionMgr::Collision_Rect(m_listObj[OBJID::PLAYER], m_listObj[OBJID::MONSTER]);
	CCollisionMgr::Collision_Rect(m_listObj[OBJID::PLAYER], m_listObj[OBJID::GOODS]);
	
	// 쉴드
	CCollisionMgr::Collision_Rect(m_listObj[OBJID::P_SHIELD], m_listObj[OBJID::M_CIRBULLET]);
	CCollisionMgr::Collision_Rect(m_listObj[OBJID::P_SHIELD], m_listObj[OBJID::M_RECTBULLET]);

	// 총알
	CCollisionMgr::Collision_Rect(m_listObj[OBJID::P_RECTBULLET], m_listObj[OBJID::MONSTER]);
	CCollisionMgr::Collision_Rect(m_listObj[OBJID::M_RECTBULLET], m_listObj[OBJID::PLAYER]);
	CCollisionMgr::Collision_CircleRect(m_listObj[OBJID::P_CIRBULLET], m_listObj[OBJID::MONSTER]);
	CCollisionMgr::Collision_CircleRect(m_listObj[OBJID::M_CIRBULLET], m_listObj[OBJID::PLAYER]);

	// 장애물 충돌판정
	CCollisionMgr::Collision_Rect(m_listObj[OBJID::PLAYER], m_listObj[OBJID::OBSTACLE]);
	CCollisionMgr::Collision_Rect(m_listObj[OBJID::MONSTER], m_listObj[OBJID::OBSTACLE]);
	CCollisionMgr::Collision_Rect(m_listObj[OBJID::P_RECTBULLET], m_listObj[OBJID::OBSTACLE]);
	CCollisionMgr::Collision_Rect(m_listObj[OBJID::M_RECTBULLET], m_listObj[OBJID::OBSTACLE]);
	CCollisionMgr::Collision_CircleRect(m_listObj[OBJID::P_CIRBULLET], m_listObj[OBJID::OBSTACLE]);
	CCollisionMgr::Collision_CircleRect(m_listObj[OBJID::M_CIRBULLET], m_listObj[OBJID::OBSTACLE]);
}

void CObjMgr::Render(HDC _DC, float _fdTime)
{
	// 주의 스크롤을 먹여야할 애가 있고 안 먹여야할 애가 있음
	// 대표적: UI 스크롤 먹이면 안 됨. 고정임.
	// OBJID::UI이면 예외처리 해주자

	if (CSceneMgr::SCENE_EDIT == CSceneMgr::Get_Instance()->Get_Scene_ID())
	{
		for (int i = 0; i < OBJID::END; ++i)
		{
			for (auto& pObj : m_listObj[i])
				pObj->Render(_DC, _fdTime, CScrollMgr::Get_Instance()->Get_ScrollX(), CScrollMgr::Get_Instance()->Get_ScrollY());
		}
	}
	else
	{
		for (int i = 0; i < GROUPID::END; ++i)
		{
			if (i == GROUPID::GAMEOBJECT)
				m_listRender[i].sort(ObjectSortY<CObj*>);

			for (auto& pObj : m_listRender[i])
				pObj->Render(_DC, _fdTime, CScrollMgr::Get_Instance()->Get_ScrollX(), CScrollMgr::Get_Instance()->Get_ScrollY());

			m_listRender[i].clear();
		}
	}
}

void CObjMgr::Release()
{
	for (int i = 0; i < OBJID::END; ++i)
		Safe_Delete_VecList(m_listObj[i]);

	for (int i = 0; i < GROUPID::END; ++i)
		Safe_Delete_VecList(m_listRender[i]);
}

void CObjMgr::Add_Object(OBJID::ID _eID, CObj * _pObj)
{
	m_listObj[_eID].emplace_back(_pObj);
}

CObj * CObjMgr::Get_Target(CObj * _pObj, OBJID::ID _eID)
{
	if (m_listObj[_eID].empty())
		return nullptr;

	CObj*	pTarget = nullptr;
	float	fDis = 0.f;

	for (auto& pNode : m_listObj[_eID])
	{
		if (pNode->Get_Dead())
			continue;

		float fX = abs(_pObj->Get_Info().fX - pNode->Get_Info().fX);
		float fY = abs(_pObj->Get_Info().fY - pNode->Get_Info().fY);
		float fDis2 = sqrtf(fX * fX + fY * fY);

		if (!pTarget || fDis > fDis2)
		{
			pTarget = pNode;
			fDis = fDis2;
		}
	}
	return pTarget;
}

void CObjMgr::Delete_ID(OBJID::ID _eID)
{
	for (auto& pObj : m_listObj[_eID]) {
		Safe_Delete(pObj);
	}

	m_listObj[_eID].clear();
}

void CObjMgr::Delete_A_Obj(OBJID::ID _eID, CObj* _pObj)
{
	if (m_listObj[_eID].empty())
		return;

	list<CObj*>::iterator iter = m_listObj[_eID].begin();
	list<CObj*>::iterator iterEnd = m_listObj[_eID].end();
	for (; iter != iterEnd; )
	{
		if ((*iter) == _pObj) {
			SAFE_DELETE(_pObj);
			iter = m_listObj[_eID].erase(iter);
		}
		else
		{
			++iter;
		}
	}
}
