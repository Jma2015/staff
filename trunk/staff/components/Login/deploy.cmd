set projectname=login

set deploydir=%cd%\..\..\deploy\win_%PROCESSOR_ARCHITECTURE%
set componentdir=%deploydir%\components\%projectname%

if not EXIST %componentdir% mkdir %componentdir%

xcopy /Y /S src\*.wsdl %componentdir%\
xcopy /Y /S src\*.wsdl %STAFF_HOME%\components\%projectname%\

if exist Debug (
  xcopy /Y /S debug\*.dll %componentdir%\
  xcopy /Y /S debug\*.dll %STAFF_HOME%\components\%projectname%\
) else (
  if exist Release (
    xcopy /Y /S Release\*.dll %componentdir%\
    xcopy /Y /S Release\*.dll %STAFF_HOME%\components\%projectname%\
  )
)
