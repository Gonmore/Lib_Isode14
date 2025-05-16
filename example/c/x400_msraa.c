/*  Copyright (c) 2005-2008, Isode Limited, London, England.
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
 * Simple example program for registering an auto-action in the message store
 */


#include <stdio.h>
#include <x400_msapi.h>
#include "example.h"

static void usage(void);

static char *optstr = "u37m:d:p:w:M:D:P:W:";

/*! Main function of mhsrcv
 * \return 0 on success, otherwise the value of status from X400 call
 */
int main (
    int         argc,
    char      **argv
) 
{
    char buffer[BUFSIZ];
    char pa[BUFSIZ];
    char orn[BUFSIZ];
    char password[BUFSIZ];
    int status;
    int nummsg;
    struct X400msSession *sp;
    struct X400Recipient *rp;
    int contype;
    char *def_oraddr;
    char *def_dn;
    char *def_pa;
    char autoaction;
    
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
    strcpy(password, buffer);

    /* Presentation address */
    printf("Presentation Address [%s] > ", def_pa);
    ic_fgets (pa, sizeof pa, stdin);

    if ( pa[strlen(pa)-1] == '\n' )
	pa[strlen(pa)-1] = '\0';

    if (pa[0] == '\0') 
	strcpy(pa, def_pa);
 
    /* Register or deregister the autoaction */
    printf("Register or Deregister the AutoAction ? (R = Register, D = Deregister): ");
    if ( ic_fgets (buffer, sizeof buffer, stdin) == NULL )
       exit (1);
    autoaction = buffer[0];
    if ((autoaction != 'R') && (autoaction != 'D'))
      exit(1);

    /* open the session */
    status = X400msOpen (contype, orn, def_dn, password, pa, &nummsg, &sp);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "Error in Open: %s\n", X400msError (status));
	exit (status);
    }

    /* setup logging from $(ETCDIR)x400api.xml or $(SHAREDIR)x400api.xml */
    X400msSetStrDefault(sp, X400_S_LOG_CONFIGURATION_FILE, "x400api.xml", 0);

    /* If we register the alert auto-action, we will get an alert indication
       when a message is delivered. So there is no need to poll at
       short intervals within X400ms_Wait - we can do a slow background
       poll and rely on the Alert indication to wake the code up instead */
    X400msSetIntDefault(sp, X400_N_WAIT_INTERVAL, 1000);


    if (contype == 0) {
	struct X400msAutoActionParameter *aa_param;

	/* Register an Autoforwarding Autoaction. */
	aa_param = X400msNewAutoActionParameter();

	/* Add mandatory things to AutoAction parameter for auto-forwarding:
	   i.e. recipient address */
	X400RecipNew(0,&rp);

	X400RecipAddStrParam(rp, X400_S_OR_ADDRESS, def_oraddr, 
			     strlen(def_oraddr));

	X400msAutoActionParameterAddRecip(aa_param,
					  X400_RECIP_STANDARD, rp);

	X400msAutoActionParameterAddStrParam(aa_param,
					     X400_S_CONTENT_IDENTIFIER,
					     "AF contentid",
					     -1);

	X400msAutoActionParameterAddIntParam(aa_param,
					     X400_N_DISCLOSURE,
					     1);

	X400msAutoActionParameterAddIntParam(aa_param,
					     X400_N_IMPLICIT_CONVERSION_PROHIBITED,
					     1);

	X400msAutoActionParameterAddIntParam(aa_param,
					     X400_N_ALTERNATE_RECIPIENT_ALLOWED,
					     1);

	X400msAutoActionParameterAddIntParam(aa_param,
					     X400_N_CONTENT_RETURN_REQUEST,
					     1);

	X400msAutoActionParameterAddIntParam(aa_param,
					     X400_N_PRIORITY,
					     2);

	X400msAutoActionParameterAddStrParam(aa_param,
					     X400_S_AUTO_FORWARDING_COMMENT,
					     "This message was autoforwarded",
					     -1);

	X400msAutoActionParameterAddStrParam(aa_param,
					     X400_S_COVER_NOTE,
					     "This is a cover note",
					     -1);

	X400msAutoActionParameterAddStrParam(aa_param,
					     X400_S_THIS_IPM_PREFIX,
					     "AutoForwarded:",
					     -1);

	status = X400msRegisterAutoAction (sp, X400_AUTO_FORWARDING, 
					   4, aa_param);
	if ( status != X400_E_NOERROR ) {
	    fprintf (stderr, 
		     "Error in RegisterAutoAction: %s\n", X400msError (status));
	    exit (status);
	}
	printf("Registered AutoForwarding autoaction (id = 4) OK\n");
	X400msFreeAutoActionParameter(aa_param);



	/* No parameter needed for Alert autoaction - we do not support 
	   configuration of requested-attributes in this API yet. */
	status = X400msRegisterAutoAction (sp, X400_AUTO_ALERT, 9, NULL);
	if ( status != X400_E_NOERROR ) {
	    fprintf (stderr, "Error in RegisterAutoAction: %s\n", 
		     X400msError (status));
	    exit (status);
	}
	printf("Registered AutoAlert autoaction (id = 9) OK\n");


	/* Just test the register and deregister functions */
	status = X400msRegisterAutoAction (sp, X400_AUTO_ALERT, 10, NULL);
	if ( status != X400_E_NOERROR ) {
	    fprintf (stderr, "Error in RegisterAutoAction: %s\n", 
		     X400msError (status));
	    exit (status);
	}
	printf("Registered AutoAlert autoaction (id = 10) OK\n");

	/* Lets do a deregistration of the action we just registered */
	status = X400msDeregisterAutoAction (sp, X400_AUTO_ALERT, 10);
	if ( status != X400_E_NOERROR ) {
	    fprintf (stderr, "Error in DeregisterAutoAction: %s\n", 
		     X400msError (status));
	    exit (status);
	}
	printf("Deregistered AutoAlert autoaction (id = 10) OK\n");
    }

    if ( nummsg == 0 ) {
      printf("no messages - waiting 60 seconds for a message to be delivered.....\n");
    } else {
      printf ("%d messages waiting\n", nummsg);
    }


    status = X400msClose (sp);
    return (status);
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

