#pragma once

#ifndef __COLLISIONMGR_H__
#define __COLLISIONMGR_H__

class CObj;

class CCollisionMgr
{
public:
	static bool Collision_Obj_Tile(CObj* _pObj, float * _fPushedX, float * _fPushedY);
	static void Collision_Obj_Tile(list<CObj*>& _Dst);
	static bool Collision_Obj_Obstacle(CObj* _pObj, float * _fPushedX, float * _fPushedY);

	static void Collision_Rect(list<CObj*>& _Dst, list<CObj*>& _Src);
	// 사각형 충돌시 밀기 _Src : 밀리는 놈, _Dst : 미는 놈
	static void Collision_RectEx(list<CObj*>& _Dst, list<CObj*>& _Src); 
	static void Collision_Circle(list<CObj*>& _Dst, list<CObj*>& _Src);
	
	// 왼쪽 인자에는 무조건 총알(또는 쉴드), 오른쪽 인자에는 플레이어, 몬스터, Obstacle등...
	static void Collision_CircleRect(list<CObj*>& _Dst, list<CObj*>& _Src);

	static bool Is_PointInRect(const POINT& _pt, LPRECT _lpRC);
	static bool Is_PointInCircle(const POINT& _pt, const POINT& _cPt, float _cRad);
	static bool Check_CircleCircle(CObj* _Dst, CObj* _Src);
	static bool Check_CircleRect(CObj* _circle, CObj* _rect);
	static bool Check_CircleRect(const RECT* _lhs, CObj* _rect);
	static bool Check_RectRect(CObj* _lhs, CObj* _rhs, float *_fX, float * _fY);
	static bool Check_RectRect(const RECT* _lhs, const RECT* _rhs, float* _fX, float* _fY);
	//

private:
	static void Play_Enemy_Hit_Rand();
	static void Add_MP_Logic(CObj* _dstObj);
	// 두번째 인자가 첫번째 인자를 밀어버린다.
	static bool CollisionRectPush(CObj* _Pushee, CObj* _Pusher, float* _pfX, float* _pfY, float _pushScale = 1.f);

	// 두번째 인자가 첫번째 인자를 밀어버린다.
	static bool CollisionDiagRectPush(CObj* _Pushee, CObj* _Pusher, float* _pfX, float* _pfY, float _pushScale = 1.f);

private:
	CCollisionMgr();
	~CCollisionMgr();
	CCollisionMgr(const CCollisionMgr&) = delete;
	CCollisionMgr& operator=(const CCollisionMgr&) = delete;
};

#endif