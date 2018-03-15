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
        LOG_ERR(L"获取配置失败,请检查配置文件!");
        Stop();
        return false;
    }

    LOG_INFO(L"同步类型: %s", nSyncType==1? L"开机同步" : L"定时同步");
    LOG_INFO(L"服务器:[%s]",  wstrIp.c_str());

    if (nSyncType == 2)
    {
        LOG_INFO(L"间隔时间: %d 分钟", nStime);
    }


    /*
    创建者:		poorboy
    添加时间:	2018/3/15 11:13 
    功能说明:   在家里的电脑上测试发现在有的电脑上注册成服务当网络还没有链接功能时先启动了服务
    执行下成的代码后就会一直执行失败,网络链接成功也是这样.原因还不找,有知道的朋友麻烦告诉我^_^
    先用个简单粗暴的方式来解决
    */


    LOG_INFO(L"检查网络链接, 当前网络链接是:%s", (Fun_InternetGetConnectedState()? L"网络有链接" : L"没有网络链接"));
    //等待网络连接成功
    while ( Fun_InternetGetConnectedState() == FALSE)
    {
        if(bIsStopThread) break;
        Sleep(3000);
    }

    LOG_INFO(L"检查网络链接, 网络有链接");

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
                    LOG_INFO(L"更新成功,服务停止!", nStime);
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
