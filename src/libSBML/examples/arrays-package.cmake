####################################################################
#
# SBML Arrays package 
#
# $Author Lucian Smith$
# $Id$
# $HeadURL$
#

if (ENABLE_ARRAYS)
#        add_subdirectory(c/arrays)
#        add_subdirectory(c++/arrays)

        if(WITH_CSHARP)
          add_subdirectory(csharp/arrays)
        endif(WITH_CSHARP)

        if(WITH_JAVA)
          add_subdirectory(java/arrays)
        endif(WITH_JAVA)
endif(ENABLE_ARRAYS)
