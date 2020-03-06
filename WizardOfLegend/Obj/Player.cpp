#include "../stdafx.h"
#include "Player.h"
#include "ObjMgr.h"
#include "../Manager/KeyMgr.h"
#include "../Manager/ScrollMgr.h"
#include "../MyBitmap/BmpMgr.h"
#include "../Manager/CtrlOwnerMgr.h"
#include "../Manager/CardMgr.h"
#include "Inventory.h"
#include "ArcRel.h"
// 아래 헤더 지워 줄것

namespace PLAYER_Space
{
	const DWORD IDLE_FRAME_SPEED = 100;
	const DWORD WALK_FRAME_SPEED = 70;
	const DWORD ATTACK_FRAME_SPEED = 120;
	const DWORD HIT_FRAME_SPEED = 80;
	const DWORD STAMP_FRAME_SPEED = 100;
	const DWORD DEAD_FRAME_SPEED = 900;
	const DWORD FALL_FRAME_SPEED = 80;
	const DWORD REBIRTH_FRAME_SPEED = 100;
	const int	P_HIT_FRAME_COUNTMAX = 2;
}

using namespace PLAYER_Space;

CPlayer::CPlayer()
	: m_bDashInit(false), m_bDontDraw(false),
	m_iSkillCode(-1), m_eRobeColor(ROBE::RED)
{
	ZeroMemory(&m_tBeforeDashPos, sizeof(LINEPOS));
	ZeroMemory(m_tEffectLine, sizeof(m_tEffectLine));
}


CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Set_PlayerState(CPlayer::STATE _eNewState)
{
	if (m_eCurState != CPlayer::DEAD)
		m_eCurState = _eNewState;
}

void CPlayer::Add_MP(int _iVal)
{
	m_iMp += _iVal;
	if (m_iMp > m_iMaxMp)
		m_iMp = m_iMaxMp;
}

bool CPlayer::Initialize()
{
	// 크기와 이동속도, 대시 이동속도를 구함.
	CMoveObj::Initialize();
	m_tInfo.iCX = 176;
	m_tInfo.iCY = 176;
	Equalize_HitPosInfoPos();
	m_tHitInfo.iCX = 70;
	m_tHitInfo.iCY = 128;
	m_fSpeed = 450.f;
	m_fRestoreSpeed = m_fSpeed;
	m_fDashSpeed = m_fSpeed * 2.4f;

	m_iMaxHp = 500;
	m_iHp = m_iMaxHp;
	m_iMaxMp = 200;
	m_iMp = 0;
	m_iGold = 0;
	m_iHitStateCount = 0;
	//
	m_bNormalAttackFire = false;
	// 시그니쳐 관련
	m_bSigniture = false;
	// m_fSigLineLen = 0.f;
	// m_fSigLineAng = 0.f;
	
	// 스프라이트 가져옴
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Player/Up_Red.bmp", "Player_Up"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Player/Down_Red.bmp", "Player_Down"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Player/Left_Red.bmp", "Player_Left"))
		return false;
	if (!CBmpMgr::Get_Instance()->Insert_Bmp(L"Bitmap/Player/Right_Red.bmp", "Player_Right"))
		return false;

	// 기본 프레임 상태 설정
	m_strFrameKey = "Player_Down";
	m_ePreState = END;
	m_eCurState = IDLE;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iFrameScene = 0;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();
	
	m_mapKeyIdx.emplace(VK_LBUTTON, 0);
	m_mapKeyIdx.emplace(VK_SPACE, 1);
	m_mapKeyIdx.emplace(VK_RBUTTON, 2);
	m_mapKeyIdx.emplace('Q', 3);
	m_mapKeyIdx.emplace('E', 4);
	m_mapKeyIdx.emplace('R', 5);

	return true;
}

