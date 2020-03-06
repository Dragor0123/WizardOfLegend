#pragma once

#ifndef __AbstractFactory_H__
#define __AbstractFactory_H__

// Ŭ������ �����ϴ� ����
// �����̶� ��ü ���⿡ ������ �ڵ带 �����ϱ� ���ؼ��̴�.

class CObj;
class CArcRel;
template <typename T>
class CAbstractFactory
{
public:
	// static ����ϴ� ����
	// static�� ����ϸ� ��ü ���� ���� ������ �����ڸ� ���� ������ �� �ִ�!
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

	// NPC, Monster, Obstacle, UnderDeco, Fable, SummonCard, UI ���� etc
	static CObj* Create(float _x, float _y, const string& _frameKey)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(_x, _y);
		pObj->Set_FrameKey(_frameKey);
		pObj->Initialize();
		return pObj;
	}

	static CObj* Create(float _x, float _y, const string& _frameKey, CObj* _pTarget)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(_x, _y);
		pObj->Set_FrameKey(_frameKey);
		pObj->Set_Target(_pTarget);
		pObj->Initialize();
		return pObj;
	}

	// Fbutton ����
	static CObj* Create(float _x, float _y, OBJID::ID _id)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(_x, _y);
		pObj->Set_OBJID(_id);
		pObj->Initialize();
		return pObj;
	}

	// Bullet & Shield;
	static CObj* Create(float _x, float _y, float _fAngle,
		const string& _frameKey)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(_x, _y);
		pObj->Set_Angle(_fAngle);
		pObj->Set_FrameKey(_frameKey);
		pObj->Initialize();
		return pObj;
	}

	//Arcana, Relic ���� : _frameKey���� Bullet�� frameKey _pTarget�� �÷��̾�
	static CArcRel* Create(const string& _frameKey, CObj* _pTarget)
	{
		CArcRel* pObj = new T;
		pObj->Set_FrameKey(_frameKey);
		pObj->Set_Target(_pTarget);
		pObj->Initialize();
		return pObj;
	}

	static CObj* Create(CObj* _pTarget)
	{
		CObj* pObj = new T;
		pObj->Set_Target(_pTarget);
		pObj->Initialize();
		return pObj;
	}

};

#endif // !__AbstractFactory_H__
