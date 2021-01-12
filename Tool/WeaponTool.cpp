// WeaponTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "WeaponTool.h"
#include "afxdialogex.h"
#include "WeaponView.h"
#include "Texture.h"
#include "TextureMgr.h"


// CWeaponTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CWeaponTool, CDialog)

CWeaponTool::CWeaponTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_WEAPONTOOL, pParent)
	, m_iComboPosIndex(0)
{

}

CWeaponTool::~CWeaponTool()
{
}

void CWeaponTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox_Player);
	DDX_Control(pDX, IDC_LIST4, m_ListBox_Weapon);
	DDX_Control(pDX, IDC_COMBO3, m_ComboBox_Position);
	DDX_Control(pDX, IDC_SLIDER1, m_SliderCtrl_PlayerScale);
	DDX_Control(pDX, IDC_SLIDER2, m_SliderCtrl_WeaponScale);
}


BEGIN_MESSAGE_MAP(CWeaponTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CWeaponTool::OnLbnSelchangePlayerList)
	ON_LBN_SELCHANGE(IDC_LIST4, &CWeaponTool::OnLbnSelchangeWeaponList)
	ON_BN_CLICKED(IDC_BUTTON8, &CWeaponTool::OnBnClickedButtonReplay)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CWeaponTool::OnCbnSelchangeComboPosition)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &CWeaponTool::OnNMReleasedcaptureSliderPlayerScale)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER2, &CWeaponTool::OnNMReleasedcaptureSliderWeaponScale)
END_MESSAGE_MAP()


// CWeaponTool �޽��� ó�����Դϴ�.


BOOL CWeaponTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_SliderCtrl_WeaponScale.SetRange(0, 100);
	m_SliderCtrl_PlayerScale.SetRange(0, 100);
	m_SliderCtrl_WeaponScale.SetPos(50);
	m_SliderCtrl_PlayerScale.SetPos(50);

	m_SliderCtrl_PlayerScale.SetTicFreq(10);
	m_SliderCtrl_WeaponScale.SetTicFreq(10);

	pContext.m_pNewViewClass = RUNTIME_CLASS(CWeaponView);

	m_pWeaponView = (CWeaponView *)((CFrameWnd*)this)->CreateView(&pContext);

	ASSERT(m_pWeaponView);

	RECT r = { 530, 50, 730, 250 };
	m_pWeaponView->ShowWindow(SW_SHOW);
	m_pWeaponView->MoveWindow(&r);

	
	// 
	wifstream		LoadFile;

	LoadFile.open(L"../Data/ImgPath.txt", ios::in);

	TCHAR		szObjKey[MIN_STR] = L"";
	TCHAR		szStateKey[MIN_STR] = L"";
	TCHAR		szCount[MIN_STR] = L"";
	TCHAR		szImgPath[MAX_PATH] = L"";

	wstring		wstrCombine = L"";

	// ���� �ִ� �� ���� �ʿ� �ִ°� ������
	while (!LoadFile.eof())
	{
		LoadFile.getline(szObjKey, MIN_STR, '|');
		LoadFile.getline(szStateKey, MIN_STR, '|');
		LoadFile.getline(szCount, MIN_STR, '|');
		LoadFile.getline(szImgPath, MAX_PATH);

		wstrCombine = wstring(szObjKey) + L"|" + wstring(szStateKey) + L"|";
		wstrCombine += szCount;
		wstrCombine += wstring(szImgPath);
	}
	// �� �ؽ��Ŀ� �ִ°� �ֱ�

	map<wstring, CTexture*> *MapTexture = CTextureMgr::Get_Instance()->Get_MapTexture();

	auto iter = MapTexture->find(L"PLAYER");

	// Obj Key ã��
	if (iter == MapTexture->end())
		return FALSE;

	// ����Ʈ �ڽ� ����
	m_ListBox_Player.ResetContent();

	map<wstring, vector<TEXINFO*>> * MapMultiTexture = nullptr;
	MapMultiTexture = dynamic_cast<CMultiTexture*>(iter->second)->Get_MapMultiTexture();

	for (auto & iter2 : *MapMultiTexture)
	{
		m_ListBox_Player.AddString(iter2.first.c_str());
	}



	// Weapon Key
	iter = MapTexture->find(L"WEAPON2");

	// Obj Key ã��
	if (iter == MapTexture->end())
		return FALSE;

	// ����Ʈ �ڽ� ����
	m_ListBox_Weapon.ResetContent();

	MapMultiTexture = nullptr;
	MapMultiTexture = dynamic_cast<CMultiTexture*>(iter->second)->Get_MapMultiTexture();

	// State Key
	for (auto & iter2 : *MapMultiTexture)
	{
		m_ListBox_Weapon.AddString(iter2.first.c_str());
	}

	//
	m_ComboBox_Position.AddString(L"PLAYER");
	m_ComboBox_Position.AddString(L"WEAPON");
	m_ComboBox_Position.SetCurSel(1);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CWeaponTool::OnLbnSelchangePlayerList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strTemp;
	int iIndex = m_ListBox_Player.GetCurSel();
	m_ListBox_Player.GetText(iIndex, strTemp);
	m_wstrPlayerStateKey = strTemp.operator LPCWSTR();
	m_pWeaponView->Set_Player_StateKey(m_wstrPlayerStateKey);
	m_pWeaponView->Set_PlayerSpriteNum(0);

	m_pWeaponView->Set_PlayerFrame(0);
	m_pWeaponView->Set_PlayerFrameMax(CTextureMgr::Get_Instance()->GetImgCount(L"PLAYER", m_wstrPlayerStateKey) - 1);
}


void CWeaponTool::OnLbnSelchangeWeaponList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString strTemp;
	int iIndex = m_ListBox_Weapon.GetCurSel();
	m_ListBox_Weapon.GetText(iIndex, strTemp);
	m_wstrWeaponStateKey = strTemp.operator LPCWSTR();
	m_pWeaponView->Set_Weapon_StateKey(m_wstrWeaponStateKey);

	//m_pWeaponView->Set_PlayerWeaNum(0);

	m_pWeaponView->Set_WeaponFrame(0);
	m_pWeaponView->Set_WeaponFrameMax(CTextureMgr::Get_Instance()->GetImgCount(L"WEAPON2", m_wstrWeaponStateKey) - 1);

}

void CWeaponTool::OnBnClickedButtonReplay()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_pWeaponView->Set_PlayerFrame(0);
	m_pWeaponView->Set_WeaponFrame(0);
}

void CWeaponTool::OnCbnSelchangeComboPosition()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_iComboPosIndex = m_ComboBox_Position.GetCurSel();
	cout << m_iComboPosIndex << endl;

	m_pWeaponView->Set_IndexPos(m_iComboPosIndex);

}


void CWeaponTool::OnNMReleasedcaptureSliderPlayerScale(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	m_pWeaponView->Set_PlayerScale(m_SliderCtrl_PlayerScale.GetPos());
}


void CWeaponTool::OnNMReleasedcaptureSliderWeaponScale(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	*pResult = 0;

	m_pWeaponView->Set_WeaponScale(m_SliderCtrl_WeaponScale.GetPos());
}