void CPlayer::Key_Check(float _fdTime)
{
	if (!CCtrlOwnerMgr::Get_Instance()->Is_Key_Available(KEYOWN::KS_PLAYER))
		return;

	if (KEY_DOWN(VK_TAB))
	{
		m_eCurState = IDLE;
		CCtrlOwnerMgr::Get_Instance()->GameObject_Off();
		CCtrlOwnerMgr::Get_Instance()->Inventory_On();
	}

	if (!m_bDashInit && KEY_PRESSING('A'))
	{
		m_strFrameKey = "Player_Left";
		if (KEY_PRESSING('W')) { // 왼쪽 위
			if (KEY_DOWN(VK_SPACE)) {
				// 왼 위 대시 누른 경우
				m_bDashInit = true;
				MoveLeftTop(m_fDashSpeed, _fdTime);
				m_eCurState = DASH;
				m_tFrame.iFrameEnd = 5;
				m_tBeforeDashPos.fX = m_tInfo.fX;
				m_tBeforeDashPos.fY = m_tInfo.fY;
			}
			else if (!KEY_DOWN(VK_SPACE)) {
				MoveLeftTop(_fdTime);
				m_tFrame.iFrameEnd = 3;
				m_eCurState = WALK;
			}

		}
		else if (KEY_PRESSING('S')) { // 왼쪽 아래
			if (KEY_DOWN(VK_SPACE)) {
				// 왼 아래 대쉬 누른 경우
				m_bDashInit = true;
				MoveLeftBottom(m_fDashSpeed, _fdTime);
				m_eCurState = DASH;
				m_tFrame.iFrameEnd = 5;
				m_tBeforeDashPos.fX = m_tInfo.fX;
				m_tBeforeDashPos.fY = m_tInfo.fY;
			}
			else if (!KEY_DOWN(VK_SPACE)) {
				MoveLeftBottom(_fdTime);
				m_tFrame.iFrameEnd = 3;
				m_eCurState = WALK;
			}
		}
		else { // 순수하게 왼쪽
			if (KEY_DOWN(VK_SPACE)) {
				m_bDashInit = true;
				MoveX(m_fDashSpeed, _fdTime, MOVEDIR::MD_BACK);
				m_eCurState = DASH;
				m_tFrame.iFrameEnd = 5;
				m_tBeforeDashPos.fX = m_tInfo.fX;
				m_tBeforeDashPos.fY = m_tInfo.fY;
			}
			else if (!KEY_DOWN(VK_SPACE)) {
				MoveXFromSpeed(_fdTime, MOVEDIR::MD_BACK);
				m_tFrame.iFrameEnd = 3;
				m_eCurState = WALK;
			}
		}
	}
	else if (!m_bDashInit && KEY_PRESSING('D'))
	{
		m_strFrameKey = "Player_Right";
		if (KEY_PRESSING('W')) { // 오른쪽 위
			if (KEY_DOWN(VK_SPACE)) {
				// 오른 위 대시 누른 경우
				m_bDashInit = true;
				MoveRightTop(m_fDashSpeed, _fdTime);
				m_eCurState = DASH;
				m_tFrame.iFrameEnd = 5;
				m_tBeforeDashPos.fX = m_tInfo.fX;
				m_tBeforeDashPos.fY = m_tInfo.fY;
			}
			else if (!KEY_DOWN(VK_SPACE)) {
				MoveRightTop(_fdTime);
				m_tFrame.iFrameEnd = 3;
				m_eCurState = WALK;
			}
		}
		else if (KEY_PRESSING('S')) { // 오른쪽 아래
			if (KEY_DOWN(VK_SPACE)) {
				// 오른 아래 대쉬 누른 경우
				m_bDashInit = true;
				MoveRightBottom(m_fDashSpeed, _fdTime);
				m_eCurState = DASH;
				m_tFrame.iFrameEnd = 5;
				m_tBeforeDashPos.fX = m_tInfo.fX;
				m_tBeforeDashPos.fY = m_tInfo.fY;
			}
			else if (!KEY_DOWN(VK_SPACE)) {
				MoveRightBottom(_fdTime);
				m_tFrame.iFrameEnd = 3;
				m_eCurState = WALK;
			}
		}
		else { // 순수 오른 쪽
			if (KEY_DOWN(VK_SPACE)) {
				m_bDashInit = true;
				MoveX(m_fDashSpeed, _fdTime, MOVEDIR::MD_FRONT);
				m_eCurState = DASH;
				m_tFrame.iFrameEnd = 5;
				m_tBeforeDashPos.fX = m_tInfo.fX;
				m_tBeforeDashPos.fY = m_tInfo.fY;
			}
			else if (!KEY_DOWN(VK_SPACE)) {
				MoveXFromSpeed(_fdTime, MOVEDIR::MD_FRONT);
				m_tFrame.iFrameEnd = 3;
				m_eCurState = WALK;
			}
		}
	}
	else if (!m_bDashInit && KEY_PRESSING('W')) {
		m_strFrameKey = "Player_Up";
		if (KEY_DOWN(VK_SPACE)) {
			m_bDashInit = true;
			MoveY(m_fDashSpeed, _fdTime, MOVEDIR::MD_BACK);
			m_eCurState = DASH;
			m_tFrame.iFrameEnd = 2;
			m_tBeforeDashPos.fX = m_tInfo.fX;
			m_tBeforeDashPos.fY = m_tInfo.fY;
		}
		else if (!KEY_DOWN(VK_SPACE)) {
			MoveYFromSpeed(_fdTime, MOVEDIR::MD_BACK);
			m_tFrame.iFrameEnd = 5;
			m_eCurState = WALK;
		}
	}
	else if (!m_bDashInit && KEY_PRESSING('S')) {
		m_strFrameKey = "Player_Down";
		if (KEY_DOWN(VK_SPACE)) {
			m_bDashInit = true;
			MoveY(m_fDashSpeed, _fdTime, MOVEDIR::MD_FRONT);
			m_eCurState = DASH;
			m_tFrame.iFrameEnd = 3;
			m_tBeforeDashPos.fX = m_tInfo.fX;
			m_tBeforeDashPos.fY = m_tInfo.fY;
		}
		else if (!KEY_DOWN(VK_SPACE)) {
			MoveYFromSpeed(_fdTime, MOVEDIR::MD_FRONT);
			m_tFrame.iFrameEnd = 5;
			m_eCurState = WALK;
		}
	}
	else if (!m_bDashInit && m_eCurState != HIT && m_eCurState != ATTACK && m_eCurState != STAMP) {
		m_eCurState = IDLE;
	}

	// 7~8 : 돈, 9 마나, 0 무적
	if (KEY_DOWN('7'))
	{
		m_iGold = 0;
	}
	if (KEY_DOWN('8'))
	{
		Add_Gold(200);
	}
	if (KEY_DOWN('9'))
	{
		Add_MP(m_iMaxMp);
	}
	if (KEY_DOWN(VK_OEM_COMMA))
	{
		Sub_Hp(100);
	}
	if (KEY_DOWN(VK_OEM_PERIOD))
	{
		Reset_Hp();
	}



	Skill_Button_KeyCheck(VK_LBUTTON);
	Skill_Button_KeyCheck(VK_RBUTTON);
	Skill_Button_KeyCheck('Q');
	Skill_Button_KeyCheck('E');
	Skill_Button_KeyCheck('R');

	Dash_Check(_fdTime);
}

