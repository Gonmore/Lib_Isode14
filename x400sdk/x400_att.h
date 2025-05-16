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

/*! \file x400_att.h
 *  \brief X400 Symbolic Constants
 */

#ifndef _X400_ATT_H
#define _X400_ATT_H

#define X400_API_VERSN 14040000
/*!< API version */

/*! \defgroup msgtype Message Types
  \{ */

#define X400_MSG_MESSAGE  1
/*!< Message (Envelope and Content) */

#define X400_MSG_REPORT   2
/*!< Report (Envelope, Report content and optional returned content) */

#define X400_MSG_PROBE    3
/*!< Probe (Envelope only) */

#define X400_MSG_SUBMITTED_MESSAGE 4
/*!< Submitted message (Envelope and Content) */

/* \} */

/*! \defgroup errs Error codes
  \{ */

#define X400_E_NOERROR        0
/*!< No error */

#define X400_E_SYSERROR       1
/*!< General system error */

#define X400_E_NOMEMORY       2
/*!< Insufficient memory */

#define X400_E_BADPARAM       3
/*!< Bad parameter value */

#define X400_E_INT_ERROR      4
/*!< Internal Error */

#define X400_E_CONFIG_ERROR   5
/*!< Internal Config Error */

#define X400_E_NYI            6
/*!< Function not yet implemented */

#define X400_E_NO_CHANNEL      7
/*!< no X400 channel in MTA */

#define X400_E_INV_MSG         8
/*!< Message received from MTA invalid */

#define X400_E_NOCONNECT      10
/*!< Cannot connect to server */

#define X400_E_BADCREDENTIALS 11
/*!< Invalid credentials for connection */

#define X400_E_CONNECT_REJ    12
/*!< Connect to server rejected */

#define X400_E_QMGR_CONGESTED 13
/*!< Connect to server rejected */

#define X400_E_BAD_QMGR_RESP  14
/*!< Connect to server rejected */

#define X400_E_MISSING_ATTR   20
/*!< Missing attribute in message */

#define X400_E_CONFLICT_ATTR  21
/*!< Conflicting attributes in message */

#define X400_E_INVALID_ATTR   22
/*!< Unknown attribute type, or attribute not appropriate for message type */

#define X400_E_INVALID_VALUE  23
/*!< Value is invalid for the attribute type */

#define X400_E_NO_VALUE       24
/*!< Value for parameter is not available */

#define X400_E_NO_MESSAGE     30
/*!< No message available */

#define X400_E_TIMED_OUT      31
/*!< Wait timed out */

#define X400_E_NO_RECIP       32
/*!< No recipient */

#define X400_E_NOSPACE        33
/*!< Insufficient space in output buffer */

#define X400_E_ADDRESS_ERROR          64
/*!< Submission error - address failed */

#define X400_E_BUFFER_FILE_ERROR      65
/*!< Submission error - buffer file failed */

#define X400_E_BUFFER_FILENAME_ERROR  66
/*!< Submission error - filename invalid */

#define X400_E_BUFFER_TYPE_ERROR      67
/*!< Submission error - invalid buffer type */

#define X400_E_CONTENT_ERROR          68
/*!< Submission error - invalid content */

#define X400_E_CONTENT_DATA_ERROR     69
/*!< Submission error - failed to initialize content data */

#define X400_E_CONTENT_INIT_ERROR     70
/*!< Submission error - failed to initialize content */

#define X400_E_CONTENT_BODY_ERROR     71
/*!< Submission error - failed to initialize content body */

#define X400_E_INIT_ERROR             72
/*!< Submission error - initialization failed */

#define X400_E_MESSAGE_ERROR          73
/*!< Submission error - conclude message failed */

#define X400_E_NO_CONTENT             74
/*!< Submission error - null content */

#define X400_E_ORIGINATOR_ERROR       75
/*!< Submission error - originator's address failed */

#define X400_E_PARMS_ERROR            76
/*!< Submission error - invalid initialization parameter */

#define X400_E_RECIPIENT_ERROR        77
/*!< Submission error - recipient's address failed */

#define X400_E_TRANSFERABLE_ERROR     78
/*!< Submission error - bad transferable */

#define X400_E_REPORT_ERROR           79
/*!< Submission error - failed transferring report */

#define X400_E_COMPLEX_BODY           80
/*!< Body part needs Bodypart object */

#define X400_E_MESSAGE_BODY           81
/*!< Body part needs Message object */

#define X400_E_UNSUPPORTED_BODY       82
/*!< Body part is not supported by this API */

#define X400_E_X509_ENV		      	83
/*!< Invalid Security environment */

#define X400_E_X509_INTERNAL_ERROR	84
/*!< Internal error in security sub system */

#define X400_E_X509_INIT              	85
/*!< Cannot initialise the security env */

#define X400_E_X509_VERIFY_FAIL_NO_CERT		87
/*!< Cannot verify sig - no originator certificate */

#define X400_E_X509_VERIFY_FAIL_NO_PUBKEY       88
/*!< Cannot verify sig - no public key in originator certificate */

#define X400_E_X509_VERIFY_FAIL_INCOMPAT_ALG	89
/*!< Cannot verify sig - algorithm in cert is incompatible */

#define X400_E_X509_VERIFY_FAIL_UNSUPPORTED_ALG	90
/*!< Cannot verify sig - algorithm in cert is incompatible */

#define X400_E_X509_VERIFY_FAIL			91
/*!< Signature Verification failed */

#define X400_E_X509_CERT_INVALID		92
/*!< Verification failed - but message has been returned */

#define X400_E_X509_ITEM_INVALID		93
/*!< Invalid security-related extension value */

#define X400_E_SIGN_NO_IDENTITY       		94
/*!< Cannot find the Identity to sign the message */

#define X400_E_SIGN                   		95
/*!< Cannot sign the message */

#define X400_E_NONDELIVERED           		96
/*!< Message non-delivered due to unhandled critical extensions */

#define X400_E_NO_MORE_RESULTS        99
/*!< No more results available */

#define X400_E_WAIT_WRITE             100
/*!< Application should wait for association to become writeable */

#define X400_E_WAIT_READ              101
/*!< Application should wait for association to become readable */

#define X400_E_WAIT_READ_WRITE        102
/*!< Application should wait for association to become readable and writeable */

