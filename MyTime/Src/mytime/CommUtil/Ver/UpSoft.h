/********************************************************************
文件名:		UpSoft.h 
创建者:		poorboy
创建时间:	2018/3/15 19:51 
网   址 :   http://www.caipiao365.vip/
修改时间:
电子邮件:   canye2008@gmail.com
文件说明:	软件更新类  
*********************************************************************/

#pragma once 
#include "../MyString/MyString.h"

class UpSoftUtil
{
public:
	bool MakeVerInfo();
	bool ChkNewVer(UString &strNotify);
	bool DownAndRun();
	bool DownNewVer();
};