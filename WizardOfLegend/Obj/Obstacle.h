#pragma once

#ifndef __OBSTACLE_H__
#define __OBSTACLE_H__

#include "StopObj.h"
class CObstacle : public CStopObj
{
public:
	CObstacle();

	virtual ~CObstacle();

private:
	
};

// HitObstacle : Hittable을 상속받음. 때릴 수 있음.
// -> 체스트, 나무, 도자기류...
// NoneHitObstacle : Hittable 상속 안 받음. 절대 못 때림. 충돌판정만함
// -> 동상, 상점테이블

#endif