@call :DO_GIT_JOB %1 %~dp0\arm_none_eabi_crt_stub
@call :DO_GIT_JOB %1 %~dp0\marty_decimal
@call :DO_GIT_JOB %1 %~dp0\sfmt
@call :DO_GIT_JOB %1 %~dp0\tvision
@call :DO_GIT_JOB %1 %~dp0\umba

@exit /B

:DO_GIT_JOB
@echo %1'ing %2
@cd %2
@git %1
@cd ..
@echo.
@exit /B

