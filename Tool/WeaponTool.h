#pragma once
#include "afxwin.h"
#include "afxcmn.h"

class CWeaponView;
// CWeaponTool ��ȭ �����Դϴ�.

class CWeaponTool : public CDialog
{
	DECLARE_DYNAMIC(CWeaponTool)

public:
	CWeaponTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CWeaponTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WEAPONTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:

private:
	CListBox m_ListBox_Player;
	CListBox m_ListBox_Weapon;

private:
	wstring m_wstrWeaponStateKey;
	wstring m_wstrPlayerStateKey;
private:
	CWeaponView* m_pWeaponView;
	CCreateContext pContext;
	// ������ �߰� �ϰ� ����̽� ������



public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangePlayerList();
	afx_msg void OnLbnSelchangeWeaponList();
	
private:
	// Control
	CComboBox m_ComboBox_Position;

private:
	// Control
	CSliderCtrl m_SliderCtrl_PlayerScale;
	CSliderCtrl m_SliderCtrl_WeaponScale;


public:
	afx_msg void OnBnClickedButtonReplay();
	afx_msg void OnCbnSelchangeComboPosition();

private:
	int m_iComboPosIndex;


public:
	afx_msg void OnNMReleasedcaptureSliderPlayerScale(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderWeaponScale(NMHDR *pNMHDR, LRESULT *pResult);
};
