#pragma once

#ifndef __ENUM_H__
#define __ENUM_H__

namespace MOVEDIR
{
	enum FB { MD_BACK = -1, MD_NONE, MD_FRONT};
	enum DIR { MD_LEFT, MD_RIGHT, MD_TOP, MD_BOT, MD_LT, MD_LB, MD_RT, MD_RB, MD_MIDDLE };
}

namespace OBJID
{
	//UI가 들어갈 수도 있음
	// FABLE을 npc, card(item, arcana), telecircle로 나눠야 함
	//  
	// PLAYER : 2,
	// MONSTER : 3,
	// BOSS : 4,
	// P_RECTBULLET :5,
	// P_CIRBULLET : 6,
	// M_RECTBULLET : 7,
	// M_CIRBULLET : 8
	enum ID { UNDERDECO, TELECIR,
		PLAYER, MONSTER, BOSS,
		P_RECTBULLET, P_CIRBULLET,
		M_RECTBULLET, M_CIRBULLET,
		NPC, CARD, ORB, PORTAL, GOODS, OBSTACLE,
		MENU_UI, PLAZA_UI, STAGE1_UI, MOUSE, END };
}

namespace COLLISION
{
	enum CODE {};
}

namespace ARCANA
{
	enum TYPE {FIRE, ICE, ELECTRIC, GROUND, WIND, CHAOS, TYPE_END};
	enum CATEGORY {BASIC, STANDARD, SIGNITURE, DASH, CATEGORY_END};
}

namespace TILEENUM
{
	enum ID { ID_PLAZA, ID_FIRE, ID_HOUSE, ID_ICE, ID_END};
	enum OPTION { OPT_MOVE, OPT_STOP, OPT_DROP, OPT_END };
}

#endif // !__ENUM_H__
