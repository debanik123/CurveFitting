# Add cmake module to use ceres library
list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/src/g2o/cmake_modules )

# Find G2O
find_package( G2O REQUIRED )
include_directories( 
    ${G2O_INCLUDE_DIRS}
    "/usr/include/eigen3"
)

# OpenCV
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_DIRS} )

add_executable( curve_fitting_g2o ${PROJECT_SOURCE_DIR}/src/g2o/main.cpp )
# Add G2O and OpenCV libs
target_link_libraries( curve_fitting_g2o 
    ${OpenCV_LIBS}
    g2o_core g2o_stuff
)
