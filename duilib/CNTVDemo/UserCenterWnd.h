#pragma once

class CUserCenterWnd : public CWindowWnd, public INotifyUI
{
public:
	CUserCenterWnd();
	~CUserCenterWnd();
public:
	LPCTSTR GetWindowClassName() const;
	UINT GetClassStyle() const;
	void Init();
public:
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void OnPrepare();
	void OnFinalMessage(HWND hWnd);

	void Notify(TNotifyUI& msg);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	CPaintManagerUI m_clPaintManager;

	CButtonUI *m_pclCloseBtn;
};

