/*  Copyright (c) 2008, Isode Limited, London, England.
 *  All rights reserved.
 *                                                                       
 *  Acquisition and use of this software and related materials for any      
 *  purpose requires a written licence agreement from Isode Limited,
 *  or a written licence from an organisation licenced by Isode Limited
 *  to grant such a licence.
 *
 */

/* x400_mstutorial.c 
 *
 * The purpose of this file is to simply explain how to send and receive
 * X400 email messages using the P7 Message store protocol.
 *
 */

/*
 * Setting up your configuration.
 * Before you go any further you need to have a suitable configuration created
 * The simplest way to do this is to run the "quickconfig" program, and
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
 *
 *
 * 
 */

#include <stdio.h>
#include <stdlib.h>


#include <x400_msapi.h>
#include "example.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

/* You will need to change these two #defines to the correct values for your
 * system */
#define HOSTNAME "dhcp-164"
#define FQ_HOSTNAME HOSTNAME".isode.net"

char *orig = "/CN=P7User1/OU=Sales/O="HOSTNAME"/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
char *recip = "/CN=P7User2/OU=Sales/O="HOSTNAME"/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
char *ms_pa = "\"3001\"/Internet="FQ_HOSTNAME"+3001"; /*Message Store presentation address */

char *password = "secret"; /* password used to bind to the store */

static void send_hello_world(
	struct X400msSession *sp
);

static void receive_msgs(
    struct X400msSession *sp,
    int nummsg
);

