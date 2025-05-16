/*  Copyright (c) 2004-2008, Isode Limited, London, England.
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
 * Simple example program for transferring a report into the MTA
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include <x400_mtapi.h>
#include "example.h"

/* local functions */
static int send_report (
    int 	drtype 
);
static int add_recips_positive(
    struct X400mtMessage *mp
);
static int add_recips_negative(
    struct X400mtMessage *mp
);
static int build_env(
    struct X400mtMessage *mp
);
static int add_env_recip_info(
    struct X400Recipient *rp
);
static void usage(void);

static int rno = 1;

static int do_redi_hist(
    struct X400Recipient *rp
);

static int do_redi_hist_env(
    struct X400mtMessage *msg
);

static void do_origandl(
    struct X400mtMessage *msg
);


/* These are the data items used */

/* The O/R addresses used are intended to be compatible with those
 * used in the quick install scripts mktailor.tcl, and createmhs.tcl.
 * Change this value to the name of your host.  */
#define HOSTNAME "dhcp-164"

static char *optstr = "uo:O:r:g:G:c:l:R:y:C:iaqsAv";

/* this value is used for the originator of the message */
static const char orig_s[] = "/S=x400test-orig/OU="HOSTNAME"/O=GatewayMTA/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
static const char *orig = orig_s;

/* default recipients */
static const char recip_s[] = "/S=x400test-recip2/OU="HOSTNAME"/O=GatewayMTA/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
static const char *recip = recip_s;


/* default envelope integer values */
static int def_content_type = 2;
static int def_bool = 0;
static int def_priority = 2;

/* default envelope string values */
static const char def_utc[] = "080924120000";
static const char msg_id[] = "/PRMD=TestPRMD/ADMD=TestADMD/C=GB/;"HOSTNAME".2810401-030924.140212";
static const char content_id[] = "030924.140212";
static const char latest_del_time[] = "100927120000Z";

/* default content integer types */
static const int importance = 2;
static const int sensitivity = 3;
static const int autoforwarded = 1;

/* default content string types */
static const char subject[] = "A Test Message";
static const char text[] = "First line\r\nSecond line\r\n";
static const char ipm_id[] = "1064400656.24922*";
static const char ipm_rep_id[] = "1064400656.24923*";
static const char ipm_obs_id[] = "1064400656.24924*";
static const char ipm_rel_id[] = "1064400656.24925*";
static const char orig_ref[] = "orig-ref-val";

/*! Main function of mhssend 
 *! \return 0 on success, otherwise the value of status from X400 call
 */
int main( 
    int         argc,
    char      **argv
) 
{
    int drtype;

    orig = strdup(x400_default_gw_originator);
    recip = strdup(x400_default_gw_recipient);

    if (get_args(argc, argv, optstr)) {
	usage();
	exit(-1);
    }

    if (x400_channel == NULL) {
	printf("You must specify an X.400 channel\n");
	usage();
	exit(-1);
    }

    printf("Delivery Report type (0 = Positive, 1 = Negative): ");
    drtype = ic_fgetc(x400_contype, stdin);
    if ((drtype != '0') && (drtype != '1'))
      exit(1);
    
    drtype -= '0';

    return send_report(drtype);
}

static int send_report (
    int 	drtype 
) 
{
    int status;
    struct X400mtSession *sp;
    struct X400mtMessage *mp;

    if (x400_channel == NULL) {
	fprintf (stderr, "No x400_channel value set in x400tailor file");
    exit(1);
    }

    /* open a new session */
    status = X400mtOpen (x400_channel, &sp);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "Error in Open: %s\n", X400mtError (status));
	exit (status);
    }

    /* start preparing a new report */
    status = X400mtMsgNew (sp, X400_MSG_REPORT, &mp);
    /* Should report all errors as above */
    if ( status != X400_E_NOERROR ) exit (status);

    /* setup originator using a single string */
     status = X400mtMsgAddStrParam (mp, X400_S_OR_ADDRESS, orig, -1);
    if ( status != X400_E_NOERROR ) exit (status);

    /* add various envelope and header recipients into the report */
    if (drtype == 0)
      status = add_recips_positive(mp);
    else
      status = add_recips_negative(mp);
    if ( status != X400_E_NOERROR ) exit (status);

