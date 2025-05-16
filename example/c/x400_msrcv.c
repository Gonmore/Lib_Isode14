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
 * Simple example program for receiving a message from a store.
 */
#include <stdio.h>
#include <stdlib.h>
#include <x400_msapi.h>
#include <amhs_att.h>
#include <seclabel_api.h> /* For security labels */
#include "example.h"

static char *optstr = "u371m:d:p:w:M:D:P:W:";

static void usage(void);


#ifdef WANT_DL_EXP_HIST
static int do_dl_hist (
    struct X400msMessage *mp
);
#endif

static int get_body_parts(
    struct X400msMessage *mp
);

static int get_bp(
    struct X400msMessage *mp,
    struct X400Bodypart **bp,
    int att_num
);

static int get_msgbp(
    struct X400msMessage *mp,
    struct X400Message **x400_mp,
    int att_num
);

static int do_xmsg_headers(
    struct X400Message *x400_mp
);

static int do_xmsg_content_as_bp(
    struct X400Message *x400_mp
);

static int do_xrep_headers(
    struct X400Message *x400_mp
);

static int do_xmsg_headers(
    struct X400Message *x400_mp
);

static int do_xrep_content_as_bp(
    struct X400Message *x400_mp
);

static int get_recips(
    struct X400Message 	*x400_mp,
    int			type
);

static int do_redirection_hist (
    struct X400Recipient *rp
);

static int do_redirection_hist_env (
    struct X400msMessage *mp
);

static int do_trace_info (
    struct X400msMessage *mp
);

static int do_internal_trace_info (
    struct X400msMessage *mp
);

static int do_orig_and_dl (
    struct X400msMessage *mp
);

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
    int type;
    int seqn;
    struct X400msSession *sp;
    struct X400msMessage *mp;
    struct X400Recipient *rp;
    int n;
    size_t length;
    int contype;
    char *def_oraddr;
    char *def_dn;
    char *def_pa;
    int intparam;
    char recipient_str[BUFSIZ];
    int num_recips = -1;

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

    if (talking_to_marben_ms)
	X400msSetConfigRequest(0);

    status = X400msOpen(contype, orn, def_dn, buffer, pa, &nummsg, &sp);
    if (status != X400_E_NOERROR) {
        fprintf(stderr, "Error in Open: %s\n", X400msError(status));
        exit(status);
    }

    if (talking_to_marben_ms)
	X400msSetIntDefault(sp, X400_N_STRICT_P7_1988, 1);

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

    status = X400msWait(sp, 1, &nummsg);
    if (status != X400_E_NOERROR) {
        fprintf(stderr, "Error from Wait: %s\n", X400msError(status));
	/* tidily close the session */
        status = X400msClose(sp);
        exit(status);
    }

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

        /* Get hold of the Originator and DL history*/
        status = do_orig_and_dl (mp);
        if (status != X400_E_NOERROR &&
            status != X400_E_MISSING_ATTR) {
            fprintf(stderr,"Failed to fetch originator and dl history: %s\n",
                    X400msError (status));
        }
        
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

            /* The original message delivery time, if this attribute exists, 
	     * then the report is a positive Delivery Report */
            status = X400msRecipGetStrParam(rp, X400_S_MESSAGE_DELIVERY_TIME,
                                            buffer, sizeof buffer, &length);
            if (status == X400_E_NOERROR) {
                /* Positive Delivery Report */
                printf("Positive Delivery Report for recipient %d: %s\n", n,
                       recipient_str);
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

        status = X400msMsgDelete(mp, 0);
        status = X400msClose(sp);
        exit(status);
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

    status = X400msMsgCountRecip(mp, X400_RECIP_PRIMARY, &num_recips);
    if (status != X400_E_NOERROR) {
        fprintf(stderr, "Error from MsgCountRecip: %s\n",
                X400msError(status));
	/* tidily close the session */
	status = X400msClose(sp);
        exit(status);
    }

    printf("There are %d recipients\n", num_recips);

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
            printf("Envelope Recipient %d: %.*s\n", n, (int)length, buffer);
        }
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

#ifdef WANT_DL_EXP_HIST
    /* DL Expansion history */
    {

	status = do_dl_hist(hist);

	/* Normally the status is "X400_E_NO_VALUE" If there is no
	 * DL Expansion history.
	 * Or X400_E_MISSING_ATTR if the DL Expansion history entry is missing
	 * (so end of loop)
	 */
	if ( status == X400_E_NO_VALUE) {
	    /* do nothing */
	} else if (status == X400_E_MISSING_ATTR ) {
	    printf("There is no DL expansion history\n");
	} else {
	    /* There has been an error fetching the DL expansion history*/
	     printf("error getting DL Expansion History\n");
	     fprintf (stderr, "Error in X400mtDLExpHistGet: %s\n", 
		      X400msError (status));
	    return status;
	}
    }
#endif

#define WANT_TRACE_INFO 1    
#ifdef WANT_TRACE_INFO
 {

     status = do_trace_info(mp);
     if ( status == X400_E_NO_VALUE) {
	 
     } else if (status == X400_E_MISSING_ATTR ) {
	 printf("There is no Trace Info\n");
     } else {
	 printf("error getting Trace Info\n");
	 fprintf (stderr, "Error fetching trace info: %s\n", 
		  X400msError (status));
	 return status;
     }
	 
 }
    
#endif

