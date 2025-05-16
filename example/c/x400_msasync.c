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
 */

#include <stdio.h>

#ifdef _WIN32

int main(int argc, char **argv)
{
    fprintf(stderr, "This example is not available on Windows\n");
    exit(1);
}

#else

static void usage(void) ;

#include <string.h>
#include <errno.h>

#if defined (__hpux)
#include <sys/types.h>
#else
#include <sys/select.h>
#endif 
#include <signal.h>
#include <sys/time.h>
#include <x400_msapi.h>
#include <x400_ms_async.h>
#include "example.h"

/* Define IC_ATTRIBUTE
 * For gcc/g++ this becomes __attribute__ and is used for the printf type
 * format checking.
 * Defined to be empty for other compilers
 */

#if defined(__GNUC__) || defined (__GNUG__)
#define IC_ATTRIBUTE(x) __attribute__ (x)
#else
#define IC_ATTRIBUTE(x)
#endif

/* To annotate function parameters which are known not to be used
 * Not in gcc/g++ prior to v4 (I think)
 */
#if (defined(__GNUG__) && __GNUG__ >= 4) || (!defined(__GNUG__) && defined(__GNUC__) && __GNUC__ >= 3)
# define ARGNOTUSED IC_ATTRIBUTE((unused))
#else
# define ARGNOTUSED
#endif

static void connection_established(struct X400msSession *session,
				   int status,
				   int num_messages_waiting);

static void connection_lost(struct X400msSession *session,
			    int reason_code,
			    char *diagnostic);

static void submission_complete(struct X400msSession *session,
				struct X400msMessage *message,
				int errorcode);

static void fetch_complete(struct X400msSession *session,
			   struct X400msMessage *message,
			   int type,
			   int seq,
			   int errorcode);

static void delete_complete(struct X400msSession *session,
			    int seqnum,
			    int errorcode);

static void wait_complete(struct X400msSession *session,
			  int num_messages_waiting,
			  int errorcode);

static void list_complete(struct X400msSession *session,
			  struct X400msListResult *listres,
			  int errorcode);

static void register_complete(struct X400msSession *session,
			      int errorcode);

static void alert_event(struct X400msSession *session);

static void event_loop(void);

static int manage_callback(struct X400msSession *session,
			   int fd,
			   int eventmask);

static int unmanage_callback(struct X400msSession *session,
			     int fd,
			     int eventmask);

struct SessionTable{
    char *oraddr;
    char *psw;
    char *pa;
    int fd;
    int eventmask;
    struct X400msSession *sp;
};

#define MAX_SESSIONS 10

struct SessionTable st[MAX_SESSIONS + 1]; 
static int dosubmit = 1;
int millisecs = 10000;
int numcons = 1;
int nummsgs = 9999999;
static char *optstr = "u371m:p:w:l:sc:o:";
int contype;

static void usage(void) {
    printf("usage: %s\n", optstr);
    printf("\t where:\n");
    printf("\t -u : Don't prompt to override defaults \n");
    printf("\t -3 : P3 connection \n");
    printf("\t -7 : P7 connection \n");
    printf("\t -m : OR Address in P7 bind arg \n");
    printf("\t -p : Presentation Address of P7 Store \n");
    printf("\t -w : P7 password of P7 user \n");
    printf("\t -s : Don't do submission\n");
    printf("\t -l : Sleep time (milliseconds)\n");
    printf("\t -c : Num associations\n");
    printf("\t -o : Exit after retrieving n messages\n"); 
    return;
}


