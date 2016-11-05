#pragma once

class CShortcut
{
public:
	CShortcut();
	~CShortcut();
public:
	static bool GetModule(LPSTR pszModulePath, int iModuleLen, LPSTR pszExeName, int iExeLen);
	static bool GetLinkPath(LPSTR pszBuffer, int iBufferLen);
	static bool CreateLink(LPCSTR lpcszPath, LPCSTR lpcszDescription, int iShowCmd = SW_SHOWNORMAL);
};

