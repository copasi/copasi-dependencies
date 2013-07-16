####################################################################
#
# SBML Rendering package 
#
# $Author Lucian Smith$
# $Id$
# $HeadURL$
#

if (ENABLE_RENDER)
#        add_subdirectory(c/render)
        add_subdirectory(c++/render)

#        if(WITH_CSHARP)
#          add_subdirectory(csharp/render)
#        endif(WITH_CSHARP)

        if(WITH_JAVA)
          add_subdirectory(java/render)
        endif(WITH_JAVA)
endif(ENABLE_RENDER)
