#pragma once

#ifndef __TIMER_H__
#define __TIMER_H__

#include "Singleton.h"

class CTimer : public Singleton<CTimer>
{
	friend class Singleton<CTimer>;

public:
	bool Initialize();
	void Update();

public:
	// Getter
	float Get_DeltaTime() const { return m_fDeltaTime * m_fTimeScale; }
	float Get_FPS() const { return m_fFPS; }
	float Get_TimeScale() const { return m_fTimeScale; }
	
	// Setter
	void Set_TimeScale(float _ftScale) { m_fTimeScale = m_fTimeScale; }
private:
	CTimer();
	~CTimer();
	CTimer(const CTimer&) = delete;
	CTimer& operator=(const CTimer&) = delete;

private:
	LARGE_INTEGER	m_tSecond;
	LARGE_INTEGER	m_tTime;
	float			m_fTimeScale;
	float			m_fDeltaTime;

	// 나중에 생각하기
	float			m_fFPS;
	float			m_fFPSTime;
	int				m_iFrameMax;
	int				m_iFrame;
};

#endif