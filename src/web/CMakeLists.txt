add_custom_target(WebBuild ALL)

if (CMAKE_BUILD_TYPE MATCHES Release)
	add_custom_command(TARGET WebBuild
		COMMAND npm install
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

add_custom_command(TARGET WebBuild
	COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/PageRoot/www
	COMMAND node_modules/.bin/gulp build
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

install(DIRECTORY ${CMAKE_SOURCE_DIR}/PageRoot/
		DESTINATION ${CMAKE_INSTALL_PREFIX}/share/dman/PageRoot/)