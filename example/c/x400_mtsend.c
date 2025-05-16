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
 * Simple example program for transferring a message into the MTA
 */

/* If you want to use raw content, remove the undef.
 * NB you will then need to put some content into <cwd>/p22
 */
#define USE_RAW_CONTENT 1
#undef USE_RAW_CONTENT

#include <stdio.h>
#include <stdlib.h>


#include <x400_mtapi.h>
#include <seclabel_api.h> /* For security labels */
#include "example.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>


/* local functions */
static int send_msgs(
    int         argc,
    char      **argv
) ;
static int send_msg(
    struct X400mtSession *sp
);
static int add_single_recip(
    struct X400mtMessage *mp
);
static int add_exempt_address(
    struct X400mtMessage *mp
);
static int add_address_list_indicator(
    struct X400mtMessage *mp
);

static int add_handling_instructions(
    struct X400mtMessage *mp
);

#ifdef add_multi_recips
static int add_multi_recips(
    struct X400mtMessage *mp
);
static int add_content_recip_info(
    struct X400Recipient     *rp,
    const char 		     *free_form_name
);
#endif
static int build_env(
    struct X400mtMessage *mp
);
static int build_content(
    struct X400mtMessage *mp
);
static int add_binary_bp(
    struct X400mtMessage *mp
);
static int add_env_recip_info(
    struct X400Recipient *rp
);
static void usage(void);

static char * set_latest_del_time(void);
static int rno = 1;
static int fwd_rno = 1;

static int load_ber(
    char *filename,
    char **output_buffer,
    int *buf_len
);
#define ADD_FWD_BODYPART 1
#ifdef ADD_FWD_BODYPART
static int add_fwd_bodypart(
    struct X400mtMessage *mp
);

static int build_fwd_env (
    struct X400Message *mp
);

static int build_fwd_content (
    struct X400Message *mp
);

static int add_fwd_recip(
    struct X400Message *mp,
    const char *oraddress,
    int type
);

static int create_fw_message(
    struct X400Message *mp
);

static int add_fwd_content_bodypart (
    struct X400mtMessage *mp
);

static int add_dl_exempted_recip_list (
    struct X400mtMessage *mp
);

#endif


/* These are the data items used */

/* The O/R addresses used are intended to be compatible with those
 * used in the quick install scripts mktailor.tcl, and createmhs.tcl.
 * (createmhs.tcl is used by x400quick).
 * Change this value to the name of your host.  */
#define HOSTNAME "dhcp-165"

/* define USE_P772_EXTS here, if you want to send a message using P772
 * military messaging extensions.
 */
#define USE_P772_EXTS  1
#ifdef USE_P772_EXTS
/* P772 extensions use an external content type */
#define WANT_EXTERNAL_CONTENT_TYPE 1
#endif
static char *optstr = "uG:O:r:g:G:c:l:R:y:C:iaqsAvf:X:S:";

/* this value is used for the originator of the message */
/* static const char orig[] = "/S=x400test-orig/OU="HOSTNAME"/O=GatewayMTA/PRMD=TestPRMD/ADMD=TestADMD/C=GB/"; */
static const char *orig;

static const char *fwd_orig;
static const char *fwd_recip;

static const char orig_ret_addr[] = "/S=x400test-orig/OU="HOSTNAME"/O=GatewayMTA/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";

/* default recipients */
/* static const char recip[] = "/S=x400test-recip/OU="HOSTNAME"/O=GatewayMTA/PRMD=TestPRMD/ADMD=TestADMD/C=GB/"; */
static char *recip;
static char *exemp_recip;

static const char recip2[] = "/S=x400test-recip2/OU="HOSTNAME"/O=GatewayMTA/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
/* This recip will cause a report to be generated */
static const char badrecip[] = "/S=x400test-recip2/OU=badhost/O=TestOrg/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
static const char auth_orig[] = "/S=x400test-auth-orig/OU="HOSTNAME"/O=TestOrg/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";

/* envelope recip values */
static const char orar[] = "/S=x400test-orig-orar/OU="HOSTNAME"/O=TestOrg/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";

static const char orardn[] = "CN=alternative,c=gb";

/* default envelope integer values */
#ifdef WANT_EXTERNAL_CONTENT_TYPE
static char *def_ext_content_type = "1.3.26.0.4406.0.4.1";
#endif
static int def_bool = 0;
static int def_priority = 2;

/* default envelope string values */
static const char def_msgid[] = "P772 MessageType identifier";
static const char def_utc[] = "050924120000";
/* Make sure the second part of msg_id (after ";") is less than 32 chars in length */
static const char msg_id[] = "/PRMD=TestPRMD/ADMD=TestADMD/C=GB/;"HOSTNAME".2810401";
static const char content_id[] = "030924.140212";
static const char fwd_msg_id[] = "/PRMD=TestFWDPRMD/ADMD=TestADMD/C=GB/;"HOSTNAME".2810401";


static const char distvalue1[] = { 0x02, 0x01, 0x0a };

static const char distvalue2[] = { 0x13, 0x0a, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a};

static const char corr_octet[] = { 0x02, 0x01, 0x0a };

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


#define XML_BUFSIZE 1024
#define STRING_BUFSIZE 1024

/*! Main function of mhssend 
 *! \return 0 on success, otherwise the value of status from X400 call
 */
int main ( 
    int         argc,
    char      **argv
) 
{
    return send_msgs (argc, argv);
}

static int send_msgs (
    int         argc,
    char      **argv
) 
{
    int status;
    struct X400mtSession *sp;

    if (get_args(argc, argv, optstr)) {
	usage();
	exit(-1);
    }

    orig = strdup(x400_default_gw_originator);
    recip = strdup(x400_default_gw_recipient);
    exemp_recip = strdup(x400_default_exempted_recipient);
    fwd_orig = strdup(x400_default_fwd_originator);
    fwd_recip = strdup(x400_default_fwd_recipient);
        
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

    /* setup logging from $(ETCDIR)x400api.xml or $(SHAREDIR)x400api.xml */
    X400mtSetStrDefault(sp, X400_S_LOG_CONFIGURATION_FILE, "x400api.xml", 0);

    /* Set the ADatP3 seperator */
    X400mtSetStrDefault(sp, X400_ADATP3_SEP,"\r\n",-1);
    
    
    while (num_msgs_to_send-- > 0)
    {
	status = send_msg(sp);
	if ( status != X400_E_NOERROR ) {
	    fprintf (stderr, "Error in send_msg: %s\n", X400mtError (status));
	    exit (status);
	}
    }

    /* close the API session */
    return X400mtClose (sp);
}

static int send_msg(
    struct X400mtSession *sp
)
{
    int status;
    struct X400mtMessage *mp;

    /* start preparing a new message */
    status = X400mtMsgNew (sp, X400_MSG_MESSAGE, &mp);
    /* Should report all errors as above */
    if ( status != X400_E_NOERROR ) exit (status);

    /* setup originator using a single string */
    printf("originator of msg is %s\n", orig);
    status = X400mtMsgAddStrParam (mp, X400_S_OR_ADDRESS, orig, -1);
    if ( status != X400_E_NOERROR ) exit (status);

    /* add various envelope and header recipients into the message */
    /* status = add_multi_recips(mp); */
    status = add_single_recip(mp);
    if ( status != X400_E_NOERROR ) exit (status);
   
    
    /* build rest of the message envelope */
    status = build_env(mp);
    if ( status != X400_E_NOERROR ) exit (status);

    /* build the message content */
    status = build_content(mp);
    if ( status != X400_E_NOERROR ) exit (status);

    /* send the message */
    status = X400mtMsgSend (mp);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "Error in MsgSend: %s\n", X400mtError (status));
	exit (status);
    }

    /* delete the message structure */
    status = X400mtMsgDelete (mp);
    if ( status != X400_E_NOERROR ) exit (status);

    return status;
}

