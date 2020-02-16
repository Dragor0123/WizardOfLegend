#pragma once

#ifndef __REF_H__
#define __REF_H__

class CRef
{
public:
	CRef();
	CRef(const CRef& _other);
	virtual ~CRef() = 0;

public:
	void AddRef() { ++m_iRef; }
	int Release()
	{
		--m_iRef;
		assert(m_iRef >= 0);
		if (0 == m_iRef) {
			delete this;
			return 0;
		}
		return m_iRef;
	}

public:
	// Getter
	bool Get_Enable() const { return m_bEnable; }
	bool Is_Dead() const { return m_bDead; }
	// Setter
	void Set_Enable(bool _bEnable) { m_bEnable = _bEnable; }
	void Die() { m_bDead = false; }

protected:
	int			m_iRef;
	bool		m_bEnable;
	bool		m_bDead;
};

#endif