#define ADD_TRACE_INFO 1
#ifdef  ADD_TRACE_INFO
 {
     struct X400TraceInfo *info1; /*Will contain all trace information */
 
     int status;

     status = X400mtTraceInfoNew(mp,&info1,X400_SUBJECT_TRACE_INFO);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,"Failed to allocate new trace info object \n");
	 exit(status);
     }
     
     
     status =  X400TraceInfoAddStrParam (info1,
					 X400_S_GLOBAL_DOMAIN_ID,
					 "/PRMD=wibble/ADMD=TestADMD/C=GB/",
					 -1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_S_GLOBAL_DOMAIN_ID to trace info\n");
	 exit(status);
     }

     status =  X400TraceInfoAddStrParam (info1,
					 X400_S_DSI_ARRIVAL_TIME,
					 "071121125704Z",
					 -1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_S_DSI_ARRIVAL_TIME to trace info\n");
	 exit(status);
     }
     
     /*Add optional*/
     status =  X400TraceInfoAddStrParam (info1,
					 X400_S_DSI_ATTEMPTED_DOMAIN,
					 "/PRMD=atmpdom/ADMD=TestADMD/C=GB/",
					 -1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_S_DSI_ATTEMPTED_DOMAIN to trace info\n");
	 exit(status);
     }
     
     
     /*Add optional*/
     status =  X400TraceInfoAddStrParam (info1,
					 X400_S_DSI_AA_DEF_TIME,
					 "071122125704Z",
					 -1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_S_DSI_AA_DEF_TIME to trace info\n");
	 exit(status);
     }
 }
#endif

 
 
    /* build rest of the report envelope */
    status = build_env(mp);
    if ( status != X400_E_NOERROR ) exit (status);

    
    /* send the report */
    status = X400mtMsgSend (mp);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "Error in MsgSend: %s\n", X400mtError (status));
	exit (status);
    }

    /* delete the message structure */
    status = X400mtMsgDelete (mp);
    if ( status != X400_E_NOERROR ) exit (status);

    mp = NULL;

    /* close the API session */
    return X400mtClose (sp);
}

static int add_recips_positive(
    struct X400mtMessage *mp
)
{
    struct X400Recipient *rp;
    int status;

    /* add new recipient to the report envelope */
    status = X400mtRecipNew (mp, X400_RECIP_ENVELOPE, &rp);
    if ( status != X400_E_NOERROR ) return  (status);
    /* give recip an address */
    status = X400mtRecipAddStrParam (rp, X400_S_OR_ADDRESS, recip, -1);
    if ( status != X400_E_NOERROR ) return  (status);
    /* add other values to recip */
    status = add_env_recip_info (rp);
    if ( status != X400_E_NOERROR ) return  (status);

    /* add new recipient to the report content */
    status = X400mtRecipNew (mp, X400_RECIP_REPORT, &rp);
    if ( status != X400_E_NOERROR ) return  (status);
    /* give recip an address */
    status = X400mtRecipAddStrParam (rp, X400_S_OR_ADDRESS, recip, -1);
    if ( status != X400_E_NOERROR ) return  (status);
    /* add other values to recip */
    status = add_env_recip_info (rp);
    if ( status != X400_E_NOERROR ) return  (status);

    /* add the arrival time for this recipient */
    status = X400mtRecipAddStrParam (rp, X400_S_ARRIVAL_TIME, "070701140026+0100", -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "X400mtMsgAddStrParam returned error: %s\n", X400mtError (status));
	return (status);
    }

    /* add the delivery time for this recipient (positive dr) */
    status = X400mtRecipAddStrParam (rp, X400_S_MESSAGE_DELIVERY_TIME, "040701140026+0100", -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "X400mtMsgAddStrParam returned error: %s\n", X400mtError (status));
	return (status);
    }
    
    /* add the CEIT for this recipient  */
    status = X400mtRecipAddStrParam (rp,X400_S_CONVERTED_ENCODED_INFORMATION_TYPES, "ia5-text", -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "X400mtMsgAddStrParam returned error: %s\n", X400mtError (status));
	return (status);
    }
