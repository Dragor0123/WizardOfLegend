#include "../stdafx.h"
#include "Scene.h"
#include "../Obj/ObjMgr.h"
CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::Collision(float _fdTime)
{
	CObjMgr::Get_Instance()->Collision(_fdTime);
}