#define X400_E_CONNECTION_LOST        110
/*!< Connection lost to Queue Manager */

#define X400_E_SHUTDOWN               111
/*!< Queue Manager shutting down */

#define X400_E_NO_MATCH               112
/*!< No match between elements */

/* \} */

/*! \defgroup reciptype Recipient Type Indicators
  These \#defines are used to indicate where a recipient object is to be
  placed or found. It is possible to combine the envelope item
  with a recipient type so that only one recipient object is required.

  Note: the message or probe envelope originator is set as part of the
  message object attributes, rather than as recipient objects.
 \{
*/

#define X400_RECIP_PRIMARY  0x10001
/*!< Primary recipient in header */

#define X400_RECIP_CC       0x10002
/*!< Carbon-copy recipient in header */

#define X400_RECIP_BCC      0x10003
/*!< Blind carbon-copy recipient in header */

#define X400_ORIGINATOR     0x10004
/*!< Originator in header, also IPN Originator in IPN */

#define X400_AUTH_ORIG      0x10005
/*!< Authorizing user in header */

#define X400_RECIP_REPLY    0x10006
/*!< Reply recpient in header */

#define X400_RECIP_REPORT   0x10008
/*!< Reported recipient in report content */

#define X400_RECIP_INTENDED 0x10009
/*!< IPM Intended Recipient in IPN */

#define X400_REPORTING_DL_NAME 0x1000A
/*!< X.400 Report Delivery Reporting DL name*/

#define X400_DL_EXEMPTED_RECIP 0x1000B
/*!< X.400 DL Exempted recipients
 * Note: While you can specify DL Exempted Recipients within the
 * X.400 API, and create a message with a suitable DL exempted recipients
 * extension. Isode's MTA will ignore any exempted recipients, when expanding
 * a distribution list. Isode's MTA will be updated at a future point to
 * correct this behaviour
 * */

#define X400_EXEMPTED_ADDRESS 0x10010
/*!< P772 exempted address list */

#define X400_RECIP_ENVELOPE 0x20000
/*!< Envelope recipient */

#define X400_RECIP_STANDARD (X400_RECIP_PRIMARY+X400_RECIP_ENVELOPE)
/*!< Recipient for envelope and as primary recipient in header */

/* \} */

/*! \defgroup address O/R Addressing attributes
 \{*/

#define X400_S_OR_ADDRESS			1
/*!< Full X.400 address in string form, using RFC 2156 conventions */

#define X400_S_COUNTRY_NAME			2
/*!< X.400 address CountryName attribute */

#define X400_S_ADMD_NAME			3
/*!< X.400 address AdministrativeManagementDomainName attribute */

#define X400_S_PRMD_NAME			4
/*!< X.400 address PrivateManagementDomainName attribute */

#define X400_S_ORGANIZATION_NAME		5
/*!< X.400 address OrganizationName attribute */

#define X400_S_ORGANIZATIONAL_UNIT_NAME_1	6	
/*!< X.400 address OrganizationalUnitName[1] attribute */

#define X400_S_ORGANIZATIONAL_UNIT_NAME_2	7	
/*!< X.400 address OrganizationalUnitName[2] attribute */

#define X400_S_ORGANIZATIONAL_UNIT_NAME_3	8	
/*!< X.400 address OrganizationalUnitName[3] attribute */

#define X400_S_ORGANIZATIONAL_UNIT_NAME_4	9	
/*!< X.400 address OrganizationalUnitName[4] attribute */

#define X400_S_SURNAME			       10	
/*!< X.400 address PersonalName.Surname attribute */

#define X400_S_GIVEN_NAME		       11
/*!< X.400 address PersonalName.GivenName attribute
     Note: a surname must be used with this attribute
*/

#define X400_S_INITIALS			       12		
/*!< X.400 address PersonalName.Initials attribute
     Note: a surname must be used with this attribute
*/

#define X400_S_GENERATION_QUALIFIER	       13
/*!< X.400 address PersonalName.GenerationQualifier attribute
     Note: a surname must be used with this attribute
*/

#define X400_S_COMMON_NAME		       14
/*!< X.400 address CommonName attribute */

#define X400_S_DIRECTORY_NAME		       20
/*!< Associated X.500 Directory Name. In RFC 2253 string form (LDAP) */

/*\}*/

/*! \defgroup envelope X.400 Envelope attributes
  \{ */

#define X400_S_MESSAGE_IDENTIFIER	           100
/*!< X.400 Message Identifier, in RFC 2156 String form, e.g "[<ORAddress>;<localpart>]". The local identifier part is limited to 32 chars*/

#define X400_N_CONTENT_TYPE			   101
/*!< X.400 Numerical Content Type */

#define X400_N_CONTENT_LENGTH			   102
/*!< X.400 Content Length (Probes only) */

#define X400_S_CONTENT_IDENTIFIER		   103
/*!< X.400 Content Identifier */

#define X400_S_ORIGINAL_ENCODED_INFORMATION_TYPES  104
/*!< X.400 Original Encoded Information Types
     Encoded as a space-separated list of standard names or numeric OID values
 */

#define X400_N_PRIORITY				   105	
/*!< X.400 Priority: 0 - normal, 1 - non-urgent, 2 - urgent */

#define X400_N_DISCLOSURE			   106
/*!< X.400 Disclosure of recipients: 0 - no, 1 - yes */

#define X400_N_IMPLICIT_CONVERSION_PROHIBITED	   107	
/*!< X.400 Implicit conversion prohibited: 0 - no, 1 - yes */

#define X400_N_ALTERNATE_RECIPIENT_ALLOWED	   108	
/*!< X.400 Alternate recipient allowed: 0 - no, 1 - yes */

#define X400_N_CONTENT_RETURN_REQUEST		   109
/*!< X.400 Content return request: 0 - no, 1 - yes */

#define X400_S_MESSAGE_SUBMISSION_TIME		   110
/*!< X.400 Message Submission Time: UTCTime format YYMMDDHHMMSS<zone> */

#define X400_S_MESSAGE_DELIVERY_TIME		   111
/*!< X.400 Message Delivery Time: UTCTime format YYMMDDHHMMSS<zone>
   Note: also used for Reports
 */

