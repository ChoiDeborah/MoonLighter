#pragma once
#include "Obj.h"
class CWave :
	public CObj
{
public:
	CWave();
	virtual ~CWave();

	// CObj��(��) ���� ��ӵ�
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
	
public:
	void Frame();
	void Matrix();

public:
	void LoadRect();
	void Render_Rect();
	void Update_Rect();

private:
	

private:
	float m_fLifeTime;
};

