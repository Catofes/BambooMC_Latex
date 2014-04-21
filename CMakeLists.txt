CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)
PROJECT(BambooMC)

OPTION(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)

IF(WITH_GEANT4_UIVIS)
  FIND_PACKAGE(Geant4 REQUIRED ui_all vis_all)
ELSE()
  FIND_PACKAGE(Geant4 REQUIRED)
ENDIF()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
INCLUDE(${Geant4_USE_FILE})

# find root and clhep
FIND_PACKAGE(ROOT)
#FIND_PACKAGE(CLHEP)

# set include directories
INCLUDE_DIRECTORIES(${ROOT_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include ${Geant4_INCLUDE_DIR})

# build with debug symbols
ADD_DEFINITIONS("-g")

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
FILE(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
FILE(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
ADD_EXECUTABLE(BambooMC BambooMC.cc ${sources} ${headers})
TARGET_LINK_LIBRARIES(BambooMC ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