#define X400_S_EXTERNAL_CONTENT_TYPE		   112
/*!< X.400 External Content Type - specify as object 
     identifier string in "1.2.3.4" format.
     Note that if both External and Built-in (numeric)
     content type attributes are present in a message
     being sent, the External attribute will 'win'.
*/

#define X400_N_RECIPIENT_REASSIGNMENT_PROHIBITED   120		
/*!< X.400 Recipient reassignment prohibited: 0 - no, 1 - yes */

#define X400_N_DL_EXPANSION_PROHIBITED		   121
/*!< X.400 Distribution List expansion prohibited: 0 - no, 1 - yes */

#define X400_N_CONVERSION_WITH_LOSS_PROHIBITED	   122	
/*!< X.400 Conversion with loss prohibited: 0 - no, 1 - yes */

#define X400_S_LATEST_DELIVERY_TIME		   123
/*!< X.400 Latest Delivery Time: UTCTime format YYMMDDHHMMSS<zone> */

#define X400_S_ORIGINATOR_RETURN_ADDRESS	   124	
/*!< X.400 Originator Return Address (X.400 String format) */

#define X400_S_OBJECTTYPE			   125
/*!< type of this object ("message", "report", "probe") */

#define X400_N_MMTS_PRIORITY_QUALIFIER	   	   126
/*!< Military Messaging Priority Qualifier: 0 - low, 1 - high */

#define X400_S_DEFERRED_DELIVERY_TIME		   127
/*!< X.400 Deferred Delivery Time: UTCTime format YYMMDDHHMMSS<zone> */

#define X400_S_DLEXP_TIME                          128
/*!< X.400 Distribution List Expansion Time: UTCTime format YYMMDDHHMMSS<zone>*/
#define X400_S_GLOBAL_DOMAIN_ID 		   129
/*!< X.400 PRMD,ADMD,GB used for tracing */

#define X400_S_DSI_ARRIVAL_TIME 		   130
/*!< X.400  Domain Supplied Information Arrival Time */

#define X400_N_DSI_ROUTING_ACTION 		   131
/*!< X.400  Domain Supplied Information Routing Action */

#define X400_S_DSI_ATTEMPTED_DOMAIN 		   132
/*!< X.400  Domain Supplied Information Attempted Domain */

#define X400_S_DSI_AA_DEF_TIME 		   	   133
/*!< X.400  Domain Supplied Information Deferred Delivery Time */

#define X400_S_DSI_AA_CEIT 		   	   134
/*!< X.400  Domain Supplied Information Converted Encoded Information Types  */

#define X400_N_DSI_AA_REDIRECTED 		   135
/*!< X.400  Domain Supplied Information Additional Action Redirection */

#define X400_N_DSI_AA_DLOPERATION 		   136
/*!< X.400  Domain Supplied Information Additional Action dl operation*/

#define X400_S_CONTENT_CORRELATOR 		   137
/*!< Depreciated */

#define X400_N_REDIRECTION_REASON 		   138
/*!< X.400 Redirection Reason */

#define X400_S_REDIRECTION_TIME 		   139
/*!< X.400 Redirection Time */

#define X400_N_CONTENT_RETURNED		           140
/*!< Indicates presence of returned content in a -ve DR: 0 - no, 1 - yes */

#define X400_S_CONTENT_CORRELATOR_IA5_STRING     X400_S_CONTENT_CORRELATOR
/*!< X.400  Content Correlator encoded as IA5 String */

#define X400_S_CONTENT_CORRELATOR_OCTET_STRING    141
/*!< X.400  Content Correlator encoded as Octet String */

/*\}*/

/*! \defgroup security Message Security Attributes
  These \#defines are used to provide the security environment used
  to sign messages and verify signatures.
  (NB Message Tokens used to provide signatures on a per recipient basis are 
  in a different section).
  All these attributes apart from X400_B_SEC_GEN_MOAC can be specified 
  in the Message object using X400msMsgAddStrParam() or X400msMsgAddIntParam(). 
  They can also be specified in the default object using X400SetStrDefault()
  or X400SetIntDefault. X400_B_SEC_GEN_MOAC can only be set using 
  X400msMsgAddIntParam().

  Values in the Message object override those set in the Default object.

  The values are all ignored when the message is constructed unless 
  X400_B_SEC_GEN_MOAC is set in the Message object.

  If X400_B_SEC_GEN_MOAC is set in the Message object, then a valid 
  security environment must have been set up (see the X.509 Setup Guide).
  The other attributes must be passed in to point to this security
  environment.

  Currently only Digital Identities in a directory called "x509" can be 
  used. The parent directory is passed into X400_S_SEC_IDENTITY.

  \{ */

#define X400_S_SEC_IDENTITY			   180
/*!< Directory in which to search for Identities: Looks in x509 Sub Directory */

#define X400_B_SEC_GEN_MOAC			   181
/*!< Generate MOAC 0: no (default), 1: yes */

#define X400_S_SEC_IDENTITY_PASSPHRASE		   182
/*!< Passphrase to open Identity */

#define X400_S_SEC_IDENTITY_DN		   	   183
/*!< DN in Certificate */

#define X400_S_SEC_ENV		   		   186
/*!< For internal use only */

#define X400_S_MOAC 				   187
/*!< X.400 Message Origin Authentication Check */

#define X400_N_MOAC_STATUS                         188
/*!< Status of MOAC in message */

#define X400_S_CERT_INFO	   		   189
/*!< For internal use only */


/*\}*/


/*! \defgroup recip Recipient Attributes
  \{ */

#define X400_N_ORIGINAL_RECIPIENT_NUMBER	        200
/*!< X.400 Original Recipient Number */

#define X400_N_RESPONSIBILITY			        201
/*!< X.400 Responsibility: 0 - not responsible, 1 - responsible */

#define X400_N_MTA_REPORT_REQUEST		        202
/*!< X.400 Originating MTA report request:
     non-delivery-report 1
     report              2
     audited-report      3
     <br>This value should be set only by Gateway applications (like an MTCU)</br>
 */

#define X400_N_REPORT_REQUEST			        203
/*!< X.400 Originator report request:
     no report           0
     non-delivery-report 1
     report              2
    <br>This value should be set by user agent applications and Gateway applications<br/>
*/

