#include "../stdafx.h"
#include "../Obj/Obj.h"
#include "CollisionMgr.h"
#include "../Manager/TileMgr.h"
#include "../Obj/Tile.h"
#include "../Obj/Player.h"
#include "../Manager/ScrollMgr.h"

CCollisionMgr::CCollisionMgr()
{
}

CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::Collision_Rect(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	if (_Src.empty() || _Dst.empty())
		return;

	for (auto& dstObj : _Dst) {
		for (auto& srcObj : _Src) {
			if (IntersectRect(&rc, &dstObj->Get_Rect(), &srcObj->Get_Rect())) {
				dstObj->Set_Dead();
				srcObj->Set_Dead();
			}
		}
	}
}

// _Dst의 원소와 _Src의 원소가 부딪히면 _Dst가 _Src를 밀어내는 충돌함수
void CCollisionMgr::Collision_RectEx(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	if (_Src.empty() || _Dst.empty())
		return;

	for (auto& dstObj : _Dst)
	{
		for (auto& srcObj : _Src)
		{
			float fX = 0.f, fY = 0.f;
			CollisionRectPush(srcObj, dstObj, &fX, &fY);
		}
	}
}

void CCollisionMgr::Collision_Circle(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	if (_Src.empty() || _Dst.empty())
		return;

	for (auto& dstObj : _Dst) {
		for (auto& srcObj : _Src) {
			if (Check_CircleCircle(dstObj, srcObj)) {
				dstObj->Set_Dead();
				srcObj->Set_Dead();
			}
		}
	}
}

/*
// _circle에는 AbsBullet류가, _rect에는 Monster, Player 등등
void CCollisionMgr::Collision_CircleRect(list<CObj*>& _circle, list<CObj*>& _rect)
{
	if (_rect.empty() || _circle.empty())
		return;


	// _circle이 총알류이면
	if (dynamic_cast<CAbsBullet*>(_circle.front()))
	{
		for (auto& _circleObj : _circle)
		{
			// 몬스터와 충돌했을 시
			if (dynamic_cast<CMonster*>(_rect.front()))
			{
				for (auto& _rcObj : _rect)
				{
					if (Check_CircleRect(_circleObj, _rcObj))
					{
						_circleObj->Set_Dead();
						//_rcObj->Set_Dead();
						_rcObj->Sub_Hp(dynamic_cast<CAbsBullet*>(_circleObj)->Get_Att());
						for (auto& others : _rect) {
							dynamic_cast<CMonster*>(others)->Set_bTarget(false);
						}
						dynamic_cast<CMonster*>(_rcObj)->Set_bTarget(true);
						break;
					}
				}
			}

			// Player와 충돌했을 시
			if (dynamic_cast<CPlayer*>(_rect.front()))
			{
				for (auto& _rcObj : _rect)
				{
					if (Check_CircleRect(_circleObj, _rcObj))
					{
						_circleObj->Set_Dead();
						_rcObj->Set_Dead();
						break;
					}
				}
			}
		}
	}
	else  //_circle이 총알류가 아니면
	{
		for (auto& _circleObj : _circle)
		{
			for (auto& _rcObj : _rect)
			{
				if (Check_CircleRect(_circleObj, _rcObj))
				{
					_circleObj->Set_Dead();
					_rcObj->Set_Dead();
					break;
				}
			}
		}
	}
}
*/


// 첫번째 인자가 밀린다.
void CCollisionMgr::CollisionRectPush(CObj* _Pushee, CObj* _Pusher, float* _pfX, float* _pfY)
{
	if (Check_RectRect(_Pusher, _Pushee, _pfX, _pfY))
	{
		if (*_pfX > *_pfY) { // 상하 충돌
			if (_Pusher->Get_HitInfo().fY < _Pushee->Get_HitInfo().fY)
				_Pushee->Set_PosY(*_pfY);
			else
				_Pushee->Set_PosY(-*_pfY);
		}
		else {	// 좌우 충돌
			if (_Pusher->Get_HitInfo().fX < _Pushee->Get_HitInfo().fX)
				_Pushee->Set_PosX(*_pfX);
			else
				_Pushee->Set_PosX(-*_pfX);
		}

		//if (dynamic_cast<CPlayer*>(_Pushee))
		//	static_cast<CPlayer*>(_Pushee)->OffSet();
	}
}

