@set UMBA_MDPP_EXTRA_OPTIONS="--batch-page-index-file=%~dp0doc\pages_index.md"
@call "%~dp0\_precommit.bat" >"%~dp0\_precommit.log" 2>&1