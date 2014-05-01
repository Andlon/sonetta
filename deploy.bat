REM Copies files necessary for deployment. Used by Qt Creator
REM Usage: deploy.bat

set SCRIPTDIR=%~dp0
set COREDIR=%SCRIPTDIR%core\

xcopy /Y /I /E %COREDIR%assets %SCRIPTDIR%debug\.
xcopy /Y /I /E %COREDIR%assets %SCRIPTDIR%release\.
