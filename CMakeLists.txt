include(FetchContent)

#.\vcpkg.exe install sqlitecpp[sqlite]:x64-windows zstd:x64-windows sqlite3:x64-windows
#cmake -DVCPKG_TARGET_TRIPLET=x64-windows -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake -B build

if(NOT DEFINED ENV{GITHUB_ACTIONS})
    if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        message( FATAL_ERROR "On Windows, please install vcpkg and set CMAKE_TOOLCHAIN_FILE as per https://github.com/microsoft/vcpkg/blob/master/docs/users/integration.md#cmake-integration" )
    endif()

    if(NOT DEFINED VCPKG_TARGET_TRIPLET)
        message( FATAL_ERROR "On Windows, when using vcpkg, set -DVCPKG_TARGET_TRIPLET=x86-windows or -DVCPKG_TARGET_TRIPLET=x64-windows" )
    endif()
endif()

project(sqlite_nested_vfs VERSION 1.0
DESCRIPTION "SQLite VFS extension storing database pages in...a SQLite database"
LANGUAGES C CXX)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(SQLiteCpp CONFIG REQUIRED)
find_package(zstd CONFIG REQUIRED)



FetchContent_Declare(
    concurrentqueue
    GIT_REPOSITORY  https://github.com/cameron314/readerwriterqueue.git
    GIT_TAG         v1.0.6
)
FetchContent_MakeAvailable(concurrentqueue)
include_directories(${concurrentqueue_SOURCE_DIR})


FetchContent_Declare(
    sqlite_web_vfs
    GIT_REPOSITORY  https://github.com/mlin/sqlite_web_vfs.git
    GIT_TAG         9ae962d
)

FetchContent_MakeAvailable(sqlite_web_vfs)
include_directories(${sqlite_web_vfs_SOURCE_DIR}/src)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#instead of __declspec, you could just get CMake to export everything but this makes the export list very messy
#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
add_library(nested_vfs SHARED src/nested_vfs.cc src/SQLiteNestedVFS.h)
SET_TARGET_PROPERTIES(nested_vfs PROPERTIES PREFIX "")
target_link_libraries(nested_vfs PRIVATE SQLiteCpp)

add_library(zstd_vfs SHARED src/zstd_vfs.cc src/SQLiteNestedVFS.h src/zstd_vfs.h)
SET_TARGET_PROPERTIES(zstd_vfs PROPERTIES PREFIX "")
target_link_libraries(zstd_vfs PRIVATE SQLiteCpp zstd::libzstd_shared)