#define USE_INTERNAL_TRACE_INFO 1
#ifdef  USE_INTERNAL_TRACE_INFO
 {

     status = do_internal_trace_info(mp);
     if ( status == X400_E_NO_VALUE) {
	 /* do nothing */
     } else if (status == X400_E_MISSING_ATTR ) {
	 printf("There is no internal Trace Info\n");
     } else {
	 printf("error getting int Trace Info\n");
	 fprintf (stderr, "Error fetching int trace info: %s\n", 
		  X400msError (status));
	 return status;
     }
	 
 }  
#endif
  

    
#define USE_REDIRECTION_HISTORY 1
#ifdef  USE_REDIRECTION_HISTORY
 {
     status = do_redirection_hist_env(mp);
     if ( status == X400_E_NO_VALUE ) {
	 /* do nothing */
     } else if (status == X400_E_MISSING_ATTR ) {
	 printf("There is no Redirection History\n");
     } else {
	 printf("error getting Redirection History\n");
	 fprintf (stderr, "Error fetching Redirection History: %s\n", 
		  X400msError (status));
	 return status;
     }  
 }
#endif   
    /* Security Label */

#ifdef USE_SEC_LABEL
    
#define XML_BUFSIZE 1024
 {
     char  xml_buffer[XML_BUFSIZE];
     unsigned char slab_buffer[XML_BUFSIZE];
     
     status = X400msMsgGetStrParam(mp,
				   X400_S_SECURITY_LABEL,
				   (char*)slab_buffer,
				   XML_BUFSIZE,
				   &length);
    if (status == X400_E_NO_VALUE) {
	printf("No security label\n");
    } else if (status !=  X400_E_NOERROR) {
	fprintf(stderr,"Failed to fetch security label: %d",status);
	exit(1);
    } else {
	int sec_status = 0;
	
	sec_status = SecLabelInit("Example program");
	if (status != SECLABEL_E_NOERROR) {
	    fprintf(stderr, "SecLabelInit returned error %d\n", status);
	    exit(1);
	}

	status =  SecLabelPrint(slab_buffer,
				length,
				xml_buffer,
				XML_BUFSIZE);
	
	if (status != SECLABEL_E_NOERROR) {
	    fprintf(stderr, "SecLabelParse returned error %d\n", status);
	    exit(1);
	}
	
	/* You could now write out the XML file, or parse it in memory..*/
	printf("Got security label:%s\n",xml_buffer);
    }

    
 }
    
#endif


 
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


    /* ATS attributes */
    status = X400msMsgGetStrParam(mp, ATS_S_PRIORITY_INDICATOR,
                                  buffer, sizeof buffer, &length);
    if (status == X400_E_NOERROR)
        printf("ATS Priority Indicator: %.*s\n", (int)length, buffer);

    status = X400msMsgGetStrParam(mp, ATS_S_FILING_TIME,
                                  buffer, sizeof buffer, &length);
    if (status == X400_E_NOERROR)
        printf("ATS Filing Time: %.*s\n", (int)length, buffer);

    status = X400msMsgGetStrParam(mp, ATS_S_OPTIONAL_HEADING_INFO,
                                  buffer, sizeof buffer, &length);
    if (status == X400_E_NOERROR)
        printf("ATS Optional Heading Info: %.*s\n", (int)length, buffer);

    status = X400msMsgGetStrParam(mp, ATS_S_TEXT,
                                  buffer, sizeof buffer, &length);
    if (status == X400_E_NOERROR)
        printf("ATS Text: %.*s\n", (int)length, buffer);

    /* get all the attachments */
    status = get_body_parts(mp);

    /* Deletes message in message store as well as internal copy */
    status = X400msMsgDelete(mp, 0);

    status = X400msClose(sp);
    return(status);
}

#ifdef WANT_DL_EXP_HIST
static int do_dl_hist (
    struct X400msMessage *mp
)
{
    char    DLORAddress[BUFSIZ];
    char    DLORDN[BUFSIZ];
    char    DLExpTime[BUFSIZ];
    size_t  length;
    int n;
    int status;
    struct X400DLExpHist *hist = NULL;

    /* Loop through the entries, pulling out the OR Address, DN, and expansion
     * time for each entry.
     * Stop looping when there is no n entry
     */
    for ( n=1; ;n++ ) {

	status = X400msDLExpHistGet (mp,n,&hist);
	if (status != X400_E_NOERROR) {
	    return status;
	}
	
	status = X400DLGetStrParam (hist, X400_S_OR_ADDRESS,
				      DLORAddress, BUFSIZ , &length);
	if (status == X400_E_NO_VALUE) {
	    return X400_E_NO_VALUE;
	}
	if ( status == X400_E_NOERROR ) {
	    DLORAddress[length] = '\0';
	    printf ("DLExpansion List entry %d OR Address:%s\n",
		    n,DLORAddress);
	} else {
	    fprintf (stderr, "Error in :X400mtDLGetStrParam OR %s\n",
		     X400msError (status));
	}

	
	
	status = X400DLGetStrParam (hist, X400_S_DIRECTORY_NAME,
				    DLORDN, BUFSIZ , &length);
	if ( status == X400_E_NOERROR ) {
	    DLORDN[length] = '\0';
	    printf ("DLExpansion List entry %d DN :%s\n",n,DLORDN);
	} else {
	    fprintf (stderr, "Error in :X400mtDLGetStrParam DN %s\n",
		     X400msError (status));
	}
	    
	status = X400DLGetStrParam (hist, X400_S_DLEXP_TIME,
				    DLExpTime, BUFSIZ , &length);
	if ( status == X400_E_NOERROR ) {
	    DLExpTime[length] = '\0';
	    printf ("DLExpansion List entry %d Time :%s\n",n,DLExpTime);
	} else {
	    fprintf (stderr, "Error in :X400mtDLGetStrParam Time %s\n",
		     X400msError (status));
	}
	    
    }
    if (n == 1) {
	printf("No DLEH found in message");
    } else {
	printf("%d DLEH entries found in message", n - 1);
    }
    return X400_E_NOERROR;
}
#endif

