#include <sstream>
#include <cstring>
#include <cstdlib>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Shlwapi.h>
#include "NTService.h"
#include "../commApi/CommApi.h"
#include "../AutoUtiles/AutoUtiles.h"
#include "../log/Log.h"

extern wstring g_wsLastError;


#define  CHK_ERROR(R) if(false == bSuccess){ sysErrorMsg(); LOG_ERR(g_wsLastError.c_str());} if(R) return bSuccess;

#define  GET_OPER(X) AutoServiceHandle  AutoH(X, m_sServiceName.c_str());
#define  CHK_AUTOH_ERROR  if(AutoH.IsError()) break;

NTService* NTService::m_pSelf = NULL;

NTService::NTService(const wstring &serviceName, const wstring &displayName)
: m_sServiceName(serviceName)
, m_sDisplayName(displayName)
{
    if (NULL != m_pSelf) {
        LOG_ERR(sysErrorMsg().c_str());  // throw tracing::runtime_error("duplicate NTService");
    }

    m_sDescription = SERVICE_DISP; 
    // Set the default service name and version
    m_dwServiceStartupType = SERVICE_AUTO_START;

    // set up the initial service status
    m_hServiceStatus = 0L;
    m_ServiceStatus.dwServiceType = SERVICE_WIN32;
    m_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    m_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;//| SERVICE_ACCEPT_SHUTDOWN
    m_ServiceStatus.dwWin32ExitCode = 0;
    m_ServiceStatus.dwServiceSpecificExitCode = 0;
    m_ServiceStatus.dwCheckPoint = 0;
    m_ServiceStatus.dwWaitHint = 0;

    // Fetch our module name
    TCHAR buf[1024];
    DWORD len = GetModuleFileName(NULL, buf, sizeof(buf));
    m_sModulePath.assign(buf, len);
    m_pSelf = this;
}

NTService::~NTService() {
    m_pSelf = NULL;
}


void NTService::RunService()
{ 
    return;
}

bool NTService::OnServiceStop()
{  
    return true;
} 

bool NTService::IsServiceInstalled() const
{
    GET_OPER(true);
    return false == AutoH.IsError();
    //CHK_ERROR(true);
}

bool NTService::InstallService() const 
{
    bool                bSuccess = false;   
    wstring             tmp(m_sDependencies);

    tmp.append(L"\0\0", 2);
    WCHAR szBuff[2048] = {0};
    wsprintf(szBuff, m_sDescription.c_str());


    GET_OPER(false);

    do 
    { 
        CHK_AUTOH_ERROR; 
        AutoH.hService = ::CreateService(AutoH.hSCM, m_sServiceName.c_str(),
            m_sDisplayName.c_str(),SERVICE_ALL_ACCESS, 
            SERVICE_WIN32_OWN_PROCESS,SERVICE_AUTO_START, 
            SERVICE_ERROR_IGNORE, m_sModulePath.c_str(), NULL,NULL,tmp.data(),NULL,NULL);  

        if(!AutoH.hService ) break;
        bSuccess = true;

        if (!m_sDescription.empty())  
        {
            SERVICE_DESCRIPTION sd;
            sd.lpDescription = szBuff;
            bSuccess = ChangeServiceConfig2(AutoH.hService, SERVICE_CONFIG_DESCRIPTION, &sd); 
        }
    } while (0);

    CHK_ERROR(true);
}

DWORD NTService::GetServiceStatus()
{
    bool             bSuccess = false; 
    SERVICE_STATUS   ServiceStatus; 
    DWORD            dwStatus = 0; 

    GET_OPER(true);
    do 
    {
        CHK_AUTOH_ERROR;
        if(QueryServiceStatus(AutoH.hService, &ServiceStatus) ==0)  break;
        bSuccess = true;
        dwStatus = ServiceStatus.dwCurrentState;
    } while (0);

    CHK_ERROR(false);

    return dwStatus;
}

bool NTService::UninstallService() const {

    bool             bSuccess = false;

    GET_OPER(true);

    do 
    {
        CHK_AUTOH_ERROR;
        bSuccess = ::DeleteService(AutoH.hService); 
    } while (0);

    CHK_ERROR(true); 
}


bool NTService::Start() const {  
    bool             bSuccess = false;

    GET_OPER(true);

    do 
    {
        CHK_AUTOH_ERROR; 
        bSuccess = ::StartService(AutoH.hService, 0, NULL); 
    } while (0);

    CHK_ERROR(true); 
}

