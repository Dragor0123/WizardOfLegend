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

// HitObstacle : Hittable�� ��ӹ���. ���� �� ����.
// -> ü��Ʈ, ����, ���ڱ��...
// NoneHitObstacle : Hittable ��� �� ����. ���� �� ����. �浹��������
// -> ����, �������̺�

#endif