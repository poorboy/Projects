/********************************************************************
文件名:		NoteDlg.h 
创建者:		poorboy
创建时间:	2018/3/11 9:04 
网   址 :   http://www.caipiao365.vip/
修改时间:
文件说明:	  
*********************************************************************/

#ifndef NoteDlg_h__
#define NoteDlg_h__
#pragma once 

#include "baseDlg.h"

class NoteDlg: public BaseDlg
{
public:
    NoteDlg( HWND _hwnd);
    ~NoteDlg(void);

    virtual CDuiString  GetSkinFile() { return _T("NoteDlg.xml"); }
    virtual LPCTSTR  GetWindowClassName() const{return L"TimeAssistantNoteGUI";}
    void    Notify(TNotifyUI& msg);
    LPWSTR GetWinTitle();

    bool CreateWin();

protected: 
    void OnPrepare();   

protected:  
    CRichEditUI * pet_Logs;  
    HWND m_hWndP;
};


#endif //  