static int get_body_parts(
    struct X400msMessage *mp
)
{
#define INIT_BUFFER_SIZE 10000
    int 	status;
    int 	num_atts = 0;
    int 	type;
    char 	*buf;
    size_t 	buflen;
    size_t 	ret_len;
    int		i;
    size_t	j;
    struct X400Message *x400_mp;
    struct X400Bodypart *bp;


    status = X400msMsgGetIntParam (mp, X400_N_NUM_ATTACHMENTS, &num_atts);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "Error in X400msMsgGetIntParam attachments %s\n",
		 X400msError (status));
	return status;
    }
    printf ("Number of attachments is %d\n", num_atts);

    buf = (char *) malloc(INIT_BUFFER_SIZE);
    if (buf == NULL) {
	fprintf(stderr, "can't alloc %d bytes\n", INIT_BUFFER_SIZE);
	return X400_E_NOMEMORY;
    }
    buflen = INIT_BUFFER_SIZE;

    for (i = 0; i <= num_atts ; i++) {
	printf ("\nGet attachment %d\n", i);
	status = X400msMsgGetAttachment(mp, i, &type, buf, buflen, &ret_len);
	if (status == X400_E_NOSPACE) {
	    fprintf(stderr, "realloc %ld bytes\n", (long)ret_len);
	    buf = (char *) realloc(buf, ret_len + 1);
	    if (buf == NULL) {
		fprintf(stderr, "can't alloc %d bytes\n", INIT_BUFFER_SIZE);
		return X400_E_NOMEMORY;
	    }
	    buflen = ret_len;
	    status = X400msMsgGetAttachment(
		mp, i, &type, buf, buflen, &ret_len);
	    /* fall through with status set */
	}
	/* for some attachments you must use the BodyPart functions */
	if (status == X400_E_COMPLEX_BODY) {
	    printf("got complex bodypart\n");
	    status = get_bp(mp, &bp, i);
	    if (status != X400_E_NOERROR ) {
		return status;
	    }
	}
	/* for forwarded messages you must use the BodyPart functions */
	else if (status == X400_E_MESSAGE_BODY) {
	    printf("got forwarded message bodypart\n");
	    status = get_msgbp(mp, &x400_mp, i);
	    if (status != X400_E_NOERROR ) {
		return status;
	    }
	}
	else if (status == X400_E_NOERROR) {
	    /* process text or binary attachment */
	    printf("got text or binary attachment\n");
	    switch (type) {
		case X400_T_BINARY:
		case X400_T_FTBP:
		    printf("First 1000 bytes of Binary Data for %s BP is:",
			type == X400_T_BINARY ? "Binary" : "FTBP");
		    for (j = 0; j < ret_len && j < 1000; j++)
			printf("%x ", buf[j]);
		    printf("\n");
		    break;
		default: 
		    printf("Printable Data for BP is:\n %s", buf);
		    break;
	    }
	}
	else if (status == X400_E_MISSING_ATTR) {
	    printf("No more attachments found\n");
	    return X400_E_NOERROR;
	}	    
	else {
	    /* got an error */
	    fprintf (stderr, "Error in X400msMsgGetAttachment %s\n",
		     X400msError (status));
	    break;
	}
    }
    return status;
}

static int get_bp(
    struct 	X400msMessage *mp,
    struct 	X400Bodypart **bpp,
    int 	att_num
)
{
    int 	status;
    int 	type;
    int 	bp_type;
    size_t	j;
    char 	*buf;
    size_t 	buflen;
    size_t 	ret_len;

#define INIT_BUFFER_SIZE 10000
    printf ("Get Bodypart %d\n", att_num);

    /* firstly get the Bodypart */
    status = X400msMsgGetBodypart(mp, att_num, &type, bpp);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "X400msMsgGetBodypart %d failed %s\n", att_num,
		 X400msError (status));
	return status;
    }
    printf("got Bodypart %d, type = %d\n", att_num, type);

    /* What kind of Bodypart using a attribute of the BP */
    status = X400BodypartGetIntParam(*bpp, X400_N_BODY_TYPE, &bp_type);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "X400msMsgGetBodypart failed %s\n",
		 X400msError (status));
	return status;
    }
    printf("got Bodypart %d, type = %s\n", att_num, 
	    bp_type == X400_T_IA5TEXT ? "IA5" : 
	    bp_type == X400_T_ISO8859_1 ? "ISO8859-1" :
	    bp_type == X400_T_ISO8859_2 ? "ISO8859_2" :
	    bp_type == X400_T_BINARY ? "Binary" : 
	    bp_type == X400_T_MESSAGE ? "Message" : 
	    bp_type == X400_T_FTBP ? "File Transfer BP" :
	    bp_type == X400_T_GENERAL_TEXT ? "General Text" : "Unknown"
	    );

    /* setup buffer to use to return BP data */
    buf = (char *) malloc(INIT_BUFFER_SIZE);
    if (buf == NULL) {
	fprintf(stderr, "can't alloc %d bytes\n", INIT_BUFFER_SIZE);
	return X400_E_NOMEMORY;
    }
    buflen = INIT_BUFFER_SIZE;
    /* Read data from the Bodypart */
    status = X400BodypartGetStrParam(*bpp, X400_S_BODY_DATA, buf, 
	buflen, &ret_len);
    if (status == X400_E_NOSPACE) {
	fprintf(stderr, "realloc %ld bytes\n", (long)ret_len);
	buf = (char *) realloc(buf, ret_len + 1);
	if (buf == NULL) {
	    fprintf(stderr, "can't alloc %d bytes\n", INIT_BUFFER_SIZE);
	    return X400_E_NOMEMORY;
	}
	buflen = ret_len;
	status = X400BodypartGetStrParam(*bpp, X400_S_BODY_DATA, buf, 
	    buflen, &ret_len);
	/* fall through with status set */
    }
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "X400msMsgGetBodypart failed %s\n",
		 X400msError (status));
	return status;
    }
    printf("got %ld bytes of Bodypart data\n", (long)ret_len);
    switch (bp_type) {
	case X400_T_BINARY:
	case X400_T_FTBP:
	    printf("First 1000 bytes of Binary Data for %s BP is:",
		type == X400_T_BINARY ? "Binary" : "FTBP");
	    for (j = 0; j < ret_len && j < 1000; j++)
		printf("%x ", buf[j]);
	    printf("\n");
	    break;
	default: 
	    printf("Printable Data for BP is:\n %s", buf);
	    break;
    }



    return X400_E_NOERROR;

}