#define USE_REDIRECTION_HISTORY 1
#ifdef  USE_REDIRECTION_HISTORY
    /* Add redirection history for this recipient (8.3.1.1.1.5)*/
    do_redi_hist(rp);

   
#endif

    return X400_E_NOERROR;
}


static int add_recips_negative(
    struct X400mtMessage *mp
)
{
    struct X400Recipient *rp;
    int status;

    /* add new recipient to the report envelope */
    status = X400mtRecipNew (mp, X400_RECIP_ENVELOPE, &rp);
    if ( status != X400_E_NOERROR ) return  (status);
    /* give recip an address */
    status = X400mtRecipAddStrParam (rp, X400_S_OR_ADDRESS, recip, -1);
    if ( status != X400_E_NOERROR ) return  (status);
    /* add other values to recip */
    status = add_env_recip_info (rp);
    if ( status != X400_E_NOERROR ) return  (status);

    /* add new recipient to the report content */
    status = X400mtRecipNew (mp, X400_RECIP_REPORT, &rp);
    if ( status != X400_E_NOERROR ) return  (status);
    /* give recip an address */
    status = X400mtRecipAddStrParam (rp, X400_S_OR_ADDRESS, recip, -1);
    if ( status != X400_E_NOERROR ) return  (status);
    /* add other values to recip */
    status = add_env_recip_info (rp);
    if ( status != X400_E_NOERROR ) return  (status);

    /* add the arrival time for this recipient */
    status = X400mtRecipAddStrParam (rp, X400_S_ARRIVAL_TIME, "070701140026+0100", -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "X400mtMsgAddStrParam returned error: %s\n", X400mtError (status));
	return (status);
    }

    /* add the supplementary info for this recipient (negative dr) */
    status = X400mtRecipAddStrParam (rp, X400_S_SUPPLEMENTARY_INFO, "Couldn't delivery message", -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "X400mtMsgAddStrParam returned error: %s\n", X400mtError (status));
	return (status);
    }

    /* add the non-delivery reason for this recipient (negative dr) */
    status = X400mtRecipAddIntParam (rp, X400_N_NON_DELIVERY_REASON, 1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "X400mtMsgAddIntParam returned error: %s\n", X400mtError (status));
	return (status);
    }

    /* add the non-delivery diagnostic for this recipient (negative dr) */
    status = X400mtRecipAddIntParam (rp, X400_N_NON_DELIVERY_DIAGNOSTIC, 2);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "X400mtMsgAddIntParam returned error: %s\n", X400mtError (status));
	return (status);
    }

    return X400_E_NOERROR;   
}

