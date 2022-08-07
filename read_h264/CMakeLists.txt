cmake_minimum_required(VERSION 3.16.3)
project(Project_ReadH264)

file(
    COPY         ${CMAKE_CURRENT_SOURCE_DIR}/../phone.h264
    DESTINATION  ${CMAKE_CURRENT_BINARY_DIR}/
)

#指定可执行文件与库文件的生成目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)  #与CMAKE_CURRENT_SOURCE_DIR一致
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)     #PROJECT_BINARY_DIR就是build目录

aux_source_directory(. SRC_LIST)  

#生成可执行程序
add_executable(ReadH264 ${SRC_LIST})  

#生成同名的静态库与动态库
add_library(ReadH264_static STATIC ${SRC_LIST})
add_library(ReadH264_shared SHARED ${SRC_LIST})
SET_TARGET_PROPERTIES(ReadH264_static PROPERTIES OUTPUT_NAME "ReadH264") 
SET_TARGET_PROPERTIES(ReadH264_shared PROPERTIES OUTPUT_NAME "ReadH264") 
SET_TARGET_PROPERTIES(ReadH264_static PROPERTIES CLEAN_DIRECT_OUTPUT 1) 
SET_TARGET_PROPERTIES(ReadH264_shared PROPERTIES CLEAN_DIRECT_OUTPUT 1) 

#需要手动 sudo make install 
# INSTALL(
#         TARGETS ReadH264
#         RUNTIME DESTINATION bin     #可执行文件  安装成功的路径：/usr/local/bin/ReadH264
#         LIBRARY DESTINATION lib     #动态库
#         ARCHIVE DESTINATION lib     #静态库
# )