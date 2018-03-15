#include "stdafx.h"
#include "../../stdafx.h"
#include "UpSoft.h"


bool UpSoftUtil::MakeVerInfo()
{
	string strTime = __DATE__;
	strTime += " ";
	strTime += __TIME__;

	CTimeEx tTime = CTimeEx();
	tTime.GetBldTime(strTime);

	CDuiString strTmp;
	CXMLMarkup XmlPrase;
	XmlPrase.SetDoc(_T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"));
	XmlPrase.AddElem(_T("Updater"));
	XmlPrase.IntoElem();
	XmlPrase.AddElem(_T("LastVersion"));
	strTmp.Format(L"%s (build%s)", _T("0.001"), tTime.GetDate().c_str());
	XmlPrase.AddAttrib(_T("version"), strTmp.GetData());
	XmlPrase.AddAttrib(_T("VerNumber"), tTime.GetTime());
	XmlPrase.AddAttrib(_T("fileName"),  APP_DOWN_NAME);
	XmlPrase.AddAttrib(_T("NOTE"), _T("   "));

	strTmp.Format(L"%sver.xml", ::GetAppDir());

	return XmlPrase.Save(strTmp.GetData());
}


bool UpSoftUtil::ChkNewVer(UString &strNotify)
{
	string strTime = __DATE__;
	strTime += " ";
	strTime += __TIME__;
	CTimeEx tBldTime = CTimeEx();
	tBldTime.GetBldTime(strTime);
	wstring strUrl = VER_URL;
	string strData = GetNetData(strUrl);
	wstring strDoc = C2W(strData);
	CTimeEx tLastUpTime;

	CXMLMarkup xmlParser;
	xmlParser.SetDoc(strDoc);
	xmlParser.ResetMainPos();
	xmlParser.FindElem(L"Updater");
	xmlParser.IntoElem();
	while (xmlParser.FindElem(L"LastVersion"));
	{
		CDuiString strVer = xmlParser.GetAttrib(L"VerNumber").c_str();
		tLastUpTime = CTimeEx(_tstoi64(strVer));
		strNotify = xmlParser.GetAttrib(L"NOTE").c_str();
	}
	if (tLastUpTime > tBldTime) return true;
	return false;
}

bool UpSoftUtil::DownAndRun()
{
	wstring _File1 = ::GetAppDir() + APP_EXE_NAME;
	wstring _File2 = ::GetAppDir() + APP_UP_NAME;
	wstring _File3 = ::GetAppDir() + APP_UP_BAK_NAME;
	DWORD lstErr = ERROR_SUCCESS;

	do
	{
		if (::IsFileExists(_File3.c_str())) ::DeleteFile(_File3.c_str());

		if (!DownNewVer())
		{
			lstErr = ERROR_INVALID_HANDLE;
			break;
		}

		CloseHandle(NULL);
		if (!::MoveFile(_File1.c_str(), _File3.c_str()))
		{
			lstErr = GetLastError();
			break;
		}

		if (!::MoveFile(_File2.c_str(), _File1.c_str()))
		{
			lstErr = GetLastError();
			break;
		}

		ShellExecute(NULL, L"open", _File1.c_str(), NULL, NULL, SW_SHOW);

	} while (0);

	if (lstErr != ERROR_SUCCESS)
	{
		MessageBox(NULL, L"自动更新失败,请手动下载更新", L"更新", MB_OK);
		ShellExecute(NULL, L"open",NEW_VER_DOWN_URL, NULL, NULL, SW_SHOW);

		wstring strExe = L"";
		if (IsFileExists(_File1.c_str()))  strExe = _File1;
		else if (IsFileExists(_File2.c_str())) strExe = _File2;
		else if (IsFileExists(_File3.c_str())) strExe = _File3;
		ShellExecute(NULL, L"open", strExe.c_str(), NULL, NULL, SW_SHOW);
	}

	return lstErr == ERROR_SUCCESS;
}

bool  UpSoftUtil::DownNewVer()
{
	wstring _url = NEW_VER_DOWN_URL;
	wstring _File = ::GetAppDir()  + APP_DOWN_NAME;
	wstring _File2 = ::GetAppDir() + APP_UP_NAME;

	LPBYTE pData = NULL;
	DWORD dwSize = 0;
	bool bRet = false;

	if (::IsFileExists(_File.c_str())) ::DeleteFile(_File.c_str());
	if (::IsFileExists(_File2.c_str())) ::DeleteFile(_File2.c_str());

	do
	{
		if (!GetNetFile(_url, _File)) break;
		ZipUtil zip(_File.c_str());

		if (!zip.IsValidZip()) break;
		bRet = zip.ExtractItem(APP_EXE_NAME, _File2.c_str());
	} while (0);
	return bRet;
}