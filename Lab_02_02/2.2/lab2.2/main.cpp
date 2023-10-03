#include <afxwin.h>
#include <windows.h>
#include "CMatrix.h"
#include "Functions.h"
#define ID_F1 2002
#define ID_F2 2003
#define ID_F12 2004
#define pi 3.14159
int f = 0;
class CMyMainWnd : public CFrameWnd
{
public:
	CMyMainWnd()
	{
		Create(NULL, L"Artsiom lab2.2");
	}
	CPlot2D p1, p2;
	CChildView funct;
	CMenu *menu;
	afx_msg int OnCreate(LPCREATESTRUCT);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT flags, CPoint Loc);
	afx_msg void OnLButtonUp(UINT flags, CPoint Loc);
	void F1();
	void F2();
	void F12();
	DECLARE_MESSAGE_MAP();
};
class CMyApp : public CWinApp
{
public:
	CMyApp(){};
	virtual BOOL InitInstance()
	{
		m_pMainWnd = new CMyMainWnd();
		m_pMainWnd->ShowWindow(SW_SHOW);
		return TRUE;
	}
};
BEGIN_MESSAGE_MAP(CMyMainWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_COMMAND(ID_F1, F1)
	ON_COMMAND(ID_F2, F2)
	ON_COMMAND(ID_F12, F12)
END_MESSAGE_MAP()

afx_msg void CMyMainWnd::OnPaint()
{
	CPaintDC dc(this);
	if (f == 1)
	{
		//dc.TextOut(400, 20, "y=sin(x) / x,    [-3pi;3pi]  dx=pi/36");
		p1.Draw(dc,1,1);
	}
	if (f == 2)
	{
		//dc.TextOut(400, 20, "y=sqrt(x)*sin(x),    [0;6pi]  dx=pi/36");
		p2.Draw(dc, 1, 1);
	}
	if (f == 3)
	{
		//dc.TextOut(150, 180, "y=sin(x) / x,    [-3pi;3pi]  dx=pi/36");
		p1.Draw(dc, 1, 1);
		//dc.TextOut(770, 180, "y=sqrt(x)*sin(x),    [0;6pi]  dx=pi/36");
		p2.Draw(dc, 1, 1);
	}
	dc.SetMapMode(MM_TEXT);
}

afx_msg int CMyMainWnd::OnCreate(LPCREATESTRUCT)
{
	menu = new CMenu();
	menu->CreateMenu();
	CMenu *subMenu = new CMenu();
	subMenu->CreatePopupMenu();
	subMenu->AppendMenu(MF_STRING, ID_F1, _T("F1"));
	subMenu->AppendMenu(MF_STRING, ID_F2, _T("F2"));
	subMenu->AppendMenu(MF_STRING, ID_F12, _T("F12"));
	this->menu->AppendMenu(MF_POPUP | MF_STRING, (UINT)subMenu->m_hMenu, _T("Tests_F"));
	SetMenu(menu);
	return 0;
}

void CMyMainWnd::F1()
{
	double xL = -3 * pi;
	double xH = -xL;
	double dx = pi / 36;
	int N = (xH - xL) / dx;
	CMatrix XX(N+1), YY(N+1);
	for (int i = 0; i <= N; i++)
	{
		XX(i) = xL + i * dx;
		YY(i) = funct.MyF1(XX(i));
	}
	CRect r(400, 100, 1000, 600);
	p1.SetParams(XX, YY, r);

	CMyPen p;p.Set(PS_SOLID, 1, RGB(255, 0, 0));
	p1.SetPenLine(p);
	p.Set(PS_SOLID, 2, RGB(0, 0, 255));
	p1.SetPenAxis(p);
	f = 1;
	InvalidateRect(0);
}


void CMyMainWnd::F2()
{
	double xL = 0;
	double xH = 6 * pi;
	double dx = pi / 36;
	int N = (xH - xL) / dx;
	CMatrix XX(N + 1), YY(N + 1);
	for (int i = 0; i <= N; i++)
	{
		XX(i) = xL + i * dx;
		YY(i) = funct.MyF2(XX(i));
	}
	CRect r(400, 100, 1000, 600);
	p2.SetParams(XX, YY, r);

	CMyPen p; p.Set(PS_DASHDOTDOT, 3, RGB(255, 0, 0));
	p2.SetPenLine(p);
	p.Set(PS_SOLID, 2, RGB(0, 0, 0));
	p2.SetPenAxis(p);
	f = 2;
	InvalidateRect(0);
}



void CMyMainWnd::F12()
{
	
	p1.SetWindowRect(CRect(150, 200, 620, 480));//(30, 40, 500, 320)
	p2.SetWindowRect(CRect(770, 200, 1240, 480)); //CRect(600, 40, 1070, 320)
	f = 3;
	InvalidateRect(0);
}
CMyApp theApp;