int CPlayer::Update(float _fdTime)
{
	if (OBJ_DEAD == CMoveObj::Update(_fdTime))
		return OBJ_DEAD;
	// 특정 조건 때문에 OBJ_DEAD일경우 그걸 리턴

	Update_ShotAngle();
	Key_Check(_fdTime);
	OffSet();
	Move_Frame();
	Scene_Change();
	Change_HitRect();
	Update_Rect();
	Update_HitRect();

	return OBJ_NOEVENT;
}

void CPlayer::Late_Update(float _fdTime)
{
	if (0 >= m_iHp)
	{
		//m_eCurState = CPlayer::DEAD;
		//m_tHitInfo.iCX = 0;
		//m_tHitInfo.iCY = 0;
	}

	Update_Rect();
	Update_HitRect();

	if (m_iHitDigitCnt > HIT_DIGIT_CNT_MAX || m_ePreState != CPlayer::HIT)
		Reset_HitDigitCnt();

	if (m_bDashInit) {
		float fX = (m_tInfo.fX - m_tBeforeDashPos.fX);
		float fY = (m_tInfo.fY - m_tBeforeDashPos.fY);
		float fDistance = sqrtf(fX * fX + fY * fY);

		if (fDistance > (float)(TILECX * 4.f))
		{
			m_bDashInit = false;
			m_eCurState = IDLE;
		}
	}
}

void CPlayer::Render(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	if (!m_bDontDraw)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_strFrameKey);

		GdiTransparentBlt(_DC,
			(int)(m_tRect.left + _fScrollX),
			(int)(m_tRect.top + _fScrollY),
			m_tInfo.iCX, m_tInfo.iCY,
			hMemDC,
			m_tFrame.iFrameStart * m_tInfo.iCX,
			m_tFrame.iFrameScene * m_tInfo.iCY,
			m_tInfo.iCX, m_tInfo.iCY,
			MAGENTA_COLOR);

		if (m_bSigniture)
		{
			Render_Sig_Effect(_DC, _fdTime, _fScrollX, _fScrollY);
		}

		Draw_HitBox(_DC, _fScrollX, _fScrollY);
	}
	//char strGold[64] = "";
	//sprintf_s(strGold, "gold : %d\n", m_iGold);
	//_cprintf(strGold);
}