static int build_env(
    struct X400mtMessage *mp
)
{
    int status;

    /* Envelope Attributes */

    /* Content Type: 2 or 22 */
    status = X400mtMsgAddIntParam (mp, X400_N_CONTENT_TYPE, def_content_type);
    if ( status != X400_E_NOERROR ) return (status);

    /* X400_N_CONTENT_LENGTH is probe only */

    /* Priority: 0 - normal, 1 - non-urgent, 2 - urgent */
    status = X400mtMsgAddIntParam (mp, X400_N_PRIORITY, def_priority);
    if ( status != X400_E_NOERROR ) return (status);

    /*  Disclosure of recipients: 0 - no, 1 - yes */
    status = X400mtMsgAddIntParam (mp, X400_N_DISCLOSURE, 1);
    if ( status != X400_E_NOERROR ) return (status);

    /* Implicit conversion prohibited: 0 - no, 1 - yes */
    status = X400mtMsgAddIntParam (mp, X400_N_IMPLICIT_CONVERSION_PROHIBITED, def_bool);
    if ( status != X400_E_NOERROR ) return (status);

    /* Alternate recipient allowed: 0 - no, 1 - yes */
    status = X400mtMsgAddIntParam (mp, X400_N_ALTERNATE_RECIPIENT_ALLOWED, def_bool);
    if ( status != X400_E_NOERROR ) return (status);

    /* Content return request: 0 - no, 1 - yes */
    /* hmm - 1 is headers - what does that do in X.400 ? */
    status = X400mtMsgAddIntParam (mp, X400_N_CONTENT_RETURN_REQUEST, 1);
    if ( status != X400_E_NOERROR ) return (status);

    /* Recipient reassignment prohibited: 0 - no, 1 - yes */
    status = X400mtMsgAddIntParam (mp, X400_N_RECIPIENT_REASSIGNMENT_PROHIBITED, def_bool);
    if ( status != X400_E_NOERROR ) return (status);

    /* Distribution List expansion prohibited: 0 - no, 1 - yes */
    status = X400mtMsgAddIntParam (mp, X400_N_DL_EXPANSION_PROHIBITED, def_bool);
    if ( status != X400_E_NOERROR ) return (status);

    /* Conversion with loss prohibited: 0 - no, 1 - yes */
    status = X400mtMsgAddIntParam (mp, X400_N_CONVERSION_WITH_LOSS_PROHIBITED, def_bool);
    if ( status != X400_E_NOERROR ) return (status);

    /* string params */

    /* Message Identifier. In RFC 2156 String form */
    status = X400mtMsgAddStrParam (mp, X400_S_MESSAGE_IDENTIFIER, msg_id, -1);
    if ( status != X400_E_NOERROR ) return (status);

    /* Content Identifier */
    status = X400mtMsgAddStrParam (mp, X400_S_CONTENT_IDENTIFIER, content_id, -1);
    if ( status != X400_E_NOERROR ) return (status);

    /*  Subject Identifier, the Message Identifier of the original message */
    status = X400mtMsgAddStrParam (mp, X400_S_SUBJECT_IDENTIFIER, msg_id, -1);
    if ( status != X400_E_NOERROR ) return (status);

    /* 
     * X400_S_ORIGINAL_ENCODED_INFORMATION_TYPES 
     * X400_S_MESSAGE_SUBMISSION_TIME 
     * X400_S_MESSAGE_DELIVERY_TIME 
     * are read only, so don't add them 
     */

    /*  Latest Delivery Time: UTCTime format YYMMDDHHMMSS<zone> */
    status = X400mtMsgAddStrParam (mp, X400_S_LATEST_DELIVERY_TIME, latest_del_time, -1);
    if ( status != X400_E_NOERROR ) return (status);
    

#ifdef  USE_REDIRECTION_HISTORY
    /*Add redirection history for the envelope (8.3.1.2.1.5) */
    do_redi_hist_env(mp);
#endif

    do_origandl(mp);
    
    /* all OK */
    return X400_E_NOERROR;
}


static int add_env_recip_info(
    struct X400Recipient *rp
)
{
    int status;

    /* add attributes to recipient in envelope */
    status = X400mtRecipAddIntParam (rp, X400_N_RESPONSIBILITY, 1);
    if ( status != X400_E_NOERROR ) return  (status);

    status = X400mtRecipAddIntParam (rp, X400_N_MTA_REPORT_REQUEST, 3);
    if ( status != X400_E_NOERROR ) return  (status);

    status = X400mtRecipAddIntParam (rp, X400_N_REPORT_REQUEST, 2);
    if ( status != X400_E_NOERROR ) return  (status);

    status = X400mtRecipAddIntParam (rp, X400_N_ORIGINAL_RECIPIENT_NUMBER, rno);
    rno++;
    if ( status != X400_E_NOERROR ) return  (status);

    return X400_E_NOERROR;
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
    printf("\t -R : Reports (0 - never, 1 - always, 2 - always NDR \n");
    printf("\t -y : Priority (0 - normal, 1 - non-urgent, 2 - urgent \n");
    printf("\t -C : Content Type (2/22/772/OID) \n");
    printf("\t -i : Implicit conversion prohibited = TRUE \n");
    printf("\t -a : Alternate Recipient Prohibited = TRUE \n");
    printf("\t -q : Content Return Request = TRUE \n");
    printf("\t -s : Disclosure of Recipient = FALSE \n");
    printf("\t -A : Recipient Reassignment Prohibited = FALSE \n");
    printf("\t -v : Conversion with Loss Prohibited = FALSE \n");
    return;
}