/*
 * Extract and return a Bodypart from an MS Message
 */
static int get_msgbp(
    struct X400msMessage *mp,
    struct X400Message **x400_mpp,
    int bp_num
)
{
    struct X400Message *x400_mp;
    int		status;
    char	buf[INIT_BUFFER_SIZE];
    int		buflen = sizeof buf;
    size_t	ret_len;

    status = X400msMsgGetMessageBody(mp, bp_num, x400_mpp);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "X400MsgGetMessageBody (%d) failed %s\n",
		 bp_num, X400msError (status));
	return status;
    }
    printf("Got Message BP from BP %d\n", bp_num);
    x400_mp = *x400_mpp;

    status = X400MsgGetStrParam(x400_mp, X400_S_OBJECTTYPE, buf, buflen, 
	    &ret_len);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "X400MsgGetStrParam (%d) failed %s\n",
		 X400_S_OBJECTTYPE, X400msError (status));
	return status;
    }
    printf("Message BP from BP is '%.*s'\n", (int)ret_len, buf);
    if (strncmp(buf, "message", ret_len) == 0 ) {
	status = do_xmsg_headers(x400_mp);
	if (status != X400_E_NOERROR) 
	    return status;
	status = do_xmsg_content_as_bp(x400_mp);
	if (status != X400_E_NOERROR) 
	    return status;
    }
    else if (strncmp(buf, "report", ret_len) == 0 ) {
	status = do_xrep_headers(x400_mp);
	if (status != X400_E_NOERROR) 
	    return status;
	status = do_xrep_content_as_bp(x400_mp);
	if (status != X400_E_NOERROR) 
	    return status;
    }
    else if (strncmp(buf, "probe", ret_len) == 0 ) {
	status = do_xmsg_headers(x400_mp);
	if (status != X400_E_NOERROR) 
	    return status;
    }
    else {
	fprintf(stderr, "got unknown object: %s\n", buf);
	return X400_E_BADPARAM;
    }


    return X400_E_NOERROR;
}

static int do_xmsg_headers(
    struct X400Message *x400_mp
)
{
    int		status;
    char	buf[INIT_BUFFER_SIZE];
    int		buflen = sizeof buf;
    size_t	ret_len;
    int		num_atts;

    printf("do_xmsg_headers\n");
    status = X400MsgGetStrParam(x400_mp, X400_S_IPM_IDENTIFIER, buf, buflen, 
	    &ret_len);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "X400MsgGetStrParam (%d) failed %s\n",
		 X400_S_OBJECTTYPE, X400msError (status));
	return status;
    }
    printf("IPM Identifier from Message BP '%.*s'\n", (int)ret_len, buf);

    status = X400MsgGetStrParam(x400_mp, X400_S_SUBJECT, buf, buflen, 
	    &ret_len);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "X400MsgGetStrParam (%d) failed %s\n",
		 X400_S_OBJECTTYPE, X400msError (status));
	return status;
    }
    printf("Subject from Message BP '%.*s'\n", (int)ret_len, buf);

    status = X400MsgGetIntParam(x400_mp, X400_N_NUM_ATTACHMENTS, &num_atts);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "X400MsgGetStrParam (%d) failed %s\n",
		 X400_S_OBJECTTYPE, X400msError (status));
	return status;
    }
    printf("Number of attachments in Message BP %d'\n", num_atts);

    status = get_recips(x400_mp, X400_ORIGINATOR);
    if (status != X400_E_NOERROR) {
	return status;
    }

    status = get_recips(x400_mp, X400_RECIP_PRIMARY);
    if (status != X400_E_NOERROR) {
	return status;
    }

    status = get_recips(x400_mp, X400_RECIP_ENVELOPE);
    if (status != X400_E_NOERROR) {
	return status;
    }

    return X400_E_NOERROR;
}


