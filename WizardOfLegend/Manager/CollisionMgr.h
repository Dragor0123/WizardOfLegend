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
	// �簢�� �浹�� �б� _Src : �и��� ��, _Dst : �̴� ��
	static void Collision_RectEx(list<CObj*>& _Dst, list<CObj*>& _Src); 
	static void Collision_Circle(list<CObj*>& _Dst, list<CObj*>& _Src);
	
	// ���� ���ڿ��� ������ �Ѿ�(�Ǵ� ����), ������ ���ڿ��� �÷��̾�, ����, Obstacle��...
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
	// �ι�° ���ڰ� ù��° ���ڸ� �о������.
	static bool CollisionRectPush(CObj* _Pushee, CObj* _Pusher, float* _pfX, float* _pfY, float _pushScale = 1.f);

	// �ι�° ���ڰ� ù��° ���ڸ� �о������.
	static bool CollisionDiagRectPush(CObj* _Pushee, CObj* _Pusher, float* _pfX, float* _pfY, float _pushScale = 1.f);

private:
	CCollisionMgr();
	~CCollisionMgr();
	CCollisionMgr(const CCollisionMgr&) = delete;
	CCollisionMgr& operator=(const CCollisionMgr&) = delete;
};

#endif