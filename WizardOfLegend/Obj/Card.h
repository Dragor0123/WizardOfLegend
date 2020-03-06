#pragma once

#ifndef __CARD_H__
#define __CARD_H__

#include "FAble.h"

class CArcRel;
class CCard : public CFAble
{
public:
	CCard();
	virtual ~CCard();

	// Inherited via CFAble
	virtual void Do_FButton_Action(float _fdTime) = 0;

public:
	int Get_CardCode() const { return m_iCardCode; }
	int Get_Price_Gold() const { return m_iPriceAsGold; }
	
	void Set_Price_Tag(int _iGold);
	void Set_Price_Gold(int _iGold);
	void Set_CardCode(int _c) { m_iCardCode = _c; }
	virtual CArcRel* Create_ArcanaRelic(int _bCondition) = 0;

protected:
	bool Is_Player_Money_Enough(CObj* _pPlayer) const;

protected:
	int		m_iPriceAsGold;
	//int		m_iPriceAsGem;
	int		m_iCardCode;

	CObj*	m_pPriceTag;
};

#endif