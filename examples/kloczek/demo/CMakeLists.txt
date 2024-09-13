project(demo CXX)

set(DEMO_SOURCES 
ascii.cc
calc.cc
calendar.cc
fileview.cc
gadgets.cc
mousedlg.cc
puzzle.cc
)

# https://cmake.org/cmake/help/latest/command/add_custom_target.html
# https://cmake.org/cmake/help/latest/policy/CMP0058.html#policy:CMP0058
# https://cmake.org/cmake/help/latest/guide/tutorial/Adding%20a%20Custom%20Command%20and%20Generated%20File.html
# https://crascit.com/2017/04/18/generated-sources-in-cmake-builds/
add_custom_target(demohelp
    #BYPRODUCTS byproduct.txt
    COMMAND rm -f ${CMAKE_CURRENT_BINARY_DIR}/demohelp.h32
    COMMAND rm -f ${CMAKE_CURRENT_BINARY_DIR}/demohelp.h
    COMMAND tvhc ${CMAKE_CURRENT_SOURCE_DIR}/demohelp.txt ${CMAKE_CURRENT_BINARY_DIR}/demohelp.h32 ${CMAKE_CURRENT_BINARY_DIR}/demohelp.h
    # OUTPUT demohelp.h           # Treated as relative to CMAKE_CURRENT_BINARY_DIR
    #BYPRODUCTS demohelp.h32 demohelp.h
    DEPENDS demohelp.txt # Treated as relative to CMAKE_CURRENT_SOURCE_DIR
)

# TVHC <Help text>[.txt] [<Help file>[.h32] [<Symbol file>[.h]]

set(TARGET tvdemo)
add_executable(${TARGET} tvdemo1.cc tvdemo2.cc tvdemo3.cc ${DEMO_SOURCES} )
# ${CMAKE_CURRENT_BINARY_DIR}/demohelp.h
target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
add_dependencies(${TARGET} demohelp)
# fw_add_target_options(${TARGET} "UNICODE" "CONSOLE" "BIGOBJ"
#                      )
target_link_libraries(${TARGET} PRIVATE "${COMMON_LIBS}")

