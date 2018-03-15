#ifndef __CCFGSET__H__
#define __CCFGSET__H__
#pragma once 

#include "../XML/Markup.h"
#include "../StdAfx.h"

class NtpItem
{
public:
    wstring strIp;
    wstring strIp2;
    wstring strName;
};

class CCfgSet
{
public:
    CCfgSet();
    void InitFromCfg(void);
    void Save2Cfg(void);

    int  GetSyncType();
    void SetSyncType(int _type);

    int  GetSTime();
    void SetSTime(int _STime);

    int  GetCuuSIDType();
    void SetCuuSIDType(int _cuuID);

    vector<NtpItem>&GetSList();

    bool IsItemInVector(NtpItem &item, wstring &Erro);
    bool AddSItem(NtpItem &item, wstring &Error);
    bool DelSItem(int nid, wstring &Error);
    bool GetItemSIP(int nid, wstring &sIP);
    const wchar_t * GetItemSName(int nid);

    int  GetDebugLevel();
    void SetDebugLevel(int _DebugLevel);

public:  
    vector<NtpItem> m_item;
    int             m_nSyncType;  
    int             m_nCuuSID;
    int             m_nSTime;
    wstring         m_cfgPath;
    int             m_DebugLevel;

}; 

CCfgSet * Get_GSet();


#endif //__CCFGSET__H__