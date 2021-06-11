# Add cmake module to use ceres library
list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/src/ceres/cmake_modules )

# Find Ceres package 
find_package( Ceres REQUIRED )
include_directories( ${CERES_INCLUDE_DIRS} )

#Build executable 
add_executable( curve_fitting_ceres ${PROJECT_SOURCE_DIR}/src/ceres/main.cpp )
# Add Ceres and OpenCV libs 
target_link_libraries( curve_fitting_ceres ${CERES_LIBRARIES} ${OpenCV_LIBS} )
