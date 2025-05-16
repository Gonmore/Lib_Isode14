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

#ifndef _X400_MTAPI_H
#define _X400_MTAPI_H

#include "cdecl.h"
#include "x400_api.h"



/*! \mainpage Isode X.400 Gateway API Overview
    <b>Version 14.4</b><br>
    <b>Copyright (c) 2005-2009, Isode Limited, London, England</b>
    
    \section intro Introduction
    
    This document is a specification of the Isode X.400
    Gateway API.  A summary of this product, which sets
    out its general function and intended application is given in
    http://www.isode.com/products/amhs-integration-library.html

    \section start How to Read this API

    If you wish to "Jump right in" then the best thing to do is:<br>
    
    -# Look at the \ref x400_mttutorial.c "x400_mttutorial.c" example program.
    It is a very
    simple streamlined program that demonstrates the basic approach of
    manipulating API objects.
    -# Look at the \ref x400_mtsend.c "x400_mtsend.c" and
    \ref x400_mtrcv.c "x400_mtrcv.c" example programs.
    These programs give a more detailed example of how the API should be used.
    -# Look at the \ref ref_table "reference table" which matches
    attributes described within the standards to the objects / attributes
    used within the API.
    -# Have a good read of the API manual.
    
    The API is read by following the links on the left of the
    page.Recommended places to start:

    \ref X400mt "The collocated interface", intended for AFTN and
    CIDIN gateway applications.

    This is an API for interfacing to an X400 MTA suitable for building
    gateway functionality. It provides mechanisms for building and
    examining X.400 message, probes and reports. It also provides
    mechanisms for transferring such objects into X.400, and 
    transferring out such objects.

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
    
    \subsection x400relationship Relationship of Gateway and Pure X.400 Interfaces
    
    Top-level message objects are built, transferred in and out, and 
    examined, using the X400 Gateway API. In particular, functions
    specific to X.400 MT message objects are used to get and set 
    attribute values within messages. For example, X400mtMsgAddStrParam()
    would be used to set a string parameter value in such a message.
    
    There is one specific situation where protocol-independent functions
    need to be used to manipulate messages: this is when dealing with
    forwarded messages (i.e. messages which have been included as 
    bodyparts of "top-level" X.400 MT messages). In this case, the
    equivalent "pure X.400" functions must be used - X400MsgAddStrParam() 
    in the equivalent case to the one described above.

    \section bindings Language Bindings

    The X.400 Gateway API is available in C, Java and Tcl bindings.
    
    \subsection cbindings C Bindings

    The example code is in:
    \arg Unix: /opt/isode/share/x400sdk/example/c
    \arg Windows: c:\\Isode\\share\\x400sdk\\example\\c

    \subsection javabindings Java Bindings
    
    The example code is in

    \arg Unix: /opt/isode/share/x400sdk/example/java
    \arg Windows: c:\\Isode\\share\\x400sdk\\example\\java

    \subsection tclbindings Tcl Bindings

    The example code is in

    \arg Unix: /opt/isode/share/x400sdk/example/tcl
    \arg Windows: c:\\Isode\\share\\x400sdk\\example\\tcl

    \subsection apistyle Style of API

    The APIs use an object-oriented style, and rely on arguments which are

    \arg integers (either explicitly or as manifest constants)
    \arg strings (whose length is passed as an integer)
    \arg opaque objects (on which the API functions perform operations)

    \section freeback Feedback
    Please send any comments on the API to support\@isode.com.
*/
/*!
    \example examples/x400_mttutorial.c
    This is a streamlined heavily commented example program,
    which transfers a message into the MTA, and then transfers
    the message out of it.
*/

/*!
    \example examples/x400_mtsend.c
    This is an example program which transfers a message into the MTA.
*/
/*!
    \example examples/x400_mtsend_rep.c
    This is an example program which transfers a report into the MTA.
*/
/*!
    \example examples/x400_mtrcv.c
    This is an example program which transfers messages and reports 
    out of the MTA.
*/


/*! \file x400_mtapi.h
    \brief X.400 Gateway Interface

    This is an API for interfacing to an X400 MTA suitable for building
    gateway functionality. It provides mechanisms for building and
    examining X.400 message, probes and reports. It also provides
    mechanisms for transferring such objects into X.400, and 
    transferring out such objects.
*/

/*! \defgroup X400mt X400 Gateway API
   These function calls provide the X.400 Gateway API.
 \{
*/

