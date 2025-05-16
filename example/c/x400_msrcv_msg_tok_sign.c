/*  Copyright (c) 2009, Isode Limited, London, England.
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
 * Simple example program for receiving a message from a store.
 * The security environment is set up so that signed messages
 * will have per message MOAC signatures and recipient Message Token 
 * signatures verified.
 * Note that a valid Digital Identity is required in order to 
 * perform verification.
 */

#include <stdio.h>
#include <stdlib.h>
#include <x400_msapi.h>
#include "example.h"
#include <seclabel_api.h> /* For security labels */

#define XML_BUFSIZE 1024

int num_unsigned_rcvd;
int num_unverified_rcvd;
int num_verified_rcvd;

static char *optstr = "u37m:d:p:w:M:D:P:W:e:b:x:E";

static void usage(void);


static int get_msg(
    struct X400msSession *sp
);

static void setup_default_sec_env(
    struct X400msSession *sp, 
    char *id, 
    char *dn, 
    char *pw
);

static void report_msg_token_info(
    struct X400Recipient *rp
);

static int get_sec_label(
    struct X400msMessage *mp
);

static void print_sec_label(
    unsigned char slab_buffer[],
    unsigned int length
);

static int get_cic(
    struct X400Recipient *rp
);

static void show_certificate (
    struct X400Recipient *rp,
    int certtype,
    const char *tag
);

/*! Main function of x400_msrcv_msg_tok_sign.c 
 * \return 0 on success, otherwise the value of status from X400 call
 */
int
main(
    int argc,
    char **argv)
{
    char buffer[BUFSIZ];
    char pa[BUFSIZ];
    char orn[BUFSIZ];
    int status;
    int nummsg;
    struct X400msSession *sp;
    int contype;
    char *def_oraddr;
    char *def_dn;
    char *def_pa;

    if (get_args(argc, argv, optstr)) {
        usage();
        exit(-1);
    }

    printf("Connection type (0 = P7, 1 = P3 submit only, 2 = P3 both directions) [%d]: ", x400_contype);
    contype = ic_fgetc(x400_contype, stdin);
    if (contype != 10)
        ic_fgetc(x400_contype, stdin);

    if ( contype < '0' || '2' < contype )
        contype = x400_contype;
    else
        contype -= '0';

    if (contype == 0) {
        def_oraddr = x400_ms_user_addr;
        def_dn = x400_ms_user_dn;
        def_pa = x400_ms_presentation_address;
    }
    else {
        def_oraddr = x400_mta_user_addr;
        def_dn = x400_mta_user_dn;
        def_pa = x400_mta_presentation_address;
    }

    printf("Your ORAddress [%s] > ", def_oraddr);
    ic_fgets(orn, sizeof orn, stdin);

    if (orn[strlen(orn) - 1] == '\n')
        orn[strlen(orn) - 1] = '\0';

    if (orn[0] == '\0')
        strcpy(orn, def_oraddr);

    /* Prompt for password; note: reflected. */
    printf("Password [%s]: ",
           contype == 0 ? x400_p7_password : x400_p3_password);
    if (ic_fgets(buffer, sizeof buffer, stdin) == NULL)
        exit(1);

    if (buffer[strlen(buffer) - 1] == '\n')
        buffer[strlen(buffer) - 1] = '\0';
    if (buffer[0] == '\0')
        strcpy(buffer, contype == 0 ? x400_p7_password : x400_p3_password);

    printf("Presentation Address [%s] > ", def_pa);
    ic_fgets(pa, sizeof pa, stdin);

    if (pa[strlen(pa) - 1] == '\n')
        pa[strlen(pa) - 1] = '\0';

    if (pa[0] == '\0')
        strcpy(pa, def_pa);

    status = X400msOpen(contype, orn, def_dn, buffer, pa, &nummsg, &sp);
    if (status != X400_E_NOERROR) {
        fprintf(stderr, "Error in Open: %s\n", X400msError(status));
        exit(status);
    }

    /* setup logging from $(ETCDIR)x400api.xml or $(SHAREDIR)x400api.xml */
    X400msSetStrDefault(sp, X400_S_LOG_CONFIGURATION_FILE, "x400api.xml", 0);

    if (nummsg == 0) {
        printf ("no messages - waiting for a message to be delivered.....\n");
    }
    else {
        printf("%d messages waiting\n", nummsg);
    }

    status = get_msg(sp);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "Error in getting msg: %s\n",
		X400msError(status));
	exit(status);
    }
    fprintf(stderr, "got first\n");

    do {
	fprintf(stderr, "================================================\n");
	status = get_msg(sp);
	if (status != X400_E_NOERROR) {
	    fprintf(stderr, "Error in getting msg: %s\n",
		    X400msError(status));
	    exit(status);
	}
    } while (until_no_more_msgs);

    status = X400msClose(sp);
    printf("%d num_unsigned_rcvd\n", num_unsigned_rcvd);
    printf("%d num_unverified_rcvd\n", num_unverified_rcvd);
    printf("%d num_verified_rcvd\n", num_verified_rcvd);
    return(status);
}