static int get_recips(
    struct X400Message 	*x400_mp,
    int			type
)
{

    struct X400Recipient 	*rp;
    int				status;
    int 			recip_num;

    for (recip_num = 1; ; recip_num++ ) {

	char	buf[INIT_BUFFER_SIZE];
	int		buflen = sizeof buf;
	size_t	ret_len;

	status = X400MsgGetRecip(x400_mp, type, recip_num, &rp);
	if (status == X400_E_NO_RECIP) {
	    if (recip_num == 1) {
		printf("no recips of type %x\n", type);
		return X400_E_NOERROR;
	    } else {
		printf("no more recips of type %x\n", type);
	    }
	    return X400_E_NOERROR;
	}
	else if (status != X400_E_NOERROR) {
	    fprintf(stderr, "X400MsgGetRecip (%d) failed %s\n",
		     X400_S_OBJECTTYPE, X400msError (status));
	    return status;
	}
	printf("got recip number %d of type %s (%x)\n", recip_num,
		type == X400_RECIP_PRIMARY ? "Primary" : 
		type == X400_RECIP_CC ? "CC" : 
		type == X400_RECIP_BCC ? "BCC" : 
		type == X400_ORIGINATOR ? "Originator" : 
		type == X400_AUTH_ORIG ? "Auth Orig" : 
		type == X400_RECIP_REPLY ? "Reply" : 
		type == X400_RECIP_REPORT ? "Reported Recip" : 
		type == X400_RECIP_INTENDED ? "Intended" : 
		type == X400_RECIP_ENVELOPE ? "Envelope" : 
		"Unknown",
		type);
	if (type == X400_RECIP_ENVELOPE) {
	    // These aren't expected as the message will be forwarded
	    // message with no envelope
	    status = X400RecipGetStrParam(rp, X400_S_OR_ADDRESS, buf, buflen, 
		    &ret_len);
	    if (status != X400_E_NOERROR) {
		fprintf(stderr, "X400MsgGetStrParam (%d) failed %s\n",
			 X400_S_OBJECTTYPE, X400msError (status));
	    } else {
		printf("Originator OR Address'%.*s'\n", (int)ret_len, buf);
	    }
	} else {
	    status = X400RecipGetStrParam(rp, X400_S_OR_ADDRESS, buf, buflen, 
		    &ret_len);
	    if (status != X400_E_NOERROR) {
		fprintf(stderr, "X400MsgGetStrParam (%d) failed %s\n",
			 X400_S_OBJECTTYPE, X400msError (status));
	    } else {
		printf("Originator OR Address'%.*s'\n", (int)ret_len, buf);
	    }

	}

#define USE_REDIRECTION_HISTORY 1
#ifdef  USE_REDIRECTION_HISTORY
 {
     status = do_redirection_hist(rp);
     if ( status == X400_E_NO_VALUE ) {
	 /* do nothing */
     } else if (status == X400_E_MISSING_ATTR ) {
	 printf("There is no Redirection History\n");
     } else if (status == X400_E_NOERROR ) {
	 printf("Found some Redirection History\n");
     } else {
	 printf("error getting Redirection History\n");
	 fprintf (stderr, "Error fetching Redirection History: %s\n", 
		  X400msError (status));
	 return status;
     }  
 }
#endif
    }

}
static int do_xmsg_content_as_bp(
    struct X400Message *x400_mp
#ifdef __GNUC__
    __attribute__((unused))
#endif
)
{
    printf("do_xmsg_content_as_bp NYI\n");
    return X400_E_NOERROR;
}


static int do_xrep_headers(
    struct X400Message *x400_mp
#ifdef __GNUC__
    __attribute__((unused))
#endif
)
{
    printf("do_xrep_headers NYI\n");
    return X400_E_NOERROR;
}


static int do_xrep_content_as_bp(
    struct X400Message *x400_mp
#ifdef __GNUC__
    __attribute__((unused))
#endif
)
{
    printf("do_xrep_content_as_bp NYI\n");
    return X400_E_NOERROR;
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
    return;
}

