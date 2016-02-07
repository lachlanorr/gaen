@ECHO OFF
SETLOCAL

CD /d %~dp0

bison -v compose.y
flex --debug compose.l
