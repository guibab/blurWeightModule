cmake_minimum_required(VERSION 3.2.2)
project(blurWeightModule)
SET(MODULE_NAME ${PROJECT_NAME} CACHE STRING "Name of the module")
SET(MAYA_VERSION 2020 CACHE STRING "Maya version number")
configure_file(${PROJECT_NAME}.mod.in ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.mod @ONLY)

# Compiled submodules
add_subdirectory(submodules/blurSkin)
add_subdirectory(submodules/brSkinBrush)
add_subdirectory(submodules/wireframeDisplay)

# Python Submodules
add_subdirectory(submodules/mPaintEditor)
add_subdirectory(submodules/mWeightEditor)

install(FILES ${PROJECT_NAME}.mod DESTINATION .)
