/********************************************************************
文件名:		CommApi.h 
创建者:		poorboy
创建时间:	2018/3/15 10:38 
网   址 :   http://www.caipiao365.vip/
修改时间:
电子邮件:   canye2008@gmail.com
文件说明:	一些公共的函数  
*********************************************************************/

#ifndef CommApi_h__
#define CommApi_h__ 

#include <xstring>
#include <tchar.h>
#include <xstring> 
#include <map>
#include <vector>
#include <io.h>
#include <sstream>
#include <cstring>
#include <cstdlib>

#include <Windows.h>

using namespace std;

bool        IsFileExists(const wchar_t * lpFileName);
bool        IsFileExists(const char * lpFileName);

bool        IsDir(wstring &strDirPath);
wstring     GetAppDir();

bool        DirExist(const TCHAR  *pszDirName);

string      W2UTF8(wstring& strUnicode); 
wstring     C2W(string &strData);

string      W2UTF8(wchar_t * wsUnicode);
wstring     C2W(const char* pBuffer);

string      W2A(wstring &wsting);
string      W2A(const wchar_t * wsting);

wstring     C2Unicode(const char *szData);
wstring     C2Unicode(string strData);

wstring     GetCurrTimeString();

wstring     N2S(ULONGLONG u);
wstring     N2S(DWORD dw);

wstring     FormatTime(ULONGLONG uTime);
void        TrimString(wstring &wsting);

wstring     sysErrorMsg();

#endif // CommApi_h__

