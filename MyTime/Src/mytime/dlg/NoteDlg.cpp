/********************************************************************
文件名:		NoteDlg.cpp 
创建者:		poorboy
创建时间:	2018/3/11 9:05 
网   址 :   http://www.caipiao365.vip/
修改时间:
文件说明:	  
*********************************************************************/


#include "StdAfx.h"
#include "../StdAfx.h"
#include "NoteDlg.h" 

NoteDlg::NoteDlg(HWND _hwnd)
{
    bIsEscHide = false;
    bIsHandSysMsg = false;

    pet_Logs = NULL; 
    m_hWndP = _hwnd;
}

NoteDlg::~NoteDlg(void)
{


} 


void NoteDlg::OnPrepare() 
{  
    pet_Logs =  static_cast<CRichEditUI *> (m_PaintManager.FindControl(L"et_Logs"));
    if (pet_Logs)
    {
        pet_Logs->SetReadOnly(true);
        //pet_Logs->SetEnabled(false);
        pet_Logs->SetText(NOTE_TEXT);
    }
}

void NoteDlg::Notify(TNotifyUI& msg)
{
    if( msg.sType == _T("windowinit")) OnPrepare();
    else if( msg.sType == _T("click") ) 
    { 
        CDuiString strName = msg.pSender->GetName();
        if(strName== L"closebtn" ) Close(); 
    }  
} 

bool NoteDlg::CreateWin()
{
    if(NULL == CreateDuiWindow(m_hWndP, GetWinTitle(), WS_OVERLAPPEDWINDOW))  return false;
    CenterWindow(); 
    //this->ShowWindow(true); 
    //SetWinVisb(); 
    return true;
}

LPWSTR NoteDlg::GetWinTitle()
{
    return L"时间同步助手说明";
}