int main(int argc, char **argv)
{
    int i;
    int status;
    char *def_oraddr;
    char *def_pa;
    char buffer[BUFSIZ];
    char pa[BUFSIZ];
    char ora[BUFSIZ];

    x400_default_alternate_recipient_allowed = 1;
    x400_channel = "1";
    x400_default_originator = "1000000";

    for (i = 0; i <= MAX_SESSIONS; i++)
	st[i].oraddr = NULL;

    if (get_args(argc, argv, optstr)) {
	usage();
	exit(-1);
    }

    def_oraddr = x400_ms_user_addr;
    def_pa = x400_ms_presentation_address;
    if (x400_logline != NULL)
	millisecs = atoi(x400_logline);

    dosubmit = x400_default_disclosure_of_recipients;
    numcons = atoi(x400_channel);
    nummsgs = atoi(x400_default_originator);

    printf("Connection type (0 = P7, 1 = P3 submit only, 2 = P3 both directions) [%d]: ", x400_contype);
    contype = ic_fgetc(x400_contype, stdin); 
    if (contype != 10)
	ic_fgetc(x400_contype, stdin);

    if ( contype < '0' || '2' < contype )
      contype = x400_contype;
    else 
	contype -= '0';

    for (i = 0; i < numcons; i++) {
	printf("ORAddress [%s] > ", def_oraddr);
	ic_fgets (ora, sizeof ora, stdin);

	if ( ora[strlen(ora)-1] == '\n' )
	    ora[strlen(ora)-1] = '\0';
	
	if (ora[0] == '\0') 
	    strcpy(ora, def_oraddr);

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
	
	st[i].oraddr = strdup(ora);
	st[i].psw = strdup(buffer);
	st[i].pa = strdup(pa);
    }
	

    if (talking_to_marben_ms)
	X400msSetConfigRequest(0);

    /* Open our connections */
    for (i = 0; (st[i].oraddr != NULL) && (i < numcons); i++) {
	status = X400msOpenAsync (contype, 
				  st[i].oraddr, NULL, st[i].psw, 
				  st[i].pa, NULL, 
				  connection_established,
				  connection_lost,
				  submission_complete,
				  fetch_complete,
				  delete_complete,
				  wait_complete,
				  list_complete,
				  register_complete,
				  alert_event,
				  manage_callback,
				  unmanage_callback,
				  &(st[i].sp));
	
	if (status != X400_E_WAIT_WRITE) {
	    fprintf (stderr, "Error in Open for %s: %s\n", 
		     st[i].oraddr, X400msError (status));
	} else {
	    printf ("Sent off connection request for session %p\n", 
		     st[i].sp);
	    X400msSetStrDefault(st[i].sp, 
				X400_S_LOG_CONFIGURATION_FILE, 
				"x400api.xml", 0);
	    
	    if (talking_to_marben_ms)
		X400msSetIntDefault(st[i].sp, X400_N_STRICT_P7_1988, 1);

	    st[i].fd = X400msGetHandle(st[i].sp); 
	}
    }

    event_loop();

    return 0;
}


static int unmanage_callback(struct X400msSession *session,
			     int fd ARGNOTUSED,
			     int eventmask)
{
    int i;

    printf("unmanage_callback, session = %p, mask = %d\n", session, eventmask);

    for (i = 0; st[i].oraddr != NULL; i++) {
	if (st[i].sp == session) {
	    st[i].eventmask &= ~eventmask;
	    return 0;
	}
    }
    return -1;
}

static int manage_callback(struct X400msSession *session,
			    int fd,
			    int eventmask)
{
    int i;

    printf("manage_callback, session = %p, fd = %d, eventmask = %d\n",
	   session, fd, eventmask);

    for (i = 0; st[i].oraddr != NULL; i++) {
	if (st[i].sp == session) {
	    st[i].eventmask |= eventmask;
	    return 0;
	}
    }
    return -1;
}


