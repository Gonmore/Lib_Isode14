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
 * Simple example program for submitting a message via a message store.
 * The security environment is set up so that messages are signed with
 * a per recipient message token.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _WIN32
#include <unistd.h>
#endif
#include <fcntl.h>
#include <errno.h>

#include <x400_msapi.h>
#include <seclabel_api.h> /* For security labels */
#include "example.h"

/* uncomment this if you want a second recipient.
 * NB as the recipient name is the same, M-Switch 
 * removes the duplicate envelope recips */
/*
#define tworecips 1
*/

/* uncomment this to cause the API to generate msg_seq_nums which
 * increment by 1 per message per session */
#define FIRST_SEQ_NUM 2

#ifdef FIRST_SEQ_NUM
int seq_num_val_1 = FIRST_SEQ_NUM;
int seq_num_val_2 = FIRST_SEQ_NUM + 100;
#endif

/*
#define ADD_MOAC
*/

static char *optstr = "u37m:d:p:w:M:D:P:W:r:o:O:r:g:G:c:l:R:y:C:iaqsAve:x:b:f:";

/* These are the data items used to construct the message for submission */
static char *default_recip = "/CN=P7User1/OU=sales/O=attlee/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
char *recip;
char *recip2;
static char *content_id = "030924.140212";
static char *subject = "A Test Message";
static const char text[] = "First line\r\nSecond line\r\n";
static char *binary_data;

int adding_sec_label = 1;

/* local functions */

static void usage(void) ;
static int send_msg(
    int contype, 
    char *orn, 
    char *def_dn, 
    char *pa, 
    char *password);

static int submit_msg(
    char *orn, 
    struct X400msSession *sp	/* session object */
);
static int setup_default_sec_env(
    struct X400msSession *sp, 
    char *id, 
    char *dn, 
    char *pw
);

static int setup_msg_sec_env(
    struct X400msMessage *mp,	/* message object */
    char *id, 
    char *dn, 
    char *pw
);

static int setup_recip_sec_env(
    struct X400Recipient *rp,	/* recipient object */
    char *id, 	/* x509 identity (directory with x509 subdir) */
    char *dn,   /* DN to use as subject of the X.509 identity */
    char *pw	/* passphrase for private key in pkcs12 file */
);

static int add_sec_label(
    struct X400msMessage *mp	/* message object */
);


/* start here */
int main (
    int         argc,
    char      **argv
) 
{
    char 	pa[BUFSIZ];
    char 	orn[BUFSIZ];
    char 	*def_oraddr;
    char 	*def_dn;
    char 	*def_pa;
    int 	contype;
    int 	status;
    char 	password[BUFSIZ];

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
    } else {
	def_oraddr = x400_mta_user_addr;
	def_dn = x400_mta_user_dn;
	def_pa = x400_mta_presentation_address;
    }

    printf("Your ORAddress [%s] > ", def_oraddr);
    ic_fgets (orn, sizeof orn, stdin);

    if ( orn[strlen(orn)-1] == '\n' )
	orn[strlen(orn)-1] = '\0';
	
    if (orn[0] == '\0') 
	strcpy(orn, def_oraddr);

    /* Prompt for password; note: reflected. */
    printf ("Password [%s]: ", 
	    contype == 0 ? x400_p7_password : x400_p3_password);
    if ( ic_fgets (password, sizeof password, stdin) == NULL )
	exit (1);

    if (password[strlen(password)-1] == '\n' )
	password[strlen(password)-1] = '\0';
    if (password[0] == '\0') 
	strcpy(password, contype == 0 ? x400_p7_password : x400_p3_password);

    /* Presentation Address */
    printf("Presentation Address [%s] > ", def_pa);
    ic_fgets (pa, sizeof pa, stdin);

    if ( pa[strlen(pa)-1] == '\n' )
	pa[strlen(pa)-1] = '\0';

    if (pa[0] == '\0') 
	strcpy(pa, def_pa);

    printf("sending message using session 1\n");
    if ((status = send_msg(contype, orn, def_dn, pa, password)) 
	    != X400_E_NOERROR ) {
	fprintf (stderr, "Error in sending message\n");
	exit (status);
    }

#ifdef notdef
    printf("sending message using session 2\n");
    if ((status = send_msg(contype, orn, def_dn, pa, password)) 
	    != X400_E_NOERROR ) {
	fprintf (stderr, "Error sending message\n");
	exit (status);
    }
#endif

    return (status);
}

