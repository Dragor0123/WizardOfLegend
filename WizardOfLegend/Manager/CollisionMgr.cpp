#include "../stdafx.h"
#include "../Obj/Obj.h"
#include "CollisionMgr.h"
#include "../Manager/TileMgr.h"
#include "../Obj/Tile.h"
#include "../Manager/ScrollMgr.h"
#include "../Obj/Player.h"
#include "../Obj/MoveObj.h"
#include "../Obj/FAble.h"
#include "../Obj/Bullet.h"
#include "../Obj/LineBullet.h"
#include "../Obj/ScrewBullet.h"
#include "../Obj/MeeleBullet.h"
#include "../Obj/Boss.h"
#include "../Obj/Shield.h"
#include "../Obj/Gold.h"
#include "../Obj/IceSphere.h"
#include "../Obj/ObjMgr.h"
#include "../Obj/IcicleEffect.h"

CCollisionMgr::CCollisionMgr()
{
}

CCollisionMgr::~CCollisionMgr()
{
}

//  _Dst : 주체, _Src : 대상(맞는놈)
void CCollisionMgr::Collision_Rect(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	if (_Src.empty() || _Dst.empty())
		return;

	int _DstGroupCode = _Dst.front()->Get_Collision_Code();
	int _SrcGroupCode = _Src.front()->Get_Collision_Code();
	bool b_srcObjCollidedWall = false;

	// 플레이어일 경우
	if (_DstGroupCode == CC_PLAYER) 
	{
		for (auto& dstObj : _Dst)
		{
			if (!(_SrcGroupCode & 0xC00) && (_SrcGroupCode >> 9 & 0x1)) // _SrcCode가 FABLE류이면
			{
				for (auto& srcObj : _Src)
				{
					if (IntersectRect(&rc, &dstObj->Get_HitRect(), &srcObj->Get_HitRect()))
						static_cast<CFAble*>(srcObj)->Enable_FButton();
					else
						static_cast<CFAble*>(srcObj)->Disable_FButton();
				}
			}
			else if (!(_SrcGroupCode & 0xF00) && (_SrcGroupCode & 0x070)) // monster임
			{
				if (_SrcGroupCode & CC_MONSTER_BOSS) // 보스일경우
				{
					for (auto& srcObj : _Src)
					{
						float fX = 0.f, fY = 0.f;
						if (!static_cast<CBoss*>(srcObj)->Get_Hittable())
						{
							// 플레이어가 보스한테 밀리는 것도 해야하나??
							if (IntersectRect(&rc, &dstObj->Get_HitRect(), &srcObj->Get_HitRect()))
								static_cast<CPlayer*>(dstObj)->Sub_Hp(12);
						}
					}
				}
				else //일반몹일경우
				{
					for (auto& srcObj : _Src)
					{
						float fX = 0.f, fY = 0.f;
						if (srcObj->Get_Collision_Code() == CC_MONSTER_PUSHABLE)
						{
							if (CollisionRectPush(srcObj, dstObj, &fX, &fY))
							{
								if (Collision_Obj_Tile(srcObj, &fX, &fY))
								{
									if (dynamic_cast<CPlayer*>(dstObj))
									{
										if (CPlayer::DASH == static_cast<CPlayer*>(dstObj)->Get_PlayerState())
											static_cast<CPlayer*>(dstObj)->Dash_Off();
									}
									CollisionRectPush(dstObj, srcObj, &fX, &fY);
								}
							}
						}
						else
						{
							if (IntersectRect(&rc, &dstObj->Get_HitRect(), &srcObj->Get_HitRect()))
								static_cast<CPlayer*>(dstObj)->Sub_Hp(12);
						}
					}
				}
			}
			else if (!(_SrcGroupCode & 0xE00) && (_SrcGroupCode >> 8 & 0x1)) // Goods일 경우
			{
				 for (auto& srcObj : _Src)
				 {
					 if (srcObj->Get_Collision_Code() == CC_GGOLD)
					 {
						 if (IntersectRect(&rc, &dstObj->Get_HitRect(), &srcObj->Get_HitRect()))
						 {
							 static_cast<CPlayer*>(dstObj)->Add_Gold(static_cast<CGold*>(srcObj)->Get_Gold());
							 srcObj->Set_Dead();
						 }
					 }
					 // 카오스잼은 생략
				 }
			}
			else if (!(_SrcGroupCode & 0x800) && (_SrcGroupCode >> 10 & 0x1)) // CC_OBSTACLE 0x400
			{ // Dst : Player , Src : OBSTACLE 
				float fX = 0.f, fY = 0.f;
				for (auto& srcObj : _Src)
				{
					if (srcObj->Get_Collision_Code() & 0x080)				// HITTABLE류 장애물들
					{
						if (srcObj->Get_Collision_Code() >= CC_HITTABLE_BLOCK_OBS)
						{ // src가 플레이어를 밀어낸다.
							if (CollisionRectPush(dstObj, srcObj, &fX, &fY))
							{
								if (Collision_Obj_Tile(dstObj, &fX, &fY)){
									if (dynamic_cast<CPlayer*>(dstObj))
									{
										if (CPlayer::DASH == static_cast<CPlayer*>(dstObj)->Get_PlayerState())
											static_cast<CPlayer*>(dstObj)->Dash_Off();
									}
								}
							}
						}
						else if (CC_HITTABLE_NOBLOCK_OBS == srcObj->Get_Collision_Code() || CC_HITTABLE_OBS == srcObj->Get_Collision_Code())
						{
							// 상황 따라 다른데...
						}
					}
					else // NO_HITTABLE
					{
						if (CC_NOHIT_BLOCK_OBS == srcObj->Get_Collision_Code())
						{
							if (CollisionRectPush(dstObj, srcObj, &fX, &fY))
							{
								if (Collision_Obj_Tile(dstObj, &fX, &fY))
								{
									if (dynamic_cast<CPlayer*>(dstObj))
									{
										if (CPlayer::DASH == static_cast<CPlayer*>(dstObj)->Get_PlayerState())
											static_cast<CPlayer*>(dstObj)->Dash_Off();
									}
								}
							}
						}
						else
						{   
							// 높은 확률로 포탈임.
							// dynamic_cast<CPortal*>
						}
					}
				}
			}
			else if (_SrcGroupCode & 0x800)	//ORB
			{

			}
		}
	}
	// dst가 총알 또는 쉴드일 경우
	else if (_DstGroupCode & 0x0F)
	{
		if (0x01 <= _DstGroupCode && _DstGroupCode <= CC_PSHIELD_REFLECT) // PLAYER BULLET + PLAYERSHIELD
		{
			for (auto& dstObj : _Dst)
			{
				if (!(_SrcGroupCode & 0xF00) && (_SrcGroupCode & 0x070)) // monster임
				{
					if (_SrcGroupCode & CC_MONSTER_BOSS)	// 보스몹임
					{
						for (auto& srcObj : _Src)
						{
							if (static_cast<CBoss*>(srcObj)->Get_Hittable())
							{
								float fX = 0.f, fY = 0.f;
								int dstObjCode = dstObj->Get_Collision_Code();
								if (0x05 <= dstObjCode && dstObjCode < CC_PSHIELD_NOREFLECT) // 충돌시 밀어버리는 총알
								{
									if (dynamic_cast<CIceSphere*>(dstObj))
									{ // 아이스 스피어는 보스 몹을 밀어내지 않는다.
										if (IntersectRect(&rc, &dstObj->Get_HitRect(), &srcObj->Get_HitRect()))
										{
											static_cast<CMonster*>(srcObj)->Sub_Hp(static_cast<CBullet*>(dstObj)->Get_Att());
											static_cast<CBoss*>(srcObj)->Set_Boss_State(7);
											static_cast<CBullet*>(dstObj)->Set_Collision(true);
										}
									}
									else if (CollisionRectPush(srcObj, dstObj, &fX, &fY))
									{
										static_cast<CMonster*>(srcObj)->Sub_Hp(static_cast<CBullet*>(dstObj)->Get_Att());
										static_cast<CBoss*>(srcObj)->Set_Boss_State(7);
										static_cast<CBullet*>(dstObj)->Set_Collision(true);
										if (Collision_Obj_Tile(srcObj, &fX, &fY))
										{
											CollisionRectPush(dstObj, srcObj, &fX, &fY);
										}
									}
								}
								else if (CC_PBULLET_NWALL_NPUSH_DRAG <= dstObjCode && dstObjCode < CC_PBULLET_NWALL_PUSH) // 밀지 않는 총알.
								{
									if (IntersectRect(&rc, &dstObj->Get_HitRect(), &srcObj->Get_HitRect()))
									{
										static_cast<CMonster*>(srcObj)->Sub_Hp(static_cast<CBullet*>(dstObj)->Get_Att());
										static_cast<CBoss*>(srcObj)->Set_Boss_State(7);
										static_cast<CBullet*>(dstObj)->Set_Collision(true);
									}
								}
								else if (dynamic_cast<CShield*>(dstObj))
								{
									if (IntersectRect(&rc, &dstObj->Get_HitRect(), &srcObj->Get_HitRect()))
									{
										static_cast<CMonster*>(srcObj)->Sub_Hp(static_cast<CShield*>(dstObj)->Get_Att());
										static_cast<CBoss*>(srcObj)->Set_Boss_State(7);
									}
								}
							}
							else // 보스가 Hittable이 아닌 경우
							{
								if (dstObj->Get_Collision_Code() < CC_PSHIELD_NOREFLECT) {
									if (IntersectRect(&rc, &dstObj->Get_HitRect(), &srcObj->Get_HitRect()))
										static_cast<CBullet*>(dstObj)->Set_Collision(true);		// 총알만 없애 준다.
								}
							}
						}
					}
					else  // 일반 몹일 경우
					{
						for (auto& srcObj : _Src)
						{
							// 일반 몹 충돌처리
							float fX = 0.f, fY = 0.f;
							int dstObjCode = dstObj->Get_Collision_Code();
							if (0x05 <= dstObjCode && dstObjCode < CC_PSHIELD_NOREFLECT) // 충돌시 밀어버리는 총알
							{
								if (CollisionRectPush(srcObj, dstObj, &fX, &fY))
								{
									static_cast<CMonster*>(srcObj)->Sub_Hp(static_cast<CBullet*>(dstObj)->Get_Att());
									static_cast<CMonster*>(srcObj)->Set_Monster_State(CMonster::HIT);
									if (dynamic_cast<CIceSphere*>(dstObj))
										static_cast<CBullet*>(dstObj)->Set_Att(0);
									else
										static_cast<CBullet*>(dstObj)->Set_Collision(true);
									if (Collision_Obj_Tile(srcObj, &fX, &fY))
									{
										if (dynamic_cast<CIceSphere*>(dstObj))
										{
											// 몬스터가 벽과 충돌했다고 dstObj에게 알려야함.
											static_cast<CIceSphere*>(dstObj)->Set_bMonsterWall(true);
											CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT,
												CAbstractFactory<CIcicleEffect>::Create(srcObj));
										}
										else
										{
											CollisionRectPush(dstObj, srcObj, &fX, &fY);
										}
									}
								}
							}
							else
							{
								if (dynamic_cast<CBullet*>(dstObj))
								{
									if (IntersectRect(&rc, &dstObj->Get_HitRect(), &srcObj->Get_HitRect()))
									{
										static_cast<CMonster*>(srcObj)->Sub_Hp(static_cast<CBullet*>(dstObj)->Get_Att());
										static_cast<CMonster*>(srcObj)->Set_Monster_State(CMonster::HIT);
										static_cast<CBullet*>(dstObj)->Set_Collision(true);
									}
								}
								else if (dynamic_cast<CShield*>(dstObj))
								{
									if (IntersectRect(&rc, &dstObj->Get_HitRect(), &srcObj->Get_HitRect()))
									{
										static_cast<CMonster*>(srcObj)->Sub_Hp(static_cast<CShield*>(dstObj)->Get_Att());
										static_cast<CMonster*>(srcObj)->Set_Monster_State(CMonster::HIT);
									}
								}
							}
						}
					}
				}// monster if문 end
				else if (!(_SrcGroupCode & 0x800) && (_SrcGroupCode >> 10 & 0x1)) // CC_OBSTACLE 0x400
				{ // 플레이어 총알&쉴드와 장애물 검사

				}

				if (dstObj->Get_Collision_Code() == CC_PSHIELD_NOREFLECT || dstObj->Get_Collision_Code() == CC_PSHIELD_REFLECT)
				{ // 플레이어 쉴드와 적 총알 충돌검사.
					if (_SrcGroupCode >= CC_MBULLET_NWALL_NPUSH && _SrcGroupCode <= CC_MBULLET_WALL_PUSH)
					{
						for (auto& srcObj : _Src) {
							if (dynamic_cast<CMeeleBullet*>(srcObj))
								continue;
							if (IntersectRect(&rc, &dstObj->Get_HitRect(), &srcObj->Get_HitRect()))
							{
								static_cast<CBullet*>(srcObj)->Set_Collision(true);
							}
						}
					}
				}
			}
		}// 플레이어 총알 & 플레이어 쉴드 if문 끝! /// 0x01 <= _DstGroupCode && _DstGroupCode <= CC_PSHIELD_REFLECT

		else // 모든 MONSTER BULLET OR MONSTER SHIELD
		{
			for (auto& dstObj : _Dst)
			{
				// 먼저 플레이어와 충돌부터 시킬것!
				if (_SrcGroupCode == 0x80)
				{
					for (auto& srcObj : _Src)
					{
						float fX = 0.f, fY = 0.f;
						int dstObjCode = dstObj->Get_Collision_Code();
						if (dstObjCode == CC_MBULLET_NWALL_NPUSH || dstObjCode == CC_MBULLET_WALL_NPUSH)
						{
							if (IntersectRect(&rc, &dstObj->Get_HitRect(), &srcObj->Get_HitRect()))
							{
								static_cast<CPlayer*>(srcObj)->Sub_Hp(static_cast<CBullet*>(dstObj)->Get_Att());
								static_cast<CPlayer*>(srcObj)->Set_PlayerState(CPlayer::HIT);
								static_cast<CBullet*>(dstObj)->Set_Collision(true);
							}
						}
						else if (dstObjCode == CC_MBULLET_NWALL_PUSH || dstObjCode == CC_MBULLET_WALL_PUSH)
						{
							if (CollisionRectPush(srcObj, dstObj, &fX, &fY))
							{
								static_cast<CPlayer*>(srcObj)->Sub_Hp(static_cast<CBullet*>(dstObj)->Get_Att());
								static_cast<CPlayer*>(srcObj)->Set_PlayerState(CPlayer::HIT);
								Collision_Obj_Tile(srcObj, &fX, &fY);
								static_cast<CBullet*>(dstObj)->Set_Collision(true);
							}
						}
						else if (dstObjCode == CC_MSHIELD_REFLECT || dstObjCode == CC_MSHIELD_NOREFLECT)
						{
							if (dynamic_cast<CShield*>(dstObj))
							{
								if (IntersectRect(&rc, &dstObj->Get_HitRect(), &srcObj->Get_HitRect()))
								{
									static_cast<CPlayer*>(srcObj)->Sub_Hp(static_cast<CShield*>(dstObj)->Get_Att());
									static_cast<CPlayer*>(srcObj)->Set_PlayerState(CPlayer::HIT);
								}
							}
						}
					}
				}
				//몹의 총알 또는 쉴드가 플레이어가 아닌 다른 것과 부딪힌 경우

			}
		}
	} /////// dst가 총알 또는 쉴드일 경우 끝
	else if (!(_DstGroupCode & 0xF00) && (_DstGroupCode & 0x070))
	{
		for (auto& dstObj : _Dst)
		{
			// 몬스터와 오브젝트 충돌
			if (!(_SrcGroupCode & 0x800) && (_SrcGroupCode >> 10 & 0x1)) // CC_OBSTACLE 0x400
			{ // Dst : 몬스터 , Src : OBSTACLE 
				float fX = 0.f, fY = 0.f;
				for (auto& srcObj : _Src)
				{
					if ((srcObj->Get_Collision_Code() == CC_NOHIT_BLOCK_OBS) ||
						(srcObj->Get_Collision_Code() == CC_HITTABLE_BLOCK_OBS) ||
						(srcObj->Get_Collision_Code() == CC_HITTABLE_BLOCK_LOCK_OBS))
					{
						CollisionRectPush(dstObj, srcObj, &fX, &fY);
					}
				}
			}
		}
	}
}

