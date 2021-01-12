#include "stdafx.h"
#include "Wave.h"
#include "TextureMgr.h"
#include "GraphicDev.h"
#include "ScrollMgr.h"
#include "SoundMgr.h"



CWave::CWave()
{
	
	m_wstrObjKey = L"BOSS_A";
	m_wstrStateKey = L"WAVE";

	m_tFrame = FRAME(0.f, 10.f, 0.f);
	m_tFrame.fMax = (float)CTextureMgr::Get_Instance()->GetImgCount(m_wstrObjKey, m_wstrStateKey) - 1;

	m_tInfo.fCX = 300.f;
	m_tInfo.fCY = 300.f;

	LoadRect();
}


CWave::~CWave()
{

}

HRESULT CWave::Initialize(void)
{
	return E_NOTIMPL;
}

int CWave::Progress(void)
{
	Frame();
	Matrix();
	Update_Rect();

	if (m_bIsDead)
		return 1;

	return 0;
}

void CWave::Render(void)
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

	//Render_Rect();

}

void CWave::Release(void)
{
}

void CWave::Frame()
{
	m_tFrame.fFrame += m_tFrame.fCount * GET_TIME;
	
	if (m_tFrame.fFrame > m_tFrame.fMax)
	{
		m_tFrame.fFrame = 0.f;
		m_bIsDead = true;
	}
}

void CWave::Matrix()
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

void CWave::LoadRect()
{
	HANDLE		hFile = CreateFile(L"../Data/BossRect.dat",
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD		dwByte = 0;


	map<wstring, CTexture*> *MapTexture = CTextureMgr::Get_Instance()->Get_MapTexture();

	auto iter = MapTexture->find(m_wstrObjKey);

	// Obj Key Ã£±â
	if (iter == MapTexture->end())
		return;


	map<wstring, vector<TEXINFO*>> * MapMultiTexture = nullptr;
	MapMultiTexture = dynamic_cast<CMultiTexture*>(iter->second)->Get_MapMultiTexture();
	int count = 0;
	// State Key
	for (auto & iter2 : *MapMultiTexture)
	{

		wstring wstrStateKey;
		wstrStateKey = iter2.first;
		for (auto & iter3 : iter2.second)
		{

			const TEXINFO*	pTexture = CTextureMgr::Get_Instance()->GetTexture(m_wstrObjKey, wstrStateKey, count);

			if (nullptr == pTexture)
				return;
			RECT rect;
			ReadFile(hFile, &rect, sizeof(RECT), &dwByte, NULL);
			CTextureMgr::Get_Instance()->Set_Rect(m_wstrObjKey, wstrStateKey, count, rect);
			count++;
		}
		count = 0;
	}

	CloseHandle(hFile);
}

void CWave::Render_Rect()
{

	D3DXVECTOR2			vPoint[5] = {
		D3DXVECTOR2((float)m_tRect.left , (float)m_tRect.top),
		D3DXVECTOR2((float)m_tRect.right  , (float)m_tRect.top),
		D3DXVECTOR2((float)m_tRect.right  , (float)m_tRect.bottom),
		D3DXVECTOR2((float)m_tRect.left  , (float)m_tRect.bottom),
		D3DXVECTOR2((float)m_tRect.left , (float)m_tRect.top)
	};


	CGraphicDev::Get_Instance()->Render_End();
	CGraphicDev::Get_Instance()->Render_Begin();

	CGraphicDev::Get_Instance()->GetLine()->Draw(vPoint, 5, D3DCOLOR_ARGB(255, 255, 0, 255));

	CGraphicDev::Get_Instance()->Render_End();
	CGraphicDev::Get_Instance()->Render_Begin();
}

void CWave::Update_Rect()
{
	const TEXINFO * m_pTexture = CTextureMgr::Get_Instance()->GetTexture(m_wstrObjKey,
		m_wstrStateKey,
		int(m_tFrame.fFrame));

	if (nullptr == m_pTexture)
		return;

	float fX = m_pTexture->tImgInfo.Width;
	float fY = m_pTexture->tImgInfo.Height;


	float fScaleX = 500.f / m_pTexture->tImgInfo.Width;
	float fScaleY = 500.f / m_pTexture->tImgInfo.Height;



	m_tRect.left = (m_pTexture->tRect.left - 250.f) / fScaleX *2.5 + m_tInfo.vPos.x + CScrollMgr::Get_ScrollX();
	m_tRect.right = (m_pTexture->tRect.right - 250.f) / fScaleX *2.5 + m_tInfo.vPos.x + CScrollMgr::Get_ScrollX();
	m_tRect.top = (m_pTexture->tRect.top - 250.f) / fScaleY *1.8 + m_tInfo.vPos.y + CScrollMgr::Get_ScrollY();
	m_tRect.bottom = (m_pTexture->tRect.bottom - 250.f) / fScaleY *1.8 + m_tInfo.vPos.y + CScrollMgr::Get_ScrollY();

}
