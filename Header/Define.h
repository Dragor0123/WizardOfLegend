#pragma once

#ifndef __DEFINE_H__
#define __DEFINE_H__

#define WINCX 1000
#define WINCY 720

//#define LEFT_CAPTION 100
//#define TOP_CAPTION 100
//#define RIGHT_CAPTION 100
//#define BOTTOM_CAPTION 100

//#define HPBARX 460
#define MAGENTA_COLOR RGB(255, 0, 255)
#define OBJ_NOEVENT 0
#define OBJ_DEAD 1

#define ARCRELIC_IDLE		0
#define ARCRELIC_DROPPED	1
#define ARCRELIC_COOLING	2

#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
#define SAFE_DELETE_ARR(p) if(p) { delete[] p; p = nullptr;}

#define PI  3.1415926535897f
#define GRAVITY 9.80665f

// Tile
#define TILECX 64
#define TILECY 64
#define TILE_NUMX 92
#define TILE_NUMY 80


//MONSTER
#define DEFAULT_DETECT_RAD 380.f

//Bullet & Skill
#define G_BULLET_LIFETIME_LIMIT 3.0f

#define HIT_DIGIT_CNT_MAX 6

// OBJECT COLLISION CODE
// 만들어라
// 
// 플레이어 + SWORDMAN, SUMMONBALL과 충돌하면 다침, ARCHER와 SUMMONER, MAGE, BOSS와 충돌하는 건 문제 없음

// PLAYER
// 몬스터 9종 SWORDMAN, ARCHER, SUMMONER, SUMMONERBALL, MAGE, BOSS1, BOSS2, BOSS3, SANDBAG
// Fable류  (NPC, RELIC, ARCANA, TELECIRCLE)
// ORB류 -> 힐링오브, 마나오브
// OBSTACLE류
// COIN 류 -> gold, chaos gem,
// BULLET류
// -> player불릿이냐 아니냐
//    -> 쉴드냐, 아니냐
//    -> player불릿이라면 -> 타일(벽)에 부딪혔을 때 없어지느냐 안 없어지느냐
//	  -> 지금 사용한 스킬의 불릿이 시그니쳐에서 나온 불릿이냐 아니냐 (시그니쳐면 마나 게이지 안 차야함)
//	  -> 몬스터를 밀어버리는 스킬이냐 아니냐
//    -> 몬스터를 끌어당기는 스킬이냐 아니냐
// -> 몬스터 불릿이라면
//						-> 벽에 부딪혔을 때 없어지느냐 안 없어지느냐

#define CC_ONLYTILE_COLLISION				0x00
#define CC_PLAYER							0x80
#define CC_PBULLET_NWALL_NPUSH_DRAG			0x01
#define CC_PBULLET_NWALL_NPUSH_NDRAG		0x02
#define CC_PBULLET_WALL_NPUSH_DRAG			0x03
#define CC_PBULLET_WALL_NPUSH_NDRAG			0x04
#define CC_PBULLET_NWALL_PUSH				0x05
#define CC_PBULLET_WALL_PUSH_DRAG			0x06
#define CC_PBULLET_WALL_PUSH_NDRAG			0x07
#define CC_PSHIELD_NOREFLECT				0x08
#define CC_PSHIELD_REFLECT					0x09
#define CC_MSHIELD_NOREFLECT				0x0A
#define CC_MSHIELD_REFLECT					0x0B
#define CC_MBULLET_NWALL_NPUSH				0x0C
#define CC_MBULLET_NWALL_PUSH				0x0D
#define CC_MBULLET_WALL_NPUSH				0x0E
#define CC_MBULLET_WALL_PUSH				0x0F

#define CC_MONSTER_COLLISION				0x10
#define CC_MONSTER_PUSHABLE					0x20
#define CC_MONSTER_BOSS						0x40

#define CC_GOODS							0x100
#define CC_GGOLD							0x110
#define CC_GCHAOS_GEM						0x120

#define CC_FABLE							0x200
#define CC_FABLE_NPC						0x210
#define CC_FABLE_RELIC						0x220
#define CC_FABLE_ARCANA						0x240
#define CC_FABLE_TELECIRCLE					0x280

#define CC_OBSTACLE							0x400
#define CC_NOHIT_OBS						0x440			// 못 때리는 것
#define CC_NOHIT_BLOCK_OBS					0x420			// 못 때리는 데 길 막하는 것 ex) 철창
#define CC_NOHIT_NOBLOCK_OBS				0x410			// 때리지도 못하고 막지도 않는 것 ex) Portal
#define CC_HITTABLE_OBS						0x480			// 때릴 수 있는 모든 Obstacle 
#define CC_HITTABLE_NOBLOCK_OBS				0x4C0			// 때릴수도 있고, 지나갈수있는(무빙으로 지나가면 알아서 부숴짐)
#define CC_HITTABLE_BLOCK_OBS				0x4E0			// 때릴 수는 있지만 지나갈 수 없는(길막)
#define CC_HITTABLE_BLOCK_LOCK_OBS			0x4F0			// 때릴 수 있고 블락인데, 잠금까지 있음

#define CC_ORB								0x800
#define CC_ORB_HEAL							0x810
#define CC_ORB_MANA							0x820

#endif // !__DEFINE_H__
