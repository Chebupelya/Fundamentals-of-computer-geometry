#pragma once
double cosv1v2(CMatrix& one, CMatrix& two);
class CPyramid
{
private:
	CMatrix Vertices; // Координаты вершин
	void GetRect(CMatrix& Vert, CRectD& RectView);

public:
	CPyramid();
	void Draw(CDC& dc, CMatrix& P, CRect& RW);
	void Draw1(CDC& dc, CMatrix& P, CRect& RW);
	void ColorDraw(CDC& dc, CMatrix& PView, CRect& RW, COLORREF color);
};

CPyramid::CPyramid()
{
	Vertices.RedimMatrix(4, 6);	// ABC    — нижнее основание
								// A'B'C' — верхнее основание
	/*       A                   B                    C                   A'                  B'                  C'      */
	Vertices(0, 0) = 0;	Vertices(0, 1) = 0;  Vertices(0, 2) = 6; Vertices(0, 3) = 0; Vertices(0, 4) = 0; Vertices(0, 5) = 3;
	Vertices(1, 0) = 6; Vertices(1, 1) = 0;  Vertices(1, 2) = 0; Vertices(1, 3) = 3; Vertices(1, 4) = 0; Vertices(1, 5) = 0;
	Vertices(2, 0) = 0; Vertices(2, 1) = 0;  Vertices(2, 2) = 0; Vertices(2, 3) = 6; Vertices(2, 4) = 6; Vertices(2, 5) = 6;
	Vertices(3, 0) = 1; Vertices(3, 1) = 1;  Vertices(3, 2) = 1; Vertices(3, 3) = 1; Vertices(3, 4) = 1; Vertices(3, 5) = 1;
}

void CPyramid::GetRect(CMatrix& Vert, CRectD& RectView) //коорд прям-ка, охват проекцию пирамиды на XY ВСК
{
	CMatrix V = Vert.GetRow(0);               // x - координаты
	double xMin = V.MinElement();
	double xMax = V.MaxElement();
	V = Vert.GetRow(1);                       // y - координаты
	double yMin = V.MinElement();
	double yMax = V.MaxElement();
	RectView.SetRectD(xMin, yMax, xMax, yMin);
}

