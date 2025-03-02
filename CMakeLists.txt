cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME ForceField3D)
message("######################################################################")
message("############################ ${PROJECT_NAME} ############################")
message("######################################################################")
message("")
project(${PROJECT_NAME})

if(ANDROID)
	set(MAKE_LIB TRUE)
endif()

### set output directories for all builds (Debug, Release, etc.)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/bin)
endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(COMMON_SRC_DIR ${SRC_DIR}/common)
set(RENDER_SRC_DIR ${COMMON_SRC_DIR}/render)
set(APP_SRC_DIR ${SRC_DIR}/electricity)
set(LODEPNG_SRC_DIR ${SRC_DIR}/3rdparty/lodepng)

### set source files
set(LODEPNG_SRC
	${LODEPNG_SRC_DIR}/lodepng.h
	${LODEPNG_SRC_DIR}/lodepng.cpp
)

set(GLAD_SRC
	${SRC_DIR}/3rdparty/glad/src/glad.c
)

#file(GLOB COMMON_SOURCES ${COMMON_SRC_DIR}/*.cpp ${COMMON_SRC_DIR}/*.h)
set(COMMON_SOURCES 
	${COMMON_SRC_DIR}/Core.hpp
	${COMMON_SRC_DIR}/Error.hpp
	${COMMON_SRC_DIR}/Error.cpp
	${COMMON_SRC_DIR}/Timing.hpp
	${COMMON_SRC_DIR}/Timing.cpp
	${COMMON_SRC_DIR}/Array.hpp
	${COMMON_SRC_DIR}/Vec.hpp
	${COMMON_SRC_DIR}/File.hpp
	${COMMON_SRC_DIR}/File.cpp
	${COMMON_SRC_DIR}/Image.hpp
	${COMMON_SRC_DIR}/Image.cpp
	${COMMON_SRC_DIR}/MathUtils.hpp
	${COMMON_SRC_DIR}/StringUtils.hpp
	${COMMON_SRC_DIR}/StringUtils.cpp
	${COMMON_SRC_DIR}/Matrix.hpp
	${COMMON_SRC_DIR}/TransformationMatrix.hpp
	${COMMON_SRC_DIR}/MeshGeneration.hpp
	${COMMON_SRC_DIR}/MeshGeneration.cpp
	${COMMON_SRC_DIR}/MaterialContainer.hpp
	${COMMON_SRC_DIR}/MaterialContainer.cpp
	${COMMON_SRC_DIR}/Object.hpp
	${COMMON_SRC_DIR}/Object.cpp
	${COMMON_SRC_DIR}/MeshObject.hpp
	${COMMON_SRC_DIR}/MeshObject.cpp
	${COMMON_SRC_DIR}/TickObserver.hpp
	${COMMON_SRC_DIR}/TickObserver.cpp
	${COMMON_SRC_DIR}/input_devices/KeyObserver.hpp
	${COMMON_SRC_DIR}/input_devices/KeyObserver.cpp
	${COMMON_SRC_DIR}/input_devices/MotionEvent.hpp
	${COMMON_SRC_DIR}/input_devices/MotionEvent.cpp
	${COMMON_SRC_DIR}/input_devices/CursorManager.cpp
	${COMMON_SRC_DIR}/input_devices/CursorManager.hpp
	${COMMON_SRC_DIR}/input_devices/Cursor3DObserver.cpp
	${COMMON_SRC_DIR}/input_devices/Cursor3DObserver.hpp

	${RENDER_SRC_DIR}/DrawCommand.hpp
	${RENDER_SRC_DIR}/OpenGLDefs.hpp
	${RENDER_SRC_DIR}/OpenGLContext.hpp
	${RENDER_SRC_DIR}/OpenGLContext.cpp
	${RENDER_SRC_DIR}/DrawingQueue.hpp
	${RENDER_SRC_DIR}/DrawingQueue.cpp
	${RENDER_SRC_DIR}/DrawingQueuePlayback.hpp
	${RENDER_SRC_DIR}/DrawingQueuePlayback.cpp
	${RENDER_SRC_DIR}/ShaderVariable.hpp
	${RENDER_SRC_DIR}/ShaderVariable.cpp
	${RENDER_SRC_DIR}/program/Shader.hpp
	${RENDER_SRC_DIR}/program/Shader.cpp
	${RENDER_SRC_DIR}/program/IDrawingProgram.hpp
	${RENDER_SRC_DIR}/program/IDrawingProgram.cpp
	${RENDER_SRC_DIR}/program/LightHandler.hpp
	${RENDER_SRC_DIR}/program/LightHandler.cpp
	${RENDER_SRC_DIR}/program/DefaultShader.hpp
	${RENDER_SRC_DIR}/program/DefaultShader.cpp
	${RENDER_SRC_DIR}/program/ShaderManager.hpp
	${RENDER_SRC_DIR}/program/ShaderManager.cpp
	${COMMON_SRC_DIR}/application/IApplication.hpp
	${COMMON_SRC_DIR}/platform/Platform.h
)

