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
 * Simple example program for receiving a message from a store.
 * The security environment is set up so that signed messages
 * will have the MOAC verified.
 * Note that in this version, a valid Digital Identity is required 
 * in order to perform verification.
 */
#include <stdio.h>
#include <stdlib.h>
#include <x400_msapi.h>
#include "example.h"

int num_unsigned_rcvd;
int num_unverified_rcvd;
int num_verified_rcvd;

static char *optstr = "u37m:d:p:w:M:D:P:W:e:b:x:";

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

static void show_certificate (struct X400msMessage *mp);

/*! Main function of mhsrcv 
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

#ifdef USING_ALERTS
    /* If we register the alert auto-action, we will get an alert indication
       when a message is delivered. So there is no need to poll at
       short intervals within X400ms_Wait - we can do a slow background
       poll and rely on the Alert indication to wake the code up instead */
    X400msSetIntDefault(sp, X400_N_WAIT_INTERVAL, 1000);
#endif

    /* setup logging from $(ETCDIR)x400api.xml or $(SHAREDIR)x400api.xml */
    X400msSetStrDefault(sp, X400_S_LOG_CONFIGURATION_FILE, "x400api.xml", 0);

    if (contype == 0) {
#ifdef WANT_AUTOFORWARDING
        struct X400msAutoActionParameter *aa_param;

        /* Register an Autoforwarding Autoaction. */
        aa_param = X400msNewAutoActionParameter();

        /* Add mandatory things to AutoAction parameter for auto-forwarding:
           i.e. recipient address */
        X400RecipNew(0, &rp);

        X400RecipAddStrParam(rp, X400_S_OR_ADDRESS, def_oraddr,
                             strlen(def_oraddr));

        X400msAutoActionParameterAddRecip(aa_param, X400_RECIP_STANDARD, rp);

        X400msAutoActionParameterAddStrParam(aa_param,
                                             X400_S_CONTENT_IDENTIFIER,
                                             "AF contentid", -1);

        X400msAutoActionParameterAddIntParam(aa_param, X400_N_DISCLOSURE, 1);

        X400msAutoActionParameterAddIntParam(aa_param,
                                             X400_N_IMPLICIT_CONVERSION_PROHIBITED,
                                             1);

        X400msAutoActionParameterAddIntParam(aa_param,
                                             X400_N_ALTERNATE_RECIPIENT_ALLOWED,
                                             1);

        X400msAutoActionParameterAddIntParam(aa_param,
                                             X400_N_CONTENT_RETURN_REQUEST,
                                             1);

        X400msAutoActionParameterAddIntParam(aa_param, X400_N_PRIORITY, 2);

        X400msAutoActionParameterAddStrParam(aa_param,
                                             X400_S_AUTO_FORWARDING_COMMENT,
                                             "This message was autoforwarded",
                                             -1);

        X400msAutoActionParameterAddStrParam(aa_param,
                                             X400_S_COVER_NOTE,
                                             "This is a cover note", -1);

        X400msAutoActionParameterAddStrParam(aa_param,
                                             X400_S_THIS_IPM_PREFIX,
                                             "AutoForwarded:", -1);

        status = X400msRegisterAutoAction(sp, X400_AUTO_FORWARDING,
                                          4, aa_param);
        if (status != X400_E_NOERROR) {
            fprintf(stderr,
                    "Error in RegisterAutoAction: %s\n", X400msError(status));
	    /* tidily close the session */
	    status = X400msClose(sp);
            exit(status);
        }
        printf("Registered AutoForwarding autoaction (id = 4) OK\n");
        X400msFreeAutoActionParameter(aa_param);
#endif

#ifdef USING_ALERTS
        /* No parameter needed for Alert autoaction - we do not support 
           configuration of requested-attributes in this API yet. */
        status = X400msRegisterAutoAction(sp, X400_AUTO_ALERT, 9, NULL);
        if (status != X400_E_NOERROR) {
            fprintf(stderr, "Error in RegisterAutoAction: %s\n",
                    X400msError(status));
	    /* tidily close the session */
	    status = X400msClose(sp);
            exit(status);
        }
        printf("Registered AutoAlert autoaction (id = 9) OK\n");
#endif

        /* Just test the register and deregister functions */
        status = X400msRegisterAutoAction(sp, X400_AUTO_ALERT, 10, NULL);
        if (status != X400_E_NOERROR) {
            fprintf(stderr, "Error in RegisterAutoAction: %s\n",
                    X400msError(status));
	    /* tidily close the session */
	    status = X400msClose(sp);
            exit(status);
        }
        printf("Registered AutoAlert autoaction (id = 10) OK\n");

        /* Lets do a deregistration of the action we just registered */
        status = X400msDeregisterAutoAction(sp, X400_AUTO_ALERT, 10);
        if (status != X400_E_NOERROR) {
            fprintf(stderr, "Error in DeregisterAutoAction: %s\n",
                    X400msError(status));
	    /* tidily close the session */
	    status = X400msClose(sp);
            exit(status);
        }
        printf("Deregistered AutoAlert autoaction (id = 10) OK\n");
    }

    if (nummsg == 0) {
        printf ("no messages - waiting 60 seconds for a message to be delivered.....\n");
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
    status = get_msg(sp);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "Error in getting msg: %s\n",
		X400msError(status));
	exit(status);
    }
    fprintf(stderr, "got second\n");

    status = get_msg(sp);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "Error in getting msg: %s\n",
		X400msError(status));
	exit(status);
    }
    fprintf(stderr, "got third\n");

    status = get_msg(sp);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "Error in getting msg: %s\n",
		X400msError(status));
	exit(status);
    }
    fprintf(stderr, "got third\n");

    status = get_msg(sp);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "Error in getting msg: %s\n",
		X400msError(status));
	exit(status);
    }
    fprintf(stderr, "got third\n");

    status = get_msg(sp);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "Error in getting msg: %s\n",
		X400msError(status));
	exit(status);
    }
    fprintf(stderr, "got third\n");

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
    setup_default_sec_env(sp, security_id, identity_dn2, passphrase);

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
	    printf("Positive Delivery Report for recipient %d: %.*s\n", n,
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
                printf("Negative Delivery Report for recipient %d: %s\n", n,
                       recipient_str);

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

    /* Check the MOAC */
    status = X400msMsgGetIntParam(mp, X400_N_MOAC_STATUS, &intparam);
    switch ( status ) {
    case X400_E_NO_VALUE:
	num_unverified_rcvd++ ;
	fprintf (stderr, "No MOAC in message\n");
	break;

    default:
	num_unverified_rcvd++ ;
	fprintf (stderr, "Unexpected error getting MOAC status: %s\n",
		 X400msError (status));
	break;

    case X400_E_NOERROR:
	fprintf (stderr, "Have MOAC in message\n");
	switch (intparam) {
	case X400_E_NOERROR:
	    fprintf(stderr, 
		    "MsgGet successfully verified signature in message\n");
	    show_certificate (mp);
	    fprintf(stderr, "continuing ...\n");
	    num_verified_rcvd++ ;
	    break;
	case X400_E_X509_VERIFY_FAIL:
	    fprintf(stderr, "Error from MsgGet: %s\n", X400msError(status));
	    show_certificate (mp);
	    fprintf(stderr, "continuing ...\n");
	    num_unverified_rcvd++ ;
	    break;
	case X400_E_X509_VERIFY_FAIL_NO_CERT:
	    fprintf(stderr, "Error from MsgGet: %s\n", X400msError(status));
	    fprintf(stderr, "continuing ...\n");
	    num_unverified_rcvd++ ;
	    break;
	case X400_E_X509_VERIFY_FAIL_NO_PUBKEY:
	    fprintf(stderr, "Error from MsgGet: %s\n", X400msError(status));
	    show_certificate (mp);
	    fprintf(stderr, "continuing ...\n");
	    num_unverified_rcvd++ ;
	    break;
	case X400_E_X509_VERIFY_FAIL_INCOMPAT_ALG:
	    fprintf(stderr, "Error from MsgGet: %s\n", X400msError(status));
	    show_certificate (mp);
	    fprintf(stderr, "continuing ...\n");
	    num_unverified_rcvd++ ;
	    break;
	default :
	    fprintf(stderr, "Error from MsgGet: %s\n", X400msError(status));
	    /* tidily close the session */
	    status = X400msClose(sp);
	    exit(status);
	    break;
	}
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
            printf("Recipient %d: %.*s\n", n, (int)length, buffer);
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

static void show_certificate (struct X400msMessage *mp)
{
    int status;
    struct X400Certificate *cert;
    char buffer[BUFSIZ];
    size_t length;
    int paramval;

    status = X400msMsgGetCert (mp, X400_N_CERT_MOAC, &cert);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "Error getting certificate: %s\n",
		 X400msError (status));
	return;
    }

    status = X400CertGetStrParam(cert, X400_S_CERT_SUBJECT_DN,
				 buffer, sizeof buffer, &length);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "Error from CertGetStrParam: %s\n",
		X400msError(status));
	return;
    }
    fprintf(stderr, 
	    "subject DN of originator certificate '%.*s'\n", (int)length, buffer);
    status = X400CertGetStrParam(cert, X400_S_CERT_ISSUER_DN,
				 buffer, sizeof buffer, &length);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "Error from CertGetStrParam: %s\n",
		X400msError(status));
	return;
    }
    fprintf(stderr, 
	    "issuer DN of originator certificate '%.*s'\n", (int)length, buffer);

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
	fprintf(stderr, "ORaddress subject alt name: '%.*s'\n", (int)length, buffer);
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
    return;
}
