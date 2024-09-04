@set ACC=al-martyn1
@set BASE=github.com


@if "%1"=="SSH"  goto SETUP_SSH
@if "%1"=="GIT"  goto SETUP_SSH
@if "%1"=="HTTP" goto SETUP_HTTP



@rem Default is HTTPS mode

:SETUP_HTTPS
@rem По https 
set PREFIX=https://%BASE%/%ACC%
goto DO_CLONE

:SETUP_HTTP
@rem По https 
set PREFIX=http://%BASE%/%ACC%
goto DO_CLONE

:SETUP_SSH
set PREFIX=git@%BASE%:%ACC%

:DO_CLONE
git clone --recurse-submodules %PREFIX%/marty_decimal.git      %~dp0\marty_decimal
git clone --recurse-submodules %PREFIX%/tvision_magiblot.git   %~dp0\tvision
git clone --recurse-submodules %PREFIX%/umba_mm_mod_umba.git   %~dp0\umba