set(APP_SOURCES
	${SRC_DIR}/scene/Cursor3D.hpp
	${SRC_DIR}/scene/Cursor3D.cpp
	${SRC_DIR}/scene/Scene.hpp
	${SRC_DIR}/scene/Scene.cpp
	${SRC_DIR}/scene/VisualSettings.hpp

	${SRC_DIR}/Application.cpp
	${SRC_DIR}/showobj/ShowObjOp.hpp
	${SRC_DIR}/forcefield/ChargeManipulator.hpp
	${SRC_DIR}/forcefield/ChargeManipulator.cpp
	${SRC_DIR}/forcefield/ForceFieldOp.hpp
	${SRC_DIR}/forcefield/ForceFieldState.hpp
	${SRC_DIR}/forcefield/ForceFieldState.cpp
	${SRC_DIR}/forcefield/PointCharges.hpp
	${SRC_DIR}/forcefield/PointCharges.cpp
	${SRC_DIR}/forcefield/ForceLines.hpp
	${SRC_DIR}/forcefield/ForceLines.cpp
	${SRC_DIR}/forcefield/EquiPotFace.hpp
	${SRC_DIR}/forcefield/EquiPotFace.cpp
	${SRC_DIR}/forcefield/SurfaceMeshing.hpp
	${SRC_DIR}/forcefield/SurfaceMeshing.cpp
)

if(NOT ANDROID) # PC
	set(COMMON_SOURCES ${COMMON_SOURCES} ${COMMON_SRC_DIR}/platform/pc/PcFile.cpp)
endif()

if(NOT MAKE_LIB) # exe
	set(COMMON_SOURCES ${COMMON_SOURCES} ${COMMON_SRC_DIR}/platform/pc/GlutApp.cpp)
endif()


### set libraries
if(ANDROID)
	set(LIBS GLESv2 GLESv3 log)
endif()

if(NOT MAKE_LIB) # exe
	add_subdirectory(${SRC_DIR}/3rdparty/freeglut EXCLUDE_FROM_ALL)
	set(LIBS ${LIBS} freeglut)
endif()


### set target
if(MAKE_LIB)
	add_library(${PROJECT_NAME}
		SHARED
		${LODEPNG_SRC}
		${GLAD_SRC}
		${COMMON_SOURCES}
		${APP_SOURCES}
	)
else()
	add_executable(${PROJECT_NAME}
		${LODEPNG_SRC}
		${GLAD_SRC}
		${COMMON_SOURCES}
		${APP_SOURCES}
	)
endif()

target_link_libraries(${PROJECT_NAME} ${LIBS})

set_target_properties(
	${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
)


### set include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR}/3rdparty)
target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR}/3rdparty/freeglut/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR}/3rdparty/glad/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${COMMON_SRC_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${RENDER_SRC_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${RENDER_SRC_DIR}/program)
target_include_directories(${PROJECT_NAME} PRIVATE ${COMMON_SRC_DIR}/platform)
target_include_directories(${PROJECT_NAME} PRIVATE ${COMMON_SRC_DIR}/application)
target_include_directories(${PROJECT_NAME} PRIVATE ${COMMON_SRC_DIR}/input_devices)
target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR}/scene)
target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR})


### set compiler flags
add_compile_definitions(MODULE_NAME="${PROJECT_NAME}")
if(NOT NDEV)
	add_compile_definitions(_DEVELOPMENT)
endif()
if(DOUBLE)
	add_compile_definitions(_DOUBLE_PRECISION)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-compare -Wno-unused-result -Wno-float-conversion -Wno-conversion -Wall -Wextra -Wno-shadow -Wformat=2 -Wno-float-equal -Wlogical-op")
	if (CMAKE_BUILD_TYPE MATCHES Debug)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2")
	endif()
endif()
