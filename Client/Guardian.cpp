#include "stdafx.h"
#include "Guardian.h"

#include "GraphicDev.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"

#include "TimeMgr.h"

#include "Player.h"
#include "GuardianIntro.h"
#include "ObjMgr.h"

#include "LineMgr.h"
#include "Line.h"

#include "GuardianBodyA.h"



CGuardian::CGuardian():
	m_eCurState(STATE_END)
	, m_eNextState(STATE_END)
	, m_eCurPattern(PATTERN_END)
	, m_eNextPattern(PATTERN_IDLE)
	, m_fAnimationTime(0.f)
	, m_bStartStage(false)
	, m_bIsIntro(true)
	, m_bScroll(false)
	, m_fDirTime(0.f)
	, m_iRandInterval(0)
	, m_iRandDir(0)
	, m_fLineTime(0.f)
	, m_fAngle(0)
	, m_fRotateTime(0.f)
{
	m_tInfo.vPos = D3DXVECTOR3(BOSSMAP_1CX / 2 - 60.f + 35.f, 670.f, 0.f);
	PrevPoint = D3DXVECTOR2(0.f, 0.f);
	CurPoint = D3DXVECTOR2(m_tInfo.vPos.x, m_tInfo.vPos.y);
	m_wstrObjKey = L"BOSS_C_HEAD";
	m_wstrStateKey = L"IDLE_UP";
	m_tFrame = FRAME(0.f, 10.f, 10.f);
	m_tInfo.fCX = 150.f;
	m_tInfo.fCY = 150.f;


	CGuardianBodyA* pGuardianBodyA = new CGuardianBodyA;
	pGuardianBodyA->Set_Target(this);
	
	pGuardianBodyA->SetPos(m_tInfo.vPos.x + 100.f, m_tInfo.vPos.y + 60.f);
	pGuardianBodyA->Initialize();

	CObjMgr::Get_Instance()->AddObject(OBJID::OBJ_BODY_A, pGuardianBodyA);
	m_pChild = pGuardianBodyA;

	LINEINFO temp;

	temp.vPrevPoint = D3DXVECTOR2(m_tInfo.vPos.x + 100.f, m_tInfo.vPos.y + 60.f);
	temp.vNextPoint = D3DXVECTOR2(m_tInfo.vPos.x + 50.f, m_tInfo.vPos.y);

	CLineMgr::Get_Instance()->Get_ListLine()->push_back(new CLine(temp));

}


CGuardian::~CGuardian()
{
	Release();
}

HRESULT CGuardian::Initialize(void)
{
	
	return E_NOTIMPL;
}

int CGuardian::Progress(void)
{
	if(m_bStartStage)
		Frame();

	Matrix();

	ChangeState();
	UpdatePattern();
	// 패턴 전환
	SwitchPattern();
	CollisionLock();
	makeNewLine();
	
	if(m_bIsMove)
		Moving();
	
	ChangeDirection();


	if (m_bCollision == false)
	{
		if (m_bRandNum == false)
		{
			m_bRandNum = true;
			m_fTempAngle = m_fAngle;
			m_iRandNum = rand() % 2;
			m_iRandOffSet = rand() % 2;
			
		}
		Rotate(m_iRandNum);
	}
	if (m_bStartStage == false)
		CheckPlayerPosition();
	
	return S_OK;
}

void CGuardian::Render(void)
{
	CObj::UpdateRect();

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
		D3DCOLOR_ARGB(255, 255, 255, 255)
	);
	
	Render_Rect();
}

void CGuardian::Release(void)
{
}

void CGuardian::Frame()
{
	m_tFrame.fFrame += m_tFrame.fCount * GET_TIME;

	if (m_tFrame.fFrame > m_tFrame.fMax)
	{
		m_tFrame.fFrame = 0.f;

		if (m_eCurState == INTRO)
			m_bIsIntro = false;
	}
}

void CGuardian::Matrix()
{
	D3DXMATRIX matTrans, matScale, matRotZ;

	D3DXMatrixIdentity(&matScale);

	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x + CScrollMgr::Get_ScrollX(),
		m_tInfo.vPos.y + CScrollMgr::Get_ScrollY(),
		0.f);

	D3DXMatrixScaling(&matScale, 1.5f, 1.5f, 0.f);

	D3DXVECTOR3 vDiff = (m_tInfo.vPos+ m_tInfo.vDir) - m_tInfo.vPos;

	//D3DXMatrixRotationZ(&matRotZ, atan2f(vDiff.y, vDiff.x));
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fAngle));

	m_tInfo.matWorld = matScale * matRotZ* matTrans;
}


