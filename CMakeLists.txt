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

# find Qt
FIND_PACKAGE(Qt COMPONENTS Xml REQUIRED)

# set include directories
INCLUDE_DIRECTORIES(${ROOT_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include ${Geant4_INCLUDE_DIR} ${QT_INCLUDE_DIR})

# with c++0x support, dangerous
ADD_DEFINITIONS("-std=c++0x")

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
FILE(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
FILE(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
FILE(GLOB Detector ${PROJECT_SOURCE_DIR}/src/detector/*.cc)
FILE(GLOB Physics ${PROJECT_SOURCE_DIR}/src/physics/*.cc)
FILE(GLOB Generator ${PROJECT_SOURCE_DIR}/src/generator/*.cc)
FILE(GLOB Analysis ${PROJECT_SOURCE_DIR}/src/analysis/*.cc)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#

ADD_LIBRARY(BambooPhysics SHARED ${Physics})
TARGET_LINK_LIBRARIES(BambooPhysics ${Geant4_LIBRARIES})
SET_TARGET_PROPERTIES(BambooPhysics PROPERTIES VERSION 0.1 SOVERSION 1)

ADD_LIBRARY(BambooGenerator SHARED ${Generator})
TARGET_LINK_LIBRARIES(BambooGenerator ${Geant4_LIBRARIES})
SET_TARGET_PROPERTIES(BambooGenerator PROPERTIES VERSION 0.1 SOVERSION 1)

ADD_LIBRARY(BambooAnalysis SHARED ${Analysis})
TARGET_LINK_LIBRARIES(BambooAnalysis ${Geant4_LIBRARIES})
SET_TARGET_PROPERTIES(BambooAnalysis PROPERTIES VERSION 0.1 SOVERSION 1)

ADD_LIBRARY(BambooDetector SHARED ${Detector})
TARGET_LINK_LIBRARIES(BambooDetector ${Geant4_LIBRARIES} BambooAnalysis)
SET_TARGET_PROPERTIES(BambooDetector PROPERTIES VERSION 0.1 SOVERSION 1)

ADD_LIBRARY(BambooCore SHARED ${sources})
TARGET_LINK_LIBRARIES(BambooCore ${Geant4_LIBRARIES} ${QT_QTXML_LIBRARY})
SET_TARGET_PROPERTIES(BambooCore PROPERTIES VERSION 0.1 SOVERSION 1)

ADD_EXECUTABLE(BambooMC BambooMC.cc)
TARGET_LINK_LIBRARIES(BambooMC BambooCore BambooGenerator BambooDetector BambooPhysics BambooAnalysis ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#--- test related ---#
SET(BUILD_TEST OFF CACHE BOOL "whether the test programs will be built")
IF(BUILD_TEST)
ENABLE_TESTING()
FILE(GLOB TEST_SRC ${PROJECT_SOURCE_DIR}/test/*.cxx)
FILE(COPY ${PROJECT_SOURCE_DIR}/test/test.xml DESTINATION ${PROJECT_BINARY_DIR})
ADD_EXECUTABLE(bambooTest ${TEST_SRC})
TARGET_LINK_LIBRARIES(bambooTest CppUTest CppUTestExt BambooDetector BambooCore BambooAnalysis ${ROOT_LIBRARIES})
ADD_TEST(allTest bambooTest)
ENDIF()
