@call :DO_GIT_JOB %1 %~dp0\tvision

@exit /B

:DO_GIT_JOB
@echo %1'ing %2
@cd %2
@git %1
@cd ..
@echo.
@exit /B

