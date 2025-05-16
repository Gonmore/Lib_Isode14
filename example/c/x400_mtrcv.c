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
 * Simple example program for transferring a message out of the MTA
 */

#include <stdio.h>
#include <stdlib.h>
#include <x400_mtapi.h>
#include <seclabel_api.h> /* For security labels */
#include "example.h"
/* to get NDR values */
/*  #include "esc.h" */

static int get_msg (
    int         argc,
    char      **argv
) ;
static int do_msg_env(
    struct X400mtMessage *mp
);
static int do_msg_headers(
    struct X400mtMessage *mp
);
static void do_msg_content(
    struct X400mtMessage *mp
);
static int get_a_msg (
    struct X400mtSession *sp
) ;
static int get_recips(
    struct 	X400mtMessage *mp,
    int		recip_type,
    char	*recip_str
);

static int get_exempted_recips (
    struct 	X400mtMessage *mp
);

static int do_rep_env(
    struct X400mtMessage *mp
);

static int do_rep_content(
    struct X400mtMessage *mp
);

static int do_rep_retcontent(
    struct X400mtMessage *mp
);

static void usage(void);

#ifdef WANT_DL_EXP_HIST  
static int do_dl_hist (
    struct X400mtMessage *mp
);
#endif

static int do_trace_info (
    struct X400mtMessage *mp
);

static int do_internal_trace_info (
    struct X400mtMessage *mp
);

static int do_redirection_hist (
    struct X400Recipient *recip
);

static int do_redirection_hist_env (
    struct X400mtMessage *msg
);

static int get_ALI (
    struct X400mtMessage *mp
);

static int get_hi (
    struct X400mtMessage *mp
);

static int get_mi (
    struct X400mtMessage *mp
);

static int get_dist_codes_sic(
    struct X400mtMessage *mp
);

static int get_other_recips(
    struct X400mtMessage *mp
);

static int get_acp127_ali (
    struct X400ACP127Resp *resp
);

static char *optstr = "uo:O:r:c:l:EQ:";

static int get_body_parts(
    struct X400mtMessage *mp
);

static int do_orig_and_dl (
    struct X400mtMessage *mp
);


int num_atts = 0;

/*! Main function of mhsrcv 
 * \return 0 on success, otherwise the value of status from X400 call
 */
int main (
    int         argc,
    char      **argv
) 
{
    int retval;

    if (x400_channel == NULL) {
	fprintf (stderr, "No x400_channel value set in x400tailor file");
	exit(1);
    }

    retval = get_msg(argc, argv);

    /*
    while (retval == X400_E_NOERROR || retval == X400_E_NO_MESSAGE) {
	retval = get_msg(argc, argv);
    }
    */

    retval = get_msg(argc, argv);
    fprintf (stderr, "Error in X400mtMsgGet: %s\n", X400mtError (retval));

    return(retval);
}
static int get_msg (
    int         argc,
    char      **argv
) 
{
    int status;
    struct X400mtSession *sp;
    char *filepath = "/var/isode";

    if (get_args(argc, argv, optstr)) {
	usage();
	exit(-1);
    }

    /* open our X400 session */
    status = X400mtOpen (x400_channel, &sp);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "Error in Open: %s\n", X400mtError (status));
	return (status);
    }

    X400mtSetStrDefault(sp, X400_S_CONTENT_FILE_PATH, 
			filepath, strlen(filepath));

    X400mtSetStrDefault(sp, X400_ADATP3_SEP,"\r\n",-1);

    if (attr_to_fault != 0) {
	status = X400mtMarkToFault(sp, attr_to_fault, 1);
	if (status != X400_E_NOERROR) {
	    fprintf (stderr, "Error in X400mtMarkToFault: %s\n", X400mtError (status));
	    X400mtClose (sp);
	    return (status);
	}
    }
    
    get_a_msg(sp);
    if ( status != X400_E_NOERROR ) {
	/* close API session */
	status = X400mtClose (sp);
	if ( status != X400_E_NOERROR ) {
	    printf("X400mtClose returned error %d\n", status);
	    fprintf (stderr, "Error in X400mtClose: %s\n", 
		X400mtError (status));
	}
	return status;
    }

    do {
	get_a_msg(sp);
	if ( status != X400_E_NOERROR ) {
	    /* close API session */
	    status = X400mtClose (sp);
	    if ( status != X400_E_NOERROR ) {
		printf("X400mtClose returned error %d\n", status);
		fprintf (stderr, "Error in X400mtClose: %s\n", 
		    X400mtError (status));
	    }
	    return status;
	}
    }
    while (until_no_more_msgs);

    /* close API session */
    status = X400mtClose (sp);
    if ( status != X400_E_NOERROR ) {
	printf("X400mtClose returned error %d\n", status);
	fprintf (stderr, "Error in X400mtClose: %s\n", 
	    X400mtError (status));
    }
    return (status);
}


static int get_a_msg (
    struct X400mtSession *sp
) 
{
    struct X400mtMessage *mp;
    int type;
    int status;
    static int reported_none = 0;
    
    /* get the message */
    status = X400mtMsgGetStart (sp, &mp, &type);
    if ( status != X400_E_NOERROR ) {
	if ( status == X400_E_NO_MESSAGE ) {
	    if (reported_none == 0) {
		printf("No messages ...\n");
		reported_none = 1;
	    }
	    return (X400_E_NO_MESSAGE);
	}
	printf("Error getting message (%d) %s\n", status, X400mtError(status));
	fprintf (stderr, "Error in X400mtMsgGet: %s\n", X400mtError (status));
	return (status);
    }

    /* process the message */
    switch ( type) {
	case X400_MSG_MESSAGE:
	    printf("Message waiting\n\n");
	    /* display message attributes */
	    do_msg_env(mp);
	    do_msg_headers(mp);
	    do_msg_content(mp);
	    break;

	case X400_MSG_REPORT:
	    printf("Report waiting\n\n");
	    /* display message attributes */
	    do_rep_env(mp);
	    do_rep_content(mp);
	    do_rep_retcontent(mp);
	    break;

	case X400_MSG_PROBE:
	    /* Not handling a Probe here */
	    printf ("Got a probe, ignoring ...\n");
	    /* Delete internal copy */
	    status = X400mtMsgDelete (mp);
	    if ( status != X400_E_NOERROR ) {
		printf("X400mtMsgDelete returned error %d\n", status);
		fprintf (stderr, "Error in X400mtMsgDelete: %s\n", 
		    X400mtError (status));
	    }
	    return (status);
    }

#ifdef always_dr_msg
#define  DRD_UNRECOGNISED_OR                    0
#define  DRR_UNABLE_TO_TRANSFER                 1

    /* finish transfer - NDR */
    status = X400mtMsgGetFinish (mp, X400_E_ADDRESS_ERROR, 
	    DRR_UNABLE_TO_TRANSFER, DRD_UNRECOGNISED_OR, "Unable to transfer");
#endif
    /* finish transfer - accept: no DR */
    status = X400mtMsgGetFinish (mp, X400_E_NOERROR, 
	    -1, -1, "");
    if ( status != X400_E_NOERROR ) {
	printf("X400mtMsgFinish returned error %d\n", status);
	fprintf (stderr, "Error in X400mtMsgFinish: %s\n", 
	    X400mtError (status));
    }
    printf("X400mtMsgFinish returned success %d\n", status);

    /* Delete internal copy */
    status = X400mtMsgDelete (mp);
    if ( status != X400_E_NOERROR ) {
	printf("X400mtMsgDelete returned error %d\n", status);
	fprintf (stderr, "Error in X400mtMsgDelete: %s\n", 
	    X400mtError (status));
    }
    return status;
}


/*
 * Message functions
 */
