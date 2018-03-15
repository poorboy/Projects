#include "stdafx.h"
#include "NetOpr.h"

#include <WinInet.h> 
#include <string>  
//
//˵����
//InternetGetConnectedState()��������״�����ܼ�ʱ��Ӧ
//IsNetworkAlive()�����Լ�ʱ��Ӧ������ͨ�����������Ҫ����System Event Notification֧�֣�ϵͳĬ���Զ������÷��񣩡�ʹ�øú�������Ҫ��װ���µ�SDK 

BOOL Fun_InternetGetConnectedState()
{
    //#define INTERNET_CONNECTION_MODEM           1
    //#define INTERNET_CONNECTION_LAN             2
    //#define INTERNET_CONNECTION_PROXY           4
    //#define INTERNET_CONNECTION_MODEM_BUSY      8
    DWORD   flags;//������ʽ 
    BOOL   m_bOnline = TRUE;//�Ƿ�����
    m_bOnline = InternetGetConnectedState(&flags,0);   
    //if(m_bOnline)//����   
    //{   
    //    if ((flags & INTERNET_CONNECTION_MODEM) ==INTERNET_CONNECTION_MODEM)
    //    {
    //        cout<<"���ߣ���������/n";
    //    }
    //    if ((flags & INTERNET_CONNECTION_LAN) ==INTERNET_CONNECTION_LAN)
    //    {
    //        cout<<"���ߣ�ͨ��������/n";
    //    }
    //    if ((flags & INTERNET_CONNECTION_PROXY) ==INTERNET_CONNECTION_PROXY)
    //    {
    //        cout<<"���ߣ�����/n";
    //    }
    //    if ((flags & INTERNET_CONNECTION_MODEM_BUSY) ==INTERNET_CONNECTION_MODEM_BUSY)
    //    {
    //        cout<<"MODEM��������INTERNET����ռ��/n";
    //    }
    //}
    //else
    //    cout<<"������/n";

    return m_bOnline;
}

string GetNetData(wstring &strURL)
{
	string strData = "";
	const size_t dwBuffer = 10240 * 5;
	HINTERNET hWeb, hRequest;
	DWORD dwReadByte = 0;
	char szBuffer[dwBuffer] = {};
	memset(szBuffer, '\0', dwBuffer);


	if (!(hWeb = InternetOpen(L"MY_STOCK", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0)))
		return strData;

	if (!(hRequest = InternetOpenUrl(hWeb, strURL.c_str(), NULL, 0, INTERNET_FLAG_DONT_CACHE, 0))) {
		InternetCloseHandle(hWeb);
		return strData;
	}

	while (InternetReadFile(hRequest, (PVOID)szBuffer, dwBuffer, &dwReadByte) && dwReadByte)
	{
		szBuffer[dwReadByte] = '\0';
		strData.append(szBuffer);
		memset(szBuffer, '\0', dwBuffer);
	}

	InternetCloseHandle(hRequest);
	InternetCloseHandle(hWeb);

	return strData;
}

bool GetNetFile(wstring &strUrl, wstring &strFile)
{
	bool bRet = false;

	const size_t dwBuffer = 10240 * 15;
	HINTERNET hWeb, hRequest;
	DWORD dwReadByte = 0;
	char szBuffer[dwBuffer] = "";
	HANDLE hdFile = INVALID_HANDLE_VALUE;
	hdFile = ::CreateFile(strFile.c_str(), GENERIC_ALL, FILE_SHARE_READ, NULL, CREATE_NEW, \
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (hdFile == INVALID_HANDLE_VALUE) return bRet;

	if (!(hWeb = InternetOpen(L"MY_STOCK", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0)))
		return bRet;

	if (!(hRequest = InternetOpenUrl(hWeb, strUrl.c_str(), NULL, 0, INTERNET_FLAG_DONT_CACHE, 0))) {
		InternetCloseHandle(hWeb);
		return bRet;
	}


	DWORD dwWriteSize = 0;
	DWORD dwWiteByte = 0;
	while (InternetReadFile(hRequest, (PVOID)szBuffer, dwBuffer, &dwReadByte) && dwReadByte)
	{
		dwWriteSize = dwReadByte;
		::WriteFile(hdFile, (PVOID)szBuffer, dwWriteSize, &dwWiteByte, NULL);
		memset(szBuffer, 0, dwBuffer);
	}
	bRet = true;
	CloseHandle(hdFile);
	InternetCloseHandle(hRequest);
	InternetCloseHandle(hWeb);
	return bRet;
}