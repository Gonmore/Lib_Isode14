/*  Copyright (c) 2003-2009, Isode Limited, London, England.
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

#ifndef _X400_MSAPI_H
#define _X400_MSAPI_H

#include "cdecl.h"
#include "x400_api.h"

/*! \mainpage Isode X.400 MS Client API Overview
    <b>Version 14.4</b><br>
    <b>Copyright (c) 2005-2009, Isode Limited, London, England</b>
    
    \section intro Introduction
    
    This document is a specification of the Isode X.400 MS Client 
    Library API. This API allows client programs to connect 
    to the Isode X.400 Message Store, using the P7 protocol, or directly to 
    the Isode X.400 MTA, using the P3 protocol. The API is suitable for 
    building the functionality of a User Agent or an Access Unit.

    P3 and P7 client applications bind as a User Agent (to the MTA and 
    Message Store respectively). Submission work in a similar way. However 
    P7 is a retrieval protocol, whereas P3 is a delivery protocol.

    P7 applications bind as a Message Store user to the Message Store. 
    The P7 application can retrieve messages, which can be deleted from 
    this user's mailbox. Only messages for the bound user can be retrieved, 
    deleted etc. When submitting messages, the originator of the message 
    must be the bound user.

    P3 applications bind to the P3 channel of the MTA. They bind to a P3 
    passive channel (ie a channel which has no "program" value in its 
    configuration). Although the P3 client authenticates as a Message 
    Store user (using the user's MTS password), this means that all 
    messages which are routed for delivery to the P3 passive channel are 
    delivered when a P3 client binds to the P3 server of the MTA. P3 
    clients are responsible for reliable storage of the messages. 
    P3 clients should only unbind after an X400msWait() operation has returned 
    indicating that there are no further messages for delivery by the MTA;
    ie all queued messages must be delivered. If this is not done, the MTA
    will conclude that the p3channel has failed, and will impose a delay
    before the delivery of the remaining messages on the channel can 
    be attempted.

    \section start How to Read this API

    If you wish to "Jump right in" then the best thing to do is:<br>
    
    -# Look at the \ref x400_mstutorial.c "x400_mstutorial.c" example program.
    It is a very simple streamlined program that demonstrates the basic
    approach of manipulating API objects.
    To keep things simple this program demonstrates how to create and send
    a message using the P7 protocol.
    -# Look at the \ref x400_mssend.c "x400_mssend.c" and
    \ref x400_msrcv.c "x400_msrcv.c" example programs.
    These programs give a more detailed example of how the API should be used.
    -# Look at the \ref ref_table "reference table" which matches
    attributes described within the standards to the objects / attributes
    used within the API.
    -# Have a good read of the API manual.

    \subsection Important Standards.
    It is highly recommended you are familiar with the following standards:
    - X.402
    - X.411
    - X.413
    - X.420
    
    - STANAG 4406 Annex A
    
    
    - ISO/IEC 10021
    - ISO/IEC 10022
    - ISO/IEC 10024
    - ISO/IEC 10025
    - ISO/IEC 10027
    
    
    \section bindings Language Bindings

    The X.400 MS API is available in C, Tcl and Java bindings.
    
    \subsection x400relationship Relationship of MS and Pure X.400 Interfaces
    
    Top-level message objects are built, submitted, retrieved and 
    examined using the X400 MS Client API. In particular, functions
    specific to X.400 MS message objects are used to get and set 
    attribute values within messages. For example, X400msMsgAddStrParam()
    would be used to set a string parameter value in such a message.
    
    There is one specific situation where protocol-independent functions
    need to be used to manipulate messages: this is when dealing with
    forwarded messages (i.e. messages which have been included as 
    bodyparts of "top-level" X.400 MS messages). In this case, the
    equivalent "pure X.400" functions must be used - X400MsgAddStrParam() 
    in the equivalent case to the one described above.
    
    \subsection cbindings C Bindings

    The example code is in:
    \arg Unix: /opt/isode/share/x400sdk/example/c
    \arg Windows: c:\\Isode\\share\\x400sdk\\example\\c

    \subsection tclbindings Tcl Bindings

    The example code is in

    \arg Unix: /opt/isode/share/x400sdk/example/tcl
    \arg Windows: c:\\Isode\\share\\x400sdk\\example\\tcl

    \subsection javabindings Java Bindings
    
    The example code is in

    \arg Unix: /opt/isode/share/x400sdk/example/java
    \arg Windows: c:\\Isode\\share\\x400sdk\\example\\java

    \subsection apistyle Style of API

    The APIs use an object-oriented style, and rely on arguments which are

    \arg integers (either explicitly or as manifest constants)
    \arg strings (whose length is passed as an integer)
    \arg opaque objects (on which the API functions perform operations)

    \section freeback Feedback
    Please send any comments on the API to support\@isode.com.

*/
/*!
    \example examples/x400_mssend.c
    This is an example program which submits a message either indirectly via 
    the P7 Message Store or directly to the MTA's P3 responder.
*/
/*!
    \example examples/x400_msrcv.c
    This is an example program which can receive a message either from a 
    P7 Message Store or directly from the MTA's P3 channel.
*/
/*!
    \example examples/x400_mssend_sign.c
    This is an example program which submits a message either indirectly via 
    the P7 Message Store or directly to the MTA's P3 responder.
    This program will attempt to sign a message using a security
    environment be present in SHAREDIR/x509. This is installed by default.
*/
/*!
    \example examples/x400_msrcv_sign.c
    This is an example program which can receive a message either from a 
    P7 Message Store or directly from the MTA's P3 channel.
    This program will attempt to verify a signature using a security
    environment be present in SHAREDIR/x509. This is installed by default.
*/
/*!
    \example examples/x400_msraa.c
    This is an example program which shows you how to register and deregister
    autoactions with the Message Store. Supported auto-actions include 
    auto-forward (to automatically forward every received message), and 
    auto-alert (to alert the UA of a new message).
*/
/*!
    \example examples/x400_mslist.c
    This is an example program which shows you how to list messages in the 
    Message Store. A summary is printed for each message in the mailbox.
*/

/*! \file x400_msapi.h
    \brief X400 MA/MS (P3/P7) Interface

    This is an API for interfacing either to an X400 MTA with P3 or
    an X400 MS with P7. It is suitable for building the functionality
    of a user agent or access unit.

    It provides mechanisms for building and examining X400 message, probes
    and reports. It also provides mechanisms for submitting such objects
    into the X400, and receiving such objects on delivery.

*/

/*! \defgroup X400ms X400 MS (P3/P7) API
  The function calls which make up the P3 or P7 (X.400 UA) interface.
 \{
*/

/*!
    \brief Obtain a string describing the meaning of the given error code
    \param error [in] error code
    \return Pointer to NUL terminated string for error
*/

X400MS_CDECL const char *X400msError (int error);

struct X400msSession;
struct X400msMessage;
struct X400msListResult;


/*!
    \brief A callback which will be invoked when a P7 bind completes 
    asynchronously, whether the completion is successful or unsuccessful.
    \param session [in] Session to which callback applies
    \param errorcode [in] Indicates success or failure of connection attempt
    \param alert [in] Set for alert indication
*/
typedef void X400msConnEstablishedCb(struct X400msSession *session,
				     int errorcode,
				     int alert);