void CPlayer::Move_Frame()
{
	static int NormAttFrameRemeber = 0;

	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		//if (m_ePreState != STATE::ATTACK)
		++m_tFrame.iFrameStart;

		m_tFrame.dwFrameTime = GetTickCount();

		if (m_ePreState == STATE::IDLE || m_ePreState == STATE::WALK) {
			m_tFrame.dwFrameSpeed = (m_ePreState == STATE::IDLE) ? IDLE_FRAME_SPEED : WALK_FRAME_SPEED;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd) {
				m_tFrame.iFrameStart = 0;
			}
		}
		if (m_ePreState == STATE::DASH) {
			if (m_tFrame.iFrameStart > 0) {
				m_tFrame.dwFrameSpeed = 50;
			}
			if (m_tFrame.iFrameStart >= m_tFrame.iFrameEnd) {
				m_tFrame.iFrameStart = m_tFrame.iFrameEnd;
			}
		}
		
		// 수정 매우 필요!
		if (m_ePreState == STATE::ATTACK)
		{
			//스킬마다 모션이 다 다르기 때문에.... FrameSpeed를 if나 switch분기로 조건 걸어서 예외처리 다 해줘야된다.
			m_tFrame.dwFrameSpeed = ATTACK_FRAME_SPEED;
			// 3 := IceSphere
			if (3 == m_iSkillCode)
			{
				m_tFrame.iFrameStart = 0;
			}
			else
			{
				if (0 > m_iSkillCode)
				{
					if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
						m_eCurState = STATE::IDLE;
				}
				if (1 == m_iSkillCode)
				{
					if (m_tFrame.iFrameStart >= m_tFrame.iFrameEnd)
						m_tFrame.iFrameStart = m_tFrame.iFrameEnd - 1;
				}
				if (6 == m_iSkillCode)
				{
					if (!m_bNormalAttackFire)
					{
						m_tFrame.dwFrameSpeed = ATTACK_FRAME_SPEED + 20;
						m_tFrame.iFrameStart = rand() % 3 + 1;
						NormAttFrameRemeber = m_tFrame.iFrameStart;
						m_bNormalAttackFire = true;
					}
					else
					{
						m_tFrame.dwFrameSpeed = ATTACK_FRAME_SPEED;
						if (m_tFrame.iFrameStart > NormAttFrameRemeber)
						{
							m_eCurState = STATE::IDLE;
							m_bNormalAttackFire = false;
							m_iSkillCode = -1;
						}
					}
				}

				if (1000 < m_iSkillCode)
				{
					if (m_bSigniture)
					{
						m_tFrame.iFrameStart = 0;
					}
					else
					{
						m_iSkillCode -= 1000;
					}
				}
			}
		}

		if (m_ePreState == CPlayer::HIT)
		{
			if (m_tFrame.iFrameStart == 0)
				m_tFrame.dwFrameSpeed = HIT_FRAME_SPEED;
			else
				m_tFrame.dwFrameSpeed = (DWORD)(HIT_FRAME_SPEED * 1.5f);
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
			{
				++m_iHitStateCount;
				if (m_iHitStateCount > P_HIT_FRAME_COUNTMAX)
				{
					m_eCurState = CPlayer::IDLE;
					m_iHitStateCount = 0;
				}
				else
				{
					m_tFrame.iFrameStart = 0;
				}
			}
		}
		//IT, STAMP, REBIRTH, FALL, DEAD
		if (m_ePreState == CPlayer::STAMP)
		{
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd) {
				m_eCurState = STATE::IDLE;
			}
		}

		if (m_ePreState == CPlayer::REBIRTH)
		{
			m_tFrame.dwFrameSpeed = REBIRTH_FRAME_SPEED;
		}
		// 중요! 중요!
		if (m_ePreState == CPlayer::FALL)
		{
			m_tFrame.dwFrameSpeed = FALL_FRAME_SPEED;
		}

		if (m_ePreState == CPlayer::DEAD)
		{
			m_tFrame.dwFrameSpeed = DEAD_FRAME_SPEED;
			if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd) {
				m_bDead = true;
			}
		}
	}
}

