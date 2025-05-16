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
 * Simple example program for listing the contents of an X.400 Message Store mailbox.
 */

#include <stdio.h>
#include <stdlib.h>

#include <x400_msapi.h>
#include "example.h"

static void usage(void);

static char *optstr = "371um:p:d:w:t:znT:";

int main (
    int         argc,
    char      **argv
) 
{
    char buffer[BUFSIZ];
    char pa[BUFSIZ];
    char orn[BUFSIZ];
    int status;
    struct X400msSession *sp;
    int contype;
    char *def_oraddr;
    char *def_dn;
    char *def_pa;
    int i;
    struct X400msListResult *lr;

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

    printf("Presentation Address [%s] > ", def_pa);
    ic_fgets (pa, sizeof pa, stdin);

    if ( pa[strlen(pa)-1] == '\n' )
	pa[strlen(pa)-1] = '\0';

    if (pa[0] == '\0') 
	strcpy(pa, def_pa);
 

    if (talking_to_marben_ms)
	X400msSetConfigRequest(0);

    status = X400msOpen (contype, orn, def_dn, buffer, pa, NULL, &sp);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "Error in Open: %s\n", X400msError (status));
	exit (status);
    }

    if (talking_to_marben_ms)
	X400msSetIntDefault(sp, X400_N_STRICT_P7_1988, 1);

    /* setup logging from $(ETCDIR)x400api.xml or $(SHAREDIR)x400api.xml */
    X400msSetStrDefault(sp, X400_S_LOG_CONFIGURATION_FILE, "x400api.xml", -1);

    for (i = 0; attrs[i] != NULL; i++) {
	X400msSetStrDefault(sp, X400_S_LIST_ATTR + i, attrs[i], -1);
    }

    /* Now list messages, since we have a P7 connection */
    
    /* List messages since a specific time and date - 20/01/2005, 12:05:06
       in this case. Specify NULL string instead to list all of them */

    status = X400msListExAux(sp, since_time, 
			     (submitted_messages == 1) ?
			     MS_ENTRY_CLASS_SUBMITTED_MESSAGES : 
			     MS_ENTRY_CLASS_STORED_MESSAGES,
			     (new_messages == 1) ? 0 : -1,
			     &lr);
    if (status != X400_E_NOERROR) {
      fprintf (stderr, "X400msList returned status: %s\n", 
	       X400msError (status));
      exit(1);
    }
    
    i = 1;

    /* Traverse list result, extracting information about each message */
    while (status == X400_E_NOERROR) {
      int n;
      int seqnum;
      int substat;
      char buf[1024];
      size_t len;
      
      /* Get sequence number */
      status = X400msListGetIntParam(lr, X400_N_MS_SEQUENCE_NUMBER, i, 
				     &seqnum);
      if (status == X400_E_NOERROR)
	printf("\nSequence number = %d\n", seqnum);
      else if (status == X400_E_NO_MORE_RESULTS)
	break;
      
      /* Get type of entry */
      substat = X400msListGetStrParam(lr,
				      X400_S_OBJECTTYPE,
				      i,
				      buf,
				      1024,
				      &len);
      
      if (substat == X400_E_NOERROR) {
	printf("Object type = %s\n", buf);
      } else {
	printf("No object type present \n");
      }
      
      /* Get Subject field, if any */
      substat = X400msListGetStrParam(lr,
				      X400_S_SUBJECT,
				      i,
				      buf,
				      1024,
				      &len);
      
      if (substat == X400_E_NOERROR) {
	printf("Subject = %s\n", buf);
      } else {
	printf("No subject present \n");
      }
      
      /* Get sender address */
      substat = X400msListGetStrParam(lr,
				      X400_S_OR_ADDRESS,
				      i,
				      buf,
				      1024,
				      &len);
      
      if (substat == X400_E_NOERROR) {
	printf("Sender = %s\n", buf);
      } else {
	printf("No sender present \n");
      }
      
      substat = X400msListGetStrParam(lr,
				      X400_S_MESSAGE_IDENTIFIER,
				      i,
				      buf,
				      1024,
				      &len);
      
      if (substat == X400_E_NOERROR) {
	printf("Msg Id = %s\n", buf);
      } else {
	printf("No Msg Id present \n");
      }
      
      substat = X400msListGetStrParam(lr,
				      X400_S_SUBJECT_IDENTIFIER,
				      i,
				      buf,
				      1024,
				      &len);
      
      if (substat == X400_E_NOERROR) {
	printf("Subject Id = %s\n", buf);
      } else {
	printf("No Subject Id present \n");
      }
      
      /* Priority */
      substat = X400msListGetIntParam(lr, X400_N_PRIORITY, i, &n);
      if (substat == X400_E_NOERROR) {
	printf("Priority = %d\n", n);
      } else {
	printf("No priority present \n");
      }
      
      /* Length */
      substat = X400msListGetIntParam(lr, X400_N_CONTENT_LENGTH, i, 
				      &n);
      if (substat == X400_E_NOERROR) {
	printf("Content length = %d\n", n);
      } else {
	printf("No content length present \n");
      }
      
      substat = X400msListGetStrParam(lr, X400_S_EXTERNAL_CONTENT_TYPE, i, 
				      buf, 1024, &len);
      if (substat == X400_E_NOERROR) {
	printf("Content type = %s\n", buf);
      } else {
	printf("No content type present \n");
      }

      /* Entry status */
      substat = X400msListGetIntParam(lr, X400_N_MS_ENTRY_STATUS, i, 
				      &n);
      if (substat == X400_E_NOERROR) {
	if ( n == X400_MS_ENTRY_STATUS_NEW ) {
	  printf("MS entry status = %d, (New message)\n", n);
	} else if ( n == X400_MS_ENTRY_STATUS_LISTED ) {
	  printf("MS entry status = %d, (Message listed)\n", n);
	} else if ( n == X400_MS_ENTRY_STATUS_FETCHED ) {
	  printf("MS entry status = %d, (Message fetched)\n", n);
	} else {
	  printf("MS entry status = %d, (Unknown status)\n", n);
	}
      } else {
	printf("No entry status present \n");
      }
      
      i++;
    }
    
    X400msListFree(lr);

    status = X400msClose (sp);
    return (status);
}

static void usage(void) {
    printf("usage: %s\n", optstr);
    printf("\t where:\n");
    printf("\t -u : Don't prompt to override defaults \n");
    printf("\t -m : OR Address in P7 bind arg \n");
    printf("\t -p : Presentation Address of P7 Store \n");
    printf("\t -d : DN in P7 bind arg \n");
    printf("\t -w : P7 password of P7 user \n");
    printf("\t -z : List submitted messages\n");
    printf("\t -n : List messages with status 'new' only\n");
    printf("\t -T : List messages with delivery time after <UTCTime>\n");
    return;
}

