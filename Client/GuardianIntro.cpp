#include "stdafx.h"
#include "GuardianIntro.h"

#include "GraphicDev.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"

#include "Guardian.h"



CGuardianIntro::CGuardianIntro()
{
	m_tInfo.vPos = D3DXVECTOR3(BOSSMAP_1CX / 2 + 35.f, 400.f, 0.f);

	m_wstrObjKey = L"BOSS_C_HEAD";
	m_wstrStateKey = L"INTRO";
	m_tFrame = FRAME(0.f, 10.f, 10.f);
	m_tFrame.fMax = CTextureMgr::Get_Instance()->GetImgCount(m_wstrObjKey, m_wstrStateKey) - 1;
}


CGuardianIntro::~CGuardianIntro()
{
}

HRESULT CGuardianIntro::Initialize(void)
{
	return E_NOTIMPL;
}

int CGuardianIntro::Progress(void)
{
	Frame();
	Matrix();

	if (m_bIsDead)
		return 1;

	return 0;
}

void CGuardianIntro::Render(void)
{
	const TEXINFO* m_pTexture = CTextureMgr::Get_Instance()->GetTexture(m_wstrObjKey,
		m_wstrStateKey,
		int(m_tFrame.fFrame));

	if (nullptr == m_pTexture)
		return;

	float fX = m_pTexture->tImgInfo.Width / 2.f;
	float fY = m_pTexture->tImgInfo.Height / 2.f;

	CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CGraphicDev::Get_Instance()->GetSprite()->Draw(m_pTexture->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CGuardianIntro::Release(void)
{
}

void CGuardianIntro::Frame(void)
{
	m_tFrame.fFrame += m_tFrame.fCount * GET_TIME;

	if (m_tFrame.fFrame > m_tFrame.fMax)
	{
		m_tFrame.fFrame = 0.f;
		if (nullptr != m_pTarget)
		{
			dynamic_cast<CGuardian*>(m_pTarget)->m_bStartStage = true;
			m_bIsDead = true;
		}
	}
}

void CGuardianIntro::Matrix(void)
{
	D3DXMATRIX matTrans, matScale;

	D3DXMatrixIdentity(&matScale);

	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x + CScrollMgr::Get_ScrollX(),
		m_tInfo.vPos.y + CScrollMgr::Get_ScrollY(),
		0.f);

	D3DXMatrixScaling(&matScale, 1.5f, 1.5f, 0.f);

	m_tInfo.matWorld = matScale * matTrans;
}
