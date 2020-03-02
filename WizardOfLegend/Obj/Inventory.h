#pragma once

#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include "StopObj.h"
#include "UISkillSet.h"

class CInventory : public CStopObj
{
	friend bool CUISkillSet::Initialize();

public:
	CInventory();
	virtual ~CInventory();

	// Inherited via CStopObj
	virtual bool Initialize();
	virtual void Key_Check(float _fdTime);
	virtual int Update(float _fdTime);
	virtual void Late_Update(float _fdTime);
	virtual void Render(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);
	virtual void Release();

public:
	// Save()
	// Load()

	int  Get_OutterArr_Code(int _idx) const { if (_idx >= 0 && (size_t)_idx < m_aOutterArray.size()) return m_aOutterArray[_idx]; else return -1; }
	int  Get_InnerArr_Code(int _idx) const { if (_idx >= 0 && (size_t)_idx < m_aInnerArray.size()) return m_aInnerArray[_idx]; else return -1; };
	bool Is_Outter_Empty();
	bool Is_Outter_Full();
	bool Is_Inner_Empty();
	bool Is_Inner_Full();
	int	 Push_Item_In_OuterArr(int _code);
	int  Push_Item_In_InnerArr(int _code);
private:
	void Pull_InArrayItems();
	void Render_Inventory_Items(HDC _DC, float _fdTime, float _fScrollX = 0.f, float _fScrollY = 0.f);

private:

	array<int, 6>					m_aOutterArray;
	array<int, 12>					m_aInnerArray;
	const float		fXInnerGap;
	const float		fYInnerGap;
	const float		fXOuterGap;
	const int		ROWSIZE;	//InnerArr
	const int		COLSIZE;	//InnerArr
	const int		INVEN_INARR_MAXNUM;
	const int		InnerSlotCX;
	const int		InnerSlotCY;
	const float		fInnerSlot_X_Offset;
	const float		fInnerSlot_Y_Offset;
	LINEPOS			m_tInnerFirstPOS;


	LINEPOS			m_tOutterFirstPOS;
	const int		OutterSlotCX;
	const int		OutterSlotCY;
	const float		fOutterSlot_X_Offset;
	const float		fOutterSlot_Y_Offset;

	int				m_iIdxInner;
	int				m_iIdxOutter;
	int				m_iMouseCnt;
	int				m_iRemberIdx;
	
	const int		m_iSlotChangeCX;
	const int		m_iSlotChangeCY;
	int				m_iLastMouseOnIdx;
	int				m_iClickedIdx;
};

#endif