/*!
    \brief A callback which will be invoked when a P7 connection is lost
    \param session [in] Session to which callback applies
    \param reason_code [in] Reason for connection loss
    \param diagnostic [in] Diagnostic text
*/
typedef void X400msConnDroppedCb(struct X400msSession *session,
				 int reason_code,
				 char *diagnostic);

/*!
    \brief A callback which will be invoked when a P7 Submission operation
    completes asynchronously, whether the completion is successful or 
    unsuccessful.
    \param session [in] Session to which callback applies
    \param message [in] On success, gives message submission result attributes
    \param errorcode [in] Indicates success or failure of submission
*/
typedef void X400msMsgSubmittedCb(struct X400msSession *session,
				  struct X400msMessage *message,
				  int errorcode);
		
/*!
    \brief A callback which will be invoked when a P7 Fetch operation
    completes asynchronously, whether the completion is successful or 
    unsuccessful.
    \param session [in] Session to which callback applies
    \param message [in] On success, message which has been fetched
    \param type [in] On success, message type
    \param seqno [in] On success, message sequence number
    \param errorcode [in] Indicates success or failure of fetch operation
*/
typedef void X400msMsgFetchedCb(struct X400msSession *session,
				struct X400msMessage *message,
				int type,
				int seqno,
				int errorcode);

/*!
    \brief A callback which will be invoked when a P7 Delete operation
    completes asynchronously, whether the completion is successful or 
    unsuccessful.
    \param session [in] Session to which callback applies
    \param errorcode [in] Indicates success or failure of delete operation
*/
typedef void X400msMsgDeletedCb(struct X400msSession *session,
				int seqno,
				int errorcode);

/*!
    \brief A callback which will be invoked when a P7 Summarize operation
    (invoked via X400msWait) completes asynchronously, whether the 
    completion is successful or unsuccessful.
    \param session [in] Session to which callback applies
    \param num_messages_waiting [in] Number of messages waiting
    \param errorcode [in] Indicates success or failure of summarize operation
*/
typedef void X400msMsgWaitingCb(struct X400msSession *session,
				int num_messages_waiting,
				int errorcode);

/*!
    \brief A callback which will be invoked when a P7 List operation
    completes asynchronously, whether the completion is successful or 
    unsuccessful.
    \param session [in] Session to which callback applies
    \param lrp [in] List result pointer on success
    \param errorcode [in] Indicates success or failure of list operation
*/
typedef void X400msListCb(struct X400msSession *session,
			  struct X400msListResult  *lrp,
			  int errorcode);

/*!
    \brief A callback which will be invoked when a P7 Register operation
    completes asynchronously, whether the completion is successful or 
    unsuccessful.
    \param session [in] Session to which callback applies
    \param errorcode [in] Indicates success or failure of register operation
*/
typedef void X400msRegisterCb(struct X400msSession *session,
			      int errorcode);

/*!
    \brief A callback which will be invoked when a P7 Alert invoke is
    received.
    \param session [in] Session to which callback applies
*/
typedef void X400msAlertCb(struct X400msSession *session);

/*! \defgroup sessiontype Session Types
  \{ */
#define X400_TYPE_MS         0
/*!< Connection is to an X.400 Message Store */

#define X400_TYPE_MTA_SUBMIT 1
/*!< Connection is to an X.400 MTA for submission only */

#define X400_TYPE_MTA_BOTH   2
/*!< Connection is to an X.400 MTA for delivery and submission */

/* \} */

/*! \defgroup eventtype Event Types
  \{ */
#define X400_EVENT_READ 1
/*!< Application should wait for read event */

#define X400_EVENT_WRITE 2
/*!< Application should wait for write event */

#define X400_EVENT_ERROR 4
/*!< Application should wait for error event */

#define X400_EVENT_ALL (X400_EVENT_READ | X400_EVENT_WRITE | X400_EVENT_ERROR)
/*!< Application should wait for all three types of event */

/* \} */

/*! \defgroup entryclass Message Store Entry Classes
  \{ */
#define MS_ENTRY_CLASS_STORED_MESSAGES 0
/*!< Messages which have been delivered into the Message Store */

#define MS_ENTRY_CLASS_SUBMITTED_MESSAGES 1
/*!< Messages which have been stored in the Message Store on submission */

/* \} */

/*!
    \brief A callback which will be invoked when a particular file descriptor
    either needs to be monitored for a given set of event types, or no 
    longer needs to be monitored for the specified event types.
    \param session [in] Session to which callback applies
    \param fd [in] File descriptor to which the callback applies
    \param event_mask [in] The set of events which need to be managed or 
    can now be ignored.
*/
typedef int X400msManageCb(struct X400msSession *session,
			   int fd,
			   int event_mask);
/*!
    \brief Initiate an asynchronous opening of a session to a Message 
    Store (P7)
    \param type [in] Connection type: X400_TYPE_MS, X400_TYPE_MTA_SUBMIT
                     or X400_TYPE_MTA_BOTH
    \param oraddr  [in] String encoding of OR-Address to use when binding
    \param dirname [in] String encoding of Directory name to use when binding
    \param credentials [in] String giving the credentials for the session
    \param pa [in] String encoding of MS or Presentation Address to contact.
    \param ret_psw [in] String giving the expected password from the MS or MTA
    \param conupcb [in] Function to be called when connection is open
    \param condowncb [in] Function to be called when connection is lost
    \param msgsubcb [in] Function to be called when submission completes
    \param msgfetchcb [in] Function to be called when fetch completes
    \param msgdelcb [in] Function to be called when delete completes
    \param msgwaitcb [in] Function to be called when wait completes
    \param listcb [in] Function to be called when list completes
    \param registercb [in] Function to be called when register completes
    \param alertcb [in] Function to be called when alert occurs
    \param managecb [in] Function to be called when an event type needs to be handled
    \param unmanagecb [in] Function to be called when an event type does not need to be handled
    \param spp [out] Pointer to receive pointer to opaque session data
    \return zero on success or non-zero error code

    Both the credentials and ret_psw strings are passwords, 
    for simple authentication. The ret_psw value (if non-NULL) specifies 
    the password we expect to get back from the MS or MTA to which we are 
    binding. The API will perform a check that the returned password matches
    the expected value and will return an error if they do not match.

    The function will return X400_E_WAIT_WRITE, indicating that the client
    application need to wait for the file descriptor associated with the
    X400msSession object to become writeable. At this point, 
    X400msProcessEvent needs to be called to handle the 'write' event (which
    actually indicates that the network-level connection has been established).
    X400msProcessEvent will then return X400_E_WAIT_READ, to indicate that 
    the application needs to wait for the file descriptor to become readable
    before calling X400msProcessEvent again. The application will need to
    call X400msProcessEvent multiple times before the association becomes
    open - at this point the "X400msConnEstablishedCb" callback will be
    invoked.
*/
X400MS_CDECL int X400msOpenAsync (int type,
				  const char *oraddr,
				  const char *dirname,
				  const char *credentials,
				  const char *pa,
				  const char *ret_psw,
				  X400msConnEstablishedCb *conupcb,
				  X400msConnDroppedCb *condowncb,
				  X400msMsgSubmittedCb *msgsubcb,
				  X400msMsgFetchedCb *msgfetchcb,
				  X400msMsgDeletedCb *msgdelcb,
				  X400msMsgWaitingCb *msgwaitcb,
				  X400msListCb *listcb,
				  X400msRegisterCb *registercb,
				  X400msAlertCb *alertcb,
				  X400msManageCb *managecb,
				  X400msManageCb *unmanagecb,
				  struct X400msSession **spp);

