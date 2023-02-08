cmake_minimum_required(VERSION 3.10.2)

project("face_ai")

set(TOOLKIT_ROOT_DIR ${CMAKE_SOURCE_DIR})
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)
include_directories(${TOOLKIT_ROOT_DIR})

find_library(log-lib log)

add_library(avcodec SHARED IMPORTED)
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/jni/${ANDROID_ABI}/libavcodec.so)

add_library(avfilter SHARED IMPORTED)
set_target_properties(avfilter PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/jni/${ANDROID_ABI}/libavfilter.so)

add_library(avformat SHARED IMPORTED)
set_target_properties(avformat PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/jni/${ANDROID_ABI}/libavformat.so)

add_library(avutil SHARED IMPORTED)
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/jni/${ANDROID_ABI}/libavutil.so)

add_library(avdevice SHARED IMPORTED)
set_target_properties(avdevice PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/jni/${ANDROID_ABI}/libavdevice.so)

add_library(swresample SHARED IMPORTED)
set_target_properties(swresample PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/jni/${ANDROID_ABI}/libswresample.so)

add_library(swscale SHARED IMPORTED)
set_target_properties(swscale PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/jni/${ANDROID_ABI}/libswscale.so)

add_library(${PROJECT_NAME} SHARED native-lib.cc)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/ffmpeg)
target_link_libraries(${PROJECT_NAME}
        android
        ${log-lib}
        jnigraphics
        avcodec
        avutil
        avformat
        avdevice
        avfilter
        swresample
        swscale
        )