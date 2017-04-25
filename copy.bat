xcopy /y	.\Engine\Exports\*.*			.\Reference\Headers\
xcopy /y	.\Engine\Headers\*.*			.\Reference\Headers\

xcopy /y	.\Engine\System\Codes\*.h		.\Reference\Headers\
xcopy /y	.\Engine\System\Bin\*.lib		.\Reference\Libraries\
xcopy /y	.\Engine\System\Bin\*.dll		.\Client\Bin\
xcopy /y	.\Engine\System\Bin\*.dll		.\Tool\Bin\

xcopy /y	.\Engine\Utility\Codes\*.h		.\Reference\Headers\
xcopy /y	.\Engine\Utility\Codes\*.hpp	.\Reference\Headers\
xcopy /y	.\Engine\Utility\Bin\*.lib		.\Reference\Libraries\
xcopy /y	.\Engine\Utility\Bin\*.dll		.\Client\Bin\
xcopy /y	.\Engine\Utility\Bin\*.dll		.\Tool\Bin\