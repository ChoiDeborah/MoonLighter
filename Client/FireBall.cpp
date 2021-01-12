#include "stdafx.h"
#include "FireBall.h"


CFireBall::CFireBall()
{
	m_wstrObjKey = L"BOSS_C";
	m_wstrStateKey = L"FIREBALL";
}


CFireBall::~CFireBall()
{
}

HRESULT CFireBall::Initialize(void)
{
	return E_NOTIMPL;
}

int CFireBall::Progress(void)
{
	return 0;
}

void CFireBall::Render(void)
{
}

void CFireBall::Release(void)
{
}