static int do_redirection_hist (
    struct X400Recipient *rp
)
{
    char  redirection_time [BUFSIZ];
    char  or_address       [BUFSIZ];
    char  dn               [BUFSIZ];
    int   rr;
    
    size_t length;
    int n;
    int status;
    
    struct X400RediHist *hist = NULL;
    
    for ( n=1; ;n++ ) {

	status = X400RediHistGet (rp,n,&hist);
	if (status != X400_E_NOERROR) {
	    return status;
	}
	
	status = X400RediHistGetStrParam (hist, X400_S_REDIRECTION_TIME,
					  redirection_time, BUFSIZ,&length);
	
	if (status == X400_E_NO_VALUE) {
	    return X400_E_NO_VALUE;
	}
	
	if ( status == X400_E_NOERROR ) {
	    redirection_time[length] = '\0';
	    printf ("Redirection time: %i %s\n",n,redirection_time);
	} else {
	    fprintf (stderr, "Error in do_redirection_hist: %s\n",
		     X400msError (status));
	}

	status = X400RediHistGetStrParam (hist, X400_S_OR_ADDRESS,
					  or_address, BUFSIZ , &length);
	if ( status == X400_E_NOERROR ) {
	    or_address[length] = '\0';
	    printf ("Redirection OR address %i  :%s\n",n,or_address);
	} else {
	    fprintf (stderr, "Error in do_redirection_hist: %s\n",
		     X400msError (status));
	}

	status = X400RediHistGetStrParam (hist, X400_S_DIRECTORY_NAME,
					  dn, BUFSIZ , &length);
	if ( status == X400_E_NOERROR ) {
	    dn[length] = '\0';
	    printf ("Redirection DN %i  :%s\n",n,dn);
	} else {
	    fprintf (stderr, "Error in do_redirection_hist: %s\n",
		     X400msError (status));
	}
	
	status = X400RediHistGetIntParam (hist, X400_N_REDIRECTION_REASON,&rr);
	if ( status == X400_E_NOERROR ) {
	    printf ("DSI routing action %d  :%s\n",n,X400RediReason(rr));
	} else {
	    fprintf (stderr, "Error in do_redirection_hist: %s\n",
		     X400msError (status));
	}
	
    }
    if (n == 1) {
	printf("No Redirection histories found in message\n");
    } else {
	printf("%d Redirection histories found in message\n", n - 1);
    }
    return X400_E_NOERROR;
    
}
static int do_redirection_hist_env (
    struct X400msMessage *mp
)
{
    char  redirection_time [BUFSIZ];
    char  or_address       [BUFSIZ];
    char  dn               [BUFSIZ];
    int   rr;
    
    size_t length;
    int n;
    int status;
    
    struct X400RediHist *hist = NULL;
    
    for ( n=1; ;n++ ) {

	status = X400msRediHistGetEnv (mp,n,&hist);
	if (status != X400_E_NOERROR) {
	    return status;
	}
	
	status = X400RediHistGetStrParam (hist, X400_S_REDIRECTION_TIME,
					  redirection_time, BUFSIZ,&length);
	
	if (status == X400_E_NO_VALUE) {
	    return X400_E_NO_VALUE;
	}
	
	if ( status == X400_E_NOERROR ) {
	    redirection_time[length] = '\0';
	    printf ("Redirection time: %i %s\n",n,redirection_time);
	} else {
	    fprintf (stderr, "Error in do_redirection_hist_env: %s\n",
		     X400msError (status));
	}

	status = X400RediHistGetStrParam (hist, X400_S_OR_ADDRESS,
					  or_address, BUFSIZ , &length);
	if ( status == X400_E_NOERROR ) {
	    or_address[length] = '\0';
	    printf ("Redirection OR address %i  :%s\n",n,or_address);
	} else {
	    fprintf (stderr, "Error in do_redirection_hist_env: %s\n",
		     X400msError (status));
	}

	status = X400RediHistGetStrParam (hist, X400_S_DIRECTORY_NAME,
					  dn, BUFSIZ , &length);
	if ( status == X400_E_NOERROR ) {
	    dn[length] = '\0';
	    printf ("Redirection DN %i  :%s\n",n,dn);
	} else {
	    fprintf (stderr, "Error in do_redirection_hist_env: %s\n",
		     X400msError (status));
	}
	
	status = X400RediHistGetIntParam (hist, X400_N_REDIRECTION_REASON,&rr);
	if ( status == X400_E_NOERROR ) {
	    printf ("DSI routing action %d  :%s\n",n,X400RediReason(rr));
	} else {
	    fprintf (stderr, "Error in do_redirection_hist_env: %s\n",
		     X400msError (status));
	}
	
    }
    if (n == 1) {
	printf("No Redirection histories found in envelope\n");
    } else {
	printf("%d Redirection histories found in envelope\n", n - 1);
    }
    return X400_E_NOERROR;
    
}


static int do_trace_info (
    struct X400msMessage *mp
)
{
    char  GDI [BUFSIZ];
    char  DSI_arrival_time[BUFSIZ];
    int   DSI_routing_action;
    char  DSI_attempted_domain[BUFSIZ];

    char  DSI_AA_def_time[BUFSIZ];
    char  DSI_AA_CEIT[BUFSIZ];
    int   DSI_AA_redirected   = 0;
    int   DSI_AA_dl_operation = 0;
    
    size_t length;
    int n;
    int status;
    
    struct X400TraceInfo *info = NULL;
    
    for ( n=1; ;n++ ) {
	status = X400msTraceInfoGet (mp, n, &info, X400_TRACE_INFO);
	if (status != X400_E_NOERROR) {
	    return status;
	}
	
	status = X400TraceInfoGetStrParam (info, X400_S_GLOBAL_DOMAIN_ID,
					   GDI, BUFSIZ , &length);
		
	if ( status == X400_E_NOERROR ) {
	    GDI[length] = '\0';
	    printf ("GDI entry %d OR Address:%s\n",n,GDI);
	} else {
	    fprintf (stderr, "Error in do_trace_info: %s\n",
		     X400msError (status));
	}

	status = X400TraceInfoGetStrParam (info, X400_S_DSI_ARRIVAL_TIME,
					   DSI_arrival_time, BUFSIZ , &length);
	if ( status == X400_E_NOERROR ) {
	    DSI_arrival_time[length] = '\0';
	    printf ("DSI arrival time %d  :%s\n",n,DSI_arrival_time);
	} else {
	    fprintf (stderr, "Error in do_trace_info: %s\n",
		     X400msError (status));
	}
	    
	status = X400TraceInfoGetIntParam (info,X400_N_DSI_ROUTING_ACTION,
					   &DSI_routing_action);
	if ( status == X400_E_NOERROR ) {
	    printf ("DSI routing action %d  :%s\n",n,
		    DSI_routing_action ? "rerouted" : "relayed");
	} else {
	    fprintf (stderr, "Error in do_trace_info: %s\n",
		     X400msError (status));
	}

	/* This one is optional*/
	status = X400TraceInfoGetStrParam (info,
					   X400_S_DSI_ATTEMPTED_DOMAIN,
					   DSI_attempted_domain,
					   BUFSIZ ,&length);
	if ( status == X400_E_NOERROR ) {
	    DSI_attempted_domain[length] = '\0';
	    printf ("DSI attempted domain %d  :%s\n",n,DSI_attempted_domain);
	} else {
	    fprintf (stderr, "Error in do_trace_info optional component: %s\n",
		     X400msError (status));
	}

	/*optional*/
	status = X400TraceInfoGetStrParam (info,
					   X400_S_DSI_AA_DEF_TIME,
					   DSI_AA_def_time,
					   BUFSIZ ,&length);
	if ( status == X400_E_NOERROR ) {
	    DSI_AA_def_time[length] = '\0';
	    printf ("DSI AA %d  :%s\n",n,DSI_AA_def_time);
	} else {
	    fprintf (stderr, "Error in do_trace_info optional component: %s\n",
		     X400msError (status));
	}

	/*optional*/
	status = X400TraceInfoGetStrParam (info,
					   X400_S_DSI_AA_CEIT,
					   DSI_AA_CEIT,
					   BUFSIZ ,&length);
	if ( status == X400_E_NOERROR ) {
	    DSI_AA_CEIT[length] = '\0';
	    printf ("DSI AA %d  :%s\n",n,DSI_AA_CEIT);
	} else {
	    fprintf (stderr, "Error in do_trace_info optional component: %s\n",
		     X400msError (status));
	}
	
	
	status = X400TraceInfoGetIntParam (info,
					   X400_N_DSI_AA_REDIRECTED,
					   &DSI_AA_redirected);
	if ( status == X400_E_NOERROR ) {
	    printf ("DSI AA redirected %d  :%d\n",n,DSI_AA_redirected );
	} else {
	    fprintf (stderr, "Error in do_trace_info: %s\n",
		     X400msError (status));
	}
	
	status = X400TraceInfoGetIntParam (info,
					   X400_N_DSI_AA_DLOPERATION,
					   &DSI_AA_dl_operation);
	if ( status == X400_E_NOERROR ) {
	    printf ("DSI AA dl operation %d  :%d\n",n,DSI_AA_dl_operation );
	} else {
	    fprintf (stderr, "Error in do_trace_info: %s\n",
		     X400msError (status));
	}

	
    }
    if (n == 1) {
	printf("No Trace Info found in message\n");
    } else {
	printf("%d Trace Info entries found in message\n", n - 1);
    }
    return X400_E_NOERROR;
    
}


