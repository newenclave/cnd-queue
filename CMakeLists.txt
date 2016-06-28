if(WIN32)
    set(
        CMAKE_USER_MAKE_RULES_OVERRIDE
        "${CMAKE_CURRENT_SOURCE_DIR}/msvc_override.cmake"
    )
endif(WIN32)

project(cnd-queue)

cmake_minimum_required(VERSION 2.8)

set(SRC_LIST)

aux_source_directory( . SRC_LIST )
aux_source_directory( ./tinycthread/source SRC_LIST )
include_directories( ./tinycthread/source )

if(NOT WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-long-long -pedantic")
	add_definitions(-DFUNC_MACRO_NAME=__func__)
else(NOT WIN32)
	# Force to always compile with W4
	add_definitions(-DFUNC_MACRO_NAME=__FUNCTION__)
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
	string( REGEX REPLACE "/W[0-4]" "/W4"
		      CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else( )
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif( )
	
	if(CMAKE_C_FLAGS MATCHES "/W[0-4]")
		string( REGEX REPLACE "/W[0-4]" "/W4"
		        CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
	else( )
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
	endif( )

endif(NOT WIN32)

add_executable(${PROJECT_NAME} ${SRC_LIST})

if(NOT WIN32)
        target_link_libraries( ${PROJECT_NAME} "pthread" )
endif(NOT WIN32)
