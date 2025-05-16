/*  Copyright (c) 2008, Isode Limited, London, England.
 *  All rights reserved.
 *                                                                       
 *  Acquisition and use of this software and related materials for any      
 *  purpose requires a written licence agreement from Isode Limited,
 *  or a written licence from an organisation licenced by Isode Limited
 *  to grant such a licence.
 *
 */


/**
 * Support methods for Java X.400 API test program
 * These methods handle a message received as a forwarded message
 * obtained using the GetBodypart methods.
 */


package com.isode.x400api.test;

import com.isode.x400api.*;
import com.isode.x400api.X400ms.*;
import com.isode.x400api.test.config;

public class X400msTestRcvNativeUtils
{ 
    /* This is probably redundant as X400 messages 
     * retrieved from a BodyPart don't have envelopes */
    public static int do_xmsg_env(Message message)
    {
	int status;
	int paramtype;
	int len = -1, maxlen = -1;
	int recip_type = 1;
	int recip_num = 1;

	String value; // string to contain value returned from API 
	int int_value; // int to contain value returned from API 

	System.out.println("do_xmsg_env");

	// Initialise object to receive returned String value
	StringBuffer ret_value = new StringBuffer();

	// get orig address: string attribute from the message envelope
	paramtype = X400_att.X400_S_OR_ADDRESS;
    	status = com.isode.x400api.X400.x400_msggetstrparam(
	    message, paramtype, ret_value);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no string value for X400_S_OR_ADDRESS " 
	    + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msggetstrparam failed " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Originator address (" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message envelope
	paramtype = X400_att.X400_S_MESSAGE_IDENTIFIER;
    	status = com.isode.x400api.X400.x400_msggetstrparam(
	    message, paramtype, ret_value);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no string value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msggetstrparam failed " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Message ID(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message envelope
	paramtype = X400_att.X400_S_CONTENT_IDENTIFIER;
    	status = com.isode.x400api.X400.x400_msggetstrparam(
	    message, paramtype, ret_value);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no string value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msggetstrparam failed " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Content ID(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message envelope
	paramtype = X400_att.X400_S_ORIGINAL_ENCODED_INFORMATION_TYPES;
    	status = com.isode.x400api.X400.x400_msggetstrparam(
	    message, paramtype, ret_value);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no string value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msggetstrparam failed " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Orig EITs(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message envelope
	paramtype = X400_att.X400_S_MESSAGE_SUBMISSION_TIME;
    	status = com.isode.x400api.X400.x400_msggetstrparam(
	    message, paramtype, ret_value);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no string value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msggetstrparam failed " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Message submission time(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message envelope
	paramtype = X400_att.X400_S_MESSAGE_DELIVERY_TIME;
    	status = com.isode.x400api.X400.x400_msggetstrparam(
	    message, paramtype, ret_value);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no string value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msggetstrparam failed " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Message delivery time(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message envelope
	paramtype = X400_att.X400_S_LATEST_DELIVERY_TIME;
    	status = com.isode.x400api.X400.x400_msggetstrparam(
	    message, paramtype, ret_value);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no string value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msggetstrparam failed " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Latest delivery time(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message envelope
	paramtype = X400_att.X400_S_ORIGINATOR_RETURN_ADDRESS;
    	status = com.isode.x400api.X400.x400_msggetstrparam(
	    message, paramtype, ret_value);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no string value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msggetstrparam failed " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Orig return address(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message envelope
	paramtype = X400_att.X400_S_MESSAGE_DELIVERY_TIME;
    	status = com.isode.x400api.X400.x400_msggetstrparam(
	    message, paramtype, ret_value);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no string value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msggetstrparam failed " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Message delivery time(" + len + ") " +  ret_value.toString());
	}

	// get some integer attributes from the message
	paramtype = X400_att.X400_N_CONTENT_TYPE;
    	status = com.isode.x400api.X400.x400_msggetintparam(
	    message, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msggetintparam failed " + status);
	} else {
	    int_value = message.GetIntValue();
	    System.out.println("Content type " + int_value);
	}

	paramtype = X400_att.X400_N_NUM_ATTACHMENTS;
    	status = com.isode.x400api.X400.x400_msggetintparam(
	    message, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msggetintparam failed " + status);
	} else {
	    int_value = message.GetIntValue();
	    System.out.println("Num attachments " + int_value);
	}

	paramtype = X400_att.X400_N_PRIORITY;
    	status = com.isode.x400api.X400.x400_msggetintparam(
	    message, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msggetintparam failed " + status);
	} else {
	    int_value = message.GetIntValue();
	    System.out.println("Priority " + int_value);
	}

	paramtype = X400_att.X400_N_DISCLOSURE;
    	status = com.isode.x400api.X400.x400_msggetintparam(
	    message, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msggetintparam failed " + status);
	} else {
	    int_value = message.GetIntValue();
	    System.out.println("Disclosure of recips prohibited " + int_value);
	}

	paramtype = X400_att.X400_N_IMPLICIT_CONVERSION_PROHIBITED;
    	status = com.isode.x400api.X400.x400_msggetintparam(
	    message, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msggetintparam failed " + status);
	} else {
	    int_value = message.GetIntValue();
	    System.out.println("Alternate recipient allowed " + int_value);
	}

	paramtype = X400_att.X400_N_CONTENT_RETURN_REQUEST;
    	status = com.isode.x400api.X400.x400_msggetintparam(
	    message, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msggetintparam failed " + status);
	} else {
	    int_value = message.GetIntValue();
	    System.out.println("Content return request " + int_value);
	}

	paramtype = X400_att.X400_N_RECIPIENT_REASSIGNMENT_PROHIBITED;
    	status = com.isode.x400api.X400.x400_msggetintparam(
	    message, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msggetintparam failed " + status);
	} else {
	    int_value = message.GetIntValue();
	    System.out.println("Recipient reassignment prohibited " + int_value);
	}

	paramtype = X400_att.X400_N_DL_EXPANSION_PROHIBITED;
    	status = com.isode.x400api.X400.x400_msggetintparam(
	    message, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msggetintparam failed " + status);
	} else {
	    int_value = message.GetIntValue();
	    System.out.println("Distribution List expansion prohibited " + int_value);
	}

	paramtype = X400_att.X400_N_CONVERSION_WITH_LOSS_PROHIBITED;
    	status = com.isode.x400api.X400.x400_msggetintparam(
	    message, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msggetintparam failed " + status);
	} else {
	    int_value = message.GetIntValue();
	    System.out.println("Conversion with loss prohibited " + int_value);
	}
	return X400_att.X400_E_NOERROR;
    }

    /**
     * Fetch and display the X.400 message headers (ie from a message
     * retrieved using x400_msggetmessagebody().
     */
    public static int do_xmsg_headers(Message message)
    {
	int status;
	int paramtype;
	int len = -1, maxlen = -1;

	String value; // string to contain value returned from API 
	int int_value; // int to contain value returned from API 

	System.out.println("X400 Message Content:");
        System.out.println("----------------");

	// Initialise object to receive returned String value
	StringBuffer ret_value = new StringBuffer();

	// get some string attributes from the message content
	paramtype = X400_att.X400_S_IPM_IDENTIFIER;
    	status = com.isode.x400api.X400.x400_msggetstrparam(
	    message, paramtype, ret_value);
	if (status != X400_att.X400_E_NOERROR) {
            System.out.println("no string value for IPM ID(" + paramtype +
		") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("IPM IDentifier(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message content
	paramtype = X400_att.X400_S_SUBJECT;
    	status = com.isode.x400api.X400.x400_msggetstrparam(
	    message, paramtype, ret_value);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for Subject(" + paramtype +
	        ") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Subject(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message content
	paramtype = X400_att.X400_S_REPLIED_TO_IDENTIFIER;
    	status = com.isode.x400api.X400.x400_msggetstrparam(
	    message, paramtype, ret_value);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for replied to ID(" 
		 + paramtype + ") result is " + status);

	} else {
	    len = ret_value.length();
	    System.out.println("Replied-to-identifier(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message content
	paramtype = X400_att.X400_S_OBSOLETED_IPMS;
    	status = com.isode.x400api.X400.x400_msggetstrparam(
	    message, paramtype, ret_value);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for obsoleted IPMs("
                + paramtype + ") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Obsoleted IPMs(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message content
	paramtype = X400_att.X400_S_RELATED_IPMS;
    	status = com.isode.x400api.X400.x400_msggetstrparam(
	    message, paramtype, ret_value);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for related IPMs("
                + paramtype + "result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Related IPMs(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message content
	paramtype = X400_att.X400_S_EXPIRY_TIME;
    	status = com.isode.x400api.X400.x400_msggetstrparam(
	    message, paramtype, ret_value);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for expiry time(" + paramtype +
                ") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Expiry Time(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message content
	paramtype = X400_att.X400_S_REPLY_TIME;
    	status = com.isode.x400api.X400.x400_msggetstrparam(
	    message, paramtype, ret_value);
	if (status != X400_att.X400_E_NOERROR) {
	     System.out.println("no string value for reply time(" + paramtype +
                ") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Reply Time(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message content
	paramtype = X400_att.X400_S_AUTHORIZATION_TIME;
    	status = com.isode.x400api.X400.x400_msggetstrparam(
	    message, paramtype, ret_value);
	if (status != X400_att.X400_E_NOERROR) {
	     System.out.println("no string value for auth time(" + paramtype +
                ") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Authorisation Time(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message content
	paramtype = X400_att.X400_S_ORIGINATORS_REFERENCE;
    	status = com.isode.x400api.X400.x400_msggetstrparam(
	    message, paramtype, ret_value);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for orig ref(" + paramtype +
                ") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Originator's Reference(" + len + ") " +  ret_value.toString());
	}

	paramtype = X400_att.X400_N_IMPORTANCE;
    	status = com.isode.x400api.X400.x400_msggetintparam(
	    message, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for importance " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msggetintparam failed " + status);
	} else {
	    int_value = message.GetIntValue();
	    System.out.println("Importance " + int_value);
	}

	paramtype = X400_att.X400_N_SENSITIVITY;
    	status = com.isode.x400api.X400.x400_msggetintparam(
	    message, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for sensitivity"
		+ paramtype + ")");
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msggetintparam failed " + status);
	} else {
	    int_value = message.GetIntValue();
	    System.out.println("Sensitivity " + int_value);
	}

	paramtype = X400_att.X400_N_AUTOFORWARDED;
    	status = com.isode.x400api.X400.x400_msggetintparam(
	    message, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no int value for autoforwarded ("
		+ paramtype + ")");
	} else {
	    int_value = message.GetIntValue();
	    System.out.println("Autoforwarded " + int_value);
	}

	paramtype = X400_att.X400_N_NUM_ATTACHMENTS;
    	status = com.isode.x400api.X400.x400_msggetintparam(
	    message, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	     System.out.println("no int value for num atts "
		+ paramtype + ")");
	} else {
	    int_value = message.GetIntValue();
	    System.out.println("number of attachments " + int_value);
	}

	paramtype = X400_att.X400_ORIGINATOR;
    	status = com.isode.x400api.X400.x400_msggetstrparam(
	    message, paramtype, ret_value);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for orig(" + paramtype +
                ") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Originator(" + len + ") " +  ret_value.toString());
	}

	status = get_recips(message, X400_att.X400_ORIGINATOR, "orig: ");
	if ( status != X400_att.X400_E_NOERROR 
	    && status != X400_att.X400_E_NO_RECIP) {
	    System.out.println("get_recips (env) failed " + status);
	    return status;
	}
	status = get_recips(message, X400_att.X400_RECIP_PRIMARY, "primary: ");
	if ( status != X400_att.X400_E_NOERROR 
	    && status != X400_att.X400_E_NO_RECIP) {
	    System.out.println("get_recips (primary) failed " + status);
	    return status;
	}
	return status;
    }

    /**
     * Fetch and display the X.400 recipients (ie from a message
     * retrieved using x400_msggetmessagebody().
     */
    private static int get_recips(Message message, int type, String logstr)  {
	int status;
	int paramtype;
	int recip_num = 1;
	int len = -1, maxlen = -1;
	// String value; // string to contain value returned from API 
	int int_value; // int to contain value returned from API 

	// Initialise object for returning string values
	StringBuffer ret_value = new StringBuffer();

	// instantiate a recip object, and retrieve a recip
	// putting it into an API object 
	Recip recip_obj = new Recip();
	for ( recip_num = 1; ; recip_num++ ) {
	
	    // get the recip
	    status = com.isode.x400api.X400.x400_msggetrecip(message, 
		type, recip_num, recip_obj);
	    if (status == X400_att.X400_E_NO_RECIP) {
		if (recip_num == 1) {
		    System.out.println("No recips of type " + logstr);
		} else {
		    System.out.println("no more recips ...");
		}
		break;
	    }
	    else if (status != X400_att.X400_E_NOERROR) {
		System.out.println("x400_recipget failed " + status);
		break;
	    }

	    System.out.println("-------------- X.400 Recipient " + recip_num + "--------------");
	    // Get envelope values 
	    if (type == X400_att.X400_RECIP_ENVELOPE) {
		// display recip properties
		paramtype = X400_att.X400_S_OR_ADDRESS;
		status = com.isode.x400api.X400.x400_recipgetstrparam(
		    recip_obj, paramtype, ret_value);
		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("x400_recipgetstrparam X400_S_OR_ADDRESS(" + paramtype + ") failed " + status);
		} else {
		    len =  ret_value.length();
		    System.out.println(logstr + recip_num + "(" + len + ")" +  ret_value.toString());
		}

		// display recip properties
		paramtype = X400_att.X400_N_RESPONSIBILITY;
		status = com.isode.x400api.X400.x400_recipgetintparam(
		    recip_obj, paramtype);
		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("x400_recipgetintparam(" + paramtype + ") failed " + status);
		} else {
		    int_value = message.GetIntValue();
		    System.out.println("Responsibility " + int_value);
		}

		// display recip properties
		paramtype = X400_att.X400_N_MTA_REPORT_REQUEST;
		status = com.isode.x400api.X400.x400_recipgetintparam(
		    recip_obj, paramtype);
		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("x400_recipgetintparam(" + paramtype + ") failed " + status);
		} else {
		    int_value = message.GetIntValue();
		    System.out.println("MTA report request " + int_value);
		}

		// display recip properties
		paramtype = X400_att.X400_N_REPORT_REQUEST;
		status = com.isode.x400api.X400.x400_recipgetintparam(
		    recip_obj, paramtype);
		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("x400_recipgetintparam(" + paramtype + ") failed " + status);
		} else {
		    int_value = message.GetIntValue();
		    System.out.println("Originator report request " + int_value);
		}

	    }

	    // Get content values 
	    if (type != X400_att.X400_RECIP_ENVELOPE) {
		// display recip address
		paramtype = X400_att.X400_S_OR_ADDRESS;
		status = com.isode.x400api.X400.x400_recipgetstrparam(
		    recip_obj, paramtype, ret_value);
		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("x400_recipgetstrparam X400_S_OR_ADDRESS(" + paramtype + ") failed " + status);
		} else {
		    len =  ret_value.length();
		    System.out.println(logstr + recip_num + "(" + len + ")" +  ret_value.toString());
		}

		// display recip properties
		paramtype = X400_att.X400_N_NOTIFICATION_REQUEST;
		status = com.isode.x400api.X400.x400_recipgetintparam(
		    recip_obj, paramtype);
		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("x400_recipgetintparam(" + paramtype + ") failed " + status);
		} else {
		    int_value = message.GetIntValue();
		    System.out.println("Responsibility " + int_value);
		}

		// display recip properties
		paramtype = X400_att.X400_N_REPLY_REQUESTED;
		status = com.isode.x400api.X400.x400_recipgetintparam(
		    recip_obj, paramtype);
		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("x400_recipgetintparam(" + paramtype + ") failed " + status);
		} else {
		    int_value = message.GetIntValue();
		    System.out.println("Reply Request " + int_value);
		}

		// display recip properties
		paramtype = X400_att.X400_S_FREE_FORM_NAME;
		status = com.isode.x400api.X400.x400_recipgetstrparam(
		    recip_obj, paramtype, ret_value);
		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("x400_recipgetstrparam(" + paramtype + ") failed " + status);
		} else {
		    len =  ret_value.length();
		    System.out.println("Free form name" + "(" + len + ")" +  ret_value.toString());
		}

		// display recip properties
		paramtype = X400_att.X400_S_TELEPHONE_NUMBER;
		status = com.isode.x400api.X400.x400_recipgetstrparam(
		    recip_obj, paramtype, ret_value);
		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("x400_recipgetstrparam(" + paramtype + ") failed " + status);
		} else {
		    len =  ret_value.length();
		    System.out.println("Telephone Number" + "(" + len + ")" +  ret_value.toString());
		}
	    }


	    // display recip properties
	    paramtype = X400_att.X400_S_MESSAGE_DELIVERY_TIME;
	    status = com.isode.x400api.X400.x400_recipgetstrparam(
		recip_obj, paramtype, ret_value);
	    if (status != X400_att.X400_E_NOERROR) {
		System.out.println("x400_recipgetstrparam(" + paramtype + ") failed " + status);
	    } else {
		len =  ret_value.length();
		System.out.println("Subject Message Delivery Time:" + "(" + len + ")" +  ret_value.toString());
	    }

	    // display recip properties
	    paramtype = X400_att.X400_N_TYPE_OF_USER;
	    status = com.isode.x400api.X400.x400_recipgetintparam(
		recip_obj, paramtype);
	    if (status != X400_att.X400_E_NOERROR) {
		System.out.println("x400_recipgetintparam(" + paramtype + ") failed " + status);
	    } else {
		int_value = message.GetIntValue();
		System.out.println("Type of MTS user " + int_value);
	    }

	    // display recip properties
	    paramtype = X400_att.X400_S_SUPPLEMENTARY_INFO;
	    status = com.isode.x400api.X400.x400_recipgetstrparam(
		recip_obj, paramtype, ret_value);
	    if (status != X400_att.X400_E_NOERROR) {
		System.out.println("x400_recipgetstrparam(" + paramtype + ") failed " + status);
	    } else {
		len =  ret_value.length();
		System.out.println("Supplementary info" + "(" + len + ")" +  ret_value.toString());
	    }

	    // display recip properties
	    paramtype = X400_att.X400_N_NON_DELIVERY_REASON;
	    status = com.isode.x400api.X400.x400_recipgetintparam(
		recip_obj, paramtype);
	    if (status != X400_att.X400_E_NOERROR) {
		System.out.println("x400_recipgetintparam(" + paramtype + ") failed " + status);
	    } else {
		int_value = message.GetIntValue();
		System.out.println("Non-delivery Reason  " + int_value);
	    }

	    // display recip properties
	    paramtype = X400_att.X400_N_NON_DELIVERY_DIAGNOSTIC;
	    status = com.isode.x400api.X400.x400_recipgetintparam(
		recip_obj, paramtype);
	    if (status != X400_att.X400_E_NOERROR) {
		System.out.println("x400_recipgetintparam(" + paramtype + ") failed " + status);
	    } else {
		int_value = message.GetIntValue();
		System.out.println("Non-delivery diagnostic  " + int_value);
	    }


	}
	return X400_att.X400_E_NOERROR;
    }

    /**
     * Fetch and display the X.400 message content (ie from a message
     * retrieved using x400_msggetmessagebody().
     * This code uses msmsggetattachment if possible (rather than bodyparts).
     */
    public static int do_xmsg_content(Message message)
    {
	int status;
	int paramtype;
	int int_value;
	int len;
	String value;
	int att_num;
	int att_type;
	int att_len;
        byte[] binarydata = new byte[config.maxlen];

	paramtype = X400_att.X400_N_NUM_ATTACHMENTS;
    	status = com.isode.x400api.X400.x400_msggetintparam(
	    message, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msggetintparam failed " + status);
	} else {
	    int_value = message.GetIntValue();
	    System.out.println("number of attachments " + int_value);
	}

	// Initialise object to receive returned String value
	StringBuffer ret_value = new StringBuffer();

	// Get an IA5 attachment as a string in the message
        System.out.println("------------------------");
        System.out.println("Reading IA5 Attachment");
	paramtype = X400_att.X400_T_IA5TEXT;
	status = com.isode.x400api.X400.x400_msggetstrparam(
	    message, paramtype, ret_value);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msggetstrparam failed " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("IA5 Text " + "(" + len + ")" +  ret_value.toString());
	}

	// Get all the attachments/bodyparts..
	BodyPart bodypart_obj = new BodyPart();
	Message message_obj = new Message();
	for ( att_num = 1; ; att_num++ ) {
	    status = com.isode.x400api.X400.x400_msggetattachment(
		message, att_num, ret_value, binarydata);
	    if (status == X400_att.X400_E_COMPLEX_BODY) {
	    	// need to get it as a bodypart object
		System.out.println("Forwarded BodyPart " 
		    + att_num + " is complex");
		status = get_bp(message, bodypart_obj, att_num);
		if (status != X400_att.X400_E_NOERROR) {
		    break;
		}
	    }
	    else if (status == X400_att.X400_E_MESSAGE_BODY) {
	    	// need to get it as a message bodypart
		System.out.println("Forwarded BodyPart " 
		    + att_num + " is a message");
	    	status = get_msgbp(message, message_obj, att_num);
		if (status != X400_att.X400_E_NOERROR) {
		    break;
		}
	    }
	    else if (status != X400_att.X400_E_NOERROR) {
		break;
	    } else {
		att_type = message.GetAttType();
		att_len = message.GetAttLen();
		System.out.println("Att type " + att_type);
		System.out.println("Att len " + att_len);
		switch (att_type) {
		    case X400_att.X400_T_BINARY:
		    case X400_att.X400_T_FTBP:
			System.out.println("Forwarded Binary Attachment (" 
			    + att_type + "), len = " + att_len);
			for (int i = 0; i < att_len ; i++) 
			     System.out.print(binarydata[i] + " ");
			 System.out.println();
			break;
		    default:
			len = ret_value.length();
			/* text attachment */
			System.out.println("Attachment " + att_num + "(" 
			    + len + ")\n" + ret_value.toString());
		}
	    }
	}

        System.out.println("Read " + att_num + " attachments");
        System.out.println("------------------------");
	return X400_att.X400_E_NOERROR;
    }

    /**
     * Fetch and display the X.400 message content (ie from a message
     * retrieved using x400_msggetmessagebody().
     * This code gets the attachments as body part object.
     */
    public static int do_xmsg_content_as_bp(Message message)
    {
	int status;
	int paramtype;
	int int_value;
	int len;
	String value;
	int att_num;
	int att_type;
	int att_len;
        byte[] binarydata = new byte[config.maxlen];

	
	paramtype = X400_att.X400_N_NUM_ATTACHMENTS;
    	status = com.isode.x400api.X400.x400_msggetintparam(
	    message, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msggetintparam failed " + status);
	    return status;
	} else {
	    int_value = message.GetIntValue();
	    System.out.println("Reading all attachments as bodyparts");
	    System.out.println("number of attachments " + int_value);
	}

	// Initialise object to receive returned String value
	StringBuffer ret_value = new StringBuffer();

	// Get all the bodyparts..
	BodyPart bodypart_obj = new BodyPart();
	for ( att_num = 1; ; att_num++ ) {
	    status = get_bp(message, bodypart_obj, att_num);
	    if (status == X400_att.X400_E_MESSAGE_BODY) {
		Message message_obj = new Message();

	    	// need to get it as a message bodypart
		System.out.println("Forwarded BodyPart " 
		    + att_num + " is a message");
	    	status = get_msgbp(message, message_obj, att_num);
		if (status != X400_att.X400_E_NOERROR) {
		    break;
		}
	    } else if (status == X400_att.X400_E_MISSING_ATTR) {
		System.out.println("No more bodyparts");
		break;
	    } else if (status != X400_att.X400_E_NOERROR) {
		return status;
	    }
	}

	return X400_att.X400_E_NOERROR;
    }

    /**
     * Fetch and display  X.400 message bodypart (ie from a message
     * retrieved using x400_msggetmessagebody().
     */
    private static int get_msgbp(Message message, Message message_obj, int att_num)
    {
	int status;
	int int_value;
	int type;
	int len;
	String value;

        System.out.println("------------------------");
        System.out.println("Reading Message BodyPart " + att_num);

	// get the message body part from the Message
	status = com.isode.x400api.X400.x400_msggetmessagebody(
	    message, att_num, message_obj);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msggetmessagebody failed " + status);
	    return status;
	}

	System.out.println("x400_msggetmessagebody succeeded " + status);

	// Initialise object to receive returned String value
	StringBuffer ret_value = new StringBuffer();

	// we've got the message body part OK, find out what it is ...
	status = com.isode.x400api.X400.x400_msggetstrparam(
                  message_obj, X400_att.X400_S_OBJECTTYPE, ret_value);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msggetstrparam failed " + status);
	    return status;
	}
	// get the returned values, and display it
	len = ret_value.length();
	//value = message_obj.GetStrValue();
	value = ret_value.toString();
	System.out.println("x400_msggetstrparam succeeded: " + value);
	if (value.equals("message")) {
            System.out.println("Retrieved msg body part");
            status = do_xmsg_env(message);
            status = do_xmsg_headers(message);
	    /* the content can be retrieved as attachments or
	     * bodyparts. Use do_xmsg_content for the former */
            status = do_xmsg_content_as_bp(message);
        } else if (value.equals("report")) {
            System.out.println("Retrieved report body part");
            status = do_xrep_env(message);
            status = do_xrep_headers(message);
            status = do_xrep_content(message);
        } else if (value.equals("probe")) {
            // Not handling a probe here 
            System.out.println("Retrieved probe body part");
	    return X400_att.X400_E_NYI;
        } else {
            // unknown type
            System.out.println("Retrieved unknown message  body part type " + value);
	    return X400_att.X400_E_BADPARAM;
        }
	return X400_att.X400_E_NOERROR;

    }

    /**
     * Fetch and display X.400 message bodypart (ie from a 
     * retrieved using x400_msggetmessagebody().
     */
    private static int get_bp(Message message, BodyPart bodypart_obj, int att_num)
    {
	int status;
	int int_value;
	int len;
	int bp_type;
        byte[] binarydata = new byte[config.maxlen];

        System.out.println("----------------");
        System.out.println("Reading X.400 BodyPart " + att_num);

	// Initialise object for returning string values
	StringBuffer ret_value = new StringBuffer();

	status = com.isode.x400api.X400.x400_msggetbodypart(
	    message, att_num, bodypart_obj);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msggetbodypart failed " + status);
	    return status;
	}

	// what kind of body part ?
	status = com.isode.x400api.X400.x400_bodypartgetintparam(
	    bodypart_obj, X400_att.X400_N_BODY_TYPE);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_bodypartgetintparam failed " + status);
	    return status;
	}
	int_value = bodypart_obj.GetIntValue();
	System.out.println("Got BodyPart int value : bp type is " + int_value);

	// read the data from the body part
	status = com.isode.x400api.X400.x400_bodypartgetstrparam(
	    bodypart_obj, X400_att.X400_S_BODY_DATA, ret_value, binarydata);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_bodypartgetstrparam failed " + status);
	    return status;
	}

	switch (int_value) {
	    case X400_att.X400_T_BINARY:
	    case X400_att.X400_T_FTBP:
		bp_type = bodypart_obj.GetBpType();
		len = bodypart_obj.GetBpLen();
		System.out.println("Binary Attachment (" 
		    + int_value + "), len = " + len);
		for (int i = 0; i < len ; i++) 
		    System.out.print(binarydata[i] + " ");
		System.out.println();
		break;
	    default:
		System.out.println("Got BodyPart string value(" 
		    + ret_value.capacity() + 
		    "): bp value is \n" +  ret_value.toString());
		break;
	}
	return status;
    }

    public static int do_xrep_env(Message message)
    {
	int status;
	int paramtype;

	return X400_att.X400_E_NYI;
    }

    public static int do_xrep_headers(Message message)
    {
	int status;
	int paramtype;

	return X400_att.X400_E_NYI;
    }

    public static int do_xrep_content(Message message)
    {
	int status;
	int paramtype;

	return X400_att.X400_E_NYI;
    }

}

