# Basic project setup
cmake_minimum_required(VERSION 3.24..4.03)

if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
endif()

project(
  "TeensyOpt"
  VERSION 0.1
  DESCRIPTION
    "Small mathematical optimization library"
  LANGUAGES CXX)

# For clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include(GNUInstallDirs)

option(TEENSYOPT_INSTALL_LIBRARY
  "Enable installing of the TeensyOpt library into default locations"
  ${PROJECT_IS_TOP_LEVEL}
)

# Add the teensyopt library
add_library(TeensyOpt INTERFACE)
# add alias so the project can be used with add_subdirectory
add_library(TeensyOpt::TeensyOpt ALIAS TeensyOpt)

target_include_directories(TeensyOpt INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_features(TeensyOpt INTERFACE cxx_std_20)

# Add test directory if this is the main project, and
# BUILD_TESTING is True
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  include(CTest)
  add_subdirectory(tests)
endif()

if(TEENSYOPT_INSTALL_LIBRARY)
  # Locations provided by GNUInstallDirs
  install(
    TARGETS TeensyOpt
    EXPORT TeensyOpt_Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(
    "TeensyOptVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
  )

  configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/TeensyOptConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/TeensyOptConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/TeensyOpt/cmake)

  install(
    EXPORT TeensyOpt_Targets
    FILE TeensyOptTargets.cmake
    NAMESPACE TeensyOpt::
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/TeensyOpt/cmake)

  install(FILES "${PROJECT_BINARY_DIR}/TeensyOptConfig.cmake"
                "${PROJECT_BINARY_DIR}/TeensyOptConfigVersion.cmake"
          DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/TeensyOpt/cmake)

  install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/TeensyOpt
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")

include(CPack)
