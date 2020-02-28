#pragma once

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
	int Get_Price_Gem()  const { return m_iPriceAsGem; }

	void Set_CardCode(int _c) { m_iCardCode = _c; }

	virtual CArcRel* Create_ArcanaRelic(int _bCondition) = 0;
protected:
	int		m_iPriceAsGold;
	int		m_iPriceAsGem;
	int		m_iCardCode;
};

