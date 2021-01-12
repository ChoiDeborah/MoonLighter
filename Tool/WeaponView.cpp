// WeaponView.cpp : ���� �����Դϴ�.
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


// CWeaponView �׸����Դϴ�.

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

	// ��������Ʈ ���� ���� �Ǿ��� ��
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

	
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


// CWeaponView �����Դϴ�.

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


// CWeaponView �޽��� ó�����Դϴ�.


void CWeaponView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}


void CWeaponView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (GetAsyncKeyState(VK_LBUTTON))
		OnLButtonDown(nFlags, point);

	CView::OnMouseMove(nFlags, point);
}


void CWeaponView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
