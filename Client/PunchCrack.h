#pragma once
#include "Obj.h"
class CPunchCrack :
	public CObj
{
public:
	CPunchCrack();
	virtual ~CPunchCrack();

	// CObj��(��) ���� ��ӵ�
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
};

