project(tvguid CXX)

set(TVGUID_SOURCES 
background.cc
listbox.cc
load.cc
nomenus.cc
splash.cc
tvedit.cc
tvgui.h
)



set(TARGET tvlife)
add_executable(${TARGET} ${TARGET}.cc  ${TVGUID_SOURCES} )
target_link_libraries(${TARGET} PRIVATE "${COMMON_LIBS}")


set(TARGET validator)
add_executable(${TARGET} ${TARGET}.cc  ${TVGUID_SOURCES} )
target_link_libraries(${TARGET} PRIVATE "${COMMON_LIBS}")


set(TARGET tvguid01)
add_executable(${TARGET} ${TARGET}.cc  ${TVGUID_SOURCES} )
target_link_libraries(${TARGET} PRIVATE "${COMMON_LIBS}")


set(TARGET tvguid02)
add_executable(${TARGET} ${TARGET}.cc  ${TVGUID_SOURCES} )
target_link_libraries(${TARGET} PRIVATE "${COMMON_LIBS}")


set(TARGET tvguid03)
add_executable(${TARGET} ${TARGET}.cc  ${TVGUID_SOURCES} )
target_link_libraries(${TARGET} PRIVATE "${COMMON_LIBS}")


set(TARGET tvguid04)
add_executable(${TARGET} ${TARGET}.cc  ${TVGUID_SOURCES} )
target_link_libraries(${TARGET} PRIVATE "${COMMON_LIBS}")


set(TARGET tvguid05)
add_executable(${TARGET} ${TARGET}.cc  ${TVGUID_SOURCES} )
target_link_libraries(${TARGET} PRIVATE "${COMMON_LIBS}")


set(TARGET tvguid06)
add_executable(${TARGET} ${TARGET}.cc  ${TVGUID_SOURCES} )
target_link_libraries(${TARGET} PRIVATE "${COMMON_LIBS}")


set(TARGET tvguid07)
add_executable(${TARGET} ${TARGET}.cc  ${TVGUID_SOURCES} )
target_link_libraries(${TARGET} PRIVATE "${COMMON_LIBS}")


set(TARGET tvguid08)
add_executable(${TARGET} ${TARGET}.cc  ${TVGUID_SOURCES} )
target_link_libraries(${TARGET} PRIVATE "${COMMON_LIBS}")


set(TARGET tvguid09)
add_executable(${TARGET} ${TARGET}.cc  ${TVGUID_SOURCES} )
target_link_libraries(${TARGET} PRIVATE "${COMMON_LIBS}")


set(TARGET tvguid10)
add_executable(${TARGET} ${TARGET}.cc  ${TVGUID_SOURCES} )
target_link_libraries(${TARGET} PRIVATE "${COMMON_LIBS}")


set(TARGET tvguid11)
add_executable(${TARGET} ${TARGET}.cc  ${TVGUID_SOURCES} )
target_link_libraries(${TARGET} PRIVATE "${COMMON_LIBS}")


set(TARGET tvguid12)
add_executable(${TARGET} ${TARGET}.cc  ${TVGUID_SOURCES} )
target_link_libraries(${TARGET} PRIVATE "${COMMON_LIBS}")


set(TARGET tvguid13)
add_executable(${TARGET} ${TARGET}.cc  ${TVGUID_SOURCES} )
target_link_libraries(${TARGET} PRIVATE "${COMMON_LIBS}")


set(TARGET tvguid14)
add_executable(${TARGET} ${TARGET}.cc  ${TVGUID_SOURCES} )
target_link_libraries(${TARGET} PRIVATE "${COMMON_LIBS}")


set(TARGET tvguid15)
add_executable(${TARGET} ${TARGET}.cc  ${TVGUID_SOURCES} )
target_link_libraries(${TARGET} PRIVATE "${COMMON_LIBS}")


set(TARGET tvguid16)
add_executable(${TARGET} ${TARGET}.cc  ${TVGUID_SOURCES} )
target_link_libraries(${TARGET} PRIVATE "${COMMON_LIBS}")




