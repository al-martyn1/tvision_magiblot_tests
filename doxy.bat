@if exist setup_doxygen.bat @call setup_doxygen.bat

@rem https://stackoverflow.com/questions/15472320/how-to-set-an-input-directory-for-doxygen
@rem https://stackoverflow.com/questions/24295820/doxygen-isnt-generating-documentation-for-source-files
@rem CLANG_ASSISTED_PARSING
copy /A /Y %~dp0\.config\doxygen_common.cfg+%~dp0\.config\doxygen_html.cfg   %~dp0\.config\doxygen.cfg
doxygen %~dp0\.config\doxygen.cfg > %~dp0\doxygen_html.log 2>&1

@rem copy /Y _doc_utils\navtree\*.* doxy\html\
@rem copy /Y _doc_utils\search\*.* doxy\html\search\

@rem copy /A /Y doxygen_common.cfg+doxygen_xml.cfg    doxygen.cfg
@rem doxygen doxygen.cfg > doxygen_xml.log 2>&1
