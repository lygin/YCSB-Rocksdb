cmake_minimum_required(VERSION 3.14)
project(ycsb)

set(CMAKE_CXX_STANDARD 17)
option(LINK_FASTER "Support Faster" ON)
if (LINK_FASTER)
    add_subdirectory(third/FASTER/cc/src)
endif ()

add_executable(ycsb "")
aux_source_directory(core CORE)
aux_source_directory(db DB)
aux_source_directory(lib LIB)
target_sources(ycsb PUBLIC
${CORE}
${DB}
${LIB}
"ycsbc.cc"
)

target_include_directories(ycsb PUBLIC .)

target_link_libraries(ycsb tbb rocksdb)

if (LINK_FASTER)
    target_include_directories(ycsb PUBLIC third/FASTER/cc/src)
    target_link_libraries(ycsb faster stdc++fs uuid tbb gcc aio m stdc++ pthread)
endif ()
