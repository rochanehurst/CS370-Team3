cmake_minimum_required(VERSION 3.16)

project(370-Project VERSION 0.1 LANGUAGES CXX)

enable_testing()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Test
    #following required for map to work
    Quick
    QuickWidgets
    Positioning
    Location
    Network
) #edited this. important!

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    createclass.ui
    createclass.h
    createclass.cpp
    class_info_unit.ui
    class_info_unit.h
    class_info_unit.cpp
    resources.qrc
    search_window.ui
    search_window.h
    search_window.cpp
    class_result.h class_result.cpp class_result.ui
    savestuff.cpp savestuff.h
    schedulelogic.h schedulelogic.cpp
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(370-Project
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        class_result.h class_result.cpp class_result.ui
        savestuff.cpp savestuff.h
        apimap.h apimap.cpp

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET 370-Project APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(370-Project SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(370-Project
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(370-Project PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    #following required for map to work    
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::QuickWidgets
    Qt${QT_VERSION_MAJOR}::Positioning
    Qt${QT_VERSION_MAJOR}::Location
    Qt${QT_VERSION_MAJOR}::Network
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.370-Project)
endif()
set_target_properties(370-Project PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS 370-Project
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(370-Project)
endif()



#  Unit Test Target

#test01
add_executable(TC01_CheckBoxesInitialState #test executable name
    tests/TC01_CheckBoxesInitialState.cpp #test source file
    createclass.cpp #dependencies
    createclass.h
    class_info_unit.cpp
    class_info_unit.h
)
target_include_directories(TC01_CheckBoxesInitialState PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}) #include path for headers


target_link_libraries(TC01_CheckBoxesInitialState PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets #widget library for ui components
    Qt${QT_VERSION_MAJOR}::Test #test library for test framework
)
add_test(NAME TC01_CheckBoxesInitialState COMMAND TC01_CheckBoxesInitialState) #Register with CTest in case is configured


#test02
add_executable(TC02_AddClassToSchedule
    tests/TC02_AddClassToSchedule.cpp
    mainwindow.cpp mainwindow.h
    class_info_unit.cpp class_info_unit.h
    createclass.cpp createclass.h
    schedulelogic.cpp schedulelogic.h
    savestuff.cpp savestuff.h
    search_window.cpp search_window.h
    class_result.cpp class_result.h
)
target_include_directories(TC02_AddClassToSchedule PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(TC02_AddClassToSchedule PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Test
)
add_test(NAME TC02_AddClassToSchedule COMMAND TC02_AddClassToSchedule)


#test03
add_executable(TC03_ScheduleConflictDetection
    tests/TC03_ScheduleConflictDetection.cpp
    schedulelogic.cpp schedulelogic.h
    createclass.cpp createclass.h
    class_info_unit.cpp class_info_unit.h
)
target_include_directories(TC03_ScheduleConflictDetection PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(TC03_ScheduleConflictDetection PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Test
)
add_test(NAME TC03_ScheduleConflictDetection COMMAND TC03_ScheduleConflictDetection)

#test04
add_executable(TC04_TimeComparison
    tests/TC04_TimeComparison.cpp
    createclass.cpp createclass.h
)
target_include_directories(TC04_TimeComparison PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(TC04_TimeComparison PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Test
)
add_test(NAME TC04_TimeComparison COMMAND TC04_TimeComparison)

#test05
add_executable(TC05_ClearAllClasses
    tests/TC05_ClearAllClasses.cpp
    createclass.cpp createclass.h
)
target_include_directories(TC05_ClearAllClasses PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(TC05_ClearAllClasses PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Test
)
add_test(NAME TC05_ClearAllClasses COMMAND TC05_ClearAllClasses)

#Test06
add_executable(TC06_UILoadsClassData
    tests/TC06_UILoadsClassData.cpp
    createclass.cpp createclass.h
)
target_include_directories(TC06_UILoadsClassData PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(TC06_UILoadsClassData PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Test
)
add_test(NAME TC06_UILoadsClassData COMMAND TC06_UILoadsClassData)

#Test07
add_executable(TC07_SaveScheduleToFile
    tests/TC07_SaveScheduleToFile.cpp
    savestuff.cpp savestuff.h
    createclass.cpp createclass.h
)
target_include_directories(TC07_SaveScheduleToFile PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(TC07_SaveScheduleToFile PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Test
)
add_test(NAME TC07_SaveScheduleToFile COMMAND TC07_SaveScheduleToFile)

# Test08
add_executable(TC08_ScheduleLogicValidation
    tests/TC08_ScheduleLogicValidation.cpp
    schedulelogic.cpp schedulelogic.h
    createclass.cpp createclass.h
    class_info_unit.cpp class_info_unit.h
)
target_include_directories(TC08_ScheduleLogicValidation PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(TC08_ScheduleLogicValidation PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Test
)
add_test(NAME TC08_ScheduleLogicValidation COMMAND TC08_ScheduleLogicValidation)

# Test09
add_executable(TC09_ClassInfoFrameData
    tests/TC09_ClassInfoFrameData.cpp
    class_info_unit.cpp class_info_unit.h
    createclass.cpp createclass.h
)
target_include_directories(TC09_ClassInfoFrameData PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(TC09_ClassInfoFrameData PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Test
)
add_test(NAME TC09_ClassInfoFrameData COMMAND TC09_ClassInfoFrameData)

# Test10
add_executable(TC10_SaveFeatureFileCheck
    tests/TC10_SaveFeatureFileCheck.cpp
    savestuff.cpp savestuff.h
    createclass.cpp createclass.h
)
target_include_directories(TC10_SaveFeatureFileCheck PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(TC10_SaveFeatureFileCheck PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Test
)
add_test(NAME TC10_SaveFeatureFileCheck COMMAND TC10_SaveFeatureFileCheck)
