// Copyright (c) 2016 Victor(516822434@qq.com)

// This code may be used in compiled form in any way you desire. These
// source files may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 

// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
// Beware of bugs.


// ExplorerDemo.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "ExplorerDemo.h"
#include "MainWnd.h"


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin\\SogouExplorerRes"));

	HRESULT hResult = ::CoInitialize(NULL);
	if (FAILED(hResult))
	{
		return 0;
	}

	CMainWnd *pclMainWnd = new CMainWnd();
	if (pclMainWnd == NULL)
	{
		return 0;
	}

	pclMainWnd->Create(NULL, _T("SogouExplorerDemo"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pclMainWnd->CenterWindow();
	pclMainWnd->ShowWindow(true);

	CPaintManagerUI::MessageLoop();

	::CoUninitialize();

	return 0;
}