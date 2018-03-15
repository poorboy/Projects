/********************************************************************
文件名:		MainDlg.h
创建者:		poorboy
创建时间:	[7/2/2014 15:23]
修改时间:
文件说明:	主界面
*********************************************************************/
#ifndef MainDlg_h__
#define MainDlg_h__
#pragma once 

#include "baseDlg.h"

class MainDlg: public BaseDlg
{
public:
    MainDlg(void);
    ~MainDlg(void);

    virtual CDuiString  GetSkinFile() { return _T("Main.xml"); }
    virtual LPCTSTR  GetWindowClassName() const;
    void    Notify(TNotifyUI& msg);    
    virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 
    static DWORD WINAPI WorkThread(LPVOID lpPara);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    
    void SetWinVisb();
    virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);	


    LPWSTR GetWinTitle();
    bool CreateWin();

protected: 
    void OnPrepare();   

    void OnInstallSvc();
    void OnStartSvc();
    void OnStopSvc();
    void OnDelSvc(); 
    void DisbaleBnt();
    void OnDelSIPBnt();
    void OnAddSIPBnt();
    
    void OnBntExit();
    void InitBntStatus();

    virtual void OnSelectChangs(TNotifyUI& msg);
    virtual void OnItemSelect(TNotifyUI& msg);

    void OnSyncTime();
    void OnShowNoteDlg();

    void LogMsg(UString &strLog);
    void LogMsg(LPCTSTR szLog);

protected:
    CButtonUI * pbnt_Exit;

    CButtonUI *  bnt_InstallService;
    CButtonUI *  bnt_startService;
    CButtonUI *  bnt_stopService;
    CButtonUI *  bnt_DelService;

    CRichEditUI * pet_Logs; 

    CLabelUI * pLTime;
    CLabelUI * pNTime; 
    CLabelUI * pSIP; 

    CTimeEx m_lTime;
    CTimeEx m_nTime;

    CComboUI * pSList;

    CControlUI * pSPan;
    CEditUI    * pEtSTime;
    bool         bIsChangIP;
};


#endif // MainDlg_h__
