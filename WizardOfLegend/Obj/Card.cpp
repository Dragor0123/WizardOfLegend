#include "../stdafx.h"
#include "Card.h"
#include "ObjMgr.h"
#include "Player.h"
#include "PriceTag.h"

CCard::CCard()
	: m_iCardCode(-1), m_iPriceAsGold(0), m_pPriceTag(nullptr)
{
}

CCard::~CCard()
{
	if (m_pPriceTag)
	{
		CObjMgr::Get_Instance()->Delete_A_Obj(OBJID::DECO, m_pPriceTag);
	}
}

void CCard::Set_Price_Tag(int _iGold)
{
	string str;

	if (m_pPriceTag)
	{
		CObjMgr::Get_Instance()->Delete_A_Obj(OBJID::DECO, m_pPriceTag);
		m_pPriceTag = nullptr;
	}

	switch (_iGold)
	{
	case 100:
		str = "PriceTag_100";
		break;
	case 125:
		str = "PriceTag_125";
		break;
	case 150:
		str = "PriceTag_150";
		break;
	}
	m_pPriceTag = CAbstractFactory<CPriceTag>::Create(m_tInfo.fX, m_tInfo.fY + 72.f, str);
	
	CObjMgr::Get_Instance()->Add_Object(OBJID::DECO,
		m_pPriceTag);
}

void CCard::Set_Price_Gold(int _iGold)
{
	m_iPriceAsGold = _iGold;
	Set_Price_Tag(_iGold);
}

bool CCard::Is_Player_Money_Enough(CObj* _pPlayer) const
{
	if (nullptr == _pPlayer)
		return false;

	if (static_cast<CPlayer*>(_pPlayer)->Get_Gold() >= this->m_iPriceAsGold)
		return true;
	else
		return false;
}
