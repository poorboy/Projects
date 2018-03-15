#include "stdafx.h"
#include "baseDlg.h"
#include <typeinfo>


bool BaseDlg::skinTypeZip = false;
bool BaseDlg::isInitSkinType = false;

BaseDlg::BaseDlg(void)
{
	bIsEscHide = false;
	bIsHandSysMsg = false;

	if(!isInitSkinType)
	{ 
		wstring strDir;
		strDir = ::GetAppDir();
		strDir.append(L"Skin\\");
		skinTypeZip = !IsDir(strDir);
		isInitSkinType = true;
	}
} 

BaseDlg::~BaseDlg(void)
{

} 

UILIB_RESOURCETYPE BaseDlg::GetResourceType() const
{
	if(skinTypeZip) return UILIB_ZIPRESOURCE;
	else return UILIB_FILE; 
}

LPCTSTR BaseDlg::GetResourceID() const
{
	if(skinTypeZip) return MAKEINTRESOURCE(IDR_ZIPRES1);
	else return L""; 
} 

//UILIB_RESOURCETYPE BaseDlg::GetResourceType() const
//{
//#ifdef XML_FROM_RES
//	return UILIB_ZIPRESOURCE;
//#else
//	return UILIB_FILE;
//#endif 
//}
//
//LPCTSTR BaseDlg::GetResourceID() const
//{
//#ifdef XML_FROM_RES
//	return MAKEINTRESOURCE(IDR_ZIPRES1);
//#else
//	return  L"";
//#endif 
//} 


/* 
string strName = FUN_NAME;
int nFind = strName.find("::");
if (nFind != string::npos) strName.substr(0, nFind);
wstring Name = C2W(strName);
CDuiString strInfo;
strInfo.Format(L"\r\nBaseDlg::GetWinClassName%s\r\n", Name.c_str());
OutputDebugString(strInfo.GetData());  
*/ 

LPCTSTR BaseDlg::GetWindowClassName() const
{
	//string temp(typeid(*this).name());
	//CDuiString strClass = C2W(temp).c_str();
	//strClass.Replace(L"class", L"");
	//strClass.Replace(L" ", L"");
	//strClass.Append(L"Class");
	//return strClass.GetData();

	return L"BaseDlg";
}

LRESULT BaseDlg::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	if (uMsg != WM_SYSCOMMAND) return 0L;

	if (wParam == SC_CLOSE)
	{
		bHandled = TRUE;
		return 1;
	}

	if (
		(wParam == SC_MAXIMIZE)
		||
		(wParam == SC_MINIMIZE)
		||
		(wParam == SC_RESTORE)
		||
		(wParam == 61490)
		||
		(wParam == 61730)  //SC_MOVE
		)
	{
		if(bIsHandSysMsg) return 0;
	}

	return WindowImplBase::OnSysCommand(uMsg, wParam, lParam, bHandled);
}

LRESULT  BaseDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0L;
}

LRESULT BaseDlg::OnHotKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0L;
}

//LRESULT BaseDlg::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{ 
//	if (uMsg == WM_KEYDOWN && wParam == VK_ESCAPE && bIsEscHide) this->ShowWindow(false);
//
//	return 0L;
//}

LRESULT BaseDlg::ResponseDefaultKeyEvent(WPARAM wParam)
{
	//if (wParam == VK_RETURN)
	//{
	//	return FALSE;
	//}
	//else if (wParam == VK_ESCAPE)
	//{
	//	Close();
	//	return TRUE;
	//}

	return FALSE;
}

LRESULT BaseDlg::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	if(uMsg == WM_TIMER) OnTimer(uMsg, wParam, lParam, bHandled);
	else if (WM_HOTKEY == uMsg) OnHotKey(uMsg, wParam, lParam, bHandled);
	return 0;
}


//LRESULT MainDlg::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//	if (uMsg == WM_TIMER)	OnTimer(uMsg, wParam, lParam, bHandled);
//	else if (WM_HOTKEY == uMsg) OnHotKey(uMsg, wParam, lParam, bHandled);
//	else if (WM_KILLFOCUS == uMsg && pStockCont) pStockCont->HideCaret();
//	else if (uMsg == WM_TASKBARCREATED)
//	{
//		if (bIsNeedAdd2Tary) AddIcon2Tary();
//		bHandled = TRUE;
//	}
//	else if (uMsg == WM_USER)
//	{
//		OnTrayMsg(wParam, lParam);
//		bHandled = TRUE;
//	}
//	return 1L;
//}