int main () 
{
  int status;
  struct X400msSession *sp; /* This is the session pointer object.
			     * All subsequent objects like message objects
			     * are associated back to this session 
			     * When the session is closed these objects are 
			     * free'd.
			     */
  int contype = 0; /* We are using P7 - connecting to the P7 Message store */
  char *def_dn= "cn=foobar,c=gb";

  int nummsg = 0;
  
  /* Open a new session, and check that the session has been opened ok.
   * The different values the API can return are in x400_att.h */
  
  status = X400msOpen (contype, orig, def_dn, password, ms_pa, &nummsg, &sp);
  if ( status != X400_E_NOERROR ) {
      fprintf (stderr, "Error in Open: %s\n", X400msError (status));
      exit (status);
  }
  
  /* We now want to configure logging
   * Isode logging is configured using a specific XML file.
   * a GUI editor SBINDIR/logconfig allows you to easily alter the XML file.
   *
   * To specify a particular xml file, we need to manipulate the
   * session object.
   *
   * To do this we call X400msSetStrDefault, passing in the session
   * pointer, the attribute of the object we wish to manipulate 
   * (X400_S_LOG_CONFIGURATION_FILE), the filename, and the size in bytes
   * of the filename.
   *
   * Since we don't actually have the size of the filename in bytes, 
   * we can pass in 0 or -1 in for the size field. The underlying code will
   * calculate the size for us.
   */

  X400msSetStrDefault(sp, X400_S_LOG_CONFIGURATION_FILE, "x400api.xml", 0);
 
  /* The reference section of the API manual contains descriptions of
   * other default attributes you may set.
   */
  
  /* We can now attempt to send a message */
  send_hello_world(sp);
  
  X400msClose (sp);
  if ( status != X400_E_NOERROR ) {
    fprintf (stderr, "Error in Close: %s\n", X400msError (status));
    exit (status);
  }

  /* We can now attempt to receive that message
   * Notice we are binding as the recipient of the message
   */

  status = X400msOpen (contype, recip, def_dn, password, ms_pa, &nummsg, &sp);
  if ( status != X400_E_NOERROR ) {
      fprintf (stderr, "Error in Open: %s\n", X400msError (status));
      exit (status);
  }

  /* sp is obviously a session pointer
   * nummsgs is an int represtenting the number of messages available
   * for the current user
   */
  receive_msgs(sp,nummsg);
  
  X400msClose (sp);
  if ( status != X400_E_NOERROR ) {
    fprintf (stderr, "Error in Close: %s\n", X400msError (status));
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
    struct X400msSession *sp
)
{
  int status;

  /* Now lets create a new message */
  struct X400msMessage *mp; /* This is the pointer to a message object */
  struct X400Recipient *rp; /* This is the pointer to a new recipient  */


  printf("Now sending the simple message\n");
  /* We now need to create a new message object, and have the new
   * message object associated with the session object.
   *
   * To do this we use X400mtMsgNew.
   */
  status = X400msMsgNew (sp, X400_MSG_MESSAGE, &mp);
  
  if ( status != X400_E_NOERROR ) {
    fprintf (stderr, "Error in MsgNew: %s\n", X400msError (status));
    exit (status);
  }
  
  /* We can now manipulate the message object */
  
  /* Add an originator. Similar concepts here,
   * we are manipulating the message object to add an originator address
   */
  status = X400msMsgAddStrParam (mp, X400_S_OR_ADDRESS, orig, -1);
    
  if ( status != X400_E_NOERROR ) {
    fprintf (stderr, "Error adding orignator: %s\n", X400msError (status));
    exit (status);
  }
  
  /* Create a new recipient object, and associate it with the message object.
   */
  status = X400msRecipNew (mp, X400_RECIP_STANDARD, &rp);
  if ( status != X400_E_NOERROR ) {
    fprintf (stderr, "Error adding recipient: %s\n", X400msError (status));
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
  status = X400msRecipAddStrParam (rp, X400_S_OR_ADDRESS, recip, -1);
  if ( status != X400_E_NOERROR ) {
    fprintf (stderr, "Error adding recipient address: %s\n", 
	     X400msError (status));
    exit (status);
  }
  
  /* The following attribute might be slightly confusing.
   * The best thing to do at this point is to take a brief look 
   * at X.411, section 12.2.1.1.1.6, this describes the responsibility
   * attribute.
   */

  status = X400msRecipAddIntParam (rp, X400_N_RESPONSIBILITY, 1);
  if ( status != X400_E_NOERROR ) {
    fprintf (stderr, "Error adding recipient responsibility: %s\n", 
	     X400msError (status));
    exit (status);
  }

  /* And now for the content */
  {
    char *content = "Hello World!";
    char *subject = "A simple test message";

    status = X400msMsgAddStrParam (mp,X400_T_IA5TEXT, content , -1);
    if ( status != X400_E_NOERROR ) {
      fprintf (stderr, "Error adding content : %s\n", 
	       X400msError (status));
      exit (status);
    }

    status = X400msMsgAddStrParam (mp, X400_S_SUBJECT, subject, -1);
     if ( status != X400_E_NOERROR ) {
       fprintf (stderr, "Error adding subject : %s\n", 
		X400msError (status));
       exit (status);
     }
  }
  
  /* we should now be able to send the message */
  status = X400msMsgSend (mp);
  if ( status != X400_E_NOERROR ) {
    fprintf (stderr, "Error in MsgSend: %s\n", X400msError (status));
    exit (status);
  }
  
  /* We can now delete the message
   * We pass in the message pointer, but also a "0".
   * The "0" means that the message will not be */
  status = X400msMsgDelete (mp,0); 
  if ( status != X400_E_NOERROR ) {
    fprintf (stderr, "Error in X400mtMsgDelete: %s\n", X400msError (status));
    exit (status);
  }
  
  printf("Sent message\n");
}


static void receive_msgs(
    struct X400msSession *sp,
    int nummsg
)
{
   
    struct X400msMessage *mp;
    struct X400Recipient *rp;

    int status;
    
    int type;
    int seqn;
    size_t length;
    char buffer[BUFSIZ];
    
    if (nummsg == 0) {
        printf ("no messages - waiting 60 seconds for a message to be delivered.....\n");
    }
    else {
        printf("%d messages waiting\n", nummsg);
    }

    /* This will add a delay, so the sent message will have sometime to get
     * through the system
     */
    status = X400msWait(sp, 1, &nummsg);
    if (status != X400_E_NOERROR) {
        fprintf(stderr, "Error from Wait: %s\n", X400msError(status));
        return;
    }
    
    /* Now we fetch the actual message.
     * to keep things simple we will only try to fetch the first message
     */
    status = X400msMsgGet(sp, 0, &mp, &type, &seqn);
    switch (status) {
    case X400_E_NOERROR:
	fprintf(stderr, "MsgGet successfully got message\n");
	break;
    default :
	fprintf(stderr, "Error from MsgGet: %s\n", X400msError(status));
	return;
    }

    /* Now we fetch various attrbitues.
     * A word of warning though:
     * These attributes may or may not be present in real messages.
     * we know exactly what attributes are avaliable in this test message.
     * so we will keep it simple and only bother checking these few */
    
    /* Fetch the originator */
    status = X400msMsgGetStrParam(mp, X400_S_OR_ADDRESS,
                                  buffer, sizeof buffer, &length);
    if (status != X400_E_NOERROR) {
        fprintf(stderr, "Error from MsgGetStrParam: %s\n",
                X400msError(status));
	return;
    }
    printf("Originator: %.*s\n", (int)length, buffer);
    
    /* Fetch the Envelope recipient */
    status = X400msRecipGet(mp, X400_RECIP_ENVELOPE, 1, &rp);
    if (status == X400_E_NO_RECIP)
	return;
    
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "Error from RecipGet: %s\n", X400msError(status));
	/* tidily close the session */
	status = X400msClose(sp);
	exit(status);
    }
    
    /* Now fetch the OR Address of the recipient.
     * We can safely assume that there is an OR Address since we created the
     * message.*/
    status = X400msRecipGetStrParam(rp, X400_S_OR_ADDRESS,
				    buffer, sizeof buffer, &length);
    if (status != X400_E_NOERROR) {
	fprintf(stderr, "Error from X400msRecipGetStrParam: %s\n",
                X400msError(status));
	return;
    }
    printf("Envelope Recipient : %.*s\n", (int)length, buffer);
    
    
    /* Now fetch the Content type int */
    status = X400msMsgGetIntParam (mp, X400_N_CONTENT_TYPE, &type);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "Error in X400msMsgGetIntParam content type: %s\n",
		 X400msError (status));
	return;
    }
    printf("Content type: %i\n",type);

    
    /* Now fetch the subject of the message */
    status = X400msMsgGetStrParam(mp, X400_S_SUBJECT,
				  buffer, sizeof buffer, &length);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "Error in X400msMsgGetStrParam subject: %s\n",
		 X400msError (status));
	return;
    }
    
        printf("Subject: %.*s\n", (int)length, buffer);
    
    /* Now fetch the IA5-text content of the message.*/
    status = X400msMsgGetStrParam(mp, X400_T_IA5TEXT,
                                  buffer, sizeof buffer, &length);
    if ( status != X400_E_NOERROR ) {
	fprintf (stderr, "Error in X400msMsgGetStrParam subject: %s\n",
		 X400msError (status));
	return;
    }

    printf("Text:\n%.*s\n", (int)length, buffer);
    
    X400msMsgDelete(mp, 0);
}
