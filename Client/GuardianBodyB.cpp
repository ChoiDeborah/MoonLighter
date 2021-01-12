#include "stdafx.h"
#include "GuardianBodyB.h"

#include "TextureMgr.h"
#include "GraphicDev.h"

#include "TimeMgr.h"
#include "Line.h"

#include "GuardianBodyC.h"
#include "ObjMgr.h"

CGuardianBodyB::CGuardianBodyB()
	: m_eCurState(STATE_END)
	, m_eNextState(IDLE)
	, m_fLineTime(0.f)
{
	m_wstrObjKey = L"BOSS_C_BODY_B";
	m_wstrStateKey = L"";

	PrevPoint = D3DXVECTOR2(0.f, 0.f);
	m_tFrame = FRAME(0.f, 10.f, 0.f);
}


CGuardianBodyB::~CGuardianBodyB()
{
	Release();
}


void CGuardianBodyB::ChangeState()
{
	if (m_eCurState != m_eNextState)
	{
		m_tFrame.fFrame = 0.f;

		switch (m_eNextState)
		{
		case CGuardianBodyB::IDLE:
			m_wstrStateKey = L"IDLE";
			m_fSpeed = 150.f;
			break;
		case CGuardianBodyB::ATTACK_A:
			m_wstrStateKey = L"ATTACK_A";
			break;
		case CGuardianBodyB::ATTACK_B:
			m_wstrStateKey = L"ATTACK_B";
			break;
		case CGuardianBodyB::DEATH:
			m_wstrStateKey = L"DEATH";
			break;
		case CGuardianBodyB::STATE_END:
			break;
		default:
			break;
		}
		m_eCurState = m_eNextState;
		m_tFrame.fMax = CTextureMgr::Get_Instance()->GetImgCount(m_wstrObjKey, m_wstrStateKey) - 1;
	}
}

void CGuardianBodyB::makeNewLine()
{
	m_fLineTime += GET_TIME;

	if (m_fLineTime > LINETIME && nullptr != m_pTarget)
	{
		m_fLineTime = 0.f;
		
		PrevPoint = CurPoint;
		CurPoint.x = m_pTarget->GetInfo()->vPos.x;
		CurPoint.y = m_pTarget->GetInfo()->vPos.y;

		LINEINFO temp;
		temp.vPrevPoint = PrevPoint;
		temp.vNextPoint = CurPoint;
	
		m_listLine.push_back(new CLine(temp));
		m_eCurDir = m_eNextDir;
	}

}

void CGuardianBodyB::Moving()
{
	if (nullptr != m_pTarget)
	{
		CLine* pLine = nullptr;
		
		if (!m_listLine.empty())
		{
			pLine = m_listLine.back();
			D3DXVECTOR2 vPos = D3DXVECTOR2(m_tInfo.vPos.x, m_tInfo.vPos.y);
			D3DXVECTOR2 vDir = pLine->Get_LineInfo().vNextPoint - vPos;
			D3DXVec2Normalize(&vDir, &vDir);

			m_tInfo.vDir.x = vDir.x;
			m_tInfo.vDir.y = vDir.y;
			m_tInfo.vDir.y = 0.f;

			m_tInfo.vPos.x += m_tInfo.vDir.x * m_fSpeed  * GET_TIME;
			m_tInfo.vPos.y += m_tInfo.vDir.x * m_fSpeed  * GET_TIME;

			// 내위치에서 끝점 까지의 거리를 구한다.
			// (p1-p2)
			float fDistance = sqrtf((m_tInfo.vPos.x - pLine->Get_LineInfo().vNextPoint.x)*(m_tInfo.vPos.x - pLine->Get_LineInfo().vNextPoint.x)
				+ (m_tInfo.vPos.y - pLine->Get_LineInfo().vNextPoint.y)*(m_tInfo.vPos.y - pLine->Get_LineInfo().vNextPoint.y));

			//cout << "Distance : " << fDistance << endl;

			if (fDistance < 5.0 && !m_listLine.empty())
			{
				Safe_Delete(m_listLine.back());
				m_listLine.pop_back();
			}

		}
	}
}

void CGuardianBodyB::Frame(void)
{
	m_tFrame.fFrame += m_tFrame.fCount * GET_TIME;

	if (m_tFrame.fFrame > m_tFrame.fMax)
	{
		m_tFrame.fFrame = 0.f;

		m_eNextState = BODY_B_STATE(rand() % 5);
		if (m_eNextState > STATE_END)
			m_eNextState = BODY_B_STATE(0);
	}
}

void CGuardianBodyB::AllMoveStop()
{
	m_bIsMove = false;
	if (m_pChild)
		m_pChild->AllMoveStop();
}

void CGuardianBodyB::AllMoveActivate()
{
	m_bIsMove = true;
	if (m_pChild)
		dynamic_cast<CGuardianBodyB*>(m_pChild)->AllMoveActivate();
}


HRESULT CGuardianBodyB::Initialize(void)
{

	CGuardianBodyC* pGuardianBodyC = new CGuardianBodyC;
	pGuardianBodyC->Set_Target(this);
	pGuardianBodyC->SetPos(m_tInfo.vPos.x - 100.f, m_tInfo.vPos.y + 50.f);
	pGuardianBodyC->Initialize();

	CObjMgr::Get_Instance()->AddObject(OBJID::OBJ_BODY_C, pGuardianBodyC);

	m_pChild = pGuardianBodyC;

	if (nullptr != m_pTarget)
	{
		CurPoint = D3DXVECTOR2(m_tInfo.vPos.x, m_tInfo.vPos.y);
	
		LINEINFO temp;

		temp.vPrevPoint = D3DXVECTOR2(m_tInfo.vPos.x, m_tInfo.vPos.y);
		temp.vNextPoint = D3DXVECTOR2(m_pTarget->GetInfo()->vPos.x + 50.f, m_pTarget->GetInfo()->vPos.y);

		m_listLine.push_back(new CLine(temp));
	}
	return E_NOTIMPL;
}

int CGuardianBodyB::Progress(void)
{
	Frame();
	CObj::Matrix();

	ChangeState();
	makeNewLine();
	
	if (m_bIsMove)
		Moving();

	CObj::CollisionLock();

	return 0;
}

void CGuardianBodyB::Render(void)
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

	Render_Rect();
}

void CGuardianBodyB::Release(void)
{
	for_each(m_listLine.begin(), m_listLine.end(), DeleteObj());
	m_listLine.clear();
}
