/********************************************************************
文件名:		CTimeEx.cpp
创建者:		poorboy
创建时间:	[8/25/2014 14:45]
修改时间:
文件说明:
*********************************************************************/
#include "StdAfx.h"
#include "CTimeEx.h"
#include <time.h>


CTimeEx::CTimeEx(LPCTSTR szTime)
{
    m_time = 0;
    wstring strTime = wstring(szTime);
    int nLen = strTime.length();
    vector<int> vctNber;
    wstring wsTmp = L"";
    wstring wchar;
    for (int i = 0; i < nLen; i++)
    {
        wchar = strTime.substr(i, 1);
        if (WISdigit(wchar.c_str()))
        {
            wsTmp += wchar;
        }
        else
        {
            if (wsTmp.length() > 0)
            {
                int nNubmer = _tstoi(wsTmp.c_str());
                vctNber.push_back(nNubmer);
                wsTmp = L"";
            }
        }
    }

    if (wsTmp.length() > 0)
    {
        int nNubmer = _tstoi(wsTmp.c_str());
        vctNber.push_back(nNubmer);
        wsTmp = L"";
    }

    MakeTimeByVct(vctNber);
}

CTimeEx::CTimeEx(const SYSTEMTIME& sysTime, int nDST)
{
    if (sysTime.wYear < 1900)
    {
        m_time = 0;
    }
    else
    {
        CTimeEx timeT = CTimeEx(
            (int)sysTime.wYear, (int)sysTime.wMonth, (int)sysTime.wDay,
            (int)sysTime.wHour, (int)sysTime.wMinute, (int)sysTime.wSecond,
            nDST);
        *this = timeT;
    }
}

CTimeEx::CTimeEx(const FILETIME& fileTime, int nDST)
{
    FILETIME localTime;
    if (!FileTimeToLocalFileTime(&fileTime, &localTime))
    {
        m_time = 0; return;
    }
    SYSTEMTIME sysTime;
    if (!FileTimeToSystemTime(&localTime, &sysTime))
    {
        m_time = 0; return;
    }

    CTimeEx timeT = CTimeEx(sysTime, nDST);
    *this = timeT;
}

CTimeEx::CTimeEx(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, int nDST)
{
    //( nYear >= 1900 );
    //( nMonth >= 1 && nMonth <= 12 );
    //( nDay >= 1 && nDay <= 31 );
    //( nHour >= 0 && nHour <= 23 );
    //( nMin >= 0 && nMin <= 59 );
    //if(!( nSec >= 0 && nSec <= 59 ))
    struct tm atm;
    atm.tm_sec = nSec;
    atm.tm_min = nMin;
    atm.tm_hour = nHour;
    atm.tm_mday = nDay;
    atm.tm_mon = nMonth - 1;
    atm.tm_year = nYear - 1900;
    atm.tm_isdst = nDST;
    m_time = _mktime64(&atm);
}

CTimeEx::CTimeEx(WORD wDosDate, WORD wDosTime, int nDST)
{
    struct tm atm;
    atm.tm_sec = (wDosTime & ~0xFFE0) << 1;
    atm.tm_min = (wDosTime & ~0xF800) >> 5;
    atm.tm_hour = wDosTime >> 11;

    atm.tm_mday = wDosDate & ~0xFFE0;
    atm.tm_mon = ((wDosDate & ~0xFE00) >> 5) - 1;
    atm.tm_year = (wDosDate >> 9) + 80;
    atm.tm_isdst = nDST;
    m_time = _mktime64(&atm);
    if (m_time == -1) m_time = 0;
}

CTimeEx::CTimeEx(__time64_t time)
:m_time(time)
{

}

void CTimeEx::GetBldTime(string &strTime)
{
    m_time = 0;
    vector<int> vctNber;
    ////01234567890123456789
    ////Apr 11 2017 08:36:37
    string strMon = strTime.substr(0, 3);
    string strDay = strTime.substr(4, 2);
    string strYear = strTime.substr(7, 4);
    string strHour = strTime.substr(12, 2);
    string strMin = strTime.substr(15, 2);
    string strSec = strTime.substr(18, 2);


    struct tm atm;
    ZeroMemory(&atm, sizeof(atm));

    atm.tm_year = atoi(strYear.c_str()) - 1900;
    atm.tm_mon = GetMonthByStr(strMon.c_str());
    atm.tm_mday = atoi(strDay.c_str());
    atm.tm_hour = atoi(strHour.c_str());
    atm.tm_min = atoi(strMin.c_str());
    atm.tm_sec = atoi(strSec.c_str());
    atm.tm_isdst = -1;
    m_time = _mktime64(&atm);
}

CTimeEx CTimeEx::GetCurrentTime() throw()
{
    return(CTimeEx(::_time64(NULL)));
}

//获取该月的天数
UINT CTimeEx::GetMonthOfDays()
{
    return CTimeEx::GetMonthOfDays(GetYear(), GetMonth());
}

//获取该月的天数
UINT CTimeEx::GetMonthOfDays(UINT year, UINT month)
{
    UINT mDay[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };//12个月的天数(平年的情况)
    //闰年时，二月加一天
    if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
    {
        mDay[1] = 29;
    }
    return mDay[month - 1];
}

