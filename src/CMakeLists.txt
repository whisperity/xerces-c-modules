# CMake build for xerces-c
#
# Written by Roger Leigh <rleigh@codelibre.net>
#
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set(EXTRA_DIST
  stricmp.h
  strnicmp.h
  towlower.h
  towupper.h
  xercesc/NLS
  xercesc/util/Xerces_autoconf_config.hpp.cmake.in
  xercesc/util/Xerces_autoconf_config.hpp.in
  xercesc/util/XercesVersion.hpp.cmake.in
  xercesc/util/XercesVersion.hpp.in
  xercesc/util/MsgLoaders/ICU/resources
)

set(missing_sources
  stricmp.c
  stricmp.h
  strnicmp.c
  strnicmp.h
  towlower.c
  towlower.h
  towupper.c
  towupper.h
)

add_custom_target(xerces-c-missing SOURCES ${missing_sources})
set_target_properties(xerces-c-missing PROPERTIES FOLDER "Library")

#libxerces_c_la_LIBADD = ${LTLIBOBJS}

#
# Definition of sources/headers
#

#
#	A line like the following will generate a list of headers
#	for a given directory: (must be issued from src directory)
#		find xercesc/framework -name '*.hpp' -exec echo {} \\ ";"
#		find xercesc/framework -name '*.cpp' -exec echo {} \\ ";"
#

# These files cannot be mapped to a module due to being implemented in multiple
# places, so they stay as normal TUs.
set(libxerces_c_HEADERS
  xercesc/internal/XProtoType.hpp
  xercesc/internal/XSerializable.hpp
  xercesc/util/XMLInitializer.hpp)
set(libxerces_c_SOURCES
  xercesc/internal/XProtoType.cpp
  xercesc/util/XMLInitializer.cpp)


set(parsers_headers
  xercesc/parsers/AbstractDOMParser.hpp
  xercesc/parsers/DOMLSParserImpl.hpp
  xercesc/parsers/SAX2XMLFilterImpl.hpp
  xercesc/parsers/SAX2XMLReaderImpl.hpp
  xercesc/parsers/SAXParser.hpp
  xercesc/parsers/XercesDOMParser.hpp
)

set(parsers_sources
  xercesc/parsers/AbstractDOMParser.cpp
  xercesc/parsers/DOMLSParserImpl.cpp
  xercesc/parsers/SAX2XMLFilterImpl.cpp
  xercesc/parsers/SAX2XMLReaderImpl.cpp
  xercesc/parsers/SAXParser.cpp
  xercesc/parsers/XercesDOMParser.cpp
)

set(icu_headers
  xercesc/util/Transcoders/ICU/ICUTransService.hpp
)

set(icu_sources
  xercesc/util/Transcoders/ICU/ICUTransService.cpp
)

set(gnuiconv_headers
  xercesc/util/Transcoders/IconvGNU/IconvGNUTransService.hpp
)

set(gnuiconv_sources
  xercesc/util/Transcoders/IconvGNU/IconvGNUTransService.cpp
)

set(iconv_headers
  xercesc/util/Transcoders/Iconv/IconvTransService.hpp
)

set(iconv_sources
  xercesc/util/Transcoders/Iconv/IconvTransService.cpp
)

set(macosunicodeconverter_headers
  xercesc/util/Transcoders/MacOSUnicodeConverter/MacOSUnicodeConverter.hpp
)

set(macosunicodeconverter_sources
  xercesc/util/Transcoders/MacOSUnicodeConverter/MacOSUnicodeConverter.cpp
)

set(wintrans_headers
  xercesc/util/Transcoders/Win32/Win32TransService.hpp
)

set(wintrans_sources
  xercesc/util/Transcoders/Win32/Win32TransService.cpp
)

set(curl_headers
  xercesc/util/NetAccessors/Curl/CurlNetAccessor.hpp
  xercesc/util/NetAccessors/Curl/CurlURLInputStream.hpp
)

set(curl_sources
  xercesc/util/NetAccessors/Curl/CurlNetAccessor.cpp
  xercesc/util/NetAccessors/Curl/CurlURLInputStream.cpp
)

set(sockets_headers
  xercesc/util/NetAccessors/Socket/SocketNetAccessor.hpp
  xercesc/util/NetAccessors/Socket/UnixHTTPURLInputStream.hpp
  xercesc/util/NetAccessors/BinHTTPInputStreamCommon.hpp
)