static int add_single_recip(
    struct X400mtMessage *mp
)
{
    struct X400Recipient *rp;
    int status;

    printf("Adding single recip %s\n", recip);
    /* add new recipient to message */
    status = X400mtRecipNew (mp, X400_RECIP_STANDARD, &rp);
    if ( status != X400_E_NOERROR ) return  (status);
    /* give recip an address */
    status = X400mtRecipAddStrParam (rp, X400_S_OR_ADDRESS, recip, -1);
    if ( status != X400_E_NOERROR ) return  (status);
    /* add other values to recip */
    status = add_env_recip_info (rp);
    if ( status != X400_E_NOERROR ) return  (status);

    /* Add an X.411 DL exempted recipient list */
    status = add_dl_exempted_recip_list (mp);
    if ( status != X400_E_NOERROR ) return  (status);
    

#define USE_REDIRECTION_HISTORY 1
#ifdef  USE_REDIRECTION_HISTORY
 {
     struct X400RediHist *hist1; 
     struct X400RediHist *hist2; 
     int status;

     status = X400RediHistNew(rp,&hist1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,"Failed to allocate new redirection history object \n");
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
     
}   
#endif

#ifdef USE_P772_EXTS   
  /* Add ACP 127 Notification types */
 status = X400mtRecipAddIntParam (rp,
				  X400_N_ACP127_NOTI_TYPE,
				  X400_ACP127_NOTI_TYPE_NEG);
 if ( status != X400_E_NOERROR ) return (status);

 
 status =  X400mtRecipAddIntParam (rp,
				   X400_N_ACP127_NOTI_TYPE,
				   X400_ACP127_NOTI_TYPE_POS);
 if ( status != X400_E_NOERROR ) return (status);

 status =  X400mtRecipAddIntParam (rp,
				   X400_N_ACP127_NOTI_TYPE,
				   X400_ACP127_NOTI_TYPE_TRANS);
 if ( status != X400_E_NOERROR ) return (status);

 /*Add ACP127 Notification response */
 {
     struct X400ACP127Resp *resp = NULL;
     struct X400ALI *ali1 = NULL;
     struct X400ALI *ali2 = NULL;
     
     /* Create new ACP127 Response object */
     status = X400ACP127RespNew(rp,&resp);
     if ( status != X400_E_NOERROR ) return (status);
     
     
     /* Add Response type */
     status = X400ACP127RespAddIntParam(resp,X400_ACP127_NOTI_TYPE_NEG);
     if ( status != X400_E_NOERROR ) return (status);
     
	
     status = X400ACP127RespAddIntParam(resp,X400_ACP127_NOTI_TYPE_POS);
     if ( status != X400_E_NOERROR ) return (status);

     
     status = X400ACP127RespAddIntParam(resp,X400_ACP127_NOTI_TYPE_TRANS);
     if ( status != X400_E_NOERROR ) return (status);
     
     
     /* Add ACP127 response time*/
     status = X400ACP127RespAddStrParam (resp,
					 X400_S_ACP127_NOTI_RESP_TIME,
					 "080101120000+0100",
					 -1);
     if ( status != X400_E_NOERROR ) return (status); 
     
     
     /* Add ACP127 response recipient*/
     status = X400ACP127RespAddStrParam (resp,
					 X400_S_ACP127_NOTI_RESP_RECIPIENT,
					 "ACP127 Recipient",
					 -1);
     if ( status != X400_E_NOERROR ) return (status); 

     /* Add ACP127 supplimentry info*/
     status = X400ACP127RespAddStrParam (resp,
					 X400_S_ACP127_NOTI_RESP_SUPP_INFO,
					 "ACP127 supp info",
					 -1);
     if ( status != X400_E_NOERROR ) return (status); 
     
     /* Add ACP127 Address List Indicator */
     status = X400ACP127RespNewALI (resp,&ali1);
     if ( status != X400_E_NOERROR ) return (status); 

     status = X400ALIAddIntParam (ali1,X400_N_ALI_TYPE,0);
     if ( status != X400_E_NOERROR ) return  (status);
     
     status = X400ALIAddIntParam (ali1,X400_N_ALI_NOTIFICTAION_REQUEST,0);
     if ( status != X400_E_NOERROR ) return  (status);
     
     status = X400ALIAddIntParam (ali1,X400_N_ALI_REPLY_REQUEST,1);
     if ( status != X400_E_NOERROR ) return  (status);
     
     status = X400ALIAddStrParam (ali1, X400_S_IOB_OR_ADDRESS,
	      	  "/cn=acp127ali1/PRMD=TestPRMD/ADMD=TestADMD/C=GB", -1);
     if ( status != X400_E_NOERROR ) return  (status);
     
     status = X400ALIAddStrParam (ali1, X400_S_IOB_DN_ADDRESS,
				  "cn=acp127ali1,c=gb", -1);
     if ( status != X400_E_NOERROR ) return  (status);
     
     status = X400ALIAddStrParam (ali1, X400_S_IOB_FREE_FORM_NAME,
				  "ACP127 Mr Address List Indicator1", -1);
     if ( status != X400_E_NOERROR ) return  (status);
     
     status = X400ALIAddStrParam (ali1, X400_S_IOB_TEL,"1270123456789",-1);
     if ( status != X400_E_NOERROR ) return  (status);
     
     /*Add a second ACP127 Address List indicator */
     status = X400ACP127RespNewALI (resp,&ali2);
     if ( status != X400_E_NOERROR ) return (status); 
     
     status = X400ALIAddIntParam (ali2,X400_N_ALI_TYPE,1);
     if ( status != X400_E_NOERROR ) return  (status);
     
     status = X400ALIAddIntParam (ali2,X400_N_ALI_NOTIFICTAION_REQUEST,1);
     if ( status != X400_E_NOERROR ) return  (status);
     
     status = X400ALIAddIntParam (ali2,X400_N_ALI_REPLY_REQUEST,0);
     if ( status != X400_E_NOERROR ) return  (status);
     
     status = X400ALIAddStrParam (ali2, X400_S_IOB_OR_ADDRESS,
		 "/cn=acp127ali2/PRMD=TestPRMD/ADMD=TestADMD/C=GB", -1);
     if ( status != X400_E_NOERROR ) return  (status);
     
     status = X400ALIAddStrParam (ali2, X400_S_IOB_DN_ADDRESS,
				  "cn=acp127ali2,c=gb", -1);
     if ( status != X400_E_NOERROR ) return  (status);
     
     status = X400ALIAddStrParam (ali2, X400_S_IOB_FREE_FORM_NAME,
				  "ACP127 Mr Address List Indicator2", -1);
     if ( status != X400_E_NOERROR ) return  (status);
     
     status = X400ALIAddStrParam (ali2, X400_S_IOB_TEL,"1270123456789",-1);
     if ( status != X400_E_NOERROR ) return  (status);
     
 }
 
#endif
    
    return X400_E_NOERROR;
}

static int add_exempt_address(
    struct X400mtMessage *mp
)
{
    struct X400Recipient *rp;
    struct X400Recipient *rp2;
    int status;
     printf("Adding exempted recip %s\n", exemp_recip);
    /* add new recipient to message */
    status = X400mtRecipNew (mp, X400_EXEMPTED_ADDRESS, &rp);
    if ( status != X400_E_NOERROR ) return  (status);
    /* give exempted recip an address */
    status = X400mtRecipAddStrParam (rp, X400_S_IOB_OR_ADDRESS,
				     exemp_recip, -1);
    if ( status != X400_E_NOERROR ) return  (status);

    status = X400mtRecipAddStrParam (rp, X400_S_IOB_DN_ADDRESS,
				     "cn=exemprecip,c=gb", -1);
    if ( status != X400_E_NOERROR ) return  (status);

    status = X400mtRecipAddStrParam (rp, X400_S_IOB_FREE_FORM_NAME,
				     "Mr Exempted", -1);
    if ( status != X400_E_NOERROR ) return  (status);

    status = X400mtRecipAddStrParam (rp, X400_S_IOB_TEL,
				     "0123456789", -1);
    if ( status != X400_E_NOERROR ) return  (status);

    /* Add second exempted recipient */
    status = X400mtRecipNew (mp, X400_EXEMPTED_ADDRESS, &rp2);
    if ( status != X400_E_NOERROR ) return  (status);
    
    status = X400mtRecipAddStrParam (rp2, X400_S_IOB_OR_ADDRESS,
				     "/CN=exemp2/PRMD=TestPRMD/ADMD=TestADMD/C=GB", -1);
    if ( status != X400_E_NOERROR ) return  (status);
    
    return X400_E_NOERROR;
}
static int add_address_list_indicator(
    struct X400mtMessage *mp
)
{
    struct X400ALI *ali  = NULL;
    struct X400ALI *ali2 = NULL;
    int status;
    
    printf("Adding address list indicators\n");
    
    /* add new Address List Indicator to message */
    status = X400mtALINew (mp,&ali);
    if ( status != X400_E_NOERROR ) return  (status);
    
    status = X400ALIAddIntParam (ali,X400_N_ALI_TYPE,0);
    if ( status != X400_E_NOERROR ) return  (status);

    status = X400ALIAddIntParam (ali,X400_N_ALI_NOTIFICTAION_REQUEST,0);
    if ( status != X400_E_NOERROR ) return  (status);
    
    status = X400ALIAddIntParam (ali,X400_N_ALI_REPLY_REQUEST,1);
    if ( status != X400_E_NOERROR ) return  (status);
    
    status = X400ALIAddStrParam (ali, X400_S_IOB_OR_ADDRESS,
				     "/cn=ALI1/PRMD=TestPRMD/ADMD=TestADMD/C=GB", -1);
    if ( status != X400_E_NOERROR ) return  (status);

    status = X400ALIAddStrParam (ali, X400_S_IOB_DN_ADDRESS,
				     "cn=ali1,c=gb", -1);
    if ( status != X400_E_NOERROR ) return  (status);

    status = X400ALIAddStrParam (ali, X400_S_IOB_FREE_FORM_NAME,
				     "Mr Address List Indicator1", -1);
    if ( status != X400_E_NOERROR ) return  (status);

    status = X400ALIAddStrParam (ali, X400_S_IOB_TEL,"0123456789", -1);
    if ( status != X400_E_NOERROR ) return  (status);

    /* Add second exempted recipient */
    status = X400mtALINew (mp, &ali2);
    if ( status != X400_E_NOERROR ) return  (status);
    
    status = X400ALIAddStrParam (ali2, X400_S_IOB_OR_ADDRESS,
				 "/CN=ALI2/PRMD=TestPRMD/ADMD=TestADMD/C=GB", -1);
    if ( status != X400_E_NOERROR ) return  (status);
    
    status = X400ALIAddIntParam (ali2,X400_N_ALI_TYPE,1);
    if ( status != X400_E_NOERROR ) return  (status);
    status = X400ALIAddStrParam (ali2, X400_S_IOB_OR_ADDRESS,
				     "/cn=ALI2/PRMD=TestPRMD/ADMD=TestADMD/C=GB", -1);
    if ( status != X400_E_NOERROR ) return  (status);

    status = X400ALIAddStrParam (ali2, X400_S_IOB_DN_ADDRESS,
				     "cn=ali2,c=gb", -1);
    if ( status != X400_E_NOERROR ) return  (status);
    
   
    
    return X400_E_NOERROR;
}