void CPlayer::Skill_Button_KeyCheck(int _iKey)
{
	int idx = m_mapKeyIdx[_iKey];
	CObj* pInven = CObjMgr::Get_Instance()->Get_listObj(OBJID::INVENTORY).front();
	int _code = static_cast<CInventory*>(pInven)->Get_OutterArr_Code(idx);
	if (_code > -1 && _code < 100)
	{
		CArcRel* pArcRel = CCardMgr::Get_Instance()->Find_ArcRel(_code);
		if (_code == 3) // 3 : IceSphere
		{
			if (KEY_PRESSING(_iKey))
			{
				if (pArcRel->Is_Fire_Available())
					pArcRel->Fire_Skill();
			}
			if (KEY_UP(_iKey))
			{
				pArcRel->Key_Up_Action();
			}
		}
		else
		{
			if (KEY_DOWN(_iKey))
			{
				if (pArcRel->Is_Fire_Available())
					pArcRel->Fire_Skill();
			}
		}
	}
}

void CPlayer::Update_ShotAngle()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	pt.x -= (LONG)CScrollMgr::Get_Instance()->Get_ScrollX();
	pt.y -= (LONG)CScrollMgr::Get_Instance()->Get_ScrollY();
	float fDX = 0.f, fDY = 0.f, fRadian = 0.f, fDist = 0.f;
	fDX = (float)pt.x - m_tInfo.fX;
	fDY = (float)pt.y - m_tInfo.fY;
	
	fDist = sqrtf(fDX * fDX + fDY * fDY);

	fRadian = acosf(fDX / fDist);

	// 타겟의 y좌표가 몬스터 y좌표보다 아래에 있다면
	if (m_tInfo.fY < (float)pt.y)   // 0 < fDY
		fRadian = 2 * PI - fRadian; //		fRadian *= -1.f;

	m_fAngle = Radian_To_Degree(fRadian);

}

void CPlayer::Change_HitRect()
{
	if (m_bSigniture)
	{
		m_tHitInfo.iCX = 0;
		m_tHitInfo.iCY = 0;
	}
	else
	{
		if ("Player_Down" == m_strFrameKey)
		{
			switch (m_ePreState)
			{
			case IDLE:
				m_tHitInfo.iCX = 70;
				m_tHitInfo.iCY = 128;
				break;
			case WALK:
				m_tHitInfo.iCX = 80;
				m_tHitInfo.iCY = 114;
				break;
			case DASH:
				m_tHitInfo.iCX = 80;
				m_tHitInfo.iCY = 88;
				if (m_tFrame.iFrameStart >= m_tFrame.iFrameEnd - 1)
					m_tHitInfo.iCY = 116;
				break;
			case ATTACK:
				if (m_tFrame.iFrameStart == 0)
				{
					m_tHitInfo.iCX = 80;
					m_tHitInfo.iCY = 112;
				}
				else if (m_tFrame.iFrameStart == 1)
				{
					m_tHitInfo.iCX = 80;
					m_tHitInfo.iCY = 116;
				}
				else if (m_tFrame.iFrameStart == 2)
				{
					m_tHitInfo.iCX = 68;
					m_tHitInfo.iCY = 108;
				}
				else
				{
					m_tHitInfo.iCX = 88;
					m_tHitInfo.iCY = 112;
				}
				break;
			case HIT:
				m_tHitInfo.iCX = 80;
				m_tHitInfo.iCY = 120;
				break;
			}
		}
		else if ("Player_Up" == m_strFrameKey)
		{
			switch (m_ePreState)
			{
			case IDLE:
				m_tHitInfo.iCX = 70;
				m_tHitInfo.iCY = 128;
				break;
			case WALK:
				m_tHitInfo.iCX = 80;
				m_tHitInfo.iCY = 120;
				break;
			case DASH:
				m_tHitInfo.iCX = 90;
				m_tHitInfo.iCY = 108;
				if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd - 1)
					m_tHitInfo.iCY = 112;
				if (m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
					m_tHitInfo.iCY = 120;
				break;
			case ATTACK:
				if (m_tFrame.iFrameStart == 0)
				{
					m_tHitInfo.iCX = 80;
					m_tHitInfo.iCY = 112;
				}
				else if (m_tFrame.iFrameStart == 1)
				{
					m_tHitInfo.iCX = 84;
					m_tHitInfo.iCY = 116;
				}
				else
				{
					m_tHitInfo.iCX = 88;
					m_tHitInfo.iCY = 116;
				}
				break;
			case HIT:
				m_tHitInfo.iCX = 68;
				m_tHitInfo.iCY = 120;
				break;
			}
		}
		else if ("Player_Left" == m_strFrameKey || "Player_Right" == m_strFrameKey)
		{
			switch (m_ePreState)
			{
			case IDLE:
				m_tHitInfo.iCX = 50;
				m_tHitInfo.iCY = 132;
				break;
			case WALK:
				m_tHitInfo.iCX = 80;
				m_tHitInfo.iCY = 128;
				break;
			case DASH:
				if (m_tFrame.iFrameStart == 0) {
					m_tHitInfo.iCX = 128;
					m_tHitInfo.iCY = 84;
				}
				else {
					m_tHitInfo.iCX = 116;
					m_tHitInfo.iCY = 108;
				}
				break;
			case ATTACK:
				if (m_tFrame.iFrameStart == 0)
				{
					m_tHitInfo.iCX = 108;
					m_tHitInfo.iCY = 112;
				}
				else if (m_tFrame.iFrameStart == 1)
				{
					m_tHitInfo.iCX = 116;
					m_tHitInfo.iCY = 100;
				}
				else if (m_tFrame.iFrameStart == 2)
				{
					m_tHitInfo.iCX = 96;
					m_tHitInfo.iCY = 120;
				}
				else
				{
					m_tHitInfo.iCX = 104;
					m_tHitInfo.iCY = 112;
				}
				break;
			case HIT:
				m_tHitInfo.iCX = 72;
				m_tHitInfo.iCY = 124;
				break;
			}
		}
	}
}

