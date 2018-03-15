#include "StdAfx.h"
#include "../stdafx.h"
#include "MainDlg.h"
#include <windows.h>
#include <crtdbg.h>
#include <netfw.h>
#include <objbase.h>
#include <oleauto.h>
#include <stdio.h>
#include "../TimeService/TimeService.h"
#include "../TimeSockService/TimeSockService.h" 
#include "NoteDlg.h"

#pragma comment( lib, "ole32.lib" )
#pragma comment( lib, "oleaut32.lib" )



#define SET_BNT_ENB(X,Y) if(X) X->SetEnabled(Y);

#define  ID_MENU_SHOW   1000
#define  ID_MENU_START  ID_MENU_SHOW+1
#define  ID_MENU_EXIT   ID_MENU_START+1 

void SetWin2Foreg(HWND hWnd)
{
    if(!hWnd) return;
    HWND hForeWnd = NULL;  
    DWORD dwForeID; 
    DWORD dwCurID; 
    if(!IsWindowVisible(hWnd)) ::ShowWindow(hWnd, SW_SHOW); 
    hForeWnd =  GetForegroundWindow(); 
    dwCurID =  GetCurrentThreadId(); 
    dwForeID =  GetWindowThreadProcessId( hForeWnd, NULL ); 
    AttachThreadInput( dwCurID, dwForeID, TRUE); 
    ShowWindow( hWnd, SW_SHOWNORMAL ); 
    SetWindowPos( hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE ); 
    SetWindowPos( hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE ); 
    SetForegroundWindow( hWnd ); 
    AttachThreadInput( dwCurID, dwForeID, FALSE);
}



MainDlg::MainDlg(void)
{
    bIsEscHide = true;
    bIsHandSysMsg = true;

    bIsChangIP = false;


    pbnt_Exit = NULL;

    bnt_InstallService = NULL;
    bnt_startService = NULL;
    bnt_stopService = NULL;
    bnt_DelService = NULL;


    pLTime  = NULL;
    pNTime  = NULL;
    pSIP    = NULL;  

    pSList = NULL; 
    pSPan = NULL; 
    pEtSTime = NULL; 

    GetTSockService()->StartService();
}

MainDlg::~MainDlg(void)
{ 
    GetTSockService()->Stop();
} 

void MainDlg::OnInstallSvc()
{
    if(!GetGService()->InstallService()) 
    { 
        UString strMsg;
        strMsg.Format(L"安装服务失败!\r\n%s", GetGService()->GetErrorStr().c_str());
        MSGOK_GUI(strMsg.GetData());
        return;
    }

    if (bnt_InstallService) bnt_InstallService->SetEnabled(false);
    if (bnt_startService)   bnt_startService->SetEnabled(true);
    if (bnt_stopService)    bnt_stopService->SetEnabled(true);
    if (bnt_DelService)     bnt_DelService->SetEnabled(true);
}

void  MainDlg::DisbaleBnt()
{
    SET_BNT_ENB(bnt_InstallService, false);
    SET_BNT_ENB(bnt_startService, false);
    SET_BNT_ENB(bnt_stopService, false);
    SET_BNT_ENB(bnt_DelService, false); 
}
void MainDlg::OnStartSvc()
{  
    DisbaleBnt();

    if(!GetGService()->Start()) 
    {
        UString strMsg;
        strMsg.Format(L"启动服务失败!\r\n%s", GetGService()->GetErrorStr().c_str());
        MSGOK_GUI(strMsg.GetData()); 
        SET_BNT_ENB(bnt_startService, true);
        return;
    } 
    SET_BNT_ENB(bnt_stopService, true); 
}

void MainDlg::OnStopSvc(){

    DisbaleBnt();

    if(!GetGService()->Stop()) 
    { 
        UString strMsg;
        strMsg.Format(L"停止服务失败!\r\n%s", GetGService()->GetErrorStr().c_str());
        MSGOK_GUI(strMsg.GetData());
        SET_BNT_ENB(bnt_stopService, true);
        return;
    } 

    SET_BNT_ENB(bnt_startService, true); 
    SET_BNT_ENB(bnt_DelService, true); 
}

