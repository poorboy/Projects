#pragma once 

class MainDlg;
#include "../AutoUtiles/AutoUtiles.h"

class AppInst
{

public:
    AppInst();
    ~AppInst();
    void DoRun(HINSTANCE _hInst, LPTSTR lpCmdLine);

protected:
    void DoMakeVerInfo();
    bool DoChkVer();
    bool ChkIsRun(); 
    void ClsHandle();

private:  
    MainDlg* m_MianFrame;
    wstring strAppName; 

#ifdef INIT_WIN_SOCKET
    SCKInit _SInit;
#else
#error "请在GlobalDefine.h文件里打开INIT_WIN_SOCKET的定义"
#endif 

    AutoMutex _aMutex; 
    HINSTANCE hInst;
};