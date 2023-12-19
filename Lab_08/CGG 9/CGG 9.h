
// CGG 9.h : главный файл заголовка для приложения CGG 9
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CCGG9App:
// О реализации данного класса см. CGG 9.cpp
//

class CCGG9App : public CWinApp
{
public:
	CCGG9App();


// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCGG9App theApp;