static int add_handling_instructions(
    struct X400mtMessage *mp
)
{
    struct X400PSS *pss  = NULL; /* Printable String Sequence object */
    struct X400PSS *pss2 = NULL;
    int status;
    
    printf("Adding handling instructions\n");
    
    /* add new Address List Indicator to message */
    status = X400mtPSSNew (mp,X400_S_HANDLING_INSTRUCTIONS,&pss);
    if ( status != X400_E_NOERROR ) return  (status);
    
    
    status = X400PSSAddStrParam (pss,
				 "Handling instruction1",
				 -1);
    if ( status != X400_E_NOERROR ) return  (status);


    status = X400mtPSSNew (mp,X400_S_HANDLING_INSTRUCTIONS,&pss2);
    if ( status != X400_E_NOERROR ) return  (status);
    
    
    status = X400PSSAddStrParam (pss2,
				 "Handling instruction2",
				 -1);
    if ( status != X400_E_NOERROR ) return  (status);
    
    
    return X400_E_NOERROR;
}

static int add_message_instructions(
    struct X400mtMessage *mp
)
{
    struct X400PSS *pss  = NULL; /* Printable String Sequence object */
    struct X400PSS *pss2 = NULL;
    int status;
    
    printf("Adding Message instructions\n");
    
    /* add new Address List Indicator to message */
    status = X400mtPSSNew (mp,X400_S_MESSAGE_INSTRUCTIONS,&pss);
    if ( status != X400_E_NOERROR ) return  (status);
    
    
    status = X400PSSAddStrParam (pss,
				 "Message instruction1",
				 -1);
    if ( status != X400_E_NOERROR ) return  (status);


    status = X400mtPSSNew (mp,X400_S_MESSAGE_INSTRUCTIONS,&pss2);
    if ( status != X400_E_NOERROR ) return  (status);
    
    
    status = X400PSSAddStrParam (pss2,
				 "Message instruction2",
				 -1);
    if ( status != X400_E_NOERROR ) return  (status);
    
    
    return X400_E_NOERROR;
}

static int add_dist_codes_sic(
    struct X400mtMessage *mp
)
{
    struct X400PSS *pss  = NULL; /* Printable String Sequence object */
    struct X400PSS *pss2 = NULL;
    struct X400DistField *distfield1 = NULL;
    struct X400DistField *distfield2 = NULL;
    int status;
    
    printf("Adding Distribution Codes\n");
    
    /* Add new SIC */
    status = X400mtPSSNew (mp,X400_S_DIST_CODES_SIC,&pss);
    if ( status != X400_E_NOERROR ) return  (status);
    
    
    status = X400PSSAddStrParam (pss,
				 "dist code 1",
				 -1);
    if ( status != X400_E_NOERROR ) return  (status);

    /* Add second SIC */
    status = X400mtPSSNew (mp,X400_S_DIST_CODES_SIC,&pss2);
    if ( status != X400_E_NOERROR ) return  (status);
    
    
    status = X400PSSAddStrParam (pss2,
				 "dist code 2",
				 -1);
    if ( status != X400_E_NOERROR ) return  (status);
    
    /* Add new Distribution Extension Field */
    status = X400mtDistFieldNew (mp,&distfield1);
    if ( status != X400_E_NOERROR ) return  (status);
    
    
    status = X400DistFieldAddStrParam (distfield1,
				       X400_S_DIST_CODES_EXT_OID,
				       "1.2.3.4.5",
				       -1);
    if ( status != X400_E_NOERROR ) return  (status);

    status = X400DistFieldAddStrParam (distfield1,
				       X400_S_DIST_CODES_EXT_VALUE,
				       distvalue1, 3);
    if ( status != X400_E_NOERROR ) return  (status);

    /* Add second SIC */
     status = X400mtDistFieldNew (mp,&distfield2);
    if ( status != X400_E_NOERROR ) return  (status);
    
    
    status = X400DistFieldAddStrParam (distfield2,
				       X400_S_DIST_CODES_EXT_OID,
				       "1.2.3.4.5.6",
				       -1);
    if ( status != X400_E_NOERROR ) return  (status);

    status = X400DistFieldAddStrParam (distfield2,
				       X400_S_DIST_CODES_EXT_VALUE,
				       distvalue2, 12);
    if ( status != X400_E_NOERROR ) return  (status);

    
    return X400_E_NOERROR;
}

static int add_other_recipient_indicator(
    struct X400mtMessage *mp
)
{
    struct X400OtherRecip *recip  = NULL; 
    struct X400OtherRecip *recip2 = NULL;
    int status;
    
    printf("Adding Other Recipient Indicators\n");
    
    /* Add new Other recipient */
    status = X400mtOtherRecipNew (mp,&recip);
    if ( status != X400_E_NOERROR ) return  (status);
    
    
    status = X400OtherRecipAddStrParam (recip,"other recip 1",-1);
    if ( status != X400_E_NOERROR ) return  (status);

    
    status = X400OtherRecipAddIntParam (recip,1);
    if ( status != X400_E_NOERROR ) return  (status);

    
    /* Add second Other recipient */
    status = X400mtOtherRecipNew (mp,&recip2);
    if ( status != X400_E_NOERROR ) return  (status);
    
    
    status = X400OtherRecipAddStrParam (recip2,"other recip 2",-1);
    if ( status != X400_E_NOERROR ) return  (status);

    
    status = X400OtherRecipAddIntParam (recip2,0);
    if ( status != X400_E_NOERROR ) return  (status);
    
    
    
    return X400_E_NOERROR;
}
#ifdef add_multi_recips
static int add_multi_recips(
    struct X400mtMessage *mp
)
{
    struct X400Recipient *rp;
    int status;

    printf("Adding multiple recips\n");
    /* add new recipient to message */
    status = X400mtRecipNew (mp, X400_RECIP_STANDARD, &rp);
    if ( status != X400_E_NOERROR ) return  (status);
    /* give recip an address */
    status = X400mtRecipAddStrParam (rp, X400_S_OR_ADDRESS, recip, -1);
    if ( status != X400_E_NOERROR ) return  (status);
    /* add other values to recip */
    status = add_env_recip_info (rp);
    if ( status != X400_E_NOERROR ) return  (status);

    /* add second recipient to message */
    status = X400mtRecipNew (mp, X400_RECIP_STANDARD, &rp);
    if ( status != X400_E_NOERROR ) exit (status);
    /* give recip an address */
    status = X400mtRecipAddStrParam (rp, X400_S_OR_ADDRESS, recip2, -1);
    if ( status != X400_E_NOERROR ) return  (status);
    status = X400mtRecipAddIntParam (rp, X400_N_ORIGINAL_RECIPIENT_NUMBER, rno);
    rno++;
    if ( status != X400_E_NOERROR ) return  (status);

    /* add bad recipient to message */
    status = X400mtRecipNew (mp, X400_RECIP_STANDARD, &rp);
    if ( status != X400_E_NOERROR ) exit (status);
    /* give recip an address */
    status = X400mtRecipAddStrParam (rp, X400_S_OR_ADDRESS, badrecip, -1);
    if ( status != X400_E_NOERROR ) return  (status);
    status = X400mtRecipAddIntParam (rp, X400_N_ORIGINAL_RECIPIENT_NUMBER, rno);
    rno++;
    if ( status != X400_E_NOERROR ) return  (status);

    /* add cc recipient to message */
    status = X400mtRecipNew (mp, X400_RECIP_CC, &rp);
    if ( status != X400_E_NOERROR ) exit (status);
    /* give recip an address */
    status = X400mtRecipAddStrParam (rp, X400_S_OR_ADDRESS, recip, -1);
    if ( status != X400_E_NOERROR ) return  (status);
    /* add other values to recip */
    status = add_content_recip_info (rp, recip);
    if ( status != X400_E_NOERROR ) return  (status);

    /* add second cc recipient to message */
    status = X400mtRecipNew (mp, X400_RECIP_CC, &rp);
    if ( status != X400_E_NOERROR ) exit (status);
    /* give recip an address */
    status = X400mtRecipAddStrParam (rp, X400_S_OR_ADDRESS, recip2, -1);
    if ( status != X400_E_NOERROR ) return  (status);
    /* add other values to recip */
    status = add_content_recip_info (rp, recip2);
    if ( status != X400_E_NOERROR ) return  (status);

    /* add bcc recipient to message */
    status = X400mtRecipNew (mp, X400_RECIP_BCC, &rp);
    if ( status != X400_E_NOERROR ) exit (status);
    /* give recip an address */
    status = X400mtRecipAddStrParam (rp, X400_S_OR_ADDRESS, recip2, -1);
    if ( status != X400_E_NOERROR ) return  (status);

    /* add reply recipient to message */
    status = X400mtRecipNew (mp, X400_RECIP_REPLY, &rp);
    if ( status != X400_E_NOERROR ) exit (status);
    /* give recip an address */
    status = X400mtRecipAddStrParam (rp, X400_S_OR_ADDRESS, recip2, -1);
    if ( status != X400_E_NOERROR ) return  (status);

    /* add auth user to message */
    status = X400mtRecipNew (mp, X400_AUTH_ORIG, &rp);
    if ( status != X400_E_NOERROR ) return (status);
    /* give auth user an address */
    status = X400mtRecipAddStrParam (rp, X400_S_OR_ADDRESS, auth_orig, -1);
    if ( status != X400_E_NOERROR ) return  (status);

    return X400_E_NOERROR;
}
#endif

