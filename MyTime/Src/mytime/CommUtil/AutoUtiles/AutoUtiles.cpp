/********************************************************************
文件名:		AutoUtiles.cpp 
创建者:		poorboy
创建时间:	2018/3/10 21:08 
网   址 :   http://www.caipiao365.vip/
修改时间:
文件说明:	  
*********************************************************************/

#include "stdafx.h"
#include "AutoUtiles.h" 

#ifdef INIT_WIN_SOCKET 
#pragma comment(lib,"ws2_32.lib") 
#endif


#ifdef INIT_WIN_SOCKET

SCKInit::SCKInit()
{
    //初始化socket库
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD( 2, 2);
    err = WSAStartup( wVersionRequested, &wsaData );

    if ( err != 0 )
    {
        LOG_ERR(L"WSAStartup Error:%d", err);
        return;
    }

    //if ( LOBYTE( wsaData.wVersion ) != 1 ||
    //    HIBYTE( wsaData.wVersion ) != 1 )
    //{
    //    WSACleanup( );
    //    return;
    //} 
}

SCKInit::~SCKInit()
{ 
    WSACleanup();
}

#endif

#ifdef INIT_WIN_SERVICE

AutoServiceHandle::AutoServiceHandle(bool bNeedOpenService, LPCTSTR ServiceName)
{
    bIsOpen = bNeedOpenService;
    hSCM     = NULL;
    hService = NULL; 
    hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if(hSCM && bNeedOpenService)
    {
        hService = OpenService(hSCM, ServiceName, SERVICE_ALL_ACCESS); 
    }
}

AutoServiceHandle::~AutoServiceHandle()
{

    if(hService)    CloseServiceHandle(hService); 
    if(hSCM)        CloseServiceHandle(hSCM);  
}

bool AutoServiceHandle::IsError()
{
    if(bIsOpen)
        return ( hSCM == NULL) ||  (hService == NULL);
    else
        return hSCM == NULL;
}


#endif




AutoMutex::AutoMutex()
{
    strMutexName = L"";
    bIsExists = false;
}


bool AutoMutex::CreateMutex(std::wstring &_strMutexName)
{
    strMutexName = _strMutexName;
    bIsExists = false;
    m_hMutex = ::CreateMutex(NULL, TRUE, strMutexName.c_str());
    if ((NULL !=  m_hMutex) && (GetLastError() == ERROR_ALREADY_EXISTS))
    { 
        bIsExists = true;  
        return false;
    }

    return true;
}

AutoMutex::AutoMutex(std::wstring &_strMutexName)
{
    strMutexName = _strMutexName;
    bIsExists = false;
    m_hMutex = ::CreateMutex(NULL, TRUE, strMutexName.c_str());
    if ((NULL !=  m_hMutex) && (GetLastError() == ERROR_ALREADY_EXISTS))
    { 
        bIsExists = true;  
    }
}



AutoMutex::~AutoMutex()
{

    ::ReleaseMutex(m_hMutex);
    ::CloseHandle(m_hMutex);
    m_hMutex = NULL;
}

bool AutoMutex::IsMutexExists()
{
    return bIsExists;
} 


void EmptyMySoftMemoSet()
{
    //HANDLE hProcess=OpenProcess(PROCESS_ALL_ACCESS,TRUE,ProcessInfo.th32ProcessID);
    //ULONGLONG uMem = 1024;//GetCuuSoftMem(); 
    //if (uMem < (1024*1024*20))  return; 

    //HANDLE hProcess = GetCurrentProcess();
    //if(hProcess)
    //{
    //    SetProcessWorkingSetSize(hProcess,-1,-1);
    //    //内存整理
    //    EmptyWorkingSet(hProcess); 
    //    CloseHandle(hProcess);
    //} 
    return;
}  
