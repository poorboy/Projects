#include "stdafx.h"
//#include "../../stdafx.h"
#include "../GlobalDefine.h"
#include "../../dlg/MainDlg.h" 
#include "../../TimeService/TimeService.h"  
 

AppInst::AppInst() 
{
    hInst = NULL;	 
    m_MianFrame = NULL;
    strAppName = APP_NAME;
}

AppInst::~AppInst()
{
    if (m_MianFrame) delete m_MianFrame;
    // 销毁资源管理器
    CResourceManager::GetInstance()->Release();
    // OLE
    OleUninitialize();
    // COM
    ::CoUninitialize();
}


void AppInst::DoMakeVerInfo()
{
    UpSoftUtil uUtil;
    uUtil.MakeVerInfo();
}

bool AppInst::DoChkVer()
{

    bool bRet = false; 
    UpSoftUtil uUtil; 
    UString strNote;
    if (uUtil.ChkNewVer(strNote))
    {
        strNote.Format(L"有新的版本,是否更新?\r\n本次主要更改了:%s", strNote.GetData());
        if (IDYES == MessageBox(NULL, strNote.GetData(), L"更新", MB_YESNO))
            bRet = true;
        uUtil.DownAndRun();
    }
    return bRet;
}

bool AppInst::ChkIsRun()
{
    wstring MutexStr = L"Global\\";
    MutexStr += strAppName.c_str(); 


    if( _aMutex.CreateMutex(MutexStr) == false)
    {
        HWND hwnd = FindWindow(m_MianFrame->GetWindowClassName(), NULL);
        if (hwnd) 
        {
            if(::IsIconic(hwnd)) ::SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0); 
            ::ShowWindow(hwnd, SW_SHOW);
            ::SetForegroundWindow(hwnd); 
        }
        return true;
    } 
    return false;
}


void AppInst::DoRun(HINSTANCE _hInst, LPTSTR lpCmdLine)
{
    hInst = _hInst; 

    // COM
    HRESULT Hr = ::CoInitialize(NULL);
    if( FAILED(Hr) ) return ;
    // OLE
    HRESULT hRes = ::OleInitialize(NULL);

    GetGService()->Execute();
    //GetGService()->bIsRun//

    // 初始化UI管理器
    CPaintManagerUI::SetInstance(_hInst);

    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));
    // 初始化资源
    //InitResource();

    do
    {
        CDuiString strCmd = lpCmdLine;
        strCmd.MakeUpper();
        if (strCmd.Find(L"MAKEVERINFO") >= 0)
        {
            DoMakeVerInfo();
            break;
        }

        if(NEED_START_CKH_NEW && DoChkVer()) break; 

        m_MianFrame = new MainDlg;
        if (ChkIsRun()) break;

        if (NULL == m_MianFrame)  break; 


        if(!m_MianFrame->CreateWin())
        {
            m_MianFrame->Close();
            m_MianFrame = NULL;
            break;
        }

        CPaintManagerUI::MessageLoop(); 

    } while (0);
} 