static int get_msg(
    struct X400msSession *sp
)
{
    char buffer[BUFSIZ];
    int status;
    int nummsg;
    int type;
    int seqn;
    struct X400msMessage *mp;
    struct X400Recipient *rp;
    int n;
    size_t length;
    int intparam;
    char recipient_str[BUFSIZ];

    printf("Waiting for new messages for 10 seconds\n");
    status = X400msWait(sp, 10, &nummsg);
    if (status != X400_E_NOERROR) {
        fprintf(stderr, "Error from Wait: %s\n", X400msError(status));
	/* tidily close the session */
        status = X400msClose(sp);
        exit(status);
    }

    /* Set up the security environment.
     * The ID is specified as a pathname, in which the subdirectory
     * "x509" is expected to contain one or more PKCS12 files.
     * The appropriate Digital Identity is determined from the
     * DN of the subject, and the passphrase is used to decrypt
     * the private key. 
     * NB even though we're not going to use our cert, (as we're 
     * going to use the cert in the message to check the MOAC
     * we need to do this due to a limitation in the underlying 
     * X.509 layer. This will be changed in future releases. */
    setup_default_sec_env(sp, security_id, identity_dn, passphrase);

    /* Turn off legacy bahaviour in which  MOAC verification failure 
     * returns X400_E_NOERROR.
     * We will now get X400_E_X509_VERIFY_FAILURE from MsgGet
     * if/when the verification fails 
     */
    X400msSetIntDefault(sp, X400_B_RETURN_VERIFICATION_ERRORS, 1); 

    if (x400_default_recipient != NULL) 
    printf("Getting message\n");
    status = X400msMsgGet(sp, 0, &mp, &type, &seqn);
    switch (status) {
    case X400_E_NOERROR:
	fprintf(stderr, "MsgGet successfully got message\n");
	break;
    default :
	fprintf(stderr, "Error from MsgGet: %s\n", X400msError(status));
	/* tidily close the session */
	status = X400msClose(sp);
	exit(status);
	break;
    }

    if (type != X400_MSG_MESSAGE) {
        fprintf(stderr, "Got a report (printing only some attributes)\n");

        /* The ORADDRESS in the message is the (envelope) originator */
        status = X400msMsgGetStrParam(mp, X400_S_SUBJECT_IDENTIFIER,
                                      buffer, sizeof buffer, &length);
        if (status != X400_E_NOERROR) {
            fprintf(stderr, "Error from MsgGetStrParam: %s\n",
                    X400msError(status));
	    /* tidily close the session */
	    status = X400msClose(sp);
            exit(status);
        }
        printf("Subject Identifier: %.*s\n", (int)length, buffer);

        /* Get the primary recipients */
        for (n = 1;; n++) {
            status = X400msRecipGet(mp, X400_RECIP_REPORT, n, &rp);
            if (status == X400_E_NO_RECIP)
                break;
            if (status != X400_E_NOERROR) {
                fprintf(stderr, "Error from RecipGet: %s\n",
                        X400msError(status));
		/* tidily close the session */
		status = X400msClose(sp);
                exit(status);
            }

            /* Note: recipient may not actually have an O/R address */
            status = X400msRecipGetStrParam(rp, X400_S_OR_ADDRESS,
                                            recipient_str,
                                            sizeof recipient_str, &length);
            if (status != X400_E_NOERROR) {
                fprintf(stderr, "Error from RecipGetStrParam: %s\n",
                        X400msError(status));
		/* tidily close the session */
		status = X400msClose(sp);
                exit(status);
            }
	    /* print out the O/R Address of the report */
	    printf("Delivery Report for recipient %d: %.*s\n", n,
		   (int)length, recipient_str);

            /* The original message delivery time, if this attribute exists, 
	     * then the report is a positive Delivery Report */
            status = X400msRecipGetStrParam(rp, X400_S_MESSAGE_DELIVERY_TIME,
                                            buffer, sizeof buffer, &length);

            if (status == X400_E_NOERROR) {
                /* Positive Delivery Report */
                printf("Delivery Time: %.*s\n", (int)length, buffer);
            }
            else {
                /* Negative Delivery Report */
                printf("Negative Delivery Report for recipient %d: %.*s\n", n,
                       (int)length, recipient_str);

                /* Supplementary Info to the report */
                status = X400msRecipGetStrParam(rp, X400_S_SUPPLEMENTARY_INFO,
                                                buffer, sizeof buffer,
                                                &length);
                if (status != X400_E_NOERROR) {
                    fprintf(stderr, "Error from RecipGetStrParam: %s\n",
                            X400msError(status));
                    buffer[0] = '\0';
                }
                printf("Supplementary Info: %.*s\n", (int)length, buffer);

                /* The reason why the message was not delivered */
                status =
                    X400msRecipGetIntParam(rp, X400_N_NON_DELIVERY_REASON,
                                           &intparam);
                if (status != X400_E_NOERROR) {
                    fprintf(stderr, "Error from MsgGetIntParam: %s\n",
                            X400msError(status));
                }
                printf("Non-Delivery Reason: %d\n", intparam);

                /* The diagnostics of the report for this recipient */
                status =
                    X400msRecipGetIntParam(rp, X400_N_NON_DELIVERY_DIAGNOSTIC,
                                           &intparam);
                if (status != X400_E_NOERROR) {
                    fprintf(stderr, "Error from MsgGetIntParam: %s\n",
                            X400msError(status));
                }
                printf("Non-Delivery Diagnostic: %d\n", intparam);
            }
        }

	num_unsigned_rcvd++ ;
        status = X400msMsgDelete(mp, 0);
	if (status != X400_E_NOERROR) {
	    fprintf(stderr, "Error from X400msMsgDelete: %s\n",
		    X400msError(status));
	}
        return (status);
    }


    /* The message identifier */
    status = X400msMsgGetStrParam(mp, X400_S_MESSAGE_IDENTIFIER,
                                  buffer, sizeof buffer, &length);
    if (status != X400_E_NOERROR) {
        fprintf(stderr, "Error from MsgGetStrParam: %s\n",
                X400msError(status));
	/* tidily close the session */
	status = X400msClose(sp);
        exit(status);
    }
    printf("Message Identifier: %.*s\n", (int)length, buffer);

    /* The ORADDRESS in the message is the (envelope) originator */
    status = X400msMsgGetStrParam(mp, X400_S_OR_ADDRESS,
                                  buffer, sizeof buffer, &length);
    if (status != X400_E_NOERROR) {
        fprintf(stderr, "Error from MsgGetStrParam: %s\n",
                X400msError(status));
	/* tidily close the session */
	status = X400msClose(sp);
        exit(status);
    }
    printf("Originator: %.*s\n", (int)length, buffer);

    /* Get the envelope recipients */
    for (n = 1;; n++) {
        status = X400msRecipGet(mp, X400_RECIP_ENVELOPE, n, &rp);
        if (status == X400_E_NO_RECIP)
            break;
        if (status != X400_E_NOERROR) {
            fprintf(stderr, "Error from RecipGet: %s\n", X400msError(status));
	    /* tidily close the session */
	    status = X400msClose(sp);
            exit(status);
        }

        /* Note: recipient may not actually have an O/R address */
        status = X400msRecipGetStrParam(rp, X400_S_OR_ADDRESS,
                                        buffer, sizeof buffer, &length);
        if (status == X400_E_NOERROR) {
            printf("Env Recipient %d: %.*s\n", n, (int)length, buffer);
        }

	/* get back status of the Proof Of Del Request in the recipient */
	status = X400msRecipGetIntParam(rp, X400_N_PROOF_OF_DEL_REQ,
				   &intparam);
        if (status == X400_E_NOERROR) {
            printf("Recipient proof of delivery request is %d\n", intparam);
        } else {
            fprintf(stderr, "Error getting proof of delivery request: %s\n", 
		X400msError(status));
	}

	/* report content integrity check info if available */
	get_cic(rp);

	/* return MessageToken info if available */
	report_msg_token_info(rp);

	/* get and display the seccurity label if available */
	get_sec_label(mp);
    }

    /* Get the primary recipients */
    for (n = 1;; n++) {
        status = X400msRecipGet(mp, X400_RECIP_PRIMARY, n, &rp);
        if (status == X400_E_NO_RECIP)
            break;
        if (status != X400_E_NOERROR) {
            fprintf(stderr, "Error from RecipGet: %s\n", X400msError(status));
	    /* tidily close the session */
	    status = X400msClose(sp);
            exit(status);
        }

        /* Note: recipient may not actually have an O/R address */
        status = X400msRecipGetStrParam(rp, X400_S_OR_ADDRESS,
                                        buffer, sizeof buffer, &length);
        if (status == X400_E_NOERROR) {
            printf("Primary Recipient %d: %.*s\n", n, (int)length, buffer);
        }
    }

    /* Subject is optional */
    status = X400msMsgGetStrParam(mp, X400_S_SUBJECT,
                                  buffer, sizeof buffer, &length);
    if (status == X400_E_NOERROR)
        printf("Subject: %.*s\n", (int)length, buffer);

    /* And message text (or it could be another type) */
    status = X400msMsgGetStrParam(mp, X400_T_IA5TEXT,
                                  buffer, sizeof buffer, &length);
    if (status == X400_E_NOERROR)
        printf("Text:\n%.*s\n", (int)length, buffer);

    /* Deletes message in message store as well as internal copy */
    status = X400msMsgDelete(mp, 0);

    return status;
}

