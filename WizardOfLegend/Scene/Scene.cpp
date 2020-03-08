#include "../stdafx.h"
#include "Scene.h"
#include "../Obj/ObjMgr.h"
#include "../Manager/SoundMgr.h"

CScene::CScene()
{
}

CScene::~CScene()
{
	CSoundMgr::Get_Instance()->StopAll();
}

void CScene::Collision(float _fdTime)
{
	CObjMgr::Get_Instance()->Collision(_fdTime);
}
