/********************************************************************
�ļ���:		TimeSockService.h 
������:		poorboy
����ʱ��:	2018/3/15 10:40 
��   ַ :   http://www.caipiao365.vip/
�޸�ʱ��:
�����ʼ�:   canye2008@gmail.com
�ļ�˵��:	ʱ����·�����  
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


    //����ʱ��ͬ���߳� 
    bool StartService();

    //ֹͣʱ������߳�
    void Stop();

    //֪ͨʱ������߳�ͬ��ʱ��
    void DoUpTime();

    //ͬ��ʱ�䷽��
    void UpTime();

    //���һ��ͬ��ʱ���Ƿ�ɹ�
    //��Ҫ�����ڿ���ͬ���˳���
    bool IsLastSyncSuccess(void);

    //��ȡ�����ϵ�ʱ��
    bool GetNetTime(const string strIP, UINT64 &uRetTime);

    //ʱ��ͬ���߳�������
    DWORD DoTask();

    //�����Խ��淽ʽ���е�������Ĵ��ھ��
    void SetGUIHwnd( HWND hWndGUI);

    //����ʱ��ͬ����������IP��������
    void SetNtpIP(string &strIP);

    static DWORD WINAPI ThreadProc(LPVOID lpParameter);

protected:
    //��ǰ��ȡʱ�乤���̵߳ľ��
    HANDLE     Threadhandle;

    //�߳̿����¼�
    HANDLE     h_Events[2]; 

    //�Խ��淽ʽ���е�������Ĵ��ھ��
    HWND       _hWndGUI;

    //��ǰʱ��������ĵ�ַ
    string     mStrIP;

    //���һ�θ����Ƿ�ɹ�
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