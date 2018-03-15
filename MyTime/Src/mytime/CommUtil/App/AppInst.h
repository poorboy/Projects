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
#error "����GlobalDefine.h�ļ����INIT_WIN_SOCKET�Ķ���"
#endif 

    AutoMutex _aMutex; 
    HINSTANCE hInst;
};