void CPyramid::Draw(CDC& dc, CMatrix& PView, CRect& RW)
// Рисует пирамиду БЕЗ удаления невидимых ребер
// Самостоятельный пересчет координат из мировых в оконные (MM_TEXT)
// dc - ссылка на класс CDC MFC
// P - координаты точки наблюдения в мировой сферической системе
// координат
// (r,fi(град.), q(град.))
// RW - область в окне для отображения	
{
	CMatrix MV = CreateViewCoord(PView(0), PView(1), PView(2)); // Матрица (4x4)
	CMatrix ViewVert = MV * Vertices; // Координаты вершин пирамиды в видовой СК
	CRectD RectView; // проекция - охватывающий прямоугольник
	GetRect(ViewVert, RectView); // Получаем охватывающий прямоугольник по к-там вершин
	CMatrix MW = SpaceToWindow(RectView, RW); // Матрица (3x3) для пересчета координат из мировых в оконные

	CPoint MasVert[6]; // Масив оконных координат вершин,
	CMatrix V(3);
	V(2) = 1;
	// Цикл по количеству вершин – вычисляем оконные коодинаты
	for (int i = 0; i < 6; i++)
	{
		V(0) = ViewVert(0, i); // x в видовой
		V(1) = ViewVert(1, i); // y в видовой
		V = MW * V;            // Оконные координаты точки
		MasVert[i].x = (int)V(0); // оконная к-ты х
		MasVert[i].y = (int)V(1); // оконная к-та у
	}

	// Рисуем
	CPen Pen(PS_SOLID, 2, RGB(0, 0, 255));
	CPen* pOldPen = dc.SelectObject(&Pen);
	dc.MoveTo(MasVert[3]);
	// Ребра НИЖНЕЙ грани
	for (int i = 0; i < 3; i++)
	{
		dc.LineTo(MasVert[i]);
	}
	dc.LineTo(MasVert[0]);
	dc.MoveTo(MasVert[5]);
	// Ребра ВЕРХНЕЙ грани
	for (int i = 3; i < 6; i++)
	{
		dc.LineTo(MasVert[i]);
	}
	// Ребра БОКОВЫХ граней
	for (int i = 0; i < 3; i++)
	{
		dc.MoveTo(MasVert[i]);
		dc.LineTo(MasVert[i + 3]);
	}
	dc.SelectObject(pOldPen);
}
void CPyramid::Draw1(CDC& dc, CMatrix& PView, CRect& RW)
// Рисует пирамиду С УДАЛЕНИЕМ невидимых ребер
{
	CMatrix ViewCart = SphereToCart(PView); // Декартовы координаты точки наблюдения
	CMatrix MV = CreateViewCoord(PView(0), PView(1), PView(2)); // Матрица (4x4) пересчета точки из мировой системы координат в видовую
	CMatrix ViewVert = MV * Vertices; // Координаты вершин пирамиды в видовой СК
	CRectD RectView;
	GetRect(ViewVert, RectView); // Получаем охватывающий прямоугольник
	CMatrix MW = SpaceToWindow(RectView, RW); // Матрица (3x3) для пересчета

	CPoint MasVert[6]; // Масив оконных координат вершин,
	CMatrix V(3);
	V(2) = 1;
	// Цикл по количеству вершин – вычисляем оконные коодинаты
	for (int i = 0; i < 6; i++)
	{
		V(0) = ViewVert(0, i); // x
		V(1) = ViewVert(1, i); // y
		V = MW * V;            // Оконные координаты точки
		MasVert[i].x = (int)V(0);
		MasVert[i].y = (int)V(1);
	}

	// Рисуем
	CPen Pen(PS_SOLID, 2, RGB(0, 0, 255));
	CPen* pOldPen = dc.SelectObject(&Pen);
	CBrush Brush(RGB(255, 0, 0));
	CBrush* pOldBrush = dc.SelectObject(&Brush);
	CMatrix R1(3), R2(3), VN(3);
	double sm;
	for (int i = 0; i < 3; i++)
	{
		CMatrix VE = Vertices.GetCol(i + 3, 0, 2); // Вершина E
		int k;
		if (i == 2)
			k = 0;
		else
			k = i + 1;
		R1 = Vertices.GetCol(i, 0, 2); // Текущая точка основания
		R2 = Vertices.GetCol(k, 0, 2); // Следующая точка основания
		CMatrix V1 = R2 - R1;          // Вектор – ребро в основании
		CMatrix V2 = VE - R1;          // Вектор – ребро к вершине
		VN = VectorMult(V2, V1);       // Вектор ВНЕШНЕЙ нормали
		sm = ScalarMult(VN, ViewCart); // Скалярное произведение 

		if (sm >= 0) // Грань видима – рисуем боковую грань
		{
			dc.MoveTo(MasVert[i]);
			dc.LineTo(MasVert[k]);
			dc.LineTo(MasVert[k + 3]);
			dc.LineTo(MasVert[i + 3]);
			dc.LineTo(MasVert[i]);
		}
	}

	VN = VectorMult(R1, R2);
	sm = ScalarMult(VN, ViewCart);
	if (sm >= 0) // Основание
	{
		dc.Polygon(MasVert, 3);
	}
	else
	{
		CBrush* topBrush = new CBrush(RGB(0, 0, 255));
		dc.SelectObject(topBrush);
		dc.Polygon(MasVert + 3, 3);	// верхнее основание
	}

	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
}

