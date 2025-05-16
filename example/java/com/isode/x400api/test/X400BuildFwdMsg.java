/*  Copyright (c) 2008, Isode Limited, London, England.
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


/**
 * Support methods for Java X.400 API test program
 * These methods build a Pure X.400 message which can be attached 
 * to an MS Message as a forwarded messages.
 * The Message itself can then be submitted using MsgSend
 */

package com.isode.x400api.test;

import com.isode.x400api.*;
import com.isode.x400api.X400ms.*;
import com.isode.x400api.test.config;

public class X400BuildFwdMsg
{

    public static int build_fwd_msg(Message message_obj, String oraddress)
    {
	int status;
	int rno = 1;
	String originator = "/CN=fwd msg orig/OU=lppt/O=" 
	    + config.hostname + "/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";

	String unknown_recip = "/CN=invalid/OU=lppt/O=" 
	    + config.hostname + "/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";

    	// make message object an API object
    	status = com.isode.x400api.X400.x400_msgnew(
	    X400_att.X400_MSG_MESSAGE, message_obj);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgnew failed " + status);
	    return status;
	}

	// add orig to msg
        status = com.isode.x400api.X400.x400_msgaddstrparam(message_obj, 
            X400_att.X400_S_OR_ADDRESS, originator, -1);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_msgaddstrparam failed " + status);
            return status;
        }
	status = add_recip(message_obj, originator, 
	    X400_att.X400_ORIGINATOR, rno++);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("add_recip failed " + status);
	    return status;
	}

	// add recip to msg
	status = add_recip(message_obj, oraddress, 
	    X400_att.X400_RECIP_STANDARD, rno++);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("add_recip failed " + status);
	    return status;
	}

	// add unknown recip to msg
	status = add_recip(message_obj, unknown_recip, 
	    X400_att.X400_RECIP_STANDARD, rno++);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("add_recip failed " + status);
	    return status;
	}

	// build the message envelope - note that we're using the
	// recipient oraddress as the originator of the message
	status = build_env(message_obj, oraddress);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("build_env failed " + status);
	    return status;
	}

	// build the message content
	status = build_content(message_obj);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("build_content failed " + status);
	    return status;
	}
	return status;
    }

    /**
     * Add a recipient to a message for adding as a message body part.
     */
    private static int add_recip(Message message_obj, String oraddress,
    	int type, int rno)
    {
	int status;
	String alt_recip = "/CN=altrecip/OU=lppt/O=" 
	    + config.hostname + "/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";

	// instantiate a recip object, and make it an API object 
	Recip recip_obj = new Recip();
    	status = com.isode.x400api.X400.x400_recipnew(
	    type, recip_obj);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_recipnew failed " + status);
	    return status;
	}

	// add some X.400 attributes into the recipient 

	// use length of -1 to indicate NULL terminated 
    	status = com.isode.x400api.X400.x400_recipaddstrparam(recip_obj, 
	    X400_att.X400_S_OR_ADDRESS, oraddress, -1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_recipaddstrparam failed " + status);
	    return status;
	}

    	status = com.isode.x400api.X400.x400_recipaddintparam(recip_obj, 
	    X400_att.X400_N_MTA_REPORT_REQUEST, 3);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_recipaddintparam failed " + status);
	    return status;
	}

    	status = com.isode.x400api.X400.x400_recipaddintparam(recip_obj, 
	    X400_att.X400_N_REPORT_REQUEST, 2);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_recipaddintparam failed " + status);
	    return status;
	}

    	status = com.isode.x400api.X400.x400_recipaddintparam(recip_obj, 
	    X400_att.X400_N_ORIGINAL_RECIPIENT_NUMBER, rno);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_recipaddintparam failed " + status);
	    return status;
	}

    	status = com.isode.x400api.X400.x400_recipaddstrparam(recip_obj, 
	    X400_att.X400_S_ORIGINATOR_REQUESTED_ALTERNATE_RECIPIENT, 
	    	alt_recip, -1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_recipaddstrparam failed " + status);
	    return status;
	}

    	status = com.isode.x400api.X400.x400_msgaddrecip(message_obj, 
	    type, recip_obj);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddrecip failed " + status);
	    return status;
	}


	System.out.println("Added recipient " + oraddress + " successfully");
	return status;
    }

    private static int build_env(Message message_obj, String originator)
    {
	int status;
	// String hostname = "attlee";
	String orig_ret_addr = "/CN=origretr/OU=lppt/O=" 
	    + config.hostname + "/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
	String msg_id = "/PRMD=TestPRMD/ADMD=TestADMD/C=GB/;" 
	    + config.hostname + ".2810401-030924.140212";
	String  content_id = "030924.140212";
	String latest_del_time = "090927120000Z";
        String del_time = "080927120000Z";
	//  Originator (X.400 String format)
    	status = com.isode.x400api.X400.x400_msgaddstrparam(message_obj, 
	    X400_att.X400_S_OR_ADDRESS, originator, 
	    originator.length());
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddstrparam failed " + status);
	    return status;
	}

	// Add an integer envelope content type 2 or 22 
    	status = com.isode.x400api.X400.x400_msgaddintparam(message_obj, 
	    X400_att.X400_N_CONTENT_TYPE, 2);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddintparam failed " + status);
	    return status;
	}

	// X400_N_CONTENT_LENGTH is probe only 

	// Priority: 0 - normal, 1 - non-urgent, 2 - urgent 
    	status = com.isode.x400api.X400.x400_msgaddintparam(message_obj, 
	    X400_att.X400_N_PRIORITY, 2);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddintparam failed " + status);
	    return status;
	}

	// Disclosure of recipients: 0 - no, 1 - yes 
    	status = com.isode.x400api.X400.x400_msgaddintparam(message_obj, 
	    X400_att.X400_N_DISCLOSURE, 1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddintparam failed " + status);
	    return status;
	}

	//  Implicit conversion prohibited: 0 - no, 1 - yes
    	status = com.isode.x400api.X400.x400_msgaddintparam(message_obj, 
	    X400_att.X400_N_IMPLICIT_CONVERSION_PROHIBITED, 1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddintparam failed " + status);
	    return status;
	}

	// Alternate recipient allowed: 0 - no, 1 - yes
    	status = com.isode.x400api.X400.x400_msgaddintparam(message_obj, 
	    X400_att.X400_N_ALTERNATE_RECIPIENT_ALLOWED, 1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddintparam failed " + status);
	    return status;
	}

	// Content return request: 0 - no, 1 - yes
    	status = com.isode.x400api.X400.x400_msgaddintparam(message_obj, 
	    X400_att.X400_N_CONTENT_RETURN_REQUEST, 1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddintparam failed " + status);
	    return status;
	}

	// Recipient reassignment prohibited: 0 - no, 1 - yes
    	status = com.isode.x400api.X400.x400_msgaddintparam(message_obj, 
	    X400_att.X400_N_RECIPIENT_REASSIGNMENT_PROHIBITED, 1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddintparam failed " + status);
	    return status;
	}

	// Distribution List expansion prohibited: 0 - no, 1 - yes
    	status = com.isode.x400api.X400.x400_msgaddintparam(message_obj, 
	    X400_att.X400_N_DL_EXPANSION_PROHIBITED, 1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddintparam failed " + status);
	    return status;
	}

	// Conversion with loss prohibited: 0 - no, 1 - yes
    	status = com.isode.x400api.X400.x400_msgaddintparam(message_obj, 
	    X400_att.X400_N_CONVERSION_WITH_LOSS_PROHIBITED, 1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddintparam failed " + status);
	    return status;
	}

	// Message Identifier. In RFC 2156 String form
    	status = com.isode.x400api.X400.x400_msgaddstrparam(message_obj, 
	    X400_att.X400_S_MESSAGE_IDENTIFIER, msg_id, msg_id.length());
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddintparam failed " + status);
	    return status;
	}

	// Content Identifier
    	status = com.isode.x400api.X400.x400_msgaddstrparam(message_obj, 
	    X400_att.X400_S_CONTENT_IDENTIFIER, content_id, 
	    content_id.length());
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddintparam failed " + status);
	    return status;
	}

	//  Latest Delivery Time: UTCTime format YYMMDDHHMMSS<zone>
    	status = com.isode.x400api.X400.x400_msgaddstrparam(message_obj, 
	    X400_att.X400_S_LATEST_DELIVERY_TIME, latest_del_time, 
	    latest_del_time.length());
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddintparam failed " + status);
	    return status;
	}

	//  Originator Return Address (X.400 String format)
    	status = com.isode.x400api.X400.x400_msgaddstrparam(message_obj, 
	    X400_att.X400_S_ORIGINATOR_RETURN_ADDRESS, orig_ret_addr, 
	    orig_ret_addr.length());
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddintparam failed " + status);
	    return status;
	}

        status = com.isode.x400api.X400.x400_msgaddstrparam(message_obj, 
	    X400_att.X400_S_MESSAGE_DELIVERY_TIME,del_time , -1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddstrparam failed " + status);
	    return status;
	}
        
	return status;

    }

    /**
     * Build the content for a message to be added as a message body part.
     */
    private static int build_content(Message message_obj)
    {
	int status;
	int num_atts = 0;
	String subject = "Forwarded: Test message from Java";
	String content_ia5= "Forwarded: IA5 content from Java\r\nLine 1\r\nLine 2";
	String content_ia5_att = "Forwarded: IA5 att content from Java\r\nLine 1\r\nLine 2";
	String content_8859_1 = "Forwarded: 8859_1 content from Java\r\nLine 1\r\nLine 2\u00a3\u00a3";
	String content_8859_2 = "Forwarded: 8859_2 content from Java\r\nLine 1\r\nLine 2\u00a3\u00a3";
	String content_bin = "Forwarded: binary content from Java\r\nLine 1\r\nLine 2\u00a3\u00a3";
	String content_ftbp = "Forwarded: ftbp content from Java\r\nLine 1\r\nLine 2\u00a3\u00a3";
	byte[] emptybinarydata = new byte[0];
	byte[] binarydata = new byte[] {97,97,98,105,110,97,114,121,32,99,111,110,116,101,110,116,32,102,114,111,109,32,74,97,118,97,13,10,76,105,110,101,32,49,13,10,76,105,110,101,32,50,0,97,0};

	String ipm_id = "1064400656.24922*";
	String ipm_rep_id = "1064400656.24923*";
	String ipm_obs_id = "1064400656.24924*";
	String ipm_rel_id = "1064400656.24925*";
	String orig_ref = "orig-ref-val";
	String def_utc = "050924120000";

	int importance = 2;
	int sensitivity = 3;
	int autoforwarded = 1;

    	status = com.isode.x400api.X400.x400_msgaddstrparam(message_obj, 
	    X400_att.X400_S_IPM_IDENTIFIER, ipm_id, ipm_id.length());
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddstrparam failed " + status);
	    return status;
	}

    	status = com.isode.x400api.X400.x400_msgaddstrparam(message_obj, 
	    X400_att.X400_S_SUBJECT, subject, -1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddstrparam failed " + status);
	    return status;
	}

    	status = com.isode.x400api.X400.x400_msgaddstrparam(message_obj, 
	    X400_att.X400_S_REPLIED_TO_IDENTIFIER, ipm_rep_id, 
	    ipm_rep_id.length());
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddstrparam failed " + status);
	    return status;
	}

    	status = com.isode.x400api.X400.x400_msgaddstrparam(message_obj, 
	    X400_att.X400_S_OBSOLETED_IPMS, ipm_obs_id, 
	    ipm_obs_id.length());
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddstrparam failed " + status);
	    return status;
	}

    	status = com.isode.x400api.X400.x400_msgaddstrparam(message_obj, 
	    X400_att.X400_S_RELATED_IPMS, ipm_rel_id, ipm_rel_id.length());
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddstrparam failed " + status);
	    return status;
	}

    	status = com.isode.x400api.X400.x400_msgaddstrparam(message_obj, 
	    X400_att.X400_S_EXPIRY_TIME, def_utc, def_utc.length());
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddstrparam failed " + status);
	    return status;
	}

    	status = com.isode.x400api.X400.x400_msgaddstrparam(message_obj, 
	    X400_att.X400_S_REPLY_TIME, def_utc, def_utc.length());
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddstrparam failed " + status);
	    return status;
	}

    	status = com.isode.x400api.X400.x400_msgaddintparam(message_obj, 
	    X400_att.X400_N_IMPORTANCE, importance);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddintparam failed " + status);
	    return status;
	}

    	status = com.isode.x400api.X400.x400_msgaddintparam(message_obj, 
	    X400_att.X400_N_SENSITIVITY, sensitivity);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddintparam failed " + status);
	    return status;
	}

    	status = com.isode.x400api.X400.x400_msgaddintparam(message_obj, 
	    X400_att.X400_N_AUTOFORWARDED, autoforwarded);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddintparam failed " + status);
	    return status;
	}

	// now add the attachments/bodyparts

	// Add an IA5 attachment using AddStrParam
    	status = com.isode.x400api.X400.x400_msgaddstrparam(message_obj, 
	    X400_att.X400_T_IA5TEXT, content_ia5, content_ia5.length());
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddstrparam failed " + status);
	    return status;
	}
	num_atts++;

	// Add an IA5 attachment
    	status = com.isode.x400api.X400.x400_msgaddattachment(message_obj, 
	    X400_att.X400_T_IA5TEXT, content_ia5_att, content_ia5_att.length(),
		emptybinarydata);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddattachment failed " + status);
	    return status;
	}
	num_atts++;

	// Add an 8859-1 attachment
    	status = com.isode.x400api.X400.x400_msgaddattachment(message_obj, 
	    X400_att.X400_T_ISO8859_1, content_8859_1, content_8859_1.length(),
		emptybinarydata);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddattachment failed " + status);
	    return status;
	}
	num_atts++;

	// Add an 8859-2 attachment
    	status = com.isode.x400api.X400.x400_msgaddattachment(message_obj, 
	    X400_att.X400_T_ISO8859_1, content_8859_2, content_8859_2.length(),
		emptybinarydata);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddattachment failed " + status);
	    return status;
	}
	num_atts++;

        // Add an bin attachment
        status = com.isode.x400api.X400.x400_msgaddattachment(
            message_obj, X400_att.X400_T_BINARY, content_bin,
            content_bin.length(), binarydata);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_ms_msgaddattachment failed (bin)" + status);
            return status;
        }

	// record number of attachments
    	status = com.isode.x400api.X400.x400_msgaddintparam(message_obj, 
	    X400_att.X400_N_NUM_ATTACHMENTS, num_atts);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddintparam failed " + status);
	    return status;
	}

        num_atts++;

	return status;

    }

}

