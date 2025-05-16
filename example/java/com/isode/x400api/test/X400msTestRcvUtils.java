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
 * Support methods receiving a message from the P7 messages store.
 */


package com.isode.x400api.test;

import com.isode.x400api.*;
import com.isode.x400api.X400ms.*;
import com.isode.x400api.test.config;
import java.io.IOException;
import java.io.FileOutputStream;
import java.io.File;

public class X400msTestRcvUtils
{

    public static int get_ms_redihist(
        MSMessage msmessage_obj,
        Recip recip_obj,
        int entry,
        RediHist redihist_obj
    )
    {
        int status, len;
        // Initialise object for returning string values
	StringBuffer ret_value = new StringBuffer();
        int ret_int = 0;

        if (msmessage_obj == null) {
            status = com.isode.x400api.X400.x400_redihistget(
                recip_obj, entry, redihist_obj);
            if (status != X400_att.X400_E_NOERROR) {
                System.out.println("no redirection history found \n" + " " + status);
                return status;
            }
        } else {
            status = com.isode.x400api.X400ms.x400_ms_redihistgetenv(msmessage_obj,
                                                                     entry,
                                                                     redihist_obj);
            if (status != X400_att.X400_E_NOERROR) {
                System.out.println("no redirection history found \n" + " " + status);
                return status;
            }
        }
	System.out.println("Redirection History found " + " " + status);
        

        status = com.isode.x400api.X400.x400_redihistgetstrparam(
            redihist_obj, X400_att.X400_S_REDIRECTION_TIME,ret_value);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for redirection history redirection time " + status);
	} else {
	    System.out.println("redirection history redirection time: " + ret_value.toString());
	}
        
