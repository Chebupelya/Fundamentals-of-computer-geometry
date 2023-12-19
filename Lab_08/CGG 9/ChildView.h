// ChildView.h : интерфейс класса CChildView
//

#pragma once

#include "CMATRIX.H"
#include "LibChart2D.h"

struct point
{
	double x;
	double y;
};

// окно CChildView

class CChildView : public CWnd
{
// Создание
public:
	CChildView();

// Атрибуты
public:
	int Index;
	int ch;
	int N_Bezier;
	CMatrix X, Y;
	CRect RW;
	CRectD RS;
	CPlot2D Graph;
	CMyPen PenLine, PenAxis;
	double xLagrangeBase[5];
	double yLagrangeBase[5];
	double xLagrangeNew[16];
	double yLagrangeNew[16];
	double xStart[3] = { 0.0, 0.4, 0.9 };
	double yStart[3] = { 0.0, 0.1, 0.4 };
	double xBezOut[20];
	double yBezOut[20];
	int BeginSize = 3;
	double M = 10.0;
	const double PI = 3.1415926535897932384626433832795;

// Операции
public:
	void Lagrange();
	void Bezier();
	void Bezier2();
	void Bezier3();
	void Bezier4();



	double Lagr(CMatrix& X, CMatrix& Y, double x, int size);

// Переопределение
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Реализация
public:
	virtual ~CChildView();

	// Созданные функции схемы сообщений
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPlotLagrange();
	afx_msg void OnPlotBezier();
	afx_msg void OnPlotBezier2();
	afx_msg void OnPlotBezier3();
	afx_msg void OnPlotBezier4();

};