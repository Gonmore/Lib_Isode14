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

/*! \file x400_ms_async.h
    \brief X400 MS (P7) Interface asynchronous event handler

    This function is called when using the X400 MS API in asynchronous mode,
    to handle any events which may have occurred.

    When a client application opens an asynchronous-mode connection to the
    Message Store, it registers a set of callback handlers for completion
    of the various MS operations (Bind, List, Fetch, Submit etc), along with
    an optional pair of 'Manage' and 'Unmanage' handlers. 

    When X400ms API functions are subsequently called, instead of waiting 
    for the operation to complete and returning a result (i.e. synchronous 
    mode), the API functions send off the operation invokation to the Message 
    Store and immediately return an error code 
    indicating that the application needs to wait for the file descriptor 
    associated with the session object to become readable (e.g. when a 
    result has arrived from the server). When this happens (detected via
    a call to Select() on Unix), the client application should call
    X400msProcessEvent to handle the events which have occurred. Typically 
    this will mean that the operation result PDU is read and decoded, and
    then the appropriate callback function for the operation is called.
    If registered, the 'Manage' and 'Unmanage' callbacks are called to 
    indicate when individual file descriptors need to be monitored for 
    read, write or error events, and when they no longer need to be monitored.
    The use of these callback functions is particularly useful when a single
    event loop is handling multiple associations (i.e. multiple session 
    objects).
 \{
*/

/*!
    \example x400_msasync.c
    This is an example program which uses the X400 MS asynchronous interface to
    send, list, receive and delete messages in the Message Store.
*/


#ifndef _X400_MS_ASYNC_H
#define _X400_MS_ASYNC_H

#include "cdecl.h"

/*! \defgroup X400msAsync X400 MS (P7) Asynchronous API
   Description of the function calls for the P7 (X.400 UA) asynchronous interface.
 \{
*/

/*!
    \brief Process outstanding read, write and error events on the
    specified set of file descriptors. Registered callback handlers 
    will be invoked to pass the result of these events back to the 
    client application.
    \param num_fds [in] Highest numbered file descriptor in any set, plus 1
    \param read_fds [in] Set of file descriptors for read events
    \param write_fds [in] Set of file descriptors for write events
    \param error_fds [in] Set of file descriptors for exception events
    \return zero on success or non-zero error code
*/
X400MS_CDECL int X400msProcessEvent(int num_fds,
				    fd_set *read_fds, 
				    fd_set *write_fds,
				    fd_set *error_fds);
/*!
  \}
  */

#endif /* _X400_MS_ASYNC_H */
/*!
  \}
  */