// _Dst : 미는 놈, _Src : 밀리는 놈
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

// _Dst : 주체, _Src : 대상(타겟)
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
bool CCollisionMgr::CollisionRectPush(CObj* _Pushee, CObj* _Pusher, float* _pfX, float* _pfY)
{
	if (Check_RectRect(_Pusher, _Pushee, _pfX, _pfY))
	{
		if (*_pfX > *_pfY)
		{ // 상하 충돌
			if (_Pusher->Get_HitInfo().fY < _Pushee->Get_HitInfo().fY)
				_Pushee->Set_PosY(*_pfY);
			else
				_Pushee->Set_PosY(-*_pfY);
		}
		else
		{	// 좌우 충돌 (*_pfX < *_pfY)
			if (_Pusher->Get_HitInfo().fX < _Pushee->Get_HitInfo().fX)
				_Pushee->Set_PosX(*_pfX);
			else
				_Pushee->Set_PosX(-*_pfX);
		}
		return true;
	}
	return false;
}


// 밀려진 _pObj가 이차적으로 타일과 충돌하는지...
bool CCollisionMgr::Collision_Obj_Tile(CObj * _pObj, float * _fPushedX, float * _fPushedY)
{
	float fObjX = _pObj->Get_HitInfo().fX;
	float fObjY = _pObj->Get_HitInfo().fY;
	const vector<CObj*>& refVec = CTileMgr::Get_Instance()->m_vecTile;
	int idxCol = (int)(fObjX / TILECX);
	int idxRow = (int)(fObjY / TILECY);

	for (int r = idxRow - 1; r <= idxRow + 1; ++r)
	{
		for (int c = idxCol - 1; c <= idxCol + 1; ++c)
		{
			if (r == idxRow && c == idxCol)
				continue;

			int iIdx = r * TILE_NUMX + c;
			if (iIdx < 0 || refVec.size() <= (size_t)iIdx)
				continue;

			// 충돌해야할 타일이라면 (MOVE타일이 아니라면)
			if (TILEENUM::OPT_MOVE != static_cast<CTile*>(refVec[iIdx])->Get_Option()) 
			{
				if (Check_RectRect(refVec[iIdx], _pObj, _fPushedX, _fPushedY))
				{
					if (dynamic_cast<CBullet*>(_pObj))
						static_cast<CBullet*>(_pObj)->Set_Collision(true);
					else
					{
						if (*_fPushedX > *_fPushedY) { // 상하 충돌
							if (refVec[iIdx]->Get_HitInfo().fY < _pObj->Get_HitInfo().fY)
								_pObj->Set_PosY(*_fPushedY);
							else
								_pObj->Set_PosY(-*_fPushedY);
						}
						else {	// 좌우 충돌 (*_pfX < *_pfY)
							if (refVec[iIdx]->Get_HitInfo().fX < _pObj->Get_HitInfo().fX)
								_pObj->Set_PosX(*_fPushedX);
							else
								_pObj->Set_PosX(-*_fPushedX);
						}

						if (dynamic_cast<CPlayer*>(_pObj))
						{
							if (CPlayer::DASH == static_cast<CPlayer*>(_pObj)->Get_PlayerState())
								static_cast<CPlayer*>(_pObj)->Dash_Off();
						}
					}
					return true;
				} // if (Check_RectRect(refVec[iIdx], _pObj, _fPushedX, _fPushedY))
			}
		}
	}
	return false;
}

