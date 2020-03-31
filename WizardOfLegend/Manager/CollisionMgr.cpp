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
#include "../Obj/SummonerBall.h"
#include "../Obj/FrostFan.h"
#include "../Obj/FireBoss.h"
#include "../Obj/Meteor.h"
#include "../Obj/EarthDrill.h"
#include "../Obj/WindFalcon.h"
#include "../Obj/NoMoveBullet.h"
#include "../Obj/FireKick.h"
#include "DigitMgr.h"
#include "SoundMgr.h"

#define INC_MP 8

CCollisionMgr::CCollisionMgr()
{
}

CCollisionMgr::~CCollisionMgr()
{
}

// _Dst : 무조건 총알만 쉴드도 제외, _Src에는 플레이어, 몬스터, Obstacle등... 맞는 놈.
void CCollisionMgr::Collision_CircleRect(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	if (_Src.empty() || _Dst.empty())
		return;

	int _DstGroupCode = _Dst.front()->Get_Collision_Code();
	int _SrcGroupCode = _Src.front()->Get_Collision_Code();

	if (!(_DstGroupCode & 0x0F))
		return;

	if (_DstGroupCode >= 0x08 && _DstGroupCode <= 0x0B)
		return;

	if (0x01 <= _DstGroupCode && _DstGroupCode < CC_PSHIELD_NOREFLECT) // PLAYER BULLET
	{

		for (auto& dstObj : _Dst)
		{
			if (static_cast<CBullet*>(dstObj)->Get_Collision())
				continue;
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
								if (CollisionRectPush(srcObj, dstObj, &fX, &fY)) // 이거 원 + 사각형 충돌 밀어버리는 건 없나?
								{
									int iDamage = static_cast<CBullet*>(dstObj)->Get_Att();
									static_cast<CMonster*>(srcObj)->Sub_Hp(iDamage);
									MAKE_DIGIT(iDamage, srcObj, DIGIT::DC_WHITE)
									static_cast<CMonster*>(srcObj)->Inc_HitDigitCnt();
									static_cast<CBoss*>(srcObj)->Set_Boss_State(7);

									STOP_SOUND(CSoundMgr::MONSTER);
									if (dynamic_cast<CFireBoss*>(srcObj))
										PLAY_SOUND(L"FireBossHurt.wav", CSoundMgr::MONSTER);
									else
										PLAY_SOUND(L"EarthBossHurt.wav", CSoundMgr::MONSTER);

									Add_MP_Logic(dstObj);
									static_cast<CBullet*>(dstObj)->Set_Collision(true);

									if (Collision_Obj_Tile(srcObj, &fX, &fY))
									{
										CollisionRectPush(dstObj, srcObj, &fX, &fY);
									}

									if (Collision_Obj_Obstacle(srcObj, &fX, &fY))
									{
										CollisionRectPush(dstObj, srcObj, &fX, &fY);
									}
								}
							}
							else if (CC_PBULLET_NWALL_NPUSH_DRAG <= dstObjCode && dstObjCode < CC_PBULLET_NWALL_PUSH) // 밀지 않는 총알.
							{
								if (Check_CircleRect(dstObj, srcObj))
								{
									int iDamage = static_cast<CBullet*>(dstObj)->Get_Att();
									static_cast<CMonster*>(srcObj)->Sub_Hp(iDamage);
									MAKE_DIGIT(iDamage, srcObj, DIGIT::DC_WHITE);
									static_cast<CMonster*>(srcObj)->Inc_HitDigitCnt();
									static_cast<CBoss*>(srcObj)->Set_Boss_State(7);

									STOP_SOUND(CSoundMgr::MONSTER);
									if (dynamic_cast<CFireBoss*>(srcObj))
										PLAY_SOUND(L"FireBossHurt.wav", CSoundMgr::MONSTER);
									else
										PLAY_SOUND(L"EarthBossHurt.wav", CSoundMgr::MONSTER);
									Add_MP_Logic(dstObj);
									static_cast<CBullet*>(dstObj)->Set_Collision(true);
								}
							}
						}
						else // 보스가 Hittable이 아닌 경우
						{
							if (dstObj->Get_Collision_Code() < CC_PSHIELD_NOREFLECT) {
								if (Check_CircleRect(dstObj, srcObj))
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
								int iDamage = static_cast<CBullet*>(dstObj)->Get_Att();
								static_cast<CMonster*>(srcObj)->Sub_Hp(iDamage);
								MAKE_DIGIT(iDamage, srcObj, DIGIT::DC_WHITE)
								static_cast<CMonster*>(srcObj)->Inc_HitDigitCnt();
								static_cast<CMonster*>(srcObj)->Set_Monster_State(CMonster::HIT);
								if (!dynamic_cast<CSummonerBall*>(srcObj))
								{
									STOP_SOUND(CSoundMgr::MONSTER);
									Play_Enemy_Hit_Rand();
								}
								Add_MP_Logic(dstObj);
								static_cast<CBullet*>(dstObj)->Set_Collision(true);

								if (Collision_Obj_Tile(srcObj, &fX, &fY)) //srcObj(몬스터)가 타일에 부딪혔을 경우
								{
									CollisionRectPush(dstObj, srcObj, &fX, &fY);
								}
								if (Collision_Obj_Obstacle(srcObj, &fX, &fY))
								{
									CollisionRectPush(dstObj, srcObj, &fX, &fY);
								}
							}
						}
						else //충돌 시 밀어버리지 않는 총알.
						{
							if (Check_CircleRect(dstObj, srcObj))
							{
								int iDamage = static_cast<CBullet*>(dstObj)->Get_Att();
								static_cast<CMonster*>(srcObj)->Sub_Hp(iDamage);
								MAKE_DIGIT(iDamage, srcObj, DIGIT::DC_WHITE)
								static_cast<CMonster*>(srcObj)->Inc_HitDigitCnt();
								static_cast<CMonster*>(srcObj)->Set_Monster_State(CMonster::HIT);
								if (!dynamic_cast<CSummonerBall*>(srcObj))
								{
									STOP_SOUND(CSoundMgr::MONSTER);
									Play_Enemy_Hit_Rand();
								}
								Add_MP_Logic(dstObj);
								static_cast<CBullet*>(dstObj)->Set_Collision(true);
							}
						}
					}
				}
			}// monster if문 end
			else if (!(_SrcGroupCode & 0x800) && (_SrcGroupCode >> 10 & 0x1)) // CC_OBSTACLE 0x400
			{ // 플레이어 총알과 장애물 검사
				float fX = 0.f, fY = 0.f;
				for (auto& srcObj : _Src)
				{
					if (srcObj->Get_Collision_Code() & 0x080)				// HITTABLE류 장애물들
					{
						// 나중에
					}
					else
					{
						if (CC_NOHIT_BLOCK_OBS == srcObj->Get_Collision_Code())
						{
							if (CC_PBULLET_NWALL_PUSH != dstObj->Get_Collision_Code() &&
								(dstObj->Get_Collision_Code() >= CC_PBULLET_WALL_NPUSH_DRAG &&
								dstObj->Get_Collision_Code() < CC_PSHIELD_NOREFLECT))
							{
								if (Check_CircleRect(dstObj, srcObj))
									static_cast<CBullet*>(dstObj)->Set_Collision(true);
							}
						}
						else
						{  
							// CC_NOHIT_NOBLOCK_OBS || CC_NOHIT_OBS. 근데 CC_NOHIT_OBS를 리턴하는 애는 없을 거임..
						   // 높은 확률로 포탈임.
						}
					}
				}
			}
		} // 		for (auto& dstObj : _Dst) 종료
	}
	else // 적 총알과 플레이어 충돌.
	{
		for (auto& dstObj : _Dst)
		{
			if (static_cast<CBullet*>(dstObj)->Get_Collision())
				continue;
			// 먼저 플레이어와 충돌부터 시킬것!
			if (_SrcGroupCode == 0x80)
			{
				for (auto& srcObj : _Src)
				{
					float fX = 0.f, fY = 0.f;
					int dstObjCode = dstObj->Get_Collision_Code();
					if (dstObjCode == CC_MBULLET_NWALL_NPUSH || dstObjCode == CC_MBULLET_WALL_NPUSH)  // 밀지 않는 총알
					{
						if (Check_CircleRect(dstObj, srcObj))
						{
							if (!g_bPlayerNoDie)
							{
								int iDamage = static_cast<CBullet*>(dstObj)->Get_Att();
								static_cast<CPlayer*>(srcObj)->Sub_Hp(iDamage);
								MAKE_DIGIT(iDamage, srcObj, DIGIT::DC_WHITE)
									static_cast<CPlayer*>(srcObj)->Inc_HitDigitCnt();
								static_cast<CPlayer*>(srcObj)->Set_PlayerState(CPlayer::HIT);
								STOP_SOUND(CSoundMgr::PLAYER);
								PLAY_SOUND(L"Player_Hit.wav", CSoundMgr::PLAYER);
							}
							static_cast<CBullet*>(dstObj)->Set_Collision(true);
						}
					}
					// 이것도 원과 사각형이 부딪혔을 경우 밀어내는 함수 생각해봐야함.
					else if (dstObjCode == CC_MBULLET_NWALL_PUSH || dstObjCode == CC_MBULLET_WALL_PUSH)
					{
						if (CollisionRectPush(srcObj, dstObj, &fX, &fY))
						{
							if (!g_bPlayerNoDie)
							{
								int iDamage = static_cast<CBullet*>(dstObj)->Get_Att();
								static_cast<CPlayer*>(srcObj)->Sub_Hp(iDamage);
								MAKE_DIGIT(iDamage, srcObj, DIGIT::DC_WHITE)
									static_cast<CPlayer*>(srcObj)->Inc_HitDigitCnt();
								static_cast<CPlayer*>(srcObj)->Set_PlayerState(CPlayer::HIT);
								STOP_SOUND(CSoundMgr::PLAYER);
								PLAY_SOUND(L"Player_Hit.wav", CSoundMgr::PLAYER);
							}

							Collision_Obj_Tile(srcObj, &fX, &fY);
							Collision_Obj_Obstacle(srcObj, &fX, &fY);
							static_cast<CBullet*>(dstObj)->Set_Collision(true);
						}
					}
				}
			}
			else if (!(_SrcGroupCode & 0x800) && (_SrcGroupCode >> 10 & 0x1))
			{
				//몹의 총알 또는 쉴드가 Obstacle류와 부딪힌 경우...
				float fX = 0.f, fY = 0.f;
				for (auto& srcObj : _Src)
				{
					if (!(srcObj->Get_Collision_Code() & 0x080)) //NOHITTABLE류 장애물
					{
						if (CC_NOHIT_BLOCK_OBS == srcObj->Get_Collision_Code())
						{
							if (CC_MBULLET_WALL_NPUSH == dstObj->Get_Collision_Code() ||
								CC_MBULLET_WALL_PUSH == dstObj->Get_Collision_Code())
							{
								if (Check_CircleRect(dstObj, srcObj))
									static_cast<CBullet*>(dstObj)->Set_Collision(true);
							}
						}
					}
				}
			}
			else if (CC_PSHIELD_NOREFLECT == _SrcGroupCode || CC_PSHIELD_REFLECT == _SrcGroupCode)
			{
				for (auto& srcObj : _Src)
				{
					if (dynamic_cast<CMeteor*>(dstObj) || dynamic_cast<CFireKick*>(dstObj))
						break;
					else
					{
						if (Check_CircleRect(dstObj, srcObj))
							static_cast<CBullet*>(dstObj)->Set_Collision(true);
					}
				}
			}
		}
	}
}

