# 最低的cmake版本
cmake_minimum_required(VERSION 3.18.1)

project("learnjni")
# 设置所有的文件
file(GLOB allCpp *.c *.h *.cpp)
#  ${CMAKE_CURRENT_LIST_DIR} 当前路径
include_directories( ${CMAKE_CURRENT_LIST_DIR})

message(WARNING "-----------------CMAKE COMMON ARGUMENT----------------------\n")
message("0. CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
message("1. CUSTOM ARGUMENT = ${WO_SHI_NI_DIE}" )
message("2. PROJECT_BINARY_DIR = ${PROJECT_BINARY_DIR}")
message("3. CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
message("4. CMAKE_CURRRENT_BINARY_DIR = ${CMAKE_CURRRENT_BINARY_DIR}")
message("5. CMAKE_CURRENT_LIST_FILE = ${CMAKE_CURRENT_LIST_FILE}")
message("6. CMAKE_CURRENT_LIST_LINE = ${CMAKE_CURRENT_LIST_LINE}")
message("7. CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")
message("8. CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message("9. EXECUTABLE_OUTPUT_PATH = ${EXECUTABLE_OUTPUT_PATH}")
message("10. LIBRARY_OUTPUT_PATH = ${LIBRARY_OUTPUT_PATH}")
message("11. PROJECT_NAME = ${PROJECT_NAME}")
message("12. PROJECT_VERSION_MAJOR = ${PROJECT_VERSION_MAJOR}")
message("13. PROJECT_VERSION_MINOR = ${PROJECT_VERSION_MINOR}")
message("14. PROJECT_VERSION_PATCH = ${PROJECT_VERSION_PATCH}")
message("15. CMAKE_SYSTEM = ${CMAKE_SYSTEM}")
message("16. CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")
message("17. CMAKE_SYSTEM_VERSION = ${CMAKE_SYSTEM_VERSION}")
message("18. BUILD_SHARED_LIBS = ${BUILD_SHARED_LIBS}")
message("19. CMAKE_C_FLAGS = ${CMAKE_C_FLAGS}")
message("20. CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
message("21. CMAKE_SYSTEM_PROCESSOR   = ${CMAKE_SYSTEM_PROCESSOR}")
message(WARNING "---------------------------------------\n")

add_library(
        learnjni
        SHARED
        ${allCpp})

find_library( # Sets the name of the path variable.
        log-lib
        log)
# 链接第三方库:只需将库放到这个目录下即可
# link_directories(${ALG_LIB_PATH}/${LIBS_NAME}/jniLibs-alg-ssp/${ANDROID_ABI})

# 链接库:需添加so库名称
target_link_libraries( # Specifies the target library.
        learnjni
#        log)
        ${log-lib})