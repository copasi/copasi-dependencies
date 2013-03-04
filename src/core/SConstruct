import glob
import sys

THREAD_LIB = 'pthread'
FRAMEWORKS = []
buildArchs = ''
compileFlags = ''
bzipLib = ''

if sys.platform in ['win32']:
	sbw_target = ' -DWIN32'
elif sys.platform in ['darwin']:
	sbw_target = ' -DDARWIN -DLINUX '
	if ARGUMENTS.get('UNIVERSAL') == 'no':
		# make sure we compile little or big endiannes correctly
		if sys.byteorder in ['little']:
			sbw_target  = sbw_target + " -D__LITTLE_ENDIAN__"
		else:
			sbw_target  = sbw_target + " -D__BIG_ENDIAN__"
	else:
		buildArchs = '  -arch i386 -arch ppc '

else:
	sbw_target = ' -DLINUX'

#if sys.byteorder in ['little']:
#	sbw_target  = sbw_target + " -D__LITTLE_ENDIAN__"
#else:
#	sbw_target  = sbw_target + " -D__BIG_ENDIAN__"

if ARGUMENTS == {}:
	print 'building SBW RELEASE'
	compileFlags 	= compileFlags + '-D_GNU_SOURCE -g -Wall -O2 ' + sbw_target
else:
	print 'building SBW DEBUG'
	compileFlags 	= compileFlags + '-D_GNU_SOURCE -g -Wall ' + sbw_target + ' -D_DEBUG'




srcSBW		= Split( glob.glob("./SBWCore/*.cpp"  ) )
srcBroker 	= Split( glob.glob("./SBWBroker/*.cpp") )
include 	= [ "./include/SBW", "./include", "/usr/local/include" ]
libSBW 		= [ THREAD_LIB ]
libBroker	= [ "SBW-static", "xml2", THREAD_LIB]
libPath 	= [ "./lib" ]

env 		= Environment( 
	CXX			= "g++", 
	CCFLAGS		= compileFlags, 
	CPPPATH		= include, 
	FRAMEWORKS	= FRAMEWORKS, 
	LINKFLAGS		= buildArchs
)

env.Append( CCFLAGS = buildArchs )


env.SharedLibrary( target = "./lib/SBW", source = srcSBW,LIBS = libSBW, LIBPATH = libPath )
env.Library( target = "./lib/SBW-static", source = srcSBW,LIBS = libSBW, LIBPATH = libPath )
env.Program( target = "./bin/Broker", source = srcBroker,LIBS = libBroker, LIBPATH = libPath )
