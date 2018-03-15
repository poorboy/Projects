/********************************************************************
文件名:		TimeService.h 
创建者:		poorboy
创建时间:	2018/3/15 14:18 
网   址 :   http://www.caipiao365.vip/
修改时间:
电子邮件:   canye2008@gmail.com
文件说明:	时间同步后台类  
*********************************************************************/

#ifndef TimeService_h__
#define TimeService_h__
#include "../stdafx.h"


#pragma once

class CTimeService: public NTService
{
public:
    CTimeService(void);
    ~CTimeService(void);

    static DWORD WINAPI ThreadProc(LPVOID lpParameter);
    bool StartService();
    DWORD DoTask();
    virtual void RunService(); 
    virtual bool OnServiceStop();

private:

#ifdef INIT_WIN_SOCKET 
    SCKInit _SInit; 
#else
#error "请在GlobalDefine.h文件里打开INIT_WIN_SOCKET的定义" ;
#endif   

    HANDLE h_StopEvents; 
    bool   bIsStopThread;
};

CTimeService* GetGService();

#endif // TimeService_h__;