CTimeEx& CTimeEx::operator=(__time64_t time)
{
    m_time = time;

    return(*this);
}

CTimeEx& CTimeEx::operator+=(DWORD span)
{
    m_time += span;

    return(*this);
}

CTimeEx& CTimeEx::operator-=(DWORD span)
{
    m_time -= span;

    return(*this);
}

__int64 CTimeEx::operator-(CTimeEx time) const
{
    return(m_time - time.m_time);
}

CTimeEx CTimeEx::operator-(DWORD span) const
{
    return(CTimeEx((m_time - span)));
}

CTimeEx CTimeEx::operator+(DWORD span) const
{
    return(CTimeEx((m_time + span)));
}

bool CTimeEx::operator==(__int64 time)const
{

    return(m_time == time);
}

bool CTimeEx::operator==(CTimeEx time) const
{
    return(m_time == time.m_time);
}

bool CTimeEx::operator!=(CTimeEx time) const
{
    return(m_time != time.m_time);
}

bool CTimeEx::operator<(CTimeEx time) const
{
    return(m_time < time.m_time);
}

bool CTimeEx::operator>(CTimeEx time) const
{
    return(m_time > time.m_time);
}

bool CTimeEx::operator<=(CTimeEx time) const
{
    return(m_time <= time.m_time);
}

bool CTimeEx::operator>=(CTimeEx time) const
{
    return(m_time >= time.m_time);
}

__time64_t CTimeEx::GetTime() const throw()
{
    return(m_time);
}

int CTimeEx::GetYear() const
{
    struct tm ttm;
    struct tm * ptm;

    ptm = GetLocalTm(&ttm);
    return ptm ? (ptm->tm_year) + 1900 : 0;
}

int CTimeEx::GetMonth() const
{
    struct tm ttm;
    struct tm * ptm;

    ptm = GetLocalTm(&ttm);
    return ptm ? ptm->tm_mon + 1 : 0;
}

int CTimeEx::GetDay() const
{
    struct tm ttm;
    struct tm * ptm;

    ptm = GetLocalTm(&ttm);
    return ptm ? ptm->tm_mday : 0;
}

int CTimeEx::GetHour() const
{
    struct tm ttm;
    struct tm * ptm;
    ptm = GetLocalTm(&ttm);
    return ptm ? ptm->tm_hour : -1;
}

int CTimeEx::GetMinute() const
{
    struct tm ttm;
    struct tm * ptm;
    ptm = GetLocalTm(&ttm);
    return ptm ? ptm->tm_min : -1;
}

int CTimeEx::GetSecond() const
{
    struct tm ttm;
    struct tm * ptm;

    ptm = GetLocalTm(&ttm);
    return ptm ? ptm->tm_sec : -1;
}

int CTimeEx::GetDayOfWeek() const
{
    struct tm ttm;
    struct tm * ptm;

    ptm = GetLocalTm(&ttm);
    return ptm ? ptm->tm_wday + 1 : 0;
}

void CTimeEx::AddDay(__time64_t nDay)
{
    AddHour(nDay * 24);
}
void CTimeEx::AddHour(__time64_t nHour)
{
    AddMinute(nHour * 60);
}

void CTimeEx::AddMinute(__time64_t nMin)
{
    AddSecond(nMin * 60);
}

void CTimeEx::AddSecond(__time64_t nSec)
{
    m_time += nSec;
}

void CTimeEx::AddMonth(int nMon)
{
    int nDay = GetDay();
    /*
    3.31号+ 2个月是 5.31
    3.31号+ 1个月是 4.30
    3.31号- 1个月是 2.29(28)
    3.31号- 2个月是 1.31
    */

    if (nMon > 0)
    {
        for (int i = 0; i < nMon; i++)
            AddDay(GetMonthOfDays());
    }
    else if (nMon < 0)
    {
        for (int i = 0; i < nMon; i++)
        {
            AddDay(GetMonthOfDays());
        }
    }

}

wstring CTimeEx::ToString()
{
    wstring wRet = L"";
    TCHAR sTmp[MAX_PATH] = { 0 };
    wsprintf(sTmp, L"%d-%02d-%02d", GetYear(), GetMonth(), GetDay());
    wRet = sTmp;
    return wRet;
}

wstring CTimeEx::GetDate()
{
    wstring wRet = L"";
    TCHAR sTmp[MAX_PATH] = { 0 };
    wsprintf(sTmp, L"%02d-%02d", GetMonth(), GetDay());
    wRet = sTmp;
    return wRet;
}