set(sockets_sources
  xercesc/util/NetAccessors/Socket/SocketNetAccessor.cpp
  xercesc/util/NetAccessors/Socket/UnixHTTPURLInputStream.cpp
  xercesc/util/NetAccessors/BinHTTPInputStreamCommon.cpp
)

set(cfurl_headers
  xercesc/util/NetAccessors/MacOSURLAccessCF/MacOSURLAccessCF.hpp
  xercesc/util/NetAccessors/MacOSURLAccessCF/URLAccessCFBinInputStream.hpp
)

set(cfurl_sources
  xercesc/util/NetAccessors/MacOSURLAccessCF/MacOSURLAccessCF.cpp
  xercesc/util/NetAccessors/MacOSURLAccessCF/URLAccessCFBinInputStream.cpp
)

set(winsock_headers
  xercesc/util/NetAccessors/WinSock/WinSockNetAccessor.hpp
  xercesc/util/NetAccessors/WinSock/BinHTTPURLInputStream.hpp
  xercesc/util/NetAccessors/BinHTTPInputStreamCommon.hpp
)

set(winsock_sources
  xercesc/util/NetAccessors/WinSock/WinSockNetAccessor.cpp
  xercesc/util/NetAccessors/WinSock/BinHTTPURLInputStream.cpp
  xercesc/util/NetAccessors/BinHTTPInputStreamCommon.cpp
)

set(msginmemory_headers
  xercesc/util/MsgLoaders/InMemory/InMemMsgLoader.hpp
  xercesc/util/MsgLoaders/InMemory/XercesMessages_en_US.hpp
)

set(msginmemory_sources
  xercesc/util/MsgLoaders/InMemory/InMemMsgLoader.cpp
)

set(msgicu_headers
  xercesc/util/MsgLoaders/ICU/ICUMsgLoader.hpp
)

set(msgicu_sources
  xercesc/util/MsgLoaders/ICU/ICUMsgLoader.cpp
)

set(msgicu_nodist_sources
  "${CMAKE_CURRENT_BINARY_DIR}/xercesc_messages.c"
  "${CMAKE_CURRENT_BINARY_DIR}/root_res.c"
)

set(msgiconv_headers
  xercesc/util/MsgLoaders/MsgCatalog/MsgCatalogLoader.hpp
  xercesc/util/MsgLoaders/MsgCatalog/XMLMsgCat_Ids.hpp
)

set(msgiconv_sources
  xercesc/util/MsgLoaders/MsgCatalog/MsgCatalogLoader.cpp
)

set(posixfmgr_headers
  xercesc/util/FileManagers/PosixFileMgr.hpp
)

set(posixfmgr_sources
  xercesc/util/FileManagers/PosixFileMgr.cpp
)

set(winfmgr_headers
  xercesc/util/FileManagers/WindowsFileMgr.hpp
)

set(winfmgr_sources
  xercesc/util/FileManagers/WindowsFileMgr.cpp
)

set(nothreadmmgr_headers
  xercesc/util/MutexManagers/NoThreadMutexMgr.hpp
)

set(nothreadmmgr_sources
  xercesc/util/MutexManagers/NoThreadMutexMgr.cpp
)

set(stdmmgr_headers
  xercesc/util/MutexManagers/StdMutexMgr.hpp
)

set(stdmmgr_sources
  xercesc/util/MutexManagers/StdMutexMgr.cpp
)

set(posixmmgr_headers
  xercesc/util/MutexManagers/PosixMutexMgr.hpp
)

set(posixmmgr_sources
  xercesc/util/MutexManagers/PosixMutexMgr.cpp
)

set(winmmgr_headers
  xercesc/util/MutexManagers/WindowsMutexMgr.hpp
)

set(winmmgr_sources
  xercesc/util/MutexManagers/WindowsMutexMgr.cpp
)

# Add replacement functions
if(NOT HAVE_STRICMP)
  list(APPEND xerces_replacement_funcs stricmp.c)
endif()
if(NOT HAVE_STRNICMP)
  list(APPEND xerces_replacement_funcs strnicmp.c)
endif()
if(NOT HAVE_TOWLOWER)
  list(APPEND xerces_replacement_funcs towlower.c)
