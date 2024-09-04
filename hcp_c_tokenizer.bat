@set OPTS=-U -I%~dp0_libs -o%~dp0_libs\tvision\include\tvision\umba\internal
@rem umba-hcp %OPTS% %~dp0_libs\umba\c_char_class.h
umba-hcp %OPTS% %~dp0_libs\umba\c_tokenizer.h


