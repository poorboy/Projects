/********************************************************************
文件名:		CommApi.cpp
创建者:		poorboy
创建时间:	2018/3/15 10:38 
网   址 :   http://www.caipiao365.vip/
修改时间:
电子邮件:   canye2008@gmail.com
文件说明:	一些公共的函数实现文件
*********************************************************************/


#include "./CommApi.h" 
#include "../debugnew/debugnew.h"
#include "../Log/Log.h" 
#include <Shlwapi.h>
#include <AclAPI.h>

std::wstring  g_wsLastError = L"";

bool IsFileExists(const wchar_t * lpFileName) 
{
    if (_waccess(lpFileName, 0) == -1)
    {
        return false;
    }

    if (::PathIsDirectoryW(lpFileName))
    {
        return false;
    }

    return true;
}

bool IsFileExists(const char * lpFileName)
{
    if (_access(lpFileName, 0) == -1)
    {
        return false;
    }

    if (::PathIsDirectoryA(lpFileName))
    {
        return false;
    }

    return true;
}

//检查是不是文件夹
bool IsDir(wstring &strDirPath)
{
    if (::PathIsDirectory(strDirPath.c_str()))
    {
        return true;
    }
    return false;
}

wstring GetAppDir()
{
    TCHAR sFilename[_MAX_PATH];
    TCHAR sDrive[_MAX_DRIVE];
    TCHAR sDir[_MAX_DIR];
    TCHAR sFname[_MAX_FNAME];
    TCHAR sExt[_MAX_EXT];

    ::GetModuleFileName(NULL, sFilename, _MAX_PATH); 

    _tsplitpath_s(sFilename, sDrive, _MAX_DRIVE, sDir, _MAX_DIR,
        sFname, _MAX_FNAME, sExt, _MAX_EXT); 

    wstring homeDir = sDrive;
    homeDir += wstring(sDir);
    int nLen = homeDir.length();
    if (homeDir.at(nLen - 1) != _T('\\'))
        homeDir += _T("\\");

    return homeDir;
}

//检查一个文件夹是否存在 
bool DirExist(const TCHAR  *pszDirName)   
{   
    try
    {
        WIN32_FIND_DATA   fileinfo;
        TCHAR   _szDir[_MAX_PATH];
        _tcscpy(_szDir,pszDirName);
        int nLen  =  _tcsclen(_szDir);
        if((_szDir[nLen-1] == _T('\\')) || (_szDir[nLen-1] == _T('/')))
        {   
            _szDir[nLen-1] = _T('\0');
        }   
        HANDLE hFind  = ::FindFirstFile(_szDir,&fileinfo);

        if(hFind == INVALID_HANDLE_VALUE)   
        {   
            return false;   
        }

        if(fileinfo.dwFileAttributes && FILE_ATTRIBUTE_DIRECTORY)   
        {   
            ::FindClose(hFind);
            return true;
        }

        ::FindClose(hFind);
    } 
    catch (...)
    {
        return false;
    }

    return false;
}   

string W2UTF8(wstring& strUnicode)
{
    string str;
    int iSize = WideCharToMultiByte(CP_UTF8, 0, strUnicode.c_str(), -1, NULL, 0, NULL, NULL);
    char*  pszMultiByte = (char*)malloc((iSize+1));
    WideCharToMultiByte(CP_UTF8, 0, strUnicode.c_str(), -1, pszMultiByte, iSize, NULL, NULL);
    str = pszMultiByte;
    delete [] pszMultiByte;
    pszMultiByte = NULL;
    return str;
}

wstring C2W(string &strData)
{
    WCHAR* buf;
    wstring str;
    int nLength = MultiByteToWideChar(CP_UTF8, 0, strData.c_str(), -1, NULL, 0);
    if (nLength <= 0) return str;
    buf = new WCHAR[nLength + 1];
    memset(buf, 0, (nLength + 1) * sizeof(WCHAR));
    int nRet = MultiByteToWideChar(CP_UTF8, 0, strData.c_str(), nLength, buf, nLength);
    buf[nRet] = 0;
    str = buf;
    delete []buf;
    buf = NULL;
    return str;
}

wstring C2W(const char* pBuffer)
{
    WCHAR* buf;
    wstring str;
    int nLength = strlen(pBuffer);
    if (nLength <= 0) return str;
    buf = new WCHAR[nLength + 1];
    memset(buf, 0, (nLength + 1) * sizeof(WCHAR));
    int nRet = MultiByteToWideChar(CP_UTF8, 0, pBuffer, nLength, buf, nLength);
    buf[nRet] = 0;
    str = buf;
    delete []buf;
    buf = NULL;
    return str;
}

