cmake_minimum_required(VERSION 3.23)
project(PointEngine) 
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

add_executable(example1)
target_include_directories(example1 PRIVATE include)
target_sources(example1
	PRIVATE
		src/examples/example1.c
)

add_executable(example2)
target_include_directories(example2 PRIVATE include)
target_sources(example2
	PRIVATE
		src/examples/example2.c
)

add_library(pe)
target_sources(pe 
	PRIVATE
        src/PointEngine.c
	
	PUBLIC
		FILE_SET HEADERS
		BASE_DIRS
			include
		FILES
            include/PointEngine.h
)

target_link_libraries(example1 PRIVATE pe SDL3::SDL3 m)
target_link_libraries(example2 PRIVATE pe SDL3::SDL3 m)
