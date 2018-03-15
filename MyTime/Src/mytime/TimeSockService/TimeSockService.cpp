/********************************************************************
�ļ���:		TimeSockService.cpp 
������:		poorboy
����ʱ��:	2018/3/15 10:41 
��   ַ :   http://www.caipiao365.vip/
�޸�ʱ��:
�����ʼ�:   canye2008@gmail.com
�ļ�˵��:	ʱ�������ʵ��  
*********************************************************************/

#include "StdAfx.h"
#include "./TimeSockService.h"
#include <time.h>
//#include <WS2tcpip.h> 
//#include <WinSock2.h>


//1900�ꡫ1970��֮���ʱ������ 
#define JAN_1970         0x83aa7e80

//Ĭ�ϵ�ʱ��ͬ��������
#define NTP_SERVER_IP    "ntp1.aliyun.com"

//ʱ��ͬ��������ͨ�Ŷ˿�
#define NTP_PORT         123

//Ĭ�ϵĳ�ʱʱ��
#define SOCKET_TIME_OUT  5000

//ͨ�Žṹ��Ķ���
struct NTPPacket
{
    union
    {
        struct _ControlWord
        {
            unsigned int uLI : 2;          // 00 = no leap, clock ok
            unsigned int uVersion : 3;     // version 3 or version 4
            unsigned int uMode : 3;        // 3 for client, 4 for server, etc.
            unsigned int uStratum : 8;     // 0 is unspecified, 1 for primary reference system,
            // 2 for next level, etc.
            int nPoll : 8;                 // seconds as the nearest power of 2
            int nPrecision : 8;            // seconds to the nearest power of 2
        };

        int nControlWord;                  // 4
    };

    int nRootDelay;                       // 4
    int nRootDispersion;                  // 4
    int nReferenceIdentifier;             // 4

    __int64 n64ReferenceTimestamp;        // 8
    __int64 n64OriginateTimestamp;        // 8
    __int64 n64ReceiveTimestamp;          // 8

    int nTransmitTimestampSeconds;       // 4
    int nTransmitTimestampFractions;     // 4
};

/*
��Ҫ�ֶεĽ������£�

l              LI��Leap Indicator��������Ϊ2���أ�ֵΪ��11��ʱ��ʾ�澯״̬��ʱ��δ��ͬ����Ϊ����ֵʱNTP����������

l              VN��Version Number��������Ϊ3���أ���ʾNTP�İ汾�ţ�Ŀǰ�����°汾Ϊ3��

l              Mode������Ϊ3���أ���ʾNTP�Ĺ���ģʽ����ͬ��ֵ����ʾ�ĺ���ֱ��ǣ�0δ���塢1��ʾ�����Ե���ģʽ��2��ʾ�����Ե���ģʽ��3��ʾ�ͻ�ģʽ��4��ʾ������ģʽ��5��ʾ�㲥ģʽ���鲥ģʽ��6��ʾ�˱���ΪNTP���Ʊ��ġ�7Ԥ�����ڲ�ʹ�á�

l              Stratum��ϵͳʱ�ӵĲ�����ȡֵ��ΧΪ1��16����������ʱ�ӵ�׼ȷ�ȡ�����Ϊ1��ʱ��׼ȷ����ߣ�׼ȷ�ȴ�1��16���εݼ�������Ϊ16��ʱ�Ӵ���δͬ��״̬��������Ϊ�ο�ʱ�ӡ�

l              Poll����ѯʱ�䣬����������NTP����֮���ʱ������

l              Precision��ϵͳʱ�ӵľ��ȡ�

l              Root Delay�����ص����ο�ʱ��Դ������ʱ�䡣

l              Root Dispersion��ϵͳʱ����������ο�ʱ�ӵ������

l              Reference Identifier���ο�ʱ��Դ�ı�ʶ��

l              Reference Timestamp��ϵͳʱ�����һ�α��趨����µ�ʱ�䡣

l              Originate Timestamp��NTP�������뿪���Ͷ�ʱ���Ͷ˵ı���ʱ�䡣

l              Receive Timestamp��NTP�����ĵ�����ն�ʱ���ն˵ı���ʱ�䡣

l              Transmit Timestamp��Ӧ�����뿪Ӧ����ʱӦ���ߵı���ʱ�䡣

l              Authenticator����֤��Ϣ��

*/ 


SYSTEMTIME st;
FILETIME ft; 

TimeSockService g_TimeSockService;

TimeSockService * GetTSockService()
{
    return & g_TimeSockService;
}

TimeSockService::TimeSockService(void)
{ 
    mStrIP      =  NTP_SERVER_IP;
    _hWndGUI    = NULL;
    h_Events[0] = CreateEvent( NULL, FALSE, FALSE, NULL );
    h_Events[1] = CreateEvent( NULL, FALSE, FALSE, NULL );
}  