string W2UTF8(wchar_t * wsUnicode)
{
    char* buf;
    string str; 
    int nLength = wcslen(wsUnicode) +1;
    buf = new char[nLength * 4 + 1]; 
    memset(buf, 0, nLength * 4 + 1);
    int nSize = nLength * 4;
    int nRet = WideCharToMultiByte(CP_UTF8, 0, wsUnicode, nLength, buf, nSize, NULL, NULL);
    buf[nRet] = 0;
    str = buf;
    delete []buf;
    buf = NULL;
    return str;
}

string W2A(wstring &wsting)
{
    char* buf;
    string str; 
    int nLength = wsting.length() +1;
    buf = new char[nLength * 4 + 1]; 
    memset(buf, 0, nLength * 4 + 1);
    int nSize = nLength * 4;
    int nRet = WideCharToMultiByte(CP_ACP, 0, wsting.c_str(), nLength, buf, nSize, NULL, NULL);
    buf[nRet] = 0;
    str = buf;
    delete []buf;
    buf = NULL;
    return str; 
}

string W2A(const wchar_t * wsting)
{
    char* buf;
    string str; 
    int nLength = wcslen(wsting) +1;
    buf = new char[nLength * 4 + 1]; 
    memset(buf, 0, nLength * 4 + 1);
    int nSize = nLength * 4;
    int nRet = WideCharToMultiByte(CP_ACP, 0, wsting, nLength, buf, nSize, NULL, NULL);
    buf[nRet] = 0;
    str = buf;
    delete []buf;
    buf = NULL;
    return str; 
}

wstring C2Unicode(const char *szData)
{
    WCHAR* buf;
    wstring str = L""; 
    int nLength = strlen(szData) + 1;
    int codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;
    buf = new WCHAR[nLength ];
    memset(buf, 0, nLength  * sizeof(WCHAR));
    int nRet = MultiByteToWideChar(codepage, 0, szData, nLength, buf, nLength); 
    str = buf;
    delete []buf;
    buf = NULL;
    return str;
}

wstring C2Unicode(string strData)
{
    WCHAR* buf;
    wstring str = L""; 
    int nLength = strData.length() + 1;
    int codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;
    buf = new WCHAR[nLength ];
    memset(buf, 0, nLength  * sizeof(WCHAR));
    int nRet = MultiByteToWideChar(codepage, 0, strData.c_str(), nLength, buf, nLength); 
    str = buf;
    delete []buf;
    buf = NULL;
    return str;
}

//// 由文件全路径名获取其文件名
//wstring ParseFileName(wstring& strFileFullName)
//{
//    strFileFullName.TrimRight(_T('\\'));
//    int nLen = strFileFullName.GetLength();
//    int nPos = strFileFullName.ReverseFind(_T('\\'));
//    if (nPos == -1)
//    {
//        return strFileFullName;
//    }
//
//    wstring strFileName = strFileFullName.Mid(nPos + 1, nLen - nPos - 1);
//
//    return strFileName;
//}

wstring GetCurrTimeString()
{ 
    wstring strTmp;
    WCHAR szTime[50];
    SYSTEMTIME st;
    ::GetLocalTime(&st);
    wsprintf(szTime, 
        _T("%d-%02d-%02d %02d:%02d:%02d"),
        st.wYear,
        st.wMonth,
        st.wDay, 
        st.wHour, 
        st.wMinute,
        st.wSecond
        );
    strTmp = szTime;
    return strTmp;
}

wstring N2S(ULONGLONG u)
{
    wstring strTmp;
    WCHAR szTmp[10]; 
    wsprintf(szTmp, _T("%I64u"), u);
    strTmp = szTmp;
    return strTmp;
}


wstring N2S(DWORD dw)
{
    wstring strTmp;
    WCHAR szTmp[10]; 
    wsprintf(szTmp, _T("%d"), dw);
    strTmp = szTmp;
    return strTmp;
}

void TrimString(wstring &wsting)
{
    int nLen = wsting.length();
    if (nLen < 1) return;

    while(
        (nLen > 0)
        &&
        (_tcscmp(wsting.substr(0,1).c_str(), L" ") == 0))
    {
        wsting = wsting.substr(1); 
        nLen = wsting.length();
    } 

    while(
        (nLen > 0)
        &&
        (_tcscmp(wsting.substr((nLen-1),1).c_str(), L" ") == 0))
    {
        wsting = wsting.substr(0,(nLen - 1)); 
        nLen = wsting.length();
    }
}


wstring sysErrorMsg() 
{
    TCHAR *buf;
    DWORD ecode = ::GetLastError();
    if (0 == ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, ecode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<TCHAR *>(&buf), 0, NULL)) 
    {
        wostringstream oss;
        oss << L"Unknown error 0x" << hex << ecode;
        return oss.str();
    }

    g_wsLastError = wstring(buf);
    LocalFree(buf);
    return g_wsLastError;
} 