/*!
    \brief Return string for error code
    \param error [in] error code
    \return Pointer to NUL terminated string for error
*/

X400MT_CDECL const char *X400mtError (int error);

struct X400mtSession;

/*!
    \brief Open a session to the MTA
    \param credentials [in] String for the credentials for the session
    \param spp [out] Pointer to receive pointer to opaque session data
    \return zero on success or non-zero error code

    Opens a session for transfer in and transfer out.
    The credentials string should be formatted as "channelname[/mtaname]".
*/

X400MT_CDECL int X400mtOpen (const char *credentials,
			     struct X400mtSession **spp);



/*!
    \brief Close a X400 Session
    \param sp [in] Pointer to session data
    \return Zero on success or non-zero error code

    Closes a session and deletes all resources associated with the session.
*/

X400MT_CDECL int X400mtClose (struct X400mtSession *sp);

/*!
    \brief Set a default integer parameter value in a session
    \param sp [in] Session handle returned from an X400mtOpen call
    \param paramtype [in] Parameter type to set
    \param value [in] Parameter value to set
    \return zero on success or non-zero error code

    Sets a default integer parameter value in a session object. If this parameter
    has previously been set in the session, the existing value is replaced. When
    a message is sent, any attribute which is not present in the message being sent
    will be taken from the session's default set.
*/

X400MT_CDECL int X400mtSetIntDefault(struct X400mtSession *sp,
				     int paramtype,
				     int value);

/*!
    \brief Set a default string parameter value in a session
    \param sp [in] Session handle returned from an X400mtOpen call
    \param paramtype [in] Parameter type to set
    \param value [in] Parameter value to set
    \param length [in] Length of parameter value to set
    \return zero on success or non-zero error code

    Sets a default string parameter value in a session object. If this parameter
    has previously been set in the session, the existing value is replaced. When
    a message is sent, any attribute which is not present in the message being sent
    will be taken from the session's default set.
*/

X400MT_CDECL int X400mtSetStrDefault(struct X400mtSession *sp,
				     int paramtype,
				     const char *value,
				     size_t length);

struct X400mtMessage;

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

X400MT_CDECL int X400mtMsgNew (struct X400mtSession *sp,
			       int type,
			       struct X400mtMessage **mpp);

/*!
    \brief Returns count of addresses of given type in message object
    \param mp   [in] X400 MT message
    \param type [in] Type of address to count
    \param cp   [out] pointer integer to receive count of addresses
    \return zero on success or non-zero error code
*/

X400MT_CDECL int X400mtMsgCountRecip(struct X400mtMessage *mp, int type, int *cp);

/*!
    \brief Send message object to MTA
    \param mp        [in] Message pointer
    \return Zero on success or non-zero error code
*/

X400MT_CDECL int X400mtMsgSend (struct X400mtMessage *mp);

/*!
    \brief Delete message object
    \param mp        [in] Message pointer
    \return Zero on success or non-zero error code

    This is called after submitting a message object or to abandon
    submission.
*/

X400MT_CDECL int X400mtMsgDelete (struct X400mtMessage *mp);

/*!
    \brief Add string-valued parameter to the message
    \param mp        [in] Message pointer
    \param paramtype [in] Type of parameter
    \param value     [in] Value of parameter
    \param length    [in] Length of parameter, -1 if NUL terminated
    \return Zero on success or non-zero error code
*/