static int build_env(
    struct X400mtMessage *mp
)
{
    int status;

    /* Envelope Attributes */

#ifdef WANT_EXTERNAL_CONTENT_TYPE
    status = X400mtMsgAddStrParam (mp, X400_S_EXTERNAL_CONTENT_TYPE, 
				   def_ext_content_type, -1);
    if ( status != X400_E_NOERROR ) return (status);
#endif
    
#ifdef WANT_DL_EXP_HIST
    /*Add a DL expansion history*/
    {
	struct X400DLExpHist *hist;
	status = X400mtDLExpHistNew (mp, &hist);
	if ( status != X400_E_NOERROR ) return (status);

	status = X400DLExpHistAddStrParam (hist,
					   X400_S_OR_ADDRESS,
					   x400_default_gw_dlexphist1_or,
					   -1);
	if ( status != X400_E_NOERROR ) return  (status);

	status = X400DLExpHistAddStrParam (hist,
					   X400_S_DIRECTORY_NAME,
					   x400_default_gw_dlexphist1_dn,
					   -1);
	if ( status != X400_E_NOERROR ) return  (status);
	
	status = X400DLExpHistAddStrParam (hist,
					   X400_S_DLEXP_TIME,
					   "070801120000+0100",
					   -1);
	if ( status != X400_E_NOERROR ) return  (status);

	
	status = X400mtDLExpHistNew (mp, &hist);
	if ( status != X400_E_NOERROR ) return (status);

	status = X400DLExpHistAddStrParam (hist,
					   X400_S_OR_ADDRESS,
					   x400_default_gw_dlexphist2_or,
					   -1);
	if ( status != X400_E_NOERROR ) return  (status);

	status = X400DLExpHistAddStrParam (hist,
					   X400_S_DIRECTORY_NAME,
					   x400_default_gw_dlexphist2_dn,
					   -1);
	if ( status != X400_E_NOERROR ) return  (status);
	
	status = X400DLExpHistAddStrParam (hist,
					   X400_S_DLEXP_TIME,
					   "070801120000+0100",
					   -1);
	if ( status != X400_E_NOERROR ) return  (status);
	
	
    }
#endif
    

#ifdef USE_SEC_LABEL
    {

	/*If you are going to use security labels,
	 *make sure the following variable points to the relevant xml file
	 */
	const char* xml_filename = "seclabel.xml";
	char xml_content[XML_BUFSIZE];
	char str_content[STRING_BUFSIZE];
	int str_len = STRING_BUFSIZE;
	FILE *fd = NULL;
	
	/* Read in the security label XML file */
	fd = fopen(xml_filename,"r");
	if(fd == NULL) {
	    fprintf(stderr,"Failed to open %s : %s\n",
		    xml_filename,strerror(errno));
	}
	
	fread(&xml_content,XML_BUFSIZE,1,fd);

	fclose(fd);

	status = SecLabelInit("Example program");
	if (status != SECLABEL_E_NOERROR) {
	    fprintf(stderr, "SecLabelInit returned error %d\n", status);
	    exit(1);
	}
	
	/* Use SecLabelParse to turn the XML into an octet string */
	status = SecLabelParse(xml_content,
			       str_content,
			       STRING_BUFSIZE,
			       &str_len);
	
	if (status != SECLABEL_E_NOERROR) {
	    fprintf(stderr, "SecLabelParse returned error %d\n", status);
	    exit(1);
	}
	    
	/* Add the octet string to the message pointer */
	status = X400mtMsgAddStrParam (mp, X400_S_SECURITY_LABEL,
				       str_content,str_len);
	if ( status != X400_E_NOERROR ) {
	    fprintf (stderr, "x400mtMsgAddStrParam returned error: %s\n", X400mtError (status));
	    exit (status);
	}
	
    }
#endif

#define ADD_TRACE_INFO 1
#ifdef ADD_TRACE_INFO
 {
     struct X400TraceInfo *info1; /*Will contain all trace information */
     struct X400TraceInfo *info2; /*Will only contain mandatory trace information*/
     int status;

     status = X400mtTraceInfoNew(mp,&info1,X400_TRACE_INFO);
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

     /* Add optional*/
     status =  X400TraceInfoAddStrParam (info1,
					 X400_S_DSI_AA_CEIT,
					 "ia5-text",
					 -1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_S_DSI_AA_CEIT to trace info\n");
	 exit(status);
     }
     
     status = X400TraceInfoAddIntParam(info1,
				       X400_N_DSI_ROUTING_ACTION,
				       1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_N_DSI_ROUTING_ACTION to trace info\n");
	 exit(status);
     }

     status = X400TraceInfoAddIntParam(info1,
				       X400_N_DSI_AA_REDIRECTED,
				       1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_N_DSI_AA_REDIRECTED to trace info\n");
	 exit(status);
     }

     status = X400TraceInfoAddIntParam(info1,
				       X400_N_DSI_AA_DLOPERATION,
				       1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_N_DSI_AA_DLOPERATION to trace info\n");
	 exit(status);
     }

     /* Trace 2 */
     
     status = X400mtTraceInfoNew(mp,&info2,X400_TRACE_INFO);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,"Failed to allocate new trace info object \n");
	 exit(status);
     }
     
     
     status =  X400TraceInfoAddStrParam (info2,
					 X400_S_GLOBAL_DOMAIN_ID,
					 "/PRMD=fluffy/ADMD=TestADMD/C=GB/",
					 -1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_S_GLOBAL_DOMAIN_ID to trace info\n");
	 exit(status);
     }

     status =  X400TraceInfoAddStrParam (info2,
					 X400_S_DSI_ARRIVAL_TIME,
					 "071121125704Z",
					 -1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_S_DSI_ARRIVAL_TIME to trace info\n");
	 exit(status);
     }
     
     status = X400TraceInfoAddIntParam(info2,
				       X400_N_DSI_AA_REDIRECTED,
				       1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_N_DSI_AA_REDIRECTED to trace info\n");
	 exit(status);
     }

     status = X400TraceInfoAddIntParam(info2,
				       X400_N_DSI_AA_DLOPERATION,
				       1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_N_DSI_AA_DLOPERATION to trace info\n");
	 exit(status);
     }
     
     
 }
#endif

