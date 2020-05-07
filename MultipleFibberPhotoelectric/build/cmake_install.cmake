# Install script for directory: /home/marcosmr/Documentos/Geant4/TRITIUM_examples/My_Examples/LastVersionOfGeant4/MultipleFibberPhotoelectric

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/MultipleFibbers" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/MultipleFibbers")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/MultipleFibbers"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/marcosmr/Documentos/Geant4/TRITIUM_examples/My_Examples/LastVersionOfGeant4/MultipleFibberPhotoelectric/build/MultipleFibbers")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/MultipleFibbers" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/MultipleFibbers")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/MultipleFibbers"
         OLD_RPATH "/usr/local/Geant4ST/lib:/usr/local/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/MultipleFibbers")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/MultipleFibbers" TYPE FILE FILES
    "/home/marcosmr/Documentos/Geant4/TRITIUM_examples/My_Examples/LastVersionOfGeant4/MultipleFibberPhotoelectric/init.mac"
    "/home/marcosmr/Documentos/Geant4/TRITIUM_examples/My_Examples/LastVersionOfGeant4/MultipleFibberPhotoelectric/init_vis.mac"
    "/home/marcosmr/Documentos/Geant4/TRITIUM_examples/My_Examples/LastVersionOfGeant4/MultipleFibberPhotoelectric/TritiumSource.mac"
    "/home/marcosmr/Documentos/Geant4/TRITIUM_examples/My_Examples/LastVersionOfGeant4/MultipleFibberPhotoelectric/TRITIUMSpectrum.txt"
    "/home/marcosmr/Documentos/Geant4/TRITIUM_examples/My_Examples/LastVersionOfGeant4/MultipleFibberPhotoelectric/run1.mac"
    "/home/marcosmr/Documentos/Geant4/TRITIUM_examples/My_Examples/LastVersionOfGeant4/MultipleFibberPhotoelectric/vis.mac"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/marcosmr/Documentos/Geant4/TRITIUM_examples/My_Examples/LastVersionOfGeant4/MultipleFibberPhotoelectric/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
