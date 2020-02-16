#pragma once

#ifndef __TEMPLATE_H__
#define __TEMPLATE_H__

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
class CObjCmp
{
public:
	bool operator()(T _a, T _b)
	{
		return _a->Get_TargetDist() < _b->Get_TargetDist();
	}
};

#endif // !__TEMPLATE_H__
