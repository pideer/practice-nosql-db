# CMakeList.txt : CMake project for single movie database, include source and define
# project specific logic here.
#
# point to folder where directory is


cmake_minimum_required (VERSION 3.8)
project("movie database")

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Add source to this project's executable.
add_executable (CMakeTarget "single movie database.cpp" "single movie database.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeTarget PROPERTY CXX_STANDARD 20)
endif()

# point to folder where directory is
if(WIN32)
  set(GLIB "S:/Projects/!SDK")
else()
  set(GLIB "/mnt/ShaDr/Projects/!SDK")
endif()
include_directories("${GLIB}/nlohmann")

# TODO: Add tests and install targets if needed.
