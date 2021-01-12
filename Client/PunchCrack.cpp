#include "stdafx.h"
#include "PunchCrack.h"

#include "ScrollMgr.h"
#include "TextureMgr.h"
#include "GraphicDev.h"
#include "SoundMgr.h"


CPunchCrack::CPunchCrack()
{
	CSoundMgr::Get_Instance()->PlaySoundW(L"golem_dungeon_king_golem_handcrash.wav", CSoundMgr::PUNCH);

	m_wstrObjKey = L"BOSS_A";
	m_wstrStateKey = L"PUNCH_CRACK";

	m_tFrame = FRAME(0.f, 0.f, 0.f);
}


CPunchCrack::~CPunchCrack()
{
}

HRESULT CPunchCrack::Initialize(void)
{
	return E_NOTIMPL;
}

int CPunchCrack::Progress(void)
{
	return 0;
}

void CPunchCrack::Render(void)
{
	D3DXMatrixTranslation(&m_matTrans,
		m_tInfo.vPos.x + CScrollMgr::Get_ScrollX(),
		m_tInfo.vPos.y + CScrollMgr::Get_ScrollY(),
		0.f);

	D3DXMatrixScaling(&m_matScale, 1.5f, 1.5f, 0.f);

	m_tInfo.matWorld = m_matScale * m_matTrans;

	const TEXINFO* 	m_pTexture = CTextureMgr::Get_Instance()->GetTexture(m_wstrObjKey,
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

void CPunchCrack::Release(void)
{
}
