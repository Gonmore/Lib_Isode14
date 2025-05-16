/*  Copyright (c) 2004-2009, Isode Limited, London, England.
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

#ifndef X400_API_H

#define X400_API_H

#include "cdecl.h"
#include "x400_att.h"

/*! \file x400_api.h
    \brief X400 Object Interface

    This is an API for accessing X.400 Message, Report and Probe objects.
    It does not provide protocol interfaces for transfer, submission or
    delivery of such objects.

*/


/*! \defgroup X400 Pure X.400 API
   Description of the function calls for the X.400 object access interface.
 \{
*/

/* generic X400 recipient */
struct X400Recipient;

/* generic X400 message */
struct X400Message;

/* generic X400 Body Part (Attachment) */
struct X400Bodypart;

/* object for Certificate associated with message */
struct X400Certificate;

/* X400 attribute structure */
struct x400_attribute;

/* X400 DL Expansion List History*/
struct X400DLExpHist;

/* X400 Trace Information Object */
struct X400TraceInfo;

/* X400 Internal Trace Information Object */
struct X400InternalTraceInfo;

/* X400 Redirection History Object */
struct X400RediHist;

/* X400 Address List indicator */
struct X400ALI;

/* X400 Printable String Sequence */
struct X400PSS;

/* X400 Distribution Field Object*/
struct X400DistField;

/* X400 Other Recipient Indicator Object */
struct X400OtherRecip;

/* X400 ACP127 Response Notification */
struct X400ACP127Resp;

/* X400 Originator and DL history Object */
struct X400ORandDL;

/*!
    \brief Initialize X.400 API
    \param myname [in] Name of application
    \return zero on success or non-zero error code
*/
X400COMMON_CDECL int X400Initialize (char *myname);


/*!
    \brief Creates new message
    \param type [in] Type of message object
    \param mpp  [out] pointer to receive pointer to message object
    \return zero on success or non-zero error code

    Message types are
    - X400_MSG_MESSAGE    A Message object
    - X400_MSG_REPORT     A Report object
    - X400_MSG_PROBE      A Probe object
*/

X400COMMON_CDECL int X400MsgNew (int type,
				 struct X400Message **mpp);

/*!
    \brief Returns count of addresses of given type in message object
    \param mp   [in] X400 message
    \param recipient_type [in] Type of address to count
    \param number   [out] pointer integer to receive count of addresses
    \return zero on success or non-zero error code

*/

X400COMMON_CDECL int X400MsgCountRecip(struct X400Message *mp,
				       int recipient_type,
				       int *number);

/*!
    \brief Frees a message
    \param mp  [in] message object to free
    \return zero on success or non-zero error code
*/
X400COMMON_CDECL int X400MsgFree (struct X400Message *mp);

/*!
    \brief Add an attachment to the message
    \param mp        [in] Message pointer
    \param type      [in] Type of attachment
    \param string    [in] String value for attachment
    \param length    [in] Length of string
    \return Zero on success or non-zero error code

    Not all X.400 body part types can be added to a message in this way.
    If the type is such that a X400Bodypart is needed, then the
    error X400_E_COMPLEX_BODY is returned.

*/
X400COMMON_CDECL int X400MsgAddAttachment (struct X400Message *mp,
					   int type,
					   const char *string,
					   size_t length);