//  _Dst : 주체, _Src : 대상(맞는놈)
void CCollisionMgr::Collision_Rect(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	RECT rc = {};

	if (_Src.empty() || _Dst.empty())
		return;

	int _DstGroupCode = _Dst.front()->Get_Collision_Code();
	int _SrcGroupCode = _Src.front()->Get_Collision_Code();

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
				// 플레이어 + 보스 밀기 충돌 다시 고려해보자... 보스가 공격 중일경우는 플레이어가 못 밀게 고쳐야함.
				if (_SrcGroupCode & CC_MONSTER_BOSS) // 보스일경우
				{
					for (auto& srcObj : _Src)
					{
						float fX = 0.f, fY = 0.f;
						if (dynamic_cast<CFireBoss*>(srcObj))
						{
							if (static_cast<CFireBoss*>(srcObj)->Get_bJump())
								continue;
						}
						
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
							if (Collision_Obj_Obstacle(srcObj, &fX, &fY))
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
				}
				else //일반몹일경우
				{
					for (auto& srcObj : _Src)
					{
						float fX = 0.f, fY = 0.f;
						if (srcObj->Get_Collision_Code() == CC_MONSTER_PUSHABLE) // 밀 수 있는 몹이라면?
						{
							if (MEZ::MZ_FROZEN == dynamic_cast<CMonster*>(srcObj)->Get_MezState())
							{
								if (CollisionRectPush(dstObj, srcObj, &fX, &fY))
								{
									if (Collision_Obj_Tile(dstObj, &fX, &fY))
									{
										CollisionRectPush(srcObj, dstObj, &fX, &fY);
									}
									if (Collision_Obj_Obstacle(dstObj, &fX, &fY))
									{
										CollisionRectPush(srcObj, dstObj, &fX, &fY);
									}
								}
							}
							else 
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
									if (Collision_Obj_Obstacle(srcObj, &fX, &fY))
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
						}
						else // 밀 수 있는 몹이 아니라면?
						{	
							if (dynamic_cast<CSummonerBall*>(srcObj))
							{
								if (static_cast<CSummonerBall*>(srcObj)->Get_Monster_State() != CMonster::ATTACK)
									continue;
								else
								{
									if (IntersectRect(&rc, &dstObj->Get_HitRect(), &srcObj->Get_HitRect()))
									{
										if (!g_bPlayerNoDie)
										{
											int iDamage = static_cast<CSummonerBall*>(srcObj)->Get_Att();
											static_cast<CPlayer*>(dstObj)->Sub_Hp(iDamage);
											MAKE_DIGIT(iDamage, dstObj, DIGIT::DC_WHITE);
											static_cast<CPlayer*>(dstObj)->Inc_HitDigitCnt();
											STOP_SOUND(CSoundMgr::PLAYER);
											PLAY_SOUND(L"Player_Hit.wav", CSoundMgr::PLAYER);
											static_cast<CPlayer*>(dstObj)->Set_PlayerState(CPlayer::HIT);
										}
									}
								}
							}
							else if (IntersectRect(&rc, &dstObj->Get_HitRect(), &srcObj->Get_HitRect()))
							{
								if (!g_bPlayerNoDie)
								{
									int iDamage = rand() % 6 + 6;
									static_cast<CPlayer*>(dstObj)->Sub_Hp(iDamage);
									MAKE_DIGIT(iDamage, dstObj, DIGIT::DC_WHITE);
									static_cast<CPlayer*>(dstObj)->Inc_HitDigitCnt();
									STOP_SOUND(CSoundMgr::PLAYER);
									PLAY_SOUND(L"Player_Hit.wav", CSoundMgr::PLAYER);
									static_cast<CPlayer*>(dstObj)->Set_PlayerState(CPlayer::HIT);
								}
							}
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
							 STOP_SOUND(CSoundMgr::EFFECT);
							 CSoundMgr::Get_Instance()->PlaySound(L"Coin.wav", CSoundMgr::EFFECT);
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
								//Collision_Obj_Tile(dstObj, &fX, &fY);
								if (dynamic_cast<CPlayer*>(dstObj))
								{
									if (CPlayer::DASH == static_cast<CPlayer*>(dstObj)->Get_PlayerState())
										static_cast<CPlayer*>(dstObj)->Dash_Off();
								}
							}
						}
						else if (CC_HITTABLE_NOBLOCK_OBS == srcObj->Get_Collision_Code() || CC_HITTABLE_OBS == srcObj->Get_Collision_Code())
						{
							// 상황 따라 다른데...
						}
					}
					else // NO_HITTABLE
					{   // 철창과 같은 못 때리고 길막하는 장애물들! -> 벽 타일 취급해야한다.
						if (CC_NOHIT_BLOCK_OBS == srcObj->Get_Collision_Code())
						{
							if (CollisionRectPush(dstObj, srcObj, &fX, &fY))
							{
								if (dynamic_cast<CPlayer*>(dstObj))
								{
									if (CPlayer::DASH == static_cast<CPlayer*>(dstObj)->Get_PlayerState())
										static_cast<CPlayer*>(dstObj)->Dash_Off();
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
				if (dynamic_cast<CShield*>(dstObj))
				{ // 플레이어 쉴드와 적 총알 충돌검사.
					for (auto& srcObj : _Src)
					{
						if (srcObj->Get_Collision_Code() >= CC_MBULLET_NWALL_NPUSH &&
							srcObj->Get_Collision_Code() <= CC_MBULLET_WALL_PUSH)
						{
							if (dynamic_cast<CMeeleBullet*>(srcObj))
								continue;
							if (dynamic_cast<CNoMoveBullet*>(srcObj))
								continue;
							if (IntersectRect(&rc, &dstObj->Get_HitRect(), &srcObj->Get_HitRect()))
							{
								static_cast<CBullet*>(srcObj)->Set_Collision(true);
							}
						}
					}
				}
				if (static_cast<CBullet*>(dstObj)->Get_Collision() && !dynamic_cast<CEarthDrill*>(dstObj))
					continue;
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
											int iDamage = static_cast<CBullet*>(dstObj)->Get_Att();
											static_cast<CMonster*>(srcObj)->Sub_Hp(iDamage);
											MAKE_DIGIT(iDamage, srcObj, DIGIT::DC_WHITE);
											static_cast<CMonster*>(srcObj)->Inc_HitDigitCnt();
											static_cast<CBoss*>(srcObj)->Set_Boss_State(7);
											STOP_SOUND(CSoundMgr::MONSTER);
											if (dynamic_cast<CFireBoss*>(srcObj))
												PLAY_SOUND(L"FireBossHurt.wav", CSoundMgr::MONSTER);
											else
												PLAY_SOUND(L"EarthBossHurt.wav", CSoundMgr::MONSTER);

											Add_MP_Logic(dstObj);

											static_cast<CBullet*>(dstObj)->Set_Collision(true);
										}
									}
									else if (CollisionRectPush(srcObj, dstObj, &fX, &fY, 2.0f))
									{
										int iDamage = static_cast<CBullet*>(dstObj)->Get_Att();
										static_cast<CMonster*>(srcObj)->Sub_Hp(iDamage);
										MAKE_DIGIT(iDamage, srcObj, DIGIT::DC_WHITE);
										static_cast<CMonster*>(srcObj)->Inc_HitDigitCnt();
										static_cast<CBoss*>(srcObj)->Set_Boss_State(7);
										STOP_SOUND(CSoundMgr::MONSTER);
										if (dynamic_cast<CFireBoss*>(srcObj))
											PLAY_SOUND(L"FireBossHurt.wav", CSoundMgr::MONSTER);
										else
											PLAY_SOUND(L"EarthBossHurt.wav", CSoundMgr::MONSTER);

										Add_MP_Logic(dstObj);

										static_cast<CBullet*>(dstObj)->Set_Collision(true);
										if (Collision_Obj_Tile(srcObj, &fX, &fY))
										{
											CollisionRectPush(dstObj, srcObj, &fX, &fY);
										}

										if (Collision_Obj_Obstacle(srcObj, &fX, &fY))
										{
											CollisionRectPush(dstObj, srcObj, &fX, &fY);
										}
									}
								}
								else if (CC_PBULLET_NWALL_NPUSH_DRAG <= dstObjCode && dstObjCode < CC_PBULLET_NWALL_PUSH) // 밀지 않는 총알.
								{
									// 여기에 WindFalcon 예외처리.
									if (IntersectRect(&rc, &dstObj->Get_HitRect(), &srcObj->Get_HitRect()))
									{
										int iDamage = static_cast<CBullet*>(dstObj)->Get_Att();
										static_cast<CMonster*>(srcObj)->Sub_Hp(iDamage);
										MAKE_DIGIT(iDamage, srcObj, DIGIT::DC_WHITE)
										static_cast<CMonster*>(srcObj)->Inc_HitDigitCnt();
										
										static_cast<CBoss*>(srcObj)->Set_Boss_State(7);
										STOP_SOUND(CSoundMgr::MONSTER);
										if (dynamic_cast<CFireBoss*>(srcObj))
											PLAY_SOUND(L"FireBossHurt.wav", CSoundMgr::MONSTER);
										else
											PLAY_SOUND(L"EarthBossHurt.wav", CSoundMgr::MONSTER);

										Add_MP_Logic(dstObj);
										static_cast<CBullet*>(dstObj)->Set_Collision(true);
									}
								}
								else if (dynamic_cast<CShield*>(dstObj))
								{
									if (IntersectRect(&rc, &dstObj->Get_HitRect(), &srcObj->Get_HitRect()))
									{
										int iDamage = static_cast<CShield*>(dstObj)->Get_Att();
										static_cast<CMonster*>(srcObj)->Sub_Hp(iDamage);
										MAKE_DIGIT(iDamage, srcObj, DIGIT::DC_WHITE)
										static_cast<CMonster*>(srcObj)->Inc_HitDigitCnt();
										// MP 차게 하나?
										static_cast<CBoss*>(srcObj)->Set_Boss_State(7);
										STOP_SOUND(CSoundMgr::MONSTER);
										if (dynamic_cast<CFireBoss*>(srcObj))
											PLAY_SOUND(L"FireBossHurt.wav", CSoundMgr::MONSTER);
										else
											PLAY_SOUND(L"EarthBossHurt.wav", CSoundMgr::MONSTER);
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
								CIceSphere* bIceSphere = dynamic_cast<CIceSphere*>(dstObj);
								if (bIceSphere)
								{
									if (Check_RectRect(dstObj, srcObj, &fX, &fY))
									{
										float fRadian = Degree_To_Radian(dstObj->Get_Angle());
										float fDiagonal = 0.f;
										if (fX > fY)
										{
											if (dstObj->Get_HitInfo().fY < srcObj->Get_HitInfo().fY)
											{ // 총알이 대상보다 더 위에 있다면
												fDiagonal = abs((1.f / sinf(fRadian)) * fY);  // | cosec(fRadian) * fY |
												fX = fDiagonal * cosf(fRadian);
												srcObj->Set_PosX(fX);
												srcObj->Set_PosY(fY);
											}
											else 
											{ // 총알이 대상보다 더 아래에 있다면
												fDiagonal = abs((1.f / sinf(fRadian)) * fY); // | cosec(fRadian) * fY |
												fX = fDiagonal * cosf(fRadian);
												srcObj->Set_PosX(fX);
												srcObj->Set_PosY(-fY);
											}
										}
										else // fX <= fY
										{
											if (dstObj->Get_HitInfo().fX < srcObj->Get_HitInfo().fX)
											{	// 총알이 대상보다 더 왼쪽에 있다.
												fDiagonal = abs((1.f / cosf(fRadian)) * fX); // | sec(fRadian) * fX |
												fY = fDiagonal * sinf(fRadian);
												srcObj->Set_PosX(fX);
												srcObj->Set_PosY(-fY);
											}
											else
											{	// 총알이 대상보다 더 오른쪽에 있다.
												fDiagonal = abs((1.f / cosf(fRadian)) * fX); // | sec(fRadian) * fX |
												fY = fDiagonal * sinf(fRadian);
												srcObj->Set_PosX(-fX);
												srcObj->Set_PosY(-fY);
											}
										}

										int iDamage = static_cast<CBullet*>(dstObj)->Get_Att();
										static_cast<CMonster*>(srcObj)->Sub_Hp(iDamage);
										MAKE_DIGIT(iDamage, srcObj, DIGIT::DC_WHITE)
										static_cast<CMonster*>(srcObj)->Inc_HitDigitCnt();
										static_cast<CMonster*>(srcObj)->Set_Monster_State(CMonster::HIT);
										if (iDamage != 0) {
											STOP_SOUND(CSoundMgr::MONSTER);
											Play_Enemy_Hit_Rand();
											Add_MP_Logic(dstObj);
										}
										static_cast<CBullet*>(dstObj)->Set_Att(0);

										if (Collision_Obj_Tile(srcObj, &fX, &fY))
										{
											static_cast<CIceSphere*>(dstObj)->Set_bMonsterWall(true);
											CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT,
												CAbstractFactory<CIcicleEffect>::Create(srcObj));
											static_cast<CBullet*>(dstObj)->Set_Collision(true);
										}

										if (Collision_Obj_Obstacle(srcObj, &fX, &fY))
										{
											static_cast<CIceSphere*>(dstObj)->Set_bMonsterWall(true);
											CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT,
												CAbstractFactory<CIcicleEffect>::Create(srcObj));
											static_cast<CBullet*>(dstObj)->Set_Collision(true);
										}
									}
								}
								else if(CollisionRectPush(srcObj, dstObj, &fX, &fY, 2.0f))
								{
									int iDamage = static_cast<CBullet*>(dstObj)->Get_Att();
									static_cast<CMonster*>(srcObj)->Sub_Hp(iDamage);
									MAKE_DIGIT(iDamage, srcObj, DIGIT::DC_WHITE);
									static_cast<CMonster*>(srcObj)->Inc_HitDigitCnt();
									static_cast<CMonster*>(srcObj)->Set_Monster_State(CMonster::HIT);
									if (!dynamic_cast<CSummonerBall*>(srcObj))
									{
										STOP_SOUND(CSoundMgr::MONSTER);
										Play_Enemy_Hit_Rand();
									}
									Add_MP_Logic(dstObj);
									static_cast<CBullet*>(dstObj)->Set_Collision(true);

									if (Collision_Obj_Tile(srcObj, &fX, &fY)) //srcObj(몬스터)가 타일에 부딪혔을 경우
									{
										CollisionRectPush(dstObj, srcObj, &fX, &fY);
									}
									if (Collision_Obj_Obstacle(srcObj, &fX, &fY))
									{
										CollisionRectPush(dstObj, srcObj, &fX, &fY);
									}
								}
							}
							else //충돌 시 밀어버리지 않는 총알.
							{
								// WindFalcon 예외처리
								if (dynamic_cast<CWindFalcon*>(dstObj))
								{
									bool _bAttackOn = static_cast<CWindFalcon*>(dstObj)->Get_bAttackOn();
									if (_bAttackOn)
									{
										if (IntersectRect(&rc, &dstObj->Get_HitRect(), &srcObj->Get_HitRect()))
										{
											int iDamage = static_cast<CBullet*>(dstObj)->Get_Att();
											static_cast<CMonster*>(srcObj)->Sub_Hp(iDamage);
											MAKE_DIGIT(iDamage, srcObj, DIGIT::DC_WHITE);
											static_cast<CMonster*>(srcObj)->Inc_HitDigitCnt();
											static_cast<CMonster*>(srcObj)->Set_Monster_State(CMonster::HIT);
											if (!dynamic_cast<CSummonerBall*>(srcObj))
											{
												STOP_SOUND(CSoundMgr::MONSTER);
												Play_Enemy_Hit_Rand();
											}
											Add_MP_Logic(dstObj);
											static_cast<CWindFalcon*>(dstObj)->Inc_TargetChangeCnt();
											
											static_cast<CWindFalcon*>(dstObj)->Set_bAttackOn(false);
											if ( (size_t)static_cast<CWindFalcon*>(dstObj)->Get_TargetChangeCnt() 
												>= CWindFalcon::iTARGET_CHANGE_MAX)
											{
												static_cast<CBullet*>(dstObj)->Set_Collision(true);
											}
										}
									}
									else // bAttackOn == false
									{
										bool _bNoHit = true;
										for (auto& monster : CObjMgr::Get_Instance()->Get_listObj(OBJID::MONSTER))
										{
											if (IntersectRect(&rc, &dstObj->Get_HitRect(), &monster->Get_HitRect()))
											{
												_bNoHit = false;
												break;
											}
										}

										if (_bNoHit)
										{
											static_cast<CWindFalcon*>(dstObj)->Set_bAttackOn(true);
										}
									}
								}
								else if (dynamic_cast<CBullet*>(dstObj))
								{
									if (IntersectRect(&rc, &dstObj->Get_HitRect(), &srcObj->Get_HitRect()))
									{
										int iDamage = static_cast<CBullet*>(dstObj)->Get_Att();
										static_cast<CMonster*>(srcObj)->Sub_Hp(iDamage);
										MAKE_DIGIT(iDamage, srcObj, DIGIT::DC_WHITE)
										static_cast<CMonster*>(srcObj)->Inc_HitDigitCnt();
										static_cast<CMonster*>(srcObj)->Set_Monster_State(CMonster::HIT);
										if (!dynamic_cast<CSummonerBall*>(srcObj))
										{
											STOP_SOUND(CSoundMgr::MONSTER);
											Play_Enemy_Hit_Rand();
										}
										Add_MP_Logic(dstObj);
										if (dynamic_cast<CFrostFan*>(dstObj))
										{
											if (static_cast<CFrostFan*>(dstObj)->Is_Making_Ice())
											{
												CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT,
													CAbstractFactory<CIcicleEffect>::Create(srcObj));
											}
										}
										static_cast<CBullet*>(dstObj)->Set_Collision(true);
									}
								}
								else if (dynamic_cast<CShield*>(dstObj))
								{
									if (IntersectRect(&rc, &dstObj->Get_HitRect(), &srcObj->Get_HitRect()))
									{
										int iDamage = static_cast<CShield*>(dstObj)->Get_Att();
										static_cast<CMonster*>(srcObj)->Sub_Hp(iDamage);
										MAKE_DIGIT(iDamage, srcObj, DIGIT::DC_WHITE)
										static_cast<CMonster*>(srcObj)->Inc_HitDigitCnt();
										static_cast<CMonster*>(srcObj)->Set_Monster_State(CMonster::HIT);
										if (!dynamic_cast<CSummonerBall*>(srcObj))
										{
											STOP_SOUND(CSoundMgr::MONSTER);
											Play_Enemy_Hit_Rand();
										}
									}
								}
							}
						}
					}
				}// monster if문 end
				else if (!(_SrcGroupCode & 0x800) && (_SrcGroupCode >> 10 & 0x1)) // CC_OBSTACLE 0x400
				{ // 플레이어 총알&쉴드와 장애물 검사
					float fX = 0.f, fY = 0.f;
					for (auto& srcObj : _Src)
					{
						if (srcObj->Get_Collision_Code() & 0x080)				// HITTABLE류 장애물들
						{
							// 나중에
						}
						else
						{
							if (CC_NOHIT_BLOCK_OBS == srcObj->Get_Collision_Code())
							{
								if (CC_PBULLET_NWALL_PUSH != dstObj->Get_Collision_Code() &&
									(dstObj->Get_Collision_Code() >= CC_PBULLET_WALL_NPUSH_DRAG &&
									dstObj->Get_Collision_Code() < CC_PSHIELD_NOREFLECT))
								{
									if (IntersectRect(&rc, &dstObj->Get_HitRect(), &srcObj->Get_HitRect()))
										static_cast<CBullet*>(dstObj)->Set_Collision(true);
								}
							}
							else
							{  // CC_NOHIT_NOBLOCK_OBS || CC_NOHIT_OBS. 근데 CC_NOHIT_OBS를 리턴하는 애는 없을 거임..
								// 높은 확률로 포탈임.

							}
						}
					}
				}
			}			//for (auto& dstObj : _Dst)
		}// 플레이어 총알 & 플레이어 쉴드 if문 끝! /// 0x01 <= _DstGroupCode && _DstGroupCode <= CC_PSHIELD_REFLECT

		else // 모든 MONSTER BULLET OR MONSTER SHIELD
		{
			for (auto& dstObj : _Dst)
			{
				if (static_cast<CBullet*>(dstObj)->Get_Collision() && !dynamic_cast<CEarthDrill*>(dstObj))
					continue;
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
								if (!g_bPlayerNoDie)
								{
									int iDamage = static_cast<CBullet*>(dstObj)->Get_Att();
									static_cast<CPlayer*>(srcObj)->Sub_Hp(iDamage);
									MAKE_DIGIT(iDamage, srcObj, DIGIT::DC_WHITE)
										static_cast<CPlayer*>(srcObj)->Inc_HitDigitCnt();

									static_cast<CPlayer*>(srcObj)->Set_PlayerState(CPlayer::HIT);
									STOP_SOUND(CSoundMgr::PLAYER);
									PLAY_SOUND(L"Player_Hit.wav", CSoundMgr::PLAYER);
								}
								static_cast<CBullet*>(dstObj)->Set_Collision(true);
							}
						}
						else if (dstObjCode == CC_MBULLET_NWALL_PUSH || dstObjCode == CC_MBULLET_WALL_PUSH)
						{
							if (CollisionRectPush(dstObj, srcObj, &fX, &fY, 2.0f))
							{
								if (!g_bPlayerNoDie)
								{
									int iDamage = static_cast<CBullet*>(dstObj)->Get_Att();
									static_cast<CPlayer*>(srcObj)->Sub_Hp(iDamage);
									MAKE_DIGIT(iDamage, srcObj, DIGIT::DC_WHITE)
										static_cast<CPlayer*>(srcObj)->Inc_HitDigitCnt();
									static_cast<CPlayer*>(srcObj)->Set_PlayerState(CPlayer::HIT);
									STOP_SOUND(CSoundMgr::PLAYER);
									PLAY_SOUND(L"Player_Hit.wav", CSoundMgr::PLAYER);
								}
								Collision_Obj_Tile(srcObj, &fX, &fY);
								Collision_Obj_Obstacle(srcObj, &fX, &fY);
								static_cast<CBullet*>(dstObj)->Set_Collision(true);
							}
						}
						else if (dstObjCode == CC_MSHIELD_REFLECT || dstObjCode == CC_MSHIELD_NOREFLECT)
						{
							if (dynamic_cast<CShield*>(dstObj))
							{
								if (IntersectRect(&rc, &dstObj->Get_HitRect(), &srcObj->Get_HitRect()))
								{
									if (!g_bPlayerNoDie)
									{
										int iDamage = static_cast<CShield*>(dstObj)->Get_Att();
										static_cast<CPlayer*>(srcObj)->Sub_Hp(iDamage);
										MAKE_DIGIT(iDamage, srcObj, DIGIT::DC_WHITE);
										static_cast<CPlayer*>(srcObj)->Inc_HitDigitCnt();
										STOP_SOUND(CSoundMgr::PLAYER);
										PLAY_SOUND(L"Player_Hit.wav", CSoundMgr::PLAYER);
										static_cast<CPlayer*>(srcObj)->Set_PlayerState(CPlayer::HIT);
									}
								}
							}
						}
					}
				}
				else if (!(_SrcGroupCode & 0x800) && (_SrcGroupCode >> 10 & 0x1))
				{
					//몹의 총알 또는 쉴드가 Obstacle류와 부딪힌 경우...
					float fX = 0.f, fY = 0.f;
					for (auto& srcObj : _Src)
					{
						if (!(srcObj->Get_Collision_Code() & 0x080)) //NOHITTABLE류 장애물
						{
							if (CC_NOHIT_BLOCK_OBS == srcObj->Get_Collision_Code())
							{
								if (CC_MBULLET_WALL_NPUSH == dstObj->Get_Collision_Code() ||
									CC_MBULLET_WALL_PUSH == dstObj->Get_Collision_Code())
								{
									if (IntersectRect(&rc, &dstObj->Get_HitRect(), &srcObj->Get_HitRect()))
										static_cast<CBullet*>(dstObj)->Set_Collision(true);
								}
							}
						}
					}
				}
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
				Collision_Obj_Obstacle(dstObj, &fX, &fY);
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

