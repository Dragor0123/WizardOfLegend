#pragma once

#ifndef __COLLISIONMGR_H__
#define __COLLISIONMGR_H__

class CCollisionMgr
{
public:
	static void Collision_Rect(list<CObj*>& _Dst, list<CObj*>& _Src);
	static void Collision_RectEx(list<CObj*>& _Dst, list<CObj*>& _Src); //사각형 충돌시 밀기
	static void Collision_Circle(list<CObj*>& _Dst, list<CObj*>& _Src);
	//static void Collision_CircleRect(list<CObj*>& _circle, list<CObj*>& _rect);
	static void Collision_Obj_Tile(list<CObj*>& _Dst);
	static bool Is_PointInRect(const POINT& _pt, LPRECT _lpRC);
	static bool Is_PointInCircle(const POINT& _pt, const POINT& _cPt, float _cRad);
	static bool Check_CircleCircle(CObj* _Dst, CObj* _Src);
	static bool Check_CircleRect(CObj* _circle, CObj* _rect);
	static bool Check_RectRect(CObj* _lhs, CObj* _rhs, float *_fX, float * _fY);

private:
	// 두번째 인자가 첫번째 인자를 밀어버린다.
	static void CollisionRectPush(CObj* _Pushee, CObj* _Pusher, float* _pfX, float* _pfY);
private:
	CCollisionMgr();
	~CCollisionMgr();
	CCollisionMgr(const CCollisionMgr&) = delete;
	CCollisionMgr& operator=(const CCollisionMgr&) = delete;
};

#endif