static int send_msg(
    int contype, 
    char *orn, 
    char *def_dn, 
    char *pa, 
    char *password
)
{
    struct X400msSession *sp;	/* session object */

    int	 	status;
    int 	nummsg;


    /* Open an API session */
    status = X400msOpen (contype, orn, def_dn, password, pa, &nummsg, &sp);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "Error in Open: %s\n", X400msError (status));
	fprintf (stderr, "%s %s %s\n", orn, def_dn, pa);
	return (status);
    }

    /* setup logging */
    X400msSetStrDefault(sp, X400_S_LOG_CONFIGURATION_FILE, "x400api.xml", 0);

    /* Set up the security environment.
     * The ID is specified as a pathname, in which the subdirectory
     * "x509" is expected to contain one or more PKCS12 files.
     * The appropriate Digital Identity is determined from the
     * DN of the subject, and the passphrase is used to decrypt
     * the private key. */
    status = setup_default_sec_env(sp, security_id, identity_dn, passphrase);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "Can't setup security environment\n");
	return (status);
    }

    printf("sending message 1\n");
    status = submit_msg(orn, sp);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "Can't submit\n");
	return (status);
    }

    /*
    status = setup_default_sec_env(sp, security_id, identity_dn2, passphrase);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "Can't setup security environment\n");
	return (status);
    }
    */

/*
    printf("sending message 2\n");
    status = submit_msg(orn, sp);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "Can't submit\n");
	return (status);
    }
    printf("sending message 3\n");
    status = submit_msg(orn, sp);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "Can't submit\n");
	return (status);
    }
*/
    status = X400msClose (sp);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "X400msClose returned error: %s\n", X400msError (status));
	return (status);
    }
    return status;
}

static int submit_msg(
    char *orn, 
    struct X400msSession *sp	/* session object */
)
{
    struct X400msMessage *mp;	/* message object */
    struct X400Recipient *rp;	/* recipient object */
#ifdef tworecips
    struct X400Recipient *rp2;	/* recipient object */
#endif
    char 	tmp[BUFSIZ];
    FILE 	*fp = NULL;
    int 	fs=0;
    int	 	status;
    static int msg_num = 0;

    msg_num++;

    if (x400_default_recipient != NULL)  {
	recip = x400_default_recipient;
	recip2 = x400_default_recipient;
    } else {
	recip = default_recip;
	recip2 = default_recip;
    }

    printf("Message recipient [%s]: ", recip);
    ic_fgets (tmp, sizeof tmp, stdin);

    if ( tmp[strlen(tmp)-1] == '\n' )
	tmp[strlen(tmp)-1] = '\0';
    if (strlen(tmp) != 0) {
	recip = strdup(tmp);
	recip2 = strdup(tmp);
    }

    printf("Subject [%s]: ", subject);
    ic_fgets (tmp, sizeof tmp, stdin);

    if ( tmp[strlen(tmp)-1] == '\n' )
	tmp[strlen(tmp)-1] = '\0';
    if (strlen(tmp) != 0)
	subject = strdup(tmp);

    printf("\n");
    status = X400msMsgNew (sp, X400_MSG_MESSAGE, &mp);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msMsgNew returned error: %s\n", 
	    X400msError (status));
	return (status);
    }

#ifdef use_diff_sec_env_second_msg
    if (msg_num > 1) {
	/* set up a security env for the message - this will be used in 
	 * preference to the default security env */
	status = setup_msg_sec_env(mp, "/root", 
	    "cn=P7User1,o=Address Book,c=GB", "secret");
	if ( status != X400_E_NOERROR ) {
	    fprintf (stderr, "setup_msg_sec_env returned error: %s\n", 
		X400msError (status));
	    /* ignore - let's see if the defaults work */
	}
    }
#endif

#ifdef ADD_MOAC
    /* Instruct the X400msMsgSend() function to generate a 
     * MOAC (signature) for
     * the message Instruct the X400msMsgSend() function to generate a MOAC
     * (signature) for the message
     * The default attributes will be used unless the attributes are
     * also included in the message eg by calling setup_msg_sec_env() above */
    status = X400msMsgAddIntParam (mp, X400_B_SEC_GEN_MOAC, 1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msMsgAddStrParam returned error: %s\n", 
	    X400msError (status));
	return (status);
    } else {
	fprintf (stderr, "Adding MOAC\n");
    }
