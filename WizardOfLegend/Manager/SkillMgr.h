#pragma once

#ifndef __SKILLMGR_H__
#define __SKILLMGR_H__
#include "Singleton.h"

// �÷��̾� Arcana �̹����� ���� ȭ�� ���� �̹��� bmp���� �߰��� �� ���⼭ �Ѵ�.

class CSkillMgr : public Singleton<CSkillMgr>
{
public:
	CSkillMgr();
	virtual ~CSkillMgr();
};

#endif
