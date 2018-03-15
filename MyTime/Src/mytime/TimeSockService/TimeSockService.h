/********************************************************************
文件名:		TimeSockService.h 
创建者:		poorboy
创建时间:	2018/3/15 10:40 
网   址 :   http://www.caipiao365.vip/
修改时间:
电子邮件:   canye2008@gmail.com
文件说明:	时间更新服务类  
*********************************************************************/



#ifndef TimeSockService_h__
#define TimeSockService_h__

#pragma once
#define  WM_GET_NET_TIME        WM_USER + 1000
#define  WM_GET_NET_TIME_ERR    WM_GET_NET_TIME + 1
 
class TimeSockService
{
public:
    TimeSockService(void);
    ~TimeSockService(void);


    //启动时间同步线程 
    bool StartService();

    //停止时间更新线程
    void Stop();

    //通知时间更新线程同步时间
    void DoUpTime();

    //同步时间方法
    void UpTime();

    //最后一次同步时间是否成功
    //主要用于在开机同后退出用
    bool IsLastSyncSuccess(void);

    //获取网络上的时间
    bool GetNetTime(const string strIP, UINT64 &uRetTime);

    //时间同步线程主方法
    DWORD DoTask();

    //设置以界面方式运行的主界面的窗口句柄
    void SetGUIHwnd( HWND hWndGUI);

    //设置时间同步服务器的IP或者域名
    void SetNtpIP(string &strIP);

    static DWORD WINAPI ThreadProc(LPVOID lpParameter);

protected:
    //当前获取时间工作线程的句柄
    HANDLE     Threadhandle;

    //线程控制事件
    HANDLE     h_Events[2]; 

    //以界面方式运行的主界面的窗口句柄
    HWND       _hWndGUI;

    //当前时间服务器的地址
    string     mStrIP;

    //最后一次更新是否成功
    bool       m_bIsSyncSuccess;
};


TimeSockService *  GetTSockService(); 

#define SET_TIME_SOCK_S_IP(X)             GetTSockService()->SetNtpIP(X)
#define START_TIME_SOCK_S                 GetTSockService()->StartService()
#define STOP_TIME_SOCK_S                  GetTSockService()->Stop()
#define TIME_SOCK_S_UPTIME                GetTSockService()->DoUpTime()
#define TIME_SOCK_S_IS_LAST_UP_SUCCESS    GetTSockService()->IsLastSyncSuccess()
#define SET_GUI_HWND_2_TIME_SOCK_S        GetTSockService()->SetGUIHwnd(GetHWND())


#endif // TimeSockService_h__ 