void CCollisionMgr::Collision_Obj_Tile(list<CObj*>& _Dst)
{
	if (CTileMgr::Get_Instance()->m_vecTile.empty())
		return;

	for (auto& dstObj : _Dst)
	{

		float fObjX = dstObj->Get_HitInfo().fX;
		float fObjY = dstObj->Get_HitInfo().fY;
		const vector<CObj*>& refVec = CTileMgr::Get_Instance()->m_vecTile;

		float left = dstObj->Get_HitInfo().fX - (float)(dstObj->Get_HitInfo().iCX >> 1);
		float right = left + (float)(dstObj->Get_HitInfo().iCX);
		float top = dstObj->Get_HitInfo().fY - (float)(dstObj->Get_HitInfo().iCY >> 1);
		float bottom = top + (float)(dstObj->Get_HitInfo().iCY);
	
		int idxCol = (int)(fObjX / TILECX);
		int idxRow = (int)(fObjY / TILECY);

		for (int r = idxRow - 1; r <= idxRow + 1; ++r)
		{
			for (int c = idxCol - 1; c <= idxCol + 1; ++c)
			{

				if (r == idxRow && c == idxCol)
					continue;
				int iIdx = r * TILE_NUMX + c;
				if (iIdx < 0 || refVec.size() <= (size_t)iIdx)
					continue;

				if (TILEENUM::OPT_MOVE != static_cast<CTile*>(refVec[iIdx])->Get_Option())
				{
					float fX = 0.f, fY = 0.f;


					if (Check_RectRect(refVec[iIdx], dstObj, &fX, &fY))
					{	
						if (dynamic_cast<CShield*>(dstObj))
							continue;
						int code = dstObj->Get_Collision_Code();

						if (CC_PBULLET_NWALL_NPUSH_DRAG == code ||
							CC_PBULLET_NWALL_NPUSH_NDRAG == code ||
							CC_PBULLET_NWALL_PUSH == code ||
							CC_MBULLET_NWALL_NPUSH == code ||
							CC_MBULLET_NWALL_PUSH == code)
							continue;

						if (dynamic_cast<CBullet*>(dstObj))
						{
							// 수정 요함
							static_cast<CBullet*>(dstObj)->Set_Collision(true);
						}
						else
						{	// dstObj가 Bullet류가 아닐 때
							if (fX > fY) { // 상하 충돌
								if (refVec[iIdx]->Get_HitInfo().fY < dstObj->Get_HitInfo().fY)
									dstObj->Set_PosY(fY);
								else
									dstObj->Set_PosY(-fY);
							}
							else {	// 좌우 충돌 (*_pfX < *_pfY)
								if (refVec[iIdx]->Get_HitInfo().fX < dstObj->Get_HitInfo().fX)
									dstObj->Set_PosX(fX);
								else
									dstObj->Set_PosX(-fX);
							}

							if (dynamic_cast<CPlayer*>(dstObj))
							{
								if (CPlayer::DASH == static_cast<CPlayer*>(dstObj)->Get_PlayerState())
									static_cast<CPlayer*>(dstObj)->Dash_Off();
							}
						}
					}
				} // if (TILEENUM::OPT_MOVE != static_cast<CTile*>(refVec[iIdx])->Get_Option())
			}
		} // 	for (int r = idxRow - 1; r <= idxRow + 1; ++r)
	} // 	for (auto& dstObj : _Dst)
}

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
	float fDistance = Get_Distance(_Dst->Get_HitInfo(), _Src->Get_HitInfo());
	float rad1 = float(_Dst->Get_HitInfo().iCX >> 1);
	float rad2 = float(_Src->Get_HitInfo().iCX >> 1);

	return (rad1 + rad2) >= fDistance;
}

