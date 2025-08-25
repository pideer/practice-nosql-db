# CMakeList.txt : CMake project for SQL movie client, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)
set(CMAKE_CXX_STANDARD 23)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("client")
# Add source to this project's executable.
add_executable (${PROJECT_NAME} "main.cpp")

# point to folder where directory is
if(WIN32)
	set(GLIB "S:/Projects/!SDK")
	set(PLAT win)
else()
	set(GLIB "/mnt/ShaDr/Projects/!SDK")
	set(PLAT unix)
endif()

set(PostgreSQL_DIR ${GLIB}/pgsql)
find_package(libpqxx REQUIRED CONFIG PATHS ${GLIB}/libpqxx/install/${PLAT})
target_link_libraries(${PROJECT_NAME} PRIVATE libpqxx::pqxx)

# TODO: Add tests and install targets if needed.
