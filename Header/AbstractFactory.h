#pragma once

#ifndef __AbstractFactory_H__
#define __AbstractFactory_H__

// 클래스로 정의하는 이유
// 조금이라도 객체 지향에 가깝게 코드를 구현하기 위해서이다.
class CObj;
template <typename T>
class CAbstractFactory
{
public:
	// static 사용하는 이유
	// static을 사용하면 객체 생성 없이 스코프 연산자를 통해 접근할 수 있다!
	static CObj* Create()
	{
		CObj* pObj = new T;
		pObj->Initialize();

		return pObj;
	}

	static CObj* Create(float _x, float _y)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(_x, _y);
		pObj->Initialize();
		return pObj;
	}

	static CObj* Create(float _x, float _y, float _fAngle)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(_x, _y);
		pObj->Set_Angle(_fAngle);
		pObj->Initialize();
		return pObj;
	}
};

#endif // !__AbstractFactory_H__
