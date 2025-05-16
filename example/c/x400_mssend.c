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
 * Simple example program for submitting a message via a message store.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#include <fcntl.h>
#include <errno.h>

#include <x400_msapi.h>
#include <seclabel_api.h> /* For security labels */
#include "example.h"


static char *optstr = "u371m:d:p:w:M:D:P:W:r:o:O:r:g:G:c:l:R:y:C:iaqsAvf:";

/* These are the data items used to construct the message for submission */
static char *default_recip = "/CN=P7User1/O=attlee/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
char *recip;
static char *subject = "A Test Message from X.400 C API";
static const char text[] = "First line\r\nSecond line\r\n";
static char *binary_data;

static char *fwd_subject = "Forwarded message subject";

static int add_binary_attachment (
    struct X400msMessage *mp,
    char * filename_to_send
);
static int add_fwd_bp (
    struct X400msMessage 	*mp,
    char 			*recip_orn,
    char 			*recip_dn
) ;
static void usage(void) ;


int main (
    int         argc,
    char      **argv
) 
{
    char buffer[BUFSIZ];
    char pa[BUFSIZ];
    char orn[BUFSIZ];
    char tmp[BUFSIZ];
    int status;
    struct X400msSession *sp;
    struct X400msMessage *mp;
    struct X400Recipient *rp;
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
    if ( ic_fgets (buffer, sizeof buffer, stdin) == NULL )
	exit (1);

    if (buffer[strlen(buffer)-1] == '\n' )
	buffer[strlen(buffer)-1] = '\0';
    if (buffer[0] == '\0') 
	strcpy(buffer, contype == 0 ? x400_p7_password : x400_p3_password);

    /* Presentation Address */
    printf("Presentation Address [%s] > ", def_pa);
    ic_fgets (pa, sizeof pa, stdin);

    if ( pa[strlen(pa)-1] == '\n' )
	pa[strlen(pa)-1] = '\0';

    if (pa[0] == '\0') 
	strcpy(pa, def_pa);

    if (talking_to_marben_ms)
	X400msSetConfigRequest(0);

    /* we've got what we need - now open an API session */
    status = X400msOpen (contype, orn, def_dn, buffer, pa, NULL, &sp);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "Error in Open: %s\n", X400msError (status));
	exit (status);
    }

    if (talking_to_marben_ms)
	X400msSetIntDefault(sp, X400_N_STRICT_P7_1988, 1);

    /* setup logging */
    X400msSetStrDefault(sp, X400_S_LOG_CONFIGURATION_FILE, "x400api.xml", 0);

    if (x400_default_recipient != NULL) 
	recip = x400_default_recipient;
    else 
	recip = default_recip;

    printf("Message recipient [%s]: ", recip);
    ic_fgets (tmp, sizeof tmp, stdin);

    if ( tmp[strlen(tmp)-1] == '\n' )
	tmp[strlen(tmp)-1] = '\0';
    if (strlen(tmp) != 0)
	recip = strdup(tmp);

    printf("Subject [%s]: ", subject);
    ic_fgets (tmp, sizeof tmp, stdin);

    if ( tmp[strlen(tmp)-1] == '\n' )
	tmp[strlen(tmp)-1] = '\0';
    if (strlen(tmp) != 0)
	subject = strdup(tmp);

    status = X400msMsgNew (sp, X400_MSG_MESSAGE, &mp);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msMsgNew returned error: %s\n", X400msError (status));
	exit (status);
    }

    status = X400msRecipNew (mp, X400_RECIP_STANDARD, &rp);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msRecipNew returned error: %s\n", X400msError (status));
	exit (status);
    }

    status = X400msRecipAddStrParam (rp, X400_S_OR_ADDRESS, recip, -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msRecipAddStrParam returned error: %s\n", X400msError (status));
	exit (status);
    }

    status = X400msRecipAddStrParam (rp, X400_S_DIRECTORY_NAME, "CN=recipient;c=gb", -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msRecipAddStrParam returned error: %s\n", X400msError (status));
	exit (status);
    }

    status = X400msMsgAddStrParam (mp, X400_S_OR_ADDRESS, orn, -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msMsgAddStrParam returned error: %s\n", X400msError (status));
	exit (status);
    }
 
    status = X400msMsgAddStrParam (mp, X400_S_DIRECTORY_NAME, "CN=originator;c=gb", -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msMsgAddStrParam returned error: %s\n", X400msError (status));
	exit (status);
    }

    /* Ask for +ve and -ve delivery reports : 2 */
    status = X400msRecipAddIntParam (rp, X400_N_REPORT_REQUEST, 1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msRecipAddStrParam returned error: %s\n", X400msError (status));
	exit (status);
    }

    /* Ask for +ve and -ve read receipts : 7*/
    status = X400msRecipAddIntParam (rp, X400_N_NOTIFICATION_REQUEST, 2);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msRecipAddIntParam returned error: %s\n", X400msError (status));
	exit (status);
    }

    if (1) {
	char *contid = "ContID00001";

	/* Content identifier so we can correlate with submission result */
	status = X400msMsgAddStrParam(mp, X400_S_CONTENT_IDENTIFIER, contid, -1);
	if ( status != X400_E_NOERROR ) {
	    fprintf (stderr, "X400msMsgAddIntParam %d returned error: %s\n", 
		     X400_S_CONTENT_IDENTIFIER, X400msError (status));
	    exit (status);
	}
    }

    /* content return request on report - 0 = no */
    status = X400msMsgAddIntParam (mp, X400_N_CONTENT_RETURN_REQUEST, 0);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "X400msMsgAddIntParam %d returned error: %s\n", 
	    X400_N_CONTENT_RETURN_REQUEST, X400msError (status));
	exit (status);
    }

    /* Priority: 0 - normal, 1 - non-urgent, 2 - urgent */
    printf("message priority is %d ( 0 - normal, 1 - non-urgent, 2 - urgent)\n", 
	    x400_default_priority);
    status = X400msMsgAddIntParam (mp, X400_N_PRIORITY, x400_default_priority);
    if ( status != X400_E_NOERROR ) return (status);

    /* Priority: 0 - normal, 1 - non-urgent, 2 - urgent */
    printf("military message priority is %d ( 0 - low, 1 - high)\n", 
	    x400_default_priority);
    status = X400msMsgAddIntParam (mp, X400_N_MMTS_PRIORITY_QUALIFIER, 1);
    if ( status != X400_E_NOERROR ) return (status);

    /* subject */
    status = X400msMsgAddStrParam (mp, X400_S_SUBJECT, subject, -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msMsgAddStrParam returned error: %s\n", X400msError (status));
	exit (status);
    }

    /* 8859-1 attachment */
    status = X400msMsgAddStrParam (mp, X400_T_ISO8859_1, text, -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400ms returned error: %s\n", X400msError (status));
	exit (status);
    }

    /* now an IA5 body part using the bodypart func  */
    status = X400msMsgAddAttachment (mp, X400_T_IA5TEXT, text, strlen(text));
    if ( status != X400_E_NOERROR ) {
	printf("failed to add X400_T_IA5TEXT BP\n");
	return (status);
    }

        
    /*  or a Binary body part using the bodypart func */
    if (filename_to_send != NULL) {
	status = add_binary_attachment (mp, filename_to_send); 
	if ( status != X400_E_NOERROR ) {
	    printf("failed to add X400_T_BINARY BP\n");
	    return (status);
	}
	status = add_fwd_bp (mp, orn, def_dn); 
	if ( status != X400_E_NOERROR ) {
	    printf("failed to add forwarded BP\n");
	    return (status);
	}
    } else {
	printf("no binary file set - not sending X400_T_BINARY\n");
	printf("no binary file set - not sending forwarded BP\n");
    }


