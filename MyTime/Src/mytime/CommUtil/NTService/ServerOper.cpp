/********************************************************************
文件名:		ServerOper.cpp 
创建者:		poorboy
创建时间:	2018/3/9 1:52 
网   址 :   http://www.caipiao365.vip/
修改时间:
文件说明:	  
*********************************************************************/

#include "stdafx.h"
#include "NTService.h"







class ServerOpear
{
public:
    ServerOpear(LPCWSTR lpServiceName, DWORD dwScmSccess=SC_MANAGER_ALL_ACCESS, DWORD dwAccess = SERVICE_ALL_ACCESS);
    ~ServerOpear(); 
    bool IsOperaSuccess();

    bool IsServiceInstall();

    bool IntallService(const std::wstring _sDependencies,
        const std::wstring _sServiceName, 
        const std::wstring _sDisplayName, 
        const std::wstring _sDescription,
        const std::wstring _sModulePath,
        const DWORD   dwServiceType,
        const DWORD   dwServiceStartupType
        );
    bool DelService();

    DWORD GetServiceStatus();
    bool StopService();
    bool StartService();
public:
    SC_HANDLE        hSCM;
    SC_HANDLE        hService;
    bool             bIsSuccess;
};


//////////////////////////////////////////////////////////////////////////
ServerOpear::ServerOpear(LPCWSTR lpServiceName, DWORD dwScmSccess, DWORD dwAccess)
{
    bIsSuccess = false;
    hSCM = NULL;
    hService = NULL; 

    hSCM = ::OpenSCManager(NULL, NULL, dwScmSccess);
    if(hSCM == NULL) return;
    hService = ::OpenService(hSCM, lpServiceName, dwAccess);
    bIsSuccess = true;
}

ServerOpear::~ServerOpear()
{
    if(hService)    CloseServiceHandle(hService); 
    if(hSCM)        CloseServiceHandle(hSCM);
}

bool ServerOpear::IsServiceInstall()
{
    return bIsSuccess;
}

bool ServerOpear::IntallService(
                                const wstring _sDependencies,
                                const wstring _sServiceName, 
                                const wstring _sDisplayName,
                                const wstring _sDescription,
                                const wstring _sModulePath,
                                const DWORD   dwServiceType,
                                const DWORD   dwServiceStartupType
                                )
{
    wstring             tmp(_sDependencies);
    tmp.append(L"\0\0", 2);
    do 
    {
        if(hSCM == NULL) break;

        hService = ::CreateService(hSCM, _sServiceName.c_str(),
            _sDisplayName.c_str(), SERVICE_ALL_ACCESS, dwServiceType,
            dwServiceStartupType, SERVICE_ERROR_NORMAL, _sModulePath.c_str(),
            NULL, NULL, tmp.data(), NULL, NULL);

        if(hService == NULL)  break; 

        if (!_sDescription.empty())
        {
            tmp = _sDescription;
            tmp.append(L"\0\0", 2);
            SERVICE_DESCRIPTION sd;
            sd.lpDescription = StrDup(tmp.c_str()); 
            bIsSuccess = ChangeServiceConfig2(hSCM, SERVICE_CONFIG_DESCRIPTION, &sd);
            free(sd.lpDescription);
        } 
    }while(0);

    return bIsSuccess;
}

bool ServerOpear::DelService()
{ 
    if(bIsSuccess)  bIsSuccess = ::DeleteService(hService); 
    return bIsSuccess;
}


DWORD ServerOpear::GetServiceStatus()
{  
    DWORD            dwStatus = 0;
    if(bIsSuccess)
    {
        SERVICE_STATUS   ServiceStatus; 
        bIsSuccess = QueryServiceStatus(hService, &ServiceStatus);
        if(bIsSuccess) dwStatus = ServiceStatus.dwCurrentState;
    } 
    return dwStatus; 
}

bool ServerOpear::StopService()
{
    if(bIsSuccess)
    {
        SERVICE_STATUS ss;
        bIsSuccess = ::ControlService(hService, SERVICE_CONTROL_STOP, &ss);
    } 
    return bIsSuccess;
}

bool ServerOpear::StartService()
{
    if(bIsSuccess)
    {
        bIsSuccess = ::StartServiceA(hService, 0, NULL);
    } 
    return bIsSuccess; 
}

bool ServerOpear::IsOperaSuccess()
{ 
    return bIsSuccess; 
}