void CCollisionMgr::Play_Enemy_Hit_Rand()
{
	int i = rand() % 3;
	switch (i)
	{
	case 0:
		PLAY_SOUND(L"EnemyHurt0.wav", CSoundMgr::MONSTER);
		break;
	case 1:
		PLAY_SOUND(L"EnemyHurt1.wav", CSoundMgr::MONSTER);
		break;
	case 2:
		PLAY_SOUND(L"EnemyHurt2.wav", CSoundMgr::MONSTER);
		break;
	}
}

void CCollisionMgr::Add_MP_Logic(CObj* _dstObj)
{
	bool bSig = static_cast<CBullet*>(_dstObj)->Is_Signiture_On();
	if (!bSig)
	{
		if (!CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).empty())
		{
			CObj* pPlayer = CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).front();
			static_cast<CPlayer*>(pPlayer)->Add_MP(INC_MP);
		}
	}
}

// 첫번째 인자가 밀린다.
bool CCollisionMgr::CollisionRectPush(CObj* _Pushee, CObj* _Pusher, float* _pfX, float* _pfY, float _pushScale)
{
	if (Check_RectRect(_Pusher, _Pushee, _pfX, _pfY))
	{
		if (*_pfX > *_pfY)
		{ // 상하 충돌
			if (_Pusher->Get_HitInfo().fY < _Pushee->Get_HitInfo().fY)
				_Pushee->Set_PosY((*_pfY) * _pushScale);
			else
				_Pushee->Set_PosY(-(*_pfY) * _pushScale);
		}
		else
		{	// 좌우 충돌 (*_pfX < *_pfY)
			if (_Pusher->Get_HitInfo().fX < _Pushee->Get_HitInfo().fX)
				_Pushee->Set_PosX( (*_pfX) * _pushScale);
			else
				_Pushee->Set_PosX(-(*_pfX) * _pushScale);
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
					{
						int code = _pObj->Get_Collision_Code();
						if (CC_PBULLET_NWALL_NPUSH_DRAG == code ||
							CC_PBULLET_NWALL_NPUSH_NDRAG == code ||
							CC_PBULLET_NWALL_PUSH == code ||
							CC_MBULLET_NWALL_NPUSH == code ||
							CC_MBULLET_NWALL_PUSH == code)
							return false;
						else
							static_cast<CBullet*>(_pObj)->Set_Collision(true);
					}
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

						if (dynamic_cast<CSummonerBall*>(_pObj))
						{
							if (static_cast<CSummonerBall*>(_pObj)->Get_Monster_State() == CMonster::ATTACK)
							{
								static_cast<CSummonerBall*>(_pObj)->Set_Monster_State(CMonster::IDLE);
								static_cast<CSummonerBall*>(_pObj)->Set_bAttack_Cool(false);
							}
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

						if (dynamic_cast<CBullet*>(dstObj))
						{
							int code = dstObj->Get_Collision_Code();
							if (CC_PBULLET_NWALL_NPUSH_DRAG == code ||
								CC_PBULLET_NWALL_NPUSH_NDRAG == code ||
								CC_PBULLET_NWALL_PUSH == code ||
								CC_MBULLET_NWALL_NPUSH == code ||
								CC_MBULLET_NWALL_PUSH == code)
								continue;
							else
								static_cast<CBullet*>(dstObj)->Set_Collision(true);
						}
						if (dynamic_cast<CSummonerBall*>(dstObj))
						{
							if (static_cast<CSummonerBall*>(dstObj)->Get_Monster_State() == CMonster::ATTACK)
							{
								static_cast<CSummonerBall*>(dstObj)->Set_Monster_State(CMonster::IDLE);
								static_cast<CSummonerBall*>(dstObj)->Set_bAttack_Cool(false);
							}
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

bool CCollisionMgr::Collision_Obj_Obstacle(CObj * _pObj, float * _fPushedX, float * _fPushedY)
{
	const list<CObj*>& _rListObstacle = CObjMgr::Get_Instance()->Get_listObj(OBJID::OBSTACLE);
	for (auto& obs : _rListObstacle)
	{
		int _obsCode = obs->Get_Collision_Code();
		int _code = _pObj->Get_Collision_Code();
		if ((_obsCode == CC_NOHIT_BLOCK_OBS) ||
			(_obsCode == CC_HITTABLE_BLOCK_OBS) ||
			(_obsCode == CC_HITTABLE_BLOCK_LOCK_OBS))
		{
			if (Check_RectRect(obs, _pObj, _fPushedX, _fPushedY))
			{
				if (dynamic_cast<CShield*>(_pObj))
				{
					continue;
				}
				if (0x0F & _code)
				{
					if ((_code < CC_PBULLET_WALL_NPUSH_DRAG) ||
						(CC_PBULLET_NWALL_PUSH == _code) ||
						(CC_MBULLET_NWALL_NPUSH == _code) ||
						(CC_MBULLET_NWALL_PUSH == _code))
					{
						continue;
					}
					else
					{
						static_cast<CBullet*>(_pObj)->Set_Collision(true);
					}
				}
				else
				{
					if (*_fPushedX > *_fPushedY)
					{ // 상하 충돌
						if (obs->Get_HitInfo().fY < _pObj->Get_HitInfo().fY)
							_pObj->Set_PosY(*_fPushedY);
						else
							_pObj->Set_PosY(-*_fPushedY);
					}
					else
					{	// 좌우 충돌 (*_pfX < *_pfY)
						if (obs->Get_HitInfo().fX < _pObj->Get_HitInfo().fX)
							_pObj->Set_PosX(*_fPushedX);
						else
							_pObj->Set_PosX(-*_fPushedX);
					}

					if (dynamic_cast<CSummonerBall*>(_pObj))
					{
						if (static_cast<CSummonerBall*>(_pObj)->Get_Monster_State() == CMonster::ATTACK)
						{
							static_cast<CSummonerBall*>(_pObj)->Set_Monster_State(CMonster::IDLE);
							static_cast<CSummonerBall*>(_pObj)->Set_bAttack_Cool(false);
						}
					}

					if (_code == CC_PLAYER)
					{
						if (CPlayer::DASH == static_cast<CPlayer*>(_pObj)->Get_PlayerState())
							static_cast<CPlayer*>(_pObj)->Dash_Off();
					}
				}
				return true;
			}
		}
	}
	return false;
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

bool CCollisionMgr::Check_CircleRect(const RECT * _lhs, CObj * _rect)
{
	LONG _cX = LONG((_lhs->left + _lhs->right) >> 1);
	LONG _cY = LONG((_lhs->top + _lhs->bottom) >> 1);
	LONG _radius = LONG((_lhs->right - _lhs->left) >> 1);

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

bool CCollisionMgr::CollisionDiagRectPush(CObj * _Pushee, CObj * _Pusher, float * _pfX, float * _pfY, float _pushScale)
{
	if (Check_RectRect(_Pusher, _Pushee, _pfX, _pfY))
	{
		float fRadian = Degree_To_Radian(_Pusher->Get_Angle());
		float fDiagonal = 0.f;
		if (*_pfX > *_pfY)
		{
			if (fRadian >= PI && fRadian < 2.f * PI || fRadian < 0.f && fRadian >= -PI)
			{ // 총알이 대상보다 더 위에 있다면
				fDiagonal = abs((1.f / sinf(fRadian)) * (*_pfY));
				*_pfX = fDiagonal * cosf(fRadian);
				_Pushee->Set_PosX((*_pfX) * (_pushScale));
				_Pushee->Set_PosY((*_pfY) * (_pushScale));
			}
			else
			{ // 총알이 대상보다 더 아래에 있다면
				fDiagonal = abs((1.f / sinf(fRadian)) * (*_pfY));
				*_pfX = fDiagonal * cosf(fRadian);
				_Pushee->Set_PosX((*_pfX) * (_pushScale));
				_Pushee->Set_PosY(-(*_pfY) * (_pushScale));
			}
		}
		else // fX <= fY
		{
			if (fRadian >= 0.f && fRadian < PI * 0.5f || fRadian >= 1.5f * PI && fRadian < 2.f * PI
				|| fRadian < 0.f && fRadian >= -0.5f * PI)
			{	// 총알이 대상보다 더 왼쪽에 있다.
				fDiagonal = abs((1.f / cosf(fRadian)) * (*_pfX));
				*_pfY = fDiagonal * sinf(fRadian);
				_Pushee->Set_PosX((*_pfX) * (_pushScale));
				_Pushee->Set_PosY(-(*_pfY) * (_pushScale));
			}
			else
			{	// 총알이 대상보다 더 오른쪽에 있다.
				fDiagonal = abs((1.f / cosf(fRadian)) * (*_pfX));
				*_pfY = fDiagonal * sinf(fRadian);
				_Pushee->Set_PosX(-(*_pfX) * (_pushScale));
				_Pushee->Set_PosY(-(*_pfY) * (_pushScale));
			}
		}
		return true;
	}
	return false;
}

// 충돌이 끝났는지 검사. Dst는 무조건 총알