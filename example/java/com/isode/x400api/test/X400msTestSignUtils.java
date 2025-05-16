/*  Copyright (c) 2008-2009, Isode Limited, London, England.
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
 * setup_default_sec_env causes a message to be signed by specifying a 
 * security environment.
 * get_sig_details provides a method which returns signature verification 
 * information.
 */


package com.isode.x400api.test;

import com.isode.x400api.*;
import com.isode.x400api.X400ms.*;
import com.isode.x400api.test.config;

public class X400msTestSignUtils
{

    /**
     * Setup default security environment so that message can be signed.
     */
    public static int setup_default_sec_env(Session session_obj, String security_id, String identity_dn, String passphrase)
    {
	int status = X400_att.X400_E_NOERROR;

	status = com.isode.x400api.X400ms.x400_ms_setstrdefault(session_obj, 
	    X400_att.X400_S_SEC_IDENTITY, security_id, -1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_setstrdefault failed " + status);
	    return status;
	}
	System.out.println("Set security ID successfully" + security_id);

	status = com.isode.x400api.X400ms.x400_ms_setstrdefault(session_obj, 
	    X400_att.X400_S_SEC_IDENTITY_DN, identity_dn, -1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_setstrdefault failed " + status);
	    return status;
	}
	System.out.println("Set security ID DN successfully to " + identity_dn);

	status = com.isode.x400api.X400ms.x400_ms_setstrdefault(session_obj, 
	    X400_att.X400_S_SEC_IDENTITY_PASSPHRASE, passphrase, -1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_setstrdefault failed " + status);
	    return status;
	}
	System.out.println("Set security ID PPHR successfully to " + passphrase);

	return status;
    }

    public static void get_sig_details(MSMessage msmessage_obj)
    {
	int status = X400_att.X400_E_NOERROR;
	StringBuffer ret_value = new StringBuffer();


	return;
    }
}