static void send_messages()
{
    int status;
    int i;
    struct X400msMessage *mp;
    struct X400Recipient *rp;
    char buf[1024];
    char contid[1024];
    static int num = 0;

    for (i = 0; st[i].oraddr != NULL; i++) {
	if (st[i].fd != -1) {

	    status = X400msMsgNew (st[i].sp, 
				   X400_MSG_MESSAGE, &mp);
	    if ( status != X400_E_NOERROR ) {
		fprintf (stderr, "x400msMsgNew returned error: %s\n", 
			 X400msError (status));
		exit (status);
	    }

	    status = X400msRecipNew (mp, X400_RECIP_STANDARD, &rp);
	    if ( status != X400_E_NOERROR ) {
		fprintf (stderr, "x400msRecipNew returned error: %s\n", 
			 X400msError (status));
		exit (status);
	    }

	    status = X400msRecipAddStrParam (rp, X400_S_OR_ADDRESS, 
					     st[i].oraddr, -1);
	    if ( status != X400_E_NOERROR ) {
		fprintf (stderr, "x400msRecipAddStrParam returned error: %s\n", 
			 X400msError (status));
		exit (status);
	    }

	    sprintf(buf, "Subject for session %p", st[i].sp);
	    status = X400msMsgAddStrParam (mp, X400_S_SUBJECT,
					   buf, -1);
	    if ( status != X400_E_NOERROR ) {
		fprintf (stderr, "x400msMsgAddStrParam returned error: %s\n", 
			 X400msError (status));
		exit (status);
	    }

	    status = X400msMsgAddStrParam (mp, X400_T_IA5TEXT, "body text", -1);
	    if ( status != X400_E_NOERROR ) {
		fprintf (stderr, "x400ms returned error: %s\n", 
			 X400msError (status));
		exit (status);
	    }
	    
	    sprintf(contid, "CONTID%d", num);
	    num++;

	    /* Content identifier so we can correlate with submission result */
	    status = X400msMsgAddStrParam(mp, X400_S_CONTENT_IDENTIFIER, contid, -1);
	    if ( status != X400_E_NOERROR ) {
		fprintf (stderr, "X400msMsgAddIntParam %d returned error: %s\n", 
			 X400_S_CONTENT_IDENTIFIER, X400msError (status));
		exit (status);
	    }

	    printf("Sending message from & to %s\n", st[i].oraddr);
	    status = X400msMsgSend(mp);

	    if ((status != X400_E_WAIT_READ) && 
		(status != X400_E_WAIT_READ_WRITE)) {
		fprintf (stderr, "Error sending message: %d\n", status);
	    }

	    X400msMsgDelete(mp, 0);
	}
    }
}


static void connection_established(struct X400msSession *session,
				   int status,
				   int alert)
{
    int retval;
    int count; 

    if (status == X400_E_NOERROR) {
	printf("Connection established for session %p, alert = %d\n", 
	       session, alert);

	if (contype == 0) {
#ifdef notdef
	    printf("Sending Register request\n");
	    retval = X400msRegisterAutoAction (session, X400_AUTO_ALERT, 
					       9, NULL);
	    if ((retval != X400_E_WAIT_READ) &&
		(retval != X400_E_WAIT_READ_WRITE))
		fprintf(stderr, 
			"Failed to issue Register for session %p, err = %d\n", 
			session, retval);
	    printf("Registered AutoAlert autoaction (id = 9) OK\n");
	    
	    printf("Sending Deregister request\n");
	    retval = X400msDeregisterAutoAction (session, 
						 X400_AUTO_ALERT, 5);
	    if ((retval != X400_E_WAIT_READ) &&
		(retval != X400_E_WAIT_READ_WRITE))
		fprintf(stderr, 
	      	"Failed to issue Deregister for session %p, err = %d\n", 
			session, retval);
	    printf("Deegistered AutoAlert autoaction (id = 5) OK\n");
#endif	    
	    printf("Sending List request\n");
	    retval = X400msList(session, NULL, NULL);
	    if ((retval != X400_E_WAIT_READ) &&
		(retval != X400_E_WAIT_READ_WRITE))
		fprintf(stderr, 
			"Failed to issue List for session %p, err = %d\n", 
			session, retval);
	} else {
	    /* If P3 connection, we just need to wait until we get invoked */
	    retval = X400msWait(session, 0, &count);
	
	    if ((retval != X400_E_WAIT_READ) && 
		(retval != X400_E_WAIT_READ_WRITE))
		fprintf(stderr, 
			"Failed to issue Wait for session %p, err = %d\n", 
			session, retval);
	}
    } else {	
	int i;

	fprintf(stderr, "Connection failed for session %p\n", session);
	for (i = 0; st[i].oraddr != NULL; i++) {
	    if (st[i].sp == session) {
		st[i].fd = -1;
		break;
	    }
	}
    }
}

static void connection_lost(struct X400msSession *session ARGNOTUSED,
			    int reason_code,
			    char *diagnostic)
{
    fprintf(stderr, "Connection lost callback, reason = %d, diag = %s\n",
	    reason_code, (diagnostic == NULL) ? "NULL" : diagnostic);
}