bool NTService::Stop() const
{

    SERVICE_STATUS   ss;
    bool             bSuccess = false;

    GET_OPER(true);
    do 
    {
        CHK_AUTOH_ERROR;
        bSuccess = ::ControlService(AutoH.hService, SERVICE_CONTROL_STOP, &ss);  
    } while (0);

    CHK_ERROR(true);  
}


bool NTService::Execute() const 
{
    SERVICE_TABLE_ENTRY st[] =
    {
        {const_cast<TCHAR *>(m_sServiceName.c_str()), _ServiceMain},
        {NULL, NULL}
    };

    if (0 == ::StartServiceCtrlDispatcher(st))
    {
        if (ERROR_FAILED_SERVICE_CONTROLLER_CONNECT != ::GetLastError()) 
        { 
            LOG_ERR(sysErrorMsg().c_str());  
        }
        return false;
    }
    return true;
}

void NTService::ServiceMain(DWORD argc, TCHAR** argv) 
{
    m_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    m_hServiceStatus =
        ::RegisterServiceCtrlHandlerEx(m_sServiceName.c_str(), _ControlHandler, this);
    if (0L == m_hServiceStatus)  return;


    if (InitializeService(argc, argv)) 
    {
        m_ServiceStatus.dwWin32ExitCode = 0;
        m_ServiceStatus.dwCheckPoint = 0;
        m_ServiceStatus.dwWaitHint = 0;
        RunService();
    }

    SetServiceStatus(SERVICE_STOPPED);
}


wstring NTService::StatusString(DWORD state)
{
    wostringstream oss;
    switch (state) {
case SERVICE_CONTINUE_PENDING:
    oss << L"SERVICE_CONTINUE_PENDING";
    break;
case SERVICE_PAUSE_PENDING:
    oss << L"SERVICE_PAUSE_PENDING";
    break;
case SERVICE_PAUSED:
    oss << "SERVICE_PAUSED";
    break;
case SERVICE_RUNNING:
    oss << L"SERVICE_RUNNING";
    break;
case SERVICE_START_PENDING:
    oss << L"SERVICE_START_PENDING";
    break;
case SERVICE_STOP_PENDING:
    oss << L"SERVICE_STOP_PENDING";
    break;
case SERVICE_STOPPED:
    oss << L"SERVICE_STOPPED";
    break;
default:
    oss << L"Unknown value 0x" << hex << state;
    break;
    }
    return oss.str();
}

wstring NTService::CtrlString(DWORD ctrl)
{
    wostringstream oss;
    switch (ctrl)
    {
    case SERVICE_CONTROL_STOP:
        oss << L"SERVICE_CONTROL_STOP";
    case SERVICE_CONTROL_PAUSE:
        oss << L"SERVICE_CONTROL_PAUSE";
    case SERVICE_CONTROL_CONTINUE:
        oss << L"SERVICE_CONTROL_CONTINUE";
    case SERVICE_CONTROL_INTERROGATE:
        oss << L"SERVICE_CONTROL_INTERROGATE";
    case SERVICE_CONTROL_SHUTDOWN:
        oss << L"SERVICE_CONTROL_SHUTDOWN";
    case SERVICE_CONTROL_PARAMCHANGE:
        oss << L"SERVICE_CONTROL_PARAMCHANGE";
    case SERVICE_CONTROL_DEVICEEVENT:
        oss << L"SERVICE_CONTROL_DEVICEEVENT";
    case SERVICE_CONTROL_HARDWAREPROFILECHANGE:
        oss << L"SERVICE_CONTROL_HARDWAREPROFILECHANGE";
    case SERVICE_CONTROL_POWEREVENT:
        oss << L"SERVICE_CONTROL_POWEREVENT";
    case SERVICE_CONTROL_SESSIONCHANGE:
        oss << L"SERVICE_CONTROL_SESSIONCHANGE";
    default:
        oss << ((ctrl >= SERVICE_CONTROL_USER) ? L"User" : L"Invalid")
            << L" value 0x" << hex << ctrl;
    }

    return oss.str();
}

void NTService::SetServiceStatus(DWORD state)
{
    m_ServiceStatus.dwCurrentState = state;
    ReportServiceStatus();
}