void CCollisionMgr::Collision_Obj_Tile(list<CObj*>& _Dst)
{
	if (CTileMgr::Get_Instance()->m_vecTile.empty())
		return;

	for (auto& dstObj : _Dst) {
		//float fBottomY = dstObj->Get_HitInfo().fY + float(dstObj->Get_HitInfo().iCY >> 1);

		//std::pair<float, float> fPair[2] = { 
		//	make_pair(dstObj->Get_HitInfo().fX, dstObj->Get_HitInfo().fY),
		//	make_pair(dstObj->Get_HitInfo().fX, fBottomY)
		//};

		float fX = dstObj->Get_HitInfo().fX;
		float fY = dstObj->Get_HitInfo().fY;
		const vector<CObj*>& refVec = CTileMgr::Get_Instance()->m_vecTile;

		int col = (int)(fX / TILECX);
		int row = (int)(fY / TILECY);
		for (int r = row - 1; r < row + 2; ++r)
		{
			for (int c = col - 1; c < col + 2; ++c)
			{
				if (r == row && c == col)
					continue;
				int iIdx = r * TILE_NUMX + c;
				if (iIdx < 0 || refVec.size() <= (size_t)iIdx)
					continue;
				if (TILEENUM::OPT_MOVE != static_cast<CTile*>(refVec[iIdx])->Get_Option())
				{
					float fX = 0.f, fY = 0.f;
					CollisionRectPush(dstObj, refVec[iIdx], &fX, &fY);
					if (dynamic_cast<CPlayer*>(dstObj))
					{
						if (CPlayer::DASH == static_cast<CPlayer*>(dstObj)->Get_PlayerState())
							static_cast<CPlayer*>(dstObj)->Dash_Off();
					}
					dstObj->Update_Rect();
					dstObj->Update_HitRect();

				}
			}
		}

	}
}


// int iIdx = row * TILE_NUMX + col;
//if (0 == i) {
//	if (iIdx - 1 > 0)
//	if (TILEENUM::OPT_MOVE != static_cast<CTile*>(refVec[iIdx - 1])->Get_Option())
//		CollisionRectPush(dstObj, refVec[iIdx - 1]);
//	if (TILEENUM::OPT_MOVE != static_cast<CTile*>(refVec[iIdx - 1 * TILECX])->Get_Option())
//		CollisionRectPush(dstObj, refVec[iIdx - 1 * TILECX]);
//}


bool CCollisionMgr::Is_PointInRect(const POINT & _pt, LPRECT _lpRC)
{
	if ((_lpRC->left <= _pt.x && _pt.x <= _lpRC->right) &&
		(_lpRC->top <= _pt.y && _pt.y <= _lpRC->bottom))
		return true;
	else
		return false;
}

bool CCollisionMgr::Is_PointInCircle(const POINT & _pt, const POINT & _cPt, float _cRad)
{
	float fDistance = Get_Distance(_pt, _cPt);
	return _cRad >= fDistance;
}

bool CCollisionMgr::Check_CircleCircle(CObj * _Dst, CObj * _Src)
{
	float fDistance = Get_Distance(_Dst->Get_Info(), _Src->Get_Info());
	float rad1 = float(_Dst->Get_Info().iCX >> 1);
	float rad2 = float(_Src->Get_Info().iCX >> 1);

	return (rad1 + rad2) >= fDistance;
}

bool CCollisionMgr::Check_CircleRect(CObj * _circle, CObj * _rect)
{
	LONG _cX = LONG(_circle->Get_Info().fX);
	LONG _cY = LONG(_circle->Get_Info().fY);
	LONG _radius = LONG(_circle->Get_Info().iCX >> 1);

	const RECT& _rc = _rect->Get_Rect();
	POINT circlePt = { _cX, _cY };

	if ((_rc.left <= _cX && _cX <= _rc.right) ||
		(_rc.top <= _cY && _cY <= _rc.bottom))
	{
		RECT rcEx = { _rc.left - _radius,
			_rc.top - _radius,
			_rc.right + _radius,
			_rc.bottom + _radius
		};

		if (Is_PointInRect(circlePt, &rcEx))
			return true;
	}
	else {
		POINT Pt1 = { _rc.left, _rc.top };
		if (Is_PointInCircle(Pt1, circlePt, float(_radius)))
			return true;

		Pt1.x = _rc.right;
		if (Is_PointInCircle(Pt1, circlePt, float(_radius)))
			return true;

		Pt1.x = _rc.left; Pt1.y = _rc.bottom;
		if (Is_PointInCircle(Pt1, circlePt, float(_radius)))
			return true;

		Pt1.x = _rc.right;
		if (Is_PointInCircle(Pt1, circlePt, float(_radius)))
			return true;
	}

	return false;
}

// CObj* _lhs, *_rhs : 충돌검사할 두 사각형, _fX : 겹친 가로 길이, _fY : 겹친 세로 길이
bool CCollisionMgr::Check_RectRect(CObj* _lhs, CObj* _rhs, float* _fX, float* _fY)
{
	// fdX : 두 사각형의 x좌표 거리
	// fdY : 두 사각형의 y좌표 거리
	float	fdX = abs(_lhs->Get_Info().fX - _rhs->Get_Info().fX);
	float	fdY = abs(_lhs->Get_Info().fY - _rhs->Get_Info().fY);

	// fCX : 두 사각형의 가로길이의 절반의 합 (반지름?의 합)
	// fCY : 두 사각형의 세로길이의 절반의 합
	float	fCX = (float)((_lhs->Get_Info().iCX + _rhs->Get_Info().iCX) >> 1);
	float	fCY = (float)((_lhs->Get_Info().iCY + _rhs->Get_Info().iCY) >> 1);

	if (fCX > fdX && fCY > fdY)
	{
		*_fX = fCX - fdX;
		*_fY = fCY - fdY;
		return true;
	}
	else
		return false;
}