X400MT_CDECL int X400mtMsgAddStrParam (struct X400mtMessage *mp,
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

X400MT_CDECL int X400mtMsgAddIntParam (struct X400mtMessage *mp,
				       int paramtype,
				       int value);


/*!
    \brief Add an attachment to the message
    \param mp        [in] Message pointer
    \param type      [in] Type of attachment
    \param string    [in] String value for attachment
    \param length    [in] Length of string
    \return Zero on success or non-zero error code
*/

X400MT_CDECL int X400mtMsgAddAttachment (struct X400mtMessage *mp,
					 int type,
					 const char *string,
					 size_t length);


/*!
    \brief Add a body part object to a message
    \param mp        [in]  Message pointer
    \param bp        [in] Bodypart pointer
    \return Zero on success or non-zero error code
*/
X400MT_CDECL int X400mtMsgAddBodypart (struct X400mtMessage *mp,
				       struct X400Bodypart *bp);

/*!
  \deprecated Use X400mtMsgAddMessageBodyWtype instead.
*/
X400MT_CDECL int X400mtMsgAddMessageBody (struct X400mtMessage *mp,
					  struct X400Message *mbp);

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
X400MT_CDECL int X400mtMsgAddMessageBodyWType (struct X400mtMessage *mp,
                                               struct X400Message *mbp,
                                               int type);

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

X400MT_CDECL int X400mtRecipNew (struct X400mtMessage *mp,
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

X400MT_CDECL int X400mtRecipAddStrParam (struct X400Recipient *rp,
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

X400MT_CDECL int X400mtRecipAddIntParam (struct X400Recipient *rp,
					 int paramtype,
					 int value);

       
/*!
    \brief Wait for messages to be transferred out
    \param sp        [in] Session pointer
    \param seconds   [in] Seconds to wait
    \param count     [out] pointer to receive number of messages
    \return Zero on success, non-zero on failure, including timeout.

	If seconds is zero, then there is no delay. If seconds is negative,
	then the delay is indefinite.
*/

X400MT_CDECL int X400mtWait (struct X400mtSession *sp,
			     int seconds,
			     int *count);

/*!
    \brief Transfer message object out from MTA completely
    \param sp        [in] Session pointer
    \param mpp       [out] Pointer to receive message object pointer
    \param typep     [out] Pointer to receive message object type.
    \return Zero on success, non-zero on failure

*/

X400MT_CDECL int X400mtMsgGet (struct X400mtSession *sp,
			       struct X400mtMessage **mpp,
			       int *typep);

/*!
    \brief Get message object for transfer out from MTA
    \param sp        [in] Session pointer
    \param mpp       [out] Pointer to receive message object pointer
    \param typep     [out] Pointer to receive message object type.
    \return Zero on success, non-zero on failure

    Starts transfer-out of a message from the MTA. Returns a handle
    to a message, but does not aceept responsibilty for the message. Once
    the message has been processed by the gateway application, 
    X400mtMsgGetFinish must be called to cause a delivery report to be
    generated if required, and to allow the MTA to release the message.
*/

X400MT_CDECL int X400mtMsgGetStart (struct X400mtSession *sp,
				    struct X400mtMessage **mpp,
				    int *typep);

/*!
    \brief Finish transfer-out of message from MTA, generate DR if required
    \param mp        [in] Message pointer
    \param status    [in] Status from gateway message handling.    
    \param reason    [in] DR reason code. See standards for values
    \param diag      [in] DR diagnostic code. See standards for values
    \param info      [in] DR supplementary info.
    \return Zero on success, non-zero on failure

    Signals that the gateway application has completed its handling of
    the specified message. 

    If the 'x400_reports' key of the x400tailor file is set to "1" and "status" 
    is specified as X400_E_NOERROR, a positive delivery report will be generated.
    If an error status is passed in, the DR reason and diagnostic codes and 
    supplementary information will be used to generate a DR. 
    
    Under normal circumstances, gateway applications should generate their own
    delivery reports, and not rely on the ones generated by this function, as
    for example, when there's more than one recipient in the message, it cannot 
    set a different status for each recipient.
*/

X400MT_CDECL int X400mtMsgGetFinish (struct X400mtMessage *mp,
				     int status,
				     int reason,
				     int diag,
				     const char *info);

/*!
    \brief Return a string-valued parameter from the message object
    \param mp        [in] message object pointer
    \param paramtype [in] type of parameter
    \param buffer    [in/out] address for result
    \param buflen    [in] length of buffer provided
    \param paramlenp  [out] Pointer to receive actual length
    \return Zero on success or non-zero error code
*/

X400MT_CDECL int X400mtMsgGetStrParam (struct X400mtMessage *mp,
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

X400MT_CDECL int X400mtMsgGetIntParam (struct X400mtMessage *mp,
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

X400MT_CDECL int X400mtMsgGetAttachment (struct X400mtMessage *mp,
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
X400MT_CDECL int X400mtMsgGetBodypart (struct X400mtMessage *mp,
				       int number,
				       int *typep,
				       struct X400Bodypart **bpp);


/*!
    \brief Return a pointer to a body part object
    \param mp        [in] message object pointer
    \param number    [in] Bodypart number
    \param mpp       [out] Pointer to pointer to object
    \return Zero on success or non-zero error code

*/
X400MT_CDECL int X400mtMsgGetMessageBody (struct X400mtMessage *mp,
					  int number,
					  struct X400Message **mpp);

/*!
    \brief Get recipient object from message
    \param mp        [in] Message object pointer
    \param type      [in] Type of recipient
    \param number    [in] Ordinal number of recipient
    \param rpp       [out] Pointer to receive recipient object pointer
    \return Zero on success, non-zero on failure

    
*/

X400MT_CDECL int X400mtRecipGet (struct X400mtMessage *mp,
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

X400MT_CDECL int X400mtRecipGetStrParam (struct X400Recipient *rp,
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

X400MT_CDECL int X400mtRecipGetIntParam (struct X400Recipient *rp,
					 int paramtype,
					 int *valp);

/*!
    \brief Create a new DL Expansion History object from the message object
    \param mp    [in] message object pointer
    \param entry [in] DL expansion history element to fetch
    \param hist  [out] pointer to receive new DL Expansion History object
    \return Zero on success, or non-zero error code
*/
X400MT_CDECL int X400mtDLExpHistGet (struct X400mtMessage *mp,
				     int entry,
				     struct X400DLExpHist **hist);

    
struct X400DLExpHist;

/*!
 *  \brief Create a new DL Expansion History object, and associate it with the
 *  existing message.
 *  \param mp [in] message pointer
 *  \param histp [out] pointer to receive new DL Expansion History object
 *  \return Zero on success, or non-zero error code
 */
X400MT_CDECL int X400mtDLExpHistNew (
     struct X400mtMessage 	*mp,
     struct X400DLExpHist 	**histp
);


/*!
 *  \brief Copy a new DL Expansion History object, and associate it with the
 *  a message.
 *  \param mp [in] message pointer
 *  \param orig [in] original DL Expansion History object to copy
 *  \return new DLExpHist object on success, or NULL on error
 */
X400MT_CDECL struct X400DLExpHist *X400mtDLExpHistDeepCopy (struct X400mtMessage *mp,
							    struct X400DLExpHist *orig);

/*!
 *  \brief Get a Trace Info object for a message object
 *  \param mp [in] message pointer
 *  \param entry [in] Trace Info object to use
 *  \param info [out] pointer to receive new trace infomation object
 *  \param type [in] type of trace info object to get
 *  \return Zero on success, or non-zero error code
 */
X400MT_CDECL int X400mtTraceInfoGet (
    struct X400mtMessage *mp,
    int entry,
    struct X400TraceInfo **info,
    int type
);
struct X400TraceInfo;


/*!
 *  \brief Create a new Trace Info object for a message object
 *  \param mp [in] message pointer
 *  \param info [out] pointer to receive new trace infomation object
 *  \param type [in] type of trace info object to create
 *  \return Zero on success, or non-zero error code
 */
X400MT_CDECL int X400mtTraceInfoNew(
    struct X400mtMessage *mp,
    struct X400TraceInfo **info,
    int type
);

/*!
 *  \brief Copy a TraceInformation object, and associate it with the
 *  a message.
 *  \param mp [in] message pointer
 *  \param orig [in] original TraceInfo object to copy
 *  \param type [in] type of trace info object to create
 *  \return new TraceInfo object on success, or NULL on error
 */
X400MT_CDECL struct X400TraceInfo *X400mtTraceInfoDeepCopy (struct X400mtMessage *mp,
							    struct X400TraceInfo *orig,
							    int type);

/*!
 *  \brief Get an Internal Trace Info object for a message object
 *  \param mp [in] message pointer
 *  \param entry [in] Trace Info object to use
 *  \param info [out] pointer to receive new trace infomation object
 *  \return Zero on success, or non-zero error code
 */
X400MT_CDECL int X400mtInternalTraceInfoGet (
    struct X400mtMessage *mp,
    int entry,
    struct X400InternalTraceInfo **info
);
struct X400InternalTraceInfo;


/*!
 *  \brief Create a new Internal Trace Info object for a message object
 *  \param mp [in] message pointer
 *  \param info [out] pointer to receive new trace infomation object
 *  \return Zero on success, or non-zero error code
 */
X400MT_CDECL int X400mtInternalTraceInfoNew(
    struct X400mtMessage *mp,
    struct X400InternalTraceInfo **info
);

/*!
 *  \brief Copy an InternalTraceInformation object, and associate it with the
 *  a message.
 *  \param mp [in] message pointer
 *  \param orig [in] original InternalTraceInfo object to copy
 *  \return new InternalTraceInfo object on success, or NULL on error
 */
X400MT_CDECL struct X400InternalTraceInfo *X400mtInternalTraceInfoDeepCopy (struct X400mtMessage *mp,
							    struct X400InternalTraceInfo *orig);

struct X400ALI;


/*!
 *  \brief Create a new Address List Indicator object for a message object
 *  \param mp [in] message pointer
 *  \param ali [out] pointer to receive ALI object
 *  \return Zero on success, or non-zero error code
 */
X400MT_CDECL int X400mtALINew(
    struct X400mtMessage *mp,
    struct X400ALI **ali
);

/*!
 *  \brief Copy an AddressListIndicator object, and associate it with the
 *  a message.
 *  \param mp [in] message pointer
 *  \param orig [in] original AddressListIndicator object to copy
 *  \return new AddressListIndicator object on success, or NULL on error
 */
X400MT_CDECL struct X400ALI *X400mtALIDeepCopy (struct X400mtMessage *mp,
						struct X400ALI *orig);

/*!
 *  \brief Get a new Address List Indicator object for a message object
 *  \param mp [in] message pointer
 *  \param entry   [in] ALI object being used.
 *  \param ali [out] pointer to receive new ALI object
 *  \return Zero on success, or non-zero error code
 */
X400MT_CDECL int X400mtALIGet (
    struct X400mtMessage *mp,
    int entry,
    struct X400ALI **ali
);


struct X400PSS;


/*!
 *  \brief Create a new Printable String Syntax object for a message object
 *  \param mp [in] message pointer
 *  \param type [in] type of PSS being used.
 *  \param pss [out] pointer to receive pss object
 *  \return Zero on success, or non-zero error code
 */
X400MT_CDECL int X400mtPSSNew(
    struct X400mtMessage *mp,
    int type,
    struct X400PSS **pss
);

/*!
 *  \brief Copy a PrintableStringSequence object, and associate it with the
 *  a message.
 *  \param mp [in] message pointer
 *  \param orig [in] original PrintableStringSyntax object to copy
 *  \param type [in] type of PSS being used.
 *  \return new PrintableStringSequence object on success, or NULL on error
 */
X400MT_CDECL struct X400PSS *X400mtPSSDeepCopy (struct X400mtMessage *mp,
						struct X400PSS *orig,
						int type);


/*!
 *  \brief Get a new Printable String Syntax object for a message object
 *  \param mp [in] message pointer
 *  \param type [in] type of PSS being used.
 *  \param entry   [in] PSS object being used.
 *  \param pss [out] pointer to receive new PSS object
 *  \return Zero on success, or non-zero error code
 */
X400MT_CDECL int X400mtPSSGet (
    struct X400mtMessage *mp,
    int type,
    int entry,
    struct X400PSS **pss
);

struct X400DistField;


/*!
 *  \brief Create a new P772 Distribution Field object for a message object
 *  \param mp [in] message pointer
 *  \param distfield [out] pointer to receive distfield object
 *  \return Zero on success, or non-zero error code
 */
X400MT_CDECL int X400mtDistFieldNew(
    struct X400mtMessage *mp,
    struct X400DistField **distfield
);

/*!
 *  \brief Copy a P772 DistributionField object, and associate it with the
 *  a message.
 *  \param mp [in] message pointer
 *  \param orig [in] original object to copy
 *  \return new DistributionField object on success, or NULL on error
 */
X400MT_CDECL struct X400DistField *X400mtDistFieldDeepCopy (struct X400mtMessage *mp,
							    struct X400DistField *orig);

/*!
 *  \brief Get a new P772 Distribution Field object for a message object
 *  \param mp [in] message pointer
 *  \param entry   [in] distfield object being used.
 *  \param distfield [out] pointer to receive new PSS object
 *  \return Zero on success, or non-zero error code
 */
X400MT_CDECL int X400mtDistFieldGet (
    struct X400mtMessage *mp,
    int entry,
    struct X400DistField **distfield
);


struct X400OtherRecip;


/*!
 *  \brief Create a new P772 Other Recipient object for a message object
 *  \param mp [in] message pointer
 *  \param otherrecip [out] pointer to receive other recipient object
 *  \return Zero on success, or non-zero error code
 */
X400MT_CDECL int X400mtOtherRecipNew(
    struct X400mtMessage *mp,
    struct X400OtherRecip **otherrecip
);

/*!
 *  \brief Copy a P772 Other Recipient object, and associate it with the
 *  a message.
 *  \param mp [in] message pointer
 *  \param orig [in] original object to copy
 *  \return new OtherRecipient object on success, or NULL on error
 */
X400MT_CDECL struct X400OtherRecip *X400mtOtherRecipDeepCopy (struct X400mtMessage *mp,
							      struct X400OtherRecip *orig);

/*!
 *  \brief Get a new P772 Other Recipient object for a message object
 *  \param mp [in] message pointer
 *  \param entry [in] Other Recip object being used.
 *  \param otherrecip [out] pointer to receive new other recipient object
 *  \return Zero on success, or non-zero error code
 */
X400MT_CDECL int X400mtOtherRecipGet (
    struct X400mtMessage *mp,
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
X400MT_CDECL int X400mtRediHistGetEnv(struct X400mtMessage *msg,
				      int entry,
				      struct X400RediHist **hist);

/*!
    \brief Create a new Redirection History object for a message envelope this is represented by 8.3.1.2.1.5 in X.411
    \param msg  [in]  X.400 Redirection history
    \param hist   [out] Pointer to redirection history object
    \return Zero on success or non-zero error code
*/
X400MT_CDECL int X400mtRediHistNewEnv(
    struct X400mtMessage *msg,
    struct X400RediHist **hist
);

/*!
 *  \brief Copy a Redirection History object, and associate it with the
 *  a message.
 *  \param mp [in] message pointer
 *  \param orig [in] original object to copy
 *  \return new redirection History object on success, or NULL on error
 */
X400MT_CDECL struct X400RediHist *X400mtRediHistDeepCopyEnv(struct X400mtMessage *mp,
							    struct X400RediHist *orig);

/*!
    \brief Create new Originator and DL expansion history object
    \param msg  [in]  X.400 Originator and DL expansion history object
    \param or_and_dl [out] Pointer to Originator and DL expansion history object
    \return Zero on success or non-zero error code
*/
X400MT_CDECL int X400mtORandDLNew(
    struct X400mtMessage *msg,
    struct X400ORandDL **or_and_dl
);

/*!
 *  \brief Copy an Originator and DL Expansion History object, and associate it with the
 *  a message.
 *  \param mp [in] message pointer
 *  \param orig [in] original object to copy
 *  \return new object on success, or NULL on error
 */
X400MT_CDECL struct X400ORandDL *X400mtORandDLDeepCopy (struct X400mtMessage *mp,
							struct X400ORandDL *orig);

/*!
    \brief Get Originator and DL expansion history object
    \param msg   [in] X.400 Message
    \param entry [in] The orginator and dl expansion history object to use
    \param or_and_dl  [out] Pointer to orig ans dl expan history object
    \return Zero on success or non-zero error code
*/
X400MT_CDECL int X400mtORandDLGet(struct X400mtMessage *msg,
                                  int entry,
                                  struct X400ORandDL **or_and_dl);

struct X400ORandDL;
/*! \} */

/* Here we add the API versioning into the API, so that it is hidden
 * from the client
 */
/*! Macro calls X400mtOpenVer() with expected version */
#define X400mtOpen(p1, p2) X400mtOpenVer(p1, p2, X400_API_VERSN)

/*!
    \brief Open X400 MT session
    \param credentials [in]  Channel and MTA information
    \param spp         [out] Receives pointer to new session
    \param version     [in]  Version of API client is using
    \return Zero on success or non-zero error code
*/
X400MT_CDECL int X400mtOpenVer (const char *credentials,
			     struct X400mtSession **spp, int version);


/*!
    \brief Control how extension attributes in Envelopes and Envelope
    Recipient addresses are handled. Setting 'TRUE' for a given attribute
    type means that any incoming message which contains the corresponding
    extension field AND which is marked as critical-for-delivery or 
    critical-for-transfer will be automatically NDRed. In the case of
    addresses, the individual address will be NDRed. If all of the recipient
    addresses for which this MT instance is responsible are NDRed, calls to
    X400mtMsgGet and X400msMsgGetStart will return an error code as well
    as causing the NDR to be generated.

    \param spp         [in] Session to perform configuration on
    \param attType     [in] X.400 attribute type to configure
    \param fault       [in] Whether to fault this attribute type or not
    \return Zero on success or non-zero error code
*/
X400MT_CDECL int X400mtMarkToFault(struct X400mtSession *sp, int attType, int fault);



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
X400MT_CDECL int X400mtGetContentOctets(struct X400mtMessage *mp,
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
X400MT_CDECL int X400mtSetContentOctets(struct X400mtMessage *mp, 
					char *buf, size_t len);


#endif /* _X400_MTAPI_H */