DWORD NTService::ServiceExitCode() const 
{
    return m_ServiceStatus.dwWin32ExitCode;
}

void NTService::ReportServiceStatus() 
{
    ::SetServiceStatus(m_hServiceStatus, &m_ServiceStatus);
}

void NTService::AddDependency(const wstring &s) 
{
    m_sDependencies.append(s.c_str(), s.length() + 1);
}


bool NTService::InitializeService(DWORD argc, TCHAR** argv) 
{

    SetServiceStatus(SERVICE_START_PENDING);


    bool bResult = OnServiceInit(argc, argv);


    m_ServiceStatus.dwWin32ExitCode = GetLastError();
    m_ServiceStatus.dwCheckPoint = 0;
    m_ServiceStatus.dwWaitHint = 0;
    if (!bResult)
    {
        SetServiceStatus(SERVICE_STOPPED);
        return false;
    }
    m_bServiceRunning = true;
    SetServiceStatus(SERVICE_RUNNING);
    return true;
}


DWORD NTService::ControlHandler(DWORD opcode, DWORD eventType, void * eventData)
{
    DWORD ret = ERROR_CALL_NOT_IMPLEMENTED;

    switch (opcode) 
    {
    case SERVICE_CONTROL_STOP:
        SetServiceStatus(SERVICE_STOP_PENDING);
        OnServiceStop();
        m_bServiceRunning = false;
        ret = NO_ERROR;
        break;
    case SERVICE_CONTROL_PAUSE:
        if (OnServicePause())
            ret = NO_ERROR;
        break;
    case SERVICE_CONTROL_CONTINUE:
        if (OnServiceContinue())
            ret = NO_ERROR;
        break;
    case SERVICE_CONTROL_INTERROGATE:
        if (OnServiceInterrogate())
            ret = NO_ERROR;
        break;
    case SERVICE_CONTROL_SHUTDOWN:
        if (OnServiceShutdown())
            ret = NO_ERROR;
        break;
    case SERVICE_CONTROL_PARAMCHANGE:
        if (OnServiceParamChange())
            ret = NO_ERROR;
        break;
    case SERVICE_CONTROL_DEVICEEVENT:
        if (OnServiceDeviceEvent())
            ret = NO_ERROR;
        break;
    case SERVICE_CONTROL_HARDWAREPROFILECHANGE:
        if (OnServiceHardwareProfileChange())
            ret = NO_ERROR;
        break;
    case SERVICE_CONTROL_POWEREVENT:
        if (OnServicePowerEvent())
            ret = NO_ERROR;
        break;
    case SERVICE_CONTROL_SESSIONCHANGE:
        if (OnServiceSessionChange())
            ret = NO_ERROR;
        break;
    default:
        if (opcode >= SERVICE_CONTROL_USER) 
        {
            if (OnServiceUserControl(opcode - SERVICE_CONTROL_USER))
                ret = NO_ERROR;
        }
        break;
    }
    return ret;
}


void NTService::_ServiceMain(unsigned long argc, TCHAR **argv) 
{
    if (m_pSelf) 
    {
        m_pSelf->ServiceMain(argc, argv);
    }
}


unsigned long NTService::_ControlHandler(unsigned long opcode, unsigned long eventType,
                                         void *eventData, void *context) 
{
    NTService *self = reinterpret_cast<NTService *>(context);
    if (NULL != self) 
    {
        return self->ControlHandler(opcode, eventType, eventData);
    }
    return ERROR_CALL_NOT_IMPLEMENTED;
}

bool NTService::OnServiceInit(DWORD, TCHAR **) 
{
    return true;
}



bool NTService::OnServiceInterrogate()
{
    ReportServiceStatus();
    return true;
}


bool NTService::OnServicePause() 
{
    return false;
}


bool NTService::OnServiceContinue()
{
    return false;
}


bool NTService::OnServiceShutdown() 
{
    return false;
}


bool NTService::OnServiceParamChange() 
{
    return false;
}


bool NTService::OnServiceDeviceEvent() 
{
    return false;
}


bool NTService::OnServiceHardwareProfileChange() 
{
    return false;
}


bool NTService::OnServicePowerEvent() 
{
    return false;
}


bool NTService::OnServiceSessionChange() 
{
    return false;
}

bool NTService::OnServiceUserControl(DWORD opcode)
{
    return false; 
}

std::wstring NTService::GetErrorStr() const
{
    return g_wsLastError;
}