#define X400_S_ORIGINATOR_REQUESTED_ALTERNATE_RECIPIENT 204
/*!< X.400 Originator requested alternate recipient:
 X.400 string form, address only, no Directory Name */

#define X400_S_ORIGINATOR_REQUESTED_ALTERNATE_RECIPIENT_DN 205
/*!< X.400 Originator requested alternate recipient,
  Directory Name in String encoding */

#define X400_S_FREE_FORM_NAME		                221
/*!< IPM Free Form Name */

#define X400_S_TELEPHONE_NUMBER		                222
/*!< IPM Telephone Number */

#define X400_N_NOTIFICATION_REQUEST	                223
/*!< IPM Notification request: logical or of:
  1 - receipt notification request
  2 - non-receipt notification request
  4 -  return request
*/

#define X400_N_REPLY_REQUESTED		                224
/*!< IPM Reply Request: boolean */
 
#define X400_N_PRECEDENCE       		        225
/*!< IPM Precedence X400(1999). Integer value */

#define X400_S_ORIGINAL_RECIPIENT_ADDRESS               226
/*!< The address in the first Redirection History element
     Also used in reported recipient information
*/

#define X400_S_CONVERTED_ENCODED_INFORMATION_TYPES 227
/*!< The CEIT used in the last trace info element of a per recipient report */

/*\}*/

/*! \defgroup content IPM Content Attributes
 *\{ */

#define X400_S_IPM_IDENTIFIER			   300
/*!< IPM Identifier - The string form is related to the RFC 2156
    form, with an addition allowing for a Directory name, namely
    \<user-relative-identifier\> '*' \<std-or-address\> [ '\<' \<DN\> '\>' ]

    e.g.,

    (No user component)
    1234.5678*

    (Only address in user, the space after '*' should not be used)
    1234.5678* /cn=Test/admd=thisadmd/c=gb/

    (Address and DN in user, the space after '*' should not be used)
     1234.5678* /cn=Test/admd=thisadmd/c=gb/\<cn=test, c=gb\>

    (Only DN in user)
     1234.5678*\<cn=test, c=gb\>

 */

#define X400_S_SUBJECT			           301
/*!< IPM Subject */

#define X400_S_REPLIED_TO_IDENTIFIER		   302
/*!< IPM Replied To Identifier - String form as for IPM_IDENTIFIER
 */

#define X400_S_OBSOLETED_IPMS			   303
/*!< IPM Obsoleted IPMS - Space separated list of string IPM identifiers
 */

#define X400_S_RELATED_IPMS			   304
/*!< IPM Related IPMS - Space separated list of string IPM identifiers */

#define X400_S_EXPIRY_TIME			   305
/*!< IPM Expiry time - UTCTime format */

#define X400_S_REPLY_TIME			   306
/*!< IPM Expiry time - UTCTime format */

#define X400_N_IMPORTANCE			   307
/*!< IPM Importance: 0 - low, 1 - normal, 2 - high */

#define X400_N_SENSITIVITY			   308
/*!< IPM Sensitivity: 1 - personal, 2 - private, 3 - company-confidential */

#define X400_N_AUTOFORWARDED			   309
/*!< IPM Autoforwarded: boolean */

#define X400_S_AUTHORIZATION_TIME		   320
/*!< IPM Authorization Time: GeneralizedTime format. X400(1999) */

#define X400_S_ORIGINATORS_REFERENCE		   321
/*!< IPM Originator's reference. X400(1999). A UTF-8 string */

#define X400_S_PRECEDENCE_POLICY_ID		   322
/*!< IPM Precedence Policy Identifier. X400(1999)
     An object identifier in numeric form.
 */

#define X400_S_CONTENT_STRING		           350
/*!< The content of the message to be sent, as an octet
     string containing the encoding of the content.
 */

#define X400_S_CONTENT_FILENAME		           351
/*!< The full file path of a file which contains the
     encoding of the content of the message to be sent.
     Note that if both X400_S_CONTENT_STRING and
     X400_S_CONTENT_FILENAME are specified in the
     same message, the value of X400_S_CONTENT_STRING
     will be used, and X400_S_CONTENT_FILENAME will
     be ignored.
 */

#define X400_N_NUM_ATTACHMENTS			   400
/*!< IPM number of attachments apart from main body part */

#define X400_T_IA5TEXT			           401
/*!< ia5-text body part. Only the ia5 repertoire is supported */

#define X400_T_ISO8859_1			   402
/*!< general-text body part containing ISO-8859-1 text.
  On creation, the appropriate character sets are included in the parameters,
  and the data has the appropriate escape sequences added to the start.
  On reception, if the escape sequences are at the start of the data,
  they are removed.
*/

#define X400_T_ISO8859_2			   403
/*!< general-text body part containing ISO-8859-2 text
  On creation, the appropriate character sets are included in the parameters,
  and the data has the appropriate escape sequences added to the start.
  On reception, if the escape sequences are at the start of the data,
  they are removed.
 */

#define X400_T_BINARY			           404
/*!< bilaterally-defined body part */

#define X400_T_MESSAGE                             405
/*!< Forwarded message body part, or returned IPM in IPN.
  Can only be used as a Body part object.
  There is an underlying message object, and therefore
*/

#define X400_T_FTBP                                406
/*!< File transfer body part
  Can only be used as a Body part object.
  Only the unstructured binary file type is supported.
  Only a restricted range of parameters are supported.
 */

#define X400_T_GENERAL_TEXT                        407
/*!< general-text body part containing arbitrary character sets
    Can only be used as a Body part object.
    Must set the character set codes in the parameters, and the correct
    escape sequences to switch in the character sets in the data.
 */

#define X400_S_EXT_AUTH_INFO 408
/*!< X.400 P772 Extended Authorisation Info */

#define X400_N_EXT_CODRESS 409
/*!< X.400 P772 Extension Codress */

#define X400_N_EXT_MSG_TYPE 410
/*!< X.400 P772 Message type from MessageType extension */

#define X400_N_EXT_PRIM_PREC 411
/*!< X.400 P772 Primary Precedence */

#define X400_N_EXT_COPY_PREC 412
/*!< X.400 P772 Copy Precedence */

#define X400_PRINTABLE_STRING_SEQ 413
/*!< X400 P772 Printable String Sequence object */

