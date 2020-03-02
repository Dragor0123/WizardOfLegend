#include "../stdafx.h"
#include "CtrlOwnerMgr.h"

CCtrlOwnerMgr::CCtrlOwnerMgr()
{
}

CCtrlOwnerMgr::~CCtrlOwnerMgr()
{
	Release();
}

bool CCtrlOwnerMgr::Initialize()
{
	memset(m_bKeyOnOffArr, 0, sizeof(m_bKeyOnOffArr));
	m_bKeyOnOffArr[KEYOWN::KS_TITLEMENU] = true;
	return true;
}

void CCtrlOwnerMgr::Release()
{
}

void CCtrlOwnerMgr::Set_All_Off()
{
	for (int i = 0; i < KEYOWN::KS_END; ++i)
		m_bKeyOnOffArr[i] = false;
}

void CCtrlOwnerMgr::Inventory_On()
{
	if (m_bKeyOnOffArr[KEYOWN::KS_FABLE])
		m_bKeyOnOffArr[KEYOWN::KS_FABLE] = false;
	if (m_bKeyOnOffArr[KEYOWN::KS_PLAYER])
		m_bKeyOnOffArr[KEYOWN::KS_PLAYER] = false;
	if (m_bKeyOnOffArr[KEYOWN::KS_WARDINVEN])
		m_bKeyOnOffArr[KEYOWN::KS_WARDINVEN] = false;

	Set_On(KEYOWN::KS_INVENTORY);
}

void CCtrlOwnerMgr::Inventory_Off()
{
	Set_Off(KEYOWN::KS_INVENTORY);
	Set_On(KEYOWN::KS_PLAYER);
	Set_On(KEYOWN::KS_FABLE);
}

void CCtrlOwnerMgr::InGameMenu_On()
{
	Set_All_Off();
	Set_On(KEYOWN::KS_INGAMEMENU);
}

void CCtrlOwnerMgr::InGameMenu_Off(KEYOWN::KEYSWITCH _idx)
{
	Set_Off(KEYOWN::KS_INGAMEMENU);
	Set_On(_idx);
}

void CCtrlOwnerMgr::TitleMenu_On()
{
	Set_All_Off();
	Set_On(KEYOWN::KS_TITLEMENU);
}

void CCtrlOwnerMgr::TitleMenu_Off()
{
	Set_Off(KEYOWN::KS_TITLEMENU);
}

void CCtrlOwnerMgr::GameObject_On()
{
	if (m_bKeyOnOffArr[KEYOWN::KS_TITLEMENU])
		Set_Off(KEYOWN::KS_TITLEMENU);
	Set_On(KEYOWN::KS_PLAYER);
	Set_On(KEYOWN::KS_FABLE);
}

void CCtrlOwnerMgr::GameObject_Off()
{
	Set_Off(KEYOWN::KS_PLAYER);
	Set_Off(KEYOWN::KS_FABLE);
}

void CCtrlOwnerMgr::WardInven_Off()
{
	Set_Off(KEYOWN::KS_WARDINVEN);
	Set_On(KEYOWN::KS_PLAYER);
	Set_On(KEYOWN::KS_FABLE);
}

void CCtrlOwnerMgr::WardInven_On()
{
	if (m_bKeyOnOffArr[KEYOWN::KS_FABLE])
		Set_Off(KEYOWN::KS_FABLE);
	if (m_bKeyOnOffArr[KEYOWN::KS_PLAYER])
		Set_Off(KEYOWN::KS_PLAYER);
	if (m_bKeyOnOffArr[KEYOWN::KS_INVENTORY])
		Set_Off(KEYOWN::KS_INVENTORY);

	Set_On(KEYOWN::KS_WARDINVEN);
}