#define USE_INTERNAL_TRACE_INFO 1
#ifdef  USE_INTERNAL_TRACE_INFO
{
    struct X400InternalTraceInfo *int_info1;
    int status;
    
    status = X400mtInternalTraceInfoNew(mp,&int_info1);
    if (status !=X400_E_NOERROR) {
	fprintf(stderr,"Failed to allocate new trace info object \n");
	exit(status);
    }

    /*GDI*/
    status =  X400InternalTraceInfoAddStrParam (int_info1,
						X400_S_GLOBAL_DOMAIN_ID,
						"/PRMD=intprmd/ADMD=TestADMD/C=GB/",
						-1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_S_GLOBAL_DOMAIN_ID to trace info\n");
	 exit(status);
     }
    
    /*MTA Name*/
     status =  X400InternalTraceInfoAddStrParam (int_info1,
						 X400_S_MTA_NAME,
						 "MTA test MTA1",
						-1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_S_MTA_NAME to trace info\n");
	 exit(status);
     }
     
    /*MTA Supplied info*/

     status =  X400InternalTraceInfoAddStrParam (int_info1,
						 X400_S_MTA_SI_TIME,
						 "081121125704Z",
						 -1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_S_MTA_SI_TIME to trace info\n");
	 exit(status);
     }
     
     /*Routing Action could be X400_MTA_SI_ROUTING_ACTION_RELAYED or
      * X400_MTA_SI_ROUTING_ACTION_REROUTED*/
     status = X400InternalTraceInfoAddIntParam(int_info1,
					       X400_N_MTA_SI_ROUTING_ACTION,
					       X400_MTA_SI_RA_MTA);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add  X400_N_MTA_SI_ROUTING_ACTION to trace info\n");
	 exit(status);
     }


     status =  X400InternalTraceInfoAddStrParam (int_info1,
						 X400_S_MTA_SI_ATTEMPTED_MTA,
						 "Attempted MTA name 1",
						 -1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_S_MTA_SI_ATTEMPTED_MTA to trace info\n");
	 exit(status);
     }

     /*
       You can either set the Attempted MTA or the Attempted Domain, but not
       both*/
     /*
     status =  X400InternalTraceInfoAddStrParam (int_info1,
						 X400_S_MTA_SI_ATTEMPTED_DOMAIN,
						 "/PRMD=int_attmped_prmd/ADMD=TestADMD/C=GB/",
						 -1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_S_MTA_SI_ATTEMPTED_DOMAIN to trace info\n");
	 exit(status);
     }

     */
     status =  X400InternalTraceInfoAddStrParam (int_info1,
						 X400_S_MTA_SI_DEFERRED_TIME,
						 "081121125714Z",
						 -1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_S_MTA_SI_DEFERRED_TIME to trace info\n");
	 exit(status);
     }
     
     status =  X400InternalTraceInfoAddStrParam (int_info1,
						 X400_S_MTA_SI_CEIT,
						 "ia5-text",
						 -1);
     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_S_MTA_SI_DEFERRED_TIME to trace info\n");
	 exit(status);
     }

     status = X400InternalTraceInfoAddIntParam(int_info1,
					       X400_N_MTA_SI_OTHER_ACTIONS,
					       X400_MTA_SI_OTHER_ACTION_REDIRECTED);

     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_N_MTA_SI_OTHER_ACTIONS to trace info\n");
	 exit(status);
     }
     
     status = X400InternalTraceInfoAddIntParam(int_info1,
					       X400_N_MTA_SI_OTHER_ACTIONS,
					       X400_MTA_SI_OTHER_ACTION_DLOPERATION);

     if (status !=X400_E_NOERROR) {
	 fprintf(stderr,
		 "Failed to add X400_N_MTA_SI_OTHER_ACTIONS to trace info\n");
	 exit(status);
     }


     
}
#endif
 

 

    
    
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

    /* 
     * X400_S_ORIGINAL_ENCODED_INFORMATION_TYPES 
     * X400_S_MESSAGE_SUBMISSION_TIME 
     * X400_S_MESSAGE_DELIVERY_TIME 
     * are read only, so don't add them 
     */
    {
	char *latest_time = set_latest_del_time();
	/*  Latest Delivery Time: UTCTime format YYMMDDHHMMSS<zone> */
	status = X400mtMsgAddStrParam (mp, X400_S_LATEST_DELIVERY_TIME, latest_time, -1);
	free(latest_time);
	if ( status != X400_E_NOERROR ) return (status);
    
    }
    
    /* Originator Return Address (X.400 String format) */
    status = X400mtMsgAddStrParam (mp, X400_S_ORIGINATOR_RETURN_ADDRESS, 
				   orig_ret_addr, -1);
    if ( status != X400_E_NOERROR ) return (status);

     /* Content Correlator */
#define CC_IA5TEXT
#ifdef CC_IA5TEXT
    status = X400mtMsgAddStrParam (mp, X400_S_CONTENT_CORRELATOR_IA5_STRING, 
				   "ABCDEFGH", -1);
    if ( status != X400_E_NOERROR ) return (status);
#else
    status = X400mtMsgAddStrParam (mp, X400_S_CONTENT_CORRELATOR_OCTET_STRING, 
				   corr_octet, 3);
    if ( status != X400_E_NOERROR ) return (status);
#endif
    
    
    /* all OK */
    return X400_E_NOERROR;
}


