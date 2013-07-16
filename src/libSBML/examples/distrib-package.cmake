####################################################################
#
# SBML Distributions package 
#
# $Author Lucian Smith$
# $Id$
# $HeadURL$
#

if (ENABLE_DISTRIB)
#        add_subdirectory(c/distrib)
#        add_subdirectory(c++/distrib)

        if(WITH_CSHARP)
#          add_subdirectory(csharp/distrib)
        endif(WITH_CSHARP)

        if(WITH_JAVA)
#          add_subdirectory(java/distrib)
        endif(WITH_JAVA)
endif(ENABLE_DISTRIB)
