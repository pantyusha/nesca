cmake_minimum_required(VERSION 3.3)
project(nesca)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package( Qt5Core REQUIRED )
find_package( Qt5Widgets REQUIRED )
find_package( Qt5Gui REQUIRED )

set(CURL_PATH Z:/curl-7.40.0-devel-mingw64/bin)
#set(SSL_PATH Z:/OpenSSL-Win32/lib)
set(SSH_PATH Z:/libssh_0.6.0/bin)
find_library(libcurl NAMES curl.dll PATHS ${CURL_PATH})
#find_library(libssl NAMES ssl.dll ssleay32.dll libeay32.dll PATHS ${SSL_PATH})
find_library(libssh NAMES ssh.dll PATHS ${SSH_PATH})

include_directories(${PROJECT_SOURCE_DIR})
include_directories("C:/Users/cora32/ClionProjects/nesca")
include_directories("Z:/hikvision_sdk/lib")
include_directories("Z:/libssh_0.6.0/include")
include_directories("Z:/OpenSSL-Win32/include")
include_directories("Z:/curl-7.40.0-devel-mingw64/include")
include_directories("Z:/curl-7.40.0-devel-mingw64/lib64")
include_directories("C:/Users/cora32/.CLion12/system/cmake/generated/22c5278e/22c5278e/Debug")

#add_executable(nesca WIN32 main.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

#get_target_property(QtCore_location Qt5::Core LOCATION)
#qt5_use_modules( nesca Core Widgets Gui )

set(SOURCE_FILES ActivityDrawerTh_HorNet.cpp
ActivityDrawerTh_HorNet.h
base64.cpp
base64.h
BasicAuth.cpp
BasicAuth.h
BruteUtils.cpp
BruteUtils.h
        IPRandomizer.h
CheckKey_Th.cpp
        IPRandomizer.cpp
CheckKey_Th.h
CMakeLists.txt
Connector.cpp
Connector.h
debugData.txt
DrawerTh_GridQoSScanner.cpp
DrawerTh_GridQoSScanner.h
DrawerTh_HorNet.cpp
DrawerTh_HorNet.h
DrawerTh_ME2Scanner.cpp
DrawerTh_ME2Scanner.h
DrawerTh_QoSScanner.cpp
DrawerTh_QoSScanner.h
DrawerTh_VoiceScanner.cpp
DrawerTh_VoiceScanner.h
Eurostile.ttf
externData.h
externFunctions.h
FileDownloader.cpp
FileDownloader.h
FileUpdater.cpp
FileUpdater.h
finder.cpp
FTPAuth.cpp
FTPAuth.h
HikvisionLogin.cpp
HikvisionLogin.h
IPCAuth.cpp
IPCAuth.h
login.txt
main.cpp
mainResources.h
MainStarter.cpp
MainStarter.h
Makefile
msgcheckerthread.cpp
msgcheckerthread.h
negatives.txt
nesca
nesca.ico
nesca.pro
nesca.pro.user
nesca_3.cpp
nesca_3.h
nesca_3.qrc
nesca_3.rc
nesca_3.ui
pass.txt
piestat.cpp
piestat.h
progressbardrawer.cpp
progressbardrawer.h
README.md
resource.h
small_font.ttf
SSHAuth.cpp
SSHAuth.h
sshpass.txt
STh.cpp
STh.h
Threader.cpp
Threader.h
ui_nesca_3.h~
Utils.cpp
Utils.h
vercheckerthread.cpp
vercheckerthread.h
version
WebformWorker.cpp
WebformWorker.h
wflogin.txt
wfpass.txt)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

if(WIN32)
    target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
else()
    target_link_libraries(${PROJECT_NAME} pthread)
endif()

target_link_libraries(${PROJECT_NAME} ${libcurl})
target_link_libraries(${PROJECT_NAME} ${libssh})
#target_link_libraries(${PROJECT_NAME} -LZ:/curl-7.40.0-devel-mingw64/lib64 curl)
target_link_libraries(${PROJECT_NAME} -LZ:/OpenSSL-Win32/lib libeay32 ssleay32)
#target_link_libraries(${PROJECT_NAME} -LZ:/libssh_0.6.0/lib ssh)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)