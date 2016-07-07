@echo off
setlocal enabledelayedexpansion
title 二进制文件合并
::mode con cols=60lines=17
:start
cls
echo   欢迎使用supersea编写的二进制文件合并批处理
echo.
echo   按顺序拖动要合并的文件到批处理窗口,按提示操作
echo.
echo   过程参数输入错误将返回要求重新输入
echo.
echo 添加目标并按回车:
set /a n=0
:input
set inputstr=""
set /a "n+=1"
set/p "inputstr= > "
if /i "!inputstr!"=="list" set /a n-=1&goto :list
if /i "!inputstr!"=="OK" set /a n-=1&goto :setoutput
if !inputstr!=="" set /a n-=1&echo 无输入,请确认&goto :input
if not exist !inputstr! set /a n-=1&echo 过程命令错误,请重新输入&goto :input
set "fn%n%=%inputstr%"
:listback
cls
echo  "list"查看任务列表;或OK结束添加;或继续添加
goto :input
:setoutput
if %n%==0 goto :start
cls
echo 拖动输出文件夹到此窗口,或回车默认输出到桌面,或手动输入路径
:setoutputagain
set /p "outputpath= > "
if "!outputpath!"=="" (
for /f "tokens=2*" %%a in (
'reg query ^"HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\User Shell Folders^" /v Desktop^|findstr /i "Desktop"') do (set outputpath="%%b")
)
if not exist !outputpath! echo 路径输入有误,请重新输入&goto :setoutputagain
cls
echo 输入生成文件名称及后缀
:setoutputfilename
set /p "outputfilename= > "
if "!outputfilename!"=="" echo 输出文件不能为空,请重新输入&goto :setoutputfilename
set inputfiles=%fn1%
set i=1
:content
set /a i+=1
set container=%inputfiles% + !fn%i%!
set inputfiles=%container%
if %i% lss %n% goto :content
cd\
cd %outputpath%
copy /b !inputfiles! = %outputfilename%
echo.
set/p="……任意键返回新操作……"
goto :start
:list
if %n%==0 goto :start
cls
for /l %%i in (1,1,%n%) do (echo !fn%%i!&&echo.)
set/p="……任意键返回……"
goto :listback