#pragma once

#ifndef __GETOBJID_H__
#define __GETOBJID_H__

#include "stdafx.h"
#include "Manager\SceneMgr.h"

inline const OBJID::ID Get_Obj_ID()
{
	OBJID::ID eID;
	switch (CSceneMgr::Get_Instance()->Get_Scene_ID())
	{
	case CSceneMgr::SCENE_PLAZA:
		eID = OBJID::PLAZA_UI;
		break;
	case CSceneMgr::SCENE_STAGE1:
		eID = OBJID::STAGE1_UI;
		break;
	case CSceneMgr::SCENE_EARTHBOSS:
		eID = OBJID::EARTHBSTAGE_UI;
		break;
	case CSceneMgr::SCENE_FIREBOSS:
		eID = OBJID::FIREBSTAGE_UI;
		break;
	default:
		eID = OBJID::END;
		break;
	}
	return eID;
}

#endif