#define X400_S_HANDLING_INSTRUCTIONS 414
/*!< X.400 P772 Used to request a printable string sequence
 * object representing the handling instructions */

#define X400_S_MESSAGE_INSTRUCTIONS 415
/*!< X.400 P772 Used to request a printable string sequence object
 * prepresenting the handling instructions
 */

#define X400_S_ORIG_REF 416
/*!< X.400 P772 Used to manipulate Originator Reference */

#define X400_S_ORIG_PLAD 417
/*!< X.400 P772 Used to manipulate Originator PLAD */

#define X400_S_DIST_CODES_SIC 418
/*!< X.400 P772 Used to manipulate distribution code . */

#define X400_DIST_CODES_EXT 419
/*!< X.400 P772 Used to manipulate distribution code extension fields */

#define X400_S_DIST_CODES_EXT_OID 420
/*!< X.400 P772 Used to manipulate distribution code extension OID */

#define X400_S_DIST_CODES_EXT_VALUE 421
/*!< X.400 P772 Used to manipulate distribution code extension values */

#define X400_OTHER_RECIP_INDICATOR 422
/*!< X.400 P772 Used to manipulate  Other Recipient Indicator object */

#define X400_S_PILOT_FWD_INFO 423
/*!< X.400 P772 Pilot Forwarding info binary blob */

#define X400_S_ACP127_MSG_ID 424
/*!< X.400 P772 ACP 127 Message Indentifier */

#define X400_ACP127_RESPONSE 425
/*!< X.400 P772 ACP 127 Notification response*/

#define X400_N_ACP127_NOTI_TYPE 426
/*!< X.400 P772 ACP 127 Notification Request Type */

#define X400_S_INFO_SEC_LABEL 427
/*!< X.400 P772 Information Security Label */

#define X400_N_ADATP3_PARM 428
/*!< X.400 P772 ADatP3 Parameter */

#define X400_N_ADATP3_CHOICE 429
/*!< X.400 P772 ADatP3 Choice */

#define X400_S_ADATP3_DATA 430
/*!< X.400 P772 ADatP3 Data value */

#define X400_T_ADATP3 431
/*!< X.400 P772 ADatP3 body part type*/

#define X400_T_CORRECTIONS 432
/*!< X.400 P772 Corrections body part type */

#define X400_N_CORREC_PARM 433
/*!< X.400 P772 Corrections body part param */

#define X400_S_CORREC_DATA 434
/*!< X.400 P772 Corrections body part data */

#define X400_T_ACP127DATA 435
/*!< X.400 P772 ACP127DATA body part */

#define X400_N_ACP127DATA_PARM 436
/*!< X.400 P772 ACP127DATA parm */

#define X400_S_ACP127_DATA 437
/*!< X.400 P772 ACP127DATA data */ 

#define X400_T_MM 438
/*!< X.400 P772 MM Forwarded bodypart */

#define X400_T_FWDENC 439
/*!< X.400 P772 Forwarded Encrypted */

#define X400_S_ENCRYPTED_DATA 440
/*!< X.400 Encrypted Data */

#define X400_S_FWD_CONTENT_STRING 441
/*!< The forwarded content body part to be sent, as an octet
     string containing the encoding of the content.
     Defined in X.420
 */

#define X400_T_FWD_CONTENT 442
/*!< X.400 Forwarded content bodypart specified within X.420 */

#define X400_S_ORIG_OR_EXAP_TIME 443
/*!< X.400 Originator OR Address and Expansion history time*/

#define X400_ORIG_OR_EXAP 444
/*!< X.400 Originator OR Address and Expansion history */

#define X400_S_ORIG_CERT 445
/*!< X.400 Originator certificate */

#define X400_N_PROOF_OF_DEL_REQ 446
/*!< X.400 Proof of Delivery Request 0 = no, 1 = yes */

#define X400_S_EXT_MSG_IDENTIFIER 447
/*!< X.400 P772 Message identifier from MessageType extension */


/*\}*/

/*! \defgroup ipn IPN Attributes
 *\{*/

#define X400_N_IS_IPN			           500
/*!< True if message is an IPN rather than IPM */

#define X400_S_SUBJECT_IPM			   501
/*!< IPM Identifier of subject message */

#define X400_S_CONVERSION_EITS                     502
/*!< Space separated list of EIT names */

#define X400_N_NON_RECEIPT_REASON		   510
/*!< Non-receipt reason: 0 - discarded; 1 - auto-forwarded */

#define X400_N_DISCARD_REASON			   511
/*!< Discard reason: 0 - ipm-expired; 1 - ipm-obsoleted;
  2 - user subscription terminated
*/

#define X400_S_AUTOFORWARD_COMMENT		   512
/*!< Autoforward comment */

#define X400_S_RECEIPT_TIME			   520
/*!< Time of receipt: UTCTime format */

#define X400_N_ACK_MODE			           521
/*!< Acknowledgement mode: 0 - normal; 1 -automatic */

#define X400_S_SUPP_RECEIPT_INFO		   522
/*!< Supplementary information associated with IPN */

/*\}*/

/*! \defgroup report X.400 Report Attributes
 *\{*/

#define X400_S_SUBJECT_IDENTIFIER		   600
/*!< X.400 Report subject identifier: RFC 2156 string form */

#define X400_S_SUPPLEMENTARY_INFO		   610
/*!< Per-reported recipient supplementary information */

#define X400_N_TYPE_OF_USER			   611
/*!< Type of MTS user for delivery - see standards for values */

#define X400_S_ARRIVAL_TIME                        612
/*!< Per-reported recipient arrival time */

#define X400_N_NON_DELIVERY_REASON		   613
/*!< Non-delivery Reason - see standards for values */

#define X400_N_NON_DELIVERY_DIAGNOSTIC		   614
/*!< Non-delivery diagnostic  - see standards for values */


/*\}*/

/*! \defgroup defaults Attributes only used in defaults
 *\{*/

#define X400_S_LOG_CONFIGURATION                   700
/*!< Magic value used for setting of logging config. 
     Value should be an XML blob containing the log
     configuration in standard R11+ Isode form. */

#define X400_N_REPORTS                             701
/*!< Magic value used for setting of x400 report control
     This only applies for the gateway interface.
     It controls what Reports are generated on transferring
     a message from the MTA to the user of the API.
     0 - (default) do not generate a report
     1 - generate a positive delivery report
     2 - always generate a non-delivery report
 */