void CPlayer::OffSet()
{
	int	iOffSetX = WINCX >> 1;
	int iOffSetY = WINCY >> 1;

	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	if (iOffSetX > m_tInfo.fX + fScrollX)	// OffsetX 보다 왼쪽으로 갔을 경우
		CScrollMgr::Get_Instance()->Set_ScrollX((float)iOffSetX - (m_tInfo.fX + fScrollX));
	else if (iOffSetX < m_tInfo.fX + fScrollX) // OffsetX 보다 오른쪽으로 갔을 경우
		CScrollMgr::Get_Instance()->Set_ScrollX((float)iOffSetX - (m_tInfo.fX + fScrollX));

	if (iOffSetY > m_tInfo.fY + fScrollY)
		CScrollMgr::Get_Instance()->Set_ScrollY((float)iOffSetY - (m_tInfo.fY + fScrollY));
	else if (iOffSetY < m_tInfo.fY + fScrollY)
		CScrollMgr::Get_Instance()->Set_ScrollY((float)iOffSetY - (m_tInfo.fY + fScrollY));
}

void CPlayer::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CPlayer::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 0;
			m_tFrame.dwFrameSpeed = IDLE_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::WALK:
			m_tFrame.iFrameStart = 0;
			if (m_strFrameKey == "Player_Left" || m_strFrameKey == "Player_Right")
				m_tFrame.iFrameEnd = 3;
			else if (m_strFrameKey == "Player_Up" || m_strFrameKey == "Player_Down")
				m_tFrame.iFrameEnd = 5;
			m_tFrame.iFrameScene = 1;
			m_tFrame.dwFrameSpeed = WALK_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::DASH:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameScene = 2;
			if (m_strFrameKey == "Player_Left" || m_strFrameKey == "Player_Right") {
				m_tFrame.iFrameEnd = 5;
				if (m_tFrame.iFrameStart == 0)
					m_tFrame.dwFrameSpeed = 30 * 4;
			}
			else if (m_strFrameKey == "Player_Up") {
				m_tFrame.iFrameEnd = 2;
				if (m_tFrame.iFrameStart == 0)
					m_tFrame.dwFrameSpeed = 30 * 7;
			}
			else if (m_strFrameKey == "Player_Down") {
				m_tFrame.iFrameEnd = 3;
				if (m_tFrame.iFrameStart == 0)
					m_tFrame.dwFrameSpeed = 30 * 6;
			}
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 3;
			m_tFrame.dwFrameSpeed = ATTACK_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::HIT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iFrameScene = 4;
			m_tFrame.dwFrameSpeed = HIT_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::STAMP:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iFrameScene = 5;
			m_tFrame.dwFrameSpeed = STAMP_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::REBIRTH:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 6;
			m_tFrame.dwFrameSpeed = REBIRTH_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::FALL:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iFrameScene = 7;
			m_tFrame.dwFrameSpeed = FALL_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CPlayer::DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iFrameScene = 8;
			m_tFrame.dwFrameSpeed = DEAD_FRAME_SPEED;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CPlayer::Dash_Check(float _fdTime)
{
	if (m_bDashInit)
	{
		m_eCurState = DASH;
		switch (m_eMoveDir)
		{
		case MOVEDIR::MD_LEFT:
			MoveX(m_fDashSpeed, _fdTime, MOVEDIR::MD_BACK);
			m_tFrame.iFrameEnd = 5;
			break;
		case MOVEDIR::MD_RIGHT:
			MoveX(m_fDashSpeed, _fdTime, MOVEDIR::MD_FRONT);
			m_tFrame.iFrameEnd = 5;
			break;
		case MOVEDIR::MD_TOP:
			MoveY(m_fDashSpeed, _fdTime, MOVEDIR::MD_BACK);
			m_tFrame.iFrameEnd = 2;
			break;
		case MOVEDIR::MD_BOT:
			MoveY(m_fDashSpeed, _fdTime, MOVEDIR::MD_FRONT);
			m_tFrame.iFrameEnd = 3;
			break;
		case MOVEDIR::MD_LT:
			MoveLeftTop(m_fDashSpeed, _fdTime);
			m_tFrame.iFrameEnd = 5;
			break;
		case MOVEDIR::MD_LB:
			MoveLeftBottom(m_fDashSpeed, _fdTime);
			m_tFrame.iFrameEnd = 5;
			break;
		case MOVEDIR::MD_RT:
			MoveRightTop(m_fDashSpeed, _fdTime);
			m_tFrame.iFrameEnd = 5;
			break;
		case MOVEDIR::MD_RB:
			MoveRightBottom(m_fDashSpeed, _fdTime);
			m_tFrame.iFrameEnd = 5;
			break;
		case MOVEDIR::MD_MIDDLE:
			m_eCurState = IDLE;
			m_bDashInit = false;
			break;
		default:
			break;
		}
	}
}