static int do_internal_trace_info (
    struct X400msMessage *mp
)
{
    
    size_t length;
    int n;
    int status;
    int SI_routing_action   = 0;
    int SI_attempted_action = 0;
    int SI_OA               = 0;
    
    char GDI                  [BUFSIZ];
    char SI_MTA_name          [BUFSIZ];
    char SI_time              [BUFSIZ];
    char SI_attempted_MTA_name[BUFSIZ];
    char SI_attempted_domain  [BUFSIZ];
    char SI_defered_time      [BUFSIZ];
    char SI_CEIT              [BUFSIZ];
    
    struct X400InternalTraceInfo *info = NULL;
    
    for ( n=1; ;n++ ) {
	/*GDI*/
	status = X400msInternalTraceInfoGet (mp,n, &info);
	if (status == X400_E_NO_VALUE) {
	    return status;
	}
	status = X400InternalTraceInfoGetStrParam (info,
						   X400_S_GLOBAL_DOMAIN_ID,
						   GDI, BUFSIZ , &length);
	if ( status == X400_E_NOERROR ) {
	    GDI[length] = '\0';
	    printf ("GDI entry %d Domain:%s\n",n,GDI);
	} else {
	    fprintf (stderr, "Error in do_trace_info: %s\n",
		     X400msError (status));
	}
	
	/*MTA Name */
	status = X400InternalTraceInfoGetStrParam (info,
						   X400_S_MTA_NAME,
						   SI_MTA_name, BUFSIZ , &length);
	
	if ( status == X400_E_NOERROR ) {
            SI_MTA_name [length] = '\0';
	    printf ("MTA Name entry %d :%s\n",n,SI_MTA_name);
	} else {
	    fprintf (stderr, "Error in do_internal_trace_info: %s\n",
		     X400msError (status));
	}

	/*SI MTA Name*/
	
	status = X400InternalTraceInfoGetStrParam (info,
						   X400_S_MTA_SI_TIME,
						   SI_time, BUFSIZ , &length);
	
	if ( status == X400_E_NOERROR ) {
	    SI_time[length] = '\0';
	    printf ("SI MTA TIME entry %d :%s\n",n,SI_time);
	} else {
	    fprintf (stderr, "Error in do_internal_trace_info: %s\n",
		     X400msError (status));
	}	
	
	/* SI routing action */
	status = X400InternalTraceInfoGetIntParam (info,
						   X400_N_MTA_SI_ROUTING_ACTION,
						   &SI_routing_action);
	if ( status == X400_E_NOERROR ) {
	    if (SI_routing_action == X400_MTA_SI_ROUTING_ACTION_RELAYED) {
		printf ("SI routing action %d  :relayed\n",n);
	    } else {
		printf ("SI routing action %d  :rerouted\n",n);
	    }
	    
	} else {
	    fprintf (stderr, "Error in do_internal_trace_info: %s\n",
		     X400msError (status));
	}

	status = X400InternalTraceInfoGetIntParam (info,
						   X400_N_MTA_SI_ATTEMPTED_ACTION,
						   &SI_attempted_action);
	if ( status == X400_E_NOERROR ) {
	    if (SI_routing_action == X400_MTA_SI_RA_MTA) {
		printf ("SI attempted action %d  :MTA\n",n);
	    } else {
		printf ("SI attempted action %d  :Domain\n",n);
	    }
	    
	} else {
	    fprintf (stderr, "Error in do_internal_trace_info: %s\n",
		     X400msError (status));
	}

	if (SI_attempted_action == X400_MTA_SI_RA_MTA) {
	    /* SI Attempted MTA Name */
	    status = X400InternalTraceInfoGetStrParam (info,
						       X400_S_MTA_SI_ATTEMPTED_MTA,
						       SI_attempted_MTA_name,
						       BUFSIZ,&length);
	
	    if ( status == X400_E_NOERROR ) {
		SI_attempted_MTA_name[length] = '\0';
		printf ("Supplied information attempted MTA %d"
			":%s\n",n,SI_attempted_MTA_name);
	    } else {
		fprintf (stderr, "Error in do_trace_info: %s\n",
			 X400msError (status));
	    }

	} else {
	
	    /*SI Attempted domain*/
	    status = X400InternalTraceInfoGetStrParam (info,
						       X400_S_MTA_SI_ATTEMPTED_DOMAIN,
						       SI_attempted_domain, BUFSIZ,
						       &length);
	
	    if ( status == X400_E_NOERROR ) {
		SI_attempted_domain[length] = '\0';
		printf ("Supplied information attempted Domain entry"
			" %d :%s\n",n,SI_attempted_domain);
	    } else {
		fprintf (stderr, "Error in do_internal_trace_info: %s\n",
			 X400msError (status));
	    }
	}

	

	/* SI MTA Defered time*/
	status = X400InternalTraceInfoGetStrParam (info,
						   X400_S_MTA_SI_DEFERRED_TIME,
						   SI_defered_time,
						   BUFSIZ , &length);
	
	if ( status == X400_E_NOERROR ) {
	    SI_defered_time[length] = '\0';
	    printf ("SI_defered_time entry %d :%s\n",n,
		    SI_defered_time);
	} else {
	    fprintf (stderr, "Error in do_internaltrace_info: %s\n",
		     X400msError (status));
	}
	
	/* SI MTA Converted Encoded Information Types*/
	status = X400InternalTraceInfoGetStrParam (info,
						   X400_S_MTA_SI_CEIT,
						   SI_CEIT,
						   BUFSIZ , &length);
	
	if ( status == X400_E_NOERROR ) {
	    SI_CEIT[length] = '\0';
	    printf ("SI_CEIT entry %d :%s\n",n,
		    SI_CEIT);
	} else {
	    fprintf (stderr, "Error in do_internaltrace_info: %s\n",
		     X400msError (status));
	}


	status = X400InternalTraceInfoGetIntParam (info,
						   X400_N_MTA_SI_OTHER_ACTIONS,
						   &SI_OA);
	if ( status == X400_E_NOERROR ) {
	    if (SI_OA & X400_MTA_SI_OTHER_ACTION_REDIRECTED) {
		printf("SI_Other_Actions: %d: Redirected\n",n);
	    }
	    if (SI_OA & X400_MTA_SI_OTHER_ACTION_DLOPERATION) {
		printf("SI_Other_Actions: %d: DLOP\n",n);
	    }
	    
	} else {
	    fprintf (stderr, "Error in do_internal_trace_info: %s\n",
		     X400msError (status));
	}

		
    }
    if (n == 1) {
	printf("No Internal Trace Info found in message\n");
    } else {
	printf("%d Internal Trace Info entries found in message\n", n - 1);
    }
    return X400_E_NOERROR;
    
}

