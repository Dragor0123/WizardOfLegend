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
	const string& Get_Tag() const { return m_strTag; }
	// Setter
	void Set_Tag(const string& _strTag) { m_strTag = _strTag; }
protected:
	int			m_iRef;
	string		m_strTag;
};

#endif