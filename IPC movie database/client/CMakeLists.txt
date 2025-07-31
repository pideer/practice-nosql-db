# CMakeList.txt : CMake project for client, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("client")

# Add source to this project's executable.
add_executable (client "main.cpp" "tunnel.cpp")
include_directories(${CMAKE_SOURCE_DIR}/include)
# point to folder where directory is
if(WIN32)
	set(GLIB "S:/Projects/!SDK")
else()
	set(GLIB "/mnt/ShaDr/Projects/!SDK")
endif()

# Import JSON library
set(JSON_BuildTests OFF CACHE INTERNAL "") # Disable tests if not needed
add_subdirectory(
	${GLIB}/json
	${GLIB}/json/build
)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

# Import Catch2
#add_subdirectory(
#${GLIB}/Catch2
#${GLIB}/Catch2/build
#)
#list(APPEND CMAKE_MODULE_PATH ${GLIB}/Catch2/extras)

#if (CMAKE_VERSION VERSION_GREATER 3.12)
#  set_property(TARGET client PROPERTY CXX_STANDARD 20)
#endif()

# TODO: Add tests and install targets if needed.
#target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
#target_link_libraries(tests PRIVATE nlohmann_json::nlohmann_json)

#include(CTest)
#include(Catch)
#catch_discover_tests(tests)