static void submission_complete(struct X400msSession *session ARGNOTUSED,
				struct X400msMessage *message,
				int errorcode)
{
    if (errorcode == X400_E_NOERROR) {
	char buf[1024];
	size_t len;
	int status;

	memset(buf,0,1024);
	if (X400msMsgGetStrParam (message, X400_S_MESSAGE_IDENTIFIER,
				  buf, 1024, &len) == X400_E_NOERROR)
	    printf("Submission successful, identifier = %s\n", buf);
	else
	    fprintf(stderr,"Submission successful but failed to get ID\n");

	status = X400msMsgGetStrParam(message, X400_S_MESSAGE_SUBMISSION_TIME, buf, 1024, &len);
	if (status != X400_E_NOERROR) {
	    fprintf(stderr, "No MessageSubmissionTime present from submission result: error %d\n", status);
	} else {
	    buf [len] = '\0';
	    printf("MessageSubmissionTime from Submission Result = %s\n", buf);
	}

	status = X400msMsgGetStrParam(message, X400_S_CONTENT_IDENTIFIER, buf, 1024, &len);
	if (status != X400_E_NOERROR) {
	    fprintf(stderr, "No ContentIdentifier present from submission result: error %d\n", status);
	} else {
	    buf [len] = '\0';
	    printf("ContentIdentifier from Submission Result = %s\n", buf);
	}

	X400msMsgDelete (message, 0);
    } else {
	fprintf(stderr, "Submission failed, errorcode = %d\n", errorcode);
    }

}


static void list_complete(struct X400msSession *session,
			  struct X400msListResult *lr,
			  int errorcode)
{
    int i = 1;
    int status = X400_E_NOERROR;

    printf("list_complete, errorcode = %d\n", errorcode);

    if (errorcode == X400_E_NOERROR) {
	/* Traverse list result, extracting information about each message */
	while (status == X400_E_NOERROR) {
	    int seqnum = 0;
	    char buf[1024];
	    char *type;
	    char *subject;
	    char *sender;
	    size_t len;
	    int length;
	    int priority;
	    int entry_status;
	    char *msgid;
	    char *subjid;

	    type = NULL;
	    subject = NULL;
	    sender = NULL;
	    msgid = NULL;
	    subjid = NULL;
	    length = -1;
	    priority = -1;
	    entry_status = -1;

	    /* Get sequence number */
	    if ((status = X400msListGetIntParam(lr, X400_N_MS_SEQUENCE_NUMBER, 
						i, &seqnum)) == X400_E_NO_MORE_RESULTS)
		break;
	    
	    if (X400msListGetStrParam(lr,
				      X400_S_OBJECTTYPE,
				      i,
				      buf,
				      1024,
				      &len) == X400_E_NOERROR)
		type = strdup(buf);
      
	    if (X400msListGetStrParam(lr,
				      X400_S_SUBJECT,
				      i,
				      buf,
				      1024,
				      &len) == X400_E_NOERROR)
		subject = strdup(buf);
		
	    if (X400msListGetStrParam(lr,
				      X400_S_OR_ADDRESS,
				      i,
				      buf,
				      1024,
				      &len) == X400_E_NOERROR) 
		sender = strdup(buf);
		
	    if (X400msListGetStrParam(lr,
				      X400_S_MESSAGE_IDENTIFIER,
				      i,
				      buf,
				      1024,
				      &len) == X400_E_NOERROR) 
		msgid = strdup(buf);

	    if (X400msListGetStrParam(lr,
				      X400_S_SUBJECT_IDENTIFIER,
				      i,
				      buf,
				      1024,
				      &len) == X400_E_NOERROR) 
		subjid = strdup(buf);

	    X400msListGetIntParam(lr, X400_N_PRIORITY, i, &priority);

	    X400msListGetIntParam(lr, X400_N_CONTENT_LENGTH, i, &length);
      
	    /* Entry status */
	    X400msListGetIntParam(lr, X400_N_MS_ENTRY_STATUS, i, 
				  &entry_status);

	    printf("S=%d, P=%d, len=%d, stat=%d, type=%s, subj=%s, orig=%s, msgid=%s, subjid=%s\n",
		   seqnum, priority, length, entry_status, 
		   (type == NULL) ? "NULL" : type, 
		   (subject == NULL) ? "NULL": subject, 
		   (sender == NULL) ? "NULL" : sender,
		   (msgid == NULL) ? "NULL" : msgid,
		   (subjid == NULL) ? "NULL" : subjid);
	    i++;
	    if (sender != NULL)
		free(sender);
	    if (subject != NULL)
		free(subject);
	    if (type != NULL)
		free(type);
	    if (msgid != NULL)
		free(msgid);
	    if (subjid != NULL)
		free(subjid);

	    if (seqnum != 0) {
		int retval;

		/* Ask for the whole message */
		printf("Session %p, issing MsgGet\n", session);
		retval = X400msMsgGet (session, seqnum, 
				       NULL, NULL, NULL);
		if ((retval != X400_E_WAIT_READ) &&
		    (retval != X400_E_WAIT_READ_WRITE)) {
		    fprintf(stderr, "MsgGet failed !\n");
		    exit(1);
		}
	    }
	}
	X400msListFree(lr);
    } else {
	fprintf(stderr,"List failed, errorcode = %d\n", errorcode);
    }
}


