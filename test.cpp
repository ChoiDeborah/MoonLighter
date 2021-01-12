// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"
#include "MainFrm.h"


// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM)
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CMyForm::OnBnClickedMapTool)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyForm::OnBnClickedObjectTool)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyForm::OnBnClickedSpriteTool)
	ON_BN_CLICKED(IDC_BUTTON4, &CMyForm::OnBnClickedPathFindTool)
END_MESSAGE_MAP()


// CMyForm 진단입니다.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm 메시지 처리기입니다.


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void CMyForm::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFormView::OnDropFiles(hDropInfo);
}


void CMyForm::OnBnClickedMapTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (NULL == m_MapTool.GetSafeHwnd())
	{
		m_MapTool.Create(IDD_MAPTOOL);
	}
	m_MapTool.ShowWindow(SW_SHOW);

}


void CMyForm::OnBnClickedObjectTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (NULL == m_ObjectTool.GetSafeHwnd())
	{
		m_ObjectTool.Create(IDD_OBJECTTOOL);
	}
	m_ObjectTool.ShowWindow(SW_SHOW);
}


void CMyForm::OnBnClickedSpriteTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (NULL == m_SpriteTool.GetSafeHwnd())
	{
		m_SpriteTool.Create(IDD_SPRITETOOL);
	}
	m_SpriteTool.ShowWindow(SW_SHOW);
}


void CMyForm::OnBnClickedPathFindTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (NULL == m_PathFindTool.GetSafeHwnd())
	{
		m_PathFindTool.Create(IDD_PATHFINDTOOL);
	}

	m_PathFindTool.ShowWindow(SW_SHOW);

}
