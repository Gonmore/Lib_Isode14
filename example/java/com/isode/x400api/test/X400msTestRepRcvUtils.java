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
 * These methods handle a report received from the message store.
 */


package com.isode.x400api.test;

import com.isode.x400api.*;
import com.isode.x400api.X400ms.*;
import com.isode.x400api.test.config;

public class X400msTestRepRcvUtils
{
    /**
     * Display a report envelope (ie from a report retrieved 
     * the message store).
     */
    public static int do_rep_env(MSMessage msmessage_obj)
    {
	int status;
	int paramtype;
	int len;
	String value;

	System.out.println("---------------");
	System.out.println("Report Envelope");

	// Initialise object to receive returned String value
	StringBuffer ret_value = new StringBuffer();

	// get originator address - which should be invalid for a report
	paramtype = X400_att.X400_S_OR_ADDRESS;
	status = com.isode.x400api.X400ms.x400_ms_msggetstrparam(
	    msmessage_obj, paramtype, ret_value);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for originator address(" + paramtype + ") failed " + status + " (as expected for a report)");
	} else {
	    len = ret_value.length();
	    System.out.println("Originator: (" + len + ")" +  ret_value.toString());
	}

        System.out.println("get envelope redihist");
        RediHist redihist_obj = new RediHist();
        int entry = 1;
        status = X400_att.X400_E_NOERROR;
        while(status == X400_att.X400_E_NOERROR)
            {
                status = X400msTestRcvUtils.get_ms_redihist(msmessage_obj,
                                                            null,
                                                            entry,
                                                            redihist_obj);
                if (status != X400_att.X400_E_NOERROR) {
                    System.out.println("get_ms_redihist failed " + status);
                    break;
                }
                entry++;
            }
        
	return X400_att.X400_E_NOERROR;
    }

    /**
     * Display report content (ie from a report retrieved 
     * the message store).
     */
    public static int do_rep_content(MSMessage msmessage_obj)
    {
	int status;
	int paramtype;
	int recip_num = 1;
	int len = -1, maxlen = -1;
	String value; // string to contain value returned from API 
	int int_value; // int to contain value returned from API 

	System.out.println("-------------");
	System.out.println("Report Content");

	// Initialise object to receive returned String value
	StringBuffer ret_value = new StringBuffer();

	// display subject identifier
	paramtype = X400_att.X400_S_SUBJECT_IDENTIFIER;
	status = com.isode.x400api.X400ms.x400_ms_msggetstrparam(
	    msmessage_obj, paramtype, ret_value);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for Subject ID(" + paramtype + ") failed " + status );
	} else {
	    len = ret_value.length();
	    System.out.println("Subject Identifier: (" + len + ")" +  ret_value.toString());
	}


	// instantiate a recip object, and retrieve a recip
	// putting it into an API object 
	Recip recip_obj = new Recip();
	// There should only be one recip of course
	for ( recip_num = 1; ; recip_num++ ) {
	
	    // get the recip
	    status = com.isode.x400api.X400ms.x400_ms_recipget(msmessage_obj, 
		X400_att.X400_RECIP_REPORT, recip_num, recip_obj);
	    if (status == X400_att.X400_E_NO_RECIP) {
		System.out.println("no more recips ...");
		break;
	    }
	    else if (status != X400_att.X400_E_NOERROR) {
		System.out.println("x400_ms_recipget failed " + status);
		break;
	    }

	    System.out.println("-------------- Recipient " + recip_num + "--------------");
	    // display recip properties
	    paramtype = X400_att.X400_S_OR_ADDRESS;
	    status = com.isode.x400api.X400ms.x400_ms_recipgetstrparam(
		recip_obj, paramtype, ret_value);
	    if (status != X400_att.X400_E_NOERROR) {
		System.out.println("no string value for originator address(" + paramtype + ") failed " + status);
	    } else {
		len =  ret_value.length();
		System.out.println("Subject Recipient " + recip_num + ": (" + len + ")" +  ret_value.toString());
	    }

	    // display recip properties
	    // Delivery time should be used to determine whether this
	    // is a positive or negative report
	    paramtype = X400_att.X400_S_MESSAGE_DELIVERY_TIME;
	    status = com.isode.x400api.X400ms.x400_ms_recipgetstrparam(
		recip_obj, paramtype, ret_value);
	    if (status != X400_att.X400_E_NOERROR) {
		System.out.println("no string value for Subject Message Delivery Time(" + paramtype + ") failed " + status);
	    } else {
		len =  ret_value.length();
		System.out.println("Subject Message Delivery Time:" + "(" + len + ")" +  ret_value.toString());
	    }

	    // display recip properties
	    paramtype = X400_att.X400_N_TYPE_OF_USER;
	    status = com.isode.x400api.X400ms.x400_ms_recipgetintparam(
		recip_obj, paramtype);
	    if (status != X400_att.X400_E_NOERROR) {
		System.out.println("no int value for Type of MTS user(" + paramtype + ") failed " + status);
	    } else {
		int_value = recip_obj.GetIntValue();
		System.out.println("Type of MTS user " + int_value);
	    }

	    // display recip properties
	    paramtype = X400_att.X400_S_SUPPLEMENTARY_INFO;
	    status = com.isode.x400api.X400ms.x400_ms_recipgetstrparam(
		recip_obj, paramtype, ret_value);
	    if (status != X400_att.X400_E_NOERROR) {
		System.out.println("no string value for Supplementary info(" + paramtype + ") failed " + status);
	    } else {
		len =  ret_value.length();
		System.out.println("Supplementary info" + "(" + len + ")" +  ret_value.toString());
	    }

	    // display recip properties
	    paramtype = X400_att.X400_N_NON_DELIVERY_REASON;
	    status = com.isode.x400api.X400ms.x400_ms_recipgetintparam(
		recip_obj, paramtype);
	    if (status != X400_att.X400_E_NOERROR) {
		System.out.println("no int value for Non-delivery Reason(" + paramtype + ") failed " + status);
	    } else {
		int_value = recip_obj.GetIntValue();
		System.out.println("Non-delivery Reason  " + int_value);
	    }

	    // display recip properties
	    paramtype = X400_att.X400_N_NON_DELIVERY_DIAGNOSTIC;
	    status = com.isode.x400api.X400ms.x400_ms_recipgetintparam(
		recip_obj, paramtype);
	    if (status != X400_att.X400_E_NOERROR) {
		System.out.println("no int value for Non-delivery diagnostic(" + paramtype + ") failed " + status);
	    } else {
		int_value = recip_obj.GetIntValue();
		System.out.println("Non-delivery diagnostic  " + int_value);
	    }
	}

	return X400_att.X400_E_NOERROR;
    }

    /**
     * Display the report returned content (ie from a report retrieved 
     * the message store). Note that there will only be returned 
     * content for negative reports.
     */
    public static int do_rep_retcontent(MSMessage msmessage_obj)
    {
	int status;
	int paramtype;
	int recip_num = 1;
	int len = -1, maxlen = -1;
	String value; // string to contain value returned from API 
	int int_value; // int to contain value returned from API 

	System.out.println("-----------------------");
	System.out.println("Report Returned Content");

	/* the content can be retrieved as attachments or
	 * bodyparts. Use do_msg_content for the former */
	return (X400msTestBodyPartRcvUtils.do_msg_content_as_bp(msmessage_obj));
    }

}

