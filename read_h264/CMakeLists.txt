cmake_minimum_required(VERSION 3.0.0)
project(ReadH264)

file(GLOB SRC_LIST     "*.cpp")
file(GLOB INCLUDE_LIST "*.h")

aux_source_directory(. SRC)

add_executable(ReadH264 ${SRC_LIST} ${INCLUDE_LIST})   #必须包含头文件


file(
    COPY         ${CMAKE_CURRENT_SOURCE_DIR}/../phone.h264
    DESTINATION  ${CMAKE_CURRENT_BINARY_DIR}/
)

INSTALL(
        TARGETS ReadH264
        RUNTIME DESTINATION bin     #可执行目标二进制
        LIBRARY DESTINATION lib     #动态库
        ARCHIVE DESTINATION lib     #静态库
)