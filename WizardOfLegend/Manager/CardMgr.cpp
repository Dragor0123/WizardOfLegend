#include "../stdafx.h"
#include "CardMgr.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Obj/ArcRel.h"
#include "../Obj/Inventory.h"
#include "../Obj/ObjMgr.h"

CCardMgr::CCardMgr()
{
}


CCardMgr::~CCardMgr()
{
}

bool CCardMgr::Initialize()
{
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/NormalDash_35_35.bmp", "NormalDash_35"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/NormalDash_46_46.bmp", "NormalDash_46"))
		return false;

	//////// 가이아실드 GaiaShieldIcon_35, GaiaShieldIcon_46
	/// 코드넘버 0
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/GaiaShieldCard.bmp", "GaiaShieldCard"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/GaiaShieldIcon_35.bmp", "GaiaShield_35"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/GaiaShieldIcon_46.bmp", "GaiaShield_46"))
		return false;

	/////// 서리부채

	return true;
}

void CCardMgr::Update(float _fdTime)
{
	auto iter = m_mapArcRelic.begin();

	for (; iter != m_mapArcRelic.end();)
	{
		int iEvent = iter->second->Update(_fdTime);
		if (ARCRELIC_DROPPED == iEvent)
		{
			Safe_Delete<CArcRel*>(iter->second);
			iter = m_mapArcRelic.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CCardMgr::Late_Update(float _fdTime)
{
	for (auto& pPair : m_mapArcRelic)
	{
		pPair.second->Late_Update(_fdTime);
	}
}

void CCardMgr::Release()
{
	Safe_Delete_Map(m_mapArcRelic);
}

HDC CCardMgr::Find_Image35(int _code)
{
	auto iter = m_mapCodeImage35.find(_code);
	if (m_mapCodeImage35.end() == iter)
		Insert_CodeImage35(_code);

	switch (_code)
	{
	case 0:
		return CBmpMgr::Get_Instance()->Find_Image("GaiaShield_35");
	case 1:
		return CBmpMgr::Get_Instance()->Find_Image("GaiaShield_35");
	case 2:
		return CBmpMgr::Get_Instance()->Find_Image("GaiaShield_35");
	case 3:
		return CBmpMgr::Get_Instance()->Find_Image("GaiaShield_35");
	case 4:
		return CBmpMgr::Get_Instance()->Find_Image("GaiaShield_35");
	default:
		return NULL;
	}
}

HDC CCardMgr::Find_Image46(int _code)
{
	auto iter = m_mapCodeImage46.find(_code);
	if (m_mapCodeImage46.end() == iter)
		Insert_CodeImage46(_code);

	switch (_code)
	{
	case 0:
		return CBmpMgr::Get_Instance()->Find_Image("GaiaShield_46");
	case 1:
		return CBmpMgr::Get_Instance()->Find_Image("GaiaShield_46");
	case 2:
		return CBmpMgr::Get_Instance()->Find_Image("GaiaShield_46");
	case 3:
		return CBmpMgr::Get_Instance()->Find_Image("GaiaShield_46");
	case 4:
		return CBmpMgr::Get_Instance()->Find_Image("GaiaShield_46");
	default:
		return NULL;
	}
}

CArcRel * CCardMgr::Find_ArcRel(int _iCardCode)
{
	auto iter = m_mapArcRelic.find(_iCardCode);
	if (m_mapArcRelic.end() == iter)
		return nullptr;
	else
		return iter->second;
}

bool CCardMgr::Insert_CodeImage35(int _code)
{
	string s35;
	switch (_code)
	{
	case 0:
		s35 = "GaiaShield_35";
		break;
	case 1:
		break;
	case 2:
		break;
	default:
		break;
	}

	m_mapCodeImage35.emplace(_code, s35);

	return true;
}

bool CCardMgr::Insert_CodeImage46(int _code)
{
	string s46;
	switch (_code)
	{
	case 0:
		s46 = "GaiaShield_46";
		break;
	case 1:
		break;
	case 2:
		break;
	default:
		break;
	}
	m_mapCodeImage46.emplace(_code, s46);
	return true;
}

bool CCardMgr::Insert_CodeArcana(int _code, CArcRel* _Arc)
{
	if (_Arc == nullptr)
		return false;

	m_mapArcRelic.emplace(_code, _Arc);
	return true;
}

int CCardMgr::Insert_Item_To_Inventory(int _code)
{
	assert(!CObjMgr::Get_Instance()->Get_listObj(OBJID::INVENTORY).empty());
	CObj* pInven = CObjMgr::Get_Instance()->Get_listObj(OBJID::INVENTORY).front();

	if (_code < 0)
		return 0;

	if (_code < 100)
	{
		int iRes = static_cast<CInventory*>(pInven)->Push_Item_In_OuterArr(_code);
		return iRes;
	}
	else
	{
		int iRes = static_cast<CInventory*>(pInven)->Push_Item_In_InnerArr(_code);
		return iRes;
	}
}
