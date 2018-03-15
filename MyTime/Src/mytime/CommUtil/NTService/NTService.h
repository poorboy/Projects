#ifndef _NTSERVICE_H_
#define _NTSERVICE_H_

#include <Winsvc.h> 
#include <string>


class NTService 
{

public:
    enum {
        SERVICE_CONTROL_USER = 128
    };

    /**
    * Constructor
    * @param displayName Mandatory display name of the service
    * @param serviceName Optional internal service name
    */
    NTService(const std::wstring & displayName, const std::wstring & serviceName = L"");

    // Destructor
    virtual ~NTService();

    /**
    * Retrieve installation status
    * @return true, if the service is installed.
    * @throws An instance of tracing::runtime_error, if the service
    * manager can not be contacted.
    */
    bool IsServiceInstalled() const;

    /**
    * Starts the service.
    * @throws An instance of tracing::runtime_error, if the service
    * manager can not be contacted or the service can not be started.
    */
    bool Start() const;

    /**
    * Stops the service.
    * @throws An instance of tracing::runtime_error, if the service
    * manager can not be contacted or the service can not be stopped.
    */
    bool Stop() const;

    /**
    * Installs the service.
    * @throws An instance of tracing::runtime_error, if the service
    * manager can not be contacted or the service can not be installed.
    */
    bool InstallService() const;

    DWORD GetServiceStatus();

    /**
    * Uninstalls the service.
    * @throws An instance of tracing::runtime_error, if the service
    * manager can not be contacted or the service can not be uninstalled.
    */
    bool UninstallService() const;

    /**
    * Attempts to start the service control dispatcher.
    * @return true, if the dispatcher was successfully started.
    * @throws An instance of tracing::runtime_error, if the service
    * manager can not be contacted.
    */
    bool Execute() const;

    /**
    * Retrieves the exit code of the service.
    * @return The exit code of the service after stopping the service.
    */
    DWORD ServiceExitCode() const;

    virtual void RunService();  

    std::wstring GetErrorStr() const; 
protected:
    /**
    * Perform initialization using the service arguments.
    * To be overridden by your derived class. The default
    * implementation just returns true.
    * @param argc The number of arguments.
    * @param argv A NULL terminated array of string pointers.
    * @return true on success.
    */
    virtual bool OnServiceInit(DWORD argc, TCHAR **argv);
    /**
    * Runs the service main loop.
    * To be overridden by your derived class.
    * The default implementation just loops until
    * the service is stopped.
    */ 
    /**
    * Notifies the service to be stopped.
    * To be overridden by your derived class.
    * The default implementation just returns true.
    * @return true on success.
    */
    virtual bool OnServiceStop();
    /**
    * Notifies the service to be queried.
    * To be overridden by your derived class.
    * The default implementation just returns true.
    * @return true on success.
    */
    virtual bool OnServiceInterrogate();
    /**
    * Notifies the service to be paused.
    * To be overridden by your derived class.
    * The default implementation just returns true.
    * @return true on success.
    */
    virtual bool OnServicePause();
    /**
    * Notifies the service to resume.
    * To be overridden by your derived class.
    * The default implementation just returns true.
    * @return true on success.
    */
    virtual bool OnServiceContinue();
    /**
    * Notifies the service about a system shutdown.
    * To be overridden by your derived class.
    * The default implementation just returns true.
    * @return true on success.
    */
    virtual bool OnServiceShutdown();
    /**
    * Notifies the service about a parameter change.
    * To be overridden by your derived class.
    * The default implementation just returns true.
    * @return true on success.
    */
    virtual bool OnServiceParamChange();
    /**
    * Notifies the service about a device event.
    * To be overridden by your derived class.
    * The default implementation just returns true.
    * @return true on success.
    */
    virtual bool OnServiceDeviceEvent();
    /**
    * Notifies the service about a changed hardware profile.
    * To be overridden by your derived class.
    * The default implementation just returns true.
    * @return true on success.
    */
    virtual bool OnServiceHardwareProfileChange();
    /**
    * Notifies the service about a power management event.
    * To be overridden by your derived class.
    * The default implementation just returns true.
    * @return true on success.
    */
    virtual bool OnServicePowerEvent();
    /**
    * Notifies the service about a session change.
    * To be overridden by your derived class.
    * The default implementation just returns true.
    * @return true on success.
    */
    virtual bool OnServiceSessionChange();
    /**
    * Notifies the service about a user-defined control message.
    * To be overridden by your derived class.
    * The default implementation just returns true.
    * @param opcode The user defined control code.
    * @return true on success.
    */

    virtual bool OnServiceUserControl(DWORD opcode);

    /**
    * Converts a service state to a human readable string.
    * @param state The state to convert.
    * @return The string representation of the provided state.
    */
    std::wstring StatusString(DWORD state);
    /**
    * Converts a control code to a human readable string.
    * @param ctrl The control code to convert.
    * @return The string representation of the provided control code.
    */
    std::wstring CtrlString(DWORD ctrl);
    /**
    * Adds a dependency to the list of prerequisites for this service.
    * @param s The name of another service on which this service depends.
    */
    void AddDependency(const std::wstring &s);
    /**
    * Changes and reports the service status to the service control manager.
    * @param state The new state of this service.
    */
    void SetServiceStatus(DWORD state);
    /**
    * Reports the current service status to the service control manager.
    */
    void ReportServiceStatus();

    // data members
protected:
    /// Flag: service is running
    bool m_bServiceRunning;
    /// The startup type of this service
    DWORD m_dwServiceStartupType;
    /// The startupt type of this service
    SERVICE_STATUS m_ServiceStatus;
    /// The internal service name
    std::wstring m_sServiceName;
    /// The display name
    std::wstring m_sDisplayName;
    /// An optional description
    std::wstring m_sDescription;
    /// Internal path to our executable
    std::wstring m_sModulePath;

private:
    bool InitializeService(DWORD argc, TCHAR **argv);
    void ServiceMain(DWORD argc, TCHAR **argv);
    DWORD ControlHandler(DWORD opcode, DWORD eventType, void *eventData);

    std::wstring m_sDependencies;
    SERVICE_STATUS_HANDLE m_hServiceStatus;


    // nasty hack to get object ptr
    static NTService* m_pSelf;

    // static callback functions (called from Service-Manager)
    static void WINAPI _ServiceMain(unsigned long argc, TCHAR **argv);
    static unsigned long WINAPI _ControlHandler(unsigned long opcode,
        unsigned long eventType, void *eventData, void *context); 
}; 

#endif // _NTSERVICE_H_
