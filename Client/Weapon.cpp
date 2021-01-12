#include "stdafx.h"
#include "Weapon.h"
#include "ScrollMgr.h"
#include "TextureMgr.h"
#include "GraphicDev.h"

CWeapon::CWeapon() :
	m_eCurWeapon(WEAPON_END)
	, m_eNextWeapon(WEAPON_END)
	, m_bStateChange(false)
	, m_bPlayFrame(false)
{
	m_wstrObjKey = L"WEAPON2";
	m_tFrame = FRAME(0.f, 10.f, 6.f);
	m_eCurDir = DOWN;

	m_tInfo.fCX = 120.f;
	m_tInfo.fCY = 80.f;

	Load_Rect();

}


CWeapon::~CWeapon()
{
}

HRESULT CWeapon::Initialize(void)
{
	
	return S_OK;
}

int CWeapon::Progress(void)
{
	if(m_bPlayFrame)
		Frame();

	Update_Rect();

	Matrix();
	ChangeType();
	Change_Direction();


	return 0;
}

void CWeapon::Render(void)
{
	if (m_bPlayFrame)
	{
		//CObj::UpdateRect();

		const TEXINFO * m_pTexture = CTextureMgr::Get_Instance()->GetTexture(m_wstrObjKey,
			m_wstrStateKey,
			int(m_tFrame.fFrame));

		if (nullptr == m_pTexture)
			return;

		float fX = m_pTexture->tImgInfo.Width / 2.f;
		float fY = m_pTexture->tImgInfo.Height / 2.f;

		
		
		//wcout << "Weapon State Key : " << m_wstrStateKey << endl;
		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

		CGraphicDev::Get_Instance()->GetSprite()->Draw(m_pTexture->pTexture,
			NULL,
			&D3DXVECTOR3(fX, fY, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));

		//Render_Rect();

	}
	
}

void CWeapon::Release(void)
{
}

void CWeapon::Render_Rect()
{

	D3DXVECTOR2			vPoint[5] = {
		D3DXVECTOR2((float)m_tRect.left , (float)m_tRect.top),
		D3DXVECTOR2((float)m_tRect.right  , (float)m_tRect.top ),
		D3DXVECTOR2((float)m_tRect.right  , (float)m_tRect.bottom),
		D3DXVECTOR2((float)m_tRect.left  , (float)m_tRect.bottom),
		D3DXVECTOR2((float)m_tRect.left , (float)m_tRect.top)
	};

	//cout <<"RECT left : " << (float)m_tRect.left + m_tInfo.vPos.x << endl;
	//cout << "RECT + SCROLL " << (float)m_tRect.left + m_tInfo.vPos.x + CScrollMgr::Get_ScrollX() << endl;

	CGraphicDev::Get_Instance()->Render_End();
	CGraphicDev::Get_Instance()->Render_Begin();

	CGraphicDev::Get_Instance()->GetLine()->Draw(vPoint, 5, D3DCOLOR_ARGB(255, 255, 0, 255));

	CGraphicDev::Get_Instance()->Render_End();
	CGraphicDev::Get_Instance()->Render_Begin();
}

void CWeapon::Update_Rect()
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



	m_tRect.left = (m_pTexture->tRect.left - 250.f) / fScaleX *1.8 + m_tInfo.vPos.x + CScrollMgr::Get_ScrollX();
	m_tRect.right = (m_pTexture->tRect.right - 250.f) / fScaleX *1.8 + m_tInfo.vPos.x + CScrollMgr::Get_ScrollX();
	m_tRect.top = (m_pTexture->tRect.top - 250.f) / fScaleY *1.8 + m_tInfo.vPos.y + CScrollMgr::Get_ScrollY();
	m_tRect.bottom = (m_pTexture->tRect.bottom - 250.f) / fScaleY *1.8 + m_tInfo.vPos.y + CScrollMgr::Get_ScrollY();

}