static int do_msg_env(
    struct X400mtMessage *mp
)
{
    char 			buffer[BUFSIZ];
    int 			status;
    size_t 			length;
    int				int_param = 0;

    /* Envelope Attributes */
    
    /* string attributes */
    /* The ORADDRESS in the message is the (envelope) originator */
    status = X400mtMsgGetStrParam (mp, X400_S_OR_ADDRESS,
				   buffer, sizeof buffer , &length);
    if ( status != X400_E_NOERROR ) {
	printf("error getting originator address\n");
	fprintf (stderr, 
		"Error in X400mtMsgGetStrParam: %s\n", X400mtError (status));
	return (status);
    }
    printf ("Originator: %.*s\n", (int)length, buffer);

    /* Envelope message IDentifier */
    status = X400mtMsgGetStrParam (mp, X400_S_MESSAGE_IDENTIFIER,
				   buffer, sizeof buffer , &length);
    if ( status != X400_E_NOERROR ) {
	printf("error getting message identifier\n");
	fprintf (stderr, "Error in X400mtMsgGetStrParam: %s\n", X400mtError (status));
	return (status);
    }
    printf ("MessageID: %.*s\n", (int)length, buffer);

    /* Content Identifier */
    status = X400mtMsgGetStrParam (mp, X400_S_CONTENT_IDENTIFIER,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("Content Identifier: %.*s\n", (int)length, buffer);
    
    /* Content Correlator */
    status = X400mtMsgGetStrParam (mp, X400_S_CONTENT_CORRELATOR_IA5_STRING,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("Content Correlator IA5Text: %.*s\n", (int)length, buffer);

    status = X400mtMsgGetStrParam (mp, X400_S_CONTENT_CORRELATOR_OCTET_STRING,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("Content Correlator Octet String: %i bytes\n", (int)length);
    
    /* Original EITs */
    status = X400mtMsgGetStrParam (mp, 
	   X400_S_ORIGINAL_ENCODED_INFORMATION_TYPES,
	   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("Original EITs: %.*s\n", (int)length, buffer);

    /* Message Submission Time */
    status = X400mtMsgGetStrParam (mp, X400_S_MESSAGE_SUBMISSION_TIME,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("Message Submission Time: %.*s\n", (int)length, buffer);

    /* Message Delivery Time */
    status = X400mtMsgGetStrParam (mp, X400_S_MESSAGE_DELIVERY_TIME,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("Message Delivery Time: %.*s\n", (int)length, buffer);

    /* Latest Delivery Time */
    status = X400mtMsgGetStrParam (mp, X400_S_LATEST_DELIVERY_TIME,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("Latest Delivery Time: %.*s\n", (int)length, buffer);

    /* Originator Return Address */
    status = X400mtMsgGetStrParam (mp, X400_S_ORIGINATOR_RETURN_ADDRESS,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("Originator Return Address : %.*s\n", (int)length, buffer);

    status = X400mtMsgGetStrParam (mp, X400_S_EXTERNAL_CONTENT_TYPE, 
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("External Content Type : %.*s\n", (int)length, buffer);

    /* Integer parameters */
    /* Content Type */
    status = X400mtMsgGetIntParam (mp, X400_N_CONTENT_TYPE, &int_param);
    if ( status == X400_E_NOERROR )
	printf ("Content type : P%d\n", int_param);

				   
    if ( status == X400_E_NOERROR )
	printf ("Content type : P%d\n", int_param);

    /* Content Length */
    status = X400mtMsgGetIntParam (mp, X400_N_CONTENT_LENGTH, &int_param);
    if ( status == X400_E_NOERROR )
	printf ("Content length: %d\n", int_param);

    /* Priority */
    status = X400mtMsgGetIntParam (mp, X400_N_PRIORITY, &int_param);
    if ( status == X400_E_NOERROR )
	printf ("Priority: %d\n", int_param);

    /* Disclosure of recips prohibited */
    status = X400mtMsgGetIntParam (mp, X400_N_DISCLOSURE, &int_param);
    if ( status == X400_E_NOERROR )
	printf ("Disclosure of recips prohibited %s\n", int_param == 0 ? "No": "Yes");

    /* Implicit conversion prohibited */
    status = X400mtMsgGetIntParam (mp, X400_N_IMPLICIT_CONVERSION_PROHIBITED, &int_param);
    if ( status == X400_E_NOERROR )
	printf ("Implicit conversion prohibited %s\n", int_param == 0 ? "No": "Yes");

    /*  Alternate recipient allowed */
    status = X400mtMsgGetIntParam (mp, X400_N_ALTERNATE_RECIPIENT_ALLOWED, &int_param);
    if ( status == X400_E_NOERROR )
	printf (" Alternate recipient allowed: %s\n", int_param == 0 ? "No": "Yes");

    /*  Content return request */
    status = X400mtMsgGetIntParam (mp, X400_N_CONTENT_RETURN_REQUEST, &int_param);
    if ( status == X400_E_NOERROR )
	printf (" Content return request: %s\n", int_param == 0 ? "No": "Yes");

    /* Recipient reassignment prohibited */
    status = X400mtMsgGetIntParam (mp, X400_N_RECIPIENT_REASSIGNMENT_PROHIBITED, &int_param);
    if ( status == X400_E_NOERROR )
	printf ("Recipient reassignment prohibited: %s\n", int_param == 0 ? "No": "Yes");

    /* Distribution List expansion prohibited */
    status = X400mtMsgGetIntParam (mp, X400_N_DL_EXPANSION_PROHIBITED, &int_param);
    if ( status == X400_E_NOERROR )
	printf ("Distribution List expansion prohibited: %s\n", int_param == 0 ? "No": "Yes");

    /* Conversion with loss prohibited */
    status = X400mtMsgGetIntParam (mp, X400_N_CONVERSION_WITH_LOSS_PROHIBITED, &int_param);
    if ( status == X400_E_NOERROR )
	printf ("Conversion with loss prohibited: %s\n", int_param == 0 ? "No": "Yes");

    /* Get the envelope recipients */
    status = get_recips(mp, X400_RECIP_ENVELOPE, "envelope");
    if ( status != X400_E_NOERROR && status != X400_E_NO_RECIP) {
	printf("error getting recipients\n");
	fprintf (stderr, "Error in getting recipient: %s\n", 
	    X400mtError (status));
	return (status);
    }
   
    /* Get X.411 exempted recipients */
    status = get_recips(mp, X400_DL_EXEMPTED_RECIP, "DL Exempted recips");
    if ( status != X400_E_NOERROR && status != X400_E_NO_RECIP) {
	printf("error getting recipients\n");
	fprintf (stderr, "Error in getting recipient: %s\n", 
	    X400mtError (status));
	return (status);
    }
    
    
#ifdef WANT_DL_EXP_HIST  
    /* DL Expansion history */
    {
	
	/*Process the DL Expansion History object */
	    status = do_dl_hist(mp);
	
	if ( status != X400_E_NO_VALUE ) {
	    /* There has been an error fetching the DL expansion history*/
	     printf("error getting DL Expansion History\n");
	     fprintf (stderr, "Error in X400mtDLExpHistGet: %s\n", 
		      X400mtError (status));
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
     
     status = X400mtMsgGetStrParam(mp,
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
		  X400mtError (status));
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
	 printf("error getting Trace Info\n");
	 fprintf (stderr, "Error fetching trace info: %s\n", 
		  X400mtError (status));
	 return status;
     }
	 
 }  
#endif
 

 /* NB: Originator cert is just being treated as a binary blob */
    status = X400mtMsgGetStrParam (mp, X400_S_ORIG_CERT,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("Originator Cert got %i bytes\n", (int)length);


    /* NB: Originator cert is just being treated as a binary blob */
    status = X400mtMsgGetStrParam (mp, X400_S_MOAC,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("MOAC got %i bytes\n", (int)length);
    
    return X400_E_NOERROR;
}

#ifdef WANT_DL_EXP_HIST  
static int do_dl_hist (
    struct X400mtMessage *mp
)
{
    struct X400DLExpHist *hist = NULL;
    char    DLORAddress[BUFSIZ];
    char    DLORDN[BUFSIZ];
    char    DLExpTime[BUFSIZ];
    size_t  length;
    int n;
    int status;
    

    /* Loop through the entries, pulling out the OR Address, DN, and expansion
     * time for each entry.
     * Stop looping when there is no n entry
     */
    for ( n=1; ;n++ ) {

	status = X400mtDLExpHistGet (mp,n,&hist);
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
	    fprintf (stderr, "Error in :X400DLGetStrParam OR %s\n",
		     X400mtError (status));
	}

	status = X400DLGetStrParam (hist,X400_S_DIRECTORY_NAME,
				    DLORDN, BUFSIZ , &length);
	if ( status == X400_E_NOERROR ) {
	    DLORDN[length] = '\0';
	    printf ("DLExpansion List entry %d DN :%s\n",n,DLORDN);
	} else {
	    fprintf (stderr, "Error in :X400DLGetStrParam DN %s\n",
		     X400mtError (status));
	}
	    
	status = X400DLGetStrParam (hist,X400_S_DLEXP_TIME,
				    DLExpTime, BUFSIZ , &length);
	if ( status == X400_E_NOERROR ) {
	    DLExpTime[length] = '\0';
	    printf ("DLExpansion List entry %d Time :%s\n",n,DLExpTime);
	} else {
	    fprintf (stderr, "Error in :X400DLGetStrParam Time %s\n",
		     X400mtError (status));
	}
	    
    }
    return X400_E_NOERROR;
}
#endif

static int do_msg_headers(
    struct X400mtMessage *mp
)
{
    char 			buffer[BUFSIZ];
    int 			status;
    size_t 			length;
    int				int_param;

    /* Message Headers */
    printf("\nContent:\n");

    /* Get the primary recipients */
    status = get_recips(mp, X400_RECIP_PRIMARY, "primary");
    if ( status != X400_E_NOERROR && status != X400_E_NO_RECIP) {
	printf("error getting recipients\n");
	fprintf (stderr, "Error in getting recipient: %s\n", 
	    X400mtError (status));
	return (status);
    }

    /* Get the cc recipients */
    status = get_recips(mp, X400_RECIP_CC, "cc");
    if ( status != X400_E_NOERROR && status != X400_E_NO_RECIP) {
	printf("error getting recipients\n");
	fprintf (stderr, "Error in getting recipient: %s\n", 
	    X400mtError (status));
	return (status);
    }

    /* Get the bcc recipients */
    status = get_recips(mp, X400_RECIP_BCC, "bcc");
    if ( status != X400_E_NOERROR && status != X400_E_NO_RECIP) {
	printf("error getting recipients\n");
	fprintf (stderr, "Error in getting recipient: %s\n", 
	    X400mtError (status));
	return (status);
    }

    /* Get the reply recipients */
    status = get_recips(mp, X400_AUTH_ORIG, "auth-orig");
    if ( status != X400_E_NOERROR && status != X400_E_NO_RECIP) {
	printf("error getting recipients\n");
	fprintf (stderr, "Error in getting recipient: %s\n", 
	    X400mtError (status));
	return (status);
    }

    /* Get the reply recipients */
    status = get_recips(mp, X400_RECIP_REPLY, "reply");
    if ( status != X400_E_NOERROR && status != X400_E_NO_RECIP) {
	printf("error getting recipients\n");
	fprintf (stderr, "Error in getting recipient: %s\n", 
	    X400mtError (status));
	return (status);
    }

    /* Get the Exempted recipients */
    status = get_exempted_recips(mp);
    if ( status != X400_E_NOERROR && status != X400_E_NO_RECIP) {
	fprintf (stderr, "Error in getting exempted recipient: %s\n", 
	    X400mtError (status));
	return (status);
    }
    
    
    /* string parameters */
    /* IPM IDentifier */
    status = X400mtMsgGetStrParam (mp, X400_S_IPM_IDENTIFIER,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("IPM IDentifier: %.*s\n", (int)length, buffer);

    /* Subject */
    status = X400mtMsgGetStrParam (mp, X400_S_SUBJECT,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("Subject: %.*s\n", (int)length, buffer);

    /* Replied-to-identifier */
    status = X400mtMsgGetStrParam (mp, X400_S_REPLIED_TO_IDENTIFIER,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("Replied-to-identifier: %.*s\n", (int)length, buffer);

    /* Obsoleted IPMs */
    status = X400mtMsgGetStrParam (mp, X400_S_OBSOLETED_IPMS,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("Obsoleted IPMs: %.*s\n", (int)length, buffer);

    /* Related IPMs */
    status = X400mtMsgGetStrParam (mp, X400_S_RELATED_IPMS,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("Related IPMs: %.*s\n", (int)length, buffer);

    /* Expiry Time */
    status = X400mtMsgGetStrParam (mp, X400_S_EXPIRY_TIME,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("Expiry Time: %.*s\n", (int)length, buffer);

    /* Reply Time */
    status = X400mtMsgGetStrParam (mp, X400_S_REPLY_TIME,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("Reply Time: %.*s\n", (int)length, buffer);

    /* Authorisation Time */
    status = X400mtMsgGetStrParam (mp, X400_S_AUTHORIZATION_TIME,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("Authorisation Time: %.*s\n", (int)length, buffer);

    /* Originator's Reference */
    status = X400mtMsgGetStrParam (mp, X400_S_ORIGINATORS_REFERENCE,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("Originator's Reference: %.*s\n", (int)length, buffer);

    /* integer arguments */
    /* Importance */
    status = X400mtMsgGetIntParam (mp, X400_N_IMPORTANCE, &int_param);
    if ( status == X400_E_NOERROR )
	printf ("Importance: %d\n", int_param);

    /* Sensitivity: 1 - personal, 2 - private, 3 - company-confidential */
    status = X400mtMsgGetIntParam (mp, X400_N_SENSITIVITY, &int_param);
    if ( status == X400_E_NOERROR )
	printf ("Sensitivity: %d\n", int_param);

    /* Autoforwarded */
    status = X400mtMsgGetIntParam (mp, X400_N_AUTOFORWARDED, &int_param);
    if ( status == X400_E_NOERROR )
	printf ("Autoforwarded: %s\n", int_param ? "false" : "true");

    /*  number of attachments apart from main body part */
    status = X400mtMsgGetIntParam (mp, X400_N_NUM_ATTACHMENTS, &num_atts);
    if ( status == X400_E_NOERROR )
	printf ("number of attachments: %d\n", num_atts);

    /* Fetch P772 Extended Auth Info */
    status = X400mtMsgGetStrParam (mp, X400_S_EXT_AUTH_INFO,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("P772 Ext Auth Info Time: %.*s\n", (int)length, buffer);

    /* Fetch P772 Codress value */

    status = X400mtMsgGetIntParam (mp, X400_N_EXT_CODRESS, &int_param);
    if ( status == X400_E_NOERROR )
	printf ("P772 Codress value: %d\n", int_param);

    /* Fetch P772 Message type value */
    status = X400mtMsgGetIntParam (mp, X400_N_EXT_MSG_TYPE, &int_param);
    if ( status == X400_E_NOERROR )
	printf ("P772 Message Type:  %.*s\n", (int)length, buffer);

    /* Fetch P772 Message id value */
    status = X400mtMsgGetStrParam (mp, X400_S_EXT_MSG_IDENTIFIER,
				   buffer, sizeof buffer, &length);
    if ( status == X400_E_NOERROR )
	printf ("P772 Message Type/Identifier: %d\n", int_param);

    /* Fetch P772 Primary Precedence */
    status = X400mtMsgGetIntParam (mp, X400_N_EXT_PRIM_PREC, &int_param);
    if ( status == X400_E_NOERROR )
	printf ("P772 Primary Precedence: %d\n", int_param);

    /* Fetch P772 Copy Precedence */
    status = X400mtMsgGetIntParam (mp, X400_N_EXT_COPY_PREC, &int_param);
    if ( status == X400_E_NOERROR )
	printf ("P772 Copy Precedence: %d\n", int_param);

    /* Fetch P772 Address List Indicators*/
    get_ALI(mp);

    /* Fetch handling instructions */
    get_hi(mp);

    /* Fetch message instructions */
    get_mi(mp);

    /* Fetch distribution codes sic */
    get_dist_codes_sic(mp);

    /* Fetch Other Recipients */
    get_other_recips(mp);

    
    status = X400mtMsgGetStrParam (mp, X400_S_ORIG_REF,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("P772 Originator Reference: %.*s\n", (int)length, buffer);
    
    status = X400mtMsgGetStrParam (mp, X400_S_ORIG_PLAD,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("P772 Originator PLAD: %.*s\n", (int)length, buffer);
    
    status = X400mtMsgGetStrParam (mp, X400_S_ACP127_MSG_ID,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("P772 ACP127 Msg ID: %.*s\n", (int)length, buffer);

    /* NB: Pilot forwarding info is just being treated as a binary blob */
    status = X400mtMsgGetStrParam (mp, X400_S_PILOT_FWD_INFO,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("P772 Pilot Forwarding Info got %i bytes\n", (int)length);
    
   
    /* NB: information security label is just being treated as a binary blob */
    status = X400mtMsgGetStrParam (mp, X400_S_INFO_SEC_LABEL,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("P772 Information Security Label got %i bytes\n", (int)length);
    
    return X400_E_NOERROR;
}

static void do_msg_content(
    struct X400mtMessage *mp
)
{
    char 			buffer[30000];
    int 			status;
    size_t 			length;

    memset(buffer,0,30000);
    
    status = X400mtMsgGetStrParam (mp, X400_S_CONTENT_FILENAME,
				   buffer, sizeof buffer , &length);

    printf("Read raw content into file %s\n", buffer);

    /* Message Body */

    /* And now get message text (or it could be another type - ie not IA5) */
    status = X400mtMsgGetStrParam (mp, X400_T_IA5TEXT,
				   buffer, sizeof buffer , &length);
   
    if ( status == X400_E_NOERROR ) {
	printf ("Text:\n%.*s\n", (int)length, buffer);
        num_atts--; /* The IA5text bodypart is infact the first "attachment"
                     * So there is one less to fetch */
    }
     /* Go through message bodyparts */
     status = get_body_parts(mp);
     
    return;
}

static int get_exempted_recips (
    struct 	X400mtMessage *mp
)
{
    struct X400Recipient 	*rp;
    int n;
    char 			buffer[BUFSIZ];
    size_t 			length;
    int 			status;
    
    for ( n = 1; ; n++ ) {
	status = X400mtRecipGet (mp,X400_EXEMPTED_ADDRESS , n, &rp);
	if ( status == X400_E_NO_RECIP ) 
	    break;
	if ( status != X400_E_NOERROR ) {
	    fprintf (stderr, "Error in X400mtRecipGet: %s\n", 
		X400mtError (status));
	    return (status);
	}
	status = X400mtRecipGetStrParam (rp, X400_S_IOB_OR_ADDRESS,
					 buffer, sizeof buffer , &length);
	if ( status == X400_E_NOERROR ) {
	    printf ("Exempted recipient OR address %d: %.*s\n",
		    n, (int)length, buffer);
	}

	status = X400mtRecipGetStrParam (rp, X400_S_IOB_DN_ADDRESS,
					 buffer, sizeof buffer , &length);
	if ( status == X400_E_NOERROR ) {
	    printf ("Exempted recipient DN address %d: %.*s\n",
		    n, (int)length, buffer);
	}

	status = X400mtRecipGetStrParam (rp, X400_S_IOB_FREE_FORM_NAME,
					 buffer, sizeof buffer , &length);
	if ( status == X400_E_NOERROR ) {
	    printf ("Exempted recipient free form name %d: %.*s\n",
		    n, (int)length, buffer);
	}
	
	status = X400mtRecipGetStrParam (rp, X400_S_IOB_TEL,
					 buffer, sizeof buffer , &length);
	if ( status == X400_E_NOERROR ) {
	    printf ("Exempted recipient Tel number %d: %.*s\n",
		    n, (int)length, buffer);
	}
	
	
    }
    return X400_E_NOERROR;
}


static int get_ALI (
    struct 	X400mtMessage *mp
)
{
    struct X400ALI *ali  = NULL;
    int n;
    char 			buffer[BUFSIZ];
    size_t 			length;
    int 			status;
    int                         int_param;
  
    for ( n = 1; ; n++ ) {
	status = X400mtALIGet (mp,n,&ali);
	if ( status != X400_E_NOERROR ) 
	    return status;
	
	status = X400ALIGetStrParam (ali,X400_S_IOB_OR_ADDRESS,
				     buffer, sizeof buffer , &length);
	if ( status == X400_E_NOERROR ) {
	    printf ("P772 ALI recipient OR address %d: %.*s\n",
		    n, (int)length, buffer);
	} else if ( status == X400_E_NO_VALUE) {
	    return X400_E_NO_VALUE;
	}
	    

	status = X400ALIGetStrParam (ali,X400_S_IOB_DN_ADDRESS,
				     buffer, sizeof buffer , &length);
	if ( status == X400_E_NOERROR ) {
	    printf ("P772 ALI recipient DN address %d: %.*s\n",
		    n, (int)length, buffer);
	}

	status = X400ALIGetStrParam (ali,X400_S_IOB_FREE_FORM_NAME,
				     buffer, sizeof buffer , &length);
	if ( status == X400_E_NOERROR ) {
	    printf ("P772 ALI recipient free form name %d: %.*s\n",
		    n, (int)length, buffer);
	}
	
	status = X400ALIGetStrParam (ali, X400_S_IOB_TEL,
				     buffer, sizeof buffer , &length);
	if ( status == X400_E_NOERROR ) {
	    printf ("P772 ALI recipient Tel number %d: %.*s\n",
		    n, (int)length, buffer);
	}

	status = X400ALIGetIntParam (ali,X400_N_ALI_TYPE , &int_param);
	if ( status == X400_E_NOERROR ) {
	    printf ("P772 ALI Type  %d\n", int_param);
	}

	status = X400ALIGetIntParam (ali,X400_N_ALI_NOTIFICTAION_REQUEST ,
				     &int_param);
	if ( status == X400_E_NOERROR ) {
	    printf ("P772 ALI Notification Request  %d\n", int_param);
	}
	

	status = X400ALIGetIntParam (ali,X400_N_ALI_REPLY_REQUEST ,&int_param);
	if ( status == X400_E_NOERROR ) {
	    printf ("P772 ALI Notification Request  %d\n", int_param);
	}
	
	
    }
}


static int get_hi (
    struct X400mtMessage *mp
)
{
    struct X400PSS *pss  = NULL;
    int n;
    char 			buffer[BUFSIZ];
    size_t 			length;
    int 			status;

  
    for ( n = 1; ; n++ ) {
	status = X400mtPSSGet (mp,X400_S_HANDLING_INSTRUCTIONS,n,&pss);
	if ( status != X400_E_NOERROR ) 
	    return status;
	
	status = X400PSSGetStrParam (pss,buffer, sizeof buffer , &length);
	if ( status == X400_E_NOERROR ) {
	    printf ("P772 Handling instruction %d: %.*s\n",
		    n, (int)length, buffer);
	} else if ( status == X400_E_NO_VALUE) {
	    return X400_E_NO_VALUE;
	}
    }
}

static int get_mi (
    struct X400mtMessage *mp
)
{
    struct X400PSS *pss  = NULL;
    int n;
    char 			buffer[BUFSIZ];
    size_t 			length;
    int 			status;

  
    for ( n = 1; ; n++ ) {
	status = X400mtPSSGet (mp,X400_S_MESSAGE_INSTRUCTIONS,n,&pss);
	if ( status != X400_E_NOERROR ) 
	    return status;
	
	status = X400PSSGetStrParam (pss,buffer, sizeof buffer , &length);
	if ( status == X400_E_NOERROR ) {
	    printf ("P772 Message instruction %d: %.*s\n",
		    n, (int)length, buffer);
	} else if ( status == X400_E_NO_VALUE) {
	    return X400_E_NO_VALUE;
	}
    }
}

static int get_dist_codes_sic(
    struct X400mtMessage *mp
)
{
    struct X400PSS *pss  = NULL;
    struct X400DistField *distfield = NULL;
    int n;
    char 			buffer[BUFSIZ];
    size_t 			length;
    int 			status;

   
    for ( n = 1; ; n++ ) {
	status = X400mtPSSGet (mp,X400_S_DIST_CODES_SIC,n,&pss);
	if (status == X400_E_NO_VALUE) {
	    break; /* Now try the extension fields*/
	} else if ( status != X400_E_NOERROR ) {
	    return status;
	}
	
	status = X400PSSGetStrParam (pss,buffer, sizeof buffer , &length);
	if ( status == X400_E_NOERROR ) {
	    printf ("P772 Distribution code SIC %d: %.*s\n",
		    n, (int)length, buffer);
	} 
    }

    
 for ( n = 1; ; n++ ) {
	status = X400mtDistFieldGet (mp,n,&distfield);
	if (status == X400_E_NO_VALUE) {
	    break; /* Now try the extension fields*/
	} else if ( status != X400_E_NOERROR ) {
	    return status;
	}
	
	status = X400DistFieldGetStrParam (
	    distfield,
	    X400_S_DIST_CODES_EXT_OID,
	    buffer,
	    sizeof buffer,
	    &length
	);
	if ( status == X400_E_NOERROR ) {
	    printf ("P772 Distribution code Field Extension OID %d: %.*s\n",
		    n, (int)length, buffer);
	} 
	
	status = X400DistFieldGetStrParam (
	    distfield,
	    X400_S_DIST_CODES_EXT_VALUE,
	    buffer,
	    sizeof buffer,
	    &length
	);
	if ( status == X400_E_NOERROR ) {
	    printf ("P772 Distribution code Field Extension value %d: %.*s\n",
		    n, (int)length, buffer);
	} 
 }
    
 return X400_E_NOERROR;
}

static int get_other_recips(
    struct X400mtMessage *mp
)
{
    struct X400OtherRecip *otherrecip = NULL;
    int n;
    char 			buffer[BUFSIZ];
    size_t 			length;
    int 			status;
    int                         type;
   
    for ( n = 1; ; n++ ) {
	status = X400mtOtherRecipGet (mp,n,&otherrecip);
	if ( status != X400_E_NOERROR ) {
	    return status;
	}

	status = X400OtherRecipGetIntParam (otherrecip,&type);
	if ( status == X400_E_NOERROR ) {
	    printf ("P772 Other Recipient type %d: %i \n",n,type);
	}
	
	
	status = X400OtherRecipGetStrParam (otherrecip,
					    buffer,
					    sizeof buffer ,
					    &length);
	if ( status == X400_E_NOERROR ) {
	    printf ("P772 Other Recipient Designator %d: %.*s\n",
		    n, (int)length, buffer);
	}
	
	
    }
    return X400_E_NOERROR;
}

static int get_recips(
    struct 	X400mtMessage *mp,
    int		recip_type,
    char	*recip_str
)
{
    char 			buffer[BUFSIZ];
    int 			status;
    struct X400Recipient 	*rp;
    size_t 			length;
    int				n;
    int				int_param;

    for ( n = 1; ; n++ ) {
	status = X400mtRecipGet (mp, recip_type, n, &rp);
	if ( status == X400_E_NO_RECIP ) 
	    break;
	if ( status != X400_E_NOERROR ) {
	    printf("error getting recip %d\n", n);
	    fprintf (stderr, "Error in X400mtRecipGet: %s\n", 
		X400mtError (status));
	    return (status);
	}
	
	/* Note: recipient may not actually have an O/R address */
	status = X400mtRecipGetStrParam (rp, X400_S_OR_ADDRESS,
					 buffer, sizeof buffer , &length);
	if ( status == X400_E_NOERROR ) {
	    printf ("%s recipient %d: %.*s\n", recip_str, n, (int)length, buffer);
	}

	/* envelope recipient integer parameters */
	/* Responsibility */
	status = X400mtRecipGetIntParam 
	    (rp, X400_N_RESPONSIBILITY, &int_param);
	if ( status == X400_E_NOERROR ) {
	    printf ("\tResponsibility %d\n", int_param);
	}

	/*  MTA report request */
	status = X400mtRecipGetIntParam 
	    (rp, X400_N_MTA_REPORT_REQUEST, &int_param);
	if ( status == X400_E_NOERROR ) {
	    printf ("\tMTA report request %d\n", int_param);
	}

	/* Originator report request */
	status = X400mtRecipGetIntParam 
	    (rp, X400_N_REPORT_REQUEST, &int_param);
	if ( status == X400_E_NOERROR ) {
	    printf ("\tOriginator report request %d\n", int_param);
	}

	/* envelope recipient string parameters */
	/* Originator requested alternate recipient: X.400 string form */
	/* this is a submission argument, so should get error */
	status = X400mtRecipGetStrParam 
	    (rp, X400_S_ORIGINATOR_REQUESTED_ALTERNATE_RECIPIENT,  
	      buffer, sizeof buffer , &length);
	if ( status == X400_E_NOERROR ) {
	    printf ("\tOriginator requested AR  %.*s\n", (int)length, buffer);
	}

	status = X400mtRecipGetStrParam 
	    (rp, X400_S_ORIGINATOR_REQUESTED_ALTERNATE_RECIPIENT_DN,  
	      buffer, sizeof buffer , &length);
	if ( status == X400_E_NOERROR ) {
	    printf ("\tOriginator requested AR DN  %.*s\n", (int)length, buffer);
	}

	/* content recipient integer parameters */
	/* Notification request */
	status = X400mtRecipGetIntParam 
	    (rp, X400_N_NOTIFICATION_REQUEST, &int_param);
	if ( status == X400_E_NOERROR ) {
	    printf ("\tNotification request %d\n", int_param);
	}

	/* Reply Request */
	status = X400mtRecipGetIntParam 
	    (rp, X400_N_REPLY_REQUESTED, &int_param);
	if ( status == X400_E_NOERROR ) {
	    printf ("\tReply Request %s\n", int_param ? "false" : "true");
	}

	/* content recipient string parameters */
	/* Notification request */
	/*  Free Form Name */
	status = X400mtRecipGetStrParam 
	    (rp, X400_S_FREE_FORM_NAME, buffer, sizeof buffer , &length);
	if ( status == X400_E_NOERROR ) {
	    printf ("\tFree form name  %.*s\n", (int)length, buffer);
	}

	/* Telephone Number */
	status = X400mtRecipGetStrParam 
	    (rp, X400_S_TELEPHONE_NUMBER, buffer, sizeof buffer , &length);
	if ( status == X400_E_NOERROR ) {
	    printf ("\tTelephone Number %.*s\n", (int)length, buffer);
	}

	status = X400mtRecipGetStrParam 
	    (rp, X400_S_ARRIVAL_TIME, buffer, sizeof buffer , &length);
	if ( status == X400_E_NOERROR )
	    printf ("Arrival Time: %.*s\n", (int)length, buffer);

	/* Positive reports */
	/* Message Delivery Time  - present for successful deliveries */
	status = X400mtRecipGetStrParam 
	    (rp, X400_S_MESSAGE_DELIVERY_TIME, buffer, sizeof buffer , &length);
	if ( status == X400_E_NOERROR )
	    printf ("Subject Message Delivery Time: %.*s\n", (int)length, buffer);

	/* type of MTS user - successful report only */
	status = X400mtRecipGetIntParam 
	    (rp, X400_N_TYPE_OF_USER, &int_param);
	if ( status == X400_E_NOERROR ) {
	    printf ("\tType of MTS user %d\n", int_param);
	}
	
	/* Negative reports */
	/* Supplementary info (reported recips only) */
	status = X400mtRecipGetStrParam 
	    (rp, X400_S_SUPPLEMENTARY_INFO, buffer, sizeof buffer , &length);
	if ( status == X400_E_NOERROR ) {
	    printf ("\tSupplementary info %.*s\n", (int)length, buffer);
	}

	/* reason - non-success report only */
	status = X400mtRecipGetIntParam 
	    (rp, X400_N_NON_DELIVERY_REASON, &int_param);
	if ( status == X400_E_NOERROR ) {
	    printf ("\tNon-delivery Reason %d\n", int_param);
	}

	/* diagnostic info - non-success report only */
	status = X400mtRecipGetIntParam 
	    (rp, X400_N_NON_DELIVERY_DIAGNOSTIC, &int_param);
	if ( status == X400_E_NOERROR ) {
	    printf ("\tNon-delivery diagnostic %d\n", int_param);
	}

	/* CEITS */
	status = X400mtRecipGetStrParam 
	    (rp, X400_S_CONVERTED_ENCODED_INFORMATION_TYPES,
	     buffer, sizeof buffer , &length);
	if ( status == X400_E_NOERROR ) {
	    printf ("\tCEIT info %.*s\n", (int)length, buffer);
	}
	

#define USE_REDIRECTION_HISTORY 1
#ifdef  USE_REDIRECTION_HISTORY
 {
   
     status = do_redirection_hist(rp);
     
     if ( status == X400_E_NO_VALUE ) {
	 /* do nothing */
     } else if (status == X400_E_MISSING_ATTR ) {
	 printf("There is no Redirection History\n");
     } else {
	 printf("error getting Redirection History\n");
	 fprintf (stderr, "Error fetching Redirection History: %s\n", 
		  X400mtError (status));
	 return status;
     }  
 }
#endif
	status = X400mtRecipGetIntParam(rp, X400_N_ACP127_NOTI_TYPE, &int_param);
	if ( status == X400_E_NOERROR ) {
	    if (int_param & X400_ACP127_NOTI_TYPE_NEG) {
		printf("P772 ACP127 Notification Request Type Negative\n");
	    }
	    if (int_param & X400_ACP127_NOTI_TYPE_POS) {
		printf("P772 ACP127 Notification Request Type Positive\n");
	    }
	    
	    if (int_param & X400_ACP127_NOTI_TYPE_TRANS) {
		printf("P772 ACP127 Notification Request Type Transfer\n");
	    }
	    
	}


 /* Fetch ACP127 Notification Response */

    {
	struct X400ACP127Resp *resp = NULL;
	int int_param = 0;
	
	status = X400ACP127RespGet(rp,&resp);
	if ( status == X400_E_NOERROR ) {

	
	status = X400ACP127RespGetIntParam(resp,&int_param);
	if ( status == X400_E_NOERROR ) {
	    if (int_param & X400_ACP127_NOTI_TYPE_NEG) {
		printf("P772 ACP127 Notification Response Type Negative\n");
	    }
	    if (int_param & X400_ACP127_NOTI_TYPE_POS) {
		printf("P772 ACP127 Notification Response Type Positive\n");
	    }
	    if (int_param & X400_ACP127_NOTI_TYPE_TRANS) {
		printf("P772 ACP127 Notification Response Type Transfer\n");
	    }
	}

	status = X400ACP127RespGetStrParam (resp,
					    X400_S_ACP127_NOTI_RESP_TIME,
					    buffer,
					    sizeof buffer,
					    &length);
	if ( status == X400_E_NOERROR )
	    printf ("P772 ACP127 Response time: %.*s\n", (int)length, buffer);

	
	status = X400ACP127RespGetStrParam (resp,
					    X400_S_ACP127_NOTI_RESP_RECIPIENT,
					    buffer,
					    sizeof buffer,
					    &length);
	if ( status == X400_E_NOERROR )
	    printf ("P772 ACP127 Recipient: %.*s\n", (int)length, buffer);

	status = X400ACP127RespGetStrParam (resp,
					    X400_S_ACP127_NOTI_RESP_SUPP_INFO,
					    buffer,
					    sizeof buffer,
					    &length);
	if ( status == X400_E_NOERROR )
	    printf ("P772 ACP127 Resp supp info: %.*s\n", (int)length, buffer);


	/* Fetch ACP127 Notification Response Address list indicator */

	get_acp127_ali(resp);
	
	
	}
    }
    
	printf("-----------------------------------------------------------\n");
    }
    return X400_E_NOERROR;
}

static int do_rep_env(
    struct X400mtMessage *mp
)
{
    char 			buffer[BUFSIZ];
    int 			status;
    size_t 			length;

    /* Envelope Attributes */
    
    /* string attributes */
    /* The ORADDRESS in the message is the (envelope) originator */
    status = X400mtMsgGetStrParam (mp, X400_S_OR_ADDRESS,
				   buffer, sizeof buffer , &length);
    if ( status != X400_E_NOERROR ) {
	if (status == X400_E_NO_VALUE) {
	    printf("Reports don't have an originator\n");
	} else {
	    printf("error getting originator address\n");
	    fprintf (stderr, "Error in getting originator: %s\n", 
		X400mtError (status));
	    return (status);
	}
    } else {
	printf ("Originator: %.*s\n", (int)length, buffer);
    }

    
    /* report identifier - rather confusingly, use X400_S_MESSAGE_IDENTIFIER */
    status = X400mtMsgGetStrParam (mp, X400_S_MESSAGE_IDENTIFIER,
				   buffer, sizeof buffer , &length);
    if ( status != X400_E_NOERROR ) {
	printf("error getting report identifier\n");
	fprintf (stderr, "Error in getting report identifier: %s\n", 
	    X400mtError (status));
	return (status);
    }
    printf ("report identifier: %.*s\n", (int)length, buffer);

    /* content identifier not present in report */

    /* original EITs */
    status = X400mtMsgGetStrParam (mp, 
	X400_S_ORIGINAL_ENCODED_INFORMATION_TYPES,
       buffer, sizeof buffer , &length);
    if (status == X400_E_NOERROR) {
        printf ("original EITs: %.*s\n", (int)length, buffer);
    } else if ( status != X400_E_NO_VALUE ) {
	fprintf (stderr, "Error in getting original EITs : %s\n", 
	    X400mtError (status));
	return (status);
    }
    

    /* submission time not present in report*/



#ifdef WANT_TRACE_INFO
 {
     status = do_trace_info(mp);
     if ( status == X400_E_NO_VALUE) {
	 
     } else if (status == X400_E_MISSING_ATTR ) {
	 printf("There is no Trace Info\n");
     } else {
	 printf("error getting Trace Info\n");
	 fprintf (stderr, "Error fetching trace info: %s\n", 
		  X400mtError (status));
	 return status;
     }
	 
 }    
#endif


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
		  X400mtError (status));
	 return status;
     }  
 }
#endif

 status = do_orig_and_dl(mp);
     
 if ( status == X400_E_NO_VALUE ) {
     /* do nothing */
 } else if (status == X400_E_MISSING_ATTR ) {
     printf("There is no orig and dl exp history\n");
 } else {
     fprintf (stderr, "Error getting orig and dl exp history: %s\n", 
              X400mtError (status));
	 return status;
 }  
 

 
 /* latest delivery time */
 status = X400mtMsgGetStrParam (mp, X400_S_LATEST_DELIVERY_TIME,
                                buffer, sizeof buffer , &length);
 if ( status != X400_E_NOERROR && status != X400_E_INVALID_ATTR) {
     printf("error getting latest delivery time\n");
     fprintf (stderr, "Error in getting latest delivery time: %s\n", 
              X400mtError (status));
     return (status);
 } else {
     printf ("latest delivery time: %.*s\n", (int)length, buffer);
 }
 /* Get the envelope recipients */
 status = get_recips(mp, X400_RECIP_ENVELOPE, "envelope");
 if ( status != X400_E_NOERROR && status != X400_E_NO_RECIP) {
     printf("error getting recipients\n");
     fprintf (stderr, "Error in getting recipient: %s\n", 
              X400mtError (status));
     return (status);
 }

/* Get the reporting dl name */
 status = get_recips(mp, X400_REPORTING_DL_NAME, "Reporting DL name");
 if ( status != X400_E_NOERROR && status != X400_E_NO_RECIP) {
     printf("error getting dl name\n");
     fprintf (stderr, "Error in getting recipient: %s\n", 
              X400mtError (status));
     return (status);
 }
 
 
    return X400_E_NOERROR;
}

static int do_rep_content(
    struct X400mtMessage *mp
)
{
    char 			buffer[BUFSIZ];
    int 			status;
    size_t 			length;

    /* Message Body */
    printf("Report content\n");

    /* subject ID */
    status = X400mtMsgGetStrParam (mp, X400_S_SUBJECT_IDENTIFIER,
				   buffer, sizeof buffer , &length);
    if ( status != X400_E_NOERROR ) {
	printf("error getting subject id\n");
	fprintf (stderr, "Error in getting subject id: %s\n", 
	    X400mtError (status));
	return (status);
    }
    printf ("subject id: %.*s\n", (int)length, buffer);

    /* Get the reported recipients */
    status = get_recips(mp, X400_RECIP_REPORT, "reported-recip");
    if ( status != X400_E_NOERROR && status != X400_E_NO_RECIP) {
	printf("error getting reported-recip\n");
	fprintf (stderr, "Error in getting reported-recip: %s\n",
		 X400mtError (status));

		return (status);
    }
    
#ifdef WANT_TRACE_INFO
 {
     status = do_trace_info(mp);
     if ( status == X400_E_NO_VALUE) {
	 
     } else if (status == X400_E_MISSING_ATTR ) {
	 printf("There is no Trace Info\n");
     } else {
	 printf("error getting Trace Info\n");
	 fprintf (stderr, "Error fetching trace info: %s\n", 
		  X400mtError (status));
	 return status;
     }
 }    
#endif
    
    return(status);
}


static int do_rep_retcontent(
    struct X400mtMessage *mp
)
{
    char 			buffer[BUFSIZ];
    int 			status;
    size_t 			length;
    int int_param;

    printf("Returned content\n");

    status = X400mtMsgGetIntParam 
	    (mp, X400_N_NUM_ATTACHMENTS, &int_param);
    if ( status == X400_E_NOERROR ) 
	    printf ("\tNum attachments %d\n", int_param);
    else 
	    printf ("\tNum attachments STATUS %d\n", status);

    /* And now get message text (or it could be another type - ie not IA5) */
    status = X400mtMsgGetStrParam (mp, X400_T_IA5TEXT,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR )
	printf ("Text:\n%.*s\n", (int)length, buffer);
    return status;
}


static void usage(void) {
    printf("usage: %s\n", optstr);
    printf("\t where:\n");
    printf("\t -u : Don't prompt to override defaults \n");
    printf("\t -o : Originator \n");
    printf("\t -O : Originator Return Address \n");
    printf("\t -r : Recipient\n");
    printf("\t -c : X.400 passive channel\n");
    printf("\t -l : Logline\n");
    printf("\t -E : Stop after one attempt to transfer a msg\n");
    return;
}

static int do_trace_info (
    struct X400mtMessage *mp
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
	status = X400mtTraceInfoGet (mp, n, &info, X400_TRACE_INFO);
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
		     X400mtError (status));
	}

	status = X400TraceInfoGetStrParam (info, X400_S_DSI_ARRIVAL_TIME,
					   DSI_arrival_time, BUFSIZ , &length);
	if ( status == X400_E_NOERROR ) {
	    DSI_arrival_time[length] = '\0';
	    printf ("DSI arrival time %d  :%s\n",n,DSI_arrival_time);
	} else {
	    fprintf (stderr, "Error in do_trace_info: %s\n",
		     X400mtError (status));
	}
	    
	status = X400TraceInfoGetIntParam (info,X400_N_DSI_ROUTING_ACTION,
					   &DSI_routing_action);
	if ( status == X400_E_NOERROR ) {
	    printf ("DSI routing action %d  :%s\n",n,
		    DSI_routing_action ? "rerouted" : "relayed");
	} else {
	    fprintf (stderr, "Error in do_trace_info: %s\n",
		     X400mtError (status));
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
		     X400mtError (status));
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
		     X400mtError (status));
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
		     X400mtError (status));
	}
	
	
	status = X400TraceInfoGetIntParam (info,
					   X400_N_DSI_AA_REDIRECTED,
					   &DSI_AA_redirected);
	if ( status == X400_E_NOERROR ) {
	    printf ("DSI AA redirected %d  :%d\n",n,DSI_AA_redirected );
	} else {
	    fprintf (stderr, "Error in do_trace_info: %s\n",
		     X400mtError (status));
	}
	
	status = X400TraceInfoGetIntParam (info,
					   X400_N_DSI_AA_DLOPERATION,
					   &DSI_AA_dl_operation);
	if ( status == X400_E_NOERROR ) {
	    printf ("DSI AA dl operation %d  :%d\n",n,DSI_AA_dl_operation );
	} else {
	    fprintf (stderr, "Error in do_trace_info: %s\n",
		     X400mtError (status));
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
    struct X400mtMessage *mp
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
	status = X400mtInternalTraceInfoGet (mp,n, &info);
	if (status == X400_E_NO_VALUE) {
	    return status;
	}
	status = X400InternalTraceInfoGetStrParam (info,
						   X400_S_GLOBAL_DOMAIN_ID,
						   GDI, BUFSIZ , &length);
	if (status == X400_E_NO_VALUE) {
	    return X400_E_NO_VALUE;
	}
	
	if ( status == X400_E_NOERROR ) {
	    GDI[length] = '\0';
	    printf ("GDI entry %d Domain:%s\n",n,GDI);
	} else {
	    fprintf (stderr, "Error in do_trace_info: %s\n",
		     X400mtError (status));
	}
	
	/*MTA Name */
	status = X400InternalTraceInfoGetStrParam (info,
						   X400_S_MTA_NAME,
						   SI_time, BUFSIZ , &length);
	if (status == X400_E_NO_VALUE) {
	    return X400_E_NO_VALUE;
	}
	
	if ( status == X400_E_NOERROR ) {
	    SI_time[length] = '\0';
	    printf ("SI time entry %d :%s\n",n,SI_time);
	} else {
	    fprintf (stderr, "Error in do_internal_trace_info: %s\n",
		     X400mtError (status));
	}

	/*SI MTA Name*/
	
	status = X400InternalTraceInfoGetStrParam (info,
						   X400_S_MTA_SI_TIME,
						   SI_MTA_name, BUFSIZ , &length);
	if (status == X400_E_NO_VALUE) {
	    return X400_E_NO_VALUE;
	}
	
	if ( status == X400_E_NOERROR ) {
	    SI_MTA_name[length] = '\0';
	    printf ("SI MTA Name entry %d :%s\n",n,SI_MTA_name);
	} else {
	    fprintf (stderr, "Error in do_internal_trace_info: %s\n",
		     X400mtError (status));
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
		     X400mtError (status));
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
		     X400mtError (status));
	}

	if (SI_attempted_action == X400_MTA_SI_RA_MTA) {
	    /* SI Attempted MTA Name */
	    status = X400InternalTraceInfoGetStrParam (info,
						       X400_S_MTA_SI_ATTEMPTED_MTA,
						       SI_attempted_MTA_name,
						       BUFSIZ,&length);
	    if (status == X400_E_NO_VALUE) {
		return X400_E_NO_VALUE;
	    }
	
	    if ( status == X400_E_NOERROR ) {
		SI_attempted_MTA_name[length] = '\0';
		printf ("Supplied information attempted MTA %d"
			":%s\n",n,SI_attempted_MTA_name);
	    } else {
		fprintf (stderr, "Error in do_trace_info: %s\n",
			 X400mtError (status));
	    }

	} else {
	
	    /*SI Attempted domain*/
	    status = X400InternalTraceInfoGetStrParam (info,
						       X400_S_MTA_SI_ATTEMPTED_DOMAIN,
						       SI_attempted_domain, BUFSIZ,
						       &length);
	    if (status == X400_E_NO_VALUE) {
		return X400_E_NO_VALUE;
	    }
	
	    if ( status == X400_E_NOERROR ) {
		SI_attempted_domain[length] = '\0';
		printf ("Supplied information attempted Domain entry"
			" %d :%s\n",n,SI_attempted_domain);
	    } else {
		fprintf (stderr, "Error in do_internal_trace_info: %s\n",
			 X400mtError (status));
	    }
	}

	

	/* SI MTA Defered time*/
	status = X400InternalTraceInfoGetStrParam (info,
						   X400_S_MTA_SI_DEFERRED_TIME,
						   SI_defered_time,
						   BUFSIZ , &length);
	if (status == X400_E_NO_VALUE) {
	    return X400_E_NO_VALUE;
	}
	
	if ( status == X400_E_NOERROR ) {
	    SI_defered_time[length] = '\0';
	    printf ("SI_defered_time entry %d :%s\n",n,
		    SI_defered_time);
	} else {
	    fprintf (stderr, "Error in do_internaltrace_info: %s\n",
		     X400mtError (status));
	}
	
	/* SI MTA Converted Encoded Information Types*/
	status = X400InternalTraceInfoGetStrParam (info,
						   X400_S_MTA_SI_CEIT,
						   SI_CEIT,
						   BUFSIZ , &length);
	if (status == X400_E_NO_VALUE) {
	    return X400_E_NO_VALUE;
	}
	
	if ( status == X400_E_NOERROR ) {
	    SI_CEIT[length] = '\0';
	    printf ("SI_CEIT entry %d :%s\n",n,
		    SI_CEIT);
	} else {
	    fprintf (stderr, "Error in do_internaltrace_info: %s\n",
		     X400mtError (status));
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
		     X400mtError (status));
	}

		
    }
    if (n == 1) {
	printf("No Internal Trace Info found in message\n");
    } else {
	printf("%d Internal Trace Info entries found in message\n", n - 1);
    }
    return X400_E_NOERROR;
    
}

static int do_redirection_hist (
    struct X400Recipient *recip
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
	status = X400RediHistGet (recip,n, &hist);
	if (status != X400_E_NOERROR) {
	    return status;
	}
	
	status = X400RediHistGetStrParam (hist,X400_S_REDIRECTION_TIME,
					  redirection_time, BUFSIZ,&length);
	

	if ( status == X400_E_NOERROR ) {
	    redirection_time[length] = '\0';
	    printf ("Redirection time: %i %s\n",n,redirection_time);
	} else {
	    fprintf (stderr, "Error in do_redirection_hist: %s\n",
		     X400mtError (status));
	}

	status = X400RediHistGetStrParam (hist, X400_S_OR_ADDRESS,
					  or_address, BUFSIZ , &length);
	if ( status == X400_E_NOERROR ) {
	    or_address[length] = '\0';
	    printf ("Redirection OR address %i  :%s\n",n,or_address);
	} else {
	    fprintf (stderr, "Error in do_trace_info: %s\n",
		     X400mtError (status));
	}

	status = X400RediHistGetStrParam (hist,X400_S_DIRECTORY_NAME,
					  dn, BUFSIZ , &length);
	if ( status == X400_E_NOERROR ) {
	    dn[length] = '\0';
	    printf ("Redirection DN %i  :%s\n",n,dn);
	} else {
	    fprintf (stderr, "Error in do_trace_info: %s\n",
		     X400mtError (status));
	}
	
	status = X400RediHistGetIntParam (hist,X400_N_REDIRECTION_REASON,&rr);
	if ( status == X400_E_NOERROR ) {
	    printf ("DSI routing action %d  :%s\n",n,X400RediReason(rr));
	} else {
	    fprintf (stderr, "Error in do_redirection_hist: %s\n",
		     X400mtError (status));
	}
	
    }
    if (n == 1) {
	printf("No Trace Info found in message\n");
    } else {
	printf("%d Trace Info entries found in message\n", n - 1);
    }
    return X400_E_NOERROR;
    
}

static int do_redirection_hist_env (
    struct X400mtMessage *msg
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
	status = X400mtRediHistGetEnv (msg,n, &hist);
	if (status != X400_E_NOERROR) {
	    return status;
	}
	
	status = X400RediHistGetStrParam (hist,X400_S_REDIRECTION_TIME,
					  redirection_time, BUFSIZ,&length);
	

	if ( status == X400_E_NOERROR ) {
	    redirection_time[length] = '\0';
	    printf ("Redirection time: %i %s\n",n,redirection_time);
	} else {
	    fprintf (stderr, "Error in do_redirection_hist: %s\n",
		     X400mtError (status));
	}

	status = X400RediHistGetStrParam (hist, X400_S_OR_ADDRESS,
					  or_address, BUFSIZ , &length);
	if ( status == X400_E_NOERROR ) {
	    or_address[length] = '\0';
	    printf ("Redirection OR address %i  :%s\n",n,or_address);
	} else {
	    fprintf (stderr, "Error in do_trace_info: %s\n",
		     X400mtError (status));
	}

	status = X400RediHistGetStrParam (hist,X400_S_DIRECTORY_NAME,
					  dn, BUFSIZ , &length);
	if ( status == X400_E_NOERROR ) {
	    dn[length] = '\0';
	    printf ("Redirection DN %i  :%s\n",n,dn);
	} else {
	    fprintf (stderr, "Error in do_trace_info: %s\n",
		     X400mtError (status));
	}
	
	status = X400RediHistGetIntParam (hist,X400_N_REDIRECTION_REASON,&rr);
	if ( status == X400_E_NOERROR ) {
	    printf ("DSI routing action %d  :%s\n",n,X400RediReason(rr));
	} else {
	    fprintf (stderr, "Error in do_redirection_hist: %s\n",
		     X400mtError (status));
	}
	
    }
    if (n == 1) {
	printf("No Redirection histories found in message\n");
    } else {
	printf("%d Redirection histories entries found in message\n", n - 1);
    }
    return X400_E_NOERROR;
    
}

static int get_acp127_ali (
    struct X400ACP127Resp *resp
)
{
    struct X400ALI *ali  = NULL;
    int n;
    char 			buffer[BUFSIZ];
    size_t 			length;
    int 			status;
    int                         int_param;
  
    for ( n = 1; ; n++ ) {
	status = X400ACP127RespGetALI (resp,&ali,n);
	if ( status != X400_E_NOERROR ) 
	    return status;
	
	status = X400ALIGetStrParam (ali,X400_S_IOB_OR_ADDRESS,
				     buffer, sizeof buffer , &length);
	if ( status == X400_E_NOERROR ) {
	    printf ("P772 ACP127 response Notification "
		    "ALI recipient OR address %d: %.*s\n",
		    n, (int)length, buffer);
	} else if ( status == X400_E_NO_VALUE) {
	    return X400_E_NO_VALUE;
	}
	    

	status = X400ALIGetStrParam (ali,X400_S_IOB_DN_ADDRESS,
				     buffer, sizeof buffer , &length);
	if ( status == X400_E_NOERROR ) {
	    printf ("P772 ACP127 response Notification"
		    " ALI recipient DN address %d: %.*s\n",
		    n, (int)length, buffer);
	}

	status = X400ALIGetStrParam(ali,X400_S_IOB_FREE_FORM_NAME,
				    buffer, sizeof buffer , &length);
	if ( status == X400_E_NOERROR ) {
	    printf ("P772 ACP127 response Notification"
		    " ALI recipient free form name %d: %.*s\n",
		    n, (int)length, buffer);
	}
	
	status = X400ALIGetStrParam (ali, X400_S_IOB_TEL,
				     buffer, sizeof buffer , &length);
	if ( status == X400_E_NOERROR ) {
	    printf ("P772 ACP127 response Notification"
		    "ALI recipient Tel number %d: %.*s\n",
		    n, (int)length, buffer);
	}

	status = X400ALIGetIntParam (ali,X400_N_ALI_TYPE , &int_param);
	if ( status == X400_E_NOERROR ) {
	    printf ("P772 ACP127 response Notification"
		    "ALI Type  %d\n", int_param);
	}

	status = X400ALIGetIntParam (ali,X400_N_ALI_NOTIFICTAION_REQUEST ,
				     &int_param);
	if ( status == X400_E_NOERROR ) {
	    printf ("P772 ACP127 response Notification"
		    "ALI Notification Request  %d\n", int_param);
	}
	

	status = X400ALIGetIntParam (ali,X400_N_ALI_REPLY_REQUEST ,&int_param);
	if ( status == X400_E_NOERROR ) {
	    printf ("P772 ACP127 response Notification"
		    "ALI Notification Request  %d\n", int_param);
	}
	
	
    }
}


/* This is a very simple function to go through the P772 body parts only.
 * It's a modified version of what's in x400_msrcv.c
 */
static int get_body_parts(
    struct X400mtMessage *mp
)
{
#define INIT_BUFFER_SIZE 10000
    int 	status;
    int 	type = 0;
    char 	*buf;
    size_t 	buflen;
    size_t 	ret_len;
    int i = 0;
    
    struct X400Bodypart *bp;

    
    buf = (char *) malloc(INIT_BUFFER_SIZE);
    if (buf == NULL) {
	fprintf(stderr, "can't alloc %d bytes\n", INIT_BUFFER_SIZE);
	return X400_E_NOMEMORY;
    }
    buflen = INIT_BUFFER_SIZE;
    
    for (i = 1; i <= num_atts ; i++) {
	printf ("Get attachment %d\n", i);
	status = X400mtMsgGetBodypart(mp, i, &type, &bp);
	if (status != X400_E_NOERROR &&
            status != X400_E_MESSAGE_BODY) {
	    fprintf(stderr, "X400msMsgGetBodypart %d failed %s\n", i,
		    X400mtError (status));
	    /* skip this bodypart.
	     * The IA5text bodypart has already been fetched throught the
	     * X400mtMsgGetStrParam interface.
	     * So fetching that bodypart fails here.
	     */
	    break;
	}
	
        printf("got Bodypart %d, type = %s\n", i, 
               type == X400_T_IA5TEXT ? "IA5" : 
               type == X400_T_ISO8859_1 ? "ISO8859-1" :
               type == X400_T_ISO8859_2 ? "ISO8859_2" :
               type == X400_T_BINARY ? "Binary" : 
               type == X400_T_MESSAGE ? "Message" : 
               type == X400_T_FTBP ? "File Transfer BP" :
               type == X400_T_GENERAL_TEXT ? "General Text" :
               type == X400_T_CORRECTIONS ? "P772 Correction" :
               type == X400_T_ACP127DATA ? "P772 ACP127 Data" :
               type == X400_T_MM ? "P772 Forwarded MM" :
               type == X400_T_FWDENC ? "P772 Forwarded Encrypted" :
               type == X400_T_FWD_CONTENT ? " Forwarded Content":
               type == X400_T_ADATP3 ? "P772 ADatP3" : "unknown"
        );
        
        switch (type) {
        case X400_T_FWD_CONTENT:
        {
            /* Fetch a forwarded bodypart */
            char 			buffer[BUFSIZ];
            struct X400Message *fwd_content_env = NULL;
            size_t length = 0;
            
            status = X400mtMsgGetMessageBody(mp,i,&fwd_content_env);
            if (status != X400_E_NOERROR) {
                fprintf(stderr, "X400mtMsgGetBodypart failed %s\n",
                        X400mtError (status));
                return status;
            }

            status = X400MsgGetStrParam (fwd_content_env,
                                         X400_S_MESSAGE_DELIVERY_TIME,
                                         buffer, sizeof buffer , &length);

            if (status == X400_E_NOERROR) {
                printf ("Forwarded Content bodypart"
                        " Message Delivery Time: %.*s\n",
                        (int)length, buffer);
            } else {
                fprintf(stderr, "X400MsgGetStrParam failed %s\n",
                        X400mtError (status));
            }

            status = X400MsgGetStrParam (fwd_content_env,
                                         X400_S_MESSAGE_IDENTIFIER,
                                         buffer, sizeof buffer , &length);

            if (status == X400_E_NOERROR) {
                printf ("Forwarded Content bodypart"
                        " Message identifier: %.*s\n",
                        (int)length, buffer);
            } else {
                fprintf(stderr, "X400MsgGetStrParam failed %s\n",
                        X400mtError (status));
            }

            /* We currently don't support proof of submission */
            
            
            status = X400MsgGetStrParam (fwd_content_env,
                                         X400_S_FWD_CONTENT_STRING,
                                         buffer, sizeof buffer , &length);
            if ( status == X400_E_NOERROR ) {
                printf ("Forwarded content bodypart data: %i bytes\n",
                        (int)length);
            } else {
                fprintf(stderr, "X400MsgGetStrParam failed %s\n",
                        X400mtError (status));
            }
            break;
            
            
        }
        case X400_T_FWDENC:
        {
            /* Fetch a forwarded bodypart */
            char 			buffer[BUFSIZ];
            struct X400Message *ENC_fwd = NULL;
            size_t length = 0;
            
            status = X400mtMsgGetMessageBody(mp,i,&ENC_fwd);
            if (status != X400_E_NOERROR) {
                fprintf(stderr, "X400mtMsgGetBodypart failed %s\n",
                        X400mtError (status));
                return status;
            }

            status = X400MsgGetStrParam (ENC_fwd, X400_S_MESSAGE_DELIVERY_TIME,
                                         buffer, sizeof buffer , &length);

            if (status == X400_E_NOERROR) {
                printf ("P772 Forwarded Encrypted bodypart"
                        " Message Delivery Time: %.*s\n",
                        (int)length, buffer);
            } else {
                fprintf(stderr, "X400MsgGetStrParam failed %s\n",
                        X400mtError (status));
            }
           
            
            status = X400MsgGetStrParam (ENC_fwd, X400_S_ENCRYPTED_DATA,
                                         buffer, sizeof buffer , &length);
            if ( status == X400_E_NOERROR ) {
                printf ("P772 Forwarded encrypted bodypart data: %i bytes\n",
                        (int)length);
            } else {
                fprintf(stderr, "X400MsgGetStrParam failed %s\n",
                        X400mtError (status));
            }
            break;
        }
            
        case X400_T_MM:
        {
            /* Fetch a forwarded bodypart */
            char 			buffer[BUFSIZ];
            struct X400Message *MM_fwd = NULL;
            size_t length = 0;
            
            status = X400mtMsgGetMessageBody(mp,i,&MM_fwd);
            if (status != X400_E_NOERROR) {
                fprintf(stderr, "X400mtMsgGetBodypart failed %s\n",
                        X400mtError (status));
                return status;
            }

            status = X400MsgGetStrParam (MM_fwd, X400_S_MESSAGE_DELIVERY_TIME,
                                         buffer, sizeof buffer , &length);

            if (status == X400_E_NOERROR) {
                printf ("P772 MM bodypart Message Delivery Time: %.*s\n",
                        (int)length, buffer);
            } else {
                fprintf(stderr, "X400MsgGetStrParam failed %s\n",
                        X400mtError (status));
            }
           
            
            status = X400MsgGetStrParam (MM_fwd, X400_S_SUBJECT,
                                         buffer, sizeof buffer , &length);
            if ( status == X400_E_NOERROR ) {
                printf ("Subject: %.*s\n", (int)length, buffer);
            } else {
                fprintf(stderr, "X400MsgGetStrParam failed %s\n",
                        X400mtError (status));
            }
            break;
        }
        case X400_T_ADATP3:
        {
            int parm_type;
            
            status = X400BodypartGetIntParam(bp,
                                             X400_N_ADATP3_PARM, &parm_type);
            if (status != X400_E_NOERROR) {
                fprintf(stderr, "X400BodypartGetIntParam failed %s\n",
                        X400mtError (status));
                return status;
            }
            
            printf("P772 ADatP3 Parm %d is %d\n",i,parm_type);

	    status = X400BodypartGetIntParam(bp,
                                             X400_N_ADATP3_CHOICE, &parm_type);
            if (status != X400_E_NOERROR) {
                fprintf(stderr, "X400BodypartGetIntParam failed %s\n",
                        X400mtError (status));
                return status;
            }
            
            printf("P772 ADatP3 Choice %d is %d\n",i,parm_type);

	    
            status = X400BodypartGetStrParam(bp,
                                             X400_S_ADATP3_DATA,
                                             buf, 
                                             buflen,
                                             &ret_len);
            if (status == X400_E_NOERROR) 
                printf("P772 ADatP3 %i data: %.*s  \n",
                       i,(int)ret_len,buf);
            break;
        }
        case  X400_T_CORRECTIONS:
        {
            int parm;
            char  buffer [BUFSIZ];
            size_t length;
            status = X400BodypartGetIntParam(bp,
                                             X400_N_CORREC_PARM,
                                             &parm);
             if (status != X400_E_NOERROR) {
                 fprintf(stderr, "X400BodypartGetIntParam failed %s\n",
                         X400mtError (status));
                 return status;
             }
            printf("P772 Corrections Parm %d is %d\n",i,parm);

            status = X400BodypartGetStrParam(bp,
                                             X400_S_CORREC_DATA,
                                             buffer,
                                             BUFSIZ,
                                             &length);
            if ( status == X400_E_NOERROR ) {
                printf("P772 Correction %i data: %.*s  \n",i,(int)length,buffer); 
            } 
            break;
        }
        
        case X400_T_ACP127DATA:
        {
            int parm;
            char  buffer [BUFSIZ];
            size_t length;
            status = X400BodypartGetIntParam(bp,
                                             X400_N_ACP127DATA_PARM,
                                             &parm);
             if (status != X400_E_NOERROR) {
                 fprintf(stderr, "X400BodypartGetIntParam failed %s\n",
                         X400mtError (status));
                 return status;
             }
            printf("P772 ACP127Data Parm %d is %d\n",i,parm);

            status = X400BodypartGetStrParam(bp,
                                             X400_S_ACP127_DATA,
                                             buffer,
                                             BUFSIZ,
                                             &length);
            if ( status == X400_E_NOERROR ) {
                printf("P772 ACP127Data %i data: %.*s  \n",i,(int)length,buffer); 
            } 
            break;
        }
        
        
        default:
            printf ("Skipping bodypart\n");
        
        }
    }
    return status;
}




static int do_orig_and_dl (
    struct X400mtMessage *mp
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

	status = X400mtORandDLGet (mp,n,&orig_and_dl);
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
		     X400mtError (status));
	}

	status =  X400ORandDLGetStrParam(orig_and_dl, X400_S_OR_ADDRESS,
                                         or_address, BUFSIZ , &length);
	if ( status == X400_E_NOERROR ) {
	    or_address[length] = '\0';
	    printf ("do_orig_and_dl OR address %i  :%s\n",n,or_address);
	} else if(status != X400_E_MISSING_ATTR ) {
	    fprintf (stderr, "Error in do_orig_and_dl: %s\n",
		     X400mtError (status));
	}

	status =  X400ORandDLGetStrParam(orig_and_dl,
                                         X400_S_DIRECTORY_NAME,
                                         dn_address, BUFSIZ , &length);
	if ( status == X400_E_NOERROR ) {
	    dn_address[length] = '\0';
	    printf ("originator and DL expansion DN %i  :%s\n",n,dn_address);
	} else if(status != X400_E_MISSING_ATTR ) {
	    fprintf (stderr, "Error in do_orig_and_dl: %s\n",
		     X400mtError (status));
	}
	
	
    }
    if (n == 1) {
	printf("No orig and dl hist found in message\n");
    } else {
	printf("%d orig and dl hist found in message\n", n - 1);
    }
    return X400_E_NOERROR;
    
}