static void fetch_complete(struct X400msSession *session,
			   struct X400msMessage *message,
			   int type,
			   int seqno,
			   int errorcode)
{ 
    printf("Fetch complete callback, session=%p, seqno=%d, type=%d, errorcode = %d\n",
	   session, seqno, type, errorcode);

    if (errorcode == X400_E_NOERROR) {
	char buf[1024];
	size_t len;
	int retval;
	char buffer[30000];
	int status;
	size_t length;

	status = X400msMsgGetStrParam (message, X400_S_CONTENT_FILENAME,
				       buffer, sizeof buffer, &length);
	if (status == X400_E_NOERROR) 
	    printf("Read raw content into file %s\n", buffer);
	else 
	    fprintf(stderr,"Failed to read raw content into file, error %d\n", status);

	status = X400msMsgGetStrParam (message, X400_S_CONTENT_STRING,
				       buffer, sizeof buffer , &length);
	if (status == X400_E_NOERROR) 
	    printf("Read raw content into buffer, length = %ld\n", (long)length);
	else 
	    fprintf(stderr,"Failed to read raw content into buffer, error %d\n", status);

	memset(buf,0,1024);
	if (X400msMsgGetStrParam (message, X400_S_OBJECTTYPE,
				  buf, 1024, &len) == X400_E_NOERROR)
	    printf("Object type = %s\n", buf);

	memset(buf,0,1024);
	if (X400msMsgGetStrParam (message, X400_S_SUBJECT,
				  buf, 1024, &len) == X400_E_NOERROR)
	    printf("Subject = %s\n", buf);

	if (contype != 0) {
	    retval = X400msMsgGetFinish (message, X400_E_NOERROR, 0);
	    if (retval != X400_E_NOERROR)
		fprintf(stderr, "MsgGetFinish failed !\n");
	}

	/* Now delete it */
	retval = X400msMsgDelete (message, 0);
	if ((retval != X400_E_NOERROR) && 
	    (retval != X400_E_WAIT_READ) && (retval != X400_E_WAIT_READ_WRITE))
	    fprintf(stderr, "MsgDelete failed !\n");
	else
	    printf("Session %p, issued MsgDelete for %d\n", session, seqno );
#ifdef notdef	
	/* Ask for the next one */
	retval = X400msMsgGet (session, 0, NULL, NULL, NULL);
	    fprintf(stderr, "MsgGet failed !\n");
	if ((retval != X400_E_WAIT_READ) && (retval != X400_E_WAIT_READ_WRITE))
	else
	    printf("Session %p, issued MsgGet\n", session);
#endif
    } else
	fprintf(stderr, "Fetch failed, errcode = %d\n",errorcode);
	
}

static void delete_complete(struct X400msSession *session,
			    int seqnum,
			    int errorcode)
{
    if (errorcode == X400_E_NOERROR) {
	printf("Session %p, - delete complete callback for seqnum %d\n",
	   session, seqnum);
    } else {
	fprintf(stderr, "******* SESSION %p, - DELETE FAILED %d **********\n",
	   session, errorcode);
    }
}

