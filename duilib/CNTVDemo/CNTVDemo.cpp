// Copyright (c) 2016 Victor(516822434@qq.com)

// This code may be used in compiled form in any way you desire. These
// source files may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 

// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
// Beware of bugs.

// CNTVDemo.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "CNTVDemo.h"
#include "MainFrameWnd.h"


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin\\CNTVRes"));

	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr))
	{
		return 0;
	}

	CMainFrameWnd* pFrame = new CMainFrameWnd();
	if (pFrame == NULL)
	{
		return 0;
	}

	pFrame->Create(NULL, _T("CNTVDemo"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pFrame->CenterWindow();
	pFrame->ShowWindow(true);

	CPaintManagerUI::MessageLoop();

	::CoUninitialize();

	return 0;
}