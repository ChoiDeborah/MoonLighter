// MyForm.cpp : ���� �����Դϴ�.
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


// CMyForm �����Դϴ�.

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


// CMyForm �޽��� ó�����Դϴ�.


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}


void CMyForm::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CFormView::OnDropFiles(hDropInfo);
}


void CMyForm::OnBnClickedMapTool()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if (NULL == m_MapTool.GetSafeHwnd())
	{
		m_MapTool.Create(IDD_MAPTOOL);
	}
	m_MapTool.ShowWindow(SW_SHOW);

}


void CMyForm::OnBnClickedObjectTool()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if (NULL == m_ObjectTool.GetSafeHwnd())
	{
		m_ObjectTool.Create(IDD_OBJECTTOOL);
	}
	m_ObjectTool.ShowWindow(SW_SHOW);
}


void CMyForm::OnBnClickedSpriteTool()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (NULL == m_SpriteTool.GetSafeHwnd())
	{
		m_SpriteTool.Create(IDD_SPRITETOOL);
	}
	m_SpriteTool.ShowWindow(SW_SHOW);
}


void CMyForm::OnBnClickedPathFindTool()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (NULL == m_PathFindTool.GetSafeHwnd())
	{
		m_PathFindTool.Create(IDD_PATHFINDTOOL);
	}

	m_PathFindTool.ShowWindow(SW_SHOW);

}
