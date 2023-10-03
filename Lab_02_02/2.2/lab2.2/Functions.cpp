#include "Functions.h"
#include <math.h>

CRectD::CRectD(double l, double t, double r, double b)
{
	left = l;
	right = r;
	bottom = b;
	top = t;
}
void CRectD::setRectD(double l, double t, double r, double b)
{
	left = l;
	right = r;
	bottom = b;
	top = t;
}
CSizeD CRectD::SizeD()
{
	CSizeD cz;
	cz.cx = fabs(right - left);
	cz.cy = fabs(top - bottom);

	return cz;
}

CMatrix SpaceToWindow(CRectD& rs, CRect& rw)
{
	// Возвращает матрицу пересчета координат из мировых в оконные
// RS - область в мировых координатах - double
// RW - область в оконных координатах - int
	CMatrix M(3, 3);
	CSize sz = rw.Size();// Размер области в ОКНЕ
	int dwx = sz.cx;//оконные(ширина)
	int dwy = sz.cy;
	CSizeD szd = rs.SizeD();// Размер области в МИРОВЫХ координатах

	double dsx = szd.cx;//мир. ширина
	double dsy = szd.cy;
	double kx = (double)dwx / dsx;//коэфф
	double ky = (double)dwy / dsy;

	M(0, 0) = kx;
	M(0, 1) = 0;
	M(0, 2) = (double)rw.left - kx*rs.left;
	M(1, 0) = 0;
	M(1, 1) = -ky;
	M(1, 2) = (double)rw.bottom + ky*rs.bottom;
	M(2, 0) = 0;
	M(2, 1) = 0;
	M(2, 2) = 1;

	return M;
}


void CPlot2D::SetParams(CMatrix& XX, CMatrix& YY, CRect& RWX)
{
	int nRowsX = XX.rows();
	int nRowsY = YY.rows();
	if (nRowsX != nRowsY)
	{
		TCHAR* error = _T("SetParams: неправильная размерность матриц");
		MessageBox(NULL, error, _T("error"),NULL);
		exit(1);
	}
	X.RedimMatrix(nRowsX);
	Y.RedimMatrix(nRowsY);
	X = XX;
	Y = YY;
	double x_max = X.MaxElement();
	double x_min = X.MinElement();
	double y_max = Y.MaxElement();
	double y_min = Y.MinElement();
	RS.setRectD(x_min, y_max, x_max, y_min);
	RW.SetRect(RWX.left, RWX.top, RWX.right, RWX.bottom);
	K = SpaceToWindow(RS, RW);
}

void CPlot2D::SetWindowRect(CRect & RWX)
{
	RW.SetRect(RWX.left, RWX.top, RWX.right, RWX.bottom);
	K = SpaceToWindow(RS, RW);
}

// для линии графика
void CPlot2D::SetPenLine(CMyPen& PLine)
{
	PenLine.PenStyle = PLine.PenStyle;
	PenLine.PenWidth = PLine.PenWidth;
	PenLine.PenColor = PLine.PenColor;
}
// для линий осей 
void CPlot2D::SetPenAxis(CMyPen& PLine)
{
	PenAxis.PenStyle = PLine.PenStyle;
	PenAxis.PenWidth = PLine.PenWidth;
	PenAxis.PenColor = PLine.PenColor;
}



void CPlot2D::GetWindowCoords(double xs, double ys, int &xw, int &yw)
{
	CMatrix V(3), W(3);
	V(0) = xs;
	V(1) = ys;
	V(2) = 1;
	W = K*V;
	xw = (int)W(0);
	yw = (int)W(1);
}

void CPlot2D::Draw(CDC& dc, int Ind1, int Int2)
{
// Ind1=1/0 - рисовать/не рисовать рамку
// Ind2=1/0 - рисовать/не рисовать оси координат
	double xs, ys;//мир
	int xw, yw;//ок.
	if (Ind1 == 1) dc.Rectangle(RW);
	if (Int2 == 1)
	{
		CPen MyPen(PenAxis.PenStyle, PenAxis.PenWidth, PenAxis.PenColor);
		CPen* pOldPen = dc.SelectObject(&MyPen);
		if (RS.left*RS.right <= 0)//ось У
		{
			xs = 0; 
			ys = RS.top;
			GetWindowCoords(xs, ys, xw, yw);
			dc.MoveTo(xw, yw);//движения пера
			xs = 0;
			ys = RS.bottom;
			GetWindowCoords(xs, ys, xw, yw);
			dc.LineTo(xw, yw);
		}
		if (RS.top*RS.bottom <= 0) //ось Х
		{
			xs = RS.left; 
			ys = 0;
			GetWindowCoords(xs, ys, xw, yw);
			dc.MoveTo(xw, yw);
			xs = RS.right;
			ys = 0;
			GetWindowCoords(xs, ys, xw, yw);
			dc.LineTo(xw, yw);
		}
		dc.SelectObject(pOldPen);
		xs = X(0);
		ys = Y(0);
		GetWindowCoords(xs, ys, xw, yw);
		dc.MoveTo(xw, yw);//перо в начало
		CPen MyPen1(PenLine.PenStyle, PenLine.PenWidth, PenLine.PenColor);
		CPen* pOldPen1 = dc.SelectObject(&MyPen1);
		for (int i = 1; i < X.rows(); i++)//пересчёт
		{
			xs = X(i);
			ys = Y(i);
			GetWindowCoords(xs, ys, xw, yw);
			dc.LineTo(xw, yw);
		}
		dc.SelectObject(pOldPen1);
	}
}

