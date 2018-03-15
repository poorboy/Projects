#include "stdafx.h"
#include "AppBase.h"
#include "AppInst.h"

#include "../MemDump/MiniDumper.h"
CMiniDumper g_miniDumper(true);


int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine); 
	AppInst Inst;
	Inst.DoRun(hInstance, lpCmdLine);
	return 0;
}