static void setup_default_sec_env(
    struct X400msSession *sp, 
    char *id, 
    char *dn, 
    char *pw
)
{
    int status;

    /* first set a default security identity. This passes in the name of a
     * directory, which contains an x509 subdirectory in which all Identities
     * are held */
    /* X400msSetStrDefault(sp, X400_S_SEC_IDENTITY, "/var/isode/dsa-db/", -1);
     * */
    X400msSetStrDefault(sp, X400_S_SEC_IDENTITY, id, -1); 

    /* select by DN which Identity is to be used (if there are several) 
     * Currently these must be PKCS12 files */
    status = X400msSetStrDefault (sp, X400_S_SEC_IDENTITY_DN, dn, -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "X400msSetStrDefault returned error: %s\n", 
	    X400msError (status));
	exit (status);
    }

    /* passphrase used to open the Identity */
    status = X400msSetStrDefault (sp, X400_S_SEC_IDENTITY_PASSPHRASE, pw, -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "X400msSetStrDefault returned error: %s\n", 
	    X400msError (status));
	exit (status);
    }
    return;
}

static void report_msg_token_info(
    struct X400Recipient *rp
)
{
    char buffer[BUFSIZ];
    int  status;
    int  param;
    size_t  length;

    status = X400msRecipGetIntParam(rp, 
	  X400_N_MSGTOK_STATUS, &param);
    if (status == X400_E_NO_VALUE) {
	fprintf(stderr, "No MessageToken present in recipient\n");
	/* no further information available */
	return ;
    } else if (status != X400_E_NOERROR) {
	fprintf(stderr, "Error from RecipGetIntParam: %s (%d)\n",
	    X400msError(status), 
	    X400_N_MSGTOK_STATUS);
	fprintf(stderr, "Message Token status not available\n");
    } else {
	fprintf(stderr, 
	    "Message Token status %d \n", param);
    }

    switch (param) {
	case X400_E_NOERROR:
	    fprintf(stderr, "Token OK (%d)\n", param);
	    break;
	case X400_E_X509_ENV:
	    fprintf(stderr, 
		"Message Token validation cannot take place because the security environment is invalid (%d):\n", 
		param);
	    /* other values will not be available */
	    return ;
    case X400_E_X509_VERIFY_FAIL_NO_CERT:
    case X400_E_X509_VERIFY_FAIL_NO_PUBKEY:
    case X400_E_X509_VERIFY_FAIL_INCOMPAT_ALG:
    case X400_E_X509_VERIFY_FAIL_UNSUPPORTED_ALG:
    case X400_E_X509_VERIFY_FAIL:
    case X400_E_X509_CERT_INVALID:
    case X400_E_X509_ITEM_INVALID:
	    fprintf(stderr, 
		"Message Token validation failed (%d): %s\n",
		    param, X400msError(param));
	    break;
	default:
	    fprintf(stderr, 
		"Unexpected Message Token validation result (%d): %s\n", 
		param, X400msError(param));
	    break;

    }

    show_certificate (rp, X400_N_CERT_MSGTOK, "message-token");

    status = X400msRecipGetStrParam(rp, 
	  X400_S_MSGTOK_RECIP,
	  buffer, sizeof buffer, &length);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "Error from RecipGetStrParam: %s\n",
		X400msError(status));

	fprintf(stderr, "recipient in token not available\n");
    } else {
	fprintf(stderr, 
	    "Message Token(%u): recipient in Token '%.*s'\n", 
		(unsigned)length, (int)length, buffer);
    }

    status = X400msRecipGetStrParam(rp, 
	  X400_S_MSGTOK_DER,
	  buffer, sizeof buffer, &length);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "Error from RecipGetStrParam: %s (%d)\n",
	    X400msError(status), 
	    X400_S_MSGTOK_DER);
	fprintf(stderr, "Message Token DER not available\n");
    } else {
	fprintf(stderr, 
		"Message Token DER available (%u bytes)\n", (unsigned)length);
    }

    status = X400msRecipGetIntParam(rp, 
	  X400_N_MSGTOK_SEQ_NUM, &param);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "Error from RecipGetStrParam: %s (%d)\n",
	    X400msError(status), 
	    X400_N_MSGTOK_STATUS);
	fprintf(stderr, "Message Token seq num not available\n");
    } else {
	fprintf(stderr, 
	    "Message Token seq num %d \n", param);
    }

    status = X400msRecipGetStrParam(rp, 
	  X400_S_MSGTOK_SEC_LAB,
	  buffer, sizeof buffer, &length);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "Error from RecipGetStrParam: %s (%d)\n",
	    X400msError(status), 
	    X400_S_MSGTOK_SEC_LAB);
	fprintf(stderr, "Message Token Security Label DER not available\n");
    } else {
	fprintf(stderr, 
		"Message Token Security Label DER available (%u bytes)\n", (unsigned)length);
	fprintf(stderr, "Security Label from Message Token is:\n");
	print_sec_label((unsigned char *)buffer, length);
    }

    /* get back status of the Content Integrity Check in the message token */
    status = X400msRecipGetIntParam(rp, X400_N_MSGTOK_CIC_STATUS,
				    &param);
    if (status == X400_E_NO_VALUE) {
	printf("No Content Integrity Check in token\n");
    } else if (status != X400_E_NOERROR) {
	printf("Error from RecipGetIntParam: %s\n", X400msError(status));

    } else {

	/* report CIC information */
	switch (param) {
	case X400_E_NOERROR:
	    printf("Content Integrity Check in token succeeded\n");
	    break;
	default:
	    printf("Content Integrity Check in token error (%d): %s\n", 
		   param, X400msError(param));
	    break;
	}

	status = X400msRecipGetStrParam(rp, 
					X400_S_MSGTOK_CIC,
					buffer, sizeof buffer, &length);
	if (status != X400_E_NOERROR) {
	    fprintf(stderr, "Error from RecipGetStrParam: %s (%d)\n",
		    X400msError(status), 
		    X400_S_MSGTOK_CIC);
	    fprintf(stderr, "Message Token Content Integrity Check DER not available\n");
	} else {
	    fprintf(stderr, 
		    "Message Token Content Integrity Check DER available (%u bytes)\n",
		    (unsigned)length);
	}

	show_certificate (rp, X400_N_CERT_MSGTOK_CIC, "token CIC");
    }

    status = X400msRecipGetIntParam(rp, 
	  X400_N_MSGTOK_PODR_STATUS, &param);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "Error from RecipGetStrParam: %s (%d)\n",
	    X400msError(status), 
	    X400_N_MSGTOK_PODR_STATUS);
	fprintf(stderr, "Message Token proof of delivery status not available\n");
    } else {
	fprintf(stderr, 
	    "Message Token proof of delivery status %d \n", param);
    }

    status = X400msRecipGetIntParam(rp, 
	  X400_N_MSGTOK_SEC_LAB_STATUS, &param);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "Error from RecipGetStrParam: %s (%d)\n",
	    X400msError(status), 
	    X400_N_MSGTOK_SEC_LAB_STATUS);
	fprintf(stderr, "Message Token security label status not available\n");
    } else {
	fprintf(stderr, 
	    "Message Token security label status %d \n", param);
    }

    status = X400msRecipGetIntParam(rp, 
	  X400_N_MSGTOK_RECIP_STATUS, &param);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "Error from RecipGetStrParam: %s (%d)\n",
	    X400msError(status), 
	    X400_N_MSGTOK_RECIP_STATUS);
	fprintf(stderr, "Message Token recip status not available\n");
    } else {
	fprintf(stderr, 
	    "Message Token recip status %d \n", param);
    }


    return;
}

   
static int get_sec_label(
    struct X400msMessage *mp
)
{
    unsigned char slab_buffer[XML_BUFSIZE];
    int status;
    size_t length;
     
    status = X400msMsgGetStrParam(mp,
				   X400_S_SECURITY_LABEL,
				   (char*)slab_buffer,
				   XML_BUFSIZE,
				   &length);
    if (status == X400_E_NO_VALUE) {
	printf("No security label\n");
    } else if (status !=  X400_E_NOERROR) {
	fprintf(stderr,"Failed to fetch security label: %d",status);
	return (status);
    } else {
	fprintf(stderr, "Security Label from envelope is:\n");
	print_sec_label(slab_buffer, length);
    }
    return X400_E_NOERROR;
}
   