static int build_content(
    struct X400mtMessage *mp
)
{
    int status;
#ifdef USE_RAW_CONTENT
#ifdef USE_STRING
    int fd;
    char buf[10 * 1024];
    int len;

    fd = open("./p22", O_RDONLY);
    if (fd == -1) {
	printf("Failed to open p22 file in cwd");
	return X400_E_SYSERROR;
    }

    len = read(fd, buf, 10 * 1024);
    close(fd);
    
    if (len == 10 * 1024) {
	printf("p22 file too long to read");
	return X400_E_SYSERROR;
    }

    status = X400mtMsgAddStrParam (mp, X400_S_CONTENT_STRING, buf , len);
    if ( status != X400_E_NOERROR ) return (status);
#else
    status = X400mtMsgAddStrParam (mp, X400_S_CONTENT_FILENAME, "./p22", 5);
    if ( status != X400_E_NOERROR ) return (status);
#endif
#else
    /* content string params */
    status = X400mtMsgAddStrParam (mp, X400_S_IPM_IDENTIFIER, ipm_id, -1);
    if ( status != X400_E_NOERROR ) return (status);

    status = X400mtMsgAddStrParam (mp, X400_S_SUBJECT, subject, -1);
    if ( status != X400_E_NOERROR ) return (status);

    status = X400mtMsgAddStrParam (mp, X400_S_REPLIED_TO_IDENTIFIER, ipm_rep_id, -1);
    if ( status != X400_E_NOERROR ) return (status);

    status = X400mtMsgAddStrParam (mp, X400_S_OBSOLETED_IPMS, ipm_obs_id, -1);
    if ( status != X400_E_NOERROR ) return (status);

    status = X400mtMsgAddStrParam (mp, X400_S_RELATED_IPMS, ipm_rel_id, -1);
    if ( status != X400_E_NOERROR ) return (status);

    status = X400mtMsgAddStrParam (mp, X400_S_EXPIRY_TIME, def_utc, -1);
    if ( status != X400_E_NOERROR ) return (status);

    status = X400mtMsgAddStrParam (mp, X400_S_REPLY_TIME, def_utc, -1);
    if ( status != X400_E_NOERROR ) return (status);

#ifdef X400_1999_SUPPORTED
    /* we don't support these yet */
    status = X400mtMsgAddStrParam (mp, X400_S_AUTHORIZATION_TIME, def_utc, -1);
    if ( status != X400_E_NOERROR ) return (status);

    status = X400mtMsgAddStrParam (mp, X400_S_ORIGINATORS_REFERENCE, orig_ref, -1);
    if ( status != X400_E_NOERROR ) return (status);
#endif

    /* content int params */
    status = X400mtMsgAddIntParam (mp, X400_N_IMPORTANCE, importance);
    if ( status != X400_E_NOERROR ) return (status);

    status = X400mtMsgAddIntParam (mp, X400_N_SENSITIVITY, sensitivity);
    if ( status != X400_E_NOERROR ) return (status);

    status = X400mtMsgAddIntParam (mp, X400_N_AUTOFORWARDED, autoforwarded);
    if ( status != X400_E_NOERROR ) return (status);

    /* now an IA5 body part */
    status = X400mtMsgAddStrParam (mp, X400_T_IA5TEXT, text, -1);
    if ( status != X400_E_NOERROR ) return (status);

    /* now a binary body part */
    status = add_binary_bp(mp);
    if ( status != X400_E_NOERROR ) return (status);

    
#ifdef ADD_FWD_BODYPART
    /* Now add a forwarded bodypart (So envelope + message)*/
    status = add_fwd_bodypart(mp);
    if ( status != X400_E_NOERROR ) return (status);
    
    /* Now add a forwarded content bodypart (so just the message content)
     * This is specified within X.420
     * */
    status = add_fwd_content_bodypart(mp);
    if ( status != X400_E_NOERROR ) return (status);
    
#endif
    
#endif
    
#ifdef USE_P772_EXTS    
    status = add_exempt_address(mp);
    if ( status != X400_E_NOERROR ) exit (status);
    
    /* Add extended Auth Info */
    status = X400mtMsgAddStrParam (mp, X400_S_EXT_AUTH_INFO, 
				   def_utc, -1);
    if ( status != X400_E_NOERROR ) return (status);

    /* Add Message Coderess */
    status = X400mtMsgAddIntParam (mp, X400_N_EXT_CODRESS, 0);
    if ( status != X400_E_NOERROR ) return (status);

    /* Add P772 Message Type "type" field */
    status = X400mtMsgAddIntParam (mp, X400_N_EXT_MSG_TYPE, 0);
    if ( status != X400_E_NOERROR ) return (status);

    /* Add P772 MessageType "identifier" field*/
    status = X400mtMsgAddStrParam (mp, X400_S_EXT_MSG_IDENTIFIER, 
				   def_msgid, -1);
    if ( status != X400_E_NOERROR ) return (status);

    /* Add Primary Precedence */
    status = X400mtMsgAddIntParam (mp, X400_N_EXT_PRIM_PREC, 5);
    if ( status != X400_E_NOERROR ) return (status);

    /* Add Copy Precedence */
    status = X400mtMsgAddIntParam (mp, X400_N_EXT_COPY_PREC, 4);
    if ( status != X400_E_NOERROR ) return (status);

    /* Add Address List Indicator */
    status = add_address_list_indicator(mp);
    if ( status != X400_E_NOERROR ) exit (status);

    /* Add Handling instructions */
    status = add_handling_instructions(mp);
    if ( status != X400_E_NOERROR ) exit (status);
    
    /* Add Message instructions */
    status = add_message_instructions(mp);
    if ( status != X400_E_NOERROR ) exit (status);

    /* Add Distribution codes SIC */
    status = add_dist_codes_sic(mp);
    if ( status != X400_E_NOERROR ) exit (status);
    
    /* Add Originator Ref */
    status = X400mtMsgAddStrParam (mp,X400_S_ORIG_REF,"orig_ref", -1);
    if ( status != X400_E_NOERROR ) exit (status);
    
    /* Add Originator PLAD */
    status = X400mtMsgAddStrParam (mp,X400_S_ORIG_PLAD,"orig_plad", -1);
    if ( status != X400_E_NOERROR ) exit (status);

    /* Add Other Recipient Indicator */
    status = add_other_recipient_indicator(mp);
    if ( status != X400_E_NOERROR ) exit (status);

    /* Add ACP127 Message identifier */
    status = X400mtMsgAddStrParam (mp,X400_S_ACP127_MSG_ID,"ACP127 msg id", -1);
    if ( status != X400_E_NOERROR ) exit (status);
    
    /* Add Pilot Forwarding information */
    {
	/* This is treated a a binary blob.
	 * You need to pass in the correct BER encoded series of bytes.
	 * 
	 */
	int buf_len = 0;
	char *pilot_ber = NULL;

	if (load_ber("pilot_fwd_info.ber",&pilot_ber,&buf_len) == 0) {
	    printf("Adding pilot forwarding info\n");
	    status = X400mtMsgAddStrParam (mp,X400_S_PILOT_FWD_INFO,
					   pilot_ber,buf_len);
            free(pilot_ber);
	    if ( status != X400_E_NOERROR ) exit (status);
	} else {
	    printf("unable to add pilot forwarding info\n");
	}
	
    }

   




    /* Add Information Security Label. (This is the P772 / STANAG 4406 A1.16)
     * */
    {
	/* This is treated a a binary blob.
	 * You need to pass in the correct BER encoded series of bytes.
	 */
	int buf_len = 0;
	char *sec_label_ber = NULL;

	if (load_ber("info_sec_label.ber",&sec_label_ber,&buf_len) == 0) {
	    printf("Adding information security label\n");
	    status = X400mtMsgAddStrParam (mp,X400_S_INFO_SEC_LABEL,
					   sec_label_ber,buf_len);
            free(sec_label_ber);
	    if ( status != X400_E_NOERROR ) exit (status);
	} else {
	    printf("unable to add information security labels\n");
	}
	
    }

    
    /* Add P772 ADatP3 object */
    {
	struct X400Bodypart *ADatP3 = NULL;
	
	status = X400BodypartNew(X400_T_ADATP3,&ADatP3);
	if (status != X400_E_NOERROR) return status;
	
	status = X400mtMsgAddBodypart(mp,ADatP3);
	if (status != X400_E_NOERROR) return status;
        
	status = X400BodypartAddIntParam(ADatP3,X400_N_ADATP3_PARM,1);
	if (status != X400_E_NOERROR) return status;

	status = X400BodypartAddIntParam(ADatP3,X400_N_ADATP3_CHOICE,0);
	if (status != X400_E_NOERROR) return status;
	
	status = X400BodypartAddStrParam (ADatP3,X400_S_ADATP3_DATA,
                                          "The first ADatP3 Data value",
                                          -1);
	if (status != X400_E_NOERROR) return status;
	
    }
    
    /* Add P772 ADatP3 object with a sequence of IA5Strings */
    {
	struct X400Bodypart *ADatP3 = NULL;
	
	status = X400BodypartNew(X400_T_ADATP3,&ADatP3);
	if (status != X400_E_NOERROR) return status;
	
	status = X400mtMsgAddBodypart(mp,ADatP3);
	if (status != X400_E_NOERROR) return status;
        
	status = X400BodypartAddIntParam(ADatP3,X400_N_ADATP3_PARM,2);
	if (status != X400_E_NOERROR) return status;

	status = X400BodypartAddIntParam(ADatP3,X400_N_ADATP3_CHOICE,1);
	if (status != X400_E_NOERROR) return status;
	
	status = X400BodypartAddStrParam (ADatP3,X400_S_ADATP3_DATA,
                                          "The second ADatP3 Data value\r\n"
                                          "The third ADatP3 Data value\r\n"
                                          "The fourth ADatP3 Data value\r\n",
                                          -1);
	if (status != X400_E_NOERROR) return status;
    }

    /* Add new P772 Corrections bodypart */

    {
        struct X400Bodypart *corrections = NULL;
        
        status = X400BodypartNew(X400_T_CORRECTIONS,&corrections);
        if (status != X400_E_NOERROR) return status;
        
        status = X400mtMsgAddBodypart(mp,corrections);
	if (status != X400_E_NOERROR) return status;

        status = X400BodypartAddIntParam(corrections,X400_N_CORREC_PARM,1);
	if (status != X400_E_NOERROR) return status;

        status = X400BodypartAddStrParam(corrections,
                                         X400_S_CORREC_DATA,
                                         "A simple P772 correction",
                                         -1);
        if (status != X400_E_NOERROR) return status;
            
    }

    /* Add new ACP127Data bodypart */
    {
        struct X400Bodypart *acp127data = NULL;
        
        status = X400BodypartNew(X400_T_ACP127DATA,&acp127data);
        if (status != X400_E_NOERROR) return status;
        
        status = X400mtMsgAddBodypart(mp,acp127data);
	if (status != X400_E_NOERROR) return status;

        status = X400BodypartAddIntParam(acp127data,X400_N_ACP127DATA_PARM,1);
	if (status != X400_E_NOERROR) return status;

        status = X400BodypartAddStrParam(acp127data,
                                         X400_S_ACP127_DATA,
                                         "A simple P772 ACP127 Data IA5Text",
                                         -1);
        if (status != X400_E_NOERROR) return status;
            
    }
    
#ifdef ADD_FWD_BODYPART
    /* Add MM Message bodypart */
    {
        struct X400Message *msg = NULL;
        
        /* We now need to add a message envelope, and the message body
         * to do this, we create a X400Message, and add that as the bodypart
         */

        status = X400MsgNew (X400_MSG_MESSAGE,&msg);
        if ( status != X400_E_NOERROR ) return (status);

        status = create_fw_message(msg);
        if ( status != X400_E_NOERROR ) return (status);

        status = X400MsgAddStrParam (msg, X400_S_EXTERNAL_CONTENT_TYPE, 
                                     def_ext_content_type, -1);
        if ( status != X400_E_NOERROR ) return (status);
        
        
        status = X400mtMsgAddMessageBodyWType (mp,msg,X400_T_MM);
        if ( status != X400_E_NOERROR ) return (status);
    }

    /* Add MM Forwarded Encrypted bodypart */
    {
        char *encrypted_data;
        int encrypted_data_len;
        struct X400Message *msg = NULL;
        
        /* Slightly confusing name here.
         * Since encrypted data just looks like a bunch of random bits,
         * then for simplicity that's what we are really using.
         * (It's not actually ber encoded.)
         */
        if (load_ber("pilot_fwd_info.ber",&encrypted_data,&encrypted_data_len)
            == 1) {
            fprintf(stderr,"Failed to load Fwd Enc data\n");
        } else {
        
            /* This is similar to the Forwarded MM message bodypart above
             * A new X400Message object is used to manipulate envelope
             * information. However for the data side of things,
             * an octet string is used.
             */
        
            status = X400MsgNew (X400_MSG_MESSAGE,&msg);
            if ( status != X400_E_NOERROR ) return (status);
            
            /* create_fw_message
             * Adds envelope and content information to the message object.
             * We will ignore the content information, and rely on the 
             * contents of X400_S_ENCRYPTED_DATA 
             */
            status = create_fw_message(msg);
            if ( status != X400_E_NOERROR ) return (status);
            
            /* NB create_fw_message sets the X400_CONTENT_TYPE to 2
             * IE interpersonal-messaging-1984
             * It's possible to set this to 1 which then means the content
             * type is externally defined
             */
            
            status = X400MsgAddStrParam (msg, X400_S_EXTERNAL_CONTENT_TYPE, 
                                         def_ext_content_type, -1);
            if ( status != X400_E_NOERROR ) return (status);
            
            
            status = X400MsgAddStrParam (msg, X400_S_ENCRYPTED_DATA, 
                                         encrypted_data, encrypted_data_len);
            free(encrypted_data);
            if ( status != X400_E_NOERROR ) return (status);
            
            status = X400mtMsgAddMessageBodyWType (mp,msg,X400_T_FWDENC);
            if ( status != X400_E_NOERROR ) return (status);
        }
    }

    
#endif
    
#endif

    /* Add originators certificate */
     {
         /* This is treated a a binary blob.
          * You need to pass in the correct BER encoded series of bytes.
          * 
          */
         int buf_len = 0;
         char *cert_ber = NULL;

	if (load_ber("1137492922.p12",&cert_ber,&buf_len) == 0) {
	    printf("Adding sample originator cert\n");
	    status = X400mtMsgAddStrParam (mp,X400_S_ORIG_CERT,
					   cert_ber,buf_len);
            free(cert_ber);
	    if ( status != X400_E_NOERROR ) exit (status);
	} else {
	    printf("unable to add originator certificate\n");
	}
	
    }

     /* Add MOAC */
     {
         /* This is treated a a binary blob.
          * You need to pass in the correct BER encoded series of bytes.
          */
         int buf_len = 0;
         char *moac_ber = NULL;

	if (load_ber("MOAC.ber",&moac_ber,&buf_len) == 0) {
	    printf("Adding MOAC\n");
	    status = X400mtMsgAddStrParam (mp,X400_S_MOAC,
					   moac_ber,buf_len);
            free(moac_ber);
	    if ( status != X400_E_NOERROR ) exit (status);
	} else {
	    printf("unable to add originator certificate\n");
	}
	
    }

     
    return X400_E_NOERROR;
}


