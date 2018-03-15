/********************************************************************
//Copyright (C), 2017-2018, http://www.caipiao365.vip/
文件名:		Log.cpp 
创建者:		poorboy
创建时间:	2018/2/27 10:59 
修改时间:
文件说明:	日志文件 
*********************************************************************/

#include "stdafx.h"
#include "../../stdafx.h"


CLog  G_Log;
#define  AUTO_LOCK_LOG   CLog::Lock lock (*this); 

wstring GetLogTimeString()
{
    wstring strTmp;
    WCHAR wsTime[100] = { 0 };
    SYSTEMTIME time;
    GetLocalTime(&time);
    wsprintf(wsTime, L"%d-%02d-%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
    strTmp = wsTime;
    return strTmp;
}


void MyLogING(int lever, const char * szfile, int nLine, LPCTSTR wszPrintString, ...)
{
    if (wszPrintString == NULL) return; 
    //if (lever < GResManager::Instance()->GetLogLevel())
    //	return;


    va_list arg_list;
    int len;
    WCHAR * wszFinalString;
    va_start(arg_list, wszPrintString);
    len = _vscwprintf(wszPrintString, arg_list) + 1;
    wszFinalString = (WCHAR *)malloc(len * sizeof(WCHAR));
    vswprintf_s(wszFinalString, len, wszPrintString, arg_list);
    va_end(arg_list);
    wstring strMesg = wszFinalString;
    free(wszFinalString);
    wszFinalString = NULL;

    wstring strFile = C2W(szfile);
    strFile = strFile.substr(strFile.find_last_of(L'\\'));
    G_Log.Loging(lever, strMesg, nLine, strFile);
}

bool CLog::bIsInstall = false;

wstring szLevel[4] = { L"Debug", L"Warning", L"Info", L"Error" };

CLog::CLog(void)
{
    // m_logFile;
    m_nwho = 0;
    m_nlevel = 0;
    m_strmesg = L"";
    m_nline = 0;
    m_strfileName = L"";
    if (Get_GSet()->GetDebugLevel() == LOG_NONE) return;

    GetLocalTime(&m_LastWriteTime);
    hLogFile = INVALID_HANDLE_VALUE;
    InitializeCriticalSection(&_lock);
    InitData();
}

CLog::~CLog(void)
{
    CLog::FinshLog(0);
    G_Log.bIsInit = false;
    bIsInit = false;

    if (INVALID_HANDLE_VALUE != hLogFile)
    {
        ::CloseHandle(hLogFile);
    }
    DeleteCriticalSection(&_lock);
}

void CLog::InitData(void)
{
    TCHAR szFile[MAX_PATH] = { 0 };
    m_nwho = 0;
    m_nlevel = 0;
    m_strmesg = L"";;
    m_nline = 0;
    m_strfileName = L"";;
    GetLocalTime(&m_LastWriteTime);
    m_nCuuDay = m_LastWriteTime.wDay;
    m_strlogfileName = L"";
    m_strlogDIR = GetAppDir();
    m_strlogDIR += wstring(L"data\\Log\\");
    wsprintf(szFile, L"%s%d-%02d-%02d.log", m_strlogDIR.c_str(), m_LastWriteTime.wYear, m_LastWriteTime.wMonth, m_LastWriteTime.wDay);
    m_strlogfileName = szFile;
    wsprintf(szFile, L"%s%d-%02d-%02d_1.log", m_strlogDIR.c_str(), m_LastWriteTime.wYear, m_LastWriteTime.wMonth, m_LastWriteTime.wDay);
    m_strlogfileName1 = szFile;

    wstring strDir =  GetAppDir()  += wstring(L"data\\dump\\");
    if (!DirExist(m_strlogDIR.c_str())) SHCreateDirectory(NULL, m_strlogDIR.c_str());
    if (!DirExist(strDir.c_str())) SHCreateDirectory(NULL, m_strlogDIR.c_str());



    wstring strDumpDir = GetAppDir();
    strDumpDir += wstring(L"data\\dump\\");
    if (!DirExist(strDumpDir.c_str())) SHCreateDirectory(NULL, strDumpDir.c_str());

    bIsInit = CLog::MyCreateFile(hLogFile, m_strlogfileName);
    if (!bIsInit) 
    {
        bIsInit = CLog::MyCreateFile(hLogFile, m_strlogfileName1);
        if (bIsInit)
        {
            m_strlogfileName = m_strlogfileName1;
        }
    }
}

bool CLog::MyCreateFile(HANDLE &hFile, wstring &wsFileName)
{
    if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

    if (!IsFileExists(wsFileName.c_str()))
    {
        hFile = ::CreateFile(wsFileName.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            //LYDBG(L"打开日志文件失败 ,错误代码是：%d", ::GetLastError());
            return false;
        }
        DWORD cbWritten = 0;
        WriteFile(hFile, "\xef\xbb\xbf", 3, &cbWritten, NULL);
    }
    else
    {
        hFile = ::CreateFile(wsFileName.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            //LYDBG(L"打开日志文件失败 ,错误代码是：%d", ::GetLastError());
            return false;
        }
    }

    DWORD dwPtr = SetFilePointer(hFile, 0, 0, FILE_END);
    if (dwPtr == INVALID_SET_FILE_POINTER)
    {
        //LYDBG(L"设置日志文件指针失败 ,错误代码是：%d", ::GetLastError());
        return false;
    }

    return true;
}
void CLog::ClearData(void)
{
    if (INVALID_HANDLE_VALUE != hLogFile)
        ::CloseHandle(hLogFile);
}


void CLog::FinshLog(int nTime)
{
    if (hLogFile != INVALID_HANDLE_VALUE)
    {
        DWORD cbLenth, cbWritten;
        cbLenth = m_strLog.length();

        if (::WriteFile(hLogFile, m_strLog.c_str(), cbLenth, &cbWritten, NULL))
        {
            m_strLog.clear();
            GetLocalTime(&m_LastWriteTime);
        }
        else
        {
            LOG_DBG(L"写日志 ,错误代码是：%d", ::GetLastError());
        }
    }
}

void CLog::Loging()
{
    TCHAR strMesg[MAX_PATH * 10] = { 0 };
    TCHAR strError[MAX_PATH * 10] = { 0 };

    wsprintf(strMesg,
        L"[%s]:[%s][%sL%d]:%s\r\n",
        GetLogTimeString().c_str(),
        szLevel[m_nlevel % 4].c_str(),
        m_strfileName.c_str(),
        m_nline,
        m_strmesg.c_str()
        );

    string strtmgMsg = W2UTF8(strMesg);

    printf("%s\r\n", strtmgMsg.c_str());

    m_strLog.append(strtmgMsg);

    if (!G_Log.bIsInit) return;

    SYSTEMTIME tCuuTime;
    GetLocalTime(&tCuuTime);
    if (m_nCuuDay != tCuuTime.wDay)
    {
        bool bNewFile = false;
        m_nCuuDay = tCuuTime.wDay;

        if (INVALID_HANDLE_VALUE != hLogFile)
        {
            ::CloseHandle(hLogFile);
        }

        TCHAR szFile[MAX_PATH] = { 0 };
        wsprintf(szFile, L"%s%d-%02d-%02d.log", m_strlogDIR.c_str(), tCuuTime.wYear, tCuuTime.wMonth, tCuuTime.wDay);
        m_strlogfileName = szFile;

        if (!CLog::MyCreateFile(hLogFile, m_strlogfileName))
        {
            return;
        }
    }

    DWORD cbLenth, cbWritten;
    cbLenth = m_strLog.length();
    //if (cbLenth < GResManager::Instance()->GetLogCacheSize()) return;

    DWORD dwSize;
    DWORD mpsize = GetFileSize(hLogFile, &dwSize);
    DWORD dwLogSize = 30;// GResManager::Instance()->GetMaxLogFileSize();
    dwLogSize *= 1024;
    dwLogSize *= 1024;

    if ((mpsize > dwLogSize)
        ||
        (dwSize > 0)
        )
    {
        CloseHandle(hLogFile);
        if (m_strlogfileName.length() > 2)
            DeleteFile(m_strlogfileName.c_str());
    }

    if (!::WriteFile(hLogFile, m_strLog.c_str(), cbLenth, &cbWritten, NULL))
    {
        LOG_DBG(L"写日志 ,错误代码是：%d", ::GetLastError());
    }

    m_strLog.clear();
    GetLocalTime(&m_LastWriteTime);
}

void CLog::Loging(int level, wstring &mesg, int line, wstring &fileName)
{
    if (level < Get_GSet()->GetDebugLevel()) return;

    AUTO_LOCK_LOG;

    m_nwho = 0;
    m_nlevel = level;
    m_strmesg = mesg;
    m_nline = line;
    m_strfileName = fileName;
    Loging();
}

CLog * GetSysLog()
{
    return &G_Log;
}
