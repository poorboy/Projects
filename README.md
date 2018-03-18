### 时间同步助手说明

界面效果

![主界面](http://www.caipiao365.vip/static/upload/20180315/1521091368276453896.png "主界面效果图")

![说明界面](http://www.caipiao365.vip/static/upload/20180315/1521121593099587496.png "说明界面效果图")


设置说明: 设置文件在软件目录下的 set.xml
    cuuId 为当前选择的时间同步服务器ID 就是下面第几个item里的配置 (从零开始)
      item 时间服务器列表项  
      name 时间服务器名称  
      ip   时间服务器地址 
      ip2  保留

   后台服务时间同步方式 
          type    1 开机就同步 同步成功后停止服务 
                  2 开始服务后每隔STime分钟同步一次 
          STime   间隔的时间

  日志记录方式
           level 2    记录程序运行生成的调试信息和错误信息
                 3    只记录程序运行产生的错误信息
                 4    不记录任何日志
				 
**注 在界面上或者手动修改配置文件后要手动停止服务在启动服务

详细帮助: [http://www.caipiao365.vip/article/MyTime](http://www.caipiao365.vip/article/MyTime)

程序缘由: [http://caipiao365.vip/article/12](http://caipiao365.vip/article/12)

程序源码:    [https://gitee.com/zuzong/Projects/tree/master/MyTime](https://gitee.com/zuzong/Projects/tree/master/MyTime)

程序下载:    [https://gitee.com/zuzong/Projects/raw/master/MyTime/exe/MyTime.zip](https://gitee.com/zuzong/Projects/raw/master/MyTime/exe/MyTime.zip)