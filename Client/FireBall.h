#pragma once
#include "Obj.h"
class CFireBall :
	public CObj
{
public:
	CFireBall();
	virtual ~CFireBall();

	// CObj��(��) ���� ��ӵ�
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
};

