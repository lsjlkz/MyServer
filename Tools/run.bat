@ECHO OFF
chcp 65001

cd ..
@REM copy Third\\CLib\\libmysql.dll cmake-build-release\\libmysql.dll
cd cmake-build-release

@REM TODO 通过消息通知进程保存数据并关闭
taskkill /f /im MyServer.exe
start MyServer.exe 3
start MyServer.exe 2