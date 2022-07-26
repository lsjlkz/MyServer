@ECHO OFF
chcp 65001
CD ..
COPY Third\\CLib\\libmysql.dll cmake-build-debug\\libmysql.dll
CD cmake-build-debug

@REM TODO 通过消息通知进程保存数据并关闭
taskkill /f /im MyServer.exe
start MyServer.exe 1
start MyServer.exe 2
start MyServer.exe 3