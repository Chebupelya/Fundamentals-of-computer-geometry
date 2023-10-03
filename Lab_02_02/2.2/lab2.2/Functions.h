#pragma once
#include <math.h>
#include "CMatrix.h"
#include <afxwin.h>

struct CSizeD
{
	double cx;
	double cy;
};
struct CRectD
{
	double left;
	double top;
	double right;
	double bottom;
	CRectD()
	{
		left = top = right = bottom = 0;
	}
	CRectD(double l, double t, double r, double b);
	void setRectD(double l, double t, double r, double b);
	CSizeD SizeD();
};
struct CMyPen
{

	int PenStyle;
	COLORREF PenColor;
	int PenWidth;
public:
	CMyPen()
	{
		PenStyle = PS_SOLID;
		PenWidth = 1;
		PenColor = RGB(0, 0, 0);
	}
	void Set(int PS, int PW, COLORREF PC)
	{
		PenStyle = PS;
		PenWidth = PW;
		PenColor = PC;
	}
};

CMatrix SpaceToWindow(CRectD& rs, CRect& rw);
// Возвращает матрицу пересчета координат из мировых в оконные
// rs - область в мировых координатах - double
// rw - область в оконных координатах - int
class CPlot2D
{
	CMatrix X;				// Аргумент
	CMatrix Y;				// Функция
	CMatrix K;				// Матрица пересчета коорднат
	CRect RW;				// Прямоугольник в окне
	CRectD RS;				// Прямоугольник области в МСК
	CMyPen PenLine;                         // Перо для линий
	CMyPen PenAxis;                        // Перо для осей
public:
	CPlot2D(){ K.RedimMatrix(3, 3); };          //Конструктор по умолчанию
	void SetParams(CMatrix& XX, CMatrix& YY, CRect& RWX); // Установка
	// параметров графика
	void SetWindowRect(CRect& RWX);	//Установка области в окне для отображения 
	//графика
	void GetWindowCoords(double xs, double ys, int &xw, int &yw); //Пересчет
	//координаты точки из МСК в оконную СК
	void SetPenLine(CMyPen& PLine);	// Перо для рисования графика
	void SetPenAxis(CMyPen& PAxis);	// Перо для осей координат
	void Draw(CDC& dc, int Ind1, int Int2);	// Рисование с самостоятельным пересчетом
	//координат
};

class CChildView
{
public:
	double MyF1(double x)
	{
		return (sin(x) / x);
	}
	double MyF2(double x)
	{
		return (sqrt(x)*sin(x));
	}
};