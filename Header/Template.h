#pragma once

#ifndef __TEMPLATE_H__
#define __TEMPLATE_H__

template <typename T>
inline void Swap(T* _a, T* _b)
{
	T temp = *_a;
	*_a = *_b;
	*_b = temp;
}

template <typename T>
void Safe_Delete(T& _obj)
{
	if (_obj)
	{
		delete _obj;
		_obj = nullptr;
	}
}

template <typename T>
class CObjDistCmp
{
public:
	bool operator()(T _a, T _b)
	{
		return _a->Get_TargetDist() < _b->Get_TargetDist();
	}
};



template <typename T>
void Safe_Delete_VecList(T& _con)
{
	typename T::iterator iter;
	typename T::iterator iterEnd = _con.end();

	for (iter = _con.begin(); iter != iterEnd; ++iter)
		SAFE_DELETE((*iter));

	_con.clear();
}

template <typename T>
void Safe_Delete_Map(T& _con)
{
	typename T::iterator iter;
	typename T::iterator iterEnd = _con.end();

	for (iter = _con.begin(); iter != iterEnd; ++iter)
		SAFE_DELETE(iter->second);

	_con.clear();
}

template <typename T>
bool ObjectSortY(T _Dst, T _Src)
{
	return _Dst->Get_Info().fY < _Src->Get_Info().fY;
}

#endif // !__TEMPLATE_H__
