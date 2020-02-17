#pragma once

#ifndef __MAINGAME_H__
#define __MAINGAME_H__

#include "Singleton.h"

class CMainGame : public Singleton<CMainGame>
{
	friend class Singleton<CMainGame>;

public:
	bool Initialize(HINSTANCE _hInst);
	void Logic();

private:
	void Update(float _fdTime);		
	void Late_Update(float _fdTime);
	void Collision(float _fdTime);
	void Render(float _fdTime);
	void Release();

private:
	CMainGame();
	~CMainGame();
	CMainGame(const CMainGame&) = delete;
	CMainGame& operator=(const CMainGame&) = delete;

private:
	HINSTANCE		m_hInst;
	HDC				m_hDC;
};

#endif