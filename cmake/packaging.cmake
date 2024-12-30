message(STATUS "PACKAGING setup ...")

# distro and package dependencies
# =================================

set(distro "debian")
message(STATUS "Build packages for distro: " ${distro})

set(project_version 1.0.0)
message(STATUS "Project version: ${project_version}")

# The binary package depends on runtime dependencies only.
# The dev package depends on the binary package and the public build dependencies.
set(dependencies_runtime "")
set(dependencies_build_public "")

set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/" CACHE PATH "installation root for packages when no DESTDIR set")
set(CPACK_INSTALL_PREFIX "/usr/" CACHE PATH "installation for packages when DESTDIR set")

set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_CMAKE_GENERATOR ${CMAKE_GENERATOR})
set(CPACK_GENERATOR "DEB")

set(CPACK_PACKAGE_CONTACT "Simon Massa <https://github.com/simchri>")
set(CPACK_PACKAGE_VERSION ${project_version})
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A simple cli-ui calculator.")


# Packages
# =================================

set(BIN_PACKAGE_COMPONENT_NAME bin)
string(TOUPPER ${BIN_PACKAGE_COMPONENT_NAME} UPPER_COMPONENT_NAME)
set(CPACK_DEBIAN_${UPPER_COMPONENT_NAME}_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
set(MY_BIN_PACKAGE_NAME ${CPACK_DEBIAN_${UPPER_COMPONENT_NAME}_PACKAGE_NAME})
set(CPACK_DEBIAN_${UPPER_COMPONENT_NAME}_FILE_NAME "DEB-DEFAULT")
set(CPACK_DEBIAN_${UPPER_COMPONENT_NAME}_PACKAGE_DEPENDS "${dependencies_runtime}" )
set(CPACK_DEBIAN_${UPPER_COMPONENT_NAME}_DESCRIPTION "Binaries for ${CMAKE_PROJECT_NAME}")

message(STATUS "Package: ${CPACK_DEBIAN_${UPPER_COMPONENT_NAME}_PACKAGE_NAME}")
message(STATUS "... associated component (UPPERCASE): ${UPPER_COMPONENT_NAME}")
message(STATUS "... CPACK_DEBIAN_${UPPER_COMPONENT_NAME}_PACKAGE_DEPENDS: ${CPACK_DEBIAN_${UPPER_COMPONENT_NAME}_PACKAGE_DEPENDS}")

# Misc
# =================================

# add /bin & /lib to install paths (according to GNU libdir convention)
include(GNUInstallDirs) # for CMAKE_INSTALL_LIBDIR, CMAKE_INSTALL_BINDIR, ...
file(RELATIVE_PATH relDir
        ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}
        ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}
)
set(CMAKE_INSTALL_RPATH $ORIGIN $ORIGIN/${relDir}) # called RPATH but sets RUNPATH

include(CPack)
