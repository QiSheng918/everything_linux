cmake_minimum_required(VERSION 3.5)
project(everything_linux)

set(CMAKE_CXX_STANDARD 11)

# 直接让cmake自动转换qt中qrc，ui，包含QOBJECT的头文件
# set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5 COMPONENTS Widgets Core Gui Sql REQUIRED)


find_path(SQLITE3_INCLUDE_DIR sqlite3.h
  /usr/include
  /usr/local/include
  $ENV{ProgramFiles}/SQLite/include
  $ENV{SystemDrive}/SQLite/include
  $ENV{ProgramFiles}/SQLite
  $ENV{SystemDrive}/SQLite
  $ENV{ProgramFiles}/SQLite3/include
  $ENV{SystemDrive}/SQLite3/include
  $ENV{ProgramFiles}/SQLite3
  $ENV{SystemDrive}/SQLite3
)

find_library(SQLITE3_LIBRARIES NAMES sqlite3
  PATHS
  /usr/lib
  /usr/local/lib
  $ENV{ProgramFiles}/SQLite/lib
  $ENV{SystemDrive}/SQLite/lib
  $ENV{ProgramFiles}/SQLite
  $ENV{SystemDrive}/SQLite
  $ENV{ProgramFiles}/SQLite3/lib
  $ENV{SystemDrive}/SQLite3/lib
  $ENV{ProgramFiles}/SQLite3
  $ENV{SystemDrive}/SQLite3
)


INCLUDE_DIRECTORIES(

  everything_updatedb/
  everything_updatedb/gnulib
  include/
  QHotkey/
  ${PROJECT_SOURCE_DIR}
  ${QT5_INCLUDE_DIRS} 
  ${SQLITE3_INCLUDE_DIR}

)



 set(Q_GLOBALSHOTCUT_LIBS Qt5::Widgets Qt5::Core Qt5::Gui)
 set(Q_GLOBALSHOTCUT_SRC_FILES  QHotkey/qhotkey.cpp)

 MESSAGE(STSTUS "###################################")
 MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")

 IF (WIN32)
        MESSAGE(STATUS "Now is windows")
   set(SRC_FILES ${SRC_FILES} QHotkey/qhotkey_win.cpp)
  
 ELSEIF (UNIX)
        MESSAGE(STATUS "Now is UNIX-like OS's.")
        find_package(X11 REQUIRED)
   find_package(Qt5X11Extras REQUIRED)
   include_directories(${X11_INCLUDE_DIR})
   set(Q_GLOBALSHOTCUT_LIBS ${Q_GLOBALSHOTCUT_LIBS} ${X11_LIBRARIES} Qt5::X11Extras)
   set(Q_GLOBALSHOTCUT_SRC_FILES ${Q_GLOBALSHOTCUT_SRC_FILES} QHotkey/qhotkey_x11.cpp)
 ELSEIF (APPLE)
        MESSAGE(STATUS "Now is Apple systens.")
 ENDIF ()

 MESSAGE(STSTUS "###################################")

 add_library(qglobalshotcut SHARED ${Q_GLOBALSHOTCUT_SRC_FILES})
 target_link_libraries(qglobalshotcut ${Q_GLOBALSHOTCUT_LIBS})


# 手动添加qrc文件，将qrc文件转换为*_qrc.cpp文件
FILE(GLOB_RECURSE QRC_SOURCE_FILES resource/*.qrc)
QT5_ADD_RESOURCES(QRC_FILES ${QRC_SOURCE_FILES})
MESSAGE(STATUS ${QRC_FILES})

# 手动添加ui文件，将ui文件转换为ui_*.hpp文件
FILE(GLOB_RECURSE QUI_SOURCE_FILES ui/*.ui)
QT5_WRAP_UI(UI_FILES ${QUI_SOURCE_FILES})
MESSAGE(STATUS ${UI_FILES})

# 手动添加moc文件，将h文件转换为moc_*.hpp文件
FILE(GLOB_RECURSE QMOC_SOURCE_FILES include/*.h)
QT5_WRAP_CPP(MOC_FILES ${QMOC_SOURCE_FILES})
MESSAGE(STATUS ${MOC_FILES})


set(qt_src_files
    src/ctableview.cpp
    src/mainwindow.cpp
    src/csqlquerymodel.cpp
    src/setting.cpp
    src/traymenu.cpp
    ${QRC_FILES}
    ${UI_FILES}
    ${MOC_FILES}
)

add_library(qmainwindow SHARED ${qt_src_files})
target_link_libraries(qmainwindow  Qt5::Widgets 
    Qt5::Core 
    Qt5::Gui
    Qt5::Sql
    qglobalshotcut
)

add_executable(everything_linux src/main.cpp)
target_link_libraries(everything_linux qmainwindow)


file(GLOB_RECURSE cpp_files everything_updatedb/*.h everything_updatedb/*.c)
add_executable(everything_updatedb ${cpp_files})
target_link_libraries(everything_updatedb ${SQLITE3_LIBRARIES})

# add_executable(everything_updatedb ${cpp_files})