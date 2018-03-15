/********************************************************************
文件名:		CTimeEx.h
创建者:		poorboy
创建时间:	[8/25/2014 14:45]
修改时间:
文件说明:
*********************************************************************/
#pragma once

class CTimeEx
{
public:
	CTimeEx(LPCTSTR szTime);
	CTimeEx(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, int nDST = -1);
	CTimeEx(const SYSTEMTIME& st, int nDST = -1);
	CTimeEx(const FILETIME& ft, int nDST = -1);
	CTimeEx(WORD wDosDate, WORD wDosTime, int nDST = -1);
	CTimeEx(__time64_t time);
	CTimeEx()
	{
		CTimeEx((__time64_t)0);
	}

    bool ToSystemTime(SYSTEMTIME& st);

	void GetBldTime(string &strTime);

	static CTimeEx GetCurrentTime() throw();
	//获取该月的天数
	static UINT GetMonthOfDays(UINT year, UINT month);
	//获取该月的天数
	UINT GetMonthOfDays();

	__time64_t GetTime() const throw();

	int GetYear() const throw();
	int GetMonth() const throw();
	int GetDay() const throw();
	int GetHour() const throw();
	int GetMinute() const throw();
	int GetSecond() const throw();
	int GetDayOfWeek() const throw();

	void AddDay(__time64_t nDay);
	void AddHour(__time64_t nHour);
	void AddMinute(__time64_t nMin);
	void AddSecond(__time64_t nSec);


	void AddMonth(int nMon);

	CTimeEx& operator=(__time64_t time);
	CTimeEx& operator+=(DWORD nSec);
	CTimeEx& operator-=(DWORD nSec);

	__int64 operator-(CTimeEx time)const;
	CTimeEx operator-(DWORD nSec)const;
	CTimeEx operator+(DWORD nSec)const;

	bool operator==(CTimeEx time)const;
	bool operator==(__int64 time)const;
	bool operator!=(CTimeEx time)const;
	bool operator<(CTimeEx time)const;
	bool operator>(CTimeEx time)const;
	bool operator<=(CTimeEx time)const;
	bool operator>=(CTimeEx time)const;

	wstring ToString();
	wstring GetDate();
	wstring GetTimeStr();
	wstring ToStringEX();
	wstring ToStringEX1();
	wstring ToString1();
	wstring GetGMTString();

private:
	void MakeTimeByVct(vector<int> &vctNumber);
	bool WISdigit(const wchar_t * wstr);
	bool ISdigit(const char * str);
	int GetMonthByStr(const char * str);
	struct tm* GetLocalTm(struct tm* ptm) const;

protected:
	__time64_t m_time;
};
