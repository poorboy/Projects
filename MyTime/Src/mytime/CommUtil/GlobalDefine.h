/********************************************************************
文件名:		GlobalDefine.h 
创建者:		poorboy
创建时间:	2018/3/15 13:51 
网   址 :   http://www.caipiao365.vip/
修改时间:
电子邮件:   canye2008@gmail.com
文件说明:	全局设置配置文件  
*********************************************************************/

#ifndef GlobalDefine_h__
#define GlobalDefine_h__


#define  INIT_WIN_SOCKET    1

#define  INIT_WIN_SERVICE   1

#define SRC_URL   L"https://gitee.com/zuzong/Projects/tree/master/MyTime/Src/mytime"
#define HELP_URL  L"http://caipiao365.vip/article/MyTime"


//服务相关
#define SERVICE_DISP_NAME       L"时间同步小助手"
#define SERVICE_NAME            L"MyTimer_Service"
#define SERVICE_DISP            L"时间同步小助手,根据设置自动同步时间.";


//GUI界面相关
#define  APP_NAME               L"MyTimerServiceGUI"
#define  MAIN_GUI_TITLE         L"时间同步助手服务配置"
#define  MAIN_GUI_CLASS_NAME    L"TimeAssistantGUI"



//更新相关
#define  NEED_START_CKH_NEW     FALSE 
#define  APP_EXE_NAME           L"MyTime.exe"
#define  APP_UP_NAME            L"MyTime_u.exe"
#define  APP_UP_BAK_NAME        L"MyTime_bak.exe"
#define  APP_DOWN_NAME          L"MyTime.zip"

#define  VER_URL                L"https://gitee.com/zuzong/Projects/raw/master/MyTime/exe/ver.xml"
#define  NEW_VER_DOWN_URL       L"https://gitee.com/zuzong/Projects/raw/master/MyTime/exe/MyTime.zip"





#define  MSG_OK(H,  A, B)               MessageBox(H, A, B, MB_OK) 
#define  MSG_BOX(H, A, B,Y)             MessageBox(H, A, B, Y)


#define  MSGOK(A, B)                    MSG_OK(NULL,  A, B) 
#define  MSGBOX(A,B,Y)                  MSG_BOX(NULL, A, B, Y)


#define  MSGOK_N(X)                     MSG_OK(NULL,  X,  MAIN_GUI_TITLE) 
#define  MSGBOX_N(X,Y)                  MSG_BOX(NULL, X,  MAIN_GUI_TITLE, Y)

#define  MSGOK_GUI(A)                   MSG_OK(GetHWND(),  A, MAIN_GUI_TITLE) 
#define  MSGBOX_GUI(A,Y)                MSG_BOX(GetHWND(), A, MAIN_GUI_TITLE, Y)

#define  NOTE_TEXT L"\
┌─────────────────────────────程序的缘由─────────────────────────────────────────────┐\r\n\
├───────────────────────────────────────────────────────────────────────────────┤\r\n\
│家里有两台老电脑,cmos电池没有电了,每次开机后时间恢复成了出厂时间.然后用win的时间同步服务一直更新不成功.折腾了好长时间(不知道为什么),估计是人长的丑的缘故吧^_^,│\r\n\
│有知道原因的麻烦告诉我.然后在网上找了一 些时间同步的工具,发现不是有广告就是要手动去更新时间,然后就想这与其乱费长时间去试一个个的软件,还不如自己花占时间写一个.│\r\n\
│                                                     就这样[时间同步小助手]就出来.                                                                            │\r\n\
└───────────────────────────────────────────────────────────────────────────────┘\r\n\
┌─────────────────────────────快速参考──────────────────────────────────────────────┐\r\n\
├───────────────────────────────────────────────────────────────────────────────┤\r\n\
│*打开本程序,先在 时间服务器 上选择可以同步的服务器 选择后要是同步成功了会在下面的网络时间上看到时间,下面的设置先要确认选择好可用的时间同步服务器,推荐用阿里的 │\r\n\
│*修改配置后会[自动保存].如已经安装了服务请先[关闭服务]在[启动服务]重新加载配置文件,网络环境复杂多样,要是更新失败,请多试几个,或者自己网上搜索在添加.           │\r\n\
│A.手动更新:    点界面右下方的  同步时间 按钮                                                                                                                  │\r\n\
│B.自动更新(注册成后台服务)                                                                                                                                    │\r\n\
│   1.点下面的 [开机同步]==>[安装服务]==>[启动服务] 就OK了 这种适合电脑的CMOS电池没有电但是系统时钟是正确的电脑                                                │\r\n\
│   2.点下面的 [定时同步]==>[输入间隔时间](1到60)==>[安装服务]==>[启动服务] 就OK了  这种适合电脑的CMOS电池没有电或者是系统时钟不正确的电脑                     │\r\n\
└───────────────────────────────────────────────────────────────────────────────┘\r\n\
┌──────────────────────────────主界面说明────────────────────────────────────────────┐\r\n\
├──────┬───────────────┬──────┬──────────────┬────┬───────────────────┬─────────┤\r\n\
│   安装服务 │ 把本程序注册成windows服务程序│  删除服务  │  从windows服务中删除本程序 │启动服务│ 安装成功后,从windows服务中启动本程序 │                  │\r\n\
├──────┼───────────────┼──────┼──────────────┼────┼───────────────────┼─────────┤\r\n\
│   关闭服务 │  从windows服务中停止本程序   │  说    明  │ 打开本界面                 │        │                                      │                  │\r\n\
├──────┼───────────────┼──────┼──────────────┼────┼───────────────────┼─────────┤\r\n\
│ 时间服务器 │  选择同步的时间服务器        │  删   除   │ 删除当前选择的服务器       │        │                                      │                  │\r\n\
├──────┼───────────────┼──────┼──────────────┼────┼───────────────────┼─────────┤\r\n\
│ 服务器名称 │  添加时间同步服务器的名称    │ 服务器地址 │添加时间同步服务器的地址    │ 添  加 │ 添加一个时间同步服务器               │                  │\r\n\
├──────┼───────────────┼──────┼──────────────┼────┼───────────────────┼─────────┤\r\n\
│时间同步方式│设置后台服务程序时间同步的方式│ 开机同步   │开机同步成功后停止后台服务  │定时同步│ 后台每隔多少分钟同步一次时间         │说明:间隔1到60分钟│\r\n\
├──────┼───────────────┼──────┴──────────────┴────┴───────────────────┴─────────┤\r\n\
│同步时间    │手动更新网络时间到本地系统    │ *日志说明:日志记录方式选择了 [记录信息和错误] 或 [只记录错误]  会在软件的当前目录下的 data\Log 目录下产生日志   │\r\n\
├──────┼───────┬───────┴──────────────┬─────┬────────────────────┬─────┬────────┤\r\n\
│日志记录方式│记录信息和错误│记录程序运行生成的调试信息和错误信息        │只记录错误│只记录程序运行产生的错误信息            │不记录日志│ 不记录任何日志 │\r\n\
└──────┴───────┴──────────────────────┴─────┴────────────────────┴─────┴────────┘\r\n\
┌─────────────────────────────详细帮助──────────────────────────────────────────────┐\r\n\
│详细帮助请见:   http://caipiao365.vip/article/MyTime                                                                                                          │\r\n\
└───────────────────────────────────────────────────────────────────────────────┘\r\n"

const UINT WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated")); 


#endif // GlobalDefine_h__