cmake_minimum_required(VERSION 3.12)

set(SOURCE_NAME
        main.cpp
        convert.cpp
        convert.hpp)

function(Flag flag init_flag flag_message)
    if(NOT DEFINED ${flag})
        set(${flag} ${init_flag} CACHE BOOL "${flag_message} : true or false" FORCE)
    endif()
endfunction(Flag)

Flag(USE_Boost true "Find and link Boost library")
Flag(USE_CV true "Find and link OpenCV library")
Flag(USE_RS2 true "Find and link librealsense2 library")
Flag(USE_PCL false "Find and link Point Cloud Library")
Flag(USE_ZED false "Find and link ZED Stereo Camera SDK")

#project : プログラム全体の構成単位
#基本的にプログラム全体を格納しているディレクトリの名前をプロジェクト名とする
if(NOT PROJECT_NAME)
    get_filename_component(DEFAULT_PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)
    set(PROJECT_NAME ${DEFAULT_PROJECT_NAME} CACHE STRING "Project name" FORCE)

endif(NOT PROJECT_NAME)
project(${PROJECT_NAME} C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CXX_SOURCE_FILE_EXTS cpp;cxx)
set(CXX_INCLUDE_FILE_EXTS hpp;h)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

#ビルドの対象と実行ファイル名
add_executable(${PROJECT_NAME} ${SOURCE_NAME})

#Boostライブラリを探す
if(USE_Boost)
    #必要なライブラリの場合はREQUIREDを付ける
    find_package(Boost REQUIRED)

    if(Boost_FOUND)
        message("---- Found Boost ${Boost_INCLUDE_DIRS}")
        set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} ${Boost_INCLUDE_DIR})

    else(Boost_FOUND)
        message(FATAL_ERROR " ---- Not Found Boost")

    endif(Boost_FOUND)

    set(ALL_LINK_LIBRARIES ${ALL_LINK_LIBRARIES} ${Boost_LIBRARIES} Boost::thread)

endif(USE_Boost)

if(USE_CV)
    find_package(OpenCV REQUIRED)

    if(OpenCV_FOUND)
        message("---- Found OpenCV ${OpenCV_INCLUDE_DIRS}")
        set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} ${OpenCV_INCLUDE_DIRS})

    else(OpenCV_FOUND)
        message(FATAL_ERROR "---- Not Found OpenCV")

    endif(OpenCV_FOUND)

    set(ALL_LINK_LIBRARIES ${ALL_LINK_LIBRARIES} ${OpenCV_LIBRARIES})
    target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

endif(USE_CV)

if(USE_RS2)
    find_package(realsense2 REQUIRED)

    if(realsense2_FOUND)
        message("---- Found realsense2 ${realsense2_INCLUDE_DIRS}")
        set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} ${realsense2_INCLUDE_DIRS})

    else(realsense2_FOUND)
        message(FATAL_ERROR "---- Not Found realsense2")

    endif(realsense2_FOUND)

    include_directories(${realsense_INCLUDE_DIR})
    set(ALL_LINK_LIBRARIES ${ALL_LINK_LIBRARIES} ${realsense2_LIBRARIES})
    target_link_libraries(${PROJECT_NAME} ${realsense2_LIBRARY})

endif(USE_RS2)

if(USE_PCL)
    find_package(PCL 1.2 REQUIRED)

    if(PCL_FOUND)
        message("---- Found PCL ${PCL_INCLUDE_DIRS}")
        #set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} ${PCL_INCLUDE_DIRS})
        include_directories(${PCL_INCLUDE_DIRS})
        link_directories(${PCL_LIBRARY_DIRS})
        add_definitions(${PCL_DEFINITIONS})

    else(PCL_FOUND)
        message(FATAL_ERROR "---- Not Found PCL")

    endif(PCL_FOUND)

    set(ALL_LINK_LIBRARIES ${ALL_LINK_LIBRARIES} ${PCL_LIBRARIES})
    target_link_libraries(${PROJECT_NAME} ${PCL_LIBRARIES})

endif(USE_PCL)

if(USE_ZED)
    find_package(ZED 2.3 REQUIRED)

    if(ZED_FOUND)
        message("---- Found ZED ${ZED_INCLUDE_DIRS}")
        find_package(CUDA ${ZED_CUDA_VERSION} EXACT REQUIRED)
        
        include_directories(${CUDA_INCLUDE_DIRS})
        include_directories(${ZED_INCLUDE_DIRS})
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

        link_directories(${ZED_LIBRARY_DIR})
        link_directories(${CUDA_LIBRARY_DIRS})

    else(ZED_FOUND)
        message(FATAL_ERROR "---- Not Found ZED")

    endif(ZED_FOUND)

    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        ${ZED_LIBRARIES}
        ${CUDA_CUDA_LIBRARY} ${CUDA_CUDART_LIBRARY} ${CUDA_NPP_LIBRARIES_ZED} )

endif(USE_ZED)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} -lpthread -lm)
