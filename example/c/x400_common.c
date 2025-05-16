/*  Copyright (c) 2004-2009, Isode Limited, London, England.
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

#include "example.h"
#include <x400_api.h>
#include <x400_att.h>
#include <stdio.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	opterr = 1;	/* if error message should be printed */
int	optind = 1;	/* index into parent argv vector */
int	optopt;		/* character checked for validity */
int	optreset;	/* reset getopt */
char	*optarg;	/* argument associated with option */

#define	BADCH	(int)'?'
#define	BADARG	(int)':'
#define	EMSG	""

/*
 * getopt --
 *	Parse argc/argv argument vector.
 *
 */
int getopt (int nargc, char *const *nargv, const char *ostr)
{
	static char *progname;
	static char *place = EMSG;		/* option letter processing */
	char *oli;				/* option letter list index */

	if (!progname) {
		if ((progname = strrchr(*nargv, '/')) == NULL)
	                progname = *nargv;
        	else
                	++progname;
	}

	if (optreset || !*place) {		/* update scanning pointer */
		optreset = 0;
		if (optind >= nargc || *(place = nargv[optind]) != '-') {
			place = EMSG;
			return (EOF);
		}
		if (place[1] && *++place == '-') {	/* found "--" */
			++optind;
			place = EMSG;
			return (EOF);
		}
	}					/* option letter okay? */
	if ((optopt = (int)*place++) == (int)':' ||
	    !(oli = strchr(ostr, optopt))) {
		/*
		 * if the user didn't specify '-' as an option,
		 * assume it means EOF.
		 */
		if (optopt == (int)'-')
			return (EOF);
		if (!*place)
			++optind;
		if (opterr && *ostr != ':')
			(void)fprintf(stderr,
			    "%s: illegal option -- %c\n", progname, optopt);
		return (BADCH);
	}
	if (*++oli != ':') {			/* don't need argument */
		optarg = NULL;
		if (!*place)
			++optind;
	}
	else {					/* need an argument */
		if (*place)			/* no white space */
			optarg = place;
		else if (nargc <= ++optind) {	/* no arg */
			place = EMSG;
			if (*ostr == ':')
				return (BADARG);
			if (opterr)
				(void)fprintf(stderr,
				    "%s: option requires an argument -- %c\n",
				    progname, optopt);
			return (BADCH);
		}
	 	else				/* white space */
			optarg = nargv[optind];
		place = EMSG;
		++optind;
	}
	return (optopt);			/* dump back option letter */
}

#define HOSTNAME "dhcp-164"
#define FQ_HOSTNAME ""HOSTNAME".isode.net"

/* use defaults without prompting */
int use_defaults = 0;

/* P3/P7 */
int x400_contype = 0;

/* Values used when binding to the Message Store. */
char *x400_ms_user_addr = "/CN=P7User2/OU=Sales/O="HOSTNAME"/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
char *x400_ms_user_dn = "cn="HOSTNAME",c=gb";
char *x400_ms_presentation_address = "\"3001\"/Internet="FQ_HOSTNAME"+3001";
char *x400_ms_password = "secret";

/* Password we expect the Store to pass back in its bind response */
char *x400_p7_password = "secret";

/* Values used when binding to the MTA directly over P3. */
char *x400_mta_user_addr = "/CN=P3User1/OU=Sales/O="HOSTNAME"/PRMD=TestPRMD/ADMD=TestAD/C=GB/";
char *x400_mta_user_dn = NULL;
char *x400_mta_presentation_address = "\"593\"/Internet="FQ_HOSTNAME;
char *x400_p3_password = "p3secret";

/* Password we expect the MTA to pass back in its bind response */
char *x400_mta_password = "p3secret";

/* Default values which can be set for message submission via the MS interface.
   If values are supplied via attributes for a particular message, these
   defaults will be overridden. */
char *x400_default_originator_return_address = "cn=lppt1;c=gb $ /CN=P7User2/OU=Sales/O="HOSTNAME"/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
int x400_default_priority = 0;
int x400_default_content_type = 2;
int x400_default_implicit_conversion_prohibited = 0;
int x400_default_alternate_recipient_allowed = 0;
int x400_default_content_return_request = 0;
int x400_default_disclosure_of_recipients = 1;
int x400_default_recipient_reassignment_prohibited = -1;
int x400_default_dl_expansion_prohibited = -1;
int x400_default_conversion_with_loss_prohibited = -1;
char *x400_default_originator 
    = "/CN=P7User2/OU=Sales/O="HOSTNAME"/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
char *x400_default_recipient 
    = "/CN=P7User2/OU=Sales/O="HOSTNAME"/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
char *x400_default_gw_originator 
    = "/CN=GatewayUser/OU=Sales/OU="HOSTNAME"/O=GatewayMTA/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
char *x400_default_gw_recipient 
    = "/CN=GatewayUser/OU=Sales/OU="HOSTNAME"/O=GatewayMTA/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
char *x400_default_gw_dlexphist1_or
    = "/CN=DLExphistuser1/OU=Sales/OU="HOSTNAME"/O=GatewayMTA/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
char *x400_default_gw_dlexphist2_or
    = "/CN=DLExphistuser2/OU=Sales/OU="HOSTNAME"/O=GatewayMTA/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
char *x400_default_gw_dlexphist1_dn = "CN=DLExphistuser1,c=GB";
char *x400_default_gw_dlexphist2_dn = "CN=DLExphistuser2,c=GB";