TimeSockService::~TimeSockService(void)
{

    CloseHandle( h_Events[0] ); 
    CloseHandle( h_Events[1] );  
}



DWORD WINAPI TimeSockService::ThreadProc(LPVOID lpParameter)
{
    TimeSockService * pTCPService = (TimeSockService *)lpParameter;
    if(pTCPService== NULL) return 0;
    return pTCPService->DoTask();
}

bool TimeSockService::StartService()
{
    bool bRet = false;

    Threadhandle = CreateThread(NULL,
        0,
        TimeSockService::ThreadProc, 
        (LPVOID)this,
        0, 
        NULL);

    if (Threadhandle) bRet = true; 
    return bRet;
}

void TimeSockService::Stop()
{
    SetEvent( h_Events[1]); 
} 

void TimeSockService::DoUpTime()
{
    SetEvent( h_Events[0]); 
} 

void TimeSockService::UpTime()
{ 
    UINT64 uRetTime = 0;
    m_bIsSyncSuccess = GetNetTime(mStrIP, uRetTime);
    if(m_bIsSyncSuccess)
    {
        if(_hWndGUI) SendMessage(_hWndGUI, WM_GET_NET_TIME, 1, (LPARAM) uRetTime); 
        else
        {
            SYSTEMTIME sysTime;
            ZeroMemory(&sysTime, sizeof(SYSTEMTIME)); 
            CTimeEx m_nTime(uRetTime + 1);
            if(m_nTime.ToSystemTime(sysTime)) ::SetLocalTime(&sysTime);
        }
    }
}

bool TimeSockService::IsLastSyncSuccess(void)
{
    return m_bIsSyncSuccess;
}



DWORD TimeSockService::DoTask()
{
    DWORD dwWaitRet = 5;
    while ( 1 )	 
    { 
        dwWaitRet = WaitForMultipleObjects(2, h_Events, FALSE, 500);
        if (dwWaitRet == WAIT_OBJECT_0 + 1)	         break;
        else if (dwWaitRet == WAIT_OBJECT_0 + 0)	 UpTime();
    }
    return 0;
} 

//��ʱ��������ϻ�ȡʱ��
//���Ǽ򵥵�UDPͨ�žͲ�дע����
bool TimeSockService::GetNetTime(const string strIP,UINT64 &uRetTime)
{
    SOCKET SendSocket;  
    sockaddr_in RecvAddr;
    UString strError; 

    NTPPacket ntpSend = { 0 };
    NTPPacket ntpRecv = { 0 };

    int tv_out = SOCKET_TIME_OUT;  
    HOSTENT* pHostent  = NULL;

    ntpSend.nControlWord = 0x1B; 

    uRetTime = 0;

    SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    RecvAddr.sin_family = AF_INET;  
    RecvAddr.sin_port = htons(NTP_PORT);

    do 
    {
        pHostent = gethostbyname(strIP.c_str());  

        if (pHostent == NULL)  
        {  
            strError.Format(L"error in gethostbyname: %d \r\n%s", WSAGetLastError(), sysErrorMsg().c_str());
            break;
        }

        RecvAddr.sin_addr.s_addr = *(u_long *)pHostent->h_addr_list[0];

        setsockopt(SendSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv_out, sizeof(tv_out)); 
        sendto(SendSocket, (char*)&ntpSend, sizeof(ntpSend), 0, (SOCKADDR *)&RecvAddr, sizeof(RecvAddr));  

        sockaddr fromAddr = { 0 };  
        int nRead = sizeof(fromAddr);  
        if (SOCKET_ERROR != recvfrom(SendSocket, (char*)&ntpRecv, sizeof(ntpRecv), 0, &fromAddr, &nRead))  
        { 
            uRetTime = ntohl(ntpRecv.nTransmitTimestampSeconds) - JAN_1970;  
        }else  strError.Format(L"error in recvfrom: %d \r\n%s", WSAGetLastError(), sysErrorMsg().c_str());  

    } while (0);


    if (uRetTime == 0)
    {
        LOG_ERR(strError.GetData());
        if (_hWndGUI) SendMessage(_hWndGUI, WM_GET_NET_TIME_ERR, 0, LPARAM(strError.GetData()));
    }

    if (SendSocket != SOCKET_ERROR) closesocket(SendSocket);

    return uRetTime > 0;   
}

void TimeSockService::SetGUIHwnd(HWND hWndGUI)
{
    _hWndGUI = hWndGUI; 
}

void TimeSockService::SetNtpIP(string &strIP)
{
    mStrIP = strIP;
}