#endif


    /* originator */
    status = X400msMsgAddStrParam (mp, X400_S_OR_ADDRESS, orn, -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msMsgAddStrParam returned error: %s\n", 
	    X400msError (status));
	return (status);
    }
 
    status = X400msMsgAddStrParam (mp, X400_S_DIRECTORY_NAME, 
	"CN=originator;c=gb", -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msMsgAddStrParam returned error: %s\n", 
	    X400msError (status));
	return (status);
    }

    if (adding_sec_label) {
	status = add_sec_label (mp);
	if ( status != X400_E_NOERROR ) {
	    fprintf (stderr, "Failed to add Security Label: %s\n", 
		X400msError (status));
	    return (status);
	} else {
	    fprintf (stderr, "added Security Label\n"); 
	}
    }

    /* first recip */
    status = X400msRecipNew (mp, X400_RECIP_STANDARD, &rp);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msRecipNew returned error: %s\n", 
	    X400msError (status));
	return (status);
    }

    status = X400msRecipAddStrParam (rp, X400_S_OR_ADDRESS, recip, -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msRecipAddStrParam returned error: %s\n", 
	    X400msError (status));
	return (status);
    }

    status = X400msRecipAddStrParam (rp, X400_S_DIRECTORY_NAME, 
	"CN=recipient;c=gb", -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msRecipAddStrParam returned error: %s\n", 
	    X400msError (status));
	return (status);
    }

    /* cause a PODR to be added as a recip extension */
    status = X400msRecipAddIntParam (rp, X400_N_PROOF_OF_DEL_REQ, 1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msMsgAddStrParam returned error: %s\n", 
	    X400msError (status));
	return (status);
    }

    /* cause a Content Integrity Check to be added as a recip extension */
    status = X400msRecipAddIntParam (rp, X400_B_SEC_CONTENT_INTEGRITY_CHECK, 1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "X400msRecipAddIntParam returned error: %s\n", 
	    X400msError (status));
	return (status);
    }

    /* Ask for Message Token for this recip */
    status = X400msRecipAddIntParam (rp, X400_B_SEC_GEN_MESSAGE_TOKEN, 1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msRecipAddStrParam returned error: %s\n", 
	    X400msError (status));
	return (status);
    }

#ifdef FIRST_SEQ_NUM
    /* Add sequence num */
    status = X400msRecipAddIntParam(rp, X400_N_MSGTOK_SEQ_NUM, 
	seq_num_val_1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msRecipAddStrParam returned error: %s\n", 
	    X400msError (status));
	return (status);
    } else {
	fprintf (stderr, "Added sequence number %d for token\n", seq_num_val_1);
    }
    seq_num_val_1++;
#endif


    /* Ask for +ve and -ve delivery reports */
    /* status = X400msRecipAddIntParam (rp, X400_N_REPORT_REQUEST, 2); */
    /* Ask for no +ve and -ve delivery reports */
    status = X400msRecipAddIntParam (rp, X400_N_REPORT_REQUEST, 0);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msRecipAddStrParam returned error: %s\n", 
	    X400msError (status));
	return (status);
    }

    /* Ask for +ve and -ve read receipts */
    status = X400msRecipAddIntParam (rp, X400_N_NOTIFICATION_REQUEST, 7);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msRecipAddStrParam returned error: %s\n", 
	    X400msError (status));
	return (status);
    }

#ifdef tworecips
    /* second recip */
    status = X400msRecipNew (mp, X400_RECIP_STANDARD, &rp2);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msRecipNew returned error: %s\n", 
	    X400msError (status));
	return (status);
    }

    /* set up a security env for the second recip - 
     * this will be used in preference to the message and 
     * default security envs */
    status = setup_recip_sec_env(rp2, "/root", 
	"cn=P7User1,o=Address Book,c=GB", "secret");
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "setup_recip_sec_env returned error: %s\n", 
	    X400msError (status));
	/* ignore - let's see if the message or defaults work */
    }

    status = X400msRecipAddStrParam (rp2, X400_S_OR_ADDRESS, recip2, -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msRecipAddStrParam returned error: %s\n", 
	    X400msError (status));
	return (status);
    }

    status = X400msRecipAddStrParam (rp2, X400_S_DIRECTORY_NAME, 
	"CN=recipient;c=gb", -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msRecipAddStrParam returned error: %s\n", 
	    X400msError (status));
	return (status);
    }

    /* Ask for Message Token for this recip */
    status = X400msRecipAddIntParam(rp2, X400_B_SEC_GEN_MESSAGE_TOKEN, 1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msRecipAddStrParam returned error: %s\n", 
	    X400msError (status));
	return (status);
    }