/*!
    \brief Initiate an asynchronous opening of a session to a Message 
    Store (P7) using an existing session object
    \param sp [in] Session to connect
    \param addr  [in] String encoding of OR-Address to use when binding
    \param dn [in] String encoding of Directory name to use when binding
    \param credentials [in] String giving the credentials for the session
    \param pa [in] String encoding of MS or Presentation Address to contact.
    \param ret_psw [in] String giving the expected password from the MS or MTA
    \param conupcb [in] Function to be called when connection is open
    \param condowncb [in] Function to be called when connection is lost
    \param msgsubcb [in] Function to be called when submission completes
    \param msgfetchcb [in] Function to be called when fetch completes
    \param msgdelcb [in] Function to be called when delete completes
    \param msgwaitcb [in] Function to be called when wait completes
    \param listcb [in] Function to be called when list completes
    \param registercb [in] Function to be called when register completes
    \param alertcb [in] Function to be called when alert occurs
    \param managecb [in] Function to be called when an event type needs to be handled
    \param unmanagecb [in] Function to be called when an event type does not need to be handled
    \return zero on success or non-zero error code

    Both the credentials and ret_psw strings are passwords, 
    for simple authentication. The ret_psw value (if non-NULL) specifies 
    the password we expect to get back from the MS or MTA to which we are 
    binding. The API will perform a check that the returned password matches
    the expected value and will return an error if they do not match.

    The function will return X400_E_WAIT_WRITE, indicating that the client
    application need to wait for the file descriptor associated with the
    X400msSession object to become writeable. At this point, 
    X400msProcessEvent needs to be called to handle the 'write' event (which
    actually indicates that the network-level connection has been established).
    X400msProcessEvent will then return X400_E_WAIT_READ, to indicate that 
    the application needs to wait for the file descriptor to become readable
    before calling X400msProcessEvent again. The application will need to
    call X400msProcessEvent multiple times before the association becomes
    open - at this point the "X400msConnEstablishedCb" callback will be
    invoked.
*/
X400MS_CDECL int X400msOpenAsyncSession (struct X400msSession *sp,
					 const char *addr,
					 const char *dn,
					 const char *credentials,
					 const char *pa,
					 const char *ret_psw,
					 X400msConnEstablishedCb *conupcb,
					 X400msConnDroppedCb *condowncb,
					 X400msMsgSubmittedCb *msgsubcb,
					 X400msMsgFetchedCb *msgfetchcb,
					 X400msMsgDeletedCb *msgdelcb,
					 X400msMsgWaitingCb *msgwaitcb,
					 X400msListCb *listcb,
					 X400msRegisterCb *registercb,
					 X400msAlertCb *alertcb,
					 X400msManageCb *managecb,
					 X400msManageCb *unmanagecb);

/*!
    \brief Create a new X400msSession object of the specified type
    \param type [in] Connection type: X400_TYPE_MS, X400_TYPE_MTA_SUBMIT
                     or X400_TYPE_MTA_BOTH
    \return Valid session pointer on success or NULL on error
*/
X400MS_CDECL struct X400msSession *X400msNewAsyncSession(int type);

/*!
    \brief Get a handle suitable for use in a call to select()
    \param session [in] Session to get handle for
    \return Valid fd on success or -1 on error
*/
X400MS_CDECL int X400msGetHandle(struct X400msSession *session);

/*!
    \brief Set a user pointer value in a session object
    \param session [in] Session to set pointer in
    \param ptr [in] Pointer value to set
    \return None
*/
X400MS_CDECL void X400msSetUserPointer(struct X400msSession *session, 
				       void *ptr);

/*!
    \brief Get a user pointer value from a session object
    \param session [in] Session to get pointer from
    \return Pointer value
*/
X400MS_CDECL void *X400msGetUserPointer(struct X400msSession *session);

/*!
    \brief Open a session to a Message Store (P7) or MTA (P3) in synchronous mode
    \param type [in] Connection type: X400_TYPE_MS, X400_TYPE_MTA_SUBMIT
                     or X400_TYPE_MTA_BOTH
    \param oraddr  [in] String encoding of OR-Address to use when binding
    \param dirname [in] String encoding of Directory name to use when binding
    \param credentials [in] String giving the credentials for the session
    \param pa [in] String encoding of MS or MTA Presentation Address to contact.
    \param messages [out] Pointer to integer to get number of messages, or NULL if
						  the result of the Summarize is not required
    \param spp [out] Pointer to receive pointer to opaque session data
    \return zero on success or non-zero error code

    The credentials string is a password, for simple authentication.

    Bear in mind that if you are binding only to submit a message, and don't need
	to know how many messages there are in the inbox, it's better to pass NULL
	to the messages paremeter, as that will prevent an expensive SUMMARIZE operation.

*/

X400MS_CDECL int X400msOpen(int type,
			    const char *oraddr,
			    const char *dirname,
			    const char *credentials,
			    const char *pa,
			    int *messages,
			    struct X400msSession **spp);

/*!
    \brief Open a session to a Message Store (P7) or MTA (P3) in synchronous mode, checking the password which the Message Store or MTA returns.
    \param type [in] Connection type: X400_TYPE_MS, X400_TYPE_MTA_SUBMIT
                     or X400_TYPE_MTA_BOTH
    \param oraddr  [in] String encoding of OR-Address to use when binding
    \param dirname [in] String encoding of Directory name to use when binding
    \param credentials [in] String giving the credentials for the session
    \param pa [in] String encoding of MS or MTA Presentation Address to contact.
    \param ret_psw [in] String for the expected password from the MS or MTA
    \param messages [out] Pointer to integer to get number of messages, or NULL if
						  the result of the Summarize is not required
    \param spp [out] Pointer to receive pointer to opaque session data
    \return zero on success or non-zero error code

    The Directory Name parameter is optional, specify it as NULL to omit.

    Both the credentials and ret_psw strings are passwords, 
    for simple authentication. The ret_psw value (if non-NULL) specifies 
    the password we expect to get back from the MS or MTA to which we are 
    binding. The API will perform a check that the returned password matches
    the expected value and will return an error if they do not match.

    Bear in mind that if you are binding only to submit a message, and don't need
	to know how many messages there are in the inbox, it's better to pass NULL
	to the messages paremeter, as that will prevent an expensive SUMMARIZE operation.

*/
X400MS_CDECL int X400msOpenCheck (int type,
				  const char *oraddr,
				  const char *dirname,
				  const char *credentials,
				  const char *pa,
				  const char *ret_psw,
				  int *messages,
				  struct X400msSession **spp);

/*!
    \brief Disable and enable configuration requests in MS Bind operations
    \param val [in] Boolean to turn on or off configuration requests
    \return None

    Disables and enables configuration requests in MS Bind operations. Default
    setting is TRUE.
*/
X400MS_CDECL void X400msSetConfigRequest(int val);


/*!
    \brief Close a X400 Session
    \param sp [in] Pointer to session data
    \return Zero on success or non-zero error code

    Closes a session and deletes all resources associated with the session.
*/

X400MS_CDECL int X400msClose (struct X400msSession *sp);


/*!
    \brief List messages in the P7 Message Store
    \param sp         [in] Pointer to session data
    \param since_time [in] Optional UTC time string
    \param lrp        [out] Pointer to receive pointer to opaque list result
    \return zero on success or non-zero error code

    The "since" parameter is optional, specify it as NULL to omit. If 
    specified, it should be a UTC time string, and is used to select only
    messages which were delivered after that time & date.
*/
X400MS_CDECL int X400msList (struct X400msSession *sp,
			     char *since_time,
			     struct X400msListResult  **lrp);

