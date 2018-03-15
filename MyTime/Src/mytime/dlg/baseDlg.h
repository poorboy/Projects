#pragma once

using namespace DuiLib;


#define FUN_NAME    __FUNCTION__

class BaseDlg : public WindowImplBase
{
public:
	BaseDlg();
	~BaseDlg(); 
	virtual LPCTSTR  GetWindowClassName() const;
	UINT     GetClassStyle() const { return CS_DBLCLKS; }//CS_IME | CS_VREDRAW | CS_HREDRAW CS_IME | CS_VREDRAW | CS_HREDRAW;
	virtual CDuiString  GetSkinFile() = 0;
	virtual CDuiString GetSkinFolder() { return L"Skin"; }
	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;

	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);	
	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnHotKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
public :
	bool bIsEscHide;
	bool bIsHandSysMsg;

	static bool skinTypeZip;
	static bool isInitSkinType;

};

