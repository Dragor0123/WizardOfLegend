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
	enum ID { PLAYER, MONSTER, BULLET, NPC, CARD, ALTAR, ORB, PORTAL, GOODS, UI, MOUSE, END };
}

namespace TILEENUM
{
	enum ID { ID_PLAZA, ID_FIRE, ID_HOUSE, ID_ICE, ID_END};
	enum OPTION { OPT_MOVE, OPT_STOP, OPT_DROP, OPT_END };
}

#endif // !__ENUM_H__
