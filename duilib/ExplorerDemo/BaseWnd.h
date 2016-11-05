#pragma once

class CBaseWnd : public CWindowWnd, public INotifyUI
{
public:
	CBaseWnd();
	virtual ~CBaseWnd();
public:
	virtual LPCTSTR GetWindowClassName() const;
	virtual UINT GetClassStyle() const;
	virtual void Init();
public:
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual void OnPrepare();
	virtual void OnFinalMessage(HWND hWnd);

	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	CPaintManagerUI * GetPaintManager();
private:
	CPaintManagerUI m_clPaintManager;
};

