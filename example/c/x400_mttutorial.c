/*  Copyright (c) 2008, Isode Limited, London, England.
 *  All rights reserved.
 *                                                                       
 *  Acquisition and use of this software and related materials for any      
 *  purpose requires a written licence agreement from Isode Limited,
 *  or a written licence from an organisation licenced by Isode Limited
 *  to grant such a licence.
 *
 */

/* x400_mttutorial.c 
 *
 * The purpose of this file is to simply explain how to send and receive
 * X400 email messages using the Message Transfer (Gateway) API.
 *
 */

/*
 * Setting up your configuration.
 * Before you go any further you need to have a suitable configuration created
 * The simplist way to do this is to run the "quickconfig" program, and
 * select the "X.400" option as the configuration type.
 *
 * The "quickconfig" program is located within BINDIR. So that's typically
 * /opt/isode/bin on unix, and c:\Program Files\Isode\bin on Windows.
 *
 *
 * The purpose of this tutorial is to provide a very simple example of the 
 * basic principles of the Isode X.400api
 *
 * Specifically it demonstrates:
 * 1) The Object Oriented approach to manipulating X.400 Messages
 * 2) How to create a new "Session"
 * 3) How to create a new message associated with that session.
 * 4) How to add attributes to that Message.
 * 5) How to receive a Message.
 * 6) How to retrieve attributes from that Message.
 * 7) How to close a session.
 * 8) How to safely destroy a Message.
 *
 * This tutorial will only cover these basics.
 * the example X.400api programs give a more detailed example of how to use 
 * the X.400api.
 */

#include <stdio.h>
#include <stdlib.h>


#include <x400_mtapi.h>
#include "example.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

char *orig = "/CN=GatewayUser/OU=Sales/OU=dhcp-164/O=GatewayMTA/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
char *recip = "/CN=GatewayUser/OU=Sales/OU=dhcp-164/O=GatewayMTA/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";

static void send_hello_world(
	struct X400mtSession *sp
);

static void receive_hello_world(
    struct X400mtSession *sp
);

int main () 
{
  int status;
  struct X400mtSession *sp; /* This is the session pointer object.
			     * All susequent objects like message objects
			     * are associated back to this session 
			     * When the session is closed these objects are 
			     * free'd.
			     */
  
  /* Open a new session, and check that the session has been opened ok.
   * The different values the API can return are in x400_att.h */
  
  status = X400mtOpen (x400_channel, &sp);
  if ( status != X400_E_NOERROR ) {
    fprintf (stderr, "Error in Open: %s\n", X400mtError (status));
    exit (status);
  }
  
  /* We now want to configure logging
   * Isode logging is configured using a specific XML file.
   * a GUI editor SBINDIR/logconfig allows you to easily alter the XML file.
   *
   * To specify a particular xml file, we need to manipulate the
   * session object.
   *
   * To do this we call X400mtSetStrDefault, passing in the session
   * pointer, the attribute of the object we wish to manipulate 
   * (X400_S_LOG_CONFIGURATION_FILE), the filename, and the size in bytes
   * of the filename.
   *
   * Since we don't actually have the size of the filename in bytes, 
   * we can pass in 0 or -1 in for the size field. The underlying code will
   * calculate the size for us.
   */

  X400mtSetStrDefault(sp, X400_S_LOG_CONFIGURATION_FILE, "x400api.xml", 0);
 
  /* The reference section of the API manual contains descriptions of
   * other default attributes you may set.
   */
  
  /* We can now attempt to send a message */
  send_hello_world(sp);
  
 

  /* We can now attempt to receive a message */
  receive_hello_world(sp);

  X400mtClose (sp);
  if ( status != X400_E_NOERROR ) {
    fprintf (stderr, "Error in Close: %s\n", X400mtError (status));
    exit (status);
  }
  
  exit(0);
}


/*
 * 1) Sending a "Hello World!" email
 * 
 * You will need to know a single X.400 email addresses.
 * This email address needs to be routed to use the Gateway channel.
 * 
 * Fortunately quickconfig helps by providing a suitable user:
 * /CN=GatewayUser/OU=Sales/OU=HOSTNAME/O=GatewayMTA/PRMD=TestPRMD/ADMD=TestADMD/C=GB/
 * 
 * You should alter "HOSTNAME" to being the correct value for your 
 * configuration.
 *
 * The following section will show you how to send a "Hello World!" 
 * X.400 email througth the messag transfer API.
 */

