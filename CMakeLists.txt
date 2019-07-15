cmake_minimum_required (VERSION 3.8)

include(${CMAKE_SOURCE_DIR}/pd.build/pd.cmake)

project(pdshmem)

set_pd_sources(${CMAKE_SOURCE_DIR}/pure-data/src)
set_pd_external_path(${CMAKE_SOURCE_DIR}/bin/)

add_pd_external(shmem shmem ${CMAKE_SOURCE_DIR}/src/shmem.c)


