####################################################################
#
# SBML Spatial Processes package 
#
# $Author Lucian Smith$
# $Id$
# $HeadURL$
#

if (ENABLE_SPATIAL)
#        add_subdirectory(c/spatial)
        add_subdirectory(c++/spatial)

        if(WITH_CSHARP)
#          add_subdirectory(csharp/spatial)
        endif(WITH_CSHARP)

        if(WITH_JAVA)
#          add_subdirectory(java/spatial)
        endif(WITH_JAVA)
endif(ENABLE_SPATIAL)
