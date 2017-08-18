
#ifndef RAPTOR_CONFIG_H
#define RAPTOR_CONFIG_H

#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif

/* getopt is not in standard win32 C library - define if we have it */
#cmakedefine HAVE_GETOPT_H 1

#cmakedefine HAVE_STDLIB_H 1

/* For using expat on win32 */
#define RAPTOR_XML_EXPAT 1
#cmakedefine HAVE_EXPAT_H 1

#cmakedefine HAVE_STRICMP 1
#cmakedefine HAVE_STRCASECMP 1
#cmakedefine HAVE_UNISTD_H 1

#if HAVE_UNISTD_H
#include <unistd.h>
#endif 

#define XMLCALL

/*#define HAVE_C99_VSNPRINTF */

/* for access() which is POSIX but doesn't seem to have the defines in VC */
#ifndef R_OK
#define R_OK 4
#endif

/* 
 * Defines that come from config.h
 */

/* Release version as a decimal */
#define RAPTOR_VERSION_DECIMAL 10419

/* Major version number */
#define RAPTOR_VERSION_MAJOR 1

/* Minor version number */
#define RAPTOR_VERSION_MINOR 4

/* Release version number */
#define RAPTOR_VERSION_RELEASE 19

/* Version number of package */
#define VERSION "1.4.19"


#ifdef RAPTOR_XML_LIBXML
/* RSS parser needs libxml 2.5.x+ */
#define RAPTOR_PARSER_RSS 1
#else
#undef RAPTOR_PARSER_RSS
#endif

#define RAPTOR_PARSER_GUESS 1
//#define RAPTOR_PARSER_GRDDL 1
//#define RAPTOR_PARSER_N3 1
#define RAPTOR_PARSER_TURTLE 1
#define RAPTOR_PARSER_NTRIPLES 1
#define RAPTOR_PARSER_RDFXML 1

#define RAPTOR_SERIALIZER_ATOM 1
#define RAPTOR_SERIALIZER_RSS_1_0 1
#define RAPTOR_SERIALIZER_RDFXML 1
#define RAPTOR_SERIALIZER_RDFXML_ABBREV 1
#define RAPTOR_SERIALIZER_NTRIPLES 1

//#define RAPTOR_WWW_LIBCURL 1

#include <memory.h>

/* bison: output uses ERROR in an enum which breaks if this is defined */
#ifdef ERROR
#undef ERROR
#endif

/* flex: const is available */
#define YY_USE_CONST

#undef RAPTOR_INLINE
#define RAPTOR_INLINE __inline

/* The size of a `unsigned char', as computed by sizeof. */
#define SIZEOF_UNSIGNED_CHAR 1

/* The size of a `unsigned short', as computed by sizeof. */
#define SIZEOF_UNSIGNED_SHORT 2

/* The size of a `unsigned int', as computed by sizeof. */
#define SIZEOF_UNSIGNED_INT 4

/* The size of a `unsigned long', as computed by sizeof. */
#define SIZEOF_UNSIGNED_LONG 4

/* The size of a `unsigned long long', as computed by sizeof. */
#define SIZEOF_UNSIGNED_LONG_LONG 8


#ifdef __cplusplus
}
#endif

#endif