static void print_sec_label(
    unsigned char slab_buffer[],
    unsigned int length
)
{

    char  xml_buffer[XML_BUFSIZE];
    int status;
    
    status = SecLabelInit("Example program");
    if (status != SECLABEL_E_NOERROR) {
	fprintf(stderr, "SecLabelInit returned error %d\n", status);
	return ;
    }

    status =  SecLabelPrint(slab_buffer,
			    length,
			    xml_buffer,
			    XML_BUFSIZE);
    
    if (status != SECLABEL_E_NOERROR) {
	fprintf(stderr, "SecLabelParse returned error %d\n", status);
	return ;
    }
    
    /* You could now write out the XML file, or parse it in memory..*/
    printf("Got security label:%s\n", xml_buffer);
    return;
}

static int get_cic(
    struct X400Recipient *rp
)
{
    char buffer[BUFSIZ];
    int  status, cic_status;
    int  intparam;
    size_t  length;

    /* get back status of the Content Integrity Check in the recipient */
    cic_status = X400msRecipGetIntParam(rp, X400_N_RECIP_CIC_STATUS,
			       &intparam);
    if (cic_status == X400_E_NO_VALUE) {
	printf("No Content Integrity Check in recipient\n");
	return X400_E_NOERROR;
    } else if (cic_status != X400_E_NOERROR) {
	printf("Error from RecipGetIntParam: %s\n", X400msError(cic_status));
	return cic_status;
    }

    /* report CIC information */
    switch (intparam) {
    case X400_E_NOERROR:
	printf("Content Integrity Check succeeded\n");
	break;
    default:
	printf("Content Integrity Check error (%d): %s\n", 
	       intparam, X400msError(intparam));
	break;
    }

    /* Return the Content Integrity Check */
    status = X400msRecipGetStrParam(rp, X400_S_RECIP_CIC,
				    buffer, sizeof buffer, &length);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "Error getting recipient cic: %s\n",
		X400msError(status));
    } else {
	printf("Content Integrity Check found in recipient (%d)\n", 
	       (int)length);
    }

    show_certificate (rp, X400_N_CERT_RECIP_CIC, "recipient CIC");

    return X400_E_NOERROR;
}