#ifdef FIRST_SEQ_NUMXXXX
    /* Add sequence num */
    status = X400msRecipAddIntParam(rp2, X400_N_MSGTOK_SEQ_NUM, 
	seq_num_val_2);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msRecipAddStrParam returned error: %s\n", 
	    X400msError (status));
	return (status);
    } else {
	fprintf (stderr, "Added sequence number %d for token\n", seq_num_val_1);
    }
    seq_num_val_2++;
#endif

    /* Ask for +ve and -ve delivery reports */
    status = X400msRecipAddIntParam (rp2, X400_N_REPORT_REQUEST, 2);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msRecipAddStrParam returned error: %s\n", 
	    X400msError (status));
	return (status);
    }

    /* Ask for +ve and -ve read receipts */
    status = X400msRecipAddIntParam (rp2, X400_N_NOTIFICATION_REQUEST, 7);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msRecipAddStrParam returned error: %s\n", 
	    X400msError (status));
	return (status);
    }

#endif

    status = X400msMsgAddStrParam (mp, X400_S_CONTENT_IDENTIFIER, 
	    content_id, -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msMsgAddStrParam returned error: %s\n", 
	    X400msError (status));
	return (status);
    }

    status = X400msMsgAddStrParam (mp, X400_S_SUBJECT, subject, -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msMsgAddStrParam returned error: %s\n", 
	    X400msError (status));
	return (status);
    }

    status = X400msMsgAddStrParam (mp, X400_T_ISO8859_1, text, -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400ms returned error: %s\n", X400msError (status));
	return (status);
    }

    /* now an IA5 body part using the bodypart func  */
    status = X400msMsgAddAttachment (mp, X400_T_IA5TEXT, text, strlen(text));
    if ( status != X400_E_NOERROR ) {
	printf("failed to add X400_T_IA5TEXT BP\n");
	return (status);
    }

    /*  or a Binary body part using the bodypart func */
    if (filename_to_send != NULL) {
	binary_data = (char *) malloc(100000);
	if ( binary_data == NULL )
	    return X400_E_NOMEMORY;
	fp = fopen(filename_to_send, "r");
	if (fp == (FILE *)NULL) {
	    printf("Cannot open binary file\n");
	    return (X400_E_SYSERROR);
	}
	if ((fs = fread (binary_data, sizeof(char), 100000/sizeof(char), fp)) 
		== -1) {
	    printf("Cannot read from binary file\n");
	    return (X400_E_SYSERROR);
	}
	fclose(fp);

	status = X400msMsgAddAttachment (mp, X400_T_BINARY, binary_data, fs);
	if ( status != X400_E_NOERROR ) {
	    printf("failed to add X400_T_BINARY BP\n");
	    return (status);
	}

	free (binary_data);

    } else {
	printf("no binary file set - not sending X400_T_BINARY\n");
    }

    status = X400msMsgSend (mp);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msMsgSend returned error: %s\n", 
	    X400msError (status));
	return (status);
    } else {
	printf("Message submitted successfully\n");
    }

    status = X400msMsgDelete (mp, 0);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msMsgDelete returned error: %s\n", 
	    X400msError (status));
	return (status);
    }

    mp = NULL;
    rp = NULL;

    return (status);
}


static int setup_default_sec_env(
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
    X400msSetStrDefault(sp, X400_S_SEC_IDENTITY, id, -1); 

    /* select by DN which Identity is to be used (if there are several) 
     * Currently these must be PKCS12 files */
    status = X400msSetStrDefault (sp, X400_S_SEC_IDENTITY_DN, dn, -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "X400msSetStrDefault returned error: %s\n", 
	    X400msError (status));
	return (status);
    }

    /* passphrase used to open the Identity */
    status = X400msSetStrDefault (sp, X400_S_SEC_IDENTITY_PASSPHRASE, pw, -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "X400msSetStrDefault returned error: %s\n", 
	    X400msError (status));
	return (status);
    }
    return status;
}

