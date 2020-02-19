#include "../stdafx.h"
#include "Timer.h"


bool CTimer::Initialize()
{
	QueryPerformanceFrequency(&m_tSecond);
	QueryPerformanceCounter(&m_tTime);
	return true;
}

void CTimer::Update()
{
	LARGE_INTEGER tTime;
	QueryPerformanceCounter(&tTime);
	m_fDeltaTime = (tTime.QuadPart - m_tTime.QuadPart) / (float)(m_tSecond.QuadPart);
	m_tTime = tTime;

	m_fFPSTime += m_fDeltaTime;
	++m_iFrame;

	if (m_fFPSTime >= 1.f) {
		m_fFPS = m_iFrame / m_fFPSTime;
		m_fFPSTime = 0.f;
		m_iFrame = 0;
	}
}

CTimer::CTimer()
	: m_fDeltaTime(0.f), m_fFPS(0.f), m_fFPSTime(0.f),
	m_iFrame(0), m_fTimeScale(1.f)
{
	ZeroMemory(&m_tSecond, sizeof(m_tSecond));
	ZeroMemory(&m_tTime, sizeof(m_tTime));
}


CTimer::~CTimer()
{
}
