// WeaponView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "WeaponView.h"

#include "GraphicDev.h"
#include "Back.h"
#include "MainFrm.h"
#include "TimeMgr.h"


#define SPRITE_TIME 100

// CWeaponView

IMPLEMENT_DYNCREATE(CWeaponView, CView)

CWeaponView::CWeaponView()
	:m_bPlayPlayerSprite(true)
	, m_bPlayWeaponSprite(true)
	, m_iPlayerSpriteNum(0)
	, m_iWeaponSpriteNum(0)
	, m_iIndexPos(1)
{

}

CWeaponView::~CWeaponView()
{
}

BEGIN_MESSAGE_MAP(CWeaponView, CView)
	ON_WM_TIMER()
	ON_WM_MOUSEACTIVATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CWeaponView 그리기입니다.

void CWeaponView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	g_hWnd = m_hWnd;
	//cout << "Weapon"<< g_hWnd << endl;


	CBack*		pBackGround = ((CMainFrame*)AfxGetMainWnd())->Get_MainView()->Get_BackGround();
	pBackGround->Set_WeaponView(this);

	if (nullptr == pBackGround)
		return;

	CGraphicDev::Get_Instance()->Render_Begin();


	SetTimer(SPRITE_TIME, 1000 / 5, NULL);



	if (m_tFramePlayer.fFrame > m_tFramePlayer.fMax)
		m_tFramePlayer.fFrame = 0.f;

	if (m_tFrameWeapon.fFrame > m_tFrameWeapon.fMax)
		m_tFrameWeapon.fFrame = 0.f;

	// 스프라이트 상태 선택 되었을 때
	if (!m_bPlayPlayerSprite) 
	{
		pBackGround->Render_Player(L"PLAYER", m_Player_StateKey, m_iPlayerSpriteNum);
	}
	else if (m_bPlayPlayerSprite) {
		//cout << "Max Frame : " << m_tFramePlayer.fMax << "Cur Frame : " << m_tFramePlayer.fFrame << endl;
		pBackGround->Render_Player(L"PLAYER", m_Player_StateKey, m_tFramePlayer.fFrame);
	}

	if (!m_bPlayWeaponSprite)
	{
		pBackGround->Render_Weapon(L"WEAPON2", m_Weapon_StateKey, m_iWeaponSpriteNum);
	}
	else if (m_bPlayWeaponSprite) {
		//cout << "Max Frame : " << m_tFrameWeapon.fMax << "Cur Frame : " << m_tFrameWeapon.fFrame << endl;
		pBackGround->Render_Weapon(L"WEAPON2", m_Weapon_StateKey, m_tFrameWeapon.fFrame);
	}


	pBackGround->Render_Center();

	CGraphicDev::Get_Instance()->Render_End(g_hWnd);

	D3DXVECTOR2			vPoint[4] = {
		D3DXVECTOR2(400, 300),
		D3DXVECTOR2(500, 400),
		D3DXVECTOR2(300, 400),
		D3DXVECTOR2(400, 300)
	};

	
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CWeaponView 진단입니다.

#ifdef _DEBUG
void CWeaponView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CWeaponView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CWeaponView 메시지 처리기입니다.


void CWeaponView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case SPRITE_TIME:
		ChangeFrame();
		Invalidate(FALSE);
		break;
	default:
		break;
	}
	CView::OnTimer(nIDEvent);
}

void CWeaponView::ChangeFrame()
{
	m_tFramePlayer.fFrame += 1;

	if (m_tFramePlayer.fFrame > m_tFramePlayer.fMax)
		m_tFramePlayer.fFrame = 0.f;

	m_tFrameWeapon.fFrame += 1;

	if (m_tFrameWeapon.fFrame > m_tFrameWeapon.fMax)
		m_tFrameWeapon.fFrame = 0.f;
}


int CWeaponView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}


void CWeaponView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (GetAsyncKeyState(VK_LBUTTON))
		OnLButtonDown(nFlags, point);

	CView::OnMouseMove(nFlags, point);
}


void CWeaponView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	cout << "Mouse X : " << point.x << " Mouse Y :" << point.y << endl;
	CBack*		pBackGround = ((CMainFrame*)AfxGetMainWnd())->Get_MainView()->Get_BackGround();
	if (nullptr == pBackGround)
		return;
	
	if(m_iIndexPos)
		pBackGround->Set_PosWeapon(point.x, point.y);
	if (!m_iIndexPos)
		pBackGround->Set_PosPlayer(point.x, point.y);
	CView::OnLButtonDown(nFlags, point);
}

void CWeaponView::Set_WeaponScale(int sacle)
{
	CBack*		pBackGround = ((CMainFrame*)AfxGetMainWnd())->Get_MainView()->Get_BackGround();
	if (nullptr == pBackGround)
		return;

	pBackGround->Set_WeaponScale(sacle);
}

void CWeaponView::Set_PlayerScale(int scale)
{
	CBack*		pBackGround = ((CMainFrame*)AfxGetMainWnd())->Get_MainView()->Get_BackGround();
	if (nullptr == pBackGround)
		return;

	pBackGround->Set_PlayerScale(scale);
}
