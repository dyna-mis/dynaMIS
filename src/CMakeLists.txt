if (WIN32)
	add_compile_definitions("$<$<CONFIG:Debug>:DYNAMIS_PRINT>")
	add_compile_definitions("$<$<CONFIG:Debug>:DYNAMIS_DEBUG>")
else (WIN32)
	add_compile_definitions("$<$<CONFIG:Debug>:DYNAMIS_DEBUG>")
endif()
# CGAL
	if (WIN32)
			#set(CGAL_DIR /usr/include/CGAL-5.0.3)
			#set(Qt5_DIR /home/guangping/opt/QT5.9.5/5.9.5/gcc_64/lib/cmake/Qt5)
	else (WIN32)
			set(CGAL_DIR /usr/include/CGAL-5.2.1)
	endif()
	
	





if(NOT POLICY CMP0070 AND POLICY CMP0053)
  # Only set CMP0053 to OLD with CMake<3.10, otherwise there is a warning.
  cmake_policy(SET CMP0053 OLD)
endif()

if(POLICY CMP0071)
  cmake_policy(SET CMP0071 NEW)
endif()

# list source files here

file (GLOB_RECURSE sources *.h *.cpp *.hpp *.tpp)
include_directories(solvers)
include_directories(solvers/exact_solver)
include_directories(solvers/mis_solver)
include_directories(solvers/line_solver)
include_directories(solvers/grid_solver)
include_directories(simulation)

include_directories(gpl_data_structure)
include_directories(tools)
add_executable(dynaMIS ${sources})
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
   set(CMAKE_INSTALL_RPATH='$ORIGIN')
endif ()


option(CXX "enable C++ compilation" ON)
if(CXX)
    enable_language(CXX)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
message (STATUS "CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")


if (MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
else ()
	#set(CMAKE_CXX_FLAGS "-Wextra")
	set(CMAKE_CXX_FLAGS_DEBUG "-g")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
	target_compile_features(dynaMIS PUBLIC cxx_std_17) 
	target_link_libraries(dynaMIS PUBLIC stdc++fs)
	
endif()


if (WIN32)
	find_package(unofficial-cairo CONFIG REQUIRED)
	set (cairo_lib unofficial::cairo::cairo)
	target_link_libraries(dynaMIS PRIVATE ${cairo_lib})
endif()



	#settings
if (WIN32)
	find_package(CGAL COMPONENTS Qt5)
	if(CGAL_Qt5_FOUND)
		add_definitions(-DCGAL_USE_BASIC_VIEWER -DQT_NO_KEYWORDS)
	endif()
	if ( CGAL_FOUND )
	target_link_libraries(dynaMIS PRIVATE ${GMP_LIBRARIES})
	#target_link_libraries(dynaMIS PRIVATE ${GMPXX_LIBRARIES})
	if(CGAL_Qt5_FOUND)
		target_link_libraries(dynaMIS PUBLIC CGAL::CGAL_Qt5 ${GMP_LIBRARIES})
	endif()
	else()
		message(STATUS "This program requires the CGAL library, and will not be compiled.")
	endif()
else (WIN32)
	find_package(CGAL REQUIRED)
	if ( CGAL_FOUND )
	target_link_libraries(dynaMIS PRIVATE CGAL::CGAL ${GMP_LIBRARIES})
	#target_link_libraries(dynaMIS PRIVATE ${GMPXX_LIBRARIES})
	else()
		message(STATUS "This program requires the CGAL library, and will not be compiled.")
	endif()
endif()





# Visual Studio compiler with static runtime libraries
if(MSVC AND MT)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
	
endif()



# if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    # include(FeatureSummary)
    # feature_summary(WHAT ALL)
# endif()




set_property(TARGET dynaMIS PROPERTY CXX_STANDARD 17)
set_property(TARGET dynaMIS PROPERTY CXX_STANDARD_REQUIRED ON)


