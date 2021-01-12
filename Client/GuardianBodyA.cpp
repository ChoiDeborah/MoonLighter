#include "stdafx.h"
#include "GuardianBodyA.h"

#include "TextureMgr.h"
#include "GraphicDev.h"

#include "TimeMgr.h"
#include "Guardian.h"
#include "ObjMgr.h"

#include "GuardianBodyB.h"
#include "LineMgr.h"
#include "Line.h"

CGuardianBodyA::CGuardianBodyA():
	m_eCurState(STATE_END)
	, m_eNextState(ATTACK_B)
{
	m_wstrObjKey = L"BOSS_C_BODY_A";
	m_wstrStateKey = L"";
	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 50.f;
	m_tFrame = FRAME(0.f, 10.f, 0.f);

	//CGuardianBodyB* pGuardianBodyB = new CGuardianBodyB;
	//pGuardianBodyB->Set_Target(this);

	//pGuardianBodyB->SetPos(m_tInfo.vPos.x + 100.f, m_tInfo.vPos.y + 60.f);
	//pGuardianBodyB->Initialize();

	//CObjMgr::Get_Instance()->AddObject(OBJID::OBJ_BODY_B, pGuardianBodyB);
	//m_pChild = pGuardianBodyB;

	m_tInfo.vDir = D3DXVECTOR3(1.f, 0.2f, 0.f);
}


CGuardianBodyA::~CGuardianBodyA()
{
}

HRESULT CGuardianBodyA::Initialize(void)
{
	//CGuardianBodyB* pGuardianBodyB = new CGuardianBodyB;
	//pGuardianBodyB->Set_Target(this);
	//pGuardianBodyB->SetPos(m_tInfo.vPos.x + 100, m_tInfo.vPos.y - 100);
	//pGuardianBodyB->Initialize();

	//CObjMgr::Get_Instance()->AddObject(OBJID::OBJ_BODY_B, pGuardianBodyB);

	//m_pChild = pGuardianBodyB;
	return E_NOTIMPL;
}

int CGuardianBodyA::Progress(void)
{
	CObj::UpdateRect();
	//cout << (int)m_eCurState << endl;
	Frame();
	CObj::Matrix();
	ChangeState();

	if (m_bIsMove)
		Moving();

	CollisionLock();


	return 0;
}

void CGuardianBodyA::Render(void)
{
	Render_Line();
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

	Render_Rect();
}

void CGuardianBodyA::Release(void)
{
}

void CGuardianBodyA::Moving()
{

	switch (m_eCurState)
	{
	case CGuardianBodyA::IDLE:
		if (nullptr != m_pTarget)
		{
			if (!CLineMgr::Get_Instance()->Get_ListLine()->empty())
			{
				CLine* pLine = CLineMgr::Get_Instance()->Get_ListLine()->back();

				D3DXVECTOR2 vPos = D3DXVECTOR2(m_tInfo.vPos.x, m_tInfo.vPos.y);
				D3DXVECTOR2 vDir = pLine->Get_LineInfo().vNextPoint - vPos;


				D3DXVec2Normalize(&vDir, &vDir);

				m_tInfo.vDir.x = vDir.x;
				m_tInfo.vDir.y = vDir.y;
				m_tInfo.vDir.z = 0.f;

				m_tInfo.vPos.x += m_tInfo.vDir.x * m_fSpeed  * GET_TIME;
				m_tInfo.vPos.y += m_tInfo.vDir.y * m_fSpeed  * GET_TIME;

				// 내위치에서 끝점 까지의 거리를 구한다.
				// (p1-p2)
				float fDistance = sqrtf((m_tInfo.vPos.x - pLine->Get_LineInfo().vNextPoint.x)*(m_tInfo.vPos.x - pLine->Get_LineInfo().vNextPoint.x)
					+ (m_tInfo.vPos.y - pLine->Get_LineInfo().vNextPoint.y)*(m_tInfo.vPos.y - pLine->Get_LineInfo().vNextPoint.y));


				if (fDistance < 5.0 && !CLineMgr::Get_Instance()->Get_ListLine()->empty())
				{
					Safe_Delete(CLineMgr::Get_Instance()->Get_ListLine()->back());
					CLineMgr::Get_Instance()->Get_ListLine()->pop_back();
				}
			}
		}
		break;
	case CGuardianBodyA::ATTACK_A:
		break;
	case CGuardianBodyA::ATTACK_B:
		m_tInfo.vPos += m_tInfo.vDir * m_fSpeed * GET_TIME;
		break;
	case CGuardianBodyA::DEATH:
		break;
	case CGuardianBodyA::STATE_END:
		break;
	default:
		break;
	}
	
}

void CGuardianBodyA::AllMoveStop()
{
	m_bIsMove = false;
	if (m_pChild)
		m_pChild->AllMoveStop();
}

void CGuardianBodyA::AllMoveActivate()
{
	m_bIsMove = true;
	if (m_pChild)
		m_pChild->AllMoveActivate();
}

void CGuardianBodyA::Hit(bool bIsY)
{

	switch (m_eCurState)
	{
	case CGuardianBodyA::IDLE:
		break;
	case CGuardianBodyA::ATTACK_A:
		break;
	case CGuardianBodyA::ATTACK_B:
		if (bIsY) 
		{
			m_tInfo.vDir.x *= -1;
		}
		else
		{
			m_tInfo.vDir.y *= -1;
		}
		break;
	case CGuardianBodyA::DEATH:
		break;
	case CGuardianBodyA::STATE_END:
		break;
	default:
		break;
	}
}

void CGuardianBodyA::Render_Line()
{
}

void CGuardianBodyA::CollisionLock()
{
	if (m_bCollision == false)
	{
		m_bHitColor = true;

		m_fCollisionTime += GET_TIME;

		if (m_fCollisionTime > 0.5)
			m_bHitColor = false;

		if (m_fCollisionTime > 0.5f)
		{
			m_fCollisionTime = 0.f;
			m_bCollision = true;
		}
	}
}


void CGuardianBodyA::ChangeState()
{
	if (m_eCurState != m_eNextState)
	{
		m_tFrame.fFrame = 0.f;

		switch (m_eNextState)
		{
		case CGuardianBodyA::IDLE:
			m_wstrStateKey = L"IDLE";
			break;
		case CGuardianBodyA::ATTACK_A:
			m_wstrStateKey = L"ATTACK_A";
			break;
		case CGuardianBodyA::ATTACK_B:
			m_wstrStateKey = L"ATTACK_B";
			m_fSpeed = 600.f;
			break;
		case CGuardianBodyA::DEATH:
			m_wstrStateKey = L"DEATH";
			break;
		case CGuardianBodyA::STATE_END:
			break;
		default:
			break;
		}
		m_eCurState = m_eNextState;
		m_tFrame.fMax = CTextureMgr::Get_Instance()->GetImgCount(m_wstrObjKey, m_wstrStateKey) - 1;
	}
}

void CGuardianBodyA::Frame()
{
	m_tFrame.fFrame += m_tFrame.fCount * GET_TIME;

	if (m_tFrame.fFrame > m_tFrame.fMax)
	{
		m_tFrame.fFrame = 0.f;

		//m_eNextState = BODY_A_STATE(rand()%5);
		//if (m_eNextState > STATE_END)
		//	m_eNextState = BODY_A_STATE(0);
	}
}