void CPlayer::Render_Sig_Effect(HDC _DC, float _fdTime, float _fScrollX, float _fScrollY)
{
	// 하얀 선 32개를 그린다고 치자.
	float fLineX, fLineY;
	HPEN hMyPen = NULL, hOldPen = NULL;

	for (int i = 0; i < 32; ++i)
	{

		hMyPen = CreatePen(PS_SOLID, m_tEffectLine[i].iWidth, RGB(255, 255, 255));
		fLineX = m_tHitInfo.fX + cosf(m_tEffectLine[i].fRadian) * m_tEffectLine[i].fRange;
		fLineY = m_tHitInfo.fY - sinf(m_tEffectLine[i].fRadian) * m_tEffectLine[i].fRange;
		hOldPen = (HPEN)SelectObject(_DC, hMyPen);
		
		MoveToEx(_DC, (int)(fLineX + _fScrollX), (int)(fLineY + _fScrollY), NULL);
		LineTo(_DC, (int)(m_tHitInfo.fX + _fScrollX), (int)(m_tHitInfo.fY + _fScrollY));
		
		m_tEffectLine[i].fRange -= _fdTime * 115.f;
		if (m_tEffectLine[i].fRange < 0)
			m_tEffectLine[i].fRange = 0.f;

		SelectObject(_DC, hOldPen);
		DeleteObject(hMyPen);
	}
}

void CPlayer::Init_Effect_Line()
{
	std::random_device rd_len;
	std::default_random_engine generator_len(rd_len());
	std::uniform_real_distribution<float> distribution_len(84.f, 282.f);

	std::random_device rd_rad;
	std::default_random_engine generator_rad(rd_rad());
	std::uniform_real_distribution<float> distribution_rad(0.f, 2.f*PI);

	for (int i = 0; i < 32; ++i)
	{
		m_tEffectLine[i].iWidth = rand() % 3 + 1;
		m_tEffectLine[i].fRadian = distribution_rad(generator_rad);
		m_tEffectLine[i].fRange = distribution_len(generator_len);
	}
}

void CPlayer::Release()
{
}