static int add_binary_bp(
    struct X400mtMessage *mp
)
{
    FILE *fp = NULL;
    int fs=0;
    char *binary_data;
    int status;

    /*  or a Binary body part using the bodypart func */
    if (filename_to_send != NULL) {
	binary_data = (char *) malloc(bin_bp_size);
	if ( binary_data == NULL )
	    return X400_E_NOMEMORY;
	fp = fopen(filename_to_send, "rb");
	if (fp == (FILE *)NULL) {
	    printf("Cannot open binary file\n");
	    return (X400_E_SYSERROR);
	}
	if ((fs = fread (binary_data, sizeof(char), 
		bin_bp_size/sizeof(char), fp) ) == -1) {
	    printf("Cannot read from binary file\n");
	    fclose(fp);
	    return (X400_E_SYSERROR);
	}
	fclose(fp);
	if (fs < bin_bp_size) {
	    printf("Cannot read %d bytes from binary file (got %d)\n",
		bin_bp_size, fs);
	    return (X400_E_SYSERROR);
	}

	status = X400mtMsgAddAttachment (mp, X400_T_BINARY, binary_data, fs);
	if ( status != X400_E_NOERROR ) {
	    printf("failed to add X400_T_BINARY BP\n");
	    return (status);
	}
    } else {
	printf("no binary file set - not sending X400_T_BINARY\n");
    }
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

    /* NDR only = 1 */
    status = X400mtRecipAddIntParam (rp, X400_N_MTA_REPORT_REQUEST, 1);
    if ( status != X400_E_NOERROR ) return  (status);

    /* none = 0 */
    status = X400mtRecipAddIntParam (rp, X400_N_REPORT_REQUEST, 0);
    if ( status != X400_E_NOERROR ) return  (status);

    status = X400mtRecipAddIntParam (rp, X400_N_ORIGINAL_RECIPIENT_NUMBER, rno);
    rno++;
    if ( status != X400_E_NOERROR ) return  (status);

    status = X400mtRecipAddStrParam (rp, 
	X400_S_ORIGINATOR_REQUESTED_ALTERNATE_RECIPIENT, orar, -1);
    if ( status != X400_E_NOERROR ) return  (status);

    status = X400mtRecipAddStrParam (rp, 
	X400_S_ORIGINATOR_REQUESTED_ALTERNATE_RECIPIENT_DN, orardn, -1);
    if ( status != X400_E_NOERROR ) return  (status);

    return X400_E_NOERROR;
}

#ifdef add_multi_recips
static int add_content_recip_info(
    struct X400Recipient     *rp,
    const char 		     *free_form_name
)
{
    int status;

    /* add attributes to recipient in content */
    status = X400mtRecipAddIntParam (rp, X400_N_NOTIFICATION_REQUEST, 7);
    if ( status != X400_E_NOERROR ) return  (status);

    status = X400mtRecipAddIntParam (rp, X400_N_REPLY_REQUESTED, 1);
    if ( status != X400_E_NOERROR ) return  (status);

    status = X400mtRecipAddStrParam (rp, 
	X400_S_FREE_FORM_NAME, free_form_name, -1);
    if ( status != X400_E_NOERROR ) return  (status);

    return X400_E_NOERROR;
}
#endif

static void usage(void) {
    printf("usage: %s\n", optstr);
    printf("\t where:\n");
    printf("\t -u : Don't prompt to override defaults \n");
    printf("\t -G : Originator \n");
    printf("\t -O : Originator Return Address \n");
    printf("\t -g : Recipient\n");
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
    printf("\t -f : Filename to transfer as binary bp\n");
    printf("\t -X : Number of messages to send\n");
    printf("\t -S : Size of binary attachment (requires -f)\n");
    return;
}


char *set_latest_del_time(
    void
)
{
    static char latest_del_time[BUFSIZ];
    time_t now_secs;
    
    time(&now_secs);
    now_secs += 60*60*24*2; /* 2 days in the future */
    
    strftime(latest_del_time,
	     BUFSIZ,
	     "%y%m%d%H%M%SZ",
	     gmtime(&now_secs));
    return strdup(latest_del_time);
}


/* This function is used to load up pre-created BER files
 * E.G P772 Pilot forwarding information*/
static int load_ber(
    char *filename,
    char **output_buffer,
    int *buf_len
)
{
    FILE *fd = NULL;
    

    struct stat stat_info;
    
    if (stat(filename, &stat_info)!=0) {
	fprintf(stderr,"Failed to stat %s : %s\n",
		filename,strerror(errno));
	return 1;
    }
    
    *output_buffer = (char*) malloc (sizeof(char) * stat_info.st_size);
    /* Read in the security label XML file */
    fd = fopen(filename,"r");
    if(fd == NULL) {
	fprintf(stderr,"Failed to open %s : %s\n",
		filename,strerror(errno));
	return 1;
    }
    
    fread(*output_buffer,stat_info.st_size,1,fd);
    
    fclose(fd);
    *buf_len = stat_info.st_size;
    
    return 0;
}

#ifdef ADD_FWD_BODYPART
static int add_fwd_bodypart(
    struct X400mtMessage *mp
)
{
    int status;
    struct X400Message *mbp;
    /* Create an X.400 Message */
    status = X400MsgNew (X400_MSG_MESSAGE,&mbp);
    if (status != X400_E_NOERROR) {
        fprintf(stderr,"add_fwd_bodypart: %s\n",X400mtError(status));
        return status;
    }
    
    status = create_fw_message(mbp);
    if (status != X400_E_NOERROR) {
        fprintf(stderr,"failed to create fwd message\n");
        return status;
    }
    
    /* Append that to the extisting message */
    return X400mtMsgAddMessageBody (mp,mbp);
}

static int build_fwd_env (
    struct X400Message *mp
)
{
    int status;
    /* Build envelope */

    /* 
     * X400_S_ORIGINAL_ENCODED_INFORMATION_TYPES 
     * X400_S_MESSAGE_SUBMISSION_TIME 
     * X400_S_MESSAGE_DELIVERY_TIME
     */

    /* Add X400_S_ORIGINAL_ENCODED_INFORMATION_TYPES */
    status = X400MsgAddStrParam (mp,
                                 X400_S_ORIGINAL_ENCODED_INFORMATION_TYPES,
                                 "ia5-text",
                                 -1);
    if ( status != X400_E_NOERROR ) return (status);

    status = X400MsgAddStrParam (mp,
                                 X400_S_MESSAGE_SUBMISSION_TIME,
                                 "070801120000+0100",
                                 -1);
    if ( status != X400_E_NOERROR ) return (status);
    
    status = X400MsgAddStrParam (mp,
                                 X400_S_MESSAGE_DELIVERY_TIME,
                                 "070801120000+0100",
                                 -1);
    if ( status != X400_E_NOERROR ) return (status);
    
    /* Add Priority */
    status = X400MsgAddIntParam (mp, X400_N_PRIORITY, 1);
    if ( status != X400_E_NOERROR ) return (status);

    /* Add recipient disclosure */
    status = X400MsgAddIntParam (mp, X400_N_DISCLOSURE, 0);
    if ( status != X400_E_NOERROR ) return (status);
    
    /* Add implicit conversion prohibited */
    status = X400MsgAddIntParam (mp,
                                 X400_N_IMPLICIT_CONVERSION_PROHIBITED,
                                 1);
    if ( status != X400_E_NOERROR ) return (status);
    
    /* Add alt recipient allowed */
    status = X400MsgAddIntParam (mp,X400_N_ALTERNATE_RECIPIENT_ALLOWED,1);
    if ( status != X400_E_NOERROR ) return (status);
    
    /* Add Content return request */
    status = X400MsgAddIntParam (mp, X400_N_CONTENT_RETURN_REQUEST, 1);
    if ( status != X400_E_NOERROR ) return (status);
    
    
    /* Add Content return request */
    status = X400MsgAddIntParam (mp, X400_N_CONTENT_RETURN_REQUEST, 1);
    if ( status != X400_E_NOERROR ) return (status);

    
    /* Add Recipient reassignment prohibited */
    status = X400MsgAddIntParam (mp,
                                 X400_N_RECIPIENT_REASSIGNMENT_PROHIBITED,0);
    if ( status != X400_E_NOERROR ) return (status);
    
    /* Add Distribution list expansion prohibited */
    status = X400MsgAddIntParam (mp,X400_N_DL_EXPANSION_PROHIBITED,1);
    if ( status != X400_E_NOERROR ) return (status);
    
    /* Add Conversion with loss prohibited */
    status = X400MsgAddIntParam (mp,
                                 X400_N_CONVERSION_WITH_LOSS_PROHIBITED,
                                 1);
    if ( status != X400_E_NOERROR ) return (status);
    
    /* Add Message Identifier */
    status = X400MsgAddStrParam (mp,
                                 X400_S_MESSAGE_IDENTIFIER,
                                 fwd_msg_id,
                                 -1);
    if ( status != X400_E_NOERROR ) return (status);
    
    /* Add Content Identifier */
    status = X400MsgAddStrParam (mp,
                                 X400_S_CONTENT_IDENTIFIER, content_id, -1);
    if ( status != X400_E_NOERROR ) return (status);
    
    /* Add Latest Delivery Time */
    {
        char *latest_time = set_latest_del_time();
        /*  Latest Delivery Time: UTCTime format YYMMDDHHMMSS<zone> */
        status = X400MsgAddStrParam (mp,
                                     X400_S_LATEST_DELIVERY_TIME,
                                     latest_time, -1);
	free(latest_time);
	if ( status != X400_E_NOERROR ) return (status);
    }
    return X400_E_NOERROR;
}

