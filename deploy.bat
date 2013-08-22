REM Copies files necessary for deployment. Used by Qt Creator
REM Usage: deploy.bat

xcopy /Y /I /E qml .\debug\qml
xcopy /Y /I /E qml .\release\qml