#define X400_S_CONTENT_FILE_PATH                   702
/*!< Configures the path that should be used for output
     of files containing raw content, when 
     X400_S_CONTENT_FILE attribute type is used to request
     raw-content-in-a-file.
*/

#define X400_N_WAIT_INTERVAL                       703
/*!< Configures the interval (in seconds) which the 
     X400msWait function should wait for between polling
     for new messages using the Summarize operation.
     If not set, the interval defaults to 2 seconds.
*/

#define X400_S_LOG_CONFIGURATION_FILE              704
/*!< Magic value used for setting of logging config. 
     Value should be the name of, or full path to,
     a file holding the log configuration in standard 
     R11+ Isode form. 
*/

#define X400_B_RETURN_VERIFICATION_ERRORS                       705
/*!< Magic value used to direct the API to return the
     signature verification errors.
     By default the API will return X400_E_NOERROR if
     verification of a MOAC fails. This ensures that
     old applications are forward compatible with this
     version of the API.
     Setting this value to TRUE causes
     the API to return X400_E_X509_VERIFY_FAILURE rather then
     X400_E_NOERROR when verification fails.
     0 - (default) return X400_E_NOERROR when verification of MOAC fails
     1 - return X400_E_X509_VERIFY_FAILURE when verification of MOAC fails
 */

#define X400_ADATP3_SEP 706
/*!< ADatP3 bodypart types can be a sequence of IA5text strings.
     This value is a NULL terminated string of characters used to
     deliminate the IA5test strings. By default this is "\r\n"
*/

#define X400_S_LIST_ATTR   750
/*!< For internal use only.
 */

/*\}*/

/*! \defgroup bodyparts X.400 Body part attributes
 *\{*/

#define X400_N_BODY_TYPE                           800
/*!< Type of body part. Value is the X400_T value */

#define X400_S_BODY_DATA			   801
/*!< Body part data as octet string. For Teletex body part this is
  the concatenation of the strings.
*/

#define X400_S_GENERAL_TEXT_CHARSETS               802
/*!< String containing the integer registration numbers for the character
     sets within the body part. The decimal values are separated by space
     characters. E.g. "1 6 100"
*/

#define X400_N_FTBP_ENCODING                       803
/*!< Boolean for encoding the Ftbp data. If unset or zero, then
     the octet-aligned encoding is used. If set to a non-zero value
     then the single-asn1 encoding choice is used.

     Only used for output.
*/

#define X400_S_FTBP_APPLICATION_REFERENCE_OID      804
/*!< The Ftbp application reference as an Object Identifier,
     with a value as a dotted decimal value.
*/

#define X400_S_FTBP_APPLICATION_REFERENCE_STR      805
/*!< The Ftbp application reference as a string.
     If both this and the OID reference are set,
     only the OID reference is used.
*/

#define X400_S_FTBP_CONTENT_DESCRIPTION            806
/*!< The user-visible-string in the environment of Ftbp */

#define X400_S_FTBP_FILENAME                       807
/*!< Name of associated file for Ftbp */

#define X400_S_FTBP_CREATION_DATE                  808
/*!< Creation date of file for Ftbp as GeneralizedTime */

#define X400_S_FTBP_MODIFICATION_DATE              809
/*!< Modification date of file for Ftbp as GeneralizedTime */

#define X400_S_FTBP_READ_DATE                      810
/*!< Read date of file for Ftbp as GeneralizedTime */

#define X400_N_FTBP_OBJECT_SIZE                    811
/*!< Recorded file size of Ftbp.
    This may be different from the transfered size
*/

#define X400_S_BODY_DATA_RAW			   812
/*!< Raw octets from/for Data component of GeneralTextBodypart,
     including character set escape sequences etc. 
*/

/*\}*/

/*! \defgroup msattributes P7 Message Store attributes
 *\{*/
#define X400_N_MS_ENTRY_STATUS                      900
/*!< Message Store entry status */

#define X400_N_MS_SEQUENCE_NUMBER                   901
/*!< Message Store sequence number */

#define X400_N_DELETE_AFTER_AUTO_FORWARDING         902
/*!< Control for RegisterMS operation */

#define X400_S_AUTO_FORWARDING_COMMENT              903
/*!< Text to be included in an IPN generated because of
     an autoforwarding auto-action */

#define X400_S_COVER_NOTE                           904
/*!< Text to be included in an autoforwarding IPM */

#define X400_S_THIS_IPM_PREFIX                      905
/*!< IPM prefix to be used in an autoforwarding IPM */

#define X400_N_DELAYED_AUTOFORWARDING_INTERVAL      906
/*!< Controls "delayed-autoforwarding" autoaction - 
  interval is in seconds */

#define X400_N_FIRST_SEQUENCE_NUMBER      907
/*!< Use in X400msSetIntDefault call to configure the API's
  view of what the lowest sequence number in the P7 Store is.
  The default value is "0" (in line with X.413), but you may
  need to configure the API to regard "1" as the lowest valid
  sequence number for interworking with some third-party
  Message Stores - in particular the Thales Store seems to
  have a magic entry with sequence number "0" which shows up
  in List and Summarize operations, but does not represent 
  a real message, and consequently cannot provide Content,
  Envelope etc to Fetch operations and cannot be
  deleted. Configuring the API so that it regards "1"
  as the lowest sequence number will allow the API to
  ignore this magic entry. */

/*\}*/

#define X400_N_STRICT_P7_1988      908
/*!< Use in X400msSetIntDefault call to configure the API to
  stick to the X.400 1988 attribute set when fetching message.
  This may be needed for inworking with some non-Isode
  Message Stores. */

/*! \defgroup msgstatus P7 Message Store Entry Status
  \{ */
#define X400_MS_ENTRY_STATUS_NEW                    1
/*!< Message is new */

#define X400_MS_ENTRY_STATUS_LISTED                 2
/*!< Message has been listed */

#define X400_MS_ENTRY_STATUS_FETCHED                3
/*!< Message has been fetched */

/*\}*/

/*! \defgroup autoactiontype Autoaction Types
  \{ */
#define X400_AUTO_ALERT                             1
/*!< The auto-alert autoaction */

