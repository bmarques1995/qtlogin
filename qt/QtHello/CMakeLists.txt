# CMakeList.txt: projeto de CMake para QtSample, incluir origem e definir
# lógica específica a um projeto aqui.
#

# Adicione a origem ao executável deste projeto.
cmake_minimum_required(VERSION 3.22)

function(gen_uih UIC_EXECUTABLE INPUT_UI)

	foreach(src ${INPUT_UI})
		get_filename_component(FILE_NAME_WE ${src} NAME_WE)
		get_filename_component(FILE_DIR ${src} DIRECTORY)
		message(WARNING ${src})
		message(WARNING ${FILE_DIR})
		message(WARNING ${FILE_NAME_WE})
		
		set(GENERATED_UI_H "${FILE_DIR}/${FILE_NAME_WE}.qtui.h")

		execute_process(
			COMMAND ${UIC_EXECUTABLE} -o ${GENERATED_UI_H} ${src}
		)
	endforeach()

endfunction()

set(TARGET_NAME qtsample)

trace_dependency(NAME Qt6 COMPONENTS Core Widgets Gui Network INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/qtbase.py" PYTHON_NAME_SUFFIX ${PYTHON_NAME_SUFFIX} LIMIT_SEARCH_PATHS)
trace_dependency(NAME Qt6 COMPONENTS Svg INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/qtsvg.py" PYTHON_NAME_SUFFIX ${PYTHON_NAME_SUFFIX} LIMIT_SEARCH_PATHS)

if (CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" OR CMAKE_SYSTEM_NAME STREQUAL "Linux")
	trace_dependency(NAME Qt6 COMPONENTS WaylandClient INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/qtwayland.py" PYTHON_NAME_SUFFIX ${PYTHON_NAME_SUFFIX} LIMIT_SEARCH_PATHS)
endif()

if(WIN32)
	if(EXECUTE_PROTOCOL_REGISTRATION)
		execute_process(COMMAND python${PYTHON_NAME_SUFFIX} -File "${PROJECT_SOURCE_DIR}/installers/register_keys.py" ${CMAKE_INSTALL_PREFIX})
	endif()
endif()

qt_standard_project_setup()

file(GLOB_RECURSE SAMPLE_HDRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.hpp")
file(GLOB_RECURSE SAMPLE_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")
file(GLOB_RECURSE SAMPLE_UIS ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.ui")

#get_target_property(UIC_EXECUTABLE Qt6::uic IMPORTED_LOCATION)
#gen_uih(${UIC_EXECUTABLE} ${SAMPLE_UIS})

if(CMAKE_BUILD_TYPE STREQUAL "Release" OR BUILD_AS_WINDOWED_APP)
	set(WINDOWED_ARG WIN32;MACOSX_BUNDLE)
	#set(WINDOWED_ARG)
endif()

qt_add_executable(${TARGET_NAME} ${WINDOWED_ARG} ${SAMPLE_HDRS} ${SAMPLE_SRCS} ${SAMPLE_UIS})

target_link_libraries(${TARGET_NAME} PUBLIC Qt6::Core Qt6::Widgets Qt6::Svg Qt6::Gui Qt6::Network)
target_include_directories(${TARGET_NAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/Editor>
	$<INSTALL_INTERFACE:./>
	PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/pch>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

if(WIN32)
	target_compile_definitions(${TARGET_NAME} PUBLIC APP_USES_WINDOWS WIN32_LEAN_AND_MEAN)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_compile_definitions(${TARGET_NAME} PUBLIC APP_USES_LINUX)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release" OR BUILD_AS_WINDOWED_APP)
	if(WIN32)
		target_compile_definitions(${TARGET_NAME} PUBLIC SAMPLE_WINDOWED)
	endif()
endif()

set_cxx_project_standards(${TARGET_NAME} 20 FALSE)

# TODO: Adicione testes e instale destinos, se necessário.
