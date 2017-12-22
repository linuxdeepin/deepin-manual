if (CMAKE_BUILD_TYPE MATCHES Release)
	add_custom_target(WebBuild ALL
		COMMAND npm install
		COMMAND node_modules/.bin/gulp build
		COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/PageRoot/www
		COMMAND cp -fv -r css index.html index.js ${CMAKE_SOURCE_DIR}/PageRoot/www
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

install(DIRECTORY ${CMAKE_SOURCE_DIR}/PageRoot/
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/dman/PageRoot/)