void MainDlg::OnSyncTime()
{
    CControlUI*  pBnt1 = m_PaintManager.FindControl(L"bnt_UpTime");
    if(pBnt1) pBnt1->SetEnabled(false);

    SYSTEMTIME sysTime;
    ZeroMemory(&sysTime, sizeof(SYSTEMTIME)); 

    CTimeEx tmpTime = m_nTime;
    tmpTime.AddSecond(1);
    if(tmpTime.ToSystemTime(sysTime)) ::SetLocalTime(&sysTime);

    if(pBnt1) pBnt1->SetEnabled(true);

}

void MainDlg::OnShowNoteDlg()
{
    NoteDlg _dlg(GetHWND());
    if(_dlg.CreateWin())
    {
        _dlg.ShowModal();
    }
}

void MainDlg::OnDelSvc(){  

    DisbaleBnt();
    if(!GetGService()->UninstallService())  
    { 
        UString strMsg;
        strMsg.Format(L"删除服务失败!\r\n%s", GetGService()->GetErrorStr().c_str());
        MSGOK_GUI(strMsg.GetData());
        SET_BNT_ENB(bnt_DelService, true); 
        return;
    }
    SET_BNT_ENB(bnt_InstallService, true); 

} 

void MainDlg::OnPrepare() 
{ 
    SetIcon(128);  

    GetTSockService()->SetGUIHwnd(GetHWND());

    pbnt_Exit = static_cast<CButtonUI *> (m_PaintManager.FindControl(L"bnt_Exit")); ;
    pet_Logs =  static_cast<CRichEditUI *> (m_PaintManager.FindControl(L"et_Logs"));


    bnt_InstallService =  static_cast<CButtonUI *> (m_PaintManager.FindControl(L"bnt_InstallService")); 
    bnt_startService   =  static_cast<CButtonUI *> (m_PaintManager.FindControl(L"bnt_startService")); 
    bnt_stopService    =  static_cast<CButtonUI *> (m_PaintManager.FindControl(L"bnt_stopService")); 
    bnt_DelService     =  static_cast<CButtonUI *> (m_PaintManager.FindControl(L"bnt_DelService"));  

    pLTime =  static_cast<CLabelUI *> (m_PaintManager.FindControl(L"lb_lTime"));
    pNTime =  static_cast<CLabelUI *> (m_PaintManager.FindControl(L"lb_nTime")); 

    pSIP  =  static_cast<CLabelUI *> (m_PaintManager.FindControl(L"lb_SIP")); 

    pSPan =  m_PaintManager.FindControl(L"s_pan"); 
    pEtSTime = static_cast<CEditUI *> (m_PaintManager.FindControl(L"et_Stime")); 

    if(pEtSTime) pEtSTime->SetNumberOnly(true);

    int nSyncType = Get_GSet()->GetSyncType();

    if( nSyncType == 1)
    {
        COptionUI * pOptSync =  static_cast<COptionUI *> (m_PaintManager.FindControl(L"opt_RunOnce"));
        if(pSPan) pSPan->SetVisible(false);
        if(pOptSync) pOptSync->Selected(true);
        static_cast<COptionUI *> (m_PaintManager.FindControl(L"opt_Sync30M"));  
    }else if(nSyncType  == 2)
    {
        if(pEtSTime)
        {
            UString strTmp;
            strTmp.Format(L"%d", Get_GSet()->GetSTime());
            pEtSTime->SetText(strTmp);
        }

        if(pSPan) pSPan->SetVisible(true);

        COptionUI * pOptSync =  static_cast<COptionUI *> (m_PaintManager.FindControl(L"opt_SyncByTime"));
        if(pOptSync) pOptSync->Selected(true);
    }

    UString strOptName = L"";

    switch(Get_GSet()->GetDebugLevel())
    {
    case LOG_INFOS:
        strOptName = L"opt_LogInfo";
        break;
    case LOG_ERROR:
        strOptName = L"opt_LogErr";
        break;
    case LOG_NONE: 
        strOptName = L"opt_LogNone";
        break;
    } 

    COptionUI * pOptLog =  static_cast<COptionUI *> (m_PaintManager.FindControl(strOptName.GetData()));
    if(pOptLog) pOptLog->Selected(true);

    pSList =  static_cast<CComboUI *> (m_PaintManager.FindControl(L"cm_SType")); 

    if (pSList)
    {
        pSList->RemoveAll();

        UString strTitle;
        vector<NtpItem> &tmp = Get_GSet()->GetSList();
        for (int i = 0; i < tmp.size(); i++)
        {
            NtpItem &_item = tmp[i];
            CListLabelElementUI *pElm = new CListLabelElementUI;
            if (pElm)
            {
                pSList->Add(pElm);
                pElm->SetOwner(pSList);
                strTitle.Format(L"%s(%s)", _item.strName.c_str(), _item.strIp.c_str());
                pElm->SetText(strTitle.GetData());
            }
        } 
        pSList->SelectItem(Get_GSet()->GetCuuSIDType());
        if(pSIP) pSIP->SetText(pSList->GetItemAt(Get_GSet()->GetCuuSIDType())->GetText());
    }

    SetTimer(*this,1,1000, NULL);
    InitBntStatus();
    if (pet_Logs)pet_Logs->SetReadOnly(true);
}
//-