char *x400_default_exempted_recipient = "/CN=exemptedrecip/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";

char *x400_default_fwd_originator = "/CN=P7User2/OU=Sales/O="HOSTNAME"/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
char *x400_default_fwd_recipient  = "/CN=P7User2/OU=Sales/O="HOSTNAME"/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";

char *x400_channel = "x400mt";
char *x400_logline = NULL;
int x400_reports = 1;
int new_messages = 0;
int submitted_messages = 0;
char *since_time = NULL;
int attr_to_fault = 0;

/* Security env */
#if _WIN32
char *security_id = "c:/isode/share";
#else
char *security_id = "/opt/isode/share";
#endif
char *passphrase = "secret";
char *identity_dn = "cn=AMHS Tester, cn=isode, c=GB";
char *identity_dn2 = "cn=AMHS Tester2, cn=isode, c=GB";
int sign_message = 0;
char *attrs[100];
int ati = 0;
int talking_to_marben_ms = 0;

/* filename to transfer as binary attachement */
char *filename_to_send = NULL;
/* number of bytes from filename to put into binary file */
int bin_bp_size = 10000;

/* number of messages to submit/transfer */
int num_msgs_to_send = 1;

/* keep going if no msgs to retrieve ? */
int until_no_more_msgs = 0;

int get_args( 
    int         argc,
    char      **argv,
    char       *optstr
)
{
    int opt;

    while ((opt = getopt (argc, argv, optstr)) != EOF) {
	switch (opt) {
	    case 't':
		attrs[ati] = optarg;
		ati++;
		attrs[ati] = NULL;
		break;
	    case 'u':
		use_defaults = 1;
	    break;
	    case 'n':
		new_messages = 1;
	    break;
	    case 'z':
		submitted_messages = 1;
	    break;
	    case '3':
		x400_contype = 1;
	    break;
	    case '7':
		x400_contype = 0;
	    break;
	    case '1':
	        talking_to_marben_ms = 1;
 	    break;
 	    case 'm':
		x400_ms_user_addr = optarg;
	    break;
	    case 'd':
		x400_ms_user_dn = optarg;
	    break;
	    case 'p':
		x400_ms_presentation_address = optarg;
	    break;
	    case 'w':
		x400_p7_password = optarg;
	    break;
	    case 'M':
		x400_mta_user_addr = optarg;
	    break;
	    case 'D':
		x400_mta_user_dn = optarg;
	    break;
	    case 'P':
		x400_mta_presentation_address = optarg;
	    break;
	    case 'W':
		x400_p3_password = optarg;
	    break;
	    case 'o':
		x400_default_originator = optarg;
	    	printf("Set default orig to %s\n", x400_default_originator);
	    break;
	    case 'O':
		x400_default_originator_return_address = optarg;
	    break;
	    case 'r':
		x400_default_recipient = optarg;
	    break;
	    case 'g':
		x400_default_gw_recipient = optarg;
	    break;
	    case 'G':
		x400_default_gw_originator = optarg;
	    break;
	    case 'c':
		x400_channel = optarg;
	    break;
	    case 'l':
		x400_logline = optarg;
	    break;
	    case 'R':
		x400_reports = atoi(optarg);
		if (x400_reports < 0 || x400_reports > 2) {
		    printf("bad report_val '%d'\n", x400_reports);
		    return(1);
		}
	    break;
	    case 'y':
		x400_default_priority =  atoi(optarg);
		if (x400_default_priority < 0 || x400_default_priority > 2) {
		    printf("bad priority '%d'\n", x400_default_priority);
		    return (1);
		}
	    break;
	    case 'C':
		x400_default_content_type =  atoi(optarg);
	    break;
	    case 'i':
		x400_default_implicit_conversion_prohibited = 1;
	    break;
	    case 'a':
		x400_default_alternate_recipient_allowed = 1;
	    break;
	    case 'q':
		x400_default_content_return_request = 1;
	    break;
	    case 's':
		x400_default_disclosure_of_recipients = 0;
	    break;
	    case 'A':
		x400_default_recipient_reassignment_prohibited = 0;
	    break;
	    case 'v':
		x400_default_conversion_with_loss_prohibited = 0;
	    break;
	    case 'e':
		security_id = optarg;
		sign_message = 1;
	    break;
	    case 'x':
		identity_dn = optarg;
		sign_message = 1;
	    break;
	    case 'b':
		passphrase = optarg;
		sign_message = 1;
	    break;
	    case 'f':
		filename_to_send = optarg;
	    break;
	    case 'X':
		num_msgs_to_send = atoi(optarg);
	    break;
	    case 'S':
		bin_bp_size = atoi(optarg);
	    break;
	    case 'E':
		until_no_more_msgs = 1;
	    break;
	case 'T':
	    since_time = optarg;
	    break;
	    
	case 'Q':
	    attr_to_fault = atoi(optarg);
	    break;

	    default:
	    	printf("bad arg '%c'\n", opt);
	    	return 1;
	}
    }
    return 0;
}

/* wrappers for standard C calls which can return a default */
int ic_fgetc(int default_char, FILE *stream)
{
    if (use_defaults)
        return default_char;
    else
        return fgetc (stream);

}

char *ic_fgets(char *s, int size, FILE *stream)
{
    if (use_defaults) {
	*s = 0;
        return s;
    } else {
        return fgets(s, size, stream);
    }
}