static int setup_msg_sec_env(
    struct X400msMessage *mp,	/* message object */
    char *id, 
    char *dn, 
    char *pw
)
{
    int status;

    printf("setting up message security env %s\n", id);

    /* overide the default security environment by specifying a new set of
     * attibutes, and put them into the message */
    /* security additions */
    status = X400msMsgAddStrParam (mp, X400_S_SEC_IDENTITY, id, -1); 
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msMsgAddStrParam returned error: %s\n", 
	    X400msError (status));
	return (status);
    }

    /* security additions */
    status = X400msMsgAddStrParam (mp, X400_S_SEC_IDENTITY_DN, dn, -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msMsgAddStrParam returned error: %s\n", 
	    X400msError (status));
	return (status);
    }

    /* security additions */
    status = X400msMsgAddStrParam (mp, X400_S_SEC_IDENTITY_PASSPHRASE, pw, -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msMsgAddStrParam returned error: %s\n", 
	    X400msError (status));
	return (status);
    }
    return (status);
}

static int setup_recip_sec_env(
    struct X400Recipient *rp,	/* recipient object */
    char *id, 	/* x509 identity (directory with x509 subdir) */
    char *dn,   /* DN to use as subject of the X.509 identity */
    char *pw	/* passphrase for private key in pkcs12 file */
)
{
    int status;

    printf("setting up recipient security env %s\n", id);

    /* overide the default and message security environment 
     * by specifying a new set of attibutes, and put them 
     * into the recipient security attributes */
    status = X400msRecipAddStrParam (rp, X400_S_SEC_IDENTITY, id, -1); 
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msRecipAddStrParam returned error: %s\n", 
	    X400msError (status));
	return (status);
    }

    status = X400msRecipAddStrParam (rp, X400_S_SEC_IDENTITY_DN, dn, -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msRecipAddStrParam returned error: %s\n", 
	    X400msError (status));
	return (status);
    }

    status = X400msRecipAddStrParam (rp, 
	X400_S_SEC_IDENTITY_PASSPHRASE, pw, -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msRecipAddStrParam returned error: %s\n", 
	    X400msError (status));
	return (status);
    }
    return (status);
}

static int add_sec_label(
    struct X400msMessage *mp	/* message object */
)
{
#define XML_BUFSIZE 1024
#define STRING_BUFSIZE 1024
    
    const char* xml_filename = "seclabel.xml";
    char xml_content[XML_BUFSIZE];
    char str_content[STRING_BUFSIZE];
    int str_len = STRING_BUFSIZE;
    FILE *fd = NULL;
    int status;
   
    /* Read in the security label XML file */
    fd = fopen(xml_filename,"r");
    if(fd == NULL) {
	fprintf(stderr,"Failed to open %s : %s\n",
		xml_filename,strerror(errno));
	return X400_E_INT_ERROR;
    }
    
    fread(&xml_content,XML_BUFSIZE,1,fd);

    fclose(fd);

    status = SecLabelInit("Example program");
    if (status != SECLABEL_E_NOERROR) {
	fprintf(stderr, "SecLabelInit returned error %d\n", status);
	return(X400_E_INT_ERROR);
    }
    
    /* Use SecLabelParse to turn the XML into an octect string */
    status = SecLabelParse(xml_content,
			   str_content,
			   STRING_BUFSIZE,
			   &str_len);
    
    if (status != SECLABEL_E_NOERROR) {
	fprintf(stderr, "SecLabelParse returned error %d\n", status);
	return(X400_E_INT_ERROR);
    }
	
    /* Add the octect string to the message pointer */
    status = X400msMsgAddStrParam (mp, X400_S_SECURITY_LABEL,
				   str_content,str_len);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msMsgAddStrParam returned error: %s\n", 
	    X400msError (status));
	return (status);
    }
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
    printf("\t -o : Originator \n");
    printf("\t -O : Originator Return Address \n");
    printf("\t -r : Recipient\n");
    printf("\t -l : Logline\n");
    printf("\t -y : Priority (0 - normal, 1 - non-urgent, 2 - urgent \n");
    printf("\t -C : Content Type (2/22/772/OID) \n");
    printf("\t -i : Implicit conversion prohibited = TRUE \n");
    printf("\t -a : Alternate Recipient Prohibited = TRUE \n");
    printf("\t -q : Content Return Request = TRUE \n");
    printf("\t -s : Disclosure of Recipient = FALSE \n");
    printf("\t -A : Recipient Reassignment Prohibited = FALSE \n");
    printf("\t -v : Conversion with Loss Prohibited = FALSE \n");
    printf("\t -e : Security Environment (dir with x509 subdir)\n");
    printf("\t -x : DN of X.509 Digital Identity\n");
    printf("\t -b : Passphrase for private key in PKCS12 file\n");
    printf("\t -f : Filename to transfer as binary bp\n");
    return;
}


