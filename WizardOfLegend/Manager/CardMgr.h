#pragma once

#ifndef __CARDMGR_H__
#define __CARDMGR_H__

#include "Singleton.h"

class CArcRel;
class CCardMgr : public Singleton<CCardMgr>
{
	friend class Singleton<CCardMgr>;

public:
	bool Initialize();
	void Update(float _fdTime);
	void Late_Update(float _fdTime);
	// Render�� �ʿ��ϰڴ�!
	void Release();

public:
	HDC Find_Image35(int _iCardCode);
	HDC Find_Image46(int _iCardCode);
	CArcRel* Find_ArcRel(int _iCardCode);
	bool Insert_CodeImage35(int _iCardCode);
	bool Insert_CodeImage46(int _iCardCode);
	bool Insert_CodeArcana(int _code, CArcRel* _Arc);
	// return 0: �κ��丮�� ���� ����, 1 : Outter�� ����, 2: Inner�� ����
	int  Insert_Item_To_Inventory(int _code);

private:
	CCardMgr();
	~CCardMgr();
	CCardMgr(const CCardMgr&) = delete;
	CCardMgr& operator=(const CCardMgr&) = delete;

private:
	map<int, CArcRel*> m_mapArcRelic;
	map<int, string> m_mapCodeImage35;
	map<int, string> m_mapCodeImage46;
};

#endif