static int do_redi_hist(
    struct X400Recipient *rp
)
 {
     struct X400RediHist *hist1; 
     struct X400RediHist *hist2; 
     int status;

     status = X400RediHistNew(rp,&hist1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,"Failed to allocate new trace info object \n");
	 exit(status);
     }
     
     /* Add Redirection History time */
     status =  X400RediHistAddStrParam (hist1,
					X400_S_REDIRECTION_TIME,
					"071121125704Z",
					-1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_S_REDIRECTION_TIME to Redirection Hist\n");
	 exit(status);
     }

     status =  X400RediHistAddStrParam (hist1,
					X400_S_OR_ADDRESS,
					"/cn=redihist/prmd=TestPRMD/admd=TestPRMD/C=gb",
					-1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add  X400_S_OR_ADDRESS to Redirection Hist\n");
	 exit(status);
     }

     status =  X400RediHistAddStrParam (hist1,
					X400_S_DIRECTORY_NAME,
					"CN=redihist,c=GB",
					-1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_S_DIRECTORY_NAME to Redirection Hist\n");
	 exit(status);
     }
     
     
     status = X400RediHistAddIntParam(hist1,
				      X400_N_REDIRECTION_REASON,
				      X400_RR_ALIAS);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add  X400_N_REDIRECTION_REASON to trace info\n");
	 exit(status);
     }

     /*hist2*/
     
     status = X400RediHistNew(rp,&hist2);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,"Failed to allocate new trace info object \n");
	 exit(status);
     }
     
     /* Add Redirection History time */
     status =  X400RediHistAddStrParam (hist2,
					X400_S_REDIRECTION_TIME,
					"071121125714Z",
					-1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_S_REDIRECTION_TIME to Redirection Hist\n");
	 exit(status);
     }

     status =  X400RediHistAddStrParam (hist2,
					X400_S_OR_ADDRESS,
					"/cn=redihist2/prmd=TestPRMD/admd=TestPRMD/C=gb",
					-1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add  X400_S_OR_ADDRESS to Redirection Hist\n");
	 exit(status);
     }

     status =  X400RediHistAddStrParam (hist2,
					X400_S_DIRECTORY_NAME,
					"CN=redihist2,c=GB",
					-1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_S_DIRECTORY_NAME to Redirection Hist\n");
	 exit(status);
     }
     
     
     status = X400RediHistAddIntParam(hist2,
				      X400_N_REDIRECTION_REASON,
				      X400_RR_RECIP_ASSIGNED_ALT_RECIP);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add  X400_N_REDIRECTION_REASON to "
		 "Redirection Hist\n");
	 exit(status);
     }
     return X400_E_NOERROR;
}

static int do_redi_hist_env(
    struct X400mtMessage *msg
)
 {
     struct X400RediHist *hist1; 
     struct X400RediHist *hist2; 
     int status;

     status = X400mtRediHistNewEnv(msg,&hist1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,"Failed to allocate new trace info object \n");
	 exit(status);
     }
     
     /* Add Redirection History time */
     status =  X400RediHistAddStrParam (hist1,
					X400_S_REDIRECTION_TIME,
					"071121125704Z",
					-1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_S_REDIRECTION_TIME to Redirection Hist\n");
	 exit(status);
     }

     status =  X400RediHistAddStrParam (hist1,
					X400_S_OR_ADDRESS,
					"/cn=redihist/prmd=TestPRMD/admd=TestPRMD/C=gb",
					-1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add  X400_S_OR_ADDRESS to Redirection Hist\n");
	 exit(status);
     }

     status =  X400RediHistAddStrParam (hist1,
					X400_S_DIRECTORY_NAME,
					"CN=redihist,c=GB",
					-1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_S_DIRECTORY_NAME to Redirection Hist\n");
	 exit(status);
     }
     
     
     status = X400RediHistAddIntParam(hist1,
				      X400_N_REDIRECTION_REASON,
				      X400_RR_ALIAS);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add  X400_N_REDIRECTION_REASON to trace info\n");
	 exit(status);
     }

     /*hist2*/
     
     status = X400mtRediHistNewEnv(msg,&hist2);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,"Failed to allocate new trace info object \n");
	 exit(status);
     }
     
     /* Add Redirection History time */
     status =  X400RediHistAddStrParam (hist2,
					X400_S_REDIRECTION_TIME,
					"071121125714Z",
					-1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_S_REDIRECTION_TIME to Redirection Hist\n");
	 exit(status);
     }

     status =  X400RediHistAddStrParam (hist2,
					X400_S_OR_ADDRESS,
					"/cn=redihist2/prmd=TestPRMD/admd=TestPRMD/C=gb",
					-1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add  X400_S_OR_ADDRESS to Redirection Hist\n");
	 exit(status);
     }

     status =  X400RediHistAddStrParam (hist2,
					X400_S_DIRECTORY_NAME,
					"CN=redihist2,c=GB",
					-1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_S_DIRECTORY_NAME to Redirection Hist\n");
	 exit(status);
     }
     
     
     status = X400RediHistAddIntParam(hist2,
				      X400_N_REDIRECTION_REASON,
				      X400_RR_RECIP_ASSIGNED_ALT_RECIP);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add  X400_N_REDIRECTION_REASON to "
		 "Redirection Hist\n");
	 exit(status);
     }
     return X400_E_NOERROR;
}   