        status = com.isode.x400api.X400.x400_redihistgetstrparam(
            redihist_obj, X400_att.X400_S_OR_ADDRESS, ret_value);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for redirection history redirection address " + status);
	} else {
	    System.out.println("redirection history redirection address: " + ret_value.toString());
	}

        status = com.isode.x400api.X400.x400_redihistgetstrparam(
            redihist_obj, X400_att.X400_S_DIRECTORY_NAME, ret_value);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for redirection history redirection DN " + status);
	} else {
	    System.out.println("redirection history redirection DN: " + ret_value.toString());
	}

        status = com.isode.x400api.X400.x400_redihistgetintparam(
            redihist_obj, X400_att.X400_N_REDIRECTION_REASON);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no int value for redirection history redirection reason" + status);
	} else {
            ret_int = redihist_obj.GetIntValue();
	    System.out.println("Got redirection history redirection reason: " + ret_int);
        }
              
        return X400_att.X400_E_NOERROR;
    }

    
    
    public static int get_ms_traceinfo(
        MSMessage msmessage_obj,
        int entry,
        Traceinfo traceinfo_obj,
        int type
    )
    {
        int status, len;
        // Initialise object for returning string values
	StringBuffer ret_value = new StringBuffer();
        int ret_int = 0;
        
        status = com.isode.x400api.X400ms.x400_ms_traceinfoget(
	    msmessage_obj,entry, traceinfo_obj,type);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no trace info found \n" + " " + status);
	    return status;
	} 
	System.out.println("Traceinfo found " + " " + status);
        
        status = com.isode.x400api.X400.x400_traceinfogetstrparam(
            traceinfo_obj,
            X400_att.X400_S_GLOBAL_DOMAIN_ID, 
            ret_value);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for trace info GDI " + status);
	} else {
	    System.out.println("Trace info GDI: " + ret_value.toString());
	}

        
        status = com.isode.x400api.X400.x400_traceinfogetstrparam(
            traceinfo_obj,
            X400_att.X400_S_DSI_ARRIVAL_TIME, 
            ret_value);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for trace info arrival time " + status);
	} else {
	    System.out.println("Trace info arrival time: " + ret_value.toString());
	}


        status = com.isode.x400api.X400.x400_traceinfogetstrparam(
            traceinfo_obj,
            X400_att.X400_S_DSI_ATTEMPTED_DOMAIN, 
            ret_value);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for trace info attempted domain " + status);
	} else {
	    System.out.println("Trace info attempted domain: " + ret_value.toString());
	}
        
        status = com.isode.x400api.X400.x400_traceinfogetstrparam(
            traceinfo_obj,
            X400_att.X400_S_DSI_AA_DEF_TIME, 
            ret_value);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for trace info deferred delivery time " + status);
	} else {
	    System.out.println("Trace info deferred delivery time : " + ret_value.toString());
	}
        
        status = com.isode.x400api.X400.x400_traceinfogetstrparam(
            traceinfo_obj,
            X400_att.X400_S_DSI_AA_CEIT, 
            ret_value);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for trace info CEIT" + status);
	} else {
	    System.out.println("Trace info CEIT : " + ret_value.toString());
	}

        status = com.isode.x400api.X400.x400_traceinfogetintparam(
            traceinfo_obj,
            X400_att.X400_N_DSI_ROUTING_ACTION);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no int value for routing action" + status);
	} else {
            ret_int = traceinfo_obj.GetIntValue();
	    System.out.println("Trace info routing action: " + ret_int);
	}
        
        status = com.isode.x400api.X400.x400_traceinfogetintparam(
            traceinfo_obj,
            X400_att.X400_N_DSI_AA_REDIRECTED);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no int value for redirection" + status);
	} else {
            ret_int = traceinfo_obj.GetIntValue();
	    System.out.println("Trace info redirected: " + ret_int);
	}
        
        status = com.isode.x400api.X400.x400_traceinfogetintparam(
            traceinfo_obj,
            X400_att.X400_N_DSI_AA_DLOPERATION);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no int value for dloperation" + status);
	} else {
            ret_int = traceinfo_obj.GetIntValue();
	    System.out.println("Trace info dloperation: " + ret_int);
	}
        
        return X400_att.X400_E_NOERROR;
    }

     public static int get_ms_internaltraceinfo(
        MSMessage msmessage_obj,
        int entry,
        InternalTraceinfo traceinfo_obj
    )
    {
        int status, len;
        // Initialise object for returning string values
	StringBuffer ret_value = new StringBuffer();
        int ret_int = 0;

        status = com.isode.x400api.X400ms.x400_ms_internaltraceinfoget(
	    msmessage_obj,entry, traceinfo_obj);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no trace info found \n" + " " + status);
	    return status;
	} 
	System.out.println("Internal Traceinfo found " + " " + status);
        

        status = com.isode.x400api.X400.x400_internaltraceinfogetstrparam(
            traceinfo_obj,
            X400_att.X400_S_GLOBAL_DOMAIN_ID, 
            ret_value);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for internal trace info GDI " + status);
	} else {
	    System.out.println("Internal Trace info GDI: " + ret_value.toString());
	}
        
        status = com.isode.x400api.X400.x400_internaltraceinfogetstrparam(
            traceinfo_obj, X400_att.X400_S_MTA_NAME, ret_value);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for internal trace info MTA Name " + status);
	} else {
	    System.out.println("Internal Trace info MTA Name: " + ret_value.toString());
	}

        /* Now the MTA supplied information */

        /* MTA SI Arrival time */
        status = com.isode.x400api.X400.x400_internaltraceinfogetstrparam(
            traceinfo_obj, X400_att.X400_S_MTA_SI_TIME, ret_value);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for internal trace info MTA SI Time " + status);
	} else {
	    System.out.println("Internal Trace info MTA SI Time: " + ret_value.toString());
	}

        /* MTA SI routing action can either be:
         * X400_MTA_SI_ROUTING_ACTION_RELAYED or
         * X400_MTA_SI_ROUTING_ACTION_REROUTED
         * */
        status = com.isode.x400api.X400.x400_internaltraceinfogetintparam(
            traceinfo_obj,
            X400_att.X400_N_MTA_SI_ROUTING_ACTION);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no int value for routing action" + status);
	} else {
            ret_int = traceinfo_obj.GetIntValue();
	    System.out.println("Got trace info routing action: ");
	
            if (ret_int == X400_att.X400_MTA_SI_ROUTING_ACTION_RELAYED) {
                System.out.println ("SI routing action "+ entry +"  :relayed");
            } else {
                System.out.println ("SI routing action "+ entry +"  :rerouted");
            }

        }

        /* Now a choice between attempted "mtaname" and "GDI" */

          status = com.isode.x400api.X400.x400_internaltraceinfogetintparam(
            traceinfo_obj,
            X400_att.X400_N_MTA_SI_ATTEMPTED_ACTION);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no int value for attempted action" + status);
	} else {
            ret_int = traceinfo_obj.GetIntValue();
	    System.out.println("Internal Trace info attempted action: ");
        
            if (ret_int == X400_att.X400_MTA_SI_RA_MTA) {
                status = com.isode.x400api.X400.x400_internaltraceinfogetstrparam(
                    traceinfo_obj, X400_att.X400_S_MTA_SI_ATTEMPTED_MTA, ret_value);
                if (status != X400_att.X400_E_NOERROR) {
                    System.out.println("no string value for internal trace info Attempted MTA Name " + status);
                } else {
                    System.out.println("Internal Trace info Attempted MTA Name: " + ret_value.toString());
                }
                
            } else {
                status = com.isode.x400api.X400.x400_internaltraceinfogetstrparam(
                    traceinfo_obj, X400_att.X400_S_MTA_SI_ATTEMPTED_DOMAIN, ret_value);
                if (status != X400_att.X400_E_NOERROR) {
                    System.out.println("no string value for internal trace info Attempted Domain " + status);
                } else {
                    System.out.println("Internal Trace info Attempted Domain : " + ret_value.toString());
                }
                
            }
        }
        /* followed by additional actions */

        /* Additional action: deferred time */
        status = com.isode.x400api.X400.x400_internaltraceinfogetstrparam(
            traceinfo_obj, X400_att.X400_S_MTA_SI_DEFERRED_TIME, ret_value);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for internal trace info X400_S_MTA_SI_DEFERRED_TIME " + status);
	} else {
	    System.out.println("Internal Trace info additional action deferred time: " + ret_value.toString());
	}

        /* Additional action: CEIT */
        
        status = com.isode.x400api.X400.x400_internaltraceinfogetstrparam(
            traceinfo_obj, X400_att.X400_S_MTA_SI_CEIT, ret_value);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for internal trace info X400_S_MTA_SI_CEIT " + status);
	} else {
	    System.out.println("Internal Trace info additional action CEIT: " + ret_value.toString());
	}

        /* Additional action: other actions */
        
        status = com.isode.x400api.X400.x400_internaltraceinfogetintparam(
            traceinfo_obj,
            X400_att.X400_N_MTA_SI_OTHER_ACTIONS);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no int value for other action" + status);
	} else {
            ret_int = traceinfo_obj.GetIntValue();
	    System.out.println("Trace info other action: " + ret_int);
	}
        
        if ((ret_int & X400_att.X400_MTA_SI_OTHER_ACTION_REDIRECTED) > 0) {
            System.out.println ("SI Other Action "+ entry +"  : redirected");
        }
        
        if ((ret_int & X400_att.X400_MTA_SI_OTHER_ACTION_DLOPERATION) > 0) {
            System.out.println ("SI Other Action "+ entry +" : DL operation");
        }
      
        return X400_att.X400_E_NOERROR;
    }

    
    private static int get_ea_recips(MSMessage msmessage_obj) {
	int status;
	int len = -1, maxlen = -1;
	int int_value; // int to contain value returned from API 
        int recip_num;
        int paramtype;
        
	// Initialise object for returning string values
	StringBuffer ret_value = new StringBuffer();
        
	// instantiate a recip object, and retrieve a recip
	// putting it into an API object 
	Recip recip_obj = new Recip();
	for ( recip_num = 1; ; recip_num++ ) {
	
	    // get the recip
	    status = com.isode.x400api.X400ms.x400_ms_recipget(
                msmessage_obj, 
                X400_att.X400_EXEMPTED_ADDRESS,
                recip_num,
                recip_obj);
	    if (status == X400_att.X400_E_NO_RECIP) {
		System.out.println("no more recips ...");
		break;
	    }
	    else if (status != X400_att.X400_E_NOERROR) {
		System.out.println("x400_ms_recipget failed " + status);
		break;
	    }

            paramtype = X400_att.X400_S_IOB_OR_ADDRESS;
	    status = com.isode.x400api.X400ms.x400_ms_recipgetstrparam(
		recip_obj, paramtype, ret_value);
	    if (status != X400_att.X400_E_NOERROR) {
		System.out.println("no string value for oraddress(" + paramtype + ") failed " + status);
	    } else {
		len =  ret_value.length();
		System.out.println("oraddress" + "(" + len + ")" + ret_value.toString());
	    }

            paramtype = X400_att.X400_S_IOB_DN_ADDRESS;
	    status = com.isode.x400api.X400ms.x400_ms_recipgetstrparam(
                recip_obj, paramtype, ret_value);
	    if (status != X400_att.X400_E_NOERROR) {
		System.out.println("no string value for oraddress(" + paramtype + ") failed " + status);
	    } else {
		len =  ret_value.length();
		System.out.println("dnaddress" + "(" + len + ")" + ret_value.toString());
	    }
            
            paramtype = X400_att.X400_S_IOB_FREE_FORM_NAME;
	    status = com.isode.x400api.X400ms.x400_ms_recipgetstrparam(
		recip_obj, paramtype, ret_value);
	    if (status != X400_att.X400_E_NOERROR) {
		System.out.println("no string value for oraddress(" + paramtype + ") failed " + status);
	    } else {
		len =  ret_value.length();
		System.out.println("free form name" + "(" + len + ")" + ret_value.toString());
	    }
            
            paramtype = X400_att.X400_S_IOB_TEL;
	    status = com.isode.x400api.X400ms.x400_ms_recipgetstrparam(
		recip_obj, paramtype, ret_value);
	    if (status != X400_att.X400_E_NOERROR) {
		System.out.println("no string value for oraddress(" + paramtype + ") failed " + status);
	    } else {
		len =  ret_value.length();
		System.out.println("free form name" + "(" + len + ")" + ret_value.toString());
	    }
            
            
        }
        return X400_att.X400_E_NOERROR;
     }
    
    
    
    public static int get_ms_distfield(
        MSMessage msmessage_obj,
        int entry,
        DistField df
    ) {
        int status;
        StringBuffer ret_value = new StringBuffer();
        status = com.isode.x400api.X400ms.x400_ms_distfieldget(msmessage_obj,entry,df);
       	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("get_distfield failed " + status);
	    return status;
	}
        
        status = com.isode.x400api.X400.x400_distfieldgetstrparam(df,X400_att.X400_S_DIST_CODES_EXT_OID, ret_value);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for distfieldgetstrparam: " + status);
	} else {
	    System.out.println("Dist Field OID: " + ret_value.toString());
	}
        
        byte[] binarydata = new byte[config.maxlen];
        status = com.isode.x400api.X400.x400_distfieldgetbyteparam(df,X400_att.X400_S_DIST_CODES_EXT_VALUE, binarydata);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for distfieldgetstrparam: " + status);
	} else {
            int len = df.GetAttLen();
	    System.out.println("Fetched dist field value:Binary data(" + len + ")" );
	}
        
        
        return X400_att.X400_E_NOERROR;
    }
    
    public static int get_ms_pss(
        MSMessage msmessage_obj,
        int entry,
        PSS dc,
        int type
    ) {
        int status;
        StringBuffer ret_value = new StringBuffer();
        status = com.isode.x400api.X400ms.x400_ms_pssget(msmessage_obj,entry,dc,type);
       	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("get_pss failed " + status);
	    return status;
	}
        
        status = com.isode.x400api.X400.x400_pssgetstrparam(dc, ret_value);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for PSS: " + status);
	} else {
	    System.out.println("PSS: " + ret_value.toString());
	}
        return X400_att.X400_E_NOERROR;
    }

    public static int get_ms_otherrecip(
        MSMessage msmessage_obj,
        int entry,
        OtherRecip otherrecip
    ) {
        int status;
        StringBuffer ret_value = new StringBuffer();
        int ret_int;
        status = com.isode.x400api.X400ms.x400_ms_otherrecipget(msmessage_obj,entry,otherrecip);
       	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("get_otherrecip failed " + status);
	    return status;
	}
        
        status = com.isode.x400api.X400.x400_otherrecipgetstrparam(otherrecip, ret_value);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for OtherRecip: " + status);
	} else {
	    System.out.println("OtherRecip OR address: " + ret_value.toString());
	}

        
        status = com.isode.x400api.X400.x400_otherrecipgetintparam(otherrecip);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no int value for otherrecip" + status);
	} else {
            ret_int = otherrecip.GetIntValue();
	    System.out.println("Got otherrecip int: " + ret_int);
        }
        return X400_att.X400_E_NOERROR;
      }
    
    public static int get_ms_ali(
        MSMessage msmessage_obj,
        int entry,
        ALI ali
    ) {
        int status;
        StringBuffer ret_value = new StringBuffer();
        int ret_int;
        status = com.isode.x400api.X400ms.x400_ms_aliget(msmessage_obj,entry,ali);
       	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("get_ali failed " + status);
	    return status;
	}
        
        status = com.isode.x400api.X400.x400_aligetstrparam(ali,X400_att.X400_S_IOB_OR_ADDRESS, ret_value);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for ALI: " + status);
	} else {
	    System.out.println("ALI OR address: " + ret_value.toString());
	}

        status = com.isode.x400api.X400.x400_aligetstrparam(ali,X400_att.X400_S_IOB_DN_ADDRESS, ret_value);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for ALI: " + status);
	} else {
	    System.out.println("ALI DN address: " + ret_value.toString());
	}
        
        status = com.isode.x400api.X400.x400_aligetstrparam(ali,X400_att.X400_S_IOB_FREE_FORM_NAME, ret_value);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for ALI: " + status);
	} else {
	    System.out.println("ALI free form  address: " + ret_value.toString());
	}

        status = com.isode.x400api.X400.x400_aligetstrparam(ali,X400_att.X400_S_IOB_TEL, ret_value);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for ALI: " + status);
	} else {
	    System.out.println("ALI Tel  address: " + ret_value.toString());
	}

        status = com.isode.x400api.X400.x400_aligetintparam(
            ali,
            X400_att.X400_N_ALI_TYPE);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no int value for ALI" + status);
	} else {
            ret_int = ali.GetIntValue();
	    System.out.println("Got ALI type: " + ret_int);
        }

        status = com.isode.x400api.X400.x400_aligetintparam(
            ali,
            X400_att.X400_N_ALI_NOTIFICTAION_REQUEST);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no int value for ALI" + status);
	} else {
            ret_int = ali.GetIntValue();
	    System.out.println("Got ALI notification request: " + ret_int);
        }

        status = com.isode.x400api.X400.x400_aligetintparam(
            ali,
            X400_att.X400_N_ALI_REPLY_REQUEST);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no int value for ALI" + status);
	} else {
            ret_int = ali.GetIntValue();
	    System.out.println("Got ALI reply request: " + ret_int);
        }
        

        
        return X400_att.X400_E_NOERROR;
    }
    
    public static int get_acp127respali(
        ACP127Resp resp_obj,
        int entry,
        ALI ali
    ) {
        int status;
        StringBuffer ret_value = new StringBuffer();
        int ret_int;
        status = com.isode.x400api.X400.x400_acp127respaliget(resp_obj,entry,ali);
       	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("get_ali failed " + status);
	    return status;
	}
        
        status = com.isode.x400api.X400.x400_aligetstrparam(ali,X400_att.X400_S_IOB_OR_ADDRESS, ret_value);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for ALI: " + status);
	} else {
	    System.out.println("ALI OR address: " + ret_value.toString());
	}

        status = com.isode.x400api.X400.x400_aligetstrparam(ali,X400_att.X400_S_IOB_DN_ADDRESS, ret_value);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for ALI: " + status);
	} else {
	    System.out.println("ALI DN address: " + ret_value.toString());
	}
        
        status = com.isode.x400api.X400.x400_aligetstrparam(ali,X400_att.X400_S_IOB_FREE_FORM_NAME, ret_value);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for ALI: " + status);
	} else {
	    System.out.println("ALI free form  address: " + ret_value.toString());
	}

        status = com.isode.x400api.X400.x400_aligetstrparam(ali,X400_att.X400_S_IOB_TEL, ret_value);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for ALI: " + status);
	} else {
	    System.out.println("ALI Tel  address: " + ret_value.toString());
	}

        status = com.isode.x400api.X400.x400_aligetintparam(
            ali,
            X400_att.X400_N_ALI_TYPE);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no int value for ALI" + status);
	} else {
            ret_int = ali.GetIntValue();
	    System.out.println("Got ALI type: " + ret_int);
        }

        status = com.isode.x400api.X400.x400_aligetintparam(
            ali,
            X400_att.X400_N_ALI_NOTIFICTAION_REQUEST);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no int value for ALI" + status);
	} else {
            ret_int = ali.GetIntValue();
	    System.out.println("Got ALI notification request: " + ret_int);
        }

        status = com.isode.x400api.X400.x400_aligetintparam(
            ali,
            X400_att.X400_N_ALI_REPLY_REQUEST);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no int value for ALI" + status);
	} else {
            ret_int = ali.GetIntValue();
	    System.out.println("Got ALI reply request: " + ret_int);
        }
        

        
        return X400_att.X400_E_NOERROR;
    }
    
   

    
    static public int get_p772 (MSMessage msmessage_obj)
    {
        int status;
        int paramtype;
        int int_value;
        int len;
        StringBuffer ret_value = new StringBuffer();
        
        System.out.println("Fetching P772");

        /* STANAG 4406 A1.1 Exempted Address*/
        System.out.println("Fetching exempted addresses");
        get_ea_recips(msmessage_obj);

        /* STANAG 4406 A1.3	Distribution codes */
        System.out.println("Fetching distribution codes");
        PSS dc_obj = new PSS();
        int entry;
        entry = 1;
        status = X400_att.X400_E_NOERROR;
        while(status == X400_att.X400_E_NOERROR)
	    {
		status = get_ms_pss(
                    msmessage_obj,
                    entry,
                    dc_obj,
                    X400_att.X400_S_DIST_CODES_SIC
                );
		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("get_ms_dist_codes failed " + status);
		    break;
		}
		entry++;
	    }
        /* Now get the dist fields */
        DistField df_obj = new DistField();
        entry = 1;
        status = X400_att.X400_E_NOERROR;
        while(status == X400_att.X400_E_NOERROR)
	    {
		status = get_ms_distfield(
                    msmessage_obj,
                    entry,
                    df_obj
                );
		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("get_ms_distfield failed " + status);
		    break;
		}
		entry++;
	    }

        
        
        
        /* STANAG 4406 A1.4	Handling instructions */
        System.out.println("Fetching handling instructions");
        PSS hi_obj = new PSS();
        entry = 1;
        status = X400_att.X400_E_NOERROR;
        while(status == X400_att.X400_E_NOERROR)
	    {
		status = get_ms_pss(
                    msmessage_obj,
                    entry,
                    hi_obj,
                    X400_att.X400_S_HANDLING_INSTRUCTIONS
                );
		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("get_ms_dist_codes failed " + status);
		    break;
		}
		entry++;
	    }
        
        /* STANAG 4406 A1.5	Message instructions */
        System.out.println("Fetching message instructions");
        PSS mi_obj = new PSS();
        entry = 1;
        status = X400_att.X400_E_NOERROR;
        while(status == X400_att.X400_E_NOERROR)
	    {
		status = get_ms_pss(
                    msmessage_obj,
                    entry,
                    mi_obj,
                    X400_att.X400_S_MESSAGE_INSTRUCTIONS
                );
		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("get_ms_pss failed " + status);
		    break;
		}
		entry++;
	    }

        
        
        /* STANAG 4406 A1.6	Codress message */
	paramtype = X400_att.X400_N_EXT_CODRESS;
    	status = com.isode.x400api.X400ms.x400_ms_msggetintparam(
	    msmessage_obj, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetintparam failed " + status);
	} else {
	    int_value = msmessage_obj.GetIntValue();
	    System.out.println("Coderess " + int_value);
	}

        /*STANAG 4406 A1.7	Originator reference*/
        paramtype = X400_att.X400_S_ORIG_REF;
    	status = com.isode.x400api.X400ms.x400_ms_msggetstrparam(
	    msmessage_obj, paramtype, ret_value);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no string value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetstrparam failed(" + paramtype + 
	    	") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Originator ref (" + len + ") " +  ret_value.toString());
	}

        
        /* STANAG 4406 A1.8	Primary precedence */
	paramtype = X400_att.X400_N_EXT_PRIM_PREC;
    	status = com.isode.x400api.X400ms.x400_ms_msggetintparam(
	    msmessage_obj, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetintparam failed " + status);
	} else {
	    int_value = msmessage_obj.GetIntValue();
	    System.out.println("Primary precedence " + int_value);
	}
        

        /* STANAG 4406 A1.9    Copy precedence */
	paramtype = X400_att.X400_N_EXT_COPY_PREC;
    	status = com.isode.x400api.X400ms.x400_ms_msggetintparam(
	    msmessage_obj, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetintparam failed " + status);
	} else {
	    int_value = msmessage_obj.GetIntValue();
	    System.out.println("Copy precedence " + int_value);
	}
        
        /* STANAG 4406 A1.10	Message type */
	paramtype = X400_att.X400_N_EXT_MSG_TYPE;
    	status = com.isode.x400api.X400ms.x400_ms_msggetintparam(
	    msmessage_obj, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetintparam failed " + status);
	} else {
	    int_value = msmessage_obj.GetIntValue();
	    System.out.println("Message type " + int_value);
	}
        
        
        
        /*STANAG 4406 A1.11	Address list indicator*/
        System.out.println("Address list indicator");
        ALI ali = new ALI();
        entry = 1;
        status = X400_att.X400_E_NOERROR;
        while(status == X400_att.X400_E_NOERROR)
	    {
		status = get_ms_ali(
                    msmessage_obj,
                    entry,
                    ali
                );
		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("get_ms_ali failed " + status);
		    break;
		}
		entry++;
	    }



        
        /*STANAG 4406 A1.12	Other Recipient*/
        System.out.println("Other Recipients");
        OtherRecip otherrecip = new OtherRecip();
        entry = 1;
        status = X400_att.X400_E_NOERROR;
        while(status == X400_att.X400_E_NOERROR)
	    {
		status = get_ms_otherrecip(
                    msmessage_obj,
                    entry,
                    otherrecip
                );
		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("get_ms_otherrecip failed " + status);
		    break;
		}
		entry++;
	    }

        
        /*STANAG 4406 A1.2	Extended authorisation information */
	paramtype = X400_att.X400_S_EXT_AUTH_INFO;
    	status = com.isode.x400api.X400ms.x400_ms_msggetstrparam(
	    msmessage_obj, paramtype, ret_value);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no string value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetstrparam failed(" + paramtype + 
	    	") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Ext auth info (" + len + ") " +  ret_value.toString());
	}
        
        /* STANAG 4406 A1.14	ACP127 message identifier */
	paramtype = X400_att.X400_S_ACP127_MSG_ID;
    	status = com.isode.x400api.X400ms.x400_ms_msggetstrparam(
	    msmessage_obj, paramtype, ret_value);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no string value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetstrparam failed(" + paramtype + 
	    	") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("ACP127 MSG ID (" + len + ") " +  ret_value.toString());
	}
        

        /* STANAG 4406 A1.15	Originator PLAD */
	paramtype = X400_att.X400_S_ORIG_PLAD;
    	status = com.isode.x400api.X400ms.x400_ms_msggetstrparam(
	    msmessage_obj, paramtype, ret_value);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no string value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetstrparam failed(" + paramtype + 
	    	") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("ORIG PLAD (" + len + ") " +  ret_value.toString());
	}

        
        
        
        /* Now get binary blobs */
        
        byte[] binarydata = new byte[config.maxlen];
        
        /* STANAG 4406 A1.13	Pilot forwarding information */
        paramtype = X400_att.X400_S_PILOT_FWD_INFO;
	status = com.isode.x400api.X400ms.x400_ms_msggetbyteparam(
	    msmessage_obj, paramtype, binarydata);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("Failed to fetch pilot fwd info " + status);

            if (status == X400_att.X400_E_NOSPACE) {
                System.out.println("Attempting again with a bigger buffer");
                binarydata = new byte [msmessage_obj.GetAttLen()];
                status = com.isode.x400api.X400ms.x400_ms_msggetbyteparam(
                    msmessage_obj, paramtype, binarydata);
            }
            
	}
        
        if (status == X400_att.X400_E_NOERROR)   {
	    len = msmessage_obj.GetAttLen();
	    System.out.println("pilot fwd info: Binary data (" + len + ")" );
            write_bin_file("pilot.ber",binarydata,len);
	}

        /* STANAG 4406 A1.16	Security Information Labels */
        paramtype = X400_att.X400_S_INFO_SEC_LABEL;
	status = com.isode.x400api.X400ms.x400_ms_msggetbyteparam(
	    msmessage_obj, paramtype, binarydata);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("Failed to fetch security info labels " + status);
            if (status == X400_att.X400_E_NOSPACE) {
                System.out.println("Attempting again with a bigger buffer");
                binarydata = new byte [msmessage_obj.GetAttLen()];
                status = com.isode.x400api.X400ms.x400_ms_msggetbyteparam(
                    msmessage_obj, paramtype, binarydata);
            }
            
	}
        
        if (status == X400_att.X400_E_NOERROR)   {
	    len = msmessage_obj.GetAttLen();
	    System.out.println("security info labels: Binary data (" + len + ")" );
            write_bin_file("sec_info_label.ber",binarydata,len);
	}

        return X400_att.X400_E_NOERROR;
    }
    
    /* Cheap noddy way of writing out sample binary file */
    static  private void write_bin_file (
         String filename,
         byte[] bytes,
         int num_of_bytes
    )
    {
     
        try {
            File fd = new File(config.outdir, filename);
            FileOutputStream fos = new FileOutputStream(fd);
            
            fos.write(bytes,0,num_of_bytes);
            fos.close();
            System.out.println("Finished write_bin_file: ");
            return;
           
            
        } catch (IOException obj) {
            System.out.println("Failed to write bin file: "+obj.getMessage());
        }
        System.out.println("Finished write_bin_file: " + filename);
        return;
    }
    
    /** 
     * Open a P3/P7 session,
     * Retrieve a message either from the Message Store
     * or the P3 server directly,
     * Close the session
     */
    public static void rcv_msg(String[] args)
    {
	/* change this depending on whether P3 or P7 reqiured */
        int type = 0;	// 0 = P7; 1 = P3 
	int status;
	int seqn = 0;
	int len = -1;
	int paramtype;
	int recip_type = 1;
	int recip_num = 1;
	int i = 0;
	
	// instantiate a message object, and make it an API object 
	// by opening an API session 
	Session session_obj = new Session();

	// Uncommenting the call below will prevent the x400_ms_open
	// call from performing a Summarize operation after a successful
	// bind.
	// session_obj.SetSummarizeOnBind(false);

	// Open the session using the instance field values
	if (type == 0) {
	    /* P7 */
	    status = com.isode.x400api.X400ms.x400_ms_open(type,
	    config.p7_bind_oraddr, config.p7_bind_dn,
		config.p7_credentials, config.p7_pa, session_obj);
	    if (status != X400_att.X400_E_NOERROR) {
		System.out.println("x400_ms_open failed " + status);
		return;
	    }
	    status = do_list_op(session_obj);
	    if (status != X400_att.X400_E_NOERROR) {
		System.out.println("list op failed " + status);
		return;
	    }

	} else {
	    /* P3 */
	    status = com.isode.x400api.X400ms.x400_ms_open(type, 
		config.p3oraddr, config.p3dn, config.p3credentials, 
		config.p3pa, session_obj);
	    if (status != X400_att.X400_E_NOERROR) {
		System.out.println("x400_ms_open failed " + status);
		return;
	    }

	    // You MUST do a wait first, or things go wrong
	    System.out.println("performing an MsWait() for 60 sec  ...");
	    status = com.isode.x400api.X400ms.x400_ms_wait(session_obj, 60);

	}
	System.out.println("Opened MS session successfully, " + session_obj.GetNumMsgs() + " messages waiting");

	// turn on trace level logging
    	status = com.isode.x400api.X400ms.x400_ms_setstrdefault(session_obj, 
	    X400_att.X400_S_LOG_CONFIGURATION_FILE, "x400api.xml", -1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_setstrdefault failed " + status);
	    return;
	}

	// turn on ability to verify signed messages
	// by configuring a security env
	if (config.verify_signed_message) {
	    //status = X400msTestSignUtils.setup_default_sec_env(session_obj, 
	    X400msTestSignUtils.setup_default_sec_env(session_obj, 
		config.sec_id, config.sec_id_dn, config.sec_pphr);
	    if (status != X400_att.X400_E_NOERROR) {
		return;
	    }
	    // cause MOAC verification errors and successes to be returned
	    status = com.isode.x400api.X400ms.x400_ms_setintdefault(session_obj, 
		X400_att.X400_B_RETURN_VERIFICATION_ERRORS, 1);
	    if (status != X400_att.X400_E_NOERROR) {
		System.out.println("x400_ms_setintdefault failed " + status);
		return;
	    }
	}
        
	
	// instantiate a message object, and retrieve a msg
	// putting it into an API object 
	MSMessage msmessage_obj = new MSMessage();
	// use seqn of 0 to retrieve the next msg 
    	status = com.isode.x400api.X400ms.x400_ms_msggetstart(session_obj, seqn, 
	    msmessage_obj);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetstart failed " + status);
	    // close the API session 
	    status = com.isode.x400api.X400ms.x400_ms_close(session_obj);
	    if (status != X400_att.X400_E_NOERROR) {
		System.out.println("x400_ms_close failed " + status);
		return;
	    }
	    System.out.println("Closed MS Session successfully\n");
	    return;
	}

	// check what we got back
	type = msmessage_obj.GetType();
	if ( type == X400_att.X400_MSG_MESSAGE ) {
	    int int_value;
	    
	    // got a message - is it an IPN ?
	    System.out.println
	    	("Retrieved MS Message successfully - displaying");
	    paramtype = X400_att.X400_N_IS_IPN;
	    status = com.isode.x400api.X400ms.x400_ms_msggetintparam(
		msmessage_obj, paramtype);
	    if (status != X400_att.X400_E_NOERROR) {
		System.out.println("failed to test whether IPN or message "
		    + status);
	    } else {
		int_value = msmessage_obj.GetIntValue();
		if (int_value != 0) {
		    // It's an IPN ...
		    System.out.println
			("Retrieved IPN successfully - displaying");
		    status = do_msg_env(msmessage_obj);
		    status = do_msg_headers(msmessage_obj);
		    /* the content can be retrieved as attachments or
		     * bodyparts. Use do_msg_content for the former */
		    status = X400msTestBodyPartRcvUtils.do_msg_content_as_bp(msmessage_obj);
		} else {
		    // It's not an IPN ...
		    System.out.println
			("Retrieved msg (not ipn) successfully - displaying");
		    status = do_msg_env(msmessage_obj);
		    status = do_msg_headers(msmessage_obj);
		    /* the content can be retrieved as attachments or
		     * bodyparts. Use do_msg_content for the former */
		    status = X400msTestBodyPartRcvUtils.do_msg_content_as_bp(msmessage_obj);
                    get_p772(msmessage_obj);
		    // Send back an IPN to the originator (if requested)
		    // -1 means positive
		    status = send_ipn(msmessage_obj, -1);
		}
	    }
	} else if ( type == X400_att.X400_MSG_REPORT) {
	    System.out.println
	    	("Retrieved MS Report successfully - displaying");
	    status = X400msTestRepRcvUtils.do_rep_env(msmessage_obj);
	    status = X400msTestRepRcvUtils.do_rep_content(msmessage_obj);
	    status = X400msTestRepRcvUtils.do_rep_retcontent(msmessage_obj);
	} else if ( type == X400_att.X400_MSG_PROBE) {
	    // Not handling a probe here 
	    System.out.println
	    	("Retrieved MS Report successfully - not displaying");
	} else {
	    // Unknown object 
	    System.out.println("Retrieved unknown message type " + type);
	}

    	status = com.isode.x400api.X400ms.x400_ms_msggetfinish(msmessage_obj,
							       0,0);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetfinish failed " + status);
	    // close the API session 
	    status = com.isode.x400api.X400ms.x400_ms_close(session_obj);
	    if (status != X400_att.X400_E_NOERROR) {
		System.out.println("x400_ms_close failed " + status);
		return;
	    }
	    System.out.println("Closed Session successfully\n");
	    return;
	}

	// delete the API msg object and from the Store
    	status = com.isode.x400api.X400ms.x400_ms_msgdel(msmessage_obj, 0);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgget failed " + status);
	    return;
	}

	// close the API session 
    	status = com.isode.x400api.X400ms.x400_ms_close(session_obj);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_close failed " + status);
	    return;
	}
	System.out.println("Closed Session successfully\n");
	return;
    }

    private static int do_list_op(Session session_obj)
    {
	int paramtype;
	int i = 0;
	int len = 0;
	int status;
	int close_status;

	// Instantiate a list result object and list messages in
	// the store
	MSListResult mslistresult_obj = new MSListResult();
	status = com.isode.x400api.X400ms.x400_ms_list(session_obj,
						       "040101000000Z",
						       mslistresult_obj);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_list failed " + status);
	    // close the API session 
	    close_status = com.isode.x400api.X400ms.x400_ms_close(session_obj);
	    if (close_status != X400_att.X400_E_NOERROR) {
		System.out.println("x400_ms_close failed " + status);
		return close_status;
	    }
	    System.out.println("Closed MS Session successfully\n");
	    return status;
	}

	System.out.println("Did MS List successfully");

	// Now examine the results
	for ( i = 1; ; i++ ) {
	    StringBuffer ret_value = new StringBuffer();

	    paramtype = X400_att.X400_N_MS_SEQUENCE_NUMBER;
	    status = com.isode.x400api.X400ms.x400_ms_listgetintparam(
				     mslistresult_obj, paramtype, i);

	    if (status == X400_att.X400_E_NO_MORE_RESULTS) {
		System.out.println("No more list results, i = " + i);
		// All done
		break;
	    }

	    if (status == X400_att.X400_E_NO_VALUE) {
		System.out.println("no int value for " + paramtype);
	    } else if (status != X400_att.X400_E_NOERROR) {
		System.out.println("x400_ms_listgetintparam failed " + status);
	    } else {
		int int_value;

		int_value = mslistresult_obj.GetIntValue();

		paramtype = X400_att.X400_S_SUBJECT;
		status = com.isode.x400api.X400ms.x400_ms_listgetstrparam(
			    mslistresult_obj, paramtype, i, ret_value);

		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("x400_ms_listgetstrparam failed(" + paramtype + 
				   ") result is " + status);
		} else {
		    len = ret_value.length();
		    System.out.println("Sequence number " + int_value + ", subject (" + len + ") " +  ret_value.toString());
		}
	    }
	}
	return X400_att.X400_E_NOERROR;
    }

    /**
     * Display a MS Message envelope (ie a message retrieved 
     * the message store).
     */
    private static int do_msg_env(MSMessage msmessage_obj)
    {
	int status;
	int paramtype;
	int len = -1, maxlen = -1;
	int recip_type = 1;
	int recip_num = 1;
	int entry = 1;
	
	String value; // string to contain value returned from API 
	int int_value; // int to contain value returned from API 

	System.out.println("Message Envelope:");
	System.out.println("----------------");

	// Initialise object to receive returned String value
	StringBuffer ret_value = new StringBuffer();

	// get orig address: string attribute from the message envelope
	paramtype = X400_att.X400_S_OR_ADDRESS;
    	status = com.isode.x400api.X400ms.x400_ms_msggetstrparam(
	    msmessage_obj, paramtype, ret_value);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no string value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println(
		"x400_ms_msggetstrparam failed X400_S_OR_ADDRESS(" 
		+ paramtype + ") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Originator address (" + len + ") " + ret_value.toString());
	}

	// get some string attributes from the message envelope
	paramtype = X400_att.X400_S_MESSAGE_IDENTIFIER;
    	status = com.isode.x400api.X400ms.x400_ms_msggetstrparam(
	    msmessage_obj, paramtype, ret_value);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no string value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetstrparam failed(" + paramtype + 
	    	") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Message ID(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message envelope
	paramtype = X400_att.X400_S_CONTENT_IDENTIFIER;
    	status = com.isode.x400api.X400ms.x400_ms_msggetstrparam(
	    msmessage_obj, paramtype, ret_value);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no string value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetstrparam failed(" + paramtype + 
	    	") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Content ID(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message envelope
	paramtype = X400_att.X400_S_ORIGINAL_ENCODED_INFORMATION_TYPES;
    	status = com.isode.x400api.X400ms.x400_ms_msggetstrparam(
	    msmessage_obj, paramtype, ret_value);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no string value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetstrparam failed(" + paramtype + 
	    	") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Orig EITs(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message envelope
	paramtype = X400_att.X400_S_MESSAGE_SUBMISSION_TIME;
    	status = com.isode.x400api.X400ms.x400_ms_msggetstrparam(
	    msmessage_obj, paramtype, ret_value);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no string value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetstrparam failed(" + paramtype + 
	    	") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Message submission time(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message envelope
	paramtype = X400_att.X400_S_MESSAGE_DELIVERY_TIME;
    	status = com.isode.x400api.X400ms.x400_ms_msggetstrparam(
	    msmessage_obj, paramtype, ret_value);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no string value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetstrparam failed(" + paramtype + 
	    	") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Message delivery time(" + len + ") " +  ret_value.toString());
	}

	// get the latest delivery time 
	// which won't work for a delivered msg
	paramtype = X400_att.X400_S_LATEST_DELIVERY_TIME;
    	status = com.isode.x400api.X400ms.x400_ms_msggetstrparam(
	    msmessage_obj, paramtype, ret_value);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no string value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetstrparam failed(" + paramtype + 
	    	") result is " + status + "(as expected)" );
	} else {
	    len = ret_value.length();
	    System.out.println("Latest delivery time(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message envelope
	paramtype = X400_att.X400_S_ORIGINATOR_RETURN_ADDRESS;
    	status = com.isode.x400api.X400ms.x400_ms_msggetstrparam(
	    msmessage_obj, paramtype, ret_value);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no string value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetstrparam failed(" + paramtype + 
	    	") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Orig return address(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message envelope
	paramtype = X400_att.X400_S_MESSAGE_DELIVERY_TIME;
    	status = com.isode.x400api.X400ms.x400_ms_msggetstrparam(
	    msmessage_obj, paramtype, ret_value);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no string value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetstrparam failed(" + paramtype + 
	    	") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Message delivery time(" + len + ") " +  ret_value.toString());
	}

	// get some integer attributes from the message
	paramtype = X400_att.X400_N_CONTENT_TYPE;
    	status = com.isode.x400api.X400ms.x400_ms_msggetintparam(
	    msmessage_obj, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetintparam failed " + status);
	} else {
	    int_value = msmessage_obj.GetIntValue();
	    System.out.println("Content type " + int_value);
	}

	paramtype = X400_att.X400_N_NUM_ATTACHMENTS;
    	status = com.isode.x400api.X400ms.x400_ms_msggetintparam(
	    msmessage_obj, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetintparam failed " + status);
	} else {
	    int_value = msmessage_obj.GetIntValue();
	    System.out.println("Num attachments " + int_value);
	}

	paramtype = X400_att.X400_N_PRIORITY;
    	status = com.isode.x400api.X400ms.x400_ms_msggetintparam(
	    msmessage_obj, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetintparam failed " + status);
	} else {
	    int_value = msmessage_obj.GetIntValue();
	    System.out.println("Priority " + int_value);
	}

	// we don't expect this to be present for delivery env
	paramtype = X400_att.X400_N_DISCLOSURE;
    	status = com.isode.x400api.X400ms.x400_ms_msggetintparam(
	    msmessage_obj, paramtype);
	if (status == X400_att.X400_E_BADPARAM) {
	    System.out.println("no int value for Disclosure of recips prohibited(" + paramtype 
	    + ") (as expected)");
	}
	else if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetintparam failed " + status);
	} else {
	    int_value = msmessage_obj.GetIntValue();
	    System.out.println("Disclosure of recips prohibited " + int_value);
	}

	paramtype = X400_att.X400_N_IMPLICIT_CONVERSION_PROHIBITED;
    	status = com.isode.x400api.X400ms.x400_ms_msggetintparam(
	    msmessage_obj, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetintparam failed " + status);
	} else {
	    int_value = msmessage_obj.GetIntValue();
	    System.out.println("Implicit conversion prohibited " + int_value);
	}

	paramtype = X400_att.X400_N_CONTENT_RETURN_REQUEST;
    	status = com.isode.x400api.X400ms.x400_ms_msggetintparam(
	    msmessage_obj, paramtype);
	if (status == X400_att.X400_E_BADPARAM) {
	    System.out.println("no int value for Content return request(" + paramtype 
	    + ") (as expected)");
	}
	else if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetintparam failed " + status);
	} else {
	    int_value = msmessage_obj.GetIntValue();
	    System.out.println("Content return request " + int_value);
	}

	paramtype = X400_att.X400_N_RECIPIENT_REASSIGNMENT_PROHIBITED;
    	status = com.isode.x400api.X400ms.x400_ms_msggetintparam(
	    msmessage_obj, paramtype);
	if (status == X400_att.X400_E_BADPARAM) {
	    System.out.println("no int value for Recipient reassignment prohibited(" + paramtype 
	    + ") (as expected)");
	}
	else if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetintparam failed " + status);
	} else {
	    int_value = msmessage_obj.GetIntValue();
	    System.out.println("Recipient reassignment prohibited " + int_value);
	}

	paramtype = X400_att.X400_N_DL_EXPANSION_PROHIBITED;
    	status = com.isode.x400api.X400ms.x400_ms_msggetintparam(
	    msmessage_obj, paramtype);
	if (status == X400_att.X400_E_BADPARAM) {
	    System.out.println("no int value for Distribution List expansion prohibited(" + paramtype 
	    + ") (as expected)");
	}
	else if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetintparam failed " + status);
	} else {
	    int_value = msmessage_obj.GetIntValue();
	    System.out.println("Distribution List expansion prohibited " + int_value);
	}

	paramtype = X400_att.X400_N_CONVERSION_WITH_LOSS_PROHIBITED;
    	status = com.isode.x400api.X400ms.x400_ms_msggetintparam(
	    msmessage_obj, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetintparam failed " + status);
	} else {
	    int_value = msmessage_obj.GetIntValue();
	    System.out.println("Conversion with loss prohibited " + int_value);
	}

	// Find out how many recips
	status = com.isode.x400api.X400ms.x400_ms_msgcountrecip(msmessage_obj, 
	    X400_att.X400_RECIP_ENVELOPE);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("can't count recips");
	} else {
	    System.out.println("Number of recipients is " 
		+ msmessage_obj.GetNumRecips() );
	}

	status = get_ms_recips(msmessage_obj, X400_att.X400_RECIP_ENVELOPE, 
	    "envelope: ");
	if ( status != X400_att.X400_E_NOERROR 
	    && status != X400_att.X400_E_NO_RECIP) {
	    System.out.println("get_ms_recips failed (env)" + status);
	    return status;
	}


        System.out.println("Get_ms_traceinfo ");
        Traceinfo traceinfo_obj = new Traceinfo();
        entry = 1;
        status = X400_att.X400_E_NOERROR;
        while(status == X400_att.X400_E_NOERROR)
	    {
		status = get_ms_traceinfo(msmessage_obj,
                                          entry,
                                          traceinfo_obj,
                                          X400_att.X400_TRACE_INFO);
		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("get_ms_traceinfo failed " + status);
		    break;
		}
		entry++;
	    }

        System.out.println("Get_ms_internal_traceinfo ");
        InternalTraceinfo internaltraceinfo_obj = new InternalTraceinfo();
        entry = 1;
        status = X400_att.X400_E_NOERROR;
        while(status == X400_att.X400_E_NOERROR)
	    {
		status = get_ms_internaltraceinfo(msmessage_obj,
                                                  entry,
                                                  internaltraceinfo_obj);
		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("get_ms_internaltraceinfo failed " + status);
		    break;
		}
		entry++;
	    }
        
        System.out.println("get_ms_dleh ");
	/* Distribution List Expansion History */
	DLExpHist dleh_obj = new DLExpHist();
	while(status == X400_att.X400_E_NOERROR)
	    {
		status = get_ms_dleh(msmessage_obj, entry, dleh_obj);
		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("get_ms_dleh failed " + status);
		    break;
		}
		entry++;
	    }
	return X400_att.X400_E_NOERROR;

    }

    /**
     * Display the MS Message headers (ie a message retrieved 
     * the message store using the msgetmsg() API call).
     */
    private static int do_msg_headers(MSMessage msmessage_obj)
    {
	int status;
	int paramtype;
	int len = -1, maxlen = -1;

	String value; // string to contain value returned from API 
	int int_value; // int to contain value returned from API 

	System.out.println("Message Content:");
	System.out.println("----------------");

	// Initialise object to receive returned String value
	StringBuffer ret_value = new StringBuffer();

	// get some string attributes from the message content
	paramtype = X400_att.X400_S_IPM_IDENTIFIER;
    	status = com.isode.x400api.X400ms.x400_ms_msggetstrparam(
	    msmessage_obj, paramtype, ret_value);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for IPM ID(" + paramtype + 
	    	") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("IPM IDentifier(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message content
	paramtype = X400_att.X400_S_SUBJECT;
    	status = com.isode.x400api.X400ms.x400_ms_msggetstrparam(
	    msmessage_obj, paramtype, ret_value);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for Subject(" + paramtype + 
	    	") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Subject(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message content
	paramtype = X400_att.X400_S_REPLIED_TO_IDENTIFIER;
    	status = com.isode.x400api.X400ms.x400_ms_msggetstrparam(
	    msmessage_obj, paramtype, ret_value);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for replied to ID(" 
		+ paramtype + ") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Replied-to-identifier(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message content
	paramtype = X400_att.X400_S_OBSOLETED_IPMS;
    	status = com.isode.x400api.X400ms.x400_ms_msggetstrparam(
	    msmessage_obj, paramtype, ret_value);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for obsoleted IPMs(" 
		+ paramtype + ") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Obsoleted IPMs(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message content
	paramtype = X400_att.X400_S_RELATED_IPMS;
    	status = com.isode.x400api.X400ms.x400_ms_msggetstrparam(
	    msmessage_obj, paramtype, ret_value);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for related IPMs(" 
		+ paramtype + "result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Related IPMs(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message content
	paramtype = X400_att.X400_S_EXPIRY_TIME;
    	status = com.isode.x400api.X400ms.x400_ms_msggetstrparam(
	    msmessage_obj, paramtype, ret_value);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for expiry time(" + paramtype + 
	    	") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Expiry Time(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message content
	paramtype = X400_att.X400_S_REPLY_TIME;
    	status = com.isode.x400api.X400ms.x400_ms_msggetstrparam(
	    msmessage_obj, paramtype, ret_value);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for reply time(" + paramtype + 
	    	") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Reply Time(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message content
	paramtype = X400_att.X400_S_AUTHORIZATION_TIME;
    	status = com.isode.x400api.X400ms.x400_ms_msggetstrparam(
	    msmessage_obj, paramtype, ret_value);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for auth time(" + paramtype + 
	    	") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Authorisation Time(" + len + ") " +  ret_value.toString());
	}

	// get some string attributes from the message content
	paramtype = X400_att.X400_S_ORIGINATORS_REFERENCE;
    	status = com.isode.x400api.X400ms.x400_ms_msggetstrparam(
	    msmessage_obj, paramtype, ret_value);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for orig ref(" + paramtype + 
	    	") result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("Originator's Reference(" + len + ") " +  ret_value.toString());
	}

	paramtype = X400_att.X400_N_IMPORTANCE;
    	status = com.isode.x400api.X400ms.x400_ms_msggetintparam(
	    msmessage_obj, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for importance " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetintparam failed " + status);
	} else {
	    int_value = msmessage_obj.GetIntValue();
	    System.out.println("Importance " + int_value);
	}

	paramtype = X400_att.X400_N_SENSITIVITY;
    	status = com.isode.x400api.X400ms.x400_ms_msggetintparam(
	    msmessage_obj, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for sensitivity" 
	    + paramtype + ")");
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetintparam failed " + status);
	} else {
	    int_value = msmessage_obj.GetIntValue();
	    System.out.println("Sensitivity " + int_value);
	}

	paramtype = X400_att.X400_N_AUTOFORWARDED;
    	status = com.isode.x400api.X400ms.x400_ms_msggetintparam(
	    msmessage_obj, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for autoforwarded (" 
	    + paramtype + ")");
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetintparam failed " + status);
	} else {
	    int_value = msmessage_obj.GetIntValue();
	    System.out.println("Autoforwarded " + int_value);
	}

	paramtype = X400_att.X400_N_NUM_ATTACHMENTS;
    	status = com.isode.x400api.X400ms.x400_ms_msggetintparam(
	    msmessage_obj, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for num atts " 
	    + paramtype + ")");
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetintparam failed " 
	    + paramtype + ")");
	} else {
	    int_value = msmessage_obj.GetIntValue();
	    System.out.println("number of attachments " + int_value);
	}

	status = get_ms_recips(msmessage_obj, X400_att.X400_ORIGINATOR, 
	    "originator: ");
	if ( status != X400_att.X400_E_NOERROR 
	    && status != X400_att.X400_E_NO_RECIP) {
	    System.out.println("get_ms_recips failed " + status);
	    return status;
	}
	status = get_ms_recips(msmessage_obj, X400_att.X400_RECIP_PRIMARY, 
	    "primary: ");
	if ( status != X400_att.X400_E_NOERROR 
	    && status != X400_att.X400_E_NO_RECIP) {
	    System.out.println("get_ms_recips failed " + status);
	    return status;
	}
	return status;
    }

    /**
     * Display the MS Message recipients (ie those of a message retrieved 
     * the message store.
     */
    private static int get_ms_recips(MSMessage msmessage_obj, int type, String logstr)  {
	int status;
	int paramtype;
	int recip_num = 1;
	int len = -1, maxlen = -1;
	int int_value; // int to contain value returned from API 

	// Initialise object for returning string values
	StringBuffer ret_value = new StringBuffer();

	// instantiate a recip object, and retrieve a recip
	// putting it into an API object 
	Recip recip_obj = new Recip();
	for ( recip_num = 1; ; recip_num++ ) {
	
	    // get the recip
	    status = com.isode.x400api.X400ms.x400_ms_recipget(msmessage_obj, 
		type, recip_num, recip_obj);
	    if (status == X400_att.X400_E_NO_RECIP) {
		System.out.println("no more recips ...");
		break;
	    }
	    else if (status != X400_att.X400_E_NOERROR) {
		System.out.println("x400_ms_recipget failed " + status);
		break;
	    }

	    System.out.println("-------------- Recipient " + recip_num + "--------------");
	    // display recip name
	    paramtype = X400_att.X400_S_OR_ADDRESS;
	    status = com.isode.x400api.X400ms.x400_ms_recipgetstrparam(
		recip_obj, paramtype, ret_value);
	    if (status != X400_att.X400_E_NOERROR) {
		System.out.println("no string value for oraddress(" + paramtype + ") failed " + status);
	    } else {
		len =  ret_value.length();
		System.out.println("oraddress" + "(" + len + ")" + ret_value.toString());
	    }

	    // Get envelope values 
	    if (type == X400_att.X400_RECIP_ENVELOPE) {
		// display recip properties
		paramtype = X400_att.X400_S_OR_ADDRESS;
		status = com.isode.x400api.X400ms.x400_ms_recipgetstrparam(
		    recip_obj, paramtype, ret_value);
		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("no string value for originator address(" + paramtype + ") failed " + status);
		} else {
		    len =  ret_value.length();
		    System.out.println(logstr + recip_num + "(" + len + ")" + ret_value.toString());
		}

		// display recip properties
		paramtype = X400_att.X400_N_RESPONSIBILITY;
		status = com.isode.x400api.X400ms.x400_ms_recipgetintparam(
		    recip_obj, paramtype);
		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("no int value for Responsibility(" + paramtype + ") failed " + status);
		} else {
		    int_value = recip_obj.GetIntValue();
		    System.out.println("Responsibility " + int_value);
		}

		// display recip properties
		paramtype = X400_att.X400_N_MTA_REPORT_REQUEST;
		status = com.isode.x400api.X400ms.x400_ms_recipgetintparam(
		    recip_obj, paramtype);
		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("no int value for MTA report req(" + paramtype + ") failed " + status);
		} else {
		    int_value = recip_obj.GetIntValue();
		    System.out.println("MTA report request " + int_value);
		}

		// display recip properties
		paramtype = X400_att.X400_N_REPORT_REQUEST;
		status = com.isode.x400api.X400ms.x400_ms_recipgetintparam(
		    recip_obj, paramtype);
		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("no int value for report req(" + paramtype + ") failed " + status);
		} else {
		    int_value = recip_obj.GetIntValue();
		    System.out.println("Originator report request " + int_value);
		}

                RediHist redihist_obj = new RediHist();
                int entry = 1;
                status = X400_att.X400_E_NOERROR;
                while(status == X400_att.X400_E_NOERROR)
                    {
                    status = get_ms_redihist(null,
                                             recip_obj,
                                             entry,
                                             redihist_obj);
                    if (status != X400_att.X400_E_NOERROR) {
                        System.out.println("get_ms_redihist failed " + status);
                        break;
                    }
                    entry++;
                    }
            
                
	    }

	    // Get content values 
	    if (type != X400_att.X400_RECIP_ENVELOPE) {
		// display recip properties
		paramtype = X400_att.X400_N_NOTIFICATION_REQUEST;
		status = com.isode.x400api.X400ms.x400_ms_recipgetintparam(
		    recip_obj, paramtype);
		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("no int value for notification req(" + paramtype + ") failed " + status);
		} else {
		    int_value = recip_obj.GetIntValue();
		    System.out.println("Responsibility " + int_value);
		}

		// display recip properties
		paramtype = X400_att.X400_N_REPLY_REQUESTED;
		status = com.isode.x400api.X400ms.x400_ms_recipgetintparam(
		    recip_obj, paramtype);
		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("no int value for reply requested(" + paramtype + ") failed " + status);
		} else {
		    int_value = recip_obj.GetIntValue();
		    System.out.println("Reply Request " + int_value);
		}

		// display recip properties
		paramtype = X400_att.X400_S_FREE_FORM_NAME;
		status = com.isode.x400api.X400ms.x400_ms_recipgetstrparam(
		    recip_obj, paramtype, ret_value);
		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("no string value for Free form name(" + paramtype + ") failed " + status);
		} else {
		    len =  ret_value.length();
		    System.out.println("Free form name" + "(" + len + ")" +  ret_value.toString());
		}

		// display recip properties
		paramtype = X400_att.X400_S_TELEPHONE_NUMBER;
		status = com.isode.x400api.X400ms.x400_ms_recipgetstrparam(
		    recip_obj, paramtype, ret_value);
		if (status != X400_att.X400_E_NOERROR) {
		    System.out.println("no string value for Telephone Number(" + paramtype + ") failed " + status);
		} else {
		    len =  ret_value.length();
		    System.out.println("Telephone Number" + "(" + len + ")" +  ret_value.toString());
		}


                if (config.mt_use_p772 == true) {
                    /* Get P772 per-recipient extensions */
                    paramtype = X400_att.X400_N_ACP127_NOTI_TYPE;
                    status = com.isode.x400api.X400ms.x400_ms_recipgetintparam(
                        recip_obj, paramtype);
                    if (status != X400_att.X400_E_NOERROR) {
                        System.out.println("no int value for Notification request(" + paramtype + ") failed " + status);
                    } else {
                        int_value = recip_obj.GetIntValue();
                        
                        if ((int_value & X400_att.X400_ACP127_NOTI_TYPE_NEG) != 0) {
                            System.out.println("P772 ACP127 Notification Request Type Negative\n");
                        }
                        if ((int_value & X400_att.X400_ACP127_NOTI_TYPE_POS) != 0) {
                            System.out.println("P772 ACP127 Notification Request Type Positive\n");
                        }
                        
                        if ((int_value & X400_att.X400_ACP127_NOTI_TYPE_TRANS) != 0) {
                            System.out.println("P772 ACP127 Notification Request Type Transfer\n");
                        }
                        
                    }
                    /* Get STANAG 4406 A3.1	ACP127 notification response */
                    System.out.println("Fetching ACP127 notification response");
                    ACP127Resp resp_obj = new ACP127Resp();
                    status = com.isode.x400api.X400.x400_acp127respget(recip_obj,
                                                                       resp_obj);
                    if (status != X400_att.X400_E_NOERROR) {
                        System.out.println("x400_acp127respget failed " + status);
                    } else {
                        status  = com.isode.x400api.X400.x400_acp127respgetintparam(resp_obj);
                        if (status != X400_att.X400_E_NOERROR) {
                            System.out.println("x400_acp127respgetintparam failed " + status);
                        } else {
                            int_value = recip_obj.GetIntValue();
                            
                            if ((int_value & X400_att.X400_ACP127_NOTI_TYPE_NEG) != 0) {
                                System.out.println("P772 ACP127 Notification response Negative\n");
                            }
                            if ((int_value & X400_att.X400_ACP127_NOTI_TYPE_POS) != 0) {
                                System.out.println("P772 ACP127 Notification response Positive\n");
                            }
                            
                            if ((int_value & X400_att.X400_ACP127_NOTI_TYPE_TRANS) != 0) {
                                System.out.println("P772 ACP127 Notification response Transfer\n");
                            }
                            
                        }
                        
                        status = com.isode.x400api.X400.x400_acp127respgetstrparam(
                            resp_obj, X400_att.X400_S_ACP127_NOTI_RESP_TIME, ret_value);
                        if (status != X400_att.X400_E_NOERROR) {
                            System.out.println("x400_acp127respgetstrparam failed(" + paramtype + 
                                               "): result is " + status);
                        } else {
                            System.out.println("P772 ACP127 Resp time " +  ret_value.toString());
                        }
                    
                        status = com.isode.x400api.X400.x400_acp127respgetstrparam(
                            resp_obj, X400_att.X400_S_ACP127_NOTI_RESP_RECIPIENT, ret_value);
                        if (status != X400_att.X400_E_NOERROR) {
                            System.out.println("x400_acp127respgetstrparam failed(" + paramtype + 
                                               "): result is " + status);
                        } else {
                            System.out.println("P772 ACP127 Resp Recipient" +  ret_value.toString());
                        }
                        
                        status = com.isode.x400api.X400.x400_acp127respgetstrparam(
                            resp_obj, X400_att.X400_S_ACP127_NOTI_RESP_SUPP_INFO, ret_value);
                        if (status != X400_att.X400_E_NOERROR) {
                            System.out.println("x400_acp127respgetstrparam failed(" + paramtype + 
                                               "): result is " + status);
                        } else {
                            System.out.println("P772 ACP127 Resp Supp info" +  ret_value.toString());
                        }
                        
                        /*Now deal with the ALIs*/
                        
                        System.out.println("Address list indicator");
                        ALI ali = new ALI();
                        int entry = 1;
                        status = X400_att.X400_E_NOERROR;
                        while(status == X400_att.X400_E_NOERROR)
                            {
                                status = X400msTestRcvUtils.get_acp127respali(
                                    resp_obj,
                                    entry,
                                    ali
                                );
                                if (status != X400_att.X400_E_NOERROR) {
                                    System.out.println("get_acp127respali failed " + status);
                                    break;
                                }
                                entry++;
                            }
                        
                    }
                    
                }
            
            }
        }
	return X400_att.X400_E_NOERROR;
    }
    
    /**
     * Display the Distribution List Expansion History if present,
     */
    private static int get_ms_dleh(MSMessage msmessage_obj, int entry, DLExpHist dleh_obj)
    {
	int status, len;
	// Initialise object for returning string values
	StringBuffer ret_value = new StringBuffer();

// Need the session object to get the error string (why ?)
//com.isode.x400api.X400ms.x400_ms_get_string_error(session_obj, status));

    	status = com.isode.x400api.X400ms.x400_ms_DLexphistget(
	    msmessage_obj,entry, dleh_obj);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no DLEH found \n" + " " + status);
	    return status;
	} 
	System.out.println("DLEH found " + " " + status);
	
	// got this DLEH element - print the values
	status = com.isode.x400api.X400.x400_DLgetstrparam(
	    dleh_obj,
	    X400_att.X400_S_OR_ADDRESS,
	    ret_value);
	
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("no string value for OR Address in DL  failed " + status);
	} else {
	    len =  ret_value.length();
	    System.out.println("DLExpansion List entry " +  "OR Address" + ret_value.toString());
	}
	
	return X400_att.X400_E_NOERROR;
    }

    /**
     * Display the MS Message content (ie the attachments/bodyparts) of 
     * a message retrieved from the message store.
     */
    private static int do_msg_content(MSMessage msmessage_obj)
    {
	int status;
	int paramtype;
	int int_value;
	int len;
	String value;
	int att_num;
	byte[] binarydata = new byte[config.maxlen];
	int att_type;
	int att_len;

	System.out.println("------");
	System.out.println("Content");

	// Initialise object to receive returned String value
	StringBuffer ret_value = new StringBuffer();

	paramtype = X400_att.X400_N_NUM_ATTACHMENTS;
    	status = com.isode.x400api.X400ms.x400_ms_msggetintparam(
	    msmessage_obj, paramtype);
	if (status == X400_att.X400_E_NO_VALUE) {
	    System.out.println("no int value for " + paramtype);
	}
	else if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetintparam (" 
		+ X400_att.X400_N_NUM_ATTACHMENTS + ") failed " + status);
	    return status;
	} 
	int_value = msmessage_obj.GetIntValue();
	System.out.println("number of attachments " + int_value);

	if ( int_value == 0) {
	    System.out.println("No attachments");
	    return X400_att.X400_E_NOERROR;
	}
		
	// Get an IA5 attachment as a string in the message
	System.out.println("------------------------");
	System.out.println("Reading IA5 Attachment");
	paramtype = X400_att.X400_T_IA5TEXT;
	status = com.isode.x400api.X400ms.x400_ms_msggetstrparam(
	    msmessage_obj, paramtype, ret_value);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetstrparam failed(" + paramtype + 
	    	"): result is " + status);
	} else {
	    len = ret_value.length();
	    System.out.println("IA5 Text " + "(" + len + ")" +  ret_value.toString());
	}

        // Get all the attachments/bodyparts..
	BodyPart bodypart_obj = new BodyPart();
	Message message_obj = new Message();
        for ( att_num = 0; ; att_num++ ) {
	    status = get_bp(msmessage_obj,bodypart_obj, att_num);
            if (status == X400_att.X400_E_MISSING_ATTR) {
		System.out.println("No more bodyparts");
		break;
	    } else if (status != X400_att.X400_E_NOERROR &&
                       status != X400_att.X400_E_MISSING_ATTR) {
		return status;
	    }
	}
        
        
        
        System.out.println("Read " + att_num + " attachments");
        System.out.println("------------------------");
	return X400_att.X400_E_NOERROR;
    }
    
    public static int get_msgbp(
        MSMessage msmessage_obj,
        Message message_obj,
        int att_num,
        int type
    )
    {
	int status;
	int int_value;

	int len;
	String value;

	System.out.println("------------------------");
	System.out.println("Reading Message BodyPart " + att_num);
	// get the message body part from the MTMessage
	status = com.isode.x400api.X400ms.x400_ms_msggetmessagebody(
	    msmessage_obj, att_num, message_obj);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msggetmessagebody failed " + status);
	    return status;
	}
        
        StringBuffer ret_value = new StringBuffer();
        System.out.println("get_msgbp "+ type);
        switch(type) {
        case X400_att.X400_T_MM:
            System.out.println("Fetched P772 Military Message bodypart");

            status = com.isode.x400api.X400.x400_msggetstrparam(
                message_obj, X400_att.X400_S_MESSAGE_DELIVERY_TIME,
                ret_value);
            if (status != X400_att.X400_E_NOERROR) {
                System.out.println("x400_msggetstrparam failed " + status);
                return status;
            } 
            System.out.println("MM bodypart delivery time "  + ret_value.toString());
            status = com.isode.x400api.X400.x400_msggetstrparam(
                message_obj, X400_att.X400_S_SUBJECT,
                ret_value);
            if (status != X400_att.X400_E_NOERROR) {
                System.out.println("x400_msggetstrparam failed " + status);
                return status;
            }
            System.out.println("MM subject: " + ret_value.toString());
            break;
        case X400_att.X400_T_FWDENC:
            System.out.println("Fetched P772 Forwarded encrypted bodypart");
            status = com.isode.x400api.X400.x400_msggetstrparam(
                message_obj, X400_att.X400_S_MESSAGE_DELIVERY_TIME,
                ret_value);
            if (status != X400_att.X400_E_NOERROR) {
                System.out.println("x400_msggetstrparam failed " + status);
                return status;
            }
            System.out.println("P772 forwarded encrypted delivery time " + ret_value.toString());
            
            status = com.isode.x400api.X400.x400_msggetstrparam(
                message_obj, X400_att.X400_S_ENCRYPTED_DATA,
                ret_value);
            if (status != X400_att.X400_E_NOERROR) {
                System.out.println("x400_msggetstrparam failed " + status);
                return status;
            }
            System.out.println("P772 forwarded encrypted data: " + ret_value.length() + " Bytes");
            
            break;
        case X400_att.X400_T_FWD_CONTENT:
            System.out.println("Fetched Forwarded content bodypart");
            status = com.isode.x400api.X400.x400_msggetstrparam(
                message_obj, X400_att.X400_S_MESSAGE_DELIVERY_TIME,
                ret_value);
            if (status != X400_att.X400_E_NOERROR) {
                System.out.println("x400_msggetstrparam failed " + status);
                return status;
            }
            System.out.println("Forwarded Content delivery time " + ret_value.toString());

            status = com.isode.x400api.X400.x400_msggetstrparam(
                message_obj, X400_att.X400_S_MESSAGE_IDENTIFIER,
                ret_value);
            if (status != X400_att.X400_E_NOERROR) {
                System.out.println("x400_msggetstrparam failed " + status);
                return status;
            }
            System.out.println("Forwarded Content bodypart message id: " + ret_value.toString());

            
            status = com.isode.x400api.X400.x400_msggetstrparam(
                message_obj, X400_att.X400_S_FWD_CONTENT_STRING,
                ret_value);
            if (status != X400_att.X400_E_NOERROR) {
                System.out.println("x400_msggetstrparam failed " + status);
                return status;
            }
            System.out.println("Forwarded Content bodypart message id: " + ret_value.length());
            
            break;
            
        default:
            // Initialise object to receive returned String value
           

            System.out.println("Read Message BodyPart successfully: " + type);

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
            System.out.println("Got BodyPart string object type successfully ");
            if (value.equals("message")) {
                System.out.println("++++++++++++++++++++++++++++++++++++++");
                System.out.println("Message body part (ie forwarded message)");
                // no envelope for a forwarded msg
                // status = X400msTestRcvNativeUtils.do_xmsg_env(message_obj);
                status = X400msTestRcvNativeUtils.do_xmsg_headers(message_obj);
                /* the content can be retrieved as attachments or
                 * bodyparts. Use do_xmsg_content for the former */
                status = X400msTestRcvNativeUtils.do_xmsg_content_as_bp(message_obj);
                // status = do_xmsg_content(message_obj);
                System.out.println("++++++++++++++++++++++++++++++++++++++");
            } else if (value.equals("report")) {
                System.out.println("Retrieved report body part");
                status = X400msTestRepRcvUtils.do_rep_env(msmessage_obj);
                status = X400msTestRepRcvUtils.do_rep_content(msmessage_obj);
                status = X400msTestRepRcvUtils.do_rep_retcontent(msmessage_obj);
            } else if (value.equals("probe")) {
                // Not handling a probe here 
                System.out.println("Retrieved probe body part");
                return X400_att.X400_E_NYI;
            } else {
                // unknown type
                System.out.println("Retrieved unknown message  body part type " + value);
                return X400_att.X400_E_BADPARAM;
            }
            
        }
	return X400_att.X400_E_NOERROR;

    }
     /**
     * Fetch and display a MT bodypart 
     */
    public static int get_bp(MSMessage msmessage_obj, BodyPart bodypart_obj, int att_num)
    {
	int status;
	int int_value;
	int len;
	int bp_type;
        byte[] binarydata = new byte[config.maxlen];

	System.out.println("----------------");
	System.out.println("Reading MS BodyPart " + att_num);

	// Initialise object for returning string values
	StringBuffer ret_value = new StringBuffer();

	status = com.isode.x400api.X400ms.x400_ms_msggetbodypart(
	    msmessage_obj, att_num, bodypart_obj);
	if (status != X400_att.X400_E_NOERROR && status != X400_att.X400_E_MESSAGE_BODY) {
	    System.out.println("x400_ms_msggetbodypart failed " + status);
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
	System.out.println("Got BodyPart int value : bp type is "
                           + int_value);
        Message message_obj = new Message();
        switch (int_value) {
        case X400_att.X400_T_FWD_CONTENT:
            System.out.println("Got Forwarded Content");
            status = get_msgbp(msmessage_obj,message_obj,att_num,int_value);
            if (status != X400_att.X400_E_NOERROR) {
                System.out.println("get_msgbp failed " + status);
                return status;
            }
            break;
        case X400_att.X400_T_MESSAGE:            
            System.out.println("Got Forwarded Message");

            status = get_msgbp(msmessage_obj,message_obj,att_num,int_value);
            if (status != X400_att.X400_E_NOERROR) {
                System.out.println("get_msgbp failed " + status);
                return status;
            }
            
            break;
        case X400_att.X400_T_ADATP3:
            /*STANAG 4406 B1.1	ADatP3*/
            System.out.println("Got P772 ADatP3");
            
            status = com.isode.x400api.X400.x400_bodypartgetintparam(
                bodypart_obj, X400_att.X400_N_ADATP3_PARM);
            if (status != X400_att.X400_E_NOERROR) {
                System.out.println("x400_bodypartgetintparam failed " + status);
                
            } else {
                int_value = bodypart_obj.GetIntValue();
                System.out.println("Got ADatP3 param is:" + int_value);
            }
            
            status = com.isode.x400api.X400.x400_bodypartgetintparam(
                bodypart_obj, X400_att.X400_N_ADATP3_CHOICE);
            if (status != X400_att.X400_E_NOERROR) {
                System.out.println("x400_bodypartgetintparam failed " + status);
                
            } else {
                int_value = bodypart_obj.GetIntValue();
                System.out.println("Got ADatP3 Choice is:" + int_value);
            }
            
            status = com.isode.x400api.X400.x400_bodypartgetstrparam(
                bodypart_obj, X400_att.X400_S_ADATP3_DATA, ret_value,
                binarydata);
            if (status != X400_att.X400_E_NOERROR) {
                System.out.println("x400_bodypartgetstrparam failed "
                                   + status);
                return status;
            } 
            System.out.println("Got ADatP3 data" + ret_value.toString());
            break;
            
        case X400_att.X400_T_CORRECTIONS:
            /*STANAG 4406 B1.2	Corrections */
            System.out.println("Got P772 Correction");
            status = com.isode.x400api.X400.x400_bodypartgetintparam(
                bodypart_obj, X400_att.X400_N_CORREC_PARM);
            if (status != X400_att.X400_E_NOERROR) {
                System.out.println("x400_bodypartgetintparam failed " + status);
                
            } else {
                int_value = bodypart_obj.GetIntValue();
                System.out.println("Got correction param is:" + int_value);
            }

            status = com.isode.x400api.X400.x400_bodypartgetstrparam(
                bodypart_obj, X400_att.X400_S_CORREC_DATA, ret_value,
                binarydata);
            if (status != X400_att.X400_E_NOERROR) {
                System.out.println("x400_bodypartgetstrparam failed "
                                   + status);
                return status;
            } 
            System.out.println("Got Correction data" + ret_value.toString());
            break;

        case X400_att.X400_T_FWDENC:
            System.out.println("Got P772 Forwarded encrypted");

            status = get_msgbp(msmessage_obj,message_obj,att_num,int_value);
            if (status != X400_att.X400_E_NOERROR) {
                System.out.println("get_msgbp failed " + status);
                return status;
            }
            break;
            
        case X400_att.X400_T_MM:
            System.out.println("Got P772 T MM");
         
            status = get_msgbp(msmessage_obj,message_obj,att_num,int_value);
            if (status != X400_att.X400_E_NOERROR) {
                System.out.println("get_msgbp failed " + status);
                return status;
            }
            break;
            
        case X400_att.X400_T_ACP127DATA:
            System.out.println("Got P772 ACP127 Data");
            status = com.isode.x400api.X400.x400_bodypartgetintparam(
                bodypart_obj, X400_att.X400_N_ACP127DATA_PARM);
            if (status != X400_att.X400_E_NOERROR) {
                System.out.println("x400_bodypartgetintparam failed " + status);
                
            } else {
                int_value = bodypart_obj.GetIntValue();
                System.out.println("Got ACP127 data param is:" + int_value);
            }

            status = com.isode.x400api.X400.x400_bodypartgetstrparam(
                bodypart_obj, X400_att.X400_S_ACP127_DATA, ret_value,
                binarydata);
            if (status != X400_att.X400_E_NOERROR) {
                System.out.println("x400_bodypartgetstrparam failed "
                                   + status);
                return status;
            } 
            System.out.println("Got ACP127Data" + ret_value.toString());
            break;

            
        case X400_att.X400_T_BINARY:
        case X400_att.X400_T_FTBP:
                // read the data from the body part
                status = com.isode.x400api.X400.x400_bodypartgetstrparam(
                    bodypart_obj, X400_att.X400_S_BODY_DATA, ret_value,
                    binarydata);
                if (status != X400_att.X400_E_NOERROR) {
                    System.out.println("x400_bodypartgetstrparam failed "
                                       + status);
                    return status;
                }
                
		bp_type = bodypart_obj.GetBpType();
		len = bodypart_obj.GetBpLen();
		System.out.println("Binary Attachment (" 
                                   + int_value + "), len = " + len);
		for (int i = 0; i < len ; i++) 
		    System.out.print(binarydata[i] + " ");
		System.out.println();
		break;
	    default:
                 // read the data from the body part
                status = com.isode.x400api.X400.x400_bodypartgetstrparam(
                    bodypart_obj, X400_att.X400_S_BODY_DATA, ret_value,
                    binarydata);
                if (status != X400_att.X400_E_NOERROR) {
                    System.out.println("x400_bodypartgetstrparam failed "
                                       + status);
                    return status;
                }
		System.out.println("Got BodyPart string value(" 
                                   + ret_value.capacity() + 
                                   "): bp value is \n" +
                                   ret_value.toString());
		break;
            }
	return status;
    }

    /**
     * Submit an IPN for the message received.
     */
    private static int send_ipn(MSMessage msmessage_obj, int reason)
    {
    	int status;
	MSMessage ipn = new MSMessage();


	System.out.println("Creating IPN ...");
    	status = com.isode.x400api.X400ms.x400_ms_msgmakeIPN(
	    msmessage_obj, reason, ipn);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("msgmakeIPN failed " + status);
	    return status;
	}

	// now submit the IPN
	System.out.println("Submitting IPN ...");
    	status = com.isode.x400api.X400ms.x400_ms_msgsend(ipn);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgsend failed " + status);
	    return status;
	}
	System.out.println("Submitted IPN successfully");

	return status;
    }

}