void CGuardian::Render_Rect()
{
	D3DXVECTOR2			vPoint[5] = 
	{
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

void CGuardian::makeNewLine()
{
	m_fLineTime += GET_TIME;
	if (m_fLineTime > LINETIME)
	{
		m_fLineTime = 0.f;
		PrevPoint = CurPoint;
		CurPoint.x = m_tInfo.vPos.x;
		CurPoint.y = m_tInfo.vPos.y;

		LINEINFO temp;

		temp.vPrevPoint = PrevPoint;
		temp.vNextPoint = CurPoint;
	

		CLineMgr::Get_Instance()->Get_ListLine()->push_back(new CLine(temp));
		m_eCurDir = m_eNextDir;
	}
}

void CGuardian::ChangeState()
{
	if (m_eCurState != m_eNextState)
	{
		m_tFrame.fFrame = 0.f;
		
		switch (m_eNextState)
		{
		case CGuardian::INTRO:
			m_wstrStateKey = L"INTRO";
			break;
		case CGuardian::IDLE_UP:
			m_wstrStateKey = L"IDLE_UP";
			break;
		case CGuardian::IDLE_DOWN:
			m_wstrStateKey = L"IDLE_DOWN";
			break;
		case CGuardian::IDLE_RIGHT:
			m_wstrStateKey = L"IDLE_RIGHT";
			break;
		case CGuardian::IDLE_LEFT:
			m_wstrStateKey = L"IDLE_LEFT";
			break;
		case CGuardian::CHARGE_UP:
			m_wstrStateKey = L"CHARGE_UP";
			break;
		case CGuardian::CHARGE_DOWN:
			m_wstrStateKey = L"CHARGE_DOWN";
			break;
		case CGuardian::CHARGE_RIGHT:
			m_wstrStateKey = L"CHARGE_RIGHT";
			break;
		case CGuardian::CHARGE_LEFT:
			m_wstrStateKey = L"CHARGE_LEFT";
			break;
		case CGuardian::CRASH_UP:
			m_wstrStateKey = L"CRASH_UP";
			break;
		case CGuardian::CRASH_DOWN:
			m_wstrStateKey = L"CRASH_DOWN";
			break;
		case CGuardian::CRASH_RIGHT:
			m_wstrStateKey = L"CRASH_RIGHT";
			break;
		case CGuardian::CRASH_LEFT:
			m_wstrStateKey = L"CRASH_LEFT";
			break;
		case CGuardian::STTACK_UP:
			m_wstrStateKey = L"STTACK_UP";
			break;
		case CGuardian::STTACK_DOWN:
			m_wstrStateKey = L"STTACK_DOWN";
			break;
		case CGuardian::STTACK_RIGHT:
			m_wstrStateKey = L"STTACK_RIGHT";
			break;
		case CGuardian::STTACK_LEFT:
			m_wstrStateKey = L"STTACK_LEFT";
			break;
		case CGuardian::DEATH:
			m_wstrStateKey = L"BOSS_C_HEAD_DEATH";
			break;
		case CGuardian::STATE_END:

			break;
		default:
			break;
		}
		m_eCurState = m_eNextState;
		m_tFrame.fMax = CTextureMgr::Get_Instance()->GetImgCount(m_wstrObjKey, m_wstrStateKey) - 1;
	}
}

void CGuardian::UpdatePattern()
{
	switch (m_eCurPattern)
	{
	case CGuardian::PATTERN_IDLE:

		break;
	case CGuardian::PATTERN_INTRO:
		
		break;
	case CGuardian::PATTERN_SCROLL:

		break;
	case CGuardian::PATTERN_CHARGE:

		break;
	case CGuardian::PATTERN_CRASH:
		break;
	case CGuardian::PATTERN_STTACH:
		break;
	case CGuardian::PATTERN_DEATH:
		break;
	case CGuardian::PATTERN_END:
	
		break;
	default:
		break;
	}
}

void CGuardian::SwitchPattern()
{
	if (m_eCurPattern != m_eNextPattern)
	{
		switch (m_eNextPattern)
		{
		case CGuardian::PATTERN_INTRO:
			//pIntro = new CGuardianIntro;
			//pIntro->Set_Target(this);
			//CObjMgr::Get_Instance()->AddObject(OBJID::OBJ_BOSS_B_INTRO, pIntro);
			break;
		case CGuardian::PATTERN_IDLE:
			//AllMoveStop();
			break;
		case CGuardian::PATTERN_CHARGE:

			break;
		case CGuardian::PATTERN_CRASH:

			break;
		case CGuardian::PATTERN_STTACH:

			break;
		case CGuardian::PATTERN_DEATH:

			break;
		case CGuardian::PATTERN_END:
		
			break;
		default:
			break;
		}

		m_eCurPattern = m_eNextPattern;
	}
}

void CGuardian::CheckPlayerPosition()
{
	/*if (nullptr != m_pTarget)
	{
		CPlayer * pPlayer = dynamic_cast<CPlayer*>(m_pTarget);
		m_vTargetPos = m_pTarget->GetInfo()->vPos;

		if (m_tInfo.vPos.y + 450.f >= m_vTargetPos.y)
		{
			pPlayer->Set_Movable(false);
			pPlayer->Set_Player_StateKey(L"Field_Idle_Up");
			m_eNextPattern = PATTERN_INTRO;
		}
	}*/
}

void CGuardian::AllMoveStop()
{
	m_bIsMove = false;
	if (m_pChild)
		m_pChild->AllMoveStop();
}

void CGuardian::AllMoveActivate()
{
	m_bIsMove = true;
	if (m_pTarget)
		m_pTarget->AllMoveActivate();
}

void CGuardian::Moving()
{
	m_fDirTime += GET_TIME;

	m_iRandInterval = rand() % 25 + 15;

	//cout << m_fAngle <<"  " <<(int)m_fAngle % 360 << endl;
	if (m_fDirTime > m_iRandInterval* 0.1f && m_bCollision)
	{
		m_fDirTime = 0.f;
		
		int iRand = rand() % 80 - 40;
		
		m_fAngle = m_fAngle + iRand;

		//m_fAngle = m_fAngle + 10;
		//if (m_fAngle > 360)
		//	m_fAngle = 0.f;

		//// 랜덤한 방향 생성D3DXToRadian
		//MakeNewDirection();
		//
		//switch (DIRECTION(m_iRandDir))
		//{
		//case CObj::UP:
		//	m_eNextDir = UP;
		//	break;
		//case CObj::DOWN:
		//	m_eNextDir = DOWN;
		//	break;
		//case CObj::RIGHT:
		//	m_eNextDir = RIGHT;
		//	break;
		//case CObj::LEFT:
		//	m_eNextDir = LEFT;
		//	break;
		//case CObj::UP_LEFT:
		//	m_eNextDir = UP_LEFT;
		//	break;
		//case CObj::UP_RIGHT:
		//	m_eNextDir = UP_RIGHT;
		//	break;
		//case CObj::DOWN_LEFT:
		//	m_eNextDir = DOWN_LEFT;
		//	break;
		//case CObj::DOWN_RIGHT:
		//	m_eNextDir = DOWN_RIGHT;
		//	break;
		//case CObj::DIRICTION_END:
		//	break;
		//default:
		//	break;
		//}
	}
	//cout <<"Angle" <<  m_fAngle << endl;
	m_tInfo.vPos.x = m_tInfo.vPos.x + cosf(D3DXToRadian(m_fAngle)) * m_fSpeed * GET_TIME;
	m_tInfo.vPos.y = m_tInfo.vPos.y + sinf(D3DXToRadian(m_fAngle)) * m_fSpeed * GET_TIME;

}

void CGuardian::DirectionReverse()
{
	m_tInfo.vDir *= -1;
}

void CGuardian::MakeNewDirection()
{
	m_iRandDir = rand() % (int)DIRICTION_END;
}

void CGuardian::ChangeDirection()
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
			m_tInfo.vDir = D3DXVECTOR3(0.f, 0.f, 0.f);
			break;
		default:
			break;
		}

	}


}

void CGuardian::Rotate(int iDir)
{
	m_fRotateTime += GET_TIME;

	if (m_fRotateTime > 0.5f)
	{
		
		if (iDir == 0)
		{
			if (m_iRandOffSet == 0)
			{
				if (m_fTempAngle + 180.f > m_fAngle)
					m_fAngle += 5.f;
			}
			else
			{
				if (m_fTempAngle + 90.f > m_fAngle)
					m_fAngle += 5.f;
			}
		}
		else
		{
			if (m_iRandOffSet == 0)
			{
				if (m_fTempAngle - 180.f < m_fAngle)
					m_fAngle -= 5.f;

			}
			else
			{
				if (m_fTempAngle - 90.f > m_fAngle)
					m_fAngle += 5.f;
			}
		}
	}
}

void CGuardian::CollisionLock()
{
	if (m_bCollision == false)
	{
		m_bHitColor = true;

		m_fCollisionTime += GET_TIME;

		if (m_fCollisionTime > 0.5)
			m_bHitColor = false;

		if (m_fCollisionTime > 1.5f)
		{
			m_fCollisionTime = 0.f;
			m_iRandOffSet = 0.f;
			m_bCollision = true;
			m_bRandNum = false;
		}
	}
}

