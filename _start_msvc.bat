@set MSVC=msvc2019
@set MSVC_ARCH=x86

@if exist "%~dp0\setup_msvc.bat" @call "%~dp0\setup_msvc.bat"
@if exist "%~dp0\set_sln.bat"    @call "%~dp0\set_sln.bat"

@call "%~dp0\.bat\_call_cmake_msvc_impl.bat" %MSVC% %MSVC_ARCH% GENERATE && @start "" ".out\%MSVC%\%MSVC_ARCH%\%SLN%.sln"