static void send_hello_world(
    struct X400mtSession *sp
)
{
  int status;

  /* Now lets create a new message */
  struct X400mtMessage *mp; /* This is the pointer to a message object */
  struct X400Recipient *rp; /* This is the pointer to a new recipient  */


  printf("Now sending simple message\n");
  /* We now need to create a new message object, and have the new
   * message object associated with the session object.
   *
   * To do this we use X400mtMsgNew.
   */
  status = X400mtMsgNew (sp, X400_MSG_MESSAGE, &mp);
  
  if ( status != X400_E_NOERROR ) {
    fprintf (stderr, "Error in MsgNew: %s\n", X400mtError (status));
    exit (status);
  }
  
  /* We can now manipulate the message object */
  
  /* Add an originator. Similar concepts here,
   * we are manipulating the message object to add an originator address
   */
  status = X400mtMsgAddStrParam (mp, X400_S_OR_ADDRESS, orig, -1);
    
  if ( status != X400_E_NOERROR ) {
    fprintf (stderr, "Error adding orignator: %s\n", X400mtError (status));
    exit (status);
  }
  
  /* Create a new recipient object, and associate it with the message object.
   */
  status = X400mtRecipNew (mp, X400_RECIP_STANDARD, &rp);
  if ( status != X400_E_NOERROR ) {
    fprintf (stderr, "Error adding recipient: %s\n", X400mtError (status));
    exit (status);
  }

  /* We can now manipulate the recipient object and add the following:
   * 1) The OR address of the recipient.
   * Obviously this is important, otherwise the message cannot be routed.
   *
   * 2) The responsibility value 
   * You don't actually need to set this. However it is usefull to see
   * how the attributes being manipulated correspond with the standards.
   */
  status = X400mtRecipAddStrParam (rp, X400_S_OR_ADDRESS, recip, -1);
  if ( status != X400_E_NOERROR ) {
    fprintf (stderr, "Error adding recipient address: %s\n", 
	     X400mtError (status));
    exit (status);
  }
  
  /* The following attribute might see slightly confusing.
   * The best thing to do at this point is to take a brief look 
   * at X.411, section 12.2.1.1.1.6, this describes the responsibility
   * attribute.
   */

  status = X400mtRecipAddIntParam (rp, X400_N_RESPONSIBILITY, 1);
  if ( status != X400_E_NOERROR ) {
    fprintf (stderr, "Error adding recipient responsibility: %s\n", 
	     X400mtError (status));
    exit (status);
  }

  /* Extra envelope attributes */
  status = X400mtMsgAddIntParam (mp, X400_N_CONTENT_TYPE, 2);
  if ( status != X400_E_NOERROR ) {
    fprintf (stderr, "Error adding Content type : %s\n", 
	     X400mtError (status));
    exit (status);
  }
  
  /* And now for the content */
  {
    char *content = "Hello World!";
    char *subject = "A simple test message";

    status = X400mtMsgAddStrParam (mp,X400_T_IA5TEXT, content , -1);
    if ( status != X400_E_NOERROR ) {
      fprintf (stderr, "Error adding content : %s\n", 
	       X400mtError (status));
      exit (status);
    }

    status = X400mtMsgAddStrParam (mp, X400_S_SUBJECT, subject, -1);
     if ( status != X400_E_NOERROR ) {
       fprintf (stderr, "Error adding subject : %s\n", 
		X400mtError (status));
       exit (status);
     }
  }
  
  /* we should now be able to send the message */
  status = X400mtMsgSend (mp);
  if ( status != X400_E_NOERROR ) {
    fprintf (stderr, "Error in MsgSend: %s\n", X400mtError (status));
    exit (status);
  }
  
  /* We can now delete the message */
  status = X400mtMsgDelete (mp);
  if ( status != X400_E_NOERROR ) {
    fprintf (stderr, "Error in X400mtMsgDelete: %s\n", X400mtError (status));
    exit (status);
  }
  
  printf("Sent message\n");
}


