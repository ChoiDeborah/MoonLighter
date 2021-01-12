#include "stdafx.h"
#include "Golem.h"

#include "ScrollMgr.h"
#include "TextureMgr.h"
#include "GraphicDev.h"
#include "TimeMgr.h"

#include "Rock.h"
#include "Punch.h"
#include "ObjMgr.h"
#include "ObjFactory.h"

#include "Player.h"
#include "Wave.h"

#include "SoundMgr.h"



CGolem::CGolem()
	: m_eCurState(STATE_END)
	, m_eNextState(IDLE)
	, m_fTime(0.f)
	, m_eCurAnimation(ANI_END)
	, m_eNextAnimation(ANI_END)
	, m_fAnimationTime(0.f)
	, m_bCreateRock(false)
	, m_pPlayer(nullptr)
	, m_bStartStage(false)
	, m_iOriginScrollX(0)
	, m_iOriginScrollY(0)
	, m_bScroll(false)
	, m_bStageStart(false)
	, m_fAttackTime(0.f)
	, m_fPatternTime(0.f)
	, m_fPatternInterver(2.f)
	, m_bPatternChange(false)
	, m_bStartPattern(false)
	, m_iHp(1000)
	, m_iMaxHp(1000)
	, m_bIsWave(false)
	, m_fWaveTime(0.f)
{
	m_fAngle = 0.f;
}


CGolem::~CGolem()
{
	//Safe_Delete(m_pTexture);
}

HRESULT CGolem::Initialize(void)
{
	m_tInfo.vPos = D3DXVECTOR3(BOSSMAP_1CX / 2 + 35.f, 400.f, 0.f);
	m_tInfo.fCX = 190.f;
	m_tInfo.fCY = 150.f;


	m_wstrObjKey = L"BOSS_A";
	m_tFrame = FRAME(0.f, 10.f, 10.f);


	return S_OK;
}

int CGolem::Progress(void)
{
	Frame();
	Matrix();
	
	ChangeState();
	ChangeAnimation();

	// 일정 시간 따라 패턴 바꿔주는 얘
	SwitchPattern();
	// 애니메이션 따라서 재생해 주는 얘
	UpdatePattern();

	CObj::CollisionLock();

	if (m_bStartStage == false)
	{
		CheckPlayerPosition();
	}

	if (m_bStageStart && m_iHp > 0)
	{
		SwitchPattern();
	}

	if (m_iHp <= 0)
	{
		m_eNextState = STATE::DEATH;
	}

	if (m_bIsDead) 
	{
		CSoundMgr::Get_Instance()->PlaySoundW(L"golem_dungeon_king_golem_death.wav", CSoundMgr::GOLEM);
		return 1;
	}
	return 0;
}

