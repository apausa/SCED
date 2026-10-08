CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

# Name
PROJECT( SCED )

# Version
SET( ${PROJECT_NAME}_VERSION_MAJOR 0 )
SET( ${PROJECT_NAME}_VERSION_MINOR 0 )
SET( ${PROJECT_NAME}_VERSION_PATCH 1 )

# Dependencies
FIND_PACKAGE( SDL3 REQUIRED CONFIG )
FIND_PACKAGE( SDL3_ttf REQUIRED )
FIND_PACKAGE( OpenGL REQUIRED )
FIND_PACKAGE( glm REQUIRED )
FIND_PACKAGE( Fontconfig REQUIRED )

# Default to an optimized build with debug information
IF( NOT CMAKE_BUILD_TYPE )
    SET( CMAKE_BUILD_TYPE "RelWithDebInfo" )
ENDIF()

# Enforce the language standard
SET( CMAKE_CXX_STANDARD 17 )
SET( CMAKE_CXX_STANDARD_REQUIRED ON )

# Compile options
ADD_COMPILE_OPTIONS( "-Wall" "-Wextra" )

# SCED CLIENT 

# Collect client source files
AUX_SOURCE_DIRECTORY( ./src/client client_sources )

# Compile client
ADD_LIBRARY( sced-client SHARED ${client_sources} )

# Headers are included relative to src/client, shared types relative to src
TARGET_INCLUDE_DIRECTORIES( sced-client PUBLIC ./src/client ./src )

INSTALL( TARGETS sced-client LIBRARY DESTINATION lib )

# Only the public API is installed
INSTALL( FILES ./src/client/sced_elements.h ./src/client/sced_connection.h ./src/sced_types.h DESTINATION include )

# SCED SERVER 

# Collect server source files, in every subdirectory (CONFIGURE_DEPENDS re-checks at build time)
FILE( GLOB_RECURSE server_sources CONFIGURE_DEPENDS ./src/server/*.cc )

# Compile server
ADD_EXECUTABLE( sced-server ${server_sources} )

# Headers are included relative to src/server
TARGET_INCLUDE_DIRECTORIES( sced-server PRIVATE ./src/server )

# Link libraries
TARGET_LINK_LIBRARIES(
  sced-server
  sced-client
  SDL3::SDL3
  OpenGL::GL
  SDL3_ttf::SDL3_ttf
  glm::glm
  Fontconfig::Fontconfig
)

INSTALL( TARGETS sced-server DESTINATION bin )