endif()
if(NOT HAVE_TOWUPPER)
  list(APPEND xerces_replacement_funcs towupper.c)
endif()
list(APPEND libxerces_c_SOURCES ${xerces_replacement_funcs})

# NetAccessors, conditionally built based on selection
if(SOCKET_LIBRARY)
  list(APPEND libxerces_c_DEPS ${SOCKET_LIBRARY})
endif()
if(NSL_LIBRARY)
  list(APPEND libxerces_c_DEPS ${NSL_LIBRARY})
endif()

if(XERCES_USE_NETACCESSOR_CURL)
  list(APPEND libxerces_c_SOURCES ${curl_sources})
  list(APPEND libxerces_c_HEADERS ${curl_headers})
  list(APPEND libxerces_c_DEPS ${CURL_LIBRARIES})
endif()

if(XERCES_USE_NETACCESSOR_SOCKET)
  list(APPEND libxerces_c_SOURCES ${sockets_sources})
  list(APPEND libxerces_c_HEADERS ${sockets_headers})
endif()

if(XERCES_USE_NETACCESSOR_CFURL)
  list(APPEND libxerces_c_SOURCES ${cfurl_sources})
  list(APPEND libxerces_c_HEADERS ${cfurl_headers})
  list(APPEND libxerces_c_DEPS ${CORE_SERVICES_LIBRARY})
endif()

if(XERCES_USE_NETACCESSOR_WINSOCK)
  list(APPEND libxerces_c_SOURCES ${winsock_sources})
  list(APPEND libxerces_c_HEADERS ${winsock_headers})
endif()


# Transcoders, conditionally built based on selection
#
if(XERCES_USE_TRANSCODER_ICU)
  list(APPEND libxerces_c_SOURCES ${icu_sources})
  list(APPEND libxerces_c_HEADERS ${icu_headers})
  list(APPEND libxerces_c_DEPS ICU::uc ICU::data)
endif()

if(XERCES_USE_TRANSCODER_GNUICONV)
  list(APPEND libxerces_c_SOURCES ${gnuiconv_sources})
  list(APPEND libxerces_c_HEADERS ${gnuiconv_headers})
endif()

if(XERCES_USE_TRANSCODER_ICONV)
  list(APPEND libxerces_c_SOURCES ${iconv_sources})
  list(APPEND libxerces_c_HEADERS ${iconv_headers})
endif()

if(XERCES_USE_TRANSCODER_MACOSUNICODECONVERTER)
  list(APPEND libxerces_c_SOURCES ${macosunicodeconverter_sources})
  list(APPEND libxerces_c_HEADERS ${macosunicodeconverter_headers})
  list(APPEND libxerces_c_DEPS ${CORE_SERVICES_LIBRARY})
endif()

if(XERCES_USE_TRANSCODER_WINDOWS)
  list(APPEND libxerces_c_SOURCES ${wintrans_sources})
  list(APPEND libxerces_c_HEADERS ${wintrans_headers})
endif()


# MsgLoaders, conditionally built based on selection
#
if(XERCES_USE_MSGLOADER_INMEMORY)
  list(APPEND libxerces_c_SOURCES ${msginmemory_sources})
  list(APPEND libxerces_c_HEADERS ${msginmemory_headers})
endif()

if(XERCES_USE_MSGLOADER_ICU)
  list(APPEND libxerces_c_SOURCES ${msgicu_sources} ${msgicu_nodist_sources})
  list(APPEND libxerces_c_HEADERS ${msgicu_headers})
  list(APPEND libxerces_c_DEPS ICU::uc ICU::data)

  # Resource shortname
  set(PKGNAME xercesc_messages_${INTERFACE_VERSION_U})

  add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/xercesc_messages.c"
    COMMAND "${ICU_GENCMN_EXECUTABLE}" --name "${PKGNAME}" -S "${PKGNAME}_dat.c" -d .
    "${CMAKE_CURRENT_SOURCE_DIR}/xercesc/util/MsgLoaders/ICU/resources/res-file-list.txt"
    COMMAND "${CMAKE_COMMAND}" -E rename "${PKGNAME}_dat.c" xercesc_messages.c
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/xercesc/util/MsgLoaders/ICU/resources/res-file-list.txt"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

  add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/root.res"
    COMMAND "${ICU_GENRB_EXECUTABLE}" -d .
    "${CMAKE_CURRENT_SOURCE_DIR}/xercesc/util/MsgLoaders/ICU/resources/root.txt"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/xercesc/util/MsgLoaders/ICU/resources/root.txt"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

  add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/root_res.c"
    COMMAND "${ICU_GENCCODE_EXECUTABLE}" --name ${PKGNAME} -d . root.res
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/root.res"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

  add_custom_target(
    genicu
    DEPENDS xercesc_messages.c root_res.c)
  set_target_properties(genicu PROPERTIES FOLDER "Library")
