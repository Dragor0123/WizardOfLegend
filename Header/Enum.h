#pragma once

#ifndef __ENUM_H__
#define __ENUM_H__

namespace MOVEDIR
{
	enum MD { MD_BACK = -1, MD_NONE, MD_FRONT};
}

namespace OBJID
{
	//UI가 들어갈 수도 있음
	enum ID { PLAYER, MONSTER, BULLET, GOODS, NPC, CARD, ALTAR, ORB, PORTAL, UI, MOUSE, END };
}

namespace TILEENUM
{
	enum ID { ID_FIRE, ID_HOUSE, ID_PLAZA, ID_ICE, ID_END};
	enum OPTION { OPT_MOVE, OPT_STOP, OPT_DROP, OPT_END };
}

#endif // !__ENUM_H__
