# Add cmake module to use ceres library
 list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/src/g2o/cmake_modules )

# Find G2O
find_package( G2O REQUIRED )
include_directories( 
    ${G2O_INCLUDE_DIRS}
    "/usr/include/eigen3"
)

# Build the executable 
add_executable( demoG2O ${PROJECT_SOURCE_DIR}/src/g2o/demoG2O.cpp )
# Add G2O and OpenCV libs
target_link_libraries( demoG2O 
    ${OpenCV_LIBS} 
    g2o_core g2o_stuff
    ${Boost_LIBRARIES}
)

