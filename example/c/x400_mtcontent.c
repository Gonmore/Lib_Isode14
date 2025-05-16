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
 * Test harness for MT Content manipulation functions
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

static int get_a_msg (struct X400mtSession *sp);

static char *optstr = "uo:O:r:c:l:EQ:";

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

static int map[] = { X400_S_SUBJECT,
		     X400_S_IPM_IDENTIFIER,
		     X400_N_IMPORTANCE,
		     X400_ORIGINATOR,
		     -1 };


static int get_a_msg (struct X400mtSession *sp)
{
    struct X400mtMessage *mp, *new1, *new2, *new3;
    int type;
    int status;
    static int reported_none = 0;
    char *buf = NULL;
    size_t buflen = 0;
    size_t required = 0;
    char tbuf[1024];
    int i;
    
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
	fprintf (stderr, "Error in X400mtMsgGet: %s\n", X400mtError (status));
	return (status);
    }

    /* Create new message object */
    X400mtMsgNew(sp, X400_MSG_MESSAGE, &new1);
    
#define INITIAL_BUFSIZE (10 * 1024)

    /* Extract content from original message */
    buf = (char*)malloc(INITIAL_BUFSIZE);
    buflen = INITIAL_BUFSIZE;

    /* Get CONTENT as a string from the original message */
    status = X400mtMsgGetStrParam(mp, X400_S_CONTENT_STRING, buf, buflen, &required);
    if ((status != X400_E_NOERROR) && (status != X400_E_NOSPACE)) {
	fprintf (stderr, "Error in X400mtGetStrParam: %s\n", X400mtError (status));
	free(buf);
	return (status);
    }

    if (status == X400_E_NOSPACE) {
	buf = (char *)realloc(buf, required);
	buflen = required;
	
	status = X400mtMsgGetStrParam(mp, X400_S_CONTENT_STRING, buf, buflen, &required);
	if (status != X400_E_NOERROR) {
	    fprintf (stderr, "Error in X400mtGetContentOctets: %s\n", X400mtError (status));
	    free(buf);
	    return (status);
	}
    }
    
    /* Finished with original message now */
    status = X400mtMsgGetFinish(mp, X400_E_NOERROR, 0, 0, NULL);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "Error in X400mtMsgGetFinish: %s\n", X400mtError (status));
	return (status);
    }
    
    X400mtMsgDelete(mp);

    /* Here we would mangle it somehow  - e.g extract P772 content from inside
     something else which is wrapping it up.*/

    /* Put content into new message */
    status = X400mtSetContentOctets(new1, buf, required);
    if ((status != X400_E_NOERROR) && (status != X400_E_NOSPACE)) {
	fprintf (stderr, "Error in X400mtSetContentOctets: %s\n", X400mtError (status));
	free(buf);
	return (status);
    }
    free(buf);

    /* Create yet another new message !*/
    X400mtMsgNew(sp, X400_MSG_MESSAGE, &new2);

    for (i = 0; map[i] != -1; i++) {
	int v;

	switch (map[i]) {
	case X400_S_SUBJECT:
	case X400_S_IPM_IDENTIFIER:
	    if (X400mtMsgGetStrParam(new1, map[i], 
				     tbuf, sizeof(tbuf), &required) != X400_E_NOERROR) {
		fprintf (stderr, "Error in X400mtMsgGetStrParam: %s\n", X400mtError (status));
	    } else {
		tbuf[required] = '\0';
		printf("Param type %d from copy message = %s\n", map[i], tbuf);

		if (map[i] == X400_S_SUBJECT) {
		    char *c;
		    /* Alter subject as example */
		    c = strstr(tbuf, "Test Message");
		    if (c != NULL)
			strcat(c, "REDACTED!!!!");
		}
		
		/* Copy attribute into new message object */
		X400mtMsgAddStrParam(new2, map[i], tbuf, -1);
	    }
	    break;

	case X400_N_IMPORTANCE:
	    if (X400mtMsgGetIntParam(new1, map[i], &v) != X400_E_NOERROR) {
		fprintf (stderr, "Error in X400mtMsgGetIntParam: %s\n", X400mtError (status));
	    } else {
		printf("Param type %d from copy message = %d\n", map[i], v);

		/* Copy attribute into new message object */
		X400mtMsgAddIntParam(new2, map[i], v);
	    }
	    break;

	case X400_ORIGINATOR:
	    {
		struct X400Recipient *rp;

		if (X400mtRecipGet(new1, map[i], 1, &rp) == X400_E_NOERROR) {
		    struct X400Recipient *nrp;

		    if ((status = X400mtRecipNew(new2, X400_ORIGINATOR, &nrp)) != X400_E_NOERROR) {
			fprintf (stderr, "Error in X400mtRecipNew: %s\n", X400mtError (status));
		    } else {
			if (X400mtRecipGetStrParam(rp, X400_S_OR_ADDRESS, tbuf, 
						   sizeof(tbuf), &required) == X400_E_NOERROR) {
			    tbuf[required] = '\0';
			    X400mtRecipAddStrParam(nrp, X400_S_OR_ADDRESS, tbuf, -1);
			}
		    }
		}
	    }
	    break;

	default:
	    /* Can't do these yet */
	    break;
	}
    }

    /* Done with this one now */
    X400mtMsgDelete(new1);

    buf = (char*)malloc(INITIAL_BUFSIZE);
    buflen = INITIAL_BUFSIZE;

    /* Now get content octets for this new message */
    status = X400mtGetContentOctets(new2, buf, buflen, &required);
    if ((status != X400_E_NOERROR) && (status != X400_E_NOSPACE)) {
	fprintf (stderr, "Error in X400mtGetContentOctets: %s\n", X400mtError (status));
	free(buf);
	return (status);
    }

    if (status == X400_E_NOSPACE) {
	buf = (char *)realloc(buf, required);
	buflen = required;
	
	status = X400mtGetContentOctets(new2, buf, buflen, &required);
	if (status != X400_E_NOERROR) {
	    fprintf (stderr, "Error in X400mtGetContentOctets: %s\n", X400mtError (status));
	    free(buf);
	    return (status);
	}
    }

    /* Now finished wih this one */
    X400mtMsgDelete(new2);
    
    /* Here we would wrap the content up somehow */

    /* And put the resulting stuff back into yet another message for transfer-in  */
    X400mtMsgNew(sp, X400_MSG_MESSAGE, &new3);

    status = X400mtMsgAddStrParam(new3, X400_S_CONTENT_STRING, buf, required);
    free(buf);

    if (status != X400_E_NOERROR) {
	fprintf (stderr, "Error in X400mtAddStrParam: %s\n", X400mtError (status));
	return (status);
    }

    /* Set up envelope attributes and transfer out to MTA */

    X400mtMsgDelete(new3);

    return X400_E_NOERROR;
}