#ifdef USE_SEC_LABEL
    {
#define XML_BUFSIZE 1024
#define STRING_BUFSIZE 1024
	
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
	status = X400msMsgAddStrParam (mp, X400_S_SECURITY_LABEL,
				       str_content,str_len);
	if ( status != X400_E_NOERROR ) {
	    fprintf (stderr, "x400msMsgAddStrParam returned error: %s\n", X400msError (status));
	    exit (status);
	}
	
    }
#endif
    
    status = X400msMsgSend (mp);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msMsgSend returned error: %s\n", X400msError (status));
	exit (status);
    } else {
	char buf[1024];
	size_t retlen;

	printf("Message submitted successfully\n");

	status = X400msMsgGetStrParam(mp, X400_S_MESSAGE_IDENTIFIER, buf, 1024, &retlen);
	if (status != X400_E_NOERROR) {
	    fprintf(stderr, "No MessageId present from submission result: error %d\n", status);
	} else {
	    buf [retlen] = '\0';
	    printf("MessageId from Submission Result = %s\n", buf);
	}

	status = X400msMsgGetStrParam(mp, X400_S_MESSAGE_SUBMISSION_TIME, buf, 1024, &retlen);
	if (status != X400_E_NOERROR) {
	    fprintf(stderr, "No MessageSubmissionTime present from submission result: error %d\n", status);
	} else {
	    buf [retlen] = '\0';
	    printf("MessageSubmissionTime from Submission Result = %s\n", buf);
	}

	status = X400msMsgGetStrParam(mp, X400_S_CONTENT_IDENTIFIER, buf, 1024, &retlen);
	if (status != X400_E_NOERROR) {
	    fprintf(stderr, "No ContentIdentifier present from submission result: error %d\n", status);
	} else {
	    buf [retlen] = '\0';
	    printf("ContentIdentifier from Submission Result = %s\n", buf);
	}
    }

    status = X400msMsgDelete (mp, 0);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msMsgDelete returned error: %s\n", X400msError (status));
	exit (status);
    }

    mp = NULL;
    rp = NULL;

    status = X400msClose (sp);
    exit (status);
    /* NOTREACHED */
}

