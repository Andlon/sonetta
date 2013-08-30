REM Copies files necessary for deployment. Used by Qt Creator
REM Usage: deploy.bat

xcopy /Y /I /E assets .\debug\.
xcopy /Y /I /E assets .\release\.
