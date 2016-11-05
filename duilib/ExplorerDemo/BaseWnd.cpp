#include "stdafx.h"
#include "BaseWnd.h"


CBaseWnd::CBaseWnd()
{
}


CBaseWnd::~CBaseWnd()
{
}

LPCTSTR CBaseWnd::GetWindowClassName() const
{
	return _T("BaseWnd");
}

UINT CBaseWnd::GetClassStyle() const
{
	return (UI_CLASSSTYLE_FRAME | CS_DBLCLKS);
}

void CBaseWnd::Init()
{
}

LRESULT CBaseWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CBaseWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	return 0;
}

LRESULT CBaseWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	return 0;
}

void CBaseWnd::OnFinalMessage(HWND hWnd)
{
	delete this;
}

void CBaseWnd::OnPrepare()
{
}

void CBaseWnd::Notify(TNotifyUI& msg)
{
}

LRESULT CBaseWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = 0;
	BOOL bHandled = TRUE;

	switch (uMsg)
	{
	case WM_CREATE:
		lResult = OnCreate(uMsg, wParam, lParam, bHandled);
		break;
	case WM_CLOSE:
		lResult = OnClose(uMsg, wParam, lParam, bHandled);
		break;
	case WM_DESTROY:
		lResult = OnDestroy(uMsg, wParam, lParam, bHandled);
		break;
	default:
		bHandled = FALSE;
		break;
	}

	if (bHandled)
	{
		return lResult;
	}

	if (m_clPaintManager.MessageHandler(uMsg, wParam, lParam, lResult))
	{
		return lResult;
	}

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

CPaintManagerUI * CBaseWnd::GetPaintManager()
{
	CPaintManagerUI *pclPaintManager = &m_clPaintManager;

	return pclPaintManager;
}