static void show_certificate (
    struct X400Recipient *rp,
    int certtype,
    const char *tag)
{
    int status;
    struct X400Certificate *cert;
    char buffer[BUFSIZ];
    size_t length;
    int paramval;

    status = X400RecipGetCert (rp, certtype, &cert);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "Error getting %s certificate: %s\n",
		 tag, X400msError (status));
	return;
    }

    /* Return the subject DN from Orig cert */
    status = X400CertGetStrParam(cert, X400_S_CERT_SUBJECT_DN,
				 buffer, sizeof buffer, &length);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "Error getting subject of cert used for %s: %s\n",
		tag, X400msError(status));
    } else {
	printf("Subject of Cert used to verify %s (%.*s)\n", 
	       tag, (int)length, buffer);
    }

    /* Return the issuer DN from Orig cert */
    status = X400CertGetStrParam(cert, X400_S_CERT_ISSUER_DN,
				 buffer, sizeof buffer, &length);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "Error getting issuer of cert used for %s: %s\n",
		tag, X400msError(status));
    } else {
	printf("Issuer of Cert used to verify %s (%.*s)\n", 
	       tag, (int)length, buffer);
    }

    /* Return the serial number from Orig cert used to verify the 
     * Content Integrity Check */
    status = X400CertGetStrParam(cert, X400_S_CERT_SERIAL_NUM,
				 buffer, sizeof buffer, &length);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "Error getting serial num of cert used for %s: %s\n",
		tag, X400msError(status));
    } else {
	printf("Serial Num of Cert used to verify %s (%.*s)\n", 
	       tag, (int)length, buffer);
    }

    /* Return the Orig cert used to verify the Content Integrity Check */
    status = X400CertGetStrParam(cert, X400_S_CERT_BER,
				 buffer, sizeof buffer, &length);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "Error getting cert der used for %s: %s\n",
		tag, X400msError(status));
    } else {
	printf(
	    "Returned Cert used to verify %s (%d)\n", 
	    tag, (int)length);
    }

    /* Find any ORaddress subject alt name and its status */
    status = X400CertGetStrParam(cert, X400_S_OR_ADDRESS,
				 buffer, sizeof buffer, &length);
    if (status != X400_E_NOERROR) {
	if ( status == X400_E_NO_VALUE ) {
	    fprintf(stderr, "No ORaddress subject alt. name\n");
	} else {
	    fprintf(stderr, "Error from CertGetStrParam: %s\n",
		    X400msError(status));
	    return;
	}
    } else {
	fprintf(stderr, "ORaddress subject alt name: '%.*s'\n",
		(int)length, buffer);
    }

    status = X400CertGetIntParam(cert, X400_N_CERT_ORADDRESS_STATUS, &paramval);
    
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "Error from CertGetStrParam: %s\n",
		    X400msError(status));
	return;
    }

    fprintf(stderr, "ORaddress subject alt name status: %s\n",
	    X400msError (paramval));
}

static void usage(void) {
    printf("usage: %s\n", optstr);
    printf("\t where:\n");
    printf("\t -u : Don't prompt to override defaults \n");
    printf("\t -3 : Use P3 connection \n");
    printf("\t -7 : Use P7 connection \n");
    printf("\t -m : OR Address in P7 bind arg \n");
    printf("\t -d : DN in P7 bind arg \n");
    printf("\t -p : Presentation Address of P7 Store \n");
    printf("\t -w : P7 password of P7 user \n");
    printf("\t -M : OR Address in P3 bind arg \n");
    printf("\t -D : DN in P3 bind arg \n");
    printf("\t -P : Presentation Address of P3 server\n");
    printf("\t -W : P3 password of P3 user \n");
    printf("\t -e : Security Environment (dir with x509 subdir)\n");
    printf("\t -x : DN of X.509 Digital Identity\n");
    printf("\t -b : Passphrase for private key in PKCS12 file\n");
    printf("\t -E : Fetch messages until all read\n");
    return;
}
