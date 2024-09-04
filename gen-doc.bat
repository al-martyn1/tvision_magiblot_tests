@set EXCLUDE_DIRS=--batch-exclude-dir=_libs,libs,_lib,lib,tests,test,rc,_generators,_distr_conf,src,.msvc2019,boost,icons,doxy
@set EXCLUDE=%EXCLUDE_DIRS%
@set COMMON=--verbose --overwrite --copy-images
@set OPTIONS= %TEMP_OPTS% %COMMON% %VARS% %EXCLUDE% %GRAPHVIZ_OPTIONS% --batch-scan-recurse=.
umba-md-pp %OPTIONS%


