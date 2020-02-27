#pragma once

#ifndef __HITTABLE_H__
#define __HITTABLE_H__

class CHittable
{
public:
	CHittable();
	virtual ~CHittable() = 0;

	// SETTER

	void Add_Hp(int _iHeal) { m_iHp += _iHeal; if (m_iHp > m_iMaxHp) m_iHp = m_iMaxHp; }
	void Sub_Hp(int _iAtt) { m_iHp -= _iAtt; }
	void Reset_Hp() { m_iHp = m_iMaxHp; }
	void Set_MezState(MEZ::STATE _eMez)
	{ 
		m_eMez = _eMez; 
		if (_eMez == MEZ::MZ_END)
			m_eMez = MEZ::MZ_NONE;
	}

	// GETTER

	MEZ::STATE Get_MezState() { return m_eMez; }
	int Get_Hp() const { return m_iHp; }
	int Get_MaxHp() const { return m_iMaxHp; }
	float	Get_Cur_HP_Ratio() const { return (float)m_iHp / m_iMaxHp;}
protected:
	int m_iHp;
	int m_iMaxHp;
	MEZ::STATE m_eMez;
};

#endif