#define X400_AUTO_FORWARDING                        2
/*!< The auto-forwarding autoaction */

/*\}*/

/*! \defgroup envelopeobjects X.400 Envelope Objects
 \{*/

#define X400_DL_EXPANSION_HISTORY 		1500
/*!< Distribution List Expansion History object */

#define X400_S_SECURITY_LABEL 			1501
/*!< X.400 Security label object */

#define X400_TRACE_INFO 			1502
/*!< X.400 Trace info object */

#define X400_REDIRECTION_HISTORY 		1503
/*!< X.400 Redirection History object */

#define X400_SUBJECT_TRACE_INFO 		1504
/*!< X.400 Subject Intermediate Trace Information used in report content */

#define X400_INTERNAL_TRACE_INFO 		1505
/*!< X.400 Internal trace info object*/

#define X400_EXT_ADDRESS_LIST_IND 		1506
/*!< X.400 P772 Address List Indicator */

#define X400_REDIRECTION_HISTORY_ENV 		1507
/*!< X.400 Redirection history object associated with the message envelope instead of being per recipient. IE 8.3.1.2.1.5 of X.411 */

/*\}*/


/*! \defgroup P772_extension P772 extension ORDescriptor
 These \#defines are used to manipulate ORDescriptors (X.420 7.1.3) within
 P772 extensions. Specifically exempted addresses,
 and address list indicators.
 \{ */

#define X400_S_IOB_OR_ADDRESS 1600
/*!<X.400 ORDescriptor OR address */

#define X400_S_IOB_DN_ADDRESS 1601
/*!<X.400 ORDescriptor DN value */

#define X400_S_IOB_FREE_FORM_NAME 1602
/*!<X.400 ORDescriptor free form name */

#define X400_S_IOB_TEL 1603
/*!<X.400 ORDescriptor telephone number */



/*\}*/

/*! \defgroup security Recipient Security Attributes
  These \#defines are used to provide the security environment used
  to sign messages and verify signatures using Message Tokens to provide signatures on a per recipient basis.
  All these attributes apart from X400_B_SEC_GEN_MESSAGE_TOKEN can be specified
    - in the Message object using X400msMsgAddStrParam() or X400msMsgAddIntParam(). 
    - in the Recipient object using X400msRecipAddStrParam() or X400msRecipAddIntParam(). 
  They can also be specified in the default object using X400SetStrDefault()
  or X400SetIntDefault. X400_B_SEC_GEN_MESSAGE_TOKEN can only be set by 
  X400msRecipAddIntParam().

  Values in the Message object override those set in the Default object.
  Values in the Recipient object override those set in the Message object.

  The values are all ignored when the message is constructed unless 
  the X400_B_SEC_GEN_MESSAGE_TOKEN is set in the Recipient object.

  If X400_B_SEC_GEN_MESSAGE_TOKEN is set in the Recipient object, then a 
  valid security environment must have been set up.
  (see the X.509 Setup Guide).
  The other attributes must be passed in to point to this security
  environment.

  Currently only Digital Identities in a directory called "x509" can be 
  used. This parent directory is passed in X400_S_SEC_IDENTITY.


 \{ */

/* values which control signing behaviour of API */
#define X400_B_SEC_GEN_MESSAGE_TOKEN		   1700
/*!< Generate Message Token 0: no (default), 1: yes */

#define X400_B_SEC_ADD_CERT_PATH		   1701
/*!< Include Certificate Path when generating Message Token 0: no (default), 1: yes */

#define X400_B_SEC_CONTENT_INTEGRITY_CHECK	   1702
/*!< Add Content Integrity Extension and use in Message Token: no (default), 1: yes */


/* Message Token values which can be retrieved from the recip */
#define X400_N_MSGTOK_STATUS		   	   1710
/*!< Status of recipient Message Token - See below for possible values */

#define X400_S_MSGTOK_DER		   	   1716
/*!< Token in recipient when verifying a Message Token signature */

#define X400_N_MSGTOK_SEQ_NUM		   	   1717
/*!< Sequence number in Message Token */

#define X400_S_MSGTOK_RECIP		   	   1718
/*!< Recipient in Message Token */

#define X400_S_MSGTOK_SEC_LAB		   	   1719
/*!< Security Label in Message Token */

#define X400_N_MSGTOK_PODR_STATUS	   	   1721
/*!< Information about PODR in Token and Envelope */

#define X400_N_MSGTOK_SEC_LAB_STATUS  	   	   1722
/*!< Information about Security Label in Token and Envelope */

#define X400_N_MSGTOK_RECIP_STATUS   	   	   1723
/*!< Information about Recipient in Token and Envelope */

#define X400_N_MSGTOK_CIC_STATUS		   1724
/*!< Status of CIC in Message Token */

#define X400_S_MSGTOK_CIC			   1726
/*!< Content Integrity Check DER from Message Token */

/*\}*/

/*! \defgroup Recipient Content Integrity Check information.
    These allow information about the CIC in the Recipient extensions
    to be returned. The signing certificate values also apply to the
    CIC if present in the Message Token.
\{ */

#define X400_N_RECIP_CIC_STATUS			1750
/*!< CIC status */

#define X400_S_RECIP_CIC			1752
/*!< Content Integrity Check BER */

/*\}*/

/*! \defgroup MessageToken Argument status bits 
 Used in  X400_N_PODR_STATUS, X400_N_SEC_LAB_STATUS, 
 X400_N_RECIP_STATUS which can be in either or both of envelope and Token.
  \{ */

#define X400_TOKEN_ARG_IN_TOK			0x0001
/*!< Bit in STATUS integer set if argument is present in Token */

#define X400_TOKEN_ARG_IN_ENV			0x0002
/*!< Bit in STATUS integer set if argument is present in envelope */

#define X400_TOKEN_TOK_AND_ENV_ARGS_MATCH	0x0004
/*!< Bit in STATUS integer set if arguments in Token and envelope match */

/*\}*/

/*! \defgroup Certificate type selectors
  \{ */

#define X400_N_CERT_MOAC	   	   1
/*!< Certificate related to message-origination-authentication-check
     in a message
*/

#define X400_N_CERT_MSGTOK		   2
/*!< Certificate related to message-token (in recipient) */

