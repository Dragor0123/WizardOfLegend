#pragma once

#ifndef __INLINE_H__
#define __INLINE_H__

inline float Get_Distance(const INFO & _p1, const INFO & _p2)
{
	float fDX = _p1.fX - _p2.fX;
	float fDY = _p1.fY - _p2.fY;
	float fDistance = sqrt(fDX * fDX + fDY * fDY);
	return fDistance;
}

inline float Get_Distance(const POINT & _p1, const POINT & _p2)
{
	float fDX = static_cast<float>(_p1.x - _p2.x);
	float fDY = static_cast<float>(_p1.y - _p2.y);
	float fDistance = sqrt(fDX * fDX + fDY * fDY);
	return fDistance;
}

inline float Degree_To_Radian(float _deg)
{
	return _deg * (PI / 180.f);
}

inline float Radian_To_Degree(float _rad)
{
	return _rad * (180.f / PI);
}

inline void Block_Obj_Escape_Rect(INFO* _info, LONG _left, LONG _top, LONG _right, LONG _bottom)
{
	RECT tRC = { _left, _top, _right, _bottom };

	if (LONG(_info->fX - (_info->iCX >> 1)) < tRC.left)
		_info->fX = float(tRC.left + (_info->iCX >> 1));
	else if (LONG(_info->fX + (_info->iCX >> 1)) > tRC.right)
		_info->fX = float(tRC.right - (_info->iCX >> 1));

	if (LONG(_info->fY - (_info->iCY >> 1)) < tRC.top)
		_info->fY = float(tRC.top + (_info->iCY >> 1));
	else if (LONG(_info->fY + (_info->iCY >> 1)) > tRC.bottom)
		_info->fY = float(tRC.bottom - (_info->iCY >> 1));
}

inline void Block_Obj_Escape_Rect(INFO* _info, const RECT* _lpRC)
{
	if (LONG(_info->fX - (_info->iCX >> 1)) < _lpRC->left)
		_info->fX = float(_lpRC->left + (_info->iCX >> 1));
	else if (LONG(_info->fX + (_info->iCX >> 1)) > _lpRC->right)
		_info->fX = float(_lpRC->right - (_info->iCX >> 1));

	if (LONG(_info->fY - (_info->iCY >> 1)) < _lpRC->top)
		_info->fY = float(_lpRC->top + (_info->iCY >> 1));
	else if (LONG(_info->fY + (_info->iCY >> 1)) > _lpRC->bottom)
		_info->fY = float(_lpRC->bottom - (_info->iCY >> 1));
}

#endif // !__INLINE_H__