endif()

if(XERCES_USE_MSGLOADER_ICONV)
  list(APPEND libxerces_c_SOURCES ${msgiconv_sources})
  list(APPEND libxerces_c_HEADERS ${msgiconv_headers})

  set(PKGNAME XercesMessages)

  set(RESTARGET ${CMAKE_CURRENT_BINARY_DIR}/${PKGNAME}_en_US.cat)
  set(RESFILE ${CMAKE_CURRENT_SOURCE_DIR}/xercesc/util/MsgLoaders/MsgCatalog/XercesMessages_en_US.Msg)

  add_custom_command(
    OUTPUT ${RESTARGET}
    COMMAND gencat ${RESTARGET} ${RESFILE}
    DEPENDS ${RESFILE})

  add_custom_target(
    gencat ALL
    DEPENDS ${RESTARGET})
  set_target_properties(gencat PROPERTIES FOLDER "Library")

  install(
    FILES "${RESTARGET}"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/msg"
    COMPONENT "runtime")
endif()

# Mutex Managers, conditionally built based on selection
#
if(XERCES_USE_MUTEXMGR_NOTHREAD)
  list(APPEND libxerces_c_SOURCES ${nothreadmmgr_sources})
  list(APPEND libxerces_c_HEADERS ${nothreadmmgr_headers})
endif()

if(XERCES_USE_MUTEXMGR_STD)
  list(APPEND libxerces_c_SOURCES ${stdmmgr_sources})
  list(APPEND libxerces_c_HEADERS ${stdmmgr_headers})
  list(APPEND libxerces_c_DEPS Threads::Threads)
endif()

if(XERCES_USE_MUTEXMGR_POSIX)
  list(APPEND libxerces_c_SOURCES ${posixmmgr_sources})
  list(APPEND libxerces_c_HEADERS ${posixmmgr_headers})
  list(APPEND libxerces_c_DEPS Threads::Threads)
endif()

if(XERCES_USE_MUTEXMGR_WINDOWS)
  list(APPEND libxerces_c_SOURCES ${winmmgr_sources})
  list(APPEND libxerces_c_HEADERS ${winmmgr_headers})
endif()


# File Managers, conditionally built based on selection
#
if(XERCES_USE_FILEMGR_POSIX)
  list(APPEND libxerces_c_SOURCES ${posixfmgr_sources})
  list(APPEND libxerces_c_HEADERS ${posixfmgr_headers})
endif()

if(XERCES_USE_FILEMGR_WINDOWS)
  list(APPEND libxerces_c_SOURCES ${winfmgr_sources})
  list(APPEND libxerces_c_HEADERS ${winfmgr_headers})
endif()

# Windows resource.  With some additional work to clean it up, it
# might be possible to use with Cygwin and MinGW.
if(MSVC)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/xercesc/util/version.rc.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/xercesc/util/version.rc"
    @ONLY)
  list(APPEND libxerces_c_RESOURCES "${CMAKE_CURRENT_BINARY_DIR}/xercesc/util/version.rc")
endif()

# Definitions required for building
add_definitions(
  -DHAVE_CONFIG_H=1
  -DXERCES_BUILDING_LIBRARY=1
)

