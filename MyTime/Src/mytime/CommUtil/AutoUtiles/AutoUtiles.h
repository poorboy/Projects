/********************************************************************
�ļ���:		AutoUtiles.h 
������:		poorboy
����ʱ��:	2018/3/10 21:07 
��   ַ :   http://www.caipiao365.vip/
�޸�ʱ��:
�ļ�˵��:	  
*********************************************************************/

#pragma once
#include "../GlobalDefine.h"

#ifdef INIT_WIN_SOCKET

class SCKInit
{
public:
    SCKInit();
    ~SCKInit(); 
};

#endif


#ifdef INIT_WIN_SERVICE

class  AutoServiceHandle
{
public:
    AutoServiceHandle(bool bNeedOpenService, LPCTSTR ServiceName);
    ~AutoServiceHandle();
    bool IsError();

    bool             bIsOpen;
    SC_HANDLE        hSCM;
    SC_HANDLE        hService;
};

#endif



class AutoMutex
{
public:
    AutoMutex();
    AutoMutex(std::wstring &_strMutexName);

    ~AutoMutex();
    bool CreateMutex(std::wstring &_strMutexName);
    bool IsMutexExists();

private:
    std::wstring strMutexName;
    HANDLE  m_hMutex;
    bool    bIsExists;
};

void EmptyMySoftMemoSet();