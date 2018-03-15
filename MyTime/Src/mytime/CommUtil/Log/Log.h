/********************************************************************
//Copyright (C), 2017-2018, http://www.caipiao365.vip/
文件名:		Log.h 
创建者:		poorboy
创建时间:	2018/2/27 10:58 
修改时间:
文件说明:	日志头文件 
*********************************************************************/

#ifndef __LOG_HEADER__
#define __LOG_HEADER__ 
#pragma once 
#pragma warning (disable:4251)


#define LOG_DEBUG        (0)
#define LOG_WARNING      (1)
#define LOG_INFOS        (2)
#define LOG_ERROR        (3)  
#define LOG_NONE         (4)  

//以字串方试返回当前时间
std::wstring GetLogTimeString();

void Log(int who, int level, LPCTSTR message, int line, const char* file, const char * funName);
void MyLogING(int lever, const char * szfile, int nLine, LPCTSTR wszPrintString, ...);

#define MY_LOG_INIT
#define MYLOG(lever, msg, ...)     MyLogING( lever, __FILE__, __LINE__, msg, ## __VA_ARGS__)

#define LOG_DBG(msg,...)             MYLOG(LOG_DEBUG, msg, ## __VA_ARGS__)
#define LOG_WARN(msg, ...)           MYLOG(LOG_WARNING, msg, ## __VA_ARGS__)
#define LOG_INFO(msg, ...)           MYLOG(LOG_INFOS, msg, ## __VA_ARGS__)
#define LOG_ERR(msg, ...)            MYLOG(LOG_ERROR, msg, ## __VA_ARGS__)

class CLog
{
public:
    CLog(void);
    ~CLog(void);
    void Loging();
    void Loging(int level, wstring &mesg, int line, wstring &fileName);
    void InitData(void);
    void ClearData(void);
    void FinshLog(int nTime);
    bool bIsInit;
    bool MyCreateFile(HANDLE &hFile, wstring &wsFileName);
protected:
    int m_nwho;
    int m_nlevel;
    wstring m_strmesg;
    int m_nline;
    wstring m_strfileName;
    wstring m_strlogfileName;
    wstring m_strlogfileName1;
    wstring m_strlogDIR;
    SYSTEMTIME m_LastWriteTime;
    string  m_strLog;
    wstring m_strLogSYS;
    bool m_bIsClearLog;
    int m_nCuuDay;
    HANDLE hLogFile;
    static bool bIsInstall;


    class Lock
    {
    public:
        Lock(CLog & _Log) : m_Log(_Log)
        {
            m_Log.lock();
        }
        ~Lock()
        {
            m_Log.unlock();
        }
    private:
        CLog &m_Log;
    };
    friend class Lock;

protected:
    void lock()
    {
        EnterCriticalSection(&_lock);
    }

    void unlock()
    {
        LeaveCriticalSection(&_lock);
    }
protected:
    CRITICAL_SECTION _lock;

};

CLog * GetSysLog();
#endif //__LOG_HEADER__