# Search the project binary dir for config.h
include_directories(
  ${PROJECT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)
#if(XERCES_USE_NETACCESSOR_CURL)
#  include_directories(SYSTEM ${CURL_INCLUDE_DIRS})
#endif()

add_custom_target(xerces-c-headers SOURCES ${libxerces_c_HEADERS})
set_target_properties(xerces-c-headers PROPERTIES FOLDER "Library")

include(ModuleList.cmake)

add_library(xerces-c
  ${libxerces_c_SOURCES}
  ${libxerces_c_RESOURCES})
target_link_libraries(xerces-c ${libxerces_c_DEPS})

foreach(_module IN LISTS REGISTERED_MODULES)
  add_module_to_target(xerces-c ${_module})
endforeach()

if(XERCES_USE_NETACCESSOR_CURL)
  target_include_directories(xerces-c SYSTEM PRIVATE ${CURL_INCLUDE_DIRS})
endif()

if(MSVC)
  # Add configuration-specific library name to resource file.
  target_compile_definitions(xerces-c PRIVATE "XERCES_DLL_NAME=\"$<TARGET_FILE_NAME:xerces-c>\\0\"")
  # Different naming for Windows than for Unix builds for backward
  # compatibility with the existing project files.
  set_target_properties(xerces-c PROPERTIES OUTPUT_NAME "xerces-c_${INTERFACE_VERSION_MAJOR}")
  set_target_properties(xerces-c PROPERTIES RUNTIME_OUTPUT_NAME "xerces-c_${INTERFACE_VERSION_U}")
  set_target_properties(xerces-c PROPERTIES DEBUG_POSTFIX "D")
elseif(UNIX)
  # For strict libtool compatibility on Unix.  It's a horrible hack to
  # set the version in the filename, and create the symlink at install
  # time.  Note: could be dropped when the SONAME is updated and
  # libtool compatibility is no longer required.
  set_target_properties(xerces-c PROPERTIES OUTPUT_NAME "xerces-c-${INTERFACE_VERSION_D}")
  file(GENERATE
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/InstallLibrarySymlink.cmake"
    CONTENT "execute_process(COMMAND ln -sf \"$<TARGET_FILE_NAME:xerces-c>\" \"\$ENV{DESTDIR}${CMAKE_INSTALL_FULL_LIBDIR}/libxerces-c.so\")")
  install(SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/InstallLibrarySymlink.cmake")
else()
  # Not used for the common cases, though this would be the default if
  # not for libtool compatibility.
  set_target_properties(xerces-c PROPERTIES SOVERSION ${INTERFACE_VERSION_D})
  if(NOT CYGWIN)
    # This property causes shared libraries on Linux to have the full
    # version encoded into their final filename.  We disable this on
    # Cygwin because it causes xerces-c-${XERCES_C_FULL_VERSION}.dll
    # to be created when xerces-c.dll seems to be the default.
    set_target_properties(xerces-c PROPERTIES VERSION ${INTERFACE_VERSION_D})
  endif()
endif()
set_target_properties(xerces-c PROPERTIES FOLDER "Library")

if(WIN32)
  set(xerces_config_dir "cmake")
else()
  set(xerces_config_dir "${CMAKE_INSTALL_LIBDIR}/cmake/XercesC")
endif()

install(TARGETS xerces-c
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT "runtime"
  INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

include(CMakePackageConfigHelpers)
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/XercesCConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/XercesCConfig.cmake"
  INSTALL_DESTINATION "${xerces_config_dir}")
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/XercesCConfigVersion.cmake
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY SameMajorVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/XercesCConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/XercesCConfigVersion.cmake
              DESTINATION "${xerces_config_dir}")

foreach(hdr IN LISTS libxerces_c_HEADERS)
  get_filename_component(hdrdir "${hdr}" DIRECTORY)
  install(
    FILES "${hdr}"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${hdrdir}"
    COMPONENT "development")
endforeach()

# Source file grouping (for IDE project layout)
set(source_files ${libxerces_c_SOURCES} ${libxerces_c_HEADERS})
list(SORT source_files)
unset(group_dir)
unset(group_files)
foreach(file IN LISTS source_files)
  get_filename_component(dir "${file}" PATH)
  if(group_dir AND NOT dir STREQUAL group_dir)
    if(group_files)
      source_group("${group_dir}" FILES ${group_files})
    endif()
    unset(group_files)
  endif()
  list(APPEND group_files "${file}")
  set(group_dir "${dir}")
endforeach()
if(group_files)
  source_group("${group_dir}" FILES ${group_files})
endif()
unset(group_dir)
unset(group_files)

# Make sure cmake-generated Xerces_autoconf_config.hpp and
# XercesVersion.hpp end up in a proper place when installed.
install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/xercesc/util/Xerces_autoconf_config.hpp"
        "${CMAKE_CURRENT_BINARY_DIR}/xercesc/util/XercesVersion.hpp"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/xercesc/util"
  COMPONENT "development")