static void do_origandl(
    struct X400mtMessage *msg
)
{
    struct X400ORandDL *or_and_dl1;
    struct X400ORandDL *or_and_dl2;
    
    const char *origin_or_address = "/cn=origandlorig/prmd=TestPRMD/admd=TestPRMD/C=gb/";
    const char *origin_dn_address = "CN=origandlorig,c=GB";
    int status;
    
    status = X400mtORandDLNew(msg,&or_and_dl1);
    if (status !=X400_E_NOERROR) {
        fprintf(stderr,"Failed to allocate new OR Address and DL "
                "expansion object \n");
        exit(status);
    }
     
    /* Add Origin or expansion time */
    status = X400ORandDLAddStrParam (or_and_dl1,
                                     X400_S_ORIG_OR_EXAP_TIME,
                                     "071121125704Z",
                                     -1);
    if (status != X400_E_NOERROR) {
        fprintf(stderr,
                "Failed to add X400_S_ORIG_OR_EXAP_TIME "
                "to X400ORandDL\n");
        exit(status);
    }
    
    /* originator or dl_name */

    /* Add Origin or expansion time */
    status = X400ORandDLAddStrParam (or_and_dl1,
                                     X400_S_OR_ADDRESS,
                                     origin_or_address,
                                     -1);
    if (status != X400_E_NOERROR) {
        fprintf(stderr,
                "Failed to add X400_S_OR_ADDRESS "
                "to X400ORandDL\n");
        exit(status);
    }

  
    status = X400ORandDLAddStrParam (or_and_dl1,
                                     X400_S_DIRECTORY_NAME,
                                     origin_dn_address,
                                     -1);
    if (status != X400_E_NOERROR) {
        fprintf(stderr,
                "Failed to add X400_S_DIRECTORY_NAME "
                "to X400ORandDL\n");
        exit(status);
    }
    status = X400mtORandDLNew(msg,&or_and_dl2);
    if (status !=X400_E_NOERROR) {
        fprintf(stderr,"Failed to allocate new OR Address and DL "
                "expansion object \n");
        exit(status);
    }
     
    /* Add Origin or expansion time */
    status = X400ORandDLAddStrParam (or_and_dl2,
                                     X400_S_ORIG_OR_EXAP_TIME,
                                     "091121125704Z",
                                     -1);
    if (status != X400_E_NOERROR) {
        fprintf(stderr,
                "Failed to add X400_S_ORIG_OR_EXAP_TIME "
                "to X400ORandDL\n");
        exit(status);
    }
    
    /* originator or dl_name */

    /* Add Origin or expansion time */
    status = X400ORandDLAddStrParam (or_and_dl2,
                                     X400_S_OR_ADDRESS,
                                     origin_or_address,
                                     -1);
    if (status != X400_E_NOERROR) {
        fprintf(stderr,
                "Failed to add X400_S_OR_ADDRESS "
                "to X400ORandDL\n");
        exit(status);
    }

  
    status = X400ORandDLAddStrParam (or_and_dl2,
                                     X400_S_DIRECTORY_NAME,
                                     origin_dn_address,
                                     -1);
    if (status != X400_E_NOERROR) {
        fprintf(stderr,
                "Failed to add X400_S_DIRECTORY_NAME "
                "to X400ORandDL\n");
        exit(status);
    }
}