static int add_binary_attachment (
    struct X400msMessage *mp,
    char * filename_to_send
) 
{
    int 	fd;
    int 	file_size;
    struct stat buf;
    int 	fs=0;
    int		status;

    printf("sending file %s\n", filename_to_send);
    if ((fd = open (filename_to_send, O_RDONLY)) == -1) {
	printf("Failed to open content file %s", filename_to_send);
	return X400_E_BADPARAM;
    }


    if (fstat(fd, &buf) != 0) {
	    close(fd);
	    printf("Can't fstat file %s %d",  filename_to_send, errno);
	    return X400_E_BADPARAM;
    }

    file_size = buf.st_size;
    printf("Content file size = %d\n", file_size);

    binary_data = (char *) malloc(file_size);
    if ( binary_data == NULL )
	return X400_E_NOMEMORY;
    if ((fs = read(fd, binary_data, file_size) ) == -1) {
	printf("Cannot read from binary file %d\n", errno);
	return (X400_E_SYSERROR);
    }
    close(fd);

    status = X400msMsgAddAttachment (mp, X400_T_BINARY, binary_data, fs);
    if ( status != X400_E_NOERROR ) {
	printf("failed to add X400_T_BINARY BP\n");
	return (status);
    }
    return (status);
}

static int add_fwd_bp (
    struct X400msMessage 	*mp,
    char 			*orig_orn,
    char 			*orig_dn
) 
{
    struct X400Message 		*x400_mp;
    struct X400Recipient 	*rp;
    int 			status;
    int 			num_atts = 0;

    printf("sending fwd bp \n");

    status = X400MsgNew (X400_MSG_MESSAGE, &x400_mp);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400MsgNew returned error: %s\n", X400msError (status));
	exit (status);
    }

    status = X400MsgAddStrParam(x400_mp, X400_S_MESSAGE_DELIVERY_TIME,
				"090909090909Z", -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400MsgAddStrParam returned error: %s\n", X400msError (status));
	exit (status);
    }

    status = X400MsgAddStrParam(x400_mp, X400_S_MESSAGE_SUBMISSION_TIME,
				"090909090909Z", -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400MsgAddStrParam returned error: %s\n", X400msError (status));
	exit (status);
    }

    /* envelope originator OR address */
    status = X400MsgAddStrParam (x400_mp, X400_S_OR_ADDRESS, orig_orn, -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400MsgAddStrParam returned error: %s\n", X400msError (status));
	exit (status);
    }
    status = X400MsgAddStrParam (x400_mp, X400_S_DIRECTORY_NAME, orig_dn, -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400MsgAddStrParam returned error: %s\n", X400msError (status));
	exit (status);
    }

    /* add originator into headers */
    status = X400RecipNew (X400_ORIGINATOR, &rp);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400RecipNew returned error: %s\n", X400msError (status));
	exit (status);
    }

    /* put in OR Address part of OR Name */
    status = X400RecipAddStrParam (rp, X400_S_OR_ADDRESS, recip, -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400RecipAddStrParam returned error: %s\n", X400msError (status));
	exit (status);
    }

    /* put in DN part of OR Name */
    status = X400RecipAddStrParam (rp, X400_S_DIRECTORY_NAME, orig_dn, -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msRecipAddStrParam returned error: %s\n", X400msError (status));
	exit (status);
    }

    /* put originator into message */
    status = X400MsgAddRecip (x400_mp, X400_ORIGINATOR, rp);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "X400MsgAddRecip returned error: %s\n", X400msError (status));
	exit (status);
    }
    printf("Put %s in as originator\n", orig_orn);

    /* add a recipient */
    status = X400RecipNew (X400_RECIP_STANDARD, &rp);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400RecipNew returned error: %s\n", X400msError (status));
	exit (status);
    }

    status = X400RecipAddStrParam (rp, X400_S_OR_ADDRESS, recip, -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400RecipAddStrParam returned error: %s\n", X400msError (status));
	exit (status);
    }

    status = X400RecipAddStrParam (rp, X400_S_DIRECTORY_NAME, "CN=recipient;c=gb", -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msRecipAddStrParam returned error: %s\n", X400msError (status));
	exit (status);
    }

    status = X400MsgAddRecip (x400_mp, X400_RECIP_STANDARD, rp);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "X400MsgAddRecip returned error: %s\n", X400msError (status));
	exit (status);
    }
    printf("Put %s in as reipient\n", recip);


    /* Add 2nd recipient */
    status = X400RecipNew (X400_RECIP_STANDARD, &rp);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400RecipNew returned error: %s\n", X400msError (status));
	exit (status);
    }

    status = X400RecipAddStrParam (rp, X400_S_OR_ADDRESS, recip, -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400RecipAddStrParam returned error: %s\n", X400msError (status));
	exit (status);
    }

    status = X400RecipAddStrParam (rp, X400_S_DIRECTORY_NAME, "CN=second recipient;c=gb", -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msRecipAddStrParam returned error: %s\n", X400msError (status));
	exit (status);
    }


    /* put recipient into message */
    status = X400MsgAddRecip (x400_mp, X400_RECIP_STANDARD, rp);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "X400MsgAddRecip returned error: %s\n", X400msError (status));
	exit (status);
    }
    printf("Put %s in as reipient\n", recip);


    /* envelope */

    /* Priority: 0 - normal, 1 - non-urgent, 2 - urgent */
    printf("message priority is %d ( 0 - normal, 1 - non-urgent, 2 - urgent)\n", 
	    x400_default_priority);
    status = X400MsgAddIntParam (x400_mp, X400_N_PRIORITY, x400_default_priority);
    if ( status != X400_E_NOERROR ) return (status);

    /* Priority: 0 - normal, 1 - non-urgent, 2 - urgent */
    printf("military message priority is %d ( 0 - low, 1 - high)\n", 
	    x400_default_priority);
    status = X400MsgAddIntParam (x400_mp, X400_N_MMTS_PRIORITY_QUALIFIER, 1);
    if ( status != X400_E_NOERROR ) return (status);

    /* content */
    /* subject */
    status = X400MsgAddStrParam (x400_mp, X400_S_SUBJECT, fwd_subject, -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400msMsgAddStrParam returned error: %s\n", X400msError (status));
	exit (status);
    }

    /* now an IA5 body part using the bodypart func  */
    status = X400MsgAddAttachment (x400_mp, X400_T_IA5TEXT, text, strlen(text));
    if ( status != X400_E_NOERROR ) {
	printf("failed to add X400_T_IA5TEXT BP\n");
	return (status);
    }
    num_atts++;

    /* 8859-1 attachment */
    status = X400MsgAddStrParam (x400_mp, X400_T_ISO8859_1, text, -1);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "x400ms returned error: %s\n", X400msError (status));
	exit (status);
    }
    num_atts++;

    status = X400MsgAddIntParam (x400_mp, X400_N_NUM_ATTACHMENTS, num_atts);
    if (status != X400_E_NOERROR ) 
	return (status);

    status = X400msMsgAddMessageBody (mp, x400_mp);
    if (status != X400_E_NOERROR ) {
	fprintf (stderr, "X400msMsgAddMessageBody returned error: %s\n", X400msError (status));
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
    printf("\t -f : Filename to transfer as binary bp\n");
    return;
}