/*!
    \brief List messages in the P7 Message Store, specifying entryclass
    \param sp         [in] Pointer to session data
    \param since_time [in] Optional UTC time string
    \param entryclass [in] Entry class to list within
    \param lrp        [out] Pointer to receive pointer to opaque list result
    \return zero on success or non-zero error code

    The "since" parameter is optional, specify it as NULL to omit. If 
    specified, it should be a UTC time string, and is used to select only
    messages which were delivered after that time & date.
*/
X400MS_CDECL int X400msListEx (struct X400msSession *sp,
			       char *since_time,
			       int entryclass,
			       struct X400msListResult  **lrp);


/*!
    \brief List messages in the P7 Message Store, specifying entryclass
           and entrystatus
    \param sp         [in] Pointer to session data
    \param since_time [in] Optional UTC time string
    \param entryclass [in] Entry class to list within
    \param entrystatus [in] Entry status of messages to list
    \param lrp        [out] Pointer to receive pointer to opaque list result
    \return zero on success or non-zero error code

    The "since" parameter is optional, specify it as NULL to omit. If 
    specified, it should be a UTC time string, and is used to select only
    messages which were delivered after that time & date.
*/
X400MS_CDECL int X400msListExAux (struct X400msSession *sp,
				  char *since_time,
				  int entryclass,
				  int entrystatus,
				  struct X400msListResult  **lrp);



#define MS_ENTRY_CLASS_STORED_MESSAGES 0
#define MS_ENTRY_CLASS_SUBMITTED_MESSAGES 1
/*!
    \brief Get an integer attribute value from an element of a ListResult
    \param lr [in] List result to access
    \param paramtype [in] Type of parameter to get
    \param number [in] Index of element in ListResult to access
    \param valp [out] Pointer to receive attribute value
    \return zero on success or non-zero error code

    Retrieve an integer parameter value from an element of a ListResult.
    Index starts at 1 for the first element of the ListResult. The
    error code X400_E_NO_MORE_RESULTS is returned when the specified
    element does not exist. Error code X400_E_NO_VALUES is returned 
    when a particular parameter type is not present in the element.
*/
X400MS_CDECL int X400msListGetIntParam(struct X400msListResult  *lr,
				       int paramtype,
				       int number,
				       int *valp);

/*!
    \brief Get a string attribute value from an element of a ListResult
    \param lr [in] List result to access
    \param paramtype [in] Type of parameter to get
    \param number [in] Index of element in ListResult to access
    \param buffer [out] Buffer receive attribute value
    \param buflen [in] Length of buffer
    \param paramlenp [out] Pointer to receive length of value
    \return zero on success or non-zero error code

    Retrieve an string parameter value from an element of a ListResult.
    Index starts at 1 for the first element of the ListResult. The
    error code X400_E_NO_MORE_RESULTS is returned when the specified
    element does not exist. Error code X400_E_NO_VALUES is returned 
    when a particular parameter type is not present in the element.
*/
X400MS_CDECL int X400msListGetStrParam(struct X400msListResult  *lr,
				       int paramtype,
				       int number,
				       char *buffer,
				       size_t buflen,
				       size_t *paramlenp);

/*!
    \brief Free the memory occupied by a ListResult
    \param lr [in] List result to free
    \return none

    Free the memory occupied by the given ListResult.
*/
X400MS_CDECL void X400msListFree (struct X400msListResult *lr);

/*!
    \brief Creates new message
    \param sp   [in] X400 session pointer
    \param type [in] Type of message object
    \param mpp  [out] pointer to receive pointer to message object
    \return zero on success or non-zero error code

    Message types are
    - X400_MSG_MESSAGE    A Message object
    - X400_MSG_REPORT     A Report object
    - X400_MSG_PROBE      A Probe object
*/

X400MS_CDECL int X400msMsgNew (struct X400msSession *sp,
			       int type,
			       struct X400msMessage **mpp);

/*!
    \brief Returns count of addresses of given type in message object
    \param mp   [in] X400 MS message
    \param type [in] Type of address to count
    \param cp   [out] pointer integer to receive count of addresses
    \return zero on success or non-zero error code

*/

X400MS_CDECL int X400msMsgCountRecip(struct X400msMessage *mp, int type, int *cp);

/*!
    \brief Send message object
    \param mp        [in] Message pointer
    \return Zero on success or non-zero error code
*/

X400MS_CDECL int X400msMsgSend (struct X400msMessage *mp);

/*!
    \brief Delete message object
    \param mp        [in] Message pointer
    \param retain    [in] If true, message is not deleted from Message Store 
    \return Zero on success or non-zero error code

    This is called after submitting a message object or to abandon
    submission. Also called when a message which has been read has been
    finished with. By default, for a P7 connection, the message is deleted
    from the message store at the same time. However, if the retain flag is
    set, then the message is not deleted from the Store. This flag has no
    effect for a P3 connection.
*/

X400MS_CDECL int X400msMsgDelete (struct X400msMessage *mp, int retain);

/*!
    \brief Add string-valued parameter to the message
    \param mp        [in] Message pointer
    \param paramtype [in] Type of parameter
    \param value     [in] Value of parameter
    \param length    [in] Length of parameter, -1 if NUL terminated
    \return Zero on success or non-zero error code
*/

X400MS_CDECL int X400msMsgAddStrParam (struct X400msMessage *mp,
				       int paramtype,
				       const char *value,
				       size_t length);

/*!
    \brief Add integer-valued parameter to the message
    \param mp        [in] Message pointer
    \param paramtype [in] Type of parameter
    \param value     [in] Value of parameter
    \return Zero on success or non-zero error code
*/

X400MS_CDECL int X400msMsgAddIntParam (struct X400msMessage *mp,
				       int paramtype,
				       int value);


/*!
    \brief Add attachment to the message
    \param mp        [in] Message pointer
    \param type      [in] Type of attachment
    \param string    [in] String value for attachment
    \param length    [in] Length of string
    \return Zero on success or non-zero error code
*/

X400MS_CDECL int X400msMsgAddAttachment (struct X400msMessage *mp,
					 int type,
					 const char *string,
					 size_t length);


/*!
 \deprecated Use X400msMsgAddMessageBodyWtype instead.
*/
X400MS_CDECL int X400msMsgAddBodypart (struct X400msMessage *mp,
				       struct X400Bodypart *bp);


/*!
    \brief Add a message body part object to a message
    \param mp        [in]  Message pointer
    \param mbp       [in]  Message body part pointer
    \param type      [in]  BodyPart type (E.G X400_T_MESSAGE)
    \return Zero on success or non-zero error code

    The message body part must be a message delivery object for
    forwarded-message body part.

    If the message object being created has an IPN content, then any
    envelope information is ignored.
*/
X400MS_CDECL int X400msMsgAddMessageBodyWType (struct X400msMessage *mp,
                                               struct X400Message *mbp,
                                               int type);


/*!
    \brief Add a message body part object to a message
    \param mp        [in]  Message pointer
    \param mbp       [in]  Message body part pointer
    \param type      [in]  BodyPart type (E.G X400_T_MESSAGE)
    \return Zero on success or non-zero error code

    The message body part must be a message delivery object for
    forwarded-message body part.

    If the message object being created has an IPN content, then any
    envelope information is ignored.
*/
X400MS_CDECL int X400msMsgAddMessageBodyWType (struct X400msMessage *mp,
                                               struct X400Message *mbp,
                                               int type);