static int build_fwd_content (
    struct X400Message *mp
)
{
    int status;
    /* Build content */
    /* Add IPM Identifier */
    status = X400MsgAddStrParam (mp, X400_S_IPM_IDENTIFIER, ipm_id, -1);
    if ( status != X400_E_NOERROR ) return (status);
    
    /* Add Subject */
    status = X400MsgAddStrParam (mp, X400_S_SUBJECT,"Forwarded test message", -1);
    if ( status != X400_E_NOERROR ) return (status);
    
    /* Add Replied to identifer */
    status = X400MsgAddStrParam (mp, X400_S_REPLIED_TO_IDENTIFIER, ipm_rel_id, -1);
    if ( status != X400_E_NOERROR ) return (status);
    
    /* Add Obseleted IPMs */
    status = X400MsgAddStrParam (mp, X400_S_OBSOLETED_IPMS, ipm_obs_id, -1);
    if ( status != X400_E_NOERROR ) return (status);
    
    /* Add Related IPMs */
    status = X400MsgAddStrParam (mp, X400_S_RELATED_IPMS, ipm_rel_id, -1);
    if ( status != X400_E_NOERROR ) return (status);
    
    /* Add Expiry times */
    status = X400MsgAddStrParam (mp, X400_S_EXPIRY_TIME, def_utc, -1);
    if ( status != X400_E_NOERROR ) return (status);
    
    /* Add Reply times */
    status = X400MsgAddStrParam (mp, X400_S_REPLY_TIME, def_utc, -1);
    if ( status != X400_E_NOERROR ) return (status);
    
    /* Add Importance */
    status = X400MsgAddIntParam (mp, X400_N_IMPORTANCE, importance);
    if ( status != X400_E_NOERROR ) return (status);
    
    /* Add Sensitivity */
    status = X400MsgAddIntParam (mp, X400_N_SENSITIVITY, sensitivity);
    if ( status != X400_E_NOERROR ) return (status);
    
    /* Add Autoforwarded */
    status = X400MsgAddIntParam (mp, X400_N_AUTOFORWARDED, autoforwarded);
    if ( status != X400_E_NOERROR ) return (status);
    
    /* Add IA5text  */
    status = X400MsgAddStrParam (mp, X400_T_IA5TEXT,
                                 "This is a forwarded test message", -1);
    if ( status != X400_E_NOERROR ) return (status);
    
    return X400_E_NOERROR;
}

static int add_fwd_recip(
    struct X400Message *mp,
    const char *oraddress,
    int type
)
{
    struct X400Recipient *rp;
    int status;

    status = X400RecipNew (type,&rp);
    if (status != X400_E_NOERROR) {
        fprintf(stderr,"Failed to create new \n");
        return status;
    }
    
    status = X400MsgAddRecip(mp, type, rp);
    if (status != X400_E_NOERROR) {
        fprintf(stderr,"Failed to add fwd recipient\n");
        return status;
    }
        
    status = X400RecipAddStrParam (rp, X400_S_OR_ADDRESS, oraddress, -1);
    if ( status != X400_E_NOERROR ) return  (status);

    status = X400RecipAddIntParam (rp, X400_N_RESPONSIBILITY, 1);
    if ( status != X400_E_NOERROR ) return  (status);
    
    status = X400RecipAddIntParam (rp, X400_N_MTA_REPORT_REQUEST, 1);
    if ( status != X400_E_NOERROR ) return  (status);

    status = X400RecipAddIntParam (rp, X400_N_REPORT_REQUEST, 0);
    if ( status != X400_E_NOERROR ) return  (status);

    status = X400RecipAddIntParam (rp, X400_N_ORIGINAL_RECIPIENT_NUMBER,
                                   fwd_rno);
    fwd_rno++;
    if ( status != X400_E_NOERROR ) return  (status);
    
    return X400_E_NOERROR;
}

static int create_fw_message(
    struct X400Message *mp
)
{
    int status;
    /* Add Originator Address */
    status = X400MsgAddStrParam (mp, X400_S_OR_ADDRESS, fwd_orig, -1);
    if ( status != X400_E_NOERROR ) exit (status);
    
    /* Add Recipient Address */
    status = add_fwd_recip(mp,fwd_orig,X400_ORIGINATOR);
    if (status != X400_E_NOERROR) {
        fprintf(stderr,"Failed to added forwarded originator\n");
        return status;
    }
    
    status = add_fwd_recip(mp,fwd_recip,X400_RECIP_STANDARD);
    if (status != X400_E_NOERROR) {
        fprintf(stderr,"Failed to added forwarded originator\n");
        return status;
    }
    
    status = build_fwd_env(mp);
    if (status != X400_E_NOERROR) {
        fprintf(stderr,"Failed to build forwarded message envelope\n");
        return status;
    }
   
    status = build_fwd_content(mp);
    if (status != X400_E_NOERROR){
        fprintf(stderr,"Failed to build forwarded message content\n");
        return status;
    }

   
    /* Add external content type */
    
    return X400_E_NOERROR;
}

static int add_fwd_content_bodypart (
    struct X400mtMessage *mp
)
{
    
    int buf_len = 0;
    char *fwd_content_ber = NULL;
    struct X400Message *mbp;
    int status;
    
/* Create an X.400 Message */
    status = X400MsgNew (X400_MSG_MESSAGE,&mbp);
    if (status != X400_E_NOERROR) {
        fprintf(stderr,"add_fwd_bodypart: %s\n",X400mtError(status));
        return status;
    }
    
    status = create_fw_message(mbp);
    if ( status != X400_E_NOERROR ) return (status);
    
    status = X400MsgAddStrParam (mbp, X400_S_EXTERNAL_CONTENT_TYPE, 
                                 def_ext_content_type, -1);
    if ( status != X400_E_NOERROR ) return (status);

    if (load_ber("example_fwd_content.ber",&fwd_content_ber,&buf_len) == 0) {
        printf("Adding forwarded content string\n");

        status = X400MsgAddStrParam (mbp, X400_S_FWD_CONTENT_STRING, 
                                     fwd_content_ber, buf_len);
        free(fwd_content_ber);
        if ( status != X400_E_NOERROR ) exit (status);

        status = X400mtMsgAddMessageBodyWType (mp,mbp,X400_T_FWD_CONTENT);
        if ( status != X400_E_NOERROR ) return (status);
    } else {
        printf("unable to add forwarded content info\n");
    }

    
    
    return X400_E_NOERROR;
}

#endif



static int add_dl_exempted_recip_list (
    struct X400mtMessage *mp
)
{
    struct X400Recipient *rp1;
    struct X400Recipient *rp2;
    struct X400Recipient *rp3;
    int status;
    
    status = X400mtRecipNew (mp,X400_DL_EXEMPTED_RECIP, &rp1);
    if ( status != X400_E_NOERROR ) return  (status);

    status = X400mtRecipAddStrParam (rp1, X400_S_OR_ADDRESS,
                                     "/cn=x411exempted1/PRMD=TestPRMD/"
                                     "ADMD=TestADMD/C=GB", -1);
    if ( status != X400_E_NOERROR ) return  (status);

    status = X400mtRecipNew (mp,X400_DL_EXEMPTED_RECIP, &rp2);
    if ( status != X400_E_NOERROR ) return  (status);

    status = X400mtRecipAddStrParam (rp2, X400_S_OR_ADDRESS,
                                     "/cn=x411exempted2/PRMD=TestPRMD/"
                                     "ADMD=TestADMD/C=GB", -1);
    if ( status != X400_E_NOERROR ) return  (status);

    status = X400mtRecipNew (mp,X400_DL_EXEMPTED_RECIP, &rp3);
    if ( status != X400_E_NOERROR ) return  (status);

    status = X400mtRecipAddStrParam (rp3, X400_S_OR_ADDRESS,
                                     "/cn=x411exempted3/PRMD=TestPRMD/"
                                     "ADMD=TestADMD/C=GB", -1);
    if ( status != X400_E_NOERROR ) return  (status);
    
    return X400_E_NOERROR;
}
