#include "stdafx.h"
#include "Shortcut.h"
#include <stdio.h>
#include <string>
#include <shlobj.h>
#pragma comment(lib, "shell32.lib")

CShortcut::CShortcut()
{
}


CShortcut::~CShortcut()
{
}

bool CShortcut::GetLinkPath(LPSTR pszBuffer, int iBufferLen)
{
	if ((pszBuffer == NULL) || (iBufferLen <= 0))
	{
		return false;
	}

	LPITEMIDLIST ppIdList = NULL;

	HRESULT lResult = SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOPDIRECTORY, &ppIdList);

	if (lResult != S_OK)
	{
		return false;
	}

	BOOL bResult = SHGetPathFromIDList(ppIdList, pszBuffer);

	CoTaskMemFree(ppIdList);
	ppIdList = NULL;

	return (bResult ? true : false);
}

bool CShortcut::GetModule(LPSTR pszModulePath, int iModuleLen, LPSTR pszExeName, int iExeLen)
{
	if ((pszModulePath == NULL) || (strlen(pszModulePath) == 0) || (iModuleLen <= 0)
		|| (pszExeName == NULL) || (strlen(pszExeName) == 0) || (iExeLen <= 0))
	{
		return false;
	}

	CHAR szTemp[MAX_PATH] = { 0 };
	CHAR szPath[MAX_PATH] = { 0 };
	bool bResult = false;

	GetModuleFileName(NULL, szTemp, sizeof(szTemp));
	LPSTR pszTemp = _tcsrchr(szTemp, '\\');
	if (pszTemp != NULL)
	{
		_snprintf_s(pszExeName, iExeLen, _TRUNCATE, "%s", pszTemp + 1);

		*pszTemp = '\0';
		_snprintf_s(pszModulePath, iModuleLen, _TRUNCATE, "%s", pszTemp);

		bResult = true;
	}

	return bResult;
}

bool CShortcut::CreateLink(LPCSTR lpcszLinkPath, LPCSTR lpcszDescription, int iShowCmd)
{
	if ((lpcszLinkPath == NULL) || (strlen(lpcszLinkPath) == 0)
		|| (lpcszDescription == NULL) || (strlen(lpcszDescription) == 0))
	{
		return false;
	}

	IShellLink *pLink = NULL;
	
	HRESULT lResult = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pLink);
	if (FAILED(lResult))
	{
		return false;
	}

	IPersistFile *ppFile = NULL;

	lResult = pLink->QueryInterface(IID_IPersistFile, (void **)&ppFile);
	if (FAILED(lResult))
	{
		pLink->Release();

		return false;
	}

	pLink->SetDescription(lpcszDescription);
	pLink->SetShowCmd(iShowCmd);

	CHAR szTemp[MAX_PATH] = { 0 };
	CHAR szPath[MAX_PATH] = { 0 };

	GetModuleFileName(NULL, szPath, sizeof(szPath));

	// 可执行程序的完整路径
	pLink->SetPath(szPath);

	LPSTR pszTemp = _tcsrchr(szPath, '\\');
	if (pszTemp == NULL)
	{
		ppFile->Release();
		pLink->Release();

		return false;
	}
	*pszTemp = '\0';

	// 可执行程序所在目录
	pLink->SetWorkingDirectory(szPath);

	_snprintf_s(szTemp, sizeof(szTemp), _TRUNCATE, "%s", pszTemp + 1);
	
	pszTemp = _tcsrchr(szTemp, '.');
	if (pszTemp == NULL)
	{
		ppFile->Release();
		pLink->Release();

		return false;
	}
	*pszTemp = '\0';

	// 快捷方式完整路径
	_snprintf_s(szPath, sizeof(szPath), _TRUNCATE, "%s\\%s%s", lpcszLinkPath, szTemp, ".lnk");

	WCHAR szBuffer[MAX_PATH] = { 0 };

	MultiByteToWideChar(CP_ACP, 0, szPath, -1, szBuffer, MAX_PATH);

	lResult = ppFile->Save(szBuffer, TRUE);

	ppFile->Release();
	pLink->Release();

	return SUCCEEDED(lResult);
}