/*!
  \deprecated Use X400msMsgAddMessageBodyWType instead.
 */
X400MS_CDECL int X400msMsgAddMessageBody (struct X400msMessage *mp,
					  struct X400Message *mbp);


struct X400Recipient;

/*!
    \brief Add new recipient to a message
    \param mp   [in] Message pointer
    \param type [in] type of recipient
    \param rpp  [out] Pointer to place for pointer to recipient information
    \return Zero on success or non-zero error code

    Creates a recipient object associated with the message.
    There are several different types for recipient object.
    Normal recipients can be specified separately for the envelope
    (i.e. controlling where the message actually goes) and
    the header which is part of the content. They can also be
    specified together. A separate type of recipient is the reported
    recipient which appears in a report content.

    - X400_RECIP_PRIMARY   Primary recipient in header
    - X400_RECIP_CC        Carbon copy recipient in header
    - X400_RECIP_BCC       Blind copy recipient in header
    - X400_RECIP_REPLY     Reply-recipient in header
    - X400_RECIP_ENVELOPE  Actual recipient in envelope
    - X400_RECIP_REPORT    Reported recipient in report content
    - X400_RECIP_ORIGINAL  Originally intended recipient in report content
    - X400_ORIGINATOR      Originator address in IPM header
    - X400_AUTH_ORIG       Authorizing originator address in IPM header

    To set a given recipient in both envelope and header, add together the
    values, e.g. X400_RECIP_PRIMARY + X400_RECIP_ENVELOPE. The value
    X400_RECIP_STANDARD is a shorthand for this combination.
*/

X400MS_CDECL int X400msRecipNew (struct X400msMessage *mp,
				 int type,
				 struct X400Recipient **rpp);


/*!
    \brief Add string-valued parameter to the message
    \param rp        [in] Recipient pointer
    \param paramtype [in] Type of parameter
    \param value     [in] Value of parameter
    \param length    [in] Length of parameter, -1 if NUL terminated
    \return Zero on success or non-zero error code
*/

X400MS_CDECL int X400msRecipAddStrParam (struct X400Recipient *rp,
					 int paramtype,
					 const char *value,
					 size_t length);

/*!
    \brief Add integer-valued parameter to the message
    \param rp        [in] Message pointer
    \param paramtype [in] Type of parameter
    \param value     [in] Value of parameter
    \return Zero on success or non-zero error code
*/

X400MS_CDECL int X400msRecipAddIntParam (struct X400Recipient *rp,
					 int paramtype,
					 int value);

       
/*!
    \brief Wait for messages to be ready to be read.
    \param sp        [in] Session pointer
    \param seconds   [in] Seconds to wait
    \param count     [out] pointer to receive number of messages
    \return Zero on success, non-zero on failure, including timeout.

	If seconds is zero, then there is no delay. If seconds is negative,
	then the delay is indefinite.
*/

X400MS_CDECL int X400msWait (struct X400msSession *sp,
			     int seconds,
			     int *count);

/*!
    \brief Wait for new messages to be ready to be read.
    \param sp        [in] Session pointer
    \param seconds   [in] Seconds to wait
    \param count     [out] pointer to receive number of messages
    \return Zero on success, non-zero on failure, including timeout.

	If seconds is zero, then there is no delay. If seconds is negative,
	then the delay is indefinite.
*/

X400MS_CDECL int X400msWaitNew (struct X400msSession 	*sp,
				int seconds,
				int *count);


/*!
    \brief Get message object for transfer out from MTA
    \param sp        [in] Session pointer
    \param number    [in] Sequence number of message to retrieve
    \param mpp       [out] Pointer to receive message object pointer
    \param typep     [out] Pointer to receive message object type.
    \param seqp      [out] Pointer to receive sequence number
    \return Zero on success, non-zero on failure or message verification failure

    Note that this function is deprecated for P3, and should be replaced by
    X400msMsgGetStart() and X400msMsgGetFinish() in order to provide
    transactional security.

    The \e number argument can be zero, which means fetch the next message object.
    For a P7 connection, it can specify the sequence number of the object
    to retrieve. It is ignored for P3.

    The \e seqp argument, if not NULL, points to an integer which receives
    the sequence number for a message object, when the connection is P7.

    Note that if the message is signed, the API will attempt to verify
    the signature. If verification fails, the call will return 
    X400_E_NOERROR, unless the defaults object has the integer value
    X400_B_RETURN_VERIFICATION_ERRORS set to 1. 

    In this case possible verification errors which can be returned are:
    - X400SEC_E_X509_ENV
    - X400SEC_E_X509_INIT
    - X400SEC_E_X509_SESSION_INIT
    - X400SEC_E_X509_VERIFY_FAIL_NO_CERT
    - X400SEC_E_X509_VERIFY_FAIL_NO_PUBKEY
    - X400SEC_E_X509_VERIFY_FAIL_INCOMPAT_ALG
    - X400SEC_E_X509_VERIFY_FAIL_UNSUPPORTED_ALG
    - X400SEC_E_X509_VERIFY_FAIL

    In all the above cases the message is returned, and can be handled,
    and must be deleted using X400msMsgDelete().

    This is to retain backwards compatibility. All users of the API which 
    wish to be a correctly report message verification failures
    should set X400_B_RETURN_VERIFICATION_ERRORS to 1, and report
    message verification failures appropriately.
*/

X400MS_CDECL int X400msMsgGet (struct X400msSession *sp,
			       int number,
			       struct X400msMessage **mpp,
			       int *typep,
			       int *seqp);

/*!
    \brief Get message object for transfer out from MTA
    \param sp        [in] Session pointer
    \param number    [in] Sequence number of message to retrieve
    \param entryclass   [in] Entry class of message to retrieve
    \param mpp       [out] Pointer to receive message object pointer
    \param typep     [out] Pointer to receive message object type.
    \param seqp      [out] Pointer to receive sequence number
    \return Zero on success, non-zero on failure or message verification failure

    Note that this function is deprecated for P3, and should be replaced by
    X400msMsgGetStart() and X400msMsgGetFinish() in order to provide
    transactional security.

    The \e number argument can be zero, which means fetch the next message object.
    For a P7 connection, it can specify the sequence number of the object
    to retrieve. It is ignored for P3.

    The \e seqp argument, if not NULL, points to an integer which receives
    the sequence number for a message object, when the connection is P7.

    Note that if the message is signed, the API will attempt to verify
    the signature. If verification fails, the call will return 
    X400_E_NOERROR, unless the defaults object has the integer value
    X400_B_RETURN_VERIFICATION_ERRORS set to 1. 

    In this case possible verification errors which can be returned are:
    - X400SEC_E_X509_ENV
    - X400SEC_E_X509_INIT
    - X400SEC_E_X509_SESSION_INIT
    - X400SEC_E_X509_VERIFY_FAIL_NO_CERT
    - X400SEC_E_X509_VERIFY_FAIL_NO_PUBKEY
    - X400SEC_E_X509_VERIFY_FAIL_INCOMPAT_ALG
    - X400SEC_E_X509_VERIFY_FAIL_UNSUPPORTED_ALG
    - X400SEC_E_X509_VERIFY_FAIL

    In all the above cases the message is returned, and can be handled,
    and must be deleted using X400msMsgDelete().

    This is to retain backwards compatibility. All users of the API which 
    wish to be a correctly report message verification failures
    should set X400_B_RETURN_VERIFICATION_ERRORS to 1, and report
    message verification failures appropriately.
*/