void CPyramid::ColorDraw(CDC& dc, CMatrix& PView, CRect& RW, COLORREF color)
{
	// Извлекаем компоненты цвета (красный, зеленый, синий)
	BYTE Red = GetRValue(color);
	BYTE Green = GetGValue(color);
	BYTE Blue = GetBValue(color);

	// Преобразование координат точки наблюдения в декартовы координаты
	CMatrix ViewCart = SphereToCart(PView);

	// Создаем матрицу вида для преобразования координат вершин пирамиды
	CMatrix MV = CreateViewCoord(PView(0), PView(1), PView(2));

	// Преобразование координат вершин пирамиды в видовую систему координат
	CMatrix ViewVert = MV * Vertices;

	// Определение охватывающего прямоугольника для проекции пирамиды на XY вида
	CRectD RectView;
	GetRect(ViewVert, RectView);

	// Создаем матрицу преобразования координат из мировых в оконные
	CMatrix MW = SpaceToWindow(RectView, RW);

	// Массив оконных координат вершин пирамиды
	CPoint MasVert[6];
	CMatrix V(3);
	V(2) = 1;

	// Цикл по количеству вершин – вычисляем оконные координаты
	for (int i = 0; i < 6; i++)
	{
		V(0) = ViewVert(0, i); // x в видовой системе координат
		V(1) = ViewVert(1, i); // y в видовой системе координат
		V = MW * V;            // Преобразование в оконные координаты
		MasVert[i].x = (int)V(0); // оконная координата x
		MasVert[i].y = (int)V(1); // оконная координата y
	}

	// Матрицы и переменные для расчета освещения
	CMatrix R1(3), R2(3), VN(3);
	double sm;

	// Цикл по трем граням пирамиды
	for (int i = 0; i < 3; i++)
	{
		// Получение вершины E
		CMatrix VE = Vertices.GetCol(i + 3, 0, 2);

		// Определение текущей и следующей точек основания
		int k = (i == 2) ? 0 : i + 1;
		R1 = Vertices.GetCol(i, 0, 2); // Текущая точка основания
		R2 = Vertices.GetCol(k, 0, 2); // Следующая точка основания

		// Векторы – ребра в основании и к вершине
		CMatrix V1 = R2 - R1;
		CMatrix V2 = VE - R1;

		// Вектор внешней нормали
		VN = VectorMult(V2, V1);

		// Скалярное произведение с вектором направления обзора
		sm = cosv1v2(VN, ViewCart);

		// Если грань видима – рисуем боковую грань
		if (sm >= 0)
		{
			// Рисование боковой грани с учетом цвета и освещения
			CPen Pen(PS_SOLID, 2, RGB(sm * sm * Red, sm * sm * Green, sm * sm * Blue));
			CPen* pOldPen = dc.SelectObject(&Pen);
			CBrush Brus(RGB(sm * sm * Red, sm * sm * Green, sm * sm * Blue));
			CBrush* pOldBrush = dc.SelectObject(&Brus);
			CPoint MasVertr[4]{ MasVert[i], MasVert[k], MasVert[k + 3],MasVert[i + 3] };
			dc.Polygon(MasVertr, 4);    // Зарисовка боковой грани
			dc.SelectObject(pOldPen);
			dc.SelectObject(pOldBrush);
		}
	}

	// Расчет освещения для основания пирамиды
	VN = VectorMult(R1, R2);
	sm = cosv1v2(VN, ViewCart);

	// Если основание видимо – рисуем его
	if (sm >= 0)
	{
		CBrush* topBrush = new CBrush(RGB(sm * Red, sm * Green, sm * Blue));
		dc.SelectObject(topBrush);
		dc.Polygon(MasVert, 3);    // Рисование основания
	}
	else
	{
		// Расчет освещения для верхнего основания
		CBrush* topBrush = new CBrush(RGB(sm * sm * Red, sm * sm * Green, sm * sm * Blue));
		dc.SelectObject(topBrush);
		dc.Polygon(MasVert + 3, 3);    // Рисование верхнего основания
	}
}


double cosv1v2(CMatrix& V1, CMatrix& V2)
{
	double modV1 = ModVec(V1); // Вычисление модуля вектора V1
	double modV2 = ModVec(V2); // Вычисление модуля вектора V2

	// Проверка на нулевую длину векторов
	int b = (modV1 < 1e-7) || (modV2 < 1e-7);
	if (b)
	{
		// Вывод сообщения об ошибке
		TCHAR* error = L"CosV1V2: модуль одного или обоих векторов < 1e-7!";
		MessageBox(NULL, error, L"Ошибка", MB_ICONSTOP);
		exit(1);
	}

	// Проверка размерностей векторов
	int b1 = (V1.cols() == 1) && (V1.rows() == 3);
	int b2 = (V2.cols() == 1) && (V2.rows() == 3);
	b = b1 && b2;
	if (!b)
	{
		// Вывод сообщения об ошибке
		TCHAR* error = L"CosV1V2: неправильные размерности векторов! ";
		MessageBox(NULL, error, L"Ошибка", MB_ICONSTOP);
		exit(1);
	}

	// Вычисление косинуса угла между векторами
	double cos_f = ScalarMult(V1, V2) / (modV1 * modV2);
	return cos_f;
}
