#include "StdAfx.h"
#include "TimeService.h" 
#include "../TimeSockService/TimeSockService.h"


CTimeService g_Service;

CTimeService* GetGService()
{
    return &g_Service;
}


CTimeService::CTimeService(void)
: NTService(SERVICE_NAME,SERVICE_DISP_NAME)
{
    m_sDescription = SERVICE_DISP;
    h_StopEvents =  CreateEvent( NULL, FALSE, FALSE, NULL ); 
    bIsStopThread = false;
}

CTimeService::~CTimeService(void)
{
    CloseHandle( h_StopEvents );
}

DWORD WINAPI CTimeService::ThreadProc(LPVOID lpParameter)
{
    CTimeService * pService = (CTimeService *)lpParameter;
    if(pService== NULL) return 0;
    return pService->DoTask();
}

DWORD CTimeService::DoTask()
{ 
    int nSyncType = Get_GSet()->GetSyncType();
    int nID = Get_GSet()->GetCuuSIDType();
    wstring wstrIp = L"";
    int nStime = Get_GSet()->GetSTime();


    if(Get_GSet()->GetItemSIP(nID, wstrIp) == false)
    {
        LOG_ERR(L"��ȡ����ʧ��,���������ļ�!");
        Stop();
        return false;
    }

    LOG_INFO(L"ͬ������: %s", nSyncType==1? L"����ͬ��" : L"��ʱͬ��");
    LOG_INFO(L"������:[%s]",  wstrIp.c_str());

    if (nSyncType == 2)
    {
        LOG_INFO(L"���ʱ��: %d ����", nStime);
    }


    /*
    ������:		poorboy
    ���ʱ��:	2018/3/15 11:13 
    ����˵��:   �ڼ���ĵ����ϲ��Է������еĵ�����ע��ɷ������绹û�����ӹ���ʱ�������˷���
    ִ���³ɵĴ����ͻ�һֱִ��ʧ��,�������ӳɹ�Ҳ������.ԭ�򻹲���,��֪���������鷳������^_^
    ���ø��򵥴ֱ��ķ�ʽ�����
    */


    LOG_INFO(L"�����������, ��ǰ����������:%s", (Fun_InternetGetConnectedState()? L"����������" : L"û����������"));
    //�ȴ��������ӳɹ�
    while ( Fun_InternetGetConnectedState() == FALSE)
    {
        if(bIsStopThread) break;
        Sleep(3000);
    }

    LOG_INFO(L"�����������, ����������");

    if (!bIsStopThread)
    { 
        string strIP = W2A(wstrIp); 

        GetTSockService()->SetNtpIP(strIP);
        GetTSockService()->StartService();
        GetTSockService()->DoUpTime();

        if (nSyncType == 1)
        {
            while ( 1 )
            {  
                if(bIsStopThread) break;

                Sleep(500);
                if (GetTSockService()->IsLastSyncSuccess())
                { 
                    LOG_INFO(L"���³ɹ�,����ֹͣ!", nStime);
                    Stop();
                    break;
                }
            }

        }else if (nSyncType == 2)
        {
            DWORD SleepTime = nStime * 120;
            DWORD dwTime = SleepTime; 

            while(1)
            {   
                if(bIsStopThread) break;
                Sleep(500);
                dwTime --;
                if (dwTime == 0)
                {
                    GetTSockService()->DoUpTime();
                    dwTime = SleepTime;
                }
            }
        }
    }
    return 0;
} 


bool CTimeService::StartService()
{
    bool bRet = false;

    HANDLE Threadhandle = CreateThread(NULL,
        0,
        CTimeService::ThreadProc, 
        (LPVOID)this,
        0, 
        NULL);

    if (Threadhandle) bRet = true; 
    return bRet;
}


void CTimeService::RunService()
{ 
    wstring MutexStr = L"Global\\";
    MutexStr.append(SERVICE_NAME);

    AutoMutex _aMutex(MutexStr);
    if(_aMutex.IsMutexExists()) return;  

    StartService();  
    WaitForSingleObject(h_StopEvents, INFINITE); 
    GetTSockService()->Stop();   
    return; 
}

bool CTimeService::OnServiceStop()
{ 
    SetEvent( h_StopEvents ); 
    bIsStopThread = true;
    return true;
}