bool CCollisionMgr::Check_CircleRect(CObj * _circle, CObj * _rect)
{
	LONG _cX = LONG(_circle->Get_HitInfo().fX);
	LONG _cY = LONG(_circle->Get_HitInfo().fY);
	LONG _radius = LONG(_circle->Get_HitInfo().iCX >> 1);

	const RECT& _rc = _rect->Get_HitRect();
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
	float	fdX = abs(_lhs->Get_HitInfo().fX - _rhs->Get_HitInfo().fX);
	float	fdY = abs(_lhs->Get_HitInfo().fY - _rhs->Get_HitInfo().fY);

	// fCX : 두 사각형의 가로길이의 절반의 합 (반지름?의 합)
	// fCY : 두 사각형의 세로길이의 절반의 합
	float	fCX = (float)((_lhs->Get_HitInfo().iCX + _rhs->Get_HitInfo().iCX) >> 1);
	float	fCY = (float)((_lhs->Get_HitInfo().iCY + _rhs->Get_HitInfo().iCY) >> 1);

	if (fCX > fdX && fCY > fdY)
	{
		*_fX = fCX - fdX;
		*_fY = fCY - fdY;
		return true;
	}
	else
		return false;
}

bool CCollisionMgr::Check_RectRect(const RECT* _lhs, const RECT* _rhs, float* _fX, float* _fY)
{
	float	fdX = (float)(abs(((_lhs->left + _lhs->right) >> 1) - ((_rhs->left + _rhs->right) >> 1)));
	float	fdY = (float)(abs(((_lhs->top + _lhs->bottom) >> 1) - ((_rhs->top + _rhs->bottom) >> 1)));

	// fCX : 두 사각형의 가로길이의 절반의 합 (반지름?의 합)
	// fCY : 두 사각형의 세로길이의 절반의 합
	float	fCX = (float)(((_lhs->right - _lhs->left) + (_rhs->right - _rhs->left)) >> 1);
	float	fCY = (float)(((_lhs->bottom - _lhs->top) + (_rhs->bottom - _rhs->top)) >> 1);

	if (fCX > fdX && fCY > fdY)
	{
		*_fX = fCX - fdX;
		*_fY = fCY - fdY;
		return true;
	}
	else
		return false;
}