void CWeapon::ChangeType()
{

	if (m_eCurWeapon != m_eNextWeapon || m_bStateChange )
	{
		if (m_bStateChange)
			m_bStateChange = false;

		m_tFrame.fFrame = 0;

		switch (m_eNextWeapon)
		{
		case BIG_SWORD:
			switch (m_eCurDir)
			{
			case CObj::UP:
				m_wstrStateKey = L"W_BIG_UP";
			
				m_tInfo.fCX = 120.f;
				m_tInfo.fCY = 80.f;

				break;
			case CObj::DOWN:
				m_wstrStateKey = L"W_BIG_DOWN";
				m_tInfo.fCX = 120.f;
				m_tInfo.fCY = 80.f;
				break;
			case CObj::RIGHT:
				m_wstrStateKey = L"W_BIG_RIGHT";
				m_tInfo.fCX = 80.f;
				m_tInfo.fCY = 120.f;
				break;
			case CObj::LEFT:
				m_wstrStateKey = L"W_BIG_LEFT";
				m_tInfo.fCX = 80.f;
				m_tInfo.fCY = 120.f;

				break;
			case CObj::UP_LEFT:
				m_wstrStateKey = L"W_BIG_UP";
				m_tInfo.fCX = 120.f;
				m_tInfo.fCY = 80.f;

				break;
			case CObj::UP_RIGHT:
				m_wstrStateKey = L"W_BIG_UP";
				m_tInfo.fCX = 120.f;
				m_tInfo.fCY = 80.f;
	
				break;
			case CObj::DOWN_LEFT:
				m_wstrStateKey = L"W_BIG_DOWN";
				m_tInfo.fCX = 120.f;
				m_tInfo.fCY = 80.f;
	
				break;
			case CObj::DOWN_RIGHT:
				m_wstrStateKey = L"W_BIG_RIGHT";
				m_tInfo.fCX = 120.f;
				m_tInfo.fCY = 80.f;
				break;
			case CObj::DIRICTION_END:
				break;
			default:
				break;
			}
			break;
		case SHORT_SWORD:
			switch (m_eCurDir)
			{
			case CObj::UP:
				m_wstrStateKey = L"W_SHORT_UP";
				m_tInfo.fCX = 120.f;
				m_tInfo.fCY = 80.f;
				break;
			case CObj::DOWN:
				m_wstrStateKey = L"W_SHORT_DOWN";
				m_tInfo.fCX = 120.f;
				m_tInfo.fCY = 80.f;
				break;
			case CObj::RIGHT:
				m_wstrStateKey = L"W_SHORT_RIGHT";
				m_tInfo.fCX = 80.f;
				m_tInfo.fCY = 120.f;
				break;
			case CObj::LEFT:
				m_wstrStateKey = L"W_SHORT_LEFT";
				m_tInfo.fCX = 80.f;
				m_tInfo.fCY = 120.f;
				break;
			case CObj::UP_LEFT:
				m_wstrStateKey = L"W_SHORT_UP";
				m_tInfo.fCX = 120.f;
				m_tInfo.fCY = 80.f;
				break;
			case CObj::UP_RIGHT:
				m_wstrStateKey = L"W_SHORT_UP";
				m_tInfo.fCX = 120.f;
				m_tInfo.fCY = 80.f;
				break;
			case CObj::DOWN_LEFT:
				m_wstrStateKey = L"W_SHORT_DOWN";
				m_tInfo.fCX = 120.f;
				m_tInfo.fCY = 80.f;
				break;
			case CObj::DOWN_RIGHT:
				m_wstrStateKey = L"W_SHORT_DOWN";
				m_tInfo.fCX = 120.f;
				m_tInfo.fCY = 80.f;
				break;
			case CObj::DIRICTION_END:
				break;
			default:
				break;
			}


			break;
		case WEAPON_END:
			break;
		default:
			break;
		}
		m_tFrame.fMax = (float)CTextureMgr::Get_Instance()->GetImgCount(m_wstrObjKey, m_wstrStateKey) - 1;

		m_eCurWeapon = m_eNextWeapon;
	}
}

void CWeapon::Frame()
{
	m_tFrame.fFrame += m_tFrame.fCount * GET_TIME;

	if (m_tFrame.fFrame > m_tFrame.fMax)
	{
		m_tFrame.fFrame = 0.f;
	}
}

void CWeapon::Matrix(void)
{

	D3DXMATRIX		matTrans, matScale;

	D3DXMatrixIdentity(&matScale);

	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x + CScrollMgr::Get_ScrollX(),
		m_tInfo.vPos.y + CScrollMgr::Get_ScrollY(),
		0.f);

	D3DXMatrixScaling(&matScale, 1.1f, 1.1f, 0.f);

	m_tInfo.matWorld = matScale * matTrans;
}

void CWeapon::Reset_Rect()
{

}

void CWeapon::Change_Direction()
{
	if (m_eCurDir != m_eNextDir)
	{
		switch (m_eNextDir)
		{
		case CObj::UP:
			m_tInfo.vDir = D3DXVECTOR3(0.f, -1.f, 0.f);

		
			break;
		case CObj::DOWN:
			m_tInfo.vDir = D3DXVECTOR3(0.f, 1.f, 0.f);

			
			break;
		case CObj::RIGHT:
			m_tInfo.vDir = D3DXVECTOR3(1.f, 0.f, 0.f);
			
			break;
		case CObj::LEFT:
			m_tInfo.vDir = D3DXVECTOR3(-1.f, 0.f, 0.f);		
			break;
		case CObj::UP_LEFT:
			m_tInfo.vDir = D3DXVECTOR3(-1.f, -1.f, 0.f);	
			break;
		case CObj::UP_RIGHT:
			m_tInfo.vDir = D3DXVECTOR3(1.f, -1.f, 0.f);		
			break;
		case CObj::DOWN_LEFT:
			m_tInfo.vDir = D3DXVECTOR3(-1.f, 1.f, 0.f);			
			break;
		case CObj::DOWN_RIGHT:
			m_tInfo.vDir = D3DXVECTOR3(1.f, 1.f, 0.f);
			break;
		case CObj::DIRICTION_END:
			break;
		default:
			break;
		}

	
		m_eCurDir = m_eNextDir;
		m_bStateChange = true;
	}
}

void CWeapon::Load_Rect()
{
	HANDLE		hFile = CreateFile(L"../Data/WeaponRect_3.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD		dwByte = 0;


	map<wstring, CTexture*> *MapTexture = CTextureMgr::Get_Instance()->Get_MapTexture();

	auto iter = MapTexture->find(L"WEAPON2");

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

			const TEXINFO*	pTexture = CTextureMgr::Get_Instance()->GetTexture(L"WEAPON2", wstrStateKey, count);

			if (nullptr == pTexture)
				return;
			RECT rect;
			ReadFile(hFile, &rect, sizeof(RECT), &dwByte, NULL);
			CTextureMgr::Get_Instance()->Set_Rect(L"WEAPON2", wstrStateKey, count, rect);
			count++;
		}
		count = 0;
	}

	CloseHandle(hFile);
}
