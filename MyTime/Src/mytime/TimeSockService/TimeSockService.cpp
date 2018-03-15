/********************************************************************
文件名:		TimeSockService.cpp 
创建者:		poorboy
创建时间:	2018/3/15 10:41 
网   址 :   http://www.caipiao365.vip/
修改时间:
电子邮件:   canye2008@gmail.com
文件说明:	时间更新类实现  
*********************************************************************/

#include "StdAfx.h"
#include "./TimeSockService.h"
#include <time.h>
//#include <WS2tcpip.h> 
//#include <WinSock2.h>


//1900年～1970年之间的时间秒数 
#define JAN_1970         0x83aa7e80

//默认的时间同步服务器
#define NTP_SERVER_IP    "ntp1.aliyun.com"

//时间同步服务器通信端口
#define NTP_PORT         123

//默认的超时时间
#define SOCKET_TIME_OUT  5000

//通信结构体的定义
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
主要字段的解释如下：

l              LI（Leap Indicator）：长度为2比特，值为“11”时表示告警状态，时钟未被同步。为其他值时NTP本身不做处理。

l              VN（Version Number）：长度为3比特，表示NTP的版本号，目前的最新版本为3。

l              Mode：长度为3比特，表示NTP的工作模式。不同的值所表示的含义分别是：0未定义、1表示主动对等体模式、2表示被动对等体模式、3表示客户模式、4表示服务器模式、5表示广播模式或组播模式、6表示此报文为NTP控制报文、7预留给内部使用。

l              Stratum：系统时钟的层数，取值范围为1～16，它定义了时钟的准确度。层数为1的时钟准确度最高，准确度从1到16依次递减，层数为16的时钟处于未同步状态，不能作为参考时钟。

l              Poll：轮询时间，即两个连续NTP报文之间的时间间隔。

l              Precision：系统时钟的精度。

l              Root Delay：本地到主参考时钟源的往返时间。

l              Root Dispersion：系统时钟相对于主参考时钟的最大误差。

l              Reference Identifier：参考时钟源的标识。

l              Reference Timestamp：系统时钟最后一次被设定或更新的时间。

l              Originate Timestamp：NTP请求报文离开发送端时发送端的本地时间。

l              Receive Timestamp：NTP请求报文到达接收端时接收端的本地时间。

l              Transmit Timestamp：应答报文离开应答者时应答者的本地时间。

l              Authenticator：验证信息。

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

//从时间服务器上获取时间
//就是简单的UDP通信就不写注释了
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