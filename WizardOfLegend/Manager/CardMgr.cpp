#include "../stdafx.h"
#include "CardMgr.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Obj/ArcRel.h"
#include "../Obj/Inventory.h"
#include "../Obj/ObjMgr.h"
#include "../Manager/SceneMgr.h"
#include "../GetObjID.h"
#include "../Obj/UISkillSet.h"

CCardMgr::CCardMgr()
{
}


CCardMgr::~CCardMgr()
{
}

bool CCardMgr::Initialize()
{
	//////// 가이아실드 GaiaShieldIcon_35, GaiaShieldIcon_46
	/// 코드넘버 0
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/GaiaShieldCard.bmp", "GaiaShieldCard"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/GaiaShieldIcon_35.bmp", "GaiaShield_35"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/GaiaShieldIcon_46.bmp", "GaiaShield_46"))
		return false;

	// 코드 넘버 1 : 드래곤아크
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/DragonArcCard.bmp", "DragonArcCard"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/DragonArc_35.bmp", "DragonArc_35"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/DragonArc_46.bmp", "DragonArc_46"))
		return false;

	// 코드 넘버 2 : 파이어볼
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/FireBallCard.bmp", "FireBallCard"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/FireBall_35.bmp", "FireBall_35"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/FireBall_46.bmp", "FireBall_46"))
		return false;

	///////코드넘버 3: 아이스 스피어 
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/IceSphereCard.bmp", "IceSphereCard"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/IceSphereIcon_35.bmp", "IceSphere_35"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/IceSphereIcon_46.bmp", "IceSphere_46"))
		return false;

	//// 코드넘버 4: 노말 대시
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/NormalDash_35_35.bmp", "NormalDash_35"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/NormalDash_46_46.bmp", "NormalDash_46"))
		return false;
	// 노말 대시
	CCardMgr::Get_Instance()->Insert_CodeImage35(4);
	CCardMgr::Get_Instance()->Insert_CodeImage46(4);

	// 코드 넘버 5: FrostFan
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/FrostFanCard.bmp", "FrostFanCard"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/FrostFanIcon_35.bmp", "FrostFan_35"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/FrostFanIcon_46.bmp", "FrostFan_46"))
		return false;

	// 코드 넘버 6: NormalAttackCard 
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/NormalAttackCard.bmp", "NormalAttackCard"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/NormalAttack_35.bmp", "NormalAttack_35"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/NormalAttack_46.bmp", "NormalAttack_46"))
		return false;

	// 코드 넘버 7: EarthDrill
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/EarthDrillCard.bmp", "EarthDrillCard"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/EarthDrill_35.bmp", "EarthDrill_35"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/EarthDrill_46.bmp", "EarthDrill_46"))
		return false;

	// 코드 넘버 8: WindFalcon
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/WindFalconCard.bmp", "WindFalconCard"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/WindFalcon_35.bmp", "WindFalcon_35"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/CardIcon/WindFalcon_46.bmp", "WindFalcon_46"))
		return false;

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
			OBJID::ID eID = Get_Obj_ID();
			CObj* pUISkillSet = nullptr;
			list<CObj*> _list = CObjMgr::Get_Instance()->Get_listObj(eID);
			list<CObj*>::iterator iterBeg = _list.begin();
			for (; iterBeg != _list.end(); ++iterBeg)
			{
				if ("UI_SKILLSET" == (*iterBeg)->Get_FrameKey())
				{
					pUISkillSet = *iterBeg;
					break;
				}
			}

			if (nullptr == pUISkillSet)
			{
				++iter;
				continue;
			}
			else
			{
				if (ARCRELIC_COOLING == iEvent)
				{
					CObj* pInven = CObjMgr::Get_Instance()->Get_listObj(OBJID::INVENTORY).front();
					int _idxOutter = static_cast<CInventory*>(pInven)->Find_Code_In_Outter(iter->first);
					
					if (_idxOutter != -1)
						static_cast<CUISkillSet*>(pUISkillSet)->Set_CoolingArr(_idxOutter);	
				}
				else if (ARCRELIC_IDLE == iEvent)
				{
					CObj* pInven = CObjMgr::Get_Instance()->Get_listObj(OBJID::INVENTORY).front();
					int _idxOutter = static_cast<CInventory*>(pInven)->Find_Code_In_Outter(iter->first);

					if (_idxOutter != -1)
						static_cast<CUISkillSet*>(pUISkillSet)->Clear_CoolingArr(_idxOutter);
				}
				++iter;
			}
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
		return CBmpMgr::Get_Instance()->Find_Image("DragonArc_35");
	case 2:
		return CBmpMgr::Get_Instance()->Find_Image("FireBall_35");
	case 3:
		return CBmpMgr::Get_Instance()->Find_Image("IceSphere_35");
	case 4:
		return CBmpMgr::Get_Instance()->Find_Image("NormalDash_35");
	case 5:
		return CBmpMgr::Get_Instance()->Find_Image("FrostFan_35");
	case 6:
		return CBmpMgr::Get_Instance()->Find_Image("NormalAttack_35");
	case 7:
		return CBmpMgr::Get_Instance()->Find_Image("EarthDrill_35");
	case 8:
		return CBmpMgr::Get_Instance()->Find_Image("WindFalcon_35");
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
		return CBmpMgr::Get_Instance()->Find_Image("DragonArc_46");
	case 2:
		return CBmpMgr::Get_Instance()->Find_Image("FireBall_46");
	case 3:
		return CBmpMgr::Get_Instance()->Find_Image("IceSphere_46");
	case 4:
		return CBmpMgr::Get_Instance()->Find_Image("NormalDash_46");
	case 5:
		return CBmpMgr::Get_Instance()->Find_Image("FrostFan_46");
	case 6:
		return CBmpMgr::Get_Instance()->Find_Image("NormalAttack_46");
	case 7:
		return CBmpMgr::Get_Instance()->Find_Image("EarthDrill_46");
	case 8:
		return CBmpMgr::Get_Instance()->Find_Image("WindFalcon_46");
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
		s35 = "DragonArc_35";
		break;
	case 2:
		s35 = "FireBall_35";
		break;
	case 3:
		s35 = "IceSphere_35";
		break;
	case 4:
		s35 = "NormalDash_35";
		break;
	case 5:
		s35 = "FrostFan_35";
		break;
	case 6:
		s35 = "NormalAttack_35";
		break;
	case 7:
		s35 = "EarthDrill_35";
		break;
	case 8:
		s35 = "WindFalcon_35";
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
		s46 = "DragonArc_46";
		break;
	case 2:
		s46 = "FireBall_46";
		break;
	case 3:
		s46 = "IceSphere_46";
		break;
	case 4:
		s46 = "NormalDash_46";
		break;
	case 5:
		s46 = "FrostFan_46";
		break;
	case 6:
		s46 = "NormalAttack_46";
		break;
	case 7:
		s46 = "EarthDrill_46";
		break;
	case 8:
		s46 = "WindFalcon_46";
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

// return 0: 인벤토리에 삽입 실패, 1 : Outter에 들어갔음, 2: Inner에 들어갔음
int CCardMgr::Insert_Item_To_Inventory(int _code)
{
	// 예외처리 해줘야한다. _code에 해당하는 카드가 이미 Inventory에 (Inner에 있건, Outter에 있건)
	// 들어갔다면, 못 집어넣게 해줘야한다.
#ifdef _DEBUG
	assert(!CObjMgr::Get_Instance()->Get_listObj(OBJID::INVENTORY).empty());
#endif
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
