#include "stdafx.h"
#include "MainWnd.h"


CMainWnd::CMainWnd()
{
	m_pclMinBtn = NULL;
	m_pclMaxBtn = NULL;
	m_pclRestoreBtn = NULL;
	m_pclCloseBtn = NULL;
}


CMainWnd::~CMainWnd()
{
	m_pclMinBtn = NULL;
	m_pclMaxBtn = NULL;
	m_pclRestoreBtn = NULL;
	m_pclCloseBtn = NULL;
}

LPCTSTR CMainWnd::GetWindowClassName() const
{
	return _T("MainWnd");
}

UINT CMainWnd::GetClassStyle() const
{
	return (UI_CLASSSTYLE_FRAME | CS_DBLCLKS);
}

void CMainWnd::Init()
{
	m_pclMinBtn = static_cast<CButtonUI*>(GetPaintManager()->FindControl(_T("minbtn")));
	m_pclMaxBtn = static_cast<CButtonUI*>(GetPaintManager()->FindControl(_T("maxbtn")));
	m_pclRestoreBtn = static_cast<CButtonUI*>(GetPaintManager()->FindControl(_T("restorebtn")));
	m_pclCloseBtn = static_cast<CButtonUI*>(GetPaintManager()->FindControl(_T("closebtn")));
}

LRESULT CMainWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG iStyle = ::GetWindowLong(*this, GWL_STYLE);
	iStyle &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, iStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	GetPaintManager()->Init(m_hWnd);

	CDialogBuilder clBuilder;

	CControlUI* pRoot = clBuilder.Create(_T("mainwnd.xml"), (UINT)0, NULL, GetPaintManager());
	ASSERT(pRoot && "Failed to parse XML");

	GetPaintManager()->AttachDialog(pRoot);
	GetPaintManager()->AddNotifier(this);

	Init();

	return 0;
}

LRESULT CMainWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	return 0;
}

LRESULT CMainWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostQuitMessage(0);

	bHandled = FALSE;

	return 0;
}

LRESULT CMainWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if (wParam == SC_CLOSE)
	{
		::PostQuitMessage(0);

		bHandled = TRUE;

		return 0;
	}

	BOOL bZoomed = ::IsZoomed(m_hWnd);

	LRESULT lResult = CWindowWnd::HandleMessage(uMsg, wParam, lParam);

	if (::IsZoomed(m_hWnd) != bZoomed)
	{
		bool bVisible = !!bZoomed;

		if (m_pclMaxBtn != NULL)
		{
			m_pclMaxBtn->SetVisible(bVisible);
		}

		if (m_pclRestoreBtn != NULL)
		{
			m_pclRestoreBtn->SetVisible(!bVisible);
		}
	}

	return lResult;
}

LRESULT CMainWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (::IsIconic(m_hWnd))
	{
		bHandled = FALSE;
	}

	return ((wParam == 0) ? TRUE : FALSE);
}

LRESULT CMainWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CMainWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}


LRESULT CMainWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT stPoint;

	stPoint.x = GET_X_LPARAM(lParam);
	stPoint.y = GET_Y_LPARAM(lParam);

	::ScreenToClient(m_hWnd, &stPoint);

	RECT stClientRect;

	::GetClientRect(m_hWnd, &stClientRect);

	RECT stCastRect = GetPaintManager()->GetCaptionRect();

	if ((stPoint.x >= stClientRect.left + stCastRect.left)
		&& (stPoint.x < stClientRect.right - stCastRect.right)
		&& (stPoint.y >= stCastRect.top)
		&& (stPoint.y < stCastRect.bottom))
	{
		CControlUI* pclControl = static_cast<CControlUI*>(GetPaintManager()->FindControl(stPoint));

		if ((pclControl != NULL)
			&& (_tcscmp(pclControl->GetClass(), _T("ButtonUI")) != 0)
			&& (_tcscmp(pclControl->GetClass(), _T("OstPointionUI")) != 0)
			&& (_tcscmp(pclControl->GetClass(), _T("TextUI")) != 0)
			&& (_tcscmp(pclControl->GetClass(), _T("EditUI")) != 0))
		{
			return HTCAPTION;
		}
	}

	return HTCLIENT;
}

LRESULT CMainWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE stRoundCorner = GetPaintManager()->GetRoundCorner();

	if (!::IsIconic(m_hWnd)
		&& ((stRoundCorner.cx != 0) || (stRoundCorner.cy != 0)))
	{
		CRect clRect;

		::GetWindowRect(m_hWnd, &clRect);

		clRect.Offset(-clRect.left, -clRect.top);

		clRect.right++; 
		clRect.bottom++;

		HRGN hRgn = ::CreateRoundRectRgn(clRect.left, clRect.top, clRect.right, clRect.bottom, stRoundCorner.cx, stRoundCorner.cy);

		::SetWindowRgn(m_hWnd, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;

	return 0;
}

LRESULT CMainWnd::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO stMonitor;

	stMonitor.cbSize = sizeof(stMonitor);
	::GetMonitorInfo(::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTOPRIMARY), &stMonitor);

	CRect clRect = stMonitor.rcWork;
	clRect.Offset(-clRect.left, -clRect.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;

	lpMMI->ptMaxPosition.x = clRect.left;
	lpMMI->ptMaxPosition.y = clRect.top;
	lpMMI->ptMaxSize.x = clRect.right;
	lpMMI->ptMaxSize.y = clRect.bottom;

	bHandled = FALSE;

	return 0;
}

void CMainWnd::OnFinalMessage(HWND hWnd)
{
	delete this;
}

void CMainWnd::OnPrepare()
{
}

void CMainWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("windowinit"))
	{
		OnPrepare();
	}
	else if (msg.sType == _T("click"))
	{
		if (msg.pSender == m_pclMinBtn)
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);

			return;
		}
		else if (msg.pSender == m_pclMaxBtn)
		{
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);

			return;
		}
		else if (msg.pSender == m_pclRestoreBtn)
		{
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);

			return;
		}
		else if (msg.pSender == m_pclCloseBtn)
		{
			PostQuitMessage(0);

			return;
		}
	}
}

LRESULT CMainWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	case WM_SYSCOMMAND:
		lResult = OnSysCommand(uMsg, wParam, lParam, bHandled);
		break;
	case WM_NCACTIVATE :
		lResult = OnNcActivate(uMsg, wParam, lParam, bHandled);
		break;
	case WM_NCCALCSIZE:
		lResult = OnNcCalcSize(uMsg, wParam, lParam, bHandled);
		break;
	case WM_NCPAINT:
		lResult = OnNcPaint(uMsg, wParam, lParam, bHandled);
		break;
	case WM_NCHITTEST:
		lResult = OnNcHitTest(uMsg, wParam, lParam, bHandled);
		break;
	case WM_SIZE:
		lResult = OnSize(uMsg, wParam, lParam, bHandled);
		break;
	case WM_GETMINMAXINFO:
		lResult = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled);
		break;
	default:
		bHandled = FALSE;
		break;
	}

	if (bHandled)
	{
		return lResult;
	}

	if (GetPaintManager()->MessageHandler(uMsg, wParam, lParam, lResult))
	{
		return lResult;
	}

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}