static void receive_hello_world(
    struct X400mtSession *sp
)
{
  struct X400mtMessage *mp;
  int status;
  int type;
  char 	  buffer[BUFSIZ];
  size_t  length;

  printf("Now fetching message\n");

  status = X400mtMsgGetStart (sp, &mp, &type);
   if ( status != X400_E_NOERROR ) {
     fprintf (stderr, "Error in X400mtMsgGetStart: %s\n", 
	      X400mtError (status));
     exit (status);
   }

   switch ( type) {
   case X400_MSG_REPORT:
     printf("Got a report\n");
     exit(0);
   case X400_MSG_PROBE:
     printf("Got a probe\n");
     exit(0);
   case X400_MSG_MESSAGE:
     break;
   }
   
   /* Get the originator */
   status = X400mtMsgGetStrParam (mp, X400_S_OR_ADDRESS,
				  buffer, sizeof buffer , &length);
   if(status == X400_E_NOERROR) {
     printf ("Originator: %.*s\n",(int)length,buffer);
   } else if (status == X400_E_NOSPACE) {
     /* This is an interesting error.
      * It means that the size of buffer is too small for the originator 
      * address. The correct thing to do here is to create a buffer 
      * of size length bytes to hold the value.
      * 
      * All of the GetStrParam functions work in this way.
      */
     
     char * big_buff = NULL;
     big_buff = (char *) malloc((sizeof(char)) * length);
     status = X400mtMsgGetStrParam (mp, X400_S_OR_ADDRESS,
				    big_buff, length , &length);
     if (status != X400_E_NOERROR) {
       fprintf(stderr, "Error in getting originator address: %s\n", 
	       X400mtError (status));
       exit (status);
     }
     printf("Large Originator: %.*s\n",(int)length,big_buff);
     free(big_buff);

   } else {
     fprintf (stderr, "Error in getting originator address: %s\n", 
	      X400mtError (status));
     exit (status);
   }

   {
     /* Fetching recipients 
      *
      * Within the message there are 1 or more recipients.
      * so it is important to try to read through the whole list.
      *
      * All list objects work in this same way.
      */
     int n;
     struct X400Recipient *rp; /* Pointer to recipient object */
      for ( n = 1; ; n++ ) {
	/* Get the next recipient object with a "Get" command
	 *  Notice that I'm using the "X400_RECIP_ENVELOPE".
	 * IE this is fetching recipients from the envelope.
	 * We can fetch recipients from the message header as well
	 * Using X400_RECIP_PRIMARY / X400_RECIP_CC / ETC
	 */
	status = X400mtRecipGet (mp, X400_RECIP_ENVELOPE, n, &rp);
	if ( status == X400_E_NO_RECIP ) {
	  printf("Got final recipient\n");
	    break;
	} else if ( status != X400_E_NOERROR ) {
	    fprintf (stderr, "Error fetching recipients: %s\n", 
		X400mtError (status));
	    exit(status);
	}
	
	/* Now we have a recipient object that represents a single 
	   recipient. 
	*/
	
	status = X400mtRecipGetStrParam (rp, X400_S_OR_ADDRESS,
					 buffer, BUFSIZ, &length);
	if ( status == X400_E_NOERROR ) {
	    printf ("%s recipient %d: %.*s\n", buffer, n, 
		    (int)length, buffer);
	} else {
	  fprintf (stderr, "Error fetching OR Address: %s\n", 
		   X400mtError (status));
	  exit (status);
	}

	/* It is possible to continue to try to obtain other recipient 
	   information. However lets keep it simple here
	*/

      }
   }/* end of fetch recips*/

   /* Now lets obtain the subject of the message */
   /* Subject */
    status = X400mtMsgGetStrParam (mp, X400_S_SUBJECT,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR ) {
      printf ("Subject: %.*s\n", (int)length, buffer);
    } else {
      fprintf (stderr, "Error fetching subject: %s\n", 
		   X400mtError (status));
      exit (status);
    }
    /* Followed by the message content (Hello World).
     * The message we just created contained an ia5-text bodypart
     * so lets request that
     */
    
    status = X400mtMsgGetStrParam (mp, X400_T_IA5TEXT,
				   buffer, sizeof buffer , &length);
    if ( status == X400_E_NOERROR ) {
      printf ("Text:\n%.*s\n", (int)length, buffer);
    } else {
      fprintf (stderr, "Error fetching ia5-text bodypart: %s\n", 
		   X400mtError (status));
      exit (status);
    }

    /* Now that we've got the message we need to let the MTA know the message
     * has been transfered ok*/
    status = X400mtMsgGetFinish (mp, X400_E_NOERROR, -1, -1, "");
    if ( status != X400_E_NOERROR ) {
	printf("X400mtMsgFinish returned error %d\n", status);
	fprintf (stderr, "Error in X400mtMsgFinish: %s\n", 
	    X400mtError (status));
    }

    /* And now we can delete our own copy of the message */
    status = X400mtMsgDelete (mp);
    if ( status != X400_E_NOERROR ) {
	printf("X400mtMsgDelete returned error %d\n", status);
	fprintf (stderr, "Error in X400mtMsgDelete: %s\n", 
	    X400mtError (status));
    }
    
}