/*!
    \brief Add a body part object to a message
    \param mp        [in]  Message pointer
    \param bp        [in] Bodypart pointer
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400MsgAddBodypart (struct X400Message *mp,
					 struct X400Bodypart *bp);

/*! \deprecated Use X400MsgAddMessageBodyWType instead.
*/
X400COMMON_CDECL int X400MsgAddMessageBody (struct X400Message *mp,
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
X400COMMON_CDECL int X400MsgAddMessageBodyWType (struct X400Message *mp,
                                                 struct X400Message *mbp,
                                                 int type);

/*!
    \brief Add integer-valued parameter to the message
    \param mp        [in] Message pointer
    \param paramtype [in] Type of parameter
    \param value     [in] Value of parameter
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400MsgAddIntParam (struct X400Message *mp,
				      int paramtype,
				      int value);

/*!
    \brief Add string-valued parameter to the message
    \param mp        [in] Message pointer
    \param paramtype [in] Type of parameter
    \param value     [in] Value of parameter
    \param length    [in] Length of parameter, -1 if NUL terminated
    \return Zero on success or non-zero error code
*/

X400COMMON_CDECL int X400MsgAddStrParam (struct X400Message *mp,
					 int paramtype,
					 const char *value,
					 size_t length);

/*!
    \brief Add a recipient object to the message
    \param mp        [in] Message pointer
    \param reciptype [in] Type of recipient
    \param recip     [in] Recipient object to add
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400MsgAddRecip(struct X400Message *mp,
				     int reciptype,
				     struct X400Recipient *recip);

/*!
    \brief Return a integer-valued parameter from the message object
    \param mp        [in] message object pointer
    \param paramtype [in] type of parameter
    \param valuep      [out] Pointer to receive value
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400MsgGetIntParam(struct X400Message *mp,
				     int paramtype,
				     int *valuep);

/*!
    \brief Return a string-valued parameter from the message object
    \param mp        [in] message object pointer
    \param paramtype [in] type of parameter
    \param buffer    [in/out] address for result
    \param buflen    [in] length of buffer provided
    \param paramlenp  [out] Pointer to receive actual length
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400MsgGetStrParam(struct X400Message *mp,
				     int paramtype,
				     char *buffer,
				     size_t buflen,
				     size_t *paramlenp);

/*!
    \brief Return data from a simple attachment
    \param mp        [in] message object pointer
    \param number    [in] Attachment number
    \param typep     [out] Pointer to receive attachment type
    \param buffer    [in] Buffer to receive string
    \param buflen    [in] Buffer length
    \param lengthp   [out] Pointer to receive length of string
    \return Zero on success or non-zero error code

    Not all X.400 body part types can be returned from a message in this way.
    If the type is such that a X400Bodypart is needed, then the
    error X400_E_COMPLEX_BODY is returned. If the type is a forwarded
    message or returned IPM, then X400_E_MESSAGE_BODY is returned.

*/
X400COMMON_CDECL int X400MsgGetAttachment (struct X400Message *mp,
					int number,
					int *typep,
					char *buffer,
					size_t buflen,
					size_t *lengthp);

/*!
                                int number,
				int *typep,
				struct X400Bodypart **bpp)
    \brief Return a pointer to a body part object
    \param mp        [in] message object pointer
    \param number    [in] Bodypart number
    \param typep     [out] Pointer to receive attachment type
    \param bpp       [out] Pointer to pointer to object
    \return Zero on success or non-zero error code

    If the body part is a forwarded message body part or a returned IPM
    then the error X400_E_MESSAGE_BODY is returned.
*/
X400COMMON_CDECL int X400MsgGetBodypart (struct X400Message *mp,
					 int number,
					 int *typep,
					 struct X400Bodypart **bpp);

/*!
    \brief Return a pointer to a body part object
    \param mp        [in] message object pointer
    \param number    [in] Bodypart number
    \param mbpp      [out] Pointer to address of BodyPart
    \return Zero on success or non-zero error code

*/
X400COMMON_CDECL int X400MsgGetMessageBody (struct X400Message *mp,
					    int number,
					    struct X400Message **mbpp);

/*!
    \brief Get recipient object from message
    \param mp        [in] Message object pointer
    \param recip_type      [in] Type of recipient
    \param number    [in] Ordinal number of recipient
    \param rpp       [out] Pointer to receive recipient object pointer
    \return Zero on success, non-zero on failure
*/

X400COMMON_CDECL int X400MsgGetRecip(struct X400Message *mp,
				  int recip_type,
				  int number,
				  struct X400Recipient **rpp);

/*!
    \brief Get certificate object from message
    This returns a certificate which was used to sign an object in the
    message. The attribute type indicates which object type.
    \param mp        [in] Message object pointer
    \param certtype  [in] Attribute type for certificate
    \param certp     [out] Pointer to receive certificate object pointer
    \return Zero on success, non-zero on failure
*/

X400COMMON_CDECL int X400MsgGetCert(struct X400Message *mp,
				    int certtype,
				    struct X400Certificate **certp);


/*!
    \brief Create a new recipient object
    \param type [in] type of recipient
    \param rpp  [out] Pointer to place for pointer to recipient information
    \return Zero on success or non-zero error code

    Creates a recipient object.
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
X400COMMON_CDECL int X400RecipNew (int type,
				   struct X400Recipient **rpp);



/*!
    \brief Free a recipient object
    \param rpp  [out] Recipient object to free
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400RecipFree (struct X400Recipient *rpp);

/*!
    \brief Add integer-valued parameter to the recipient
    \param rp        [in] Message pointer
    \param paramtype [in] Type of parameter
    \param value     [in] Value of parameter
    \return Zero on success or non-zero error code
*/

X400COMMON_CDECL int X400RecipAddIntParam (struct X400Recipient *rp,
					int paramtype,
					int value);

/*!
    \brief Add string-valued parameter to the recipient
    \param rp        [in] Recipient pointer
    \param paramtype [in] Type of parameter
    \param value     [in] Value of parameter
    \param length    [in] Length of parameter, -1 if NUL terminated
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400RecipAddStrParam (struct X400Recipient *rp,
					int paramtype,
					const char *value,
					size_t length);

/*!
    \brief Return a string-valued parameter from the recipient object
    \param rp        [in] recipient object pointer
    \param paramtype [in] type of parameter
    \param buffer    [in/out] address for result
    \param buflen    [in] length of buffer provided
    \param paramlenp  [out] Pointer to receive actual length
    \return Zero on success or non-zero error code
*/

X400COMMON_CDECL int X400RecipGetStrParam (struct X400Recipient *rp,
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
X400COMMON_CDECL int X400RecipGetIntParam (struct X400Recipient *rp,
					int paramtype,
					int *valp);



/*!
    \brief Get certificate object from recipient
    This returns a certificate which was used to sign an object in the
    recipient. The attribute type indicates which object type.
    \param mp        [in] Recipient object pointer
    \param certtype  [in] Attribute type for certificate
    \param certp     [out] Pointer to receive certificate object pointer
    \return Zero on success, non-zero on failure
*/

X400COMMON_CDECL int X400RecipGetCert(struct X400Recipient *rp,
				      int certtype,
				      struct X400Certificate **certp);


/*!
    \brief Create a new body part object
    \param type [in] type of bodypart
    \param bpp  [out] Pointer to place for pointer to body part object
    \return Zero on success or non-zero error code

    Creates a body part object.
    There are several different types for body part object.
*/
X400COMMON_CDECL int X400BodypartNew (int type,
				      struct X400Bodypart **bpp);



/*!
    \brief Free a body part object
    \param bpp  [out] body part object to free
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400BodypartFree (struct X400Bodypart *bpp);

/*!
    \brief Add integer-valued parameter to the body part
    \param bp        [in] Body part pointer
    \param paramtype [in] Type of parameter
    \param value     [in] Value of parameter
    \return Zero on success or non-zero error code
*/

X400COMMON_CDECL int X400BodypartAddIntParam (struct X400Bodypart *bp,
					      int paramtype,
					      int value);

/*!
    \brief Add string-valued parameter to the body part
    \param bp        [in] Body part pointer
    \param paramtype [in] Type of parameter
    \param value     [in] Value of parameter
    \param length    [in] Length of parameter, -1 if NUL terminated
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400BodypartAddStrParam (struct X400Bodypart *bp,
					      int paramtype,
					      const char *value,
					      size_t length);

/*!
    \brief Return a string-valued parameter from the body part object
    \param bp        [in] body part object pointer
    \param paramtype [in] type of parameter
    \param buffer    [in/out] address for result
    \param buflen    [in] length of buffer provided
    \param paramlenp  [out] Pointer to receive actual length
    \return Zero on success or non-zero error code
*/

X400COMMON_CDECL int X400BodypartGetStrParam (struct X400Bodypart *bp,
					      int paramtype,
					      char *buffer,
					      size_t buflen,
					      size_t *paramlenp);

/*!
    \brief Return a integer-valued parameter from the body part object
    \param bp        [in] bodypart object pointer
    \param paramtype [in] type of parameter
    \param valp      [out] Pointer to receive value
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400BodypartGetIntParam (struct X400Bodypart *bp,
					      int paramtype,
					      int *valp);



/*!
    \brief Return a new defaults object.
    \param ap [out] Address of pointer to receive defaults object
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400DefaultsNew (struct x400_attribute **ap);

/*!
    \brief Free a defaults object.
    \param ap [in] Address of defaults object to free
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400DefaultsFree (struct x400_attribute *ap);


/*!
    \brief Set a string value in a defaults object
    \param ap [in] Defaults object to operate on
    \param paramtype [in] Type of parameter to set
    \param value [in] String value to set
    \param length [in] Length of value to set
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400SetStrDefault(struct x400_attribute *ap,
				    int paramtype,
				    const char *value,
				    size_t length);

/*!
    \brief Set an integer value in a defaults object
    \param ap [in] Defaults object to operate on
    \param paramtype [in] Type of parameter to set
    \param value [in] Integer value to set
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400SetIntDefault(struct x400_attribute *ap,
				    int paramtype,
				    int value);

/*!
    \brief Return string for error code
    \param error [in] error code
    \return Pointer to NUL terminated string for error
*/
X400COMMON_CDECL const char *X400Error(int error);

/*!
    \brief Create a new DL Expansion History object from the message object
    \param mp    [in] message object pointer
    \param entry [in] DL expansion history element to fetch
    \param hist  [out] pointer to receive new DL Expansion History object
    \return Zero on success, or non-zero error code
*/
X400COMMON_CDECL int X400DLExpHistGet (struct X400Message *mp,
				       int entry,
				       struct X400DLExpHist **hist);

/*!
    \brief Delete the DL Expansion History object
    \param hist [in] DL Expansion History object to be deleted
    \return Zero on success, or non-zero error code.
 */
X400COMMON_CDECL int X400DLExpHistDelete (struct X400DLExpHist *hist);

/*!
    \brief Return a string parameter from the DL Expansion History object
    \param hist      [in] DL Expansion History object pointer
    \param paramtype [in] type of parameter
    \param buffer    [in/out] pointer to receive value
    \param buflen    [in] Size of buffer
    \param paramlenp [out] pointer to receive actual length
    \return Zero on success, or non-zero error code
*/
X400COMMON_CDECL int X400DLExpHistGetStrParam (struct X400DLExpHist *hist,	      
					int paramtype,
					char *buffer,
					size_t buflen,
					size_t *paramlenp);

/* For backwards compatibility - function had wrong name originally */
#define X400DLGetStrParam X400DLExpHistGetStrParam

/*!
    \brief Create a new X400DLExpHist object
    \param histp [out] pointer to receive object
    \return Zero on success, or non-zero error code
 */
X400COMMON_CDECL int X400DLExpHistNew (
    struct X400DLExpHist **histp
);

/*!
   \brief Associates a X400DLExpHist object with a X400Message object
   \param mp [in] Pointer to X400Message object
   \param histp [in] Pointer to X400DLExpHist object.
   \return Zero on success, or non-zero error code
 */
X400COMMON_CDECL int X400MsgAddDLExpHist(
    struct X400Message *mp,
    struct X400DLExpHist *histp
);


/*!
    \brief Add string-valued parameter to the X400DLExpHist object
    \param hist      [in] X400DLExpHist pointer
    \param paramtype [in] Type of parameter
    \param value     [in] Value of parameter
    \param length    [in] Length of parameter, -1 if NULL terminated
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400DLExpHistAddStrParam (
    struct X400DLExpHist *hist,
    int paramtype,
    const char *value,
    size_t length
);

/*!
    \brief Copy an existing DL Expansion History object
    \param original  [in]  X.400 DL Expansion History to copy
    \return Copy of DL Expansion History or NULL on error
*/
X400COMMON_CDECL struct X400DLExpHist *X400DLExpHistDeepCopy(struct X400DLExpHist *original);


/*!
    \brief Get the Trace information object from a message
    \param mp      [in] X.400 Message pointer
    \param entry     [in] Trace Info object to use
    \param info    [out] Pointer to trace info object
    \param trace_info_type [in] Type of trace info object.
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400TraceInfoGet(struct X400Message *mp,
				      int entry,
				      struct X400TraceInfo **info,
				      int trace_info_type);

/*!
    \brief Get string parameter from Trace Information object
    \param info      [in] Trace Info object
    \param paramtype [in] Type of parameter
    \param buffer    [out] Buffer to contain string value
    \param buflen    [in] Length of buffer
    \param paramlenp [out] Length of require buffer
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400TraceInfoGetStrParam (struct X400TraceInfo *info,
					       int paramtype,
					       char *buffer,
					       size_t buflen ,
					       size_t *paramlenp);
/*!
    \brief Get int parameter from Trace Information object
    \param info      [in] Trace Info object
    \param paramtype [in] Type of parameter
    \param valuep    [out] pointer to integer to return
    \return Zero on success or non-zero error code  
*/
X400COMMON_CDECL int X400TraceInfoGetIntParam (struct X400TraceInfo *info,
					       int paramtype,
					       int *valuep);



/*!
    \brief Create a new Trace information object.
    \param msg     [in] X400 message pointer
    \param info    [out] Pointer to trace info object
    \param type    [in] Type info object X400_TRACE_INFO or X400_SUBJECT_TRACE_INFO
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400TraceInfoNew(
    struct X400Message    *msg,
    struct X400TraceInfo **info,
    int type
);


/*!
    \brief Copy an existing Trace information object.
    \param mp   [in]  X400 message pointer (destination of copy object)
    \param info [in] Pointer to original trace info object
    \param type [in] Either X400_TRACE_INFO or X400_SUBJECT_TRACE_INFO
    \return Copy of object or NULL on error
*/
X400COMMON_CDECL struct X400TraceInfo *X400TraceInfoDeepCopy(struct X400Message *mp,
							     struct X400TraceInfo *original,
							     int type);

/*!
    \brief Create a new Trace information object.
    \param mp   [in]  X400 message pointer
    \param info [in] Pointer to trace info object
    \param type [in] Either X400_TRACE_INFO or X400_SUBJECT_TRACE_INFO
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400MsgAddTraceInfo(
    struct X400Message *mp,
    struct X400TraceInfo *info,
    int type
);

/*!
    \brief Add string-valued parameter to the X400TraceInfo object
    \param info      [in] X400TraceInfo pointer
    \param paramtype [in] Type of parameter
    \param value     [in] Value of parameter
    \param length    [in] Length of parameter, -1 if NULL terminated
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400TraceInfoAddStrParam (
    struct X400TraceInfo *info,
    int paramtype,
    const char *value,
    size_t length
);


/*!
    \brief Set an integer value in a Trace Info object
    \param info [in] Trace Info object to operate on
    \param paramtype [in] Type of parameter to set
    \param value [in] Integer value to set
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400TraceInfoAddIntParam(
    struct X400TraceInfo *info,
    int paramtype,
    int value
);


/*!
    \brief Add string-valued parameter to the X400RediHist object
    \param hist      [in] X400RediHist pointer
    \param paramtype [in] Type of parameter
    \param value     [in] Value of parameter
    \param length    [in] Length of parameter, -1 if NULL terminated
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400RediHistAddStrParam (
    struct X400RediHist *hist,
    int paramtype,
    const char *value,
    size_t length
);

/*!
    \brief Set an integer value in a Redirection History object
    \param hist [in] Redirection History object to operate on
    \param paramtype [in] Type of parameter to set
    \param value [in] Integer value to set
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400RediHistAddIntParam (
    struct X400RediHist *hist,
    int paramtype,
    int value
);


/*!
    \brief Get string-valued parameter from the X400RediHist object
    \param hist      [in] X400RediHist pointer
    \param paramtype [in] Type of parameter
    \param value     [in] Value of parameter
    \param length    [in] Length of parameter
    \param lengthp   [in] Required length of value
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400RediHistGetStrParam (
    struct X400RediHist *hist,
    int paramtype,
    char *value,
    size_t length,
    size_t *lengthp
);


/*!
    \brief Get int parameter from Redirection History object
    \param info      [in] Redirection History object
    \param paramtype [in] Type of parameter
    \param valuep    [out] pointer to integer to return
    \return Zero on success or non-zero error code  
*/
X400COMMON_CDECL int X400RediHistGetIntParam (struct X400RediHist *info,
					      int paramtype,
					      int *valuep);

/*!
    \brief Get the Redirection History object from a recipient
    \param recip   [in] X.400 Recipient
    \param entry   [in] Trace Info object to use
    \param hist    [out] Pointer to redirection history object
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400RediHistGet(struct X400Recipient *recip,
				     int entry,
				     struct X400RediHist **hist);

/*!
    \brief Create a new Redirection History object.
    \param recip  [in]  X.400 Recipient history
    \param hist   [out] Pointer to redirection history object
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400RediHistNew(
    struct X400Recipient *recip,
    struct X400RediHist **hist
);

/*!
    \brief Get the Redirection History object from a message envelope this is represented by 8.3.1.2.1.5 in X.411
    \param msg   [in] X.400 Message
    \param entry   [in] Trace Info object to use
    \param hist    [out] Pointer to redirection history object
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400RediHistGetEnv(struct X400Message *msg,
					int entry,
					struct X400RediHist **hist);

/*!
    \brief Create a new Redirection History object for a message envelope this is represented by 8.3.1.2.1.5 in X.411
    \param msg  [in]  X.400 Recipient history
    \param hist   [out] Pointer to redirection history object
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400RediHistNewEnv(
    struct X400Message *msg,
    struct X400RediHist **hist
);


/*!
    \brief Copy an existing Redirection History object into a message
    \param msg   [in] X.400 Message
    \param original  [in]  X.400 Redirection history to copy
    \return Copy of Redirection History or NULL on error
*/
X400COMMON_CDECL struct X400RediHist *X400RediHistDeepCopyEnv(struct X400Message *msg,
							      struct X400RediHist *original);


/*!
    \brief Copy an existing Redirection History object into a recipient
    \param recip   [in] X.400 Recipient
    \param original  [in]  X.400 Redirection history to copy
    \return Copy of Redirection History or NULL on error
*/
X400COMMON_CDECL struct X400RediHist *X400RediHistDeepCopy(struct X400Recipient *recip,
							   struct X400RediHist *original);

/*!
    \brief Return a human readable string based on a redirection reason
    \param redirection_reason   [in] redirection reason
    \return Pointer to human readable string.
*/
X400COMMON_CDECL const char * X400RediReason(
    int redirection_reason
);


/*!
    \brief Get the Trace information object from a message
    \param mp      [in] X.400 Message pointer
    \param entry   [in] Trace Info object to use
    \param info    [out] Pointer to trace info object
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400InternalTraceInfoGet(struct X400Message *mp,
					      int entry,
					      struct X400InternalTraceInfo **info);

/*!
    \brief Copy an existing Internal Trace information object.
    \param mp   [in]  X400 message pointer (destination of copy object)
    \param info [in] Pointer to original trace info object
    \return Copy of object or NULL on error
*/
X400COMMON_CDECL struct X400InternalTraceInfo *X400InternalTraceInfoDeepCopy(
     struct X400Message *mp,
     struct X400InternalTraceInfo *original);

/*!
    \brief Create a new Trace information object.
    \param msg     [in] X400 message pointer
    \param info    [out] Pointer to trace info object
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400InternalTraceInfoNew(
    struct X400Message    *msg,
    struct X400InternalTraceInfo **info
);


/*!
    \brief Create a new Trace information object.
    \param mp   [in]  X400 message pointer
    \param info [in] Type of trace info object to add
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400MsgAddInternalTraceInfo(
    struct X400Message *mp,
    struct X400InternalTraceInfo *info
);

/*!
    \brief Get string parameter from Internal Trace Information object
    \param info      [in] Trace Info object
    \param paramtype [in] Type of parameter
    \param buffer    [out] Buffer to contain string value
    \param buflen    [in] Length of buffer
    \param paramlenp [out] Length of require buffer
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400InternalTraceInfoGetStrParam (struct X400InternalTraceInfo *info,
						       int paramtype,
						       char *buffer,
						       size_t buflen ,
						       size_t *paramlenp);

/*!
    \brief Get int parameter from Internal Trace Information object
    \param info      [in] Trace Info object
    \param paramtype [in] Type of parameter
    \param valuep    [out] pointer to integer to return
    \return Zero on success or non-zero error code  
*/
X400COMMON_CDECL int X400InternalTraceInfoGetIntParam (struct X400InternalTraceInfo *info,
						       int paramtype,
						       int *valuep);

/*!
    \brief Add string-valued parameter to the Internal Trace info object
    \param info      [in] Internal Trace info pointer
    \param paramtype [in] Type of parameter
    \param value     [in] Value of parameter
    \param length    [in] Length of parameter, -1 if NULL terminated
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400InternalTraceInfoAddStrParam (
    struct X400InternalTraceInfo *info,
    int paramtype,
    const char *value,
    size_t length
);


/*!
    \brief Set an integer value in an Internal Trace Info object
    \param info      [in] Trace Info object to operate on
    \param paramtype [in] Type of parameter to set
    \param value     [in] Integer value to set
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400InternalTraceInfoAddIntParam(
    struct X400InternalTraceInfo *info,
    int paramtype,
    int value
);



/*!
    \brief Get the Address List Indicator information object from a message
    \param mp      [in] X.400 Message pointer
    \param entry   [in] ALI object being used.
    \param ali     [out] Pointer to ALI object
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400ALIGet(struct X400Message *mp,
				int entry,
				struct X400ALI **ali);

/*!
    \brief Create an Address List Indicator object.
    \param msg     [in] X400 message pointer
    \param ali     [out] Pointer ALI object
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400ALINew(
    struct X400Message    *msg,
    struct X400ALI **ali
);

/*!
    \brief Copy an Address List Indicator object.
    \param msg     [in] X400 message pointer (for copy)
    \param ali     [out] ALI object to copy
    \return Copy of ALI or NULL on error
*/
X400COMMON_CDECL struct X400ALI *X400ALIDeepCopy(struct X400Message *mp,
						 struct X400ALI *ali);

/*!
    \brief Get string parameter from Address List Indicator object
    \param ALI       [in] ALI object
    \param paramtype [in] Type of parameter
    \param buffer    [out] Buffer to contain string value
    \param buflen    [in] Length of Buffer
    \param paramlenp [out] Length of required buffer
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400ALIGetStrParam (struct X400ALI *ALI,
					 int paramtype,
					 char *buffer,
					 size_t buflen ,
					 size_t *paramlenp);

/*!
    \brief Get int parameter from Address List Indicator object
    \param ALI       [in] ALI object
    \param paramtype [in] Type of parameter
    \param valuep    [out] pointer to integer to return
    \return Zero on success or non-zero error code  
*/
X400COMMON_CDECL int X400ALIGetIntParam (struct X400ALI *ALI,
					 int paramtype,
					 int *valuep);

/*!
    \brief Add string-valued parameter to the ALI info object
    \param info      [in] ALI info pointer
    \param paramtype [in] Type of parameter
    \param value     [in] Value of parameter
    \param length    [in] Length of parameter, -1 if NULL terminated
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400ALIAddStrParam (
    struct X400ALI *info,
    int paramtype,
    const char *value,
    size_t length
);


/*!
    \brief Set an integer value in an Address List Indicator object
    \param info      [in] ALI object to operate on
    \param paramtype [in] Type of parameter to set
    \param value     [in] Integer value to set
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400ALIAddIntParam(
    struct X400ALI *info,
    int paramtype,
    int value
);

/*!
    \brief Get the Printable String Sequence object from a message
    \param mp      [in] X.400 Message pointer
    \param type    [in] type of printable string syntax to fetch
    \param entry   [in] PSS object being used.
    \param pss     [out] Pointer to PSS object
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400PSSGet(
    struct X400Message *mp,
    int type,
    int entry,
    struct X400PSS **pss
);

/*!
    \brief Copy a Printable String Sequence object.
    \param msg     [in] X400 message pointer (for copy)
    \param pss     [out] PSS object to copy
    \param type    [in] Type of PSS object
    \return Copy of PSS or NULL on error
*/
X400COMMON_CDECL struct X400PSS *X400PSSDeepCopy(struct X400Message *mp,
						 struct X400PSS *pss,
						 int type);

/*!
    \brief Create a Printable String Sequence object.
    \param msg     [in] X400 message pointer
    \param pss     [out] Pointer PSS object
    \param type    [in] Type of PSS object
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400PSSNew(
    struct X400Message    *msg,
    struct X400PSS **pss,
    int type
);

/*!
    \brief Get string parameter from Printable String Sequence object
    \param pss       [in] PSS object
    \param buffer    [out] Buffer to contain string value
    \param buflen    [in] Length of Buffer
    \param paramlenp [out] Length of required buffer
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400PSSGetStrParam (
    struct X400PSS *pss,
    char *buffer,
    size_t buflen ,
    size_t *paramlenp
);

/*!
    \brief Add string-valued parameter to the PSS info object
    \param pss      [in] pss info pointer
    \param value     [in] Value of parameter
    \param length    [in] Length of parameter, -1 if NULL terminated
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400PSSAddStrParam (
    struct X400PSS *pss,
    const char *value,
    size_t length
);
/*!
    \brief Get the Distribution Code extension Field object from a message
    \param mp      [in] X.400 Message pointer
    \param entry   [in] DistField object being used.
    \param distfield [out] Pointer to Distribution Code extension object
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400DistFieldGet(
    struct X400Message *mp,
    int entry,
    struct X400DistField **distfield
);

/*!
    \brief Copy a P772 Distribution Code extension Field object.
    \param msg     [in] X400 message pointer for copy
    \param distfield     [out] X400DistField object to copy
    \return New X400DistField or NULL on error
*/
X400COMMON_CDECL struct X400DistField *X400DistFieldDeepCopy(struct X400Message *mp,
							     struct X400DistField *original);

/*!
    \brief Create a P772 Distribution Code extension Field object.
    \param msg     [in] X400 message pointer
    \param distfield     [out] Pointer X400DistField object
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400DistFieldNew(
    struct X400Message    *msg,
    struct X400DistField **distfield
);

/*!
    \brief Get string parameter from Distribution Code extension Field object
    \param distfield       [in] DistField object
    \param type      [in] The type of distfield object
    \param buffer    [out] Buffer to contain string value
    \param buflen    [in] Length of Buffer
    \param paramlenp [out] Length of required buffer
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400DistFieldGetStrParam (
    struct X400DistField *distfield,
    int type,
    char *buffer,
    size_t buflen ,
    size_t *paramlenp
);

/*!
    \brief Add string-valued parameter to the DistField info object
    \param distfield [in] distfield info pointer
    \param type      [in] The type of distfield object
    \param value     [in] Value of parameter
    \param length    [in] Length of parameter, -1 if NULL terminated
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400DistFieldAddStrParam (
    struct X400DistField *distfield,
    int type,
    const char *value,
    size_t length
);

/*!
 *  \brief Get a new P772 Other Recipient object for a message object
 *  \param mp [in] message pointer
 *  \param entry [in] Other Recip object being used.
 *  \param otherrecip [out] pointer to receive new other recipient object
 *  \return Zero on success, or non-zero error code
 */
X400COMMON_CDECL int X400OtherRecipGet (
    struct X400Message *mp,
    int entry,
    struct X400OtherRecip **otherrecip
);

/*!
 *  \brief Copy a P772 Other Recipient object
 *  \param mp [in] message pointer for copy
 *  \param original [in] object to copy
 *  \return Copy of object or NULL on error
 */
X400COMMON_CDECL struct X400OtherRecip *X400OtherRecipDeepCopy(struct X400Message *mp,
							       struct X400OtherRecip *original);

/*!
 *  \brief Create a new P772 Other Recipient object for a message object
 *  \param mp [in] message pointer
 *  \param otherrecip [out] pointer to receive other recipient object
 *  \return Zero on success, or non-zero error code
 */
X400COMMON_CDECL int X400OtherRecipNew(
    struct X400Message *mp,
    struct X400OtherRecip **otherrecip
);

/*!
    \brief Get string parameter from Other Recipient object
    \param otherrecip       [in] otherrecip object
    \param buffer    [out] Buffer to contain string value
    \param buflen    [in] Length of Buffer
    \param paramlenp [out] Length of required buffer
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400OtherRecipGetStrParam (
    struct X400OtherRecip *otherrecip,
    char *buffer,
    size_t buflen ,
    size_t *paramlenp
);

/*!
    \brief Add string-valued parameter to the Other Recipient Indicator object
    \param otherrecip [in] Other Recipient object pointer
    \param value     [in] Value of parameter
    \param length    [in] Length of parameter, -1 if NULL terminated
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400OtherRecipAddStrParam (
    struct X400OtherRecip *otherrecip,
    const char *value,
    size_t length
);

/*!
    \brief Add integer parameter to the Other Recipient Indicator object
    \param otherrecip [in] Other Recipient object pointer
    \param value     [in] Value of parameter
    \return Zero on success or non-zero error code
*/

X400COMMON_CDECL int X400OtherRecipAddIntParam(
    struct X400OtherRecip *otherrecip,
    int value
);


/*!
    \brief Get integer parameter from the Other Recipient Indicator object
    \param otherrecip [in] Other Recipient object pointer
    \param value     [out] address of value integer
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400OtherRecipGetIntParam(
    struct X400OtherRecip *otherrecip,
    int *value
);




/*!
    \brief Get the ACP127 Response object from a recipient
    \param rp      [in] X.400 Recipient pointer
    \param resp [out] Pointer to ACP127 Response object
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400ACP127RespGet(
    struct X400Recipient *rp,
    struct X400ACP127Resp **resp
);

/*!
 *  \brief Copy a P772 ACP127 Recipient object
 *  \param mp [in] recipient pointer for copy
 *  \param original [in] object to copy
 *  \return Copy of object or NULL on error
 */
X400COMMON_CDECL struct X400ACP127Resp *X400ACP127RespDeepCopy(struct X400Recipient *rp,
							       struct X400ACP127Resp *original);

/*!
    \brief Create a P772 ACP127 Response object.
    \param rp     [in] X400 recipient pointer
    \param resp    [out] Pointer X400ACP127Resp object
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400ACP127RespNew(
    struct X400Recipient    *rp,
    struct X400ACP127Resp **resp
);

/*!
    \brief Get string parameter from ACP127 Response object
    \param resp [in] ACP127 Repsonse object
    \param type      [in] The type of string element to get
    \param buffer    [out] Buffer to contain string value
    \param buflen    [in] Length of Buffer
    \param paramlenp [out] Length of required buffer
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400ACP127RespGetStrParam (
    struct X400ACP127Resp *resp,
    int type,
    char *buffer,
    size_t buflen ,
    size_t *paramlenp
);


/*!
    \brief Add string-valued parameter to the ACP127 Response object
    \param resp [in] ACP127 Response object pointer
    \param type [in] Type of value to be added
    \param value     [in] Value of parameter
    \param length    [in] Length of parameter, -1 if NULL terminated
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400ACP127RespAddStrParam (
    struct X400ACP127Resp *resp,
    int type,
    const char *value,
    size_t length
);

/*!
    \brief Add integer parameter to the ACP127 Response object
    \param resp [in] ACP127 Response object pointer
    \param value     [in] Value of parameter
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400ACP127RespAddIntParam(
    struct X400ACP127Resp *resp,
    int value
);


/*!
    \brief Get integer parameter from the ACP127 Response object
    \param resp  [in] ACP127 object pointer
    \param value [out] address of value integer
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400ACP127RespGetIntParam(
    struct X400ACP127Resp *resp,
    int *value
);


/*!
    \brief Create new Address List Indicator object associated with the X400ACP127 Resp.
    \param resp  [in] ACP127 response object.
    \param ali [out] X400ALI Address List Indicator object 
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int  X400ACP127RespNewALI (
    struct X400ACP127Resp *resp,
    struct X400ALI **ali
);

/*!
  \brief Get an Address List Indicator object associated with the X400ACP127 object.
  \param resp  [in] ACP127 response object.
  \param ali [out] X400ALI Address List Indicator object
  \param entry [in] The X400ALI object to get. 
  \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400ACP127RespGetALI (
    struct X400ACP127Resp *resp,
    struct X400ALI **ali,
    int entry
);


/*!
  \brief Add string parameter for a Originator and DL Expansion History object
  \param or_and_dl [in] X400ORandDL Repsonse object
  \param type [in] The type of string element to add
  \param value [in] The sting to add
  \param length [in] The size of the string to add.
  \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400ORandDLAddStrParam (
    struct X400ORandDL *or_and_dl,
    int type,
    const char *value,
    size_t length
);

/*!
    \brief Copy Originator and DL Expansion history object.
    \param msg       [in]  Message object pointer for copy
    \param or_and_dl [in] Original to copy
    \return New object or NULL on error
*/
X400COMMON_CDECL struct X400ORandDL *X400ORandDLDeepCopy(struct X400Message *msg,
							 struct X400ORandDL *orig);

/*!
    \brief Create new Originator and DL Expansion history object.
    \param msg       [in]  Message object pointer
    \param or_and_dl [out] Pointer to receive new DL expansion object
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400ORandDLNew(
    struct X400Message *msg,
    struct X400ORandDL **or_and_dl
);

/*!
    \brief Get Originator and DL expansion history object
    \param msg   [in] X.400 Message
    \param entry [in] The orginator and dl expansion history object to use
    \param or_and_dl  [out] Pointer to orig and dl expan history object
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400MsgORandDLGet(
    struct X400Message  *msg,
    int entry,
    struct X400ORandDL **or_and_dl
);


/*!
    \brief Get string parameter from ACP127 Response object
    \param or_and_dl [in] X400ORandDL object
    \param paramtype [in] The type of string element to get
    \param buffer    [out] Buffer to contain string value
    \param buflen    [in] Length of Buffer
    \param paramlenp [out] Length of required buffer
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400ORandDLGetStrParam (
    struct X400ORandDL *or_and_dl,
    int paramtype,
    char *buffer,
    size_t buflen ,
    size_t *paramlenp
);

/*!
    \brief Return a string-valued parameter from the certificate object
    \param rp        [in] certificate object pointer
    \param paramtype [in] type of parameter
    \param buffer    [in/out] address for result
    \param buflen    [in] length of buffer provided
    \param paramlenp  [out] Pointer to receive actual length
    \return Zero on success or non-zero error code
*/

X400COMMON_CDECL int X400CertGetStrParam (struct X400Certificate *cp,
					  int paramtype,
					  char *buffer,
					  size_t buflen,
					  size_t *paramlenp);

/*!
    \brief Return a integer-valued parameter from the certificate object
    \param rp        [in] certificate object pointer
    \param paramtype [in] type of parameter
    \param valp      [out] Pointer to receive value
    \return Zero on success or non-zero error code
*/
X400COMMON_CDECL int X400CertGetIntParam (struct X400Certificate *cp,
					  int paramtype,
					  int *valp);


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
X400COMMON_CDECL int X400GetContentOctets(struct X400Message *mp,
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
X400COMMON_CDECL int X400SetContentOctets(struct X400Message *mp, 
					  char *buf, size_t len);

/* \} */

#endif
