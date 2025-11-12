cmake_minimum_required(VERSION 4.0)
project(
    "OTBV" 
    VERSION 1.0.0
    DESCRIPTION "Compression of binary volumes using octrees"
    LANGUAGES CXX
)
include(GNUInstallDirs)
include(CTest)

# add_library(${PROJECT_NAME} SHARED 
#     src/conversion.cpp
#     src/io.cpp
# )

# set_target_properties(${PROJECT_NAME} PROPERTIES
#     VERSION ${PROJECT_VERSION}
#     SOVERSION ${PROJECT_VERSION_MAJOR}
#     PUBLIC_HEADER include/otbv.h)
# configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)
# target_include_directories(${PROJECT_NAME} PRIVATE .)
# install(TARGETS ${PROJECT_NAME}
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc
#     DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

add_library(${PROJECT_NAME} STATIC 
    src/conversion.cpp
    src/io.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    PUBLIC_HEADER include/otbv.h)
configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)
target_include_directories(${PROJECT_NAME} PRIVATE .)
install(TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

add_executable(testing tests/tests.cpp)
target_link_libraries(testing PRIVATE ${PROJECT_NAME})
enable_testing()
add_test(NAME testing COMMAND $<TARGET_FILE:testing>)