#include "stdafx.h"
#include "Rock.h"
#include "TextureMgr.h"
#include "GraphicDev.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "SoundMgr.h"

CRock::CRock():
	m_fTime(0.f)
	, m_bStateChange(false)
	, m_fOriginY(0.f)
	, m_bShadow(true)
	, m_bBroken(false)
	, m_eCurState(STATE_END)
	, m_eNextState(IDLE)
{
	m_tInfo.fCX = 35.f;
	m_tInfo.fCY = 35.f;

	m_wstrObjKey = L"BOSS_A";
	m_wstrStateKey = L"ROCK";
	m_tFrame = FRAME(0.f, 100.f, 0.f);
	m_iTypeOfRock = rand() % 6;
	m_bCollision = false;

	switch (m_iTypeOfRock)
	{
	case 0:
		m_tFrame.fFrame = 0;
		break;
	case 1:
		m_tFrame.fFrame = 1;
		break;
	case 2:
		m_tFrame.fFrame = 2;
		break;
	case 3:
		m_tFrame.fFrame = 3;
		break;
	case 4:
		m_tFrame.fFrame = 4;
		break;
	case 5:
		m_tFrame.fFrame = 5;
		break;
	default:
		break;
	}

	m_fSpeed = 50.f;
}


CRock::~CRock()
{
	Release();
}

HRESULT CRock::Initialize(void)
{

	return E_NOTIMPL;
}

int CRock::Progress(void)
{
	//if(m_bCollision == false)
	//	cout << "false" << endl;
	//else 
	//	cout << "true" << endl;

	m_fTime += GET_TIME;

	if (m_fTime > 10.f)
	{
		m_eNextState = BROKEN;
		m_bCollision = false;

	}

	if (m_fTime > 1.f && m_bStateChange == false)
	{
		m_bStateChange = true;

		m_tInfo.vPos.y -= 200.f;
	}

	// 돌 깨질 때 이거 수행 안한다.
	if (m_bStateChange && m_eCurState != BROKEN)
	{
		if (m_tInfo.vPos.y < m_fOriginY) {
			m_tInfo.vPos.y +=  m_fSpeed;
			
		}
		else m_bCollision = true;

	}


	
	if(m_eCurState == BROKEN)
		Frame();
	ChangeState();

	if (m_bIsDead) {
		CSoundMgr::Get_Instance()->PlaySoundW(L"golem_dungeon_king_golem_rockdestroy.wav", CSoundMgr::ROCK);

		return 1;
	}

	return 0;
}

void CRock::Render(void)
{
	CObj::UpdateRect();

	D3DXMATRIX matTrans, matScale;

	D3DXMatrixScaling(&matScale, 1.3f, 1.3f, 0.f);
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x + CScrollMgr::Get_ScrollX(),
		m_fOriginY + 15.f + CScrollMgr::Get_ScrollY(),
		0.f);

	m_tInfo.matWorld = matScale * matTrans;
	
	// Shadow

	const TEXINFO* m_pTextureShadow = CTextureMgr::Get_Instance()->GetTexture(m_wstrObjKey,
		L"ROCK_SHADOW",0);

	if (nullptr == m_pTextureShadow)
		return;

	float fX = m_pTextureShadow->tImgInfo.Width / 2.f;
	float fY = m_pTextureShadow->tImgInfo.Height / 2.f;

	CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CGraphicDev::Get_Instance()->GetSprite()->Draw(m_pTextureShadow->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));
	

	// Rock
	if (m_bStateChange)
	{
		D3DXMatrixTranslation(&matTrans,
			m_tInfo.vPos.x + CScrollMgr::Get_ScrollX(),
			m_tInfo.vPos.y + CScrollMgr::Get_ScrollY(),
			0.f);

		const TEXINFO* m_pTexture = CTextureMgr::Get_Instance()->GetTexture(m_wstrObjKey,
			m_wstrStateKey,
			int(m_tFrame.fFrame));

		if (nullptr == m_pTexture)
			return;

		float fX = m_pTexture->tImgInfo.Width / 2.f;
		float fY = m_pTexture->tImgInfo.Height / 2.f;
		m_tInfo.matWorld = matScale * matTrans;

		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
		CGraphicDev::Get_Instance()->GetSprite()->Draw(m_pTexture->pTexture,
			NULL,
			&D3DXVECTOR3(fX, fY, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	//Render_Rect();

}

void CRock::Release(void)
{
	//Safe_Delete(m_pTexture);
}

void CRock::ChangeState(void)
{
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case CRock::FALLDOWN:
			break;
		case CRock::IDLE:
			break;
		case CRock::BROKEN:
			m_bBroken = true;
			m_tFrame.fFrame = 0;
			m_tFrame.fMax = 10.f;
			m_tFrame.fCount = 10.f;
			m_wstrStateKey = L"ROCK_HIT";
			
			break;
		case CRock::STATE_END:
			break;
		default:
			break;
		}

		m_eCurState = m_eNextState;
	}
}

void CRock::Render_Rect()
{
	D3DXVECTOR2			vPoint[5] = {
		D3DXVECTOR2((float)m_tRect.left, (float)m_tRect.top),
		D3DXVECTOR2((float)m_tRect.right, (float)m_tRect.top),
		D3DXVECTOR2((float)m_tRect.right, (float)m_tRect.bottom),
		D3DXVECTOR2((float)m_tRect.left, (float)m_tRect.bottom),
		D3DXVECTOR2((float)m_tRect.left, (float)m_tRect.top)
	};

	CGraphicDev::Get_Instance()->Render_End();
	CGraphicDev::Get_Instance()->Render_Begin();

	CGraphicDev::Get_Instance()->GetLine()->Draw(vPoint, 5, D3DCOLOR_ARGB(255, 255, 0, 255));

	CGraphicDev::Get_Instance()->Render_End();
	CGraphicDev::Get_Instance()->Render_Begin();
}

void CRock::SetOriginPosY()
{
	m_fOriginY = m_tInfo.vPos.y;
}

void CRock::Break()
{
	m_eNextState = BROKEN;
}

void CRock::Frame(void)
{
	m_tFrame.fFrame += m_tFrame.fCount * GET_TIME;

	if (m_tFrame.fFrame > m_tFrame.fMax)
	{
		m_tFrame.fFrame = 0.f;

		if (m_bBroken) {
			m_bIsDead = true;
			//cout << "Broken!" << endl;
		}
	}
}