void MainDlg::InitBntStatus( )
{
    bool bInstall = GetGService()->IsServiceInstalled();
    DWORD dwStatus = 0;

    if (!bInstall)
    {
        SET_BNT_ENB(bnt_InstallService, true);
        SET_BNT_ENB(bnt_startService,   false);
        SET_BNT_ENB(bnt_stopService,    false); 
        SET_BNT_ENB(bnt_DelService,     false);
        return;
    }

    SET_BNT_ENB(bnt_InstallService, false);

    dwStatus = GetGService()->GetServiceStatus();
    SET_BNT_ENB(bnt_DelService,    true);

    if (dwStatus == SERVICE_RUNNING)
    {
        SET_BNT_ENB(bnt_startService,  false); 
        SET_BNT_ENB(bnt_stopService,   true);
        SET_BNT_ENB(bnt_DelService,    false); 

    }else if (dwStatus == SERVICE_STOPPED)
    {
        SET_BNT_ENB(bnt_startService,  true); 
        SET_BNT_ENB(bnt_stopService,   false);
    }else
    {
        SET_BNT_ENB(bnt_startService,  false); 
        SET_BNT_ENB(bnt_stopService,   false);
    } 
}


void MainDlg::Notify(TNotifyUI& msg)
{
    if( msg.sType == _T("windowinit")) OnPrepare();
    else if( msg.sType == _T("click") ) 
    { 
        CDuiString strName = msg.pSender->GetName();
        if(strName== L"btn_Refersh" )                InitBntStatus(); 
        else if (strName == L"bnt_UpTime")           OnSyncTime();
        else if (strName == L"bnt_Note")             OnShowNoteDlg(); 
        else if (strName == L"bnt_Add")              OnAddSIPBnt();
        else if (strName == L"bnt_Del")              OnDelSIPBnt();
        else if (strName == L"bnt_src")              ShellExecute(NULL, L"open", L"https://gitee.com/zuzong/Projects/tree/master/MyTime", NULL, NULL, SW_SHOW);
        else if (strName == L"bnt_bolg")             ShellExecute(NULL, L"open", L"http://www.caipiao365.vip/article/MyTime", NULL, NULL, SW_SHOW); 
        else if (bnt_InstallService == msg.pSender)  OnInstallSvc();
        else if (bnt_startService == msg.pSender)    OnStartSvc();
        else if (bnt_stopService == msg.pSender)     OnStopSvc();
        else if (bnt_DelService == msg.pSender)      OnDelSvc(); 
        else if (pbnt_Exit == msg.pSender)           OnBntExit();  	
    }
    else if(msg.sType == DUI_MSGTYPE_SELECTCHANGED) OnSelectChangs(msg);
    else if(msg.sType == DUI_MSGTYPE_ITEMSELECT) OnItemSelect(msg);
    else if (msg.sType == DUI_MSGTYPE_KILLFOCUS) //DUI_MSGTYPE_TEXTCHANGED
    {
        if(msg.pSender == pEtSTime)
        {
            UString strTmp = pEtSTime->GetText();
            int nTime = _tstoi(strTmp.GetData());
            if (nTime > 0 &&  nTime < 61) Get_GSet()->SetSTime(nTime);
            else
            {
                strTmp.Format(L"%d", Get_GSet()->GetSTime());
                pEtSTime->SetText(strTmp.GetData());
            }
        }
    }
}  

