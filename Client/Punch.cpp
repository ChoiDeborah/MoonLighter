#include "stdafx.h"
#include "Punch.h"

#include "TextureMgr.h"
#include "GraphicDev.h"
#include "TimeMgr.h"

#include "Player.h"
#include "ScrollMgr.h"

#include "ObjMgr.h"
#include "PunchCrack.h"

#include "ObjFactory.h"

CPunch::CPunch()
	: m_eCurState(PUNCH_END)
	, m_eNextState(PUNCH_UP)
	, m_pPlayer(nullptr)
	, m_fOriginX(0.f)
	, m_fOriginY(0.f)
	, m_fPatternTime(0.f)
	, m_bIsCrack(false)
	, m_bIsDown(false)
	, m_fShadowScaleX(1.f)
	, m_fShadowScaleY(1.f)
	, m_fLifeTime(0.f)
{
	m_wstrObjKey = L"BOSS_A";
	m_tFrame = FRAME(0.f, 10.f, 0.f);
	m_fScaleX = 1.5f;
	m_fScaleY = 1.5f;
	m_fSpeed = 2500.f;
	m_tInfo.fCX = 60.f;
	m_tInfo.fCY = 80.f;
	m_vPlayerPos = D3DXVECTOR3(0.f, 0.f, 0.f);
}



CPunch::~CPunch()
{
	//Safe_Delete(m_pTexture);
}

HRESULT CPunch::Initialize(void)
{
	return E_NOTIMPL;
}

int CPunch::Progress(void)
{
	m_fLifeTime += GET_TIME;

	if (m_fLifeTime > 10.f)
		return 1;
	CObj::Frame();
	CObj::Matrix();
	Moving();
	StateChange();
	PatternChange();
	return 0;
}

void CPunch::Render(void)
{
	CObj::UpdateRect();
	if (nullptr != m_pPlayer && m_eCurState != PUNCH_DOWN)
	{
		m_vPlayerPos = m_pPlayer->GetInfo()->vPos;
		m_tInfo.vPos.x = m_vPlayerPos.x;
	}


	// Shadow

	D3DXMatrixTranslation(&m_matTrans,
		//m_fOriginX + CScrollMgr::Get_ScrollX(),
		//m_fOriginY + CScrollMgr::Get_ScrollY(),
		m_vPlayerPos.x + CScrollMgr::Get_ScrollX(),
		m_vPlayerPos.y + CScrollMgr::Get_ScrollY(),
		0.f);

	D3DXMatrixScaling(&m_matScale, m_fShadowScaleX, m_fShadowScaleY, 0.f);

	m_tInfo.matWorld = m_matScale * m_matTrans;

	////Shadow
	const TEXINFO* m_pTextureShadow = CTextureMgr::Get_Instance()->GetTexture(m_wstrObjKey,
		L"PUNCH_SHADOW", 0);

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

	

	// PUNCH
	//cout << "Punch X" << m_tInfo.vPos.x  << "Punch Y" << m_tInfo.vPos.y << endl;
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

	fX = m_pTexture->tImgInfo.Width / 2.f;
	fY = m_pTexture->tImgInfo.Height / 2.f;

	CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CGraphicDev::Get_Instance()->GetSprite()->Draw(m_pTexture->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	//Render_Rect();

}

void CPunch::Release(void)
{

}

void CPunch::StateChange(void)
{
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case CPunch::PUNCH_CRACK:
			m_wstrStateKey = L"PUNCH_CRACK";
			m_eNextDir = DIRECTION::DIRICTION_END;
			break;
		case CPunch::PUNCH_DOWN:
			m_wstrStateKey = L"PUNCH_DOWN";
			m_eNextDir = DIRECTION::DOWN;
			
			break;
		case CPunch::PUNCH_SHADOW:
			m_wstrStateKey = L"PUNCH_SHADOW";
			m_eNextDir = DIRECTION::DIRICTION_END;
			break;
		case CPunch::PUNCH_UP:
			m_wstrStateKey = L"PUNCH_UP";
			m_eNextDir = DIRECTION::UP;
			m_bIsCrack = false;

			break;
		case CPunch::PUNCH_END:

			break;
		default:
			break;
		}
		m_tFrame.fMax = CTextureMgr::Get_Instance()->GetImgCount(m_wstrObjKey, m_wstrStateKey) - 1;
		m_eCurState = m_eNextState;
	}

	
}

void CPunch::Set_OriginPosition(float fX, float fY)
{
	m_fOriginX = m_tInfo.vPos.x;
	m_fOriginY = m_tInfo.vPos.y;
}

void CPunch::Moving()
{
	switch (m_eCurState)
	{
	case CPunch::PUNCH_CRACK:

		break;
	case CPunch::PUNCH_DOWN:
		//cout << "Player Y : " << m_vPlayerPos.y << "Punch Y : "<< m_tInfo.vPos.y <<  endl;
		if (m_vPlayerPos.y - 20.f > m_tInfo.vPos.y)
		{
			m_tInfo.vPos.y += m_fSpeed* GET_TIME;
			// 그림자 커지게
			if (m_fShadowScaleX < 1.5f)
			{
				m_fShadowScaleX += GET_TIME;
				m_fShadowScaleY += GET_TIME;
			}
		
		}
		else 
		{
			m_tInfo.vPos.y = m_vPlayerPos.y - 20.f;
			if (m_bIsCrack == false)
			{
				m_bIsCrack = true;
				// 펀치 크랙 생성
				//cout << " Crack!" << endl;
				CObjMgr::Get_Instance()->AddObject(OBJID::OBJ_PUNCH_CRACK, CObjFactory<CPunchCrack>::CreateObj(m_tInfo.vPos.x, m_tInfo.vPos.y));

			}
		}
		break;
	case CPunch::PUNCH_SHADOW:

		break;
	case CPunch::PUNCH_UP:
		// -100.f 까지 위로 올라간다.
		if(m_tInfo.vPos.y > -100.f)
			m_tInfo.vPos.y -= m_fSpeed * GET_TIME;
		// 그림자 줄어 들게
		if (m_fShadowScaleX > 0.5f)
		{
			m_fShadowScaleX -= GET_TIME;
			m_fShadowScaleY -= GET_TIME;
		}

		break;
	case CPunch::PUNCH_END:

		break;
	default:
		break;
	}

}

void CPunch::PatternChange()
{
	m_fPatternTime += GET_TIME;

	if (m_fPatternTime > 2.f)
	{
		m_fPatternTime = 0.f;
		if(m_eCurState == PUNCH_UP)
			m_eNextState = PUNCH_DOWN;
		if(m_eCurState == PUNCH_DOWN)
			m_eNextState = PUNCH_UP;
	}
}

void CPunch::Render_Rect()
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