X400MS_CDECL int X400msMsgGetEx (struct X400msSession *sp,
				 int number,
				 int entryclass,
				 struct X400msMessage **mpp,
				 int *typep,
				 int *seqp);

/*!
    \brief Get message object for transfer out from MTA
    \param sp        [in] Session pointer
    \param number    [in] Sequence number of message to retrieve
    \param mpp       [out] Pointer to receive message object pointer
    \param typep     [out] Pointer to receive message object type.
    \param seqp      [out] Pointer to receive sequence number
    \return Zero on success, non-zero on failure or message verification failure

    The \e number argument can be zero, which means fetch the next message object.
    For a P7 connection, it can specify the sequence number of the object
    to retrieve. It is ignored for P3.

    The \e seqp argument, if not NULL, points to an integer which receives
    the sequence number for a message object, when the connection is P7.

    When used in P3 mode, this function does not acknowledge recipt of the
    message it returns: this should be done by calling X400msMsgGetFinish()
    once the message has been successfully processed.

    Note that if the message is signed, the API will attempt to verify
    the signature. If verification fails, the call will return 
    X400_E_NOERROR, unless the defaults object has the integer value
    X400_B_RETURN_VERIFICATION_ERRORS set to 1. 

    In this case possible verification errors which can be returned are:
    - X400SEC_E_X509_ENV
    - X400SEC_E_X509_INIT
    - X400SEC_E_X509_SESSION_INIT
    - X400SEC_E_X509_VERIFY_FAIL_NO_CERT
    - X400SEC_E_X509_VERIFY_FAIL_NO_PUBKEY
    - X400SEC_E_X509_VERIFY_FAIL_INCOMPAT_ALG
    - X400SEC_E_X509_VERIFY_FAIL_UNSUPPORTED_ALG
    - X400SEC_E_X509_VERIFY_FAIL

    In all the above cases the message is returned, and can be handled,
    and must be deleted using X400msMsgDelete().

    This is retain backwards compatibility. All users of the API which 
    wish to be a correctly report message verification failures
    should set X400_B_RETURN_VERIFICATION_ERRORS to 1, and report
    message verification failures appropriately.
*/

X400MS_CDECL int X400msMsgGetStart (
    struct X400msSession *sp,
    int 		number,
    struct X400msMessage  **mpp,
    int 		*typep,
    int 		*seqp);

/*!
    \brief Get message object for transfer out from MTA
    \param sp        [in] Session pointer
    \param number    [in] Sequence number of message to retrieve
    \param entryclass    [in] Entry class of message to retrieve
    \param mpp       [out] Pointer to receive message object pointer
    \param typep     [out] Pointer to receive message object type.
    \param seqp      [out] Pointer to receive sequence number
    \return Zero on success, non-zero on failure or message verification failure

    The \e number argument can be zero, which means fetch the next message object.
    For a P7 connection, it can specify the sequence number of the object
    to retrieve. It is ignored for P3.

    The \e seqp argument, if not NULL, points to an integer which receives
    the sequence number for a message object, when the connection is P7.

    When used in P3 mode, this function does not acknowledge recipt of the
    message it returns: this should be done by calling X400msMsgGetFinish()
    once the message has been successfully processed.

    Note that if the message is signed, the API will attempt to verify
    the signature. If verification fails, the call will return 
    X400_E_NOERROR, unless the defaults object has the integer value
    X400_B_RETURN_VERIFICATION_ERRORS set to 1. 

    In this case possible verification errors which can be returned are:
    - X400SEC_E_X509_ENV
    - X400SEC_E_X509_INIT
    - X400SEC_E_X509_SESSION_INIT
    - X400SEC_E_X509_VERIFY_FAIL_NO_CERT
    - X400SEC_E_X509_VERIFY_FAIL_NO_PUBKEY
    - X400SEC_E_X509_VERIFY_FAIL_INCOMPAT_ALG
    - X400SEC_E_X509_VERIFY_FAIL_UNSUPPORTED_ALG
    - X400SEC_E_X509_VERIFY_FAIL

    In all the above cases the message is returned, and can be handled,
    and must be deleted using X400msMsgDelete().

    This is retain backwards compatibility. All users of the API which 
    wish to be a correctly report message verification failures
    should set X400_B_RETURN_VERIFICATION_ERRORS to 1, and report
    message verification failures appropriately.
*/

X400MS_CDECL int X400msMsgGetStartEx (
    struct X400msSession *sp,
    int 		number,
    int                 entryclass,
    struct X400msMessage  **mpp,
    int 		*typep,
    int 		*seqp);

/*! \defgroup deliveryErrors Error codes for passing into X400msMsgGetFinish
  \{ */

 #define X400_DE_SECURITY_ERROR 12
/*!< Security error on delivery */

 #define X400_DE_DELIVERY_CONTROL_VIOLATED 1
/*!< Delivery control violated*/

 #define X400_DE_UNSUPPORTED_CRITICAL_FUNCTION 13
/*!< Unsupported critical function */

 #define X400_DE_REMOTE_BIND_ERROR 15
/*!< Remote bind error */

/* \} */

/*!
    \brief Generate delivery result or error for a message
    \param mp       [in] Message object for which to signal success/failure
    \param errnum   [in] Error code for non-delivery reason
    \param problem  [in] Problem code for non-delivery (see X.411 for values)

    This function is a no-op for P7. For P3 mode, this function
    signals successful delivery of a message back to the MTA.
*/
X400MS_CDECL int X400msMsgGetFinish (struct X400msMessage *mp,
				     int errnum,
				     int problem);


/*!
    \brief Return a string-valued parameter from the message object
    \param mp        [in] message object pointer
    \param paramtype [in] type of parameter
    \param buffer    [in/out] address for result
    \param buflen    [in] length of buffer provided
    \param paramlenp  [out] Pointer to receive actual length
    \return Zero on success or non-zero error code
*/

X400MS_CDECL int X400msMsgGetStrParam (struct X400msMessage *mp,
				       int paramtype,
				       char *buffer,
				       size_t buflen,
				       size_t *paramlenp);

/*!
    \brief Return a integer-valued parameter from the message object
    \param mp        [in] message object pointer
    \param paramtype [in] type of parameter
    \param valp      [out] Pointer to receive value
    \return Zero on success or non-zero error code
*/

X400MS_CDECL int X400msMsgGetIntParam (struct X400msMessage *mp,
				       int paramtype,
				       int *valp);

/*!
    \brief Return the data of an attachment (=bodypart) from the message object.
    \param mp        [in] message object pointer
    \param number    [in] Attachment number
    \param typep     [out] Pointer to receive attachment type
    \param buffer    [in] Buffer to receive data
    \param buflen    [in] Length of buffer
    \param lengthp   [out] Length of string
    \return Zero on success or non-zero error code

    This is different from X400msMsgGetBodypart() in that only the data
    and not the bodypart itself is returned.
*/

X400MS_CDECL int X400msMsgGetAttachment (struct X400msMessage *mp,
					 int number,
					 int *typep,
					 char *buffer,
					 size_t buflen,
					 size_t *lengthp);


