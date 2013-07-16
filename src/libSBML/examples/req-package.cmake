####################################################################
#
# SBML Required Elements package 
#
# $Author Lucian Smith$
# $Id$
# $HeadURL$
#

if (ENABLE_REQUIREDELEMENTS)
#        add_subdirectory(c/req)
        add_subdirectory(c++/req)

        if(WITH_CSHARP)
#          add_subdirectory(csharp/req)
        endif(WITH_CSHARP)

        if(WITH_JAVA)
#          add_subdirectory(java/req)
        endif(WITH_JAVA)
endif(ENABLE_REQUIREDELEMENTS)