void CGolem::Render(void)
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
	if (m_bHitColor == false) 
	{
		
		CGraphicDev::Get_Instance()->GetSprite()->Draw(m_pTexture->pTexture,
			NULL,
			&D3DXVECTOR3(fX, fY, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else
	{
		CGraphicDev::Get_Instance()->GetSprite()->Draw(m_pTexture->pTexture,
			NULL,
			&D3DXVECTOR3(fX, fY, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 0, 0));
	}

	//Render_Rect();
}

void CGolem::Release(void)
{
}

void CGolem::Render_Rect(void)
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

void CGolem::Frame(void)
{
	m_tFrame.fFrame += m_tFrame.fCount * GET_TIME;

	if (m_tFrame.fFrame > m_tFrame.fMax)
	{
		m_tFrame.fFrame = 0.f;

		if (m_eCurAnimation == ANI_WAKEUP && m_eCurState == WAKEUP)
		{
			m_eNextAnimation = ANI_SCROLL;
			m_eNextState = IDLE;
		}

		if (m_eCurState == DEATH)
		{
			m_bIsDead = true;
		}
	}
}

void CGolem::Matrix(void)
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

void CGolem::ChangeState(void)
{
	if (m_eCurState != m_eNextState)
	{
		m_tFrame.fFrame = 0;
		m_fAttackTime = 0.f;

		switch (m_eNextState)
		{
		case CGolem::IDLE:
			m_wstrStateKey = L"IDLE";
			m_tFrame.fCount = 10.f;
			break;
		case CGolem::DEATH:
			m_wstrStateKey = L"DEATH";
			break;
		case CGolem::LAUNCH:
			m_wstrStateKey = L"LAUNCH";
			break;
		case CGolem::NOARM_IDLE:
			m_wstrStateKey = L"NOARM_IDLE";
			break;
		case CGolem::RECOVER:
			m_wstrStateKey = L"RECOVER";
			break;
		case CGolem::SPAWN:
			m_wstrStateKey = L"SPAWN";
			break;
		case CGolem::WAKEUP:
			m_wstrStateKey = L"WAKEUP";
			m_tFrame.fCount = 8.f;
			break;
		case CGolem::STATE_END:

			break;
		default:
			break;
		}
		m_tFrame.fMax = (float)CTextureMgr::Get_Instance()->GetImgCount(m_wstrObjKey, m_wstrStateKey) - 1;
	
		m_eCurState = m_eNextState;
	}
}

void CGolem::Create_Rock()
{
	int iCreatePattern = rand() % 2;

	// 막 뿌리는 거
	if (iCreatePattern == 0)
	{
		int iNumOfRock = rand() % 30 + 20;
		int iRandY = 0;
		int iRandX = 0;
		
		for (int i = 0; i < iNumOfRock; ++i)
		{
			iRandY = rand() % 20 + 10;
			iRandX = rand() % 43 + 5;
			CRock* pRock = new CRock;
			pRock->SetPos(24 * 1.5 * iRandX, 24 * 1.5 * iRandY);;
			pRock->SetOriginPosY();
			CObjMgr::Get_Instance()->AddObject(OBJ_ROCK, pRock);
		}
	}

	// 원형으로 뿌리는 거 
	// 일정 간격으로 생성해야 함
	if (iCreatePattern == 1)
	{
		for (int j = 0; j < 8; ++j)
		{
			int iDistance = j * 65.f  + 220.f;
			for (int i = 0; i < 7; ++i)
			{
				m_fAngle = 180.f + i* 30.f;
			
				float fPosX = m_tInfo.vPos.x + cosf(D3DXToRadian(m_fAngle)) * iDistance;
				float fPosY = m_tInfo.vPos.y + -sinf(D3DXToRadian(m_fAngle)) * iDistance;

				CRock* pRock = new CRock;
				pRock->SetPos(fPosX, fPosY);
				pRock->SetOriginPosY();
				CObjMgr::Get_Instance()->AddObject(OBJ_ROCK, pRock);

			}
		}
	}

}

void CGolem::ChangeAnimation(void)
{
	if (m_eCurAnimation != m_eNextAnimation)
	{
		switch (m_eNextAnimation)
		{
		case CGolem::ANI_WAKEUP:
			m_eNextState = WAKEUP;
			CSoundMgr::Get_Instance()->PlaySoundW(L"golem_dungeon_king_golem_awake.wav", CSoundMgr::GOLEM);
			break;
		case CGolem::ANI_SCROLL:
			CSoundMgr::Get_Instance()->PlayBGM(L"golem_boss_track.wav");
			break;
		case CGolem::ANI_IDLE:
			break;
		case CGolem::ANI_SPAWN:
			break;
		case CGolem::ANI_ARM:
			break;
		case CGolem::ANI_ROCK:

			break;
		default:
			break;
		}

		m_eCurAnimation = m_eNextAnimation;
	}

}

void CGolem::SwitchPattern()
{
	// 현재 애니메이션과 다음 애니메이션이 다를 때 까지
	if(m_bStartPattern)
	{
		while (m_eCurAnimation == m_eNextAnimation)
		{
			// 패턴 시작변수 거짓
			m_bStartPattern = false;
			// 패턴 난수 생성
			int iRandPattern = rand() % 2;
			// 애니메이션 시간 0으로 초기화
			m_fAnimationTime = 0.f;

			// 난수 값 따라 애니메이션 결정
			switch (iRandPattern)
			{
			case 0:
				m_eNextAnimation = ANIMATION_PATTERN::ANI_ROCK;
				//cout << "Rock" << endl;
				break;
			case 1:
				m_eNextAnimation = ANIMATION_PATTERN::ANI_ARM;
				//cout << "Punch" << endl;
				break;
			default:
				break;
			}
		}
		m_bIsRock = false;
		m_bIsLanch = false;
	}

}


void CGolem::CreatePunch()
{
	CPunch* pPunch = new CPunch;
	pPunch->SetPos(600.f, 600.f);
	pPunch->Set_OriginPosition(m_tInfo.vPos.x, m_tInfo.vPos.y);
	pPunch->Set_Player(dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Object(OBJID::OBJ_PLAYER)->front()));
	
	CObjMgr::Get_Instance()->AddObject(OBJID::OBJ_PUNCH, pPunch);
}

void CGolem::CheckPlayerPosition()
{
	if (nullptr != m_pPlayer)
	{
		m_vPlayerPos = m_pPlayer->GetInfo()->vPos;

		if (m_tInfo.vPos.y + 450.f >= m_vPlayerPos.y)
		{
			m_pPlayer->Set_Movable(false);
			m_pPlayer->Set_Player_StateKey(L"Field_Idle_Up");
			m_bStartStage = true;
			m_eNextAnimation = ANI_WAKEUP;
		}
	}
}

void CGolem::UpdatePattern()
{
	m_fAnimationTime += GET_TIME;
	//cout <<"Time : "<< m_fAnimationTime << endl;
	
	m_fWaveTime += GET_TIME;
	
	
	if (m_fWaveTime > 5.f)
	{
		m_fWaveTime = 0.f;
		CObjMgr::Get_Instance()->AddObject(OBJID::OBJ_WAVE, CObjFactory<CWave>::CreateObj(m_tInfo.vPos));
		CSoundMgr::Get_Instance()->PlaySoundW(L"golem_dungeon_king_golem_wave", CSoundMgr::WAVE);
	}

	switch (m_eCurAnimation)
	{
	case CGolem::ANI_SCROLL:
		if (m_eCurState == IDLE)
		{
			m_fTime += GET_TIME;

			if (m_fTime < 1.f)
			{
				if (CScrollMgr::Get_ScrollX() > BOSSMAP_1CX / 2 + 35.f)
				{
					CScrollMgr::Set_ScrollX(-250.f* GET_TIME);
				}

				if (CScrollMgr::Get_ScrollY() > m_iOriginScrollY)
				{
					CScrollMgr::Set_ScrollY(-250.f* GET_TIME);
				}
			}
			// 캐릭터 움직일 수 있게
			else 
			{
				m_pPlayer->Set_Movable(true);
				m_eNextAnimation = ANI_IDLE;
				m_bStageStart = true;
				m_bStartPattern = true;
			}
		}
		break;
	case CGolem::ANI_WAKEUP:
		if (m_bScroll == false)
		{
			m_bScroll = true;
			m_iOriginScrollX = CScrollMgr::Get_ScrollX();
			m_iOriginScrollY = CScrollMgr::Get_ScrollY();
		}
		if (CScrollMgr::Get_ScrollX() < -326)
		{
			CScrollMgr::Set_ScrollX(250.f * GET_TIME);
		}
		if (CScrollMgr::Get_ScrollY() < -170)
		{
			CScrollMgr::Set_ScrollY(250.f * GET_TIME);
		}
		break;
	case CGolem::ANI_SPAWN:

		break;
	case CGolem::ANI_ARM:
	{
		// 팔 생성하는 패턴
		
		if (m_fAnimationTime < 3 && m_bIsLanch == false)
		{
			m_bIsLanch = true;
			m_eNextState = LAUNCH;
			CreatePunch();
			//cout << "Create Punch" << endl;
		}
		if (m_fAnimationTime < 1.5)
		{
			m_eNextState = NOARM_IDLE;
		}

		if (m_fAnimationTime > 10.f)
		{
			m_eNextState = RECOVER;
		}

		if (m_fAnimationTime > 13.f)
			m_eNextState = IDLE;

		if (m_fAnimationTime > 15.f)
		{
			m_bStartPattern = true;
		}

	}
		break;
	case CGolem::ANI_ROCK:
		// 바위 생성하는 패턴
		if (m_fAnimationTime < 3 && m_bIsRock == false)
		{
			m_bIsRock = true;
			m_eNextState = SPAWN;
			//cout << "Create Rock" << endl;
			Create_Rock();
		}

		if (m_fAnimationTime > 4 && m_bIsRock)
		{
			m_eNextState = IDLE;
		}

		if (m_fAnimationTime > 15.f)
		{
			m_bStartPattern = true;
		}
		
		break;
	case CGolem::ANI_END:

		break;
	case CGolem::ANI_IDLE:

		break;
	default:
		break;
	}
}

