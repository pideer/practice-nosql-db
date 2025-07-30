cmake_minimum_required(VERSION 3.10.0)
project(IPC-json_test-server VERSION 0.1.0 LANGUAGES C CXX)

add_executable(IPC-json_test-server main.cpp "tunnel.cpp")
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
add_subdirectory(
${GLIB}/Catch2
${GLIB}/Catch2/build
)
list(APPEND CMAKE_MODULE_PATH ${GLIB}/Catch2/extras)

add_executable(tests test.cpp "tunnel.cpp")
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests PRIVATE nlohmann_json::nlohmann_json)

include(CTest)
include(Catch)
catch_discover_tests(tests)