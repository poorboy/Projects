/********************************************************************
�ļ���:		TimeService.h 
������:		poorboy
����ʱ��:	2018/3/15 14:18 
��   ַ :   http://www.caipiao365.vip/
�޸�ʱ��:
�����ʼ�:   canye2008@gmail.com
�ļ�˵��:	ʱ��ͬ����̨��  
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
#error "����GlobalDefine.h�ļ����INIT_WIN_SOCKET�Ķ���" ;
#endif   

    HANDLE h_StopEvents; 
    bool   bIsStopThread;
};

CTimeService* GetGService();

#endif // TimeService_h__;