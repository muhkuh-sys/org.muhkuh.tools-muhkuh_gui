CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT("muhkuh_gui")

# This is the list of the wxwidgets modules required for the application.
FIND_PACKAGE(wxWidgets 3.0 REQUIRED html adv aui net xml core base)
INCLUDE(${wxWidgets_USE_FILE})


#----------------------------------------------------------------------------
#
# Build the muhkuh GUI.
#

# Set the windows specific sources. This will be empty on other platforms.
IF(WIN32)
	CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/templates/muhkuh.rc ${CMAKE_CURRENT_BINARY_DIR}/muhkuh.rc @ONLY)
	
	SET(VAR_muhkuh_app_platform_sources
		${CMAKE_CURRENT_BINARY_DIR}/muhkuh.rc
	)
ENDIF(WIN32)


SET(VAR_muhkuh_app_sources
	growbuffer.cpp
	license.cpp
	muhkuh_aboutDialog.cpp
	muhkuh_app.cpp
	muhkuh_available_languages.cpp
	muhkuh_brokenPluginDialog.cpp
	muhkuh_configDialog.cpp
	muhkuh_config_data.cpp
	muhkuh_config_reposEntryDialog.cpp
	muhkuh_copy_process.cpp
	muhkuh_dirlistbox.cpp
	muhkuh_flags.cpp
	muhkuh_icons.cpp
	muhkuh_lua.cpp
	muhkuh_mainFrame.cpp
	muhkuh_plugin.cpp
	muhkuh_plugin_manager.cpp
	muhkuh_repository.cpp
	muhkuh_repository_manager.cpp
	muhkuh_split_testdescription.cpp
	muhkuh_wrap_xml.cpp
	readFsFile.cpp
)

SET(VAR_muhkuh_app_headers
	growbuffer.h
	license.h
	muhkuh_aboutDialog.h
	muhkuh_app.h
	muhkuh_available_languages.h
	muhkuh_configDialog.h
	muhkuh_config_data.h
	muhkuh_config_reposEntryDialog.h
	muhkuh_copy_process.h
	muhkuh_dirlistbox.h
	muhkuh_flags.h
	muhkuh_icons.h
	muhkuh_id.h
	muhkuh_lua.h
	muhkuh_mainFrame.h
	muhkuh_plugin.h
	muhkuh_plugin_manager.h
	muhkuh_repository.h
	muhkuh_repository_manager.h
	muhkuh_split_testdescription.h
	muhkuh_testTreeItemData.h
	muhkuh_version.h
	muhkuh_wrap_xml.h
	readFsFile.h
)


CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/templates/muhkuh_version.h ${CMAKE_CURRENT_BINARY_DIR}/version/muhkuh_version.h)


# Build the executable.
ADD_EXECUTABLE(TARGET_muhkuh_gui WIN32
	${VAR_muhkuh_app_sources}
	${VAR_muhkuh_app_headers}
	${VAR_muhkuh_app_platform_sources}
)

TARGET_INCLUDE_DIRECTORIES(TARGET_muhkuh_gui
                           PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/version ${LUA_INCLUDE_DIR})
TARGET_COMPILE_DEFINITIONS(TARGET_muhkuh_gui
                           PRIVATE ${wxWidgets_DEFINITIONS})

TARGET_LINK_LIBRARIES(TARGET_muhkuh_gui
                      PRIVATE ${wxWidgets_LIBRARIES} ${LUA_LIBRARIES})

# Set the name of the output file to "muhkuh".
SET_TARGET_PROPERTIES(TARGET_muhkuh_gui PROPERTIES OUTPUT_NAME "muhkuh")

# On mingw link all compiler libraries static.
IF((${CMAKE_SYSTEM_NAME} STREQUAL "Windows") AND (${CMAKE_COMPILER_IS_GNUCC}))
	SET_PROPERTY(TARGET TARGET_muhkuh_gui PROPERTY LINK_FLAGS "-static -static-libgcc -static-libstdc++")
ENDIF((${CMAKE_SYSTEM_NAME} STREQUAL "Windows") AND (${CMAKE_COMPILER_IS_GNUCC}))

# Add tests for this module.
IF((${CMAKE_SYSTEM_NAME} STREQUAL "Windows") AND (${CMAKE_COMPILER_IS_GNUCC}))
	# Here are the MinGW specific tests.
	ADD_TEST(NAME romloader_uart_MinGW_DLL_dependencies
	         COMMAND "${PYTHON_EXECUTABLE}" ${CMAKE_HOME_DIRECTORY}/cmake/tests/mingw_dll_dependencies.py $<TARGET_FILE:TARGET_muhkuh_gui>)
ENDIF((${CMAKE_SYSTEM_NAME} STREQUAL "Windows") AND (${CMAKE_COMPILER_IS_GNUCC}))



# Copy the application.
INSTALL(TARGETS TARGET_muhkuh_gui
        DESTINATION ${INSTALL_DIR_EXECUTABLES})
# Copy the tips file.
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/muhkuh_tips.txt
        DESTINATION ${INSTALL_DIR_EXECUTABLES})
