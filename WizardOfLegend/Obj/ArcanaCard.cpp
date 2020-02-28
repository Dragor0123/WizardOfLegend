#include "../stdafx.h"
#include "ArcanaCard.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Manager/CardMgr.h"
#include "../Obj/ObjMgr.h"
#include "ArcRel.h"
#include "ArcGaia.h"
#include "ArcSphere.h"

CArcanaCard::CArcanaCard()
	: m_fmoveTime(0.f)
{
}


CArcanaCard::~CArcanaCard()
{
	Release();
}

bool CArcanaCard::Initialize()
{ // GaiaShieldCard
	if (!CFAble::Initialize())
		return false;

	m_tInfo.iCX = 65;
	m_tInfo.iCY = 98;

	if (m_strFrameKey == "")
		return false;

	if (m_strFrameKey == "GaiaShieldCard")
		m_iCardCode = 0;
	if (m_strFrameKey == "DragonArcCard")
		m_iCardCode = 1;
	if (m_strFrameKey == "FireBallCard")
		m_iCardCode = 2;
	if (m_strFrameKey == "IceSphereCard")
		m_iCardCode = 3;
	if (m_strFrameKey == "NormalDashCard")
		m_iCardCode = 4;

	//////// Cardmgr
	//....
	m_tHitInfo.fX = m_tInfo.fX;
	m_tHitInfo.fY = m_tInfo.fY + 25.f;

	m_tHitInfo.iCX = 90;
	m_tHitInfo.iCY = 110;

	m_fSpeed = 25.f;
	m_iDir = -1;

	return true;
}

int CArcanaCard::Update(float _fdTime)
{
	if (OBJ_DEAD == CFAble::Update(_fdTime))
		return OBJ_DEAD;

	m_fmoveTime += _fdTime;
	
	if (m_fmoveTime > 0.30f)
	{
		m_fmoveTime = 0.f;
		m_iDir *= -1;
	}

	MoveY(m_fSpeed, _fdTime, (MOVEDIR::FB)m_iDir);

	Update_Rect();
	Update_HitRect();
	return 0;
}

void CArcanaCard::Late_Update(float _fdTime)
{
	CFAble::Key_Check(_fdTime);

	Update_Rect();
	Update_HitRect();

	int a = 3;
}

void CArcanaCard::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	Update_Rect();
	Update_HitRect();
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
	GdiTransparentBlt(_DC
		, (int)(m_tRect.left + _fScrollX)
		, (int)(m_tRect.top + _fScrollY)
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC, 0, 0
		, m_tInfo.iCX, m_tInfo.iCY
		, MAGENTA_COLOR);

	//히트박스 조건 걸자
	Draw_HitBox(_DC, _fScrollX, _fScrollY);
}

void CArcanaCard::Release()
{
	if (m_pFbutton)
	{
		CObjMgr::Get_Instance()->Delete_A_Obj(m_pFbutton->Get_OBJID(), m_pFbutton);
	}
}

CArcRel * CArcanaCard::Create_ArcanaRelic(int _bCondition)
{
	CArcRel *pArcRel = nullptr;
	if (CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).empty() || _bCondition == 0)
		return nullptr;

	CObj* pPlayer = CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).front();

	switch (m_iCardCode)
	{
	case 0: // Gaia Shield
		pArcRel = CAbstractFactory<CArcGaia>::Create("GaiaShield", pPlayer);
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		pArcRel = CAbstractFactory<CArcSphere>::Create("IceSphere", pPlayer);
		break;
	case 4:	//기본대쉬
		return nullptr;
	}

	if (_bCondition == 1)
		pArcRel->Set_Outter_On();
	else if (_bCondition == 2)
		pArcRel->Set_Inner_On();

	return pArcRel;
}

void CArcanaCard::MoveY(float yourSpeed, float fDeltaTime, MOVEDIR::FB eDir)
{
	m_tInfo.fY += yourSpeed * fDeltaTime * eDir;
	m_tHitInfo.fY += yourSpeed * fDeltaTime * eDir;
}

void CArcanaCard::Do_FButton_Action(float _fdTime)
{
	//CArcRel *pArcRel = Create_ArcanaRelic();
	CCardMgr::Get_Instance()->Insert_CodeImage35(m_iCardCode);
	CCardMgr::Get_Instance()->Insert_CodeImage46(m_iCardCode);
	int bInsertCondition = CCardMgr::Get_Instance()->Insert_Item_To_Inventory(m_iCardCode);

	if (bInsertCondition)
	{
		bool bSuccess = false;
		CArcRel* pArcRel = Create_ArcanaRelic(bInsertCondition);
		bSuccess = CCardMgr::Get_Instance()->Insert_CodeArcana(m_iCardCode, pArcRel);
	}

	m_bDead = true;
}
