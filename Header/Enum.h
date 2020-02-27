#pragma once

#ifndef __ENUM_H__
#define __ENUM_H__

namespace MOVEDIR
{
	enum FB { MD_BACK = -1, MD_NONE, MD_FRONT};
	enum DIR { MD_LEFT, MD_RIGHT, MD_TOP, MD_BOT, MD_LT, MD_LB, MD_RT, MD_RB, MD_MIDDLE };
}
namespace MEZ
{
	enum STATE {MZ_NONE, MZ_FROZEN, MZ_POISON, MZ_END};
}

namespace OBJID
{
	//UI가 들어갈 수도 있음
	// FABLE을 npc, card(item, arcana), telecircle로 나눠야 함
	enum ID
	{
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
		INVENTORY, MOUSE, END
	};
}

namespace KEYOWN
{
	enum KEYSWITCH { KS_PLAYER, KS_FABLE, KS_TITLEMENU, KS_INVENTORY, KS_INGAMEMENU, KS_END };
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
	enum ID { ID_PLAZA, ID_FIRE, ID_EARTHBOSS_ROOM, ID_FIREBOSS_ROOM, ID_END};
	enum OPTION { OPT_MOVE, OPT_STOP, OPT_DROP, OPT_END };
}

#endif // !__ENUM_H__
