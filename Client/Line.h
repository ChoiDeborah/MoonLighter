#pragma once
class CLine
{
public:
	CLine(LINEINFO temp) { m_tLine = temp; }
	~CLine();
	CLine();
public:
	LINEINFO & Get_LineInfo() { return m_tLine; }
	void Set_LineInfo(LINEINFO temp) { m_tLine = temp; }
public:
	void Render(void);


private:
	LINEINFO m_tLine;
};