/*!
    \brief Return a pointer to a body part object
    \param mp        [in] message object pointer
    \param number    [in] Bodypart number
    \param typep     [out] Pointer to receive attachment type
    \param bpp       [out] Pointer to pointer to object
    \return Zero on success or non-zero error code

    If the body part is a forwarded message body part or a returned IPM
    then the error X400_E_MESSAGE_BODY is returned.
*/
X400MS_CDECL int X400msMsgGetBodypart (struct X400msMessage *mp,
				       int number,
				       int *typep,
				       struct X400Bodypart **bpp);


/*!
    \brief Return a pointer to a message part object
    \param mp        [in] message object pointer
    \param number    [in] Bodypart number
    \param mpp       [out] Pointer to pointer to object
    \return Zero on success or non-zero error code

*/
X400MS_CDECL int X400msMsgGetMessageBody (struct X400msMessage *mp,
					  int number,
					  struct X400Message **mpp);

/*!
    \brief Get certificate object from message
    This returns a certificate which was used to sign an object in the
    message. The attribute type indicates which object type.
    \param mp        [in] Message object pointer
    \param certtype  [in] Attribute type for certificate
    \param certp     [out] Pointer to receive certificate object pointer
    \return Zero on success, non-zero on failure
*/

X400COMMON_CDECL int X400msMsgGetCert(struct X400msMessage *mp,
				      int certtype,
				      struct X400Certificate **certp);

/*!
                           int non_receipt_reason,
                           struct X400msMessage **mpp)
    \brief Get recipient object from message
    \param mp                 [in] Message object pointer
    \param non_receipt_reason [in] Non-receipt reason (-1 for receipt)
    \param mpp                [out] Pointer to receive message object pointer
    \return Zero on success, non-zero on failure

    Generate a new message object ready for submission as a Inter-Personal
    Notification based on the received message object. It is associated with
    the same session as the subject message.

    If the non-receipt reason is specified as -1, then the IPN is a
    receipt notification. The receipt time is set to the current date and time.

    For a non-receipt notification, the original IPM is returned if the
    requested by the sender.

    If a notification of the type indicated has not been requested by the
    sender, then the function returns X400_E_NO_VALUE.
*/

X400MS_CDECL int X400msMakeIPN (struct X400msMessage *mp,
				int non_receipt_reason,
				struct X400msMessage **mpp);


/*!
    \brief Get recipient object from message
    \param mp        [in] Message object pointer
    \param type      [in] Type of recipient
    \param number    [in] Ordinal number of recipient
    \param rpp       [out] Pointer to receive recipient object pointer
    \return Zero on success, non-zero on failure

    
*/

X400MS_CDECL int X400msRecipGet (struct X400msMessage *mp,
				 int type,
				 int number,
				 struct X400Recipient **rpp);


/*!
    \brief Return a string-valued parameter from the recipient object
    \param rp        [in] recipient object pointer
    \param paramtype [in] type of parameter
    \param buffer    [in/out] address for result
    \param buflen    [in] length of buffer provided
    \param paramlenp  [out] Pointer to receive actual length
    \return Zero on success or non-zero error code
*/

X400MS_CDECL int X400msRecipGetStrParam (struct X400Recipient *rp,
					 int paramtype,
					 char *buffer,
					 size_t buflen,
					 size_t *paramlenp);

/*!
    \brief Return a integer-valued parameter from the recipient object
    \param rp        [in] recipient object pointer
    \param paramtype [in] type of parameter
    \param valp      [out] Pointer to receive value
    \return Zero on success or non-zero error code
*/

X400MS_CDECL int X400msRecipGetIntParam (struct X400Recipient *rp,
					 int paramtype,
					 int *valp);

/*!
    \brief Set a default integer parameter value in a session
    \param sp [in] Session handle returned from an X400msOpen call
    \param paramtype [in] Parameter type to set
    \param value [in] Parameter value to set
    \return zero on success or non-zero error code

    Sets a default integer parameter value in a session object. If 
    this parameter has previously been set in the session, the existing
    value is replaced. When a message is sent, any attribute which is 
    not present in the message being sent
    will be taken from the session's default set.
*/

X400MS_CDECL int X400msSetIntDefault(struct X400msSession *sp,
				     int paramtype,
				     int value);
			
/*!
    \brief Set a default string parameter value in a session
    \param sp [in] Session handle returned from an X400msOpen call
    \param paramtype [in] Parameter type to set
    \param value [in] Parameter value to set
    \param length [in] Length of parameter value to set
    \return zero on success or non-zero error code

    Sets a default string parameter value in a session object. 
    If this parameter has previously been set in the session, 
    the existing value is replaced. When a message is sent, 
    any attribute which is not present in the message being sent
    will be taken from the session's default set.
*/

X400MS_CDECL int X400msSetStrDefault(struct X400msSession *sp,
				     int paramtype,
				     const char *value,
				     size_t length);

struct X400msAutoActionParameter;

/*!
    \brief Register an autoaction with the Message Store
    \param sp [in] Session handle returned from an X400msOpen call
    \param type [in] Autoaction type to register
    \param id [in] Identifier for autoaction
    \param aa_param [in] Parameter for new autoaction
    \return zero on success or non-zero error code

    Registers an autoaction with the Store. Any existing 
    autoaction with the same id will be overwritten.
    Available values for autoaction type are X400_AUTO_ALERT 
    and X400_AUTO_FORWARD.
*/
X400MS_CDECL int 
X400msRegisterAutoAction (struct X400msSession *sp,
			  int type,
			  int id,
			  struct X400msAutoActionParameter *aa_param);

/*!
    \brief Deregister an autoaction from the Message Store
    \param sp [in] Session handle returned from an X400msOpen call
    \param type [in] Autoaction type to deregister
    \param id [in] Identifier of autoaction to deregister
    \return zero on success or non-zero error code

    Deregisters an autoaction with the Store.
    Available values for autoaction type are X400_AUTO_ALERT 
    and X400_AUTO_FORWARD.
*/
X400MS_CDECL int 
X400msDeregisterAutoAction (struct X400msSession *sp,
			    int type,
			    int id);

/*!
    \brief Create a new (empty) autoaction parameter structure
    \return New autoaction handle
*/
X400MS_CDECL struct X400msAutoActionParameter *X400msNewAutoActionParameter(void);

/*!
    \brief Free an autoaction parameter
    \param aa_param [in] Handle to autoaction parameter to be freed
    \return None
*/
X400MS_CDECL void 
X400msFreeAutoActionParameter(struct X400msAutoActionParameter *aa_param);


/*!
    \brief Add a receipient to the autoaction parameter
    \param aap       [in] Autoaction parameter
    \param reciptype [in] Type of recipient parameter
    \param recip     [in] Recipient structure
    \return Zero on success or non-zero error code
*/
X400MS_CDECL int 
X400msAutoActionParameterAddRecip(struct X400msAutoActionParameter *aap,
				  int reciptype,
				  struct X400Recipient *recip);

/*!
    \brief Add integer-valued parameter to the autoaction parameter
    \param aap       [in] Autoaction parameter
    \param paramtype [in] Parameter type
    \param value     [in] Value of parameter
    \return Zero on success or non-zero error code
*/
X400MS_CDECL int  
X400msAutoActionParameterAddIntParam(struct X400msAutoActionParameter *aap,
				     int paramtype,
				     int value);

