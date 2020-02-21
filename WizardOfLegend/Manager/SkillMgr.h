#pragma once

#ifndef __SKILLMGR_H__
#define __SKILLMGR_H__
#include "Singleton.h"

// 플레이어 Arcana 이미지와 몬스터 화살 같은 이미지 bmp파일 추가를 다 여기서 한다.

class CSkillMgr : public Singleton<CSkillMgr>
{
public:
	CSkillMgr();
	virtual ~CSkillMgr();
};

#endif
