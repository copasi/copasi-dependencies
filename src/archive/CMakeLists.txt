cmake_minimum_required (VERSION 3.10)
project (copasi-dependencies-archive VERSION 1.0.0)

include(GNUInstallDirs)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "COPASI DEPENDENCIES contain required static libraries to build COPASI.")
set(CPACK_PACKAGE_NAME "copasi-dependencies")
set(CPACK_PACKAGE_VENDOR "copasi.org")
set(CPACK_PACKAGE_CONTACT "COPASI Support <support@copasi.org>")
set(CPACK_PACKAGE_DIRECTORY ${CMAKE_INSTALL_PREFIX})
set(CPACK_GENERATOR "ZIP")

include(CPack)

install(DIRECTORY ${CMAKE_INSTALL_FULL_INCLUDEDIR} ${CMAKE_INSTALL_FULL_LIBDIR} ${CMAKE_INSTALL_FULL_DATAROOTDIR} DESTINATION .)