/*!
    \brief Add string-valued parameter to the autoaction parameter
    \param aap       [in] Autoaction parameter
    \param paramtype [in] Type of parameter
    \param value     [in] Value of parameter
    \param length    [in] Length of parameter, -1 if NUL terminated
    \return Zero on success or non-zero error code
*/
X400MS_CDECL int 
X400msAutoActionParameterAddStrParam(struct X400msAutoActionParameter *aap,
				     int paramtype,
				     const char *value,
				     size_t length);

/*!
    \brief Create a new DL Expansion History object from the message object
    \param mp    [in] message object pointer
    \param entry [in] DL expansion history element to fetch
    \param hist  [out] pointer to receive new DL Expansion History object
    \return Zero on success, or non-zero error code
*/
X400MS_CDECL int X400msDLExpHistGet (struct X400msMessage *mp,
				     int entry,
				     struct X400DLExpHist **hist);

/*!
 *  \brief Create a new DL Expansion History object, and associate it with the
 *  existing message.
 *  \param mp [in] message pointer
 *  \param histp [out] pointer to receive new DL Expansion History object
 *  \return Zero on success, or non-zero error code
 */
X400MS_CDECL int X400msDLExpHistNew (
     struct X400msMessage 	*mp,
     struct X400DLExpHist 	**histp
);

struct X400DLExpHist;


/*!
 *  \brief Create a new Address List Indicator Object,
 *  and associate it with the existing message.
 *  \param mp [in] message pointer
 *  \param ali [out] pointer to receive new ALI object
 *  \return Zero on success, or non-zero error code
 */
X400MS_CDECL int X400msALINew(
    struct X400msMessage *mp,
    struct X400ALI **ali
);

/*!
 *  \brief Get a Address List Indicator Object for a message object
 *  \param mp [in] message pointer
 *  \param entry   [in] ALI object being used.
 *  \param info [out] pointer to receive new ALI object
 *  \return Zero on success, or non-zero error code
 */
X400MS_CDECL int X400msALIGet (
    struct X400msMessage *mp,
    int entry,
    struct X400ALI **info
);

struct X400ALI;


/*!
 *  \brief Create a new Printable String Syntax Object,
 *  and associate it with the existing message.
 *  \param mp [in] message pointer
 *  \param pss [out] pointer to receive new PSS object
 *  \param type [in] type of PSS object
 *  \return Zero on success, or non-zero error code
 */
X400MS_CDECL int X400msPSSNew(
    struct X400msMessage *mp,
    struct X400PSS **pss,
    int type
);

/*!
 *  \brief Get a Printable String Syntax Object for a message object
 *  \param mp [in] message pointer
 *  \param type [in] type of PSS being used.
 *  \param entry   [in] PSS object being used.
 *  \param pss [out] pointer to receive new pss object
 *  \return Zero on success, or non-zero error code
 */
X400MS_CDECL int X400msPSSGet (
    struct X400msMessage *mp,
    int type,
    int entry,
    struct X400PSS **pss
);

struct X400PSS;


/*!
 *  \brief Create a new P772 Distribution Field object for a message object
 *  \param mp [in] message pointer
 *  \param pss [out] pointer to receive distfield object
 *  \return Zero on success, or non-zero error code
 */
X400MS_CDECL int X400msDistFieldNew(
    struct X400msMessage *mp,
    struct X400DistField **pss
);

/*!
 *  \brief Get a new P772 Distribution Field object for a message object
 *  \param mp [in] message pointer
 *  \param entry   [in] distfield object being used.
 *  \param pss [out] pointer to receive new PSS object
 *  \return Zero on success, or non-zero error code
 */
X400MS_CDECL int X400msDistFieldGet (
    struct X400msMessage *mp,
    int entry,
    struct X400DistField **pss
);

struct X400DistField;

struct X400OtherRecip;


/*!
 *  \brief Create a new P772 Other Recipient object for a message object
 *  \param mp [in] message pointer
 *  \param otherrecip [out] pointer to receive other recipient object
 *  \return Zero on success, or non-zero error code
 */
X400MS_CDECL int X400msOtherRecipNew(
    struct X400msMessage *mp,
    struct X400OtherRecip **otherrecip
);

/*!
 *  \brief Get a new P772 Other Recipient object for a message object
 *  \param mp [in] message pointer
 *  \param entry [in] Other Recip object being used.
 *  \param otherrecip [out] pointer to receive new other recipient object
 *  \return Zero on success, or non-zero error code
 */
X400MS_CDECL int X400msOtherRecipGet (
    struct X400msMessage *mp,
    int entry,
    struct X400OtherRecip **otherrecip
);



/*!
    \brief Get the Redirection History object from a message envelope this is represented by 8.3.1.2.1.5 in X.411
    \param msg   [in] X.400 Message
    \param entry   [in] Redirection history object to use
    \param hist    [out] Pointer to redirection history object
    \return Zero on success or non-zero error code
*/
X400MS_CDECL int X400msRediHistGetEnv(struct X400msMessage *msg,
				      int entry,
				      struct X400RediHist **hist);



/*!
 *  \brief Get a Trace Info object for a message object
 *  \param mp [in] message pointer
 *  \param entry [in] Trace Info object to use
 *  \param info [out] pointer to receive new trace infomation object
 *  \param type [in] type of trace info object to get
 *  \return Zero on success, or non-zero error code
 */
X400MS_CDECL int X400msTraceInfoGet (
    struct X400msMessage *mp,
    int entry,
    struct X400TraceInfo **info,
    int type
);
struct X400TraceInfo;

/*!
 *  \brief Get an Internal Trace Info object for a message object
 *  \param mp [in] message pointer
 *  \param entry [in] Trace Info object to use
 *  \param info [out] pointer to receive new trace infomation object
 *  \return Zero on success, or non-zero error code
 */
X400MS_CDECL int X400msInternalTraceInfoGet (
    struct X400msMessage *mp,
    int entry,
    struct X400InternalTraceInfo **info
);
struct X400InternalTraceInfo;


/*!
    \brief Get Originator and DL expansion history object
    \param msg   [in] X.400 Message
    \param entry [in] The orginator and dl expansion history object to use
    \param or_and_dl  [out] Pointer to orig ans dl expan history object
    \return Zero on success or non-zero error code
*/
X400MS_CDECL int X400msORandDLGet(struct X400msMessage *msg,
                                  int entry,
                                  struct X400ORandDL **or_and_dl);

struct X400ORandDL;

/*!
    \brief Obtains the bytes containing the ASN.1 encoding of the Content
    of a message. The Content is obtained by building an IPM from
    the attributes of the message and then encoding it.

    \param mp          [in]  Message from which to get Content encoding
    \param buf         [in]  Buffer for return of content
    \param buflen      [in]  Length of buffer
    \param lenp        [out] Length of data returned
    \return Zero on success or non-zero error code
*/
X400MS_CDECL int X400msGetContentOctets(struct X400msMessage *mp,
					char *buf,
					size_t buflen,
					size_t *lenp);


/*!
    \brief Takes a byte stream containing the ASN.1 encoding of
    a P22 (or P772) content, decodes it and sets it into a message,
    so that attributes can be extracted from it.

    \param mp          [in]  Message into which to put the Content
    \param buf         [in]  Buffer containing encoding of content
    \param buflen      [in]  Length of buffer
    \return Zero on success or non-zero error code
*/
X400MS_CDECL int X400msSetContentOctets(struct X400msMessage *mp, 
					char *buf, size_t len);
/*! \} */

#endif /* _X400_MSAPI_H */

