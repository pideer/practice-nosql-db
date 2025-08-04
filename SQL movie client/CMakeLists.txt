# CMakeList.txt : CMake project for SQL movie client, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)
set(CMAKE_CXX_STANDARD 23)

cmake_policy(SET CMP0074 NEW)  # Enable *_ROOT variables
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
else()
	set(GLIB "/mnt/ShaDr/Projects/!SDK")
endif()

if(WIN32)
	set(PostgreSQL_ROOT ${GLIB}/pgsql)
	set(libpqxx_ROOT ${GLIB}/libpqxx/install/win)
	configure_file(${PostgreSQL_ROOT}/bin/libpq.dll ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
else()
	set(libpqxx_ROOT ${GLIB}/libpqxx/install/unix)
endif()
find_package(PostgreSQL REQUIRED)
find_package(libpqxx REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE libpqxx::pqxx)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
