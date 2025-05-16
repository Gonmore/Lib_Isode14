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

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef int (*IFP) ();

extern int use_defaults;
extern int x400_contype;
extern char *x400_ms_user_addr;
extern char *x400_ms_user_dn;
extern char *x400_ms_presentation_address;
extern char *x400_ms_password;
extern char *x400_p7_password;
extern char *x400_mta_user_addr;
extern char *x400_mta_user_dn;
extern char *x400_mta_presentation_address;
extern char *x400_mta_password;
extern char *x400_p3_password;
extern char *x400_default_originator_return_address;
extern char *x400_default_recipient;
extern char *x400_default_gw_recipient;
extern int x400_default_priority;
extern int x400_default_content_type;
extern int x400_default_implicit_conversion_prohibited;
extern int x400_default_alternate_recipient_allowed;
extern int x400_default_content_return_request;
extern int x400_default_disclosure_of_recipients;
extern int x400_default_recipient_reassignment_prohibited;
extern int x400_default_dl_expansion_prohibited;
extern int x400_default_conversion_with_loss_prohibited;
extern char *x400_default_originator;
extern char *x400_default_gw_originator;
extern char *x400_default_exempted_recipient;
extern char *x400_default_fwd_originator;
extern char *x400_default_fwd_recipient;
extern char *x400_channel;
extern char *x400_logline;
extern int x400_reports;
extern char *security_id;
extern char *passphrase;
extern char *identity_dn;
extern char *identity_dn2;
extern int sign_message;
extern char *filename_to_send;
extern int num_msgs_to_send;
extern int bin_bp_size;
extern int until_no_more_msgs;
extern int new_messages;
extern int submitted_messages;
extern char *since_time;
extern int attr_to_fault;
extern char *attrs[];
extern char *x400_default_gw_dlexphist1_or;
extern char *x400_default_gw_dlexphist1_dn;
extern char *x400_default_gw_dlexphist2_or;
extern char *x400_default_gw_dlexphist2_dn;
extern int talking_to_marben_ms;

int getopt (int nargc, char *const *nargv, const char *ostr);
int get_args(int argc, char **argv, char *optstr);
char *ic_fgets(char *s, int size, FILE *stream);
int ic_fgetc(int default_char, FILE *stream);


