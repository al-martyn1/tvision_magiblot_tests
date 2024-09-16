@echo off
setlocal disabledelayedexpansion
set "prev="
for /f "delims=" %%F in ('sort %~dp0\tv_all_uses.h.in') do (
  set "curr=%%F"
  setlocal enabledelayedexpansion
  if "!prev!" neq "!curr!" echo !curr!
  endlocal
  set "prev=%%F"
)