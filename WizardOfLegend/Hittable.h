#pragma once

#ifndef __HITTABLE_H__
#define __HITTABLE_H__

class CHittable
{
public:
	CHittable();
	virtual ~CHittable() = 0;

	void Add_Hp(int _iHeal) { m_iHp += _iHeal; if (m_iHp > m_iMaxHp) m_iHp = m_iMaxHp; }
	void Sub_Hp(int _iAtt) { m_iHp -= _iAtt; }

	void Reset_Hp() { m_iHp = m_iMaxHp; }
	int Get_Hp() const { return m_iHp; }
	int Get_MaxHp() const { return m_iHp; }

protected:
	int m_iHp;
	int m_iMaxHp;
};

#endif