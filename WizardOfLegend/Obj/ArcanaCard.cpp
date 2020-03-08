#include "../stdafx.h"
#include "ArcanaCard.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Manager/CardMgr.h"
#include "ObjMgr.h"
#include "ArcRel.h"
#include "ArcGaia.h"
#include "ArcSphere.h"
#include "ArcFrostFan.h"
#include "ArcDragonarc.h"
#include "ArcFireball.h"
#include "ArcNormalAtt.h"
#include "ArcEarthDrill.h"
#include "ArcWindFalcon.h"
#include "Player.h"
#include "NPC.h"
#include "../Manager/SoundMgr.h"

// CArcanaCard �� m_pTarget�� nullptr�� ���� �ְ�, SpellSeller�� ���� �ִ�.
CArcanaCard::CArcanaCard()
	: m_fmoveTime(0.f)
{
}


CArcanaCard::~CArcanaCard()
{
}

bool CArcanaCard::Initialize()
{ // GaiaShieldCard
	if (!CFAble::Initialize())
		return false;

	m_tInfo.iCX = 65;
	m_tInfo.iCY = 98;

	if (m_strFrameKey == "")
		return false;

	if (m_strFrameKey == "GaiaShieldCard") // stage1
		m_iCardCode = 0;
	if (m_strFrameKey == "DragonArcCard") // plaza
		m_iCardCode = 1;
	if (m_strFrameKey == "FireBallCard") // stage1
		m_iCardCode = 2;
	if (m_strFrameKey == "IceSphereCard") // plaza
		m_iCardCode = 3;
	if (m_strFrameKey == "NormalDashCard") // �⺻
		m_iCardCode = 4;
	if (m_strFrameKey == "FrostFanCard") //  stage1
		m_iCardCode = 5;
	if (m_strFrameKey == "NormalAttackCard") //  plaza
		m_iCardCode = 6;
	if (m_strFrameKey == "EarthDrillCard")  // earthboss  ����
		m_iCardCode = 7;
	if (m_strFrameKey == "WindFalconCard") // stage1
		m_iCardCode = 8;


	m_tHitInfo.fX = m_tInfo.fX;
	m_tHitInfo.fY = m_tInfo.fY + 25.f;

	m_tHitInfo.iCX = 76;
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
	return OBJ_NOEVENT;
}

void CArcanaCard::Late_Update(float _fdTime)
{
	CFAble::Key_Check(_fdTime);

	Update_Rect();
	Update_HitRect();
}

void CArcanaCard::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);
	GdiTransparentBlt(_DC
		, (int)(m_tRect.left + _fScrollX)
		, (int)(m_tRect.top + _fScrollY)
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC, 0, 0
		, m_tInfo.iCX, m_tInfo.iCY
		, MAGENTA_COLOR);

	//��Ʈ�ڽ� ���� ����
	Draw_HitBox(_DC, _fScrollX, _fScrollY);
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
	case 1: // "DragonArcCard"
		pArcRel = CAbstractFactory<CArcDragonarc>::Create("DragonArc", pPlayer);
		break;
	case 2: // "FireBallCard"
		pArcRel = CAbstractFactory<CArcFireball>::Create("FireBall", pPlayer);
		break;
	case 3:
		pArcRel = CAbstractFactory<CArcSphere>::Create("IceSphere", pPlayer);
		break;
	case 4:	//�⺻�뽬
		return nullptr;
	case 5: //������ä (FrostFan)
		pArcRel = CAbstractFactory<CArcFrostFan>::Create("FrostFan", pPlayer);
		break;
	case 6: // NormalAttackCard
		pArcRel = CAbstractFactory<CArcNormalAtt>::Create("NormalAttack", pPlayer);
		break;
	case 7: // EarthDrillCard
		pArcRel = CAbstractFactory<CArcEarthDrill>::Create("EarthDrill", pPlayer);
		break;
	case 8: // WindFalconCard
		pArcRel = CAbstractFactory<CArcWindFalcon>::Create("WindFalcon", pPlayer);
		break;
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
	if (m_pTarget && m_iPriceAsGold > 0) //�����ϰ��,
	{
		if (!dynamic_cast<CNPC*>(m_pTarget))
			return;

		if (CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).empty())
			return;

		CObj* pPlayer = CObjMgr::Get_Instance()->Get_listObj(OBJID::PLAYER).front();

		if (Is_Player_Money_Enough(pPlayer))
		{
			CCardMgr::Get_Instance()->Insert_CodeImage35(m_iCardCode);
			CCardMgr::Get_Instance()->Insert_CodeImage46(m_iCardCode);
			int bInsertCondition = CCardMgr::Get_Instance()->Insert_Item_To_Inventory(m_iCardCode);

			if (bInsertCondition)
			{
				bool bSuccess = false;
				CArcRel* pArcRel = Create_ArcanaRelic(bInsertCondition);
				bSuccess = CCardMgr::Get_Instance()->Insert_CodeArcana(m_iCardCode, pArcRel);
				static_cast<CPlayer*>(pPlayer)->Sub_Gold(this->m_iPriceAsGold);
				// m_pTarget�� ǥ�� ������ֱ�.
				static_cast<CNPC*>(m_pTarget)->Set_Emoji_State(CEmoji::ES_YES);
				STOP_SOUND(CSoundMgr::UI);
				STOP_SOUND(CSoundMgr::EFFECT);
				PLAY_SOUND(L"Correct_Sound.wav", CSoundMgr::UI);
				PLAY_SOUND(L"Get_Skill.wav", CSoundMgr::EFFECT);
				m_bDead = true;
			}
			else
			{
				// �κ��丮�� ���� á���ϴ�!
				static_cast<CNPC*>(m_pTarget)->Set_Emoji_State(CEmoji::ES_NO);
				STOP_SOUND(CSoundMgr::EFFECT);
				PLAY_SOUND(L"Error_Sound.wav", CSoundMgr::EFFECT);
			}
		}
		else // �÷��̾��� �ݾ��� ��ǰ�� ���ݺ��� ���� ���
		{
			// �ݾ��� ���ڶ��ϴ�! m_pTarget ǥ�� �����
			static_cast<CNPC*>(m_pTarget)->Set_Emoji_State(CEmoji::ES_NO);
			STOP_SOUND(CSoundMgr::EFFECT);
			PLAY_SOUND(L"Error_Sound.wav", CSoundMgr::EFFECT);
		}
	}
	else //�����ϰ��
	{
		CCardMgr::Get_Instance()->Insert_CodeImage35(m_iCardCode);
		CCardMgr::Get_Instance()->Insert_CodeImage46(m_iCardCode);
		int bInsertCondition = CCardMgr::Get_Instance()->Insert_Item_To_Inventory(m_iCardCode);

		if (bInsertCondition)
		{
			bool bSuccess = false;
			CArcRel* pArcRel = Create_ArcanaRelic(bInsertCondition);
			bSuccess = CCardMgr::Get_Instance()->Insert_CodeArcana(m_iCardCode, pArcRel);
			STOP_SOUND(CSoundMgr::UI);
			STOP_SOUND(CSoundMgr::EFFECT);
			PLAY_SOUND(L"Correct_Sound.wav", CSoundMgr::UI);
			PLAY_SOUND(L"Get_Skill.wav", CSoundMgr::EFFECT);
			m_bDead = true;
		}
		else
		{
			// �κ��丮�� ���� á���ϴ�!
			STOP_SOUND(CSoundMgr::EFFECT);
			PLAY_SOUND(L"Error_Sound.wav", CSoundMgr::EFFECT);
		}
	}
}

void CArcanaCard::Release()
{
}
