/********************************************************************
�ļ���:		UpSoft.h 
������:		poorboy
����ʱ��:	2018/3/15 19:51 
��   ַ :   http://www.caipiao365.vip/
�޸�ʱ��:
�����ʼ�:   canye2008@gmail.com
�ļ�˵��:	���������  
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