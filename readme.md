# LOS2FreeRTOS
----

## 1. 在CMake中使用
1. 在项目的主CMakeLists.txt中导入LOS2FreeRTOS静态库
2. 为LOS2FreeRTOS库链接FreeRTOS库
3. 为项目的可执行文件链接LOS2FreeRTOS库和添加LOS2FreeRTOS库的头文件

```cmake
# 在项目的主CMakeLists.txt文件中添加下面的代码

# 1. 导入LOS2FreeRTOS
add_subdirectory("LOS2FreeRTOS")
# 2. 为LOS2FreeRTOS库链接FreeRTOS库 (这里的FreeRTOS由stm32cubemx管理)
target_link_libraries(LOS2FreeRTOS stm32cubemx)
# 3. 为项目的可执行文件链接LOS2FreeRTOS库和添加LOS2FreeRTOS库的头文件
target_include_directories(${PROJECT_NAME} PRIVATE "${LOS2FreeRTOS_INCLUDE_DIRS}")
target_link_libraries(${PROJECT_NAME} LOS2FreeRTOS)
```