static void register_complete(struct X400msSession *session,
			      int errorcode)
{
    if (errorcode == X400_E_NOERROR) {
	printf("Session %p, - register complete callback\n",
	   session);
    } else {
	fprintf(stderr, "******* SESSION %p, - REGISTER FAILED %d **********\n",
	   session, errorcode);
    }
}

static void wait_complete(struct X400msSession *session,
				int num_messages_waiting,
				int errorcode)
{
    printf("Wait complete callback, errorcode = %d, num = %d\n",
	   errorcode, num_messages_waiting);

    if (num_messages_waiting > 0) {
	int retval;

	/* Get first available message */
	printf("Session %p, issuing MsgGet OK\n", session);
	retval = X400msMsgGet (session, 0, NULL, NULL, NULL);
	if ((retval != X400_E_WAIT_READ) && (retval != X400_E_WAIT_READ_WRITE))
	    fprintf(stderr, "MsgGet failed !\n");
    }
}

static void alert_event(struct X400msSession *session)
{
    int retval;

    printf("Got an alert !\n");

    printf("Issuing List op for session %p\n", session);
    retval = X400msList(session, NULL, NULL);
    if ((retval != X400_E_WAIT_READ) && (retval != X400_E_WAIT_READ_WRITE))
	fprintf(stderr, "Failed to issue List op for session %p, error %d\n", 
		session, retval);
}


static void event_loop()
{
    fd_set rfds, wfds, efds;
    struct timeval timeout, *tp;
    int status;
    int n;
    int i;

    while (1) {
	int num_fds = 0;

	FD_ZERO (&rfds);
	FD_ZERO (&efds);
	FD_ZERO (&wfds);

	for (i = 0; st[i].oraddr != NULL; i++) {
	    if (st[i].fd != -1) {
		
				printf("Expecting ");
		if (st[i].eventmask & X400_EVENT_READ) {
		    		    printf("read ");
		    FD_SET(st[i].fd, &rfds);
		}

		if (st[i].eventmask & X400_EVENT_WRITE) {
		    		    printf("write ");
		    FD_SET(st[i].fd, &wfds);
		} 

		if (st[i].eventmask & X400_EVENT_ERROR) {
		   		    printf("error ");
		    FD_SET(st[i].fd, &efds);
		}
				printf("on fd %d\n", st[i].fd);
	    }

	    if (st[i].fd > num_fds)
		num_fds = st[i].fd;
	}

	if ( millisecs >= 0 ) {
	    timeout.tv_sec  = millisecs / 1000;
	    timeout.tv_usec = (millisecs % 1000) *1000;
	    tp = &timeout;
	} else {
	    tp = 0;
	}
	//printf("num fds = %d\n", num_fds);
	n = select (num_fds + 1, &rfds, &wfds, &efds, tp);
	
	if (n < 0) {
	    fprintf(stderr, "select returned %d: errno = %d\n", n, errno);
	    exit(1);
	}
	else if (n == 0) {
	    int i;

	    if (nummsgs < 0) {
		printf("Exiting now\n");
		for (i = 0; st[i].oraddr != NULL; i++)
		    if (st[i].fd != -1) 
			X400msClose(st[i].sp);
		exit(0);
	    } else
		nummsgs--;

	    printf(">>>>> Timeout\n");
	    
	    if (contype == 0) {
		for (i = 0; st[i].oraddr != NULL; i++) {
		    int retval;
		    
		    if (st[i].fd != -1) {
			printf("Issuing List for session %p\n", st[i].sp);
			retval = X400msList(st[i].sp, NULL, NULL);
			if ((retval != X400_E_WAIT_READ) && 
			    (retval != X400_E_WAIT_READ_WRITE))
			    fprintf(stderr, 
				    "Failed to issue List for session %p, err = %d\n", 
				st[i].sp, retval);
		    }
		}
	    }

	    if (dosubmit == 1)
		/* Submit some new message */
		send_messages();

	} else {
	    status = X400msProcessEvent(num_fds + 1, 
					&rfds, &wfds, &efds);
	    //printf("X400msProcessEvent returned %d\n", status);
	}
    }
}

#endif
