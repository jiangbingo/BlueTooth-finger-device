@echo off
setlocal enabledelayedexpansion
title �������ļ��ϲ�
::mode con cols=60lines=17
:start
cls
echo   ��ӭʹ��supersea��д�Ķ������ļ��ϲ�������
echo.
echo   ��˳���϶�Ҫ�ϲ����ļ�����������,����ʾ����
echo.
echo   ���̲���������󽫷���Ҫ����������
echo.
echo ���Ŀ�겢���س�:
set /a n=0
:input
set inputstr=""
set /a "n+=1"
set/p "inputstr= > "
if /i "!inputstr!"=="list" set /a n-=1&goto :list
if /i "!inputstr!"=="OK" set /a n-=1&goto :setoutput
if !inputstr!=="" set /a n-=1&echo ������,��ȷ��&goto :input
if not exist !inputstr! set /a n-=1&echo �����������,����������&goto :input
set "fn%n%=%inputstr%"
:listback
cls
echo  "list"�鿴�����б�;��OK�������;��������
goto :input
:setoutput
if %n%==0 goto :start
cls
echo �϶�����ļ��е��˴���,��س�Ĭ�����������,���ֶ�����·��
:setoutputagain
set /p "outputpath= > "
if "!outputpath!"=="" (
for /f "tokens=2*" %%a in (
'reg query ^"HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\User Shell Folders^" /v Desktop^|findstr /i "Desktop"') do (set outputpath="%%b")
)
if not exist !outputpath! echo ·����������,����������&goto :setoutputagain
cls
echo ���������ļ����Ƽ���׺
:setoutputfilename
set /p "outputfilename= > "
if "!outputfilename!"=="" echo ����ļ�����Ϊ��,����������&goto :setoutputfilename
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
set/p="��������������²�������"
goto :start
:list
if %n%==0 goto :start
cls
for /l %%i in (1,1,%n%) do (echo !fn%%i!&&echo.)
set/p="������������ء���"
goto :listback