void MainDlg::OnAddSIPBnt()
{
    CControlUI*  pEt1 = m_PaintManager.FindControl(L"et_addNtpAddress");
    CControlUI*  pEt2 = m_PaintManager.FindControl(L"et_addNtpName");
    NtpItem _item;
    if (pEt1) _item.strIp = pEt1->GetText().GetData();
    if (pEt2) _item.strName = pEt2->GetText().GetData(); 

    if (_item.strName.length() < 1)
    {
        MSGOK_GUI(L"请输入服务器名称!");
        if (pEt2) pEt2->SetFocus();
        return;
    }

    if (_item.strIp.length() < 1)
    {
        MSGOK_GUI(L"请输入服务器的域名或者IP地址!");
        if (pEt1) pEt1->SetFocus();
        return;
    }

    wstring strError; 
    if(!Get_GSet()->AddSItem( _item, strError))
    {
        MSGOK_GUI(strError.c_str());
    }else
    {
        UString strTitle;
        CListLabelElementUI *pElm = new CListLabelElementUI;
        if (pElm)
        {
            pSList->Add(pElm);
            pElm->SetOwner(pSList);
            strTitle.Format(L"%s(%s)", _item.strName.c_str(), _item.strIp.c_str());
            pElm->SetText(strTitle.GetData());
        }  
        if (pEt1) pEt1->SetText(L"");
        if (pEt2) pEt2->SetText(L"");
        MSGOK_GUI(L"添加成功!");
    }

}
void MainDlg::OnDelSIPBnt()
{
    UString strNote;
    strNote.Format(L"确定要删除[%s]吗?", pSList->GetItemAt(pSList->GetCurSel())->GetText());
    if(MSGBOX_GUI(strNote, MB_YESNO) == IDYES)
    {
        wstring strError;
        if(!Get_GSet()->DelSItem(pSList->GetCurSel(), strError))
        {
            MSGOK_GUI(strError.c_str());
        }else
        {
            pSList->RemoveAt(pSList->GetCurSel());
        }
    }
}
LRESULT MainDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{  
    KillTimer(*this,1);
    PostQuitMessage(0);
    return 0;
}


bool MainDlg::CreateWin()
{
    CreateDuiWindow(NULL, GetWinTitle(), WS_OVERLAPPEDWINDOW);
    this->ShowWindow(false);
    CenterWindow(); 

    SetWinVisb();
    return true;
}

LPWSTR MainDlg::GetWinTitle()
{
    return MAIN_GUI_TITLE;
}


LPCTSTR  MainDlg::GetWindowClassName() const
{
    return MAIN_GUI_CLASS_NAME;
}

LRESULT MainDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    int tID = (int)wParam; 
    if (tID == 1) 
    {
        InitBntStatus();
        GetTSockService()->DoUpTime(); 
        m_lTime = CTimeEx::GetCurrentTime();
        if(pLTime) pLTime->SetText(m_lTime.ToStringEX().c_str());
    }
    return 0L;
}