#define X400_N_CERT_RECIP_CIC		   3
/*!< Certificate related to content-integrity-check (in recipient extension) */

#define X400_N_CERT_MSGTOK_CIC		   4
/*!< Certificate related to content-integrity-check in message-token
     in a recipient
 */

/*\}*/

/*! \defgroup Certificate object attributes
  \{ */

#define X400_S_CERT_BER		   	   1800
/*!< Certificate raw data */

#define X400_S_CERT_SUBJECT_DN		   1801
/*!< DN of subject from certificate */

#define X400_S_CERT_ISSUER_DN		   1802
/*!< DN of issuer of certificate */

#define X400_S_CERT_SERIAL_NUM		   1803
/*!< Serial number of certificate */

#define X400_N_CERT_ORADDRESS_STATUS       1804
/*!< Status of match between ORaddress subject alt. name and originator
     If X400_E_NOERROR, then present and a match.
     If X400_E_NO_VALUE, then no ORaddress in certificate
     If X400_E_NO_MATCH, then present but does not match
*/

/*\}*/


/*! \defgroup Redirection_reason Redirection Reason
  \{ */

#define X400_RR_RECIP_ASSIGNED_ALT_RECIP 1
/*!< X.400 Recipient assigned alternate recipient */

#define X400_RR_ORIG_REQUESTED_ALT_RECIP 2
/*!< X.400 Origintator requested alternate recipient */

#define X400_RR_RECIP_MD_ASSIGNED_ALT_RECIP 3
/*!< X.400 Recipient MD assigned  alternate recipient */

#define X400_RR_ALIAS 4
/*!< X.400 Alias */

#define X400_RR_IC_SYNONYM 5
/*!< X.400 IC synonym */

#define X400_RR_IC_EXTERNAL_SYNONYM 6
/*!< X.400 IC external synonym */

#define X400_RR_IC_AMBIGUOUS 7 
/*!< X.400 IC ambiguous */

/*\}*/


/*! \defgroup MTA_internal_trace MTA Internal trace information
  These attributes allow the manipulation of internal trace information
  objects. As defined in X.411 12.2.1.1.1.4.
  Note: X400_S_GLOBAL_DOMAIN_ID is used to describe the global domain
  identifier of the internal trace information object.
 \{ */

#define X400_S_MTA_NAME 1
/*!<X.400 MTA Name */

#define X400_S_MTA_SI_TIME 2
/*!<X.400 MTA Supplied Information Arrival Time */

#define X400_N_MTA_SI_ROUTING_ACTION 3
/*!<X.400 MTA Supplied Information Routing Action */

#define X400_S_MTA_SI_ATTEMPTED_MTA 4
/*!<X.400 MTA Supplied Information Attempted MTA */

#define X400_S_MTA_SI_ATTEMPTED_DOMAIN 5
/*!<X.400 MTA Supplied Information Attempted Domain*/

#define X400_S_MTA_SI_DEFERRED_TIME 6
/*!<X.400 MTA Supplied Information Deferered time */

#define X400_S_MTA_SI_CEIT 7
/*!<X.400 MTA Supplied Information CEIT */

#define X400_N_MTA_SI_OTHER_ACTIONS 8
/*!<X.400 MTA Supplied Information Other Actions */

#define X400_N_MTA_SI_ATTEMPTED_ACTION 9
/*!<X.400 MTA Supplied Information Attempted Action */

#define X400_MTA_SI_RA_MTA 1
/*!<X.400 MTA Supplied Information Routing Action MTA */

#define X400_MTA_SI_DOMAIN 2
/*!<X.400 MTA Supplied Information Routing Action Domain */

#define X400_MTA_SI_ROUTING_ACTION_RELAYED 0
/*!<X.400 MTA Supplied Information Routing Action Relayed */

#define X400_MTA_SI_ROUTING_ACTION_REROUTED 1
/*!<X.400 MTA Supplied Information Routing Action Rerouted */

#define X400_MTA_SI_OTHER_ACTION_REDIRECTED 0x00001
/*!<X.400 MTA Supplied Information Other Action Redirected */

#define X400_MTA_SI_OTHER_ACTION_DLOPERATION 0x00002
/*!<X.400 MTA Supplied Information Other Action DL operation */

/*\}*/

/*! \defgroup P772_ALI P772 Address List Indicator Object Attributes
  These attributes combined with the P772 ORDescriptor attributes
  allow the manipulation of Address List Indicator Objects
  ALI are defined within STANAG 4006 A1.11
 \{ */

#define X400_N_ALI_TYPE 0
/*!<X.400 Address List Indicator object type */

#define X400_N_ALI_NOTIFICTAION_REQUEST 1
/*!<X.400 Address List Indicator Notification Request */

#define X400_N_ALI_REPLY_REQUEST 2
/*!<X.400 Address List Indicator Reply Request */

/*\}*/

/*! \defgroup P772_NOTI_TYPE P772 ACP127 Notification Type values
  These are the possible values the ACP127 Notification type can be.
  
 \{ */

#define X400_ACP127_NOTI_TYPE_NEG 0x00001
/*!<X.400 Acp127 Notification Type: Negative Notification */

#define X400_ACP127_NOTI_TYPE_POS 0x00002
/*!<X.400 Acp127 Notification Type: Positive Notification */

#define X400_ACP127_NOTI_TYPE_TRANS 0x00004
/*!<X.400 Acp127 Notification Type: Transfer Notification */

/*\}*/

/*! \defgroup P772_NOTI_RESP P772 ACP127 Notification Reponse 
  These are the attributes for the ACP127 Notification Response object.
 \{ */


#define X400_N_ACP127_NOTI_RESP_TYPE 1
/*!< X400. P772 ACP 127 Notification Response Type */

#define X400_S_ACP127_NOTI_RESP_TIME 2
/*!< X400. P772 ACP 127 Notification Response Time */

#define X400_S_ACP127_NOTI_RESP_ALI 3
/*!< Used internally. */

#define X400_S_ACP127_NOTI_RESP_RECIPIENT 4
/*!< X400. P772 ACP 127 Notification Response Recipient */

#define X400_S_ACP127_NOTI_RESP_SUPP_INFO 5
/*!< X400. P772 ACP 127 Notification Suppliement information */

/*\}*/


#endif /* _X400_ATT_H */