wstring CTimeEx::GetTimeStr()
{
    wstring wRet = L"";
    TCHAR sTmp[MAX_PATH] = { 0 };
    wsprintf(sTmp, L"%02d:%02d:%02d", GetHour(), GetMinute(), GetSecond());
    wRet = sTmp;
    return wRet;
}
wstring CTimeEx::ToStringEX()
{
    wstring wRet = L"";
    TCHAR sTmp[MAX_PATH] = { 0 };
    wsprintf(sTmp, L"%d-%02d-%02d %02d:%02d:%02d", GetYear(), GetMonth(), GetDay(), GetHour(), GetMinute(), GetSecond());
    wRet = sTmp;
    return wRet;
}
wstring CTimeEx::ToStringEX1()
{
    wstring wRet = L"";
    TCHAR sTmp[MAX_PATH] = { 0 };
    wsprintf(sTmp, L"%d-%02d-%02dT%02d:%02d:%02d", GetYear(), GetMonth(), GetDay(), GetHour(), GetMinute(), GetSecond());
    wRet = sTmp;
    return wRet;
}

wstring CTimeEx::ToString1()
{
    wstring wRet = L"";
    TCHAR sTmp[MAX_PATH] = { 0 };
    wsprintf(sTmp, L"%d_%02d_%02d_%02d_%02d_%02d", GetYear(), GetMonth(), GetDay(), GetHour(), GetMinute(), GetSecond());
    wRet = sTmp;
    return wRet;
}


wstring CTimeEx::GetGMTString()
{
    char szBuf[128] = { 0 };
    strftime(szBuf, 127, "%A,%d-%b-%y %H:%M:%S", gmtime(&m_time));
    wstring wRet = C2W(szBuf);
    return wRet;
}


void CTimeEx::MakeTimeByVct(vector<int> &vctNumber)
{
    int nLen = vctNumber.size();
    if (nLen <= 0) return;
    struct tm atm;
    ZeroMemory(&atm, sizeof(atm));
    int j = 0;
    if (nLen > j) atm.tm_year = vctNumber.at(j++) - 1900;
    if (nLen > j) atm.tm_mon = vctNumber.at(j++) - 1;
    if (nLen > j) atm.tm_mday = vctNumber.at(j++);
    if (nLen > j) atm.tm_hour = vctNumber.at(j++);
    if (nLen > j) atm.tm_min = vctNumber.at(j++);
    if (nLen > j) atm.tm_sec = vctNumber.at(j++);
    atm.tm_isdst = -1;
    m_time = _mktime64(&atm);
}

struct tm* CTimeEx::GetLocalTm(struct tm* ptm) const
{
    if (ptm != NULL)
    {
        struct tm ptmTemp;
        errno_t err = _localtime64_s(&ptmTemp, &m_time);

        if (err != 0)
        {
            return NULL;    // indicates that m_time was not initialized!
        }

        *ptm = ptmTemp;
        return ptm;
    }

    return NULL;
}

bool CTimeEx::WISdigit(const wchar_t * wstr)
{
    bool bRet = false;
    if (_tcscmp(wstr, L"0") == 0) bRet = true;
    else if (_tcscmp(wstr, L"1") == 0) bRet = true;
    else if (_tcscmp(wstr, L"2") == 0) bRet = true;
    else if (_tcscmp(wstr, L"3") == 0) bRet = true;
    else if (_tcscmp(wstr, L"4") == 0) bRet = true;
    else if (_tcscmp(wstr, L"5") == 0) bRet = true;
    else if (_tcscmp(wstr, L"6") == 0) bRet = true;
    else if (_tcscmp(wstr, L"7") == 0) bRet = true;
    else if (_tcscmp(wstr, L"8") == 0) bRet = true;
    else if (_tcscmp(wstr, L"9") == 0) bRet = true;
    return bRet;
}


bool CTimeEx::ISdigit(const char * str)
{
    bool bRet = false;
    if (strcmp(str, "0") == 0) bRet = true;
    else if (strcmp(str, "1") == 0) bRet = true;
    else if (strcmp(str, "2") == 0) bRet = true;
    else if (strcmp(str, "3") == 0) bRet = true;
    else if (strcmp(str, "4") == 0) bRet = true;
    else if (strcmp(str, "5") == 0) bRet = true;
    else if (strcmp(str, "6") == 0) bRet = true;
    else if (strcmp(str, "7") == 0) bRet = true;
    else if (strcmp(str, "8") == 0) bRet = true;
    else if (strcmp(str, "9") == 0) bRet = true;
    return bRet;
}

int CTimeEx::GetMonthByStr(const char * str)
{
    //JanFebMarAprMayJunJulAugSepOctNovDec
    static char * szMon[12] = {
        "Jan", "Feb", "Mar", "Apr",
        "May", "Jun", "Jul", "Aug",
        "Sep", "Oct", "Nov", "Dec",
    };

    int  nMonth = 0;
    for (; nMonth < 12; nMonth++)
    {
        if (strcmpi(str, szMon[nMonth]) == 0) break;
    }
    return nMonth;
}

bool CTimeEx::ToSystemTime(SYSTEMTIME& sysTime)
{
    if (m_time < 100) return false; 
    sysTime.wYear = GetYear();
    if (sysTime.wYear < 2018) return false;

    sysTime.wMonth = GetMonth();
    sysTime.wDay = GetDay();
    sysTime.wHour = GetHour();
    sysTime.wMinute = GetMinute();
    sysTime.wSecond = GetSecond(); 
    return true;
}