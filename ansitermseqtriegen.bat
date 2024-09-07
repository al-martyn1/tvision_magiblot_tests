".out\GCC 13.2 MinGW\win32-x64\Debug\ansitermseqtriegen.exe"  > _libs\tvision\include\tvision\umba\ansiterm\seqtrie.h
".out\GCC 13.2 MinGW\win32-x64\Debug\ansitermseqtriegraphgen.exe"  > _libs\tvision\include\tvision\umba\ansiterm\seqtrie.dot
rem -Gdpi=300 
dot -Tsvg           -o _libs\tvision\include\tvision\umba\ansiterm\seqtrie.svg _libs\tvision\include\tvision\umba\ansiterm\seqtrie.dot
dot -Tpng -Gdpi=300 -o _libs\tvision\include\tvision\umba\ansiterm\seqtrie.png _libs\tvision\include\tvision\umba\ansiterm\seqtrie.dot
