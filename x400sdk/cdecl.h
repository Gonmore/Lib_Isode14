/*  Copyright (c) 2003-2008, Isode Limited, London, England.
 *  All rights reserved.
 *                                                                       
 *  Acquisition and use of this software and related materials for any      
 *  purpose requires a written licence agreement from Isode Limited,
 *  or a written licence from an organisation licenced by Isode Limited
 *  to grant such a licence.
 *
 */

/* 
 *
 * 14.4v12-0
 */

#ifndef _X400_CDECL_H
#define _X400_CDECL_H

#ifdef __cplusplus
 #define X400_CDECL extern "C" 
 #define X400_CSPEC extern "C"
 #define X400_CTYPE "C"

#else

 #define X400_CSPEC
 #define X400_CDECL extern
 #define X400_CTYPE

#endif

#ifdef _WIN32

/* Default to DLL import */
 #define X400MT_CDECL X400_CSPEC __declspec(dllimport)
 #define X400MS_CDECL X400_CSPEC __declspec(dllimport)
 #define X400COMMON_CDECL X400_CSPEC __declspec(dllimport)

 #ifdef LIBX400MT
   /* Defined if building the library */
   #undef  X400MT_CDECL
   #define X400MT_CDECL X400_CSPEC __declspec(dllexport)
 #endif

 #ifdef LIBX400MS
   /* Defined if building the library */
   #undef  X400MS_CDECL
   #define X400MS_CDECL X400_CSPEC __declspec(dllexport)
 #endif

 #ifdef LIBX400COMMON
   /* Defined if building the library */
   #undef  X400COMMON_CDECL
   #define X400COMMON_CDECL X400_CSPEC __declspec(dllexport)
 #endif

#else
 /* Not on Windows - use standard extern declarations */
 #define X400MT_CDECL extern X400_CTYPE
 #define X400MS_CDECL extern X400_CTYPE
 #define X400COMMON_CDECL extern X400_CTYPE

#endif

#endif /* _X400_CDECL_H */
