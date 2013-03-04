C++ Broker 
==========

The C++ Broker is a port from the original SBW Broker (written in JAVA) to C++. 
The current version implements all the functionality for the local side. Meaning 
if you will just use the Broker on a single machine you should be fine using the 
C++ Broker. On the other side, if you are currently using the remote capabilities 
of the SBW Broker (that is you connect to some modules on a non-local host) you 
should not try to use the C++ version. 

This Version has been tested on WindowsXP Professional and Fedora 2. To compile it 
under Linux just use the SConstruct file in the main directory. (You will need to 
have scons installed on your system though. For details see  http://scons.sf.net). 

This Version has now been tested successful under Mac OSX (10.3). To compile it under 
OSX make sure that the SConstruct file in the main directory uses the flags -DLINUX and
-DDARWIN. With that it should compile without any problems. (Again you will need to 
have scons installed on your system. For details see http://scons.sf.net) 
_________
December, 2004

