#include "StdAfx.h"
#include "../../stdafx.h"

CCfgSet g_Set;


CCfgSet::CCfgSet()
{  
    m_nSyncType = 1;
    m_DebugLevel = LOG_INFOS; 
    m_cfgPath = GetAppDir() + L"set.xml";
    InitFromCfg();  
}

void CCfgSet::InitFromCfg(void)
{

    CXMLMarkup _xml;

    if (!_xml.Load(m_cfgPath)) 
    {
        LOG_ERR(L"加载配置文件失败:[%s]", _xml.GetError().c_str());     
        return;
    }

    /*
    <sets>
    <NtpS cuuId="2">
    <item ip="210.72.145.44" name="(国家授时中心服务器IP地址)"/>
    </NtpS>
    <!--
    更新方式
    1 开机就同步 同步成功后停止服务     
    2 开始服务后每隔30分钟同步一次  
    -->
    <syncType type ="1"/>
    </sets>            
    */

    if (!_xml.FindElem(L"sets")) return;

    _xml.IntoElem();


    if (_xml.FindElem(L"NtpS"))
    {
        m_nCuuSID = _tstoi(_xml.GetAttrib(L"cuuId").c_str());
        _xml.IntoElem(); 
        while(_xml.FindElem(L"item"))
        { 
            NtpItem _item;

            _item.strIp = _xml.GetAttrib(L"ip");
            _item.strIp2 = _xml.GetAttrib(L"ip2");
            _item.strName = _xml.GetAttrib(L"name");
            m_item.push_back(_item); 
        };
    }
    _xml.OutOfElem();

    //<syncType type ="1"/> 
    if (_xml.FindElem(L"syncType"))
    {
        m_nSyncType = _tstoi(_xml.GetAttrib(L"type").c_str());
        m_nSTime = _tstoi(_xml.GetAttrib(L"STime").c_str());
    }

    if (m_nSTime < 1 || m_nSTime > 61) m_nSTime = 5;


    if (_xml.FindElem(L"LogLevel"))
    {
        m_DebugLevel = _tstoi(_xml.GetAttrib(L"level").c_str()); 
    } 

    if (m_nSTime < LOG_INFOS || m_nSTime > LOG_NONE) m_nSTime = LOG_INFOS;
}  



void CCfgSet::Save2Cfg(void)
{ 
    CXMLMarkup _xml;
    _xml.SetDoc(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
    _xml.AddElem(L"sets");

    _xml.IntoElem();

    _xml.AddElem(L"NtpS"); 
    _xml.AddAttrib(L"cuuId", m_nCuuSID);  
    _xml.IntoElem();

    for (int i=0; i < m_item.size(); i++)
    {
        NtpItem &_item = m_item[i];
        _xml.AddElem(L"item"); 
        _xml.AddAttrib(L"name", _item.strName);  
        _xml.AddAttrib(L"ip",  _item.strIp);  
        _xml.AddAttrib(L"ip2", _item.strIp2);  
    }  

    _xml.OutOfElem();

    _xml.AddElem(L"syncType"); 
    _xml.AddAttrib(L"type", m_nSyncType);  
    _xml.AddAttrib(L"STime", m_nSTime);

    _xml.AddElem(L"LogLevel"); 
    _xml.AddAttrib(L"level", m_DebugLevel);
 
    _xml.Save(m_cfgPath);
} 

int CCfgSet::GetSyncType()
{
    return m_nSyncType;
}

void CCfgSet::SetSyncType(int _type)
{
    if (m_nSyncType == _type) return;

    m_nSyncType = _type;
    Save2Cfg();
}

int CCfgSet::GetCuuSIDType()
{
    return m_nCuuSID;
}

void CCfgSet::SetCuuSIDType(int _cuuID)
{
    if (m_nCuuSID == _cuuID) return;
    m_nCuuSID = _cuuID;
    Save2Cfg();
}

vector<NtpItem>& CCfgSet::GetSList()
{
    return m_item;
}


bool CCfgSet::IsItemInVector(NtpItem &item, wstring &Erro)
{
    for (int i=0; i < m_item.size(); i++)
    {
        NtpItem &_item = m_item[i];
        if (item.strName == _item.strName)
        {
            Erro = L"改名称已经存在,请换一个名称!";
            return true;
        }

        if (item.strIp == _item.strIp)
        {
            Erro = L"改服务器地址已经存在,请检查您的地址!";
            return true;
        }

        //if (item.strIp2 == _item.strIp2)
        //{
        //    Erro = L"改服务器地址已经存在,请检查您的地址!";
        //    return true;
        //} 
    }
    return false;
}

bool CCfgSet::AddSItem(NtpItem &item, wstring &Error)
{
    if(IsItemInVector(item,Error))
    {
        return false;
    }

    m_item.push_back(item);
    return true;
}

bool CCfgSet::DelSItem(int nid, wstring &Error)
{
    vector<NtpItem> tmp;
    bool bRet = false;
    for (int i=0; i < m_item.size(); i++)
    {
        if (nid == i)
        {
            bRet = true;
            continue;
        }
        tmp.push_back(m_item[i]);
    }
    m_item = tmp;
    Save2Cfg();
    return bRet;
}

bool CCfgSet::GetItemSIP(int nid, wstring &sIP)
{

    if (nid < 0 || nid >= m_item.size()) 
    {
        sIP = L"错误的ID";
        LOG_ERR(L"%s:===>%d", sIP.c_str(), nid);
        return false;
    }

    sIP = m_item[nid].strIp;
    return  true;
}

int CCfgSet::GetSTime()
{
    return m_nSTime;

}

void CCfgSet::SetSTime(int _STime)
{
    if (m_nSTime == _STime) return;

    m_nSTime = _STime;
    Save2Cfg();
}

const wchar_t * CCfgSet::GetItemSName(int nid)
{
    if (nid < 0 || nid >= m_item.size()) 
    {
        LOG_ERR(L"错误的ID:===>%d",  nid);
        return  L"错误的ID"; 
    }

    return m_item[nid].strName.c_str();
}

int CCfgSet::GetDebugLevel()
{
    return m_DebugLevel;

}

void CCfgSet::SetDebugLevel(int _DebugLevel)
{
    if (m_DebugLevel  == _DebugLevel) return;
    m_DebugLevel = _DebugLevel;
    Save2Cfg();
}



CCfgSet * Get_GSet()
{
    return &g_Set;
}