void MainDlg::SetWinVisb()
{		
    HWND hWnd= GetHWND(); 
    ::ShowWindow(hWnd, IsWindowVisible(hWnd) ? SW_HIDE : SW_SHOW);
    if(IsWindowVisible(hWnd)) SetWin2Foreg(hWnd); 
}

void MainDlg::OnBntExit()
{
    Close();
}


LRESULT MainDlg::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{ 
    if (uMsg == WM_GET_NET_TIME)
    {
        bHandled = TRUE;
        UINT64 uTime = (UINT64 ) lParam;
        m_nTime = CTimeEx(uTime);
        UString strTmp( L"获取失败!请换服务器!"); 

        if(uTime != 0) strTmp = m_nTime.ToStringEX().c_str(); 


        m_lTime = CTimeEx::GetCurrentTime();
        if(pLTime) pLTime->SetText(m_lTime.ToStringEX().c_str());

        if(pNTime) pNTime->SetText(strTmp.GetData()); 

    }else if (uMsg == WM_GET_NET_TIME_ERR)
    {
        LogMsg((const wchar_t * ) lParam); 
    }
    return BaseDlg::HandleCustomMessage(uMsg,  wParam,  lParam,  bHandled);
}


void MainDlg::OnItemSelect(TNotifyUI& msg)
{
    if(msg.pSender == pSList)
    {
        int nID = pSList->GetCurSel();
        Get_GSet()->SetCuuSIDType(nID);
        wstring strIP = L"";
        UString strLogs;
        if(Get_GSet()->GetItemSIP(nID, strIP))
        {
            string sIP = W2A(strIP).c_str();
            strLogs.Format(L"正在从[%s]获取时间\r", Get_GSet()->GetItemSName(nID));

            LogMsg(strLogs); 
            bIsChangIP = true;

            if(pNTime) pNTime->SetText(L"正在获取网络时间");

            if(pSIP) pSIP->SetText(pSList->GetItemAt(Get_GSet()->GetCuuSIDType())->GetText());
            GetTSockService()->SetNtpIP(sIP);
        }
    }
}

void MainDlg::OnSelectChangs(TNotifyUI& msg)
{ 

    COptionUI * pCuuOpt =  static_cast<COptionUI *> (msg.pSender);
    if (!pCuuOpt) return;

    if (!pCuuOpt->IsSelected()) return;

    UString strName = pCuuOpt->GetName();

    if (strName.GetLength() < 3) return; 

    if (strName == L"opt_RunOnce")
    {
        Get_GSet()->SetSyncType(1);
        if(pSPan) pSPan->SetVisible(false);
    }else  if (strName == L"opt_SyncByTime")
    {
        if(pSPan) pSPan->SetVisible(true);
        Get_GSet()->SetSyncType(2); 
    }else  if (strName == L"opt_LogInfo")  Get_GSet()->SetDebugLevel(LOG_INFOS);  
    else  if (strName == L"opt_LogErr")    Get_GSet()->SetDebugLevel(LOG_ERROR); 
    else  if (strName == L"opt_LogNone")   Get_GSet()->SetDebugLevel(LOG_NONE); 

}

void MainDlg::LogMsg(UString &strLog)
{
    LogMsg(strLog.GetData());
}

void MainDlg::LogMsg(LPCTSTR szLog)
{
    if(szLog && pet_Logs)
    {
        if (pet_Logs->GetLineCount() > 100)
        {
            UString strLogs = pet_Logs->GetText();
            int nFind = strLogs.ReverseFind(L'\r\n');
            strLogs = strLogs.Mid( nFind+1);
            strLogs.Append(L"\r\n");
            strLogs.Append(szLog);
            pet_Logs->SetText(strLogs.GetData());
        }else {
            pet_Logs->AppendText(szLog); 
        }
        pet_Logs->PageDown();
    }
}
