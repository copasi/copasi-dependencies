####################################################################
#
# SBML Groups package 
#
# $Author Lucian Smith$
# $Id$
# $HeadURL$
#

if (ENABLE_GROUPS)
#        add_subdirectory(c/groups)
        add_subdirectory(c++/groups)

        if(WITH_CSHARP)
          add_subdirectory(csharp/groups)
        endif(WITH_CSHARP)

        if(WITH_JAVA)
          add_subdirectory(java/groups)
        endif(WITH_JAVA)
endif(ENABLE_GROUPS)