// 충돌이 끝났는지 검사. Dst는 무조건 총알
void CCollisionMgr::Collision_End_Rect(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	if (_Src.empty() || _Dst.empty())
		return;

	int _DstGroupCode = _Dst.front()->Get_Collision_Code();
	int _SrcGroupCode = _Src.front()->Get_Collision_Code();

	if (!(_DstGroupCode & 0x0F))
		return;

	if (0x01 <= _DstGroupCode && _DstGroupCode < CC_MSHIELD_NOREFLECT) // PLAYER BULLET과 SHIELD -> 몬스터와 검사.
	{
		if (!(_SrcGroupCode & 0xF00) && (_SrcGroupCode & 0x070)) // monster임
		{
			if (_SrcGroupCode & CC_MONSTER_BOSS)	// 보스냐
			{
				for (auto& srcObj : _Src)
				{
					bool bCollided = false;
					for (auto& dstObj : _Dst)
					{
						if (IntersectRect(&rc, &dstObj->Get_HitRect(), &srcObj->Get_HitRect()))
							bCollided = true;
					}
					if (!bCollided) {
						if (CBoss::HIT == static_cast<CBoss*>(srcObj)->Get_Boss_State())
							static_cast<CBoss*>(srcObj)->Set_Boss_State(0);	//idle == 0
					}
				}
			}
			else // 일반몹이냐
			{
				for (auto& srcObj : _Src)
				{
					bool bCollided = false;
					for (auto& dstObj : _Dst)
					{
						if (IntersectRect(&rc, &dstObj->Get_HitRect(), &srcObj->Get_HitRect()))
							bCollided = true;
					}
					if (!bCollided) {
						if (CMonster::HIT == static_cast<CMonster*>(srcObj)->Get_Monster_State())
							static_cast<CMonster*>(srcObj)->Set_Monster_State(CMonster::IDLE);
					}
				}
			}
		}
	}
	else // 몬스터 총알 -> 플레이어와 검사
	{

	}
}