static int do_orig_and_dl (
    struct X400msMessage *mp
)
{
    char  exp_time   [BUFSIZ];
    char  or_address [BUFSIZ];
    char  dn_address [BUFSIZ];
    
    size_t length;
    int n;
    int status;
    
    struct X400ORandDL *orig_and_dl = NULL;
    
    for ( n=1; ;n++ ) {

	status = X400msORandDLGet (mp,n,&orig_and_dl);
        if (status == X400_E_NO_VALUE) {
            printf("Finished getting originator and dl expansion history\n");
            return status;
        } else if (status != X400_E_NOERROR) {
	    return status;
	}
	
	status = X400ORandDLGetStrParam (orig_and_dl,
                                         X400_S_ORIG_OR_EXAP_TIME,
                                         exp_time, BUFSIZ,&length);
	
	if ( status == X400_E_NOERROR ) {
	    exp_time[length] = '\0';
	    printf ("do_orig_and_dl: %i %s\n",n,exp_time);
        } else if(status != X400_E_MISSING_ATTR ) {
	    fprintf (stderr, "Error in do_orig_and_dl: %s\n",
		     X400msError (status));
	}

	status =  X400ORandDLGetStrParam(orig_and_dl, X400_S_OR_ADDRESS,
                                         or_address, BUFSIZ , &length);
	if ( status == X400_E_NOERROR ) {
	    or_address[length] = '\0';
	    printf ("do_orig_and_dl OR address %i  :%s\n",n,or_address);
	} else if(status != X400_E_MISSING_ATTR ) {
	    fprintf (stderr, "Error in do_orig_and_dl: %s\n",
		     X400msError (status));
	}

	status =  X400ORandDLGetStrParam(orig_and_dl,
                                         X400_S_DIRECTORY_NAME,
                                         dn_address, BUFSIZ , &length);
	if ( status == X400_E_NOERROR ) {
	    dn_address[length] = '\0';
	    printf ("originator and DL expansion DN %i  :%s\n",n,dn_address);
	} else if(status != X400_E_MISSING_ATTR ) {
	    fprintf (stderr, "Error in do_orig_and_dl: %s\n",
		     X400msError (status));
	}
	
	
    }
    if (n == 1) {
	printf("No orig and dl hist found in message\n");
    } else {
	printf("%d orig and dl hist found in message\n", n - 1);
    }
    return X400_E_NOERROR;  
}

