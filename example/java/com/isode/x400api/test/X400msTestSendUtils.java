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

// 

/**
 ** Set of functions which submit a message into a P7 messge store.
 */


package com.isode.x400api.test;

import com.isode.x400api.*;
import com.isode.x400api.X400ms.*;
import com.isode.x400api.test.config;

import java.io.IOException;
import java.io.FileInputStream;
import java.io.File;

public class X400msTestSendUtils
{

    /* Cheap noddy way of reading in sample binary file */
    private static byte[] read_bin_file (
        String filename
    )
        {
            byte[] bytes;
            long file_length;
            try {
                File file = new File(config.sharedir, filename);
                file_length = file.length();
                
                FileInputStream filestream = new FileInputStream(file);
                bytes = new byte[(int)file_length];
                
                System.out.println("Reading in " + file_length + " bytes ");
                filestream.read(bytes,0,(int)file_length);
                filestream.close();
                System.out.println("Finished read_bin_file: ");
                return bytes;
           
            
            } catch (IOException obj) {
                System.out.println("Failed to add bin: "+obj.getMessage());
            }
            System.out.println("Finished read_bin_file: ");
            return null;
        }
    
    private static int add_acp127resp(Recip recip_obj){
        int status;
        ACP127Resp resp_obj = new ACP127Resp();
        
        System.out.println("Adding acp127 resp");
        status = com.isode.x400api.X400.x400_acp127respnew(recip_obj,
                                                           resp_obj);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("add_acp127resp failed " + status);
            return status;    
        }

        status  = com.isode.x400api.X400.x400_acp127respaddintparam(
            resp_obj,X400_att.X400_ACP127_NOTI_TYPE_NEG);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_acp127respaddintparam failed " + status);
            return status;
        }

        status  = com.isode.x400api.X400.x400_acp127respaddintparam(
            resp_obj,X400_att.X400_ACP127_NOTI_TYPE_POS);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_acp127respaddintparamfailed " + status);
            return status;
        }
        

        status  = com.isode.x400api.X400.x400_acp127respaddintparam(
            resp_obj,X400_att.X400_ACP127_NOTI_TYPE_TRANS);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_acp127respaddintparam failed " + status);
            return status;
        }

        status  = com.isode.x400api.X400.x400_acp127addstrparam(
            resp_obj,
            X400_att.X400_S_ACP127_NOTI_RESP_TIME,
            config.mt_acp127resp_resp_time);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_acp127addstrparam failed " + status);
            return status;
        }

        status  = com.isode.x400api.X400.x400_acp127addstrparam(
            resp_obj,
            X400_att.X400_S_ACP127_NOTI_RESP_RECIPIENT,
            config.mt_acp127resp_resp_recip);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_acp127addstrparam failed " + status);
            return status;
        }
        
        status  = com.isode.x400api.X400.x400_acp127addstrparam(
            resp_obj,
            X400_att.X400_S_ACP127_NOTI_RESP_SUPP_INFO,
            config.mt_acp127resp_supp_info);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_acp127addstrparam failed " + status);
            return status;
        }
        
        /* Now add an ALI */
        ALI ali = new ALI();
        System.out.println("Adding acp127resp ALI");
        status = com.isode.x400api.X400.x400_acp127respalinew(resp_obj,
                                                              ali);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_acp127respalinew failed " + status);
            return status;    
        }
         status  = com.isode.x400api.X400.x400_aliaddstrparam(ali,X400_att.X400_S_IOB_OR_ADDRESS, config.mt_ali_or1);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_aliaddstrparam failed " + status);
            return status;
        }
          
        status  = com.isode.x400api.X400.x400_aliaddstrparam(ali,X400_att.X400_S_IOB_DN_ADDRESS, config.mt_ali_dn1);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_aliaddstrparam failed " + status);
            return status;
        }
        
        status  = com.isode.x400api.X400.x400_aliaddstrparam(ali,X400_att.X400_S_IOB_FREE_FORM_NAME, config.mt_ali_free_form_name1);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_aliaddstrparam failed " + status);
            return status;
        }
        
        status  = com.isode.x400api.X400.x400_aliaddstrparam(ali,X400_att.X400_S_IOB_TEL, config.mt_ali_tel1);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_aliaddstrparam failed " + status);
            return status;
        }

        status  = com.isode.x400api.X400.x400_aliaddintparam(ali,X400_att.X400_N_ALI_TYPE, config.mt_ali_type1);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_aliaddintparam failed " + status);
            return status;
        }


        status  = com.isode.x400api.X400.x400_aliaddintparam(ali,X400_att.X400_N_ALI_NOTIFICTAION_REQUEST, config.mt_ali_noti_request1);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_aliaddintparam failed " + status);
            return status;
        }
        
        status  = com.isode.x400api.X400.x400_aliaddintparam(ali,X400_att.X400_N_ALI_REPLY_REQUEST, config.mt_ali_reply_request1);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_aliaddintparam failed " + status);
            return status;
        }
        
        return X400_att.X400_E_NOERROR;
        
    }
    
     private static int add_fwd_enc (MSMessage msmessage_obj) {
        int status;
        byte [] fwd_enc_bytes;
        Message fwd_enc_message_obj = new Message();
        System.out.println("Adding P772 Forwarded encrypted bodypart ");
        /* NB: We are just re-using the pilot_fwd_info BER as forwarded
         * encrypted ber
         */
        fwd_enc_bytes = read_bin_file("pilot_fwd_info.ber"); 

        
        /* Add Envelope information to this forwarded encrypted
         * message */
        status = X400BuildFwdMsg.build_fwd_msg(fwd_enc_message_obj,
                                               config.fwd_recip_oraddress);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("build_fwd_msg_env failed " + status);
	    return status;
	}
        
        /* Set the content type to be p772 */
        status = com.isode.x400api.X400.x400_msgaddstrparam(fwd_enc_message_obj, 
        X400_att.X400_S_EXTERNAL_CONTENT_TYPE, config.ms_msg_external_type,-1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddstrparam failed " + status);
	    return status;
	}

        status = com.isode.x400api.X400.x400_msgaddbyteparam(fwd_enc_message_obj, 
        X400_att.X400_S_ENCRYPTED_DATA,fwd_enc_bytes);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddbyteparam failed " + status);
	    return status;
	}

        status = com.isode.x400api.X400ms.x400_ms_msgaddmessagebodywtype(msmessage_obj,fwd_enc_message_obj,X400_att.X400_T_FWDENC);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddstrparam failed " + status);
	    return status;
	}
        
        
        return X400_att.X400_E_NOERROR;
    }
    
    
    private static int add_fwd_MM (MSMessage msmessage_obj) {
        int status;
        Message fwd_MM_obj = new Message();
        System.out.println("Adding P772 Forwarded Military Message ");
        /* The build_fwd_msg will create suitable envelope and message content
         */
        status = X400BuildFwdMsg.build_fwd_msg(fwd_MM_obj,
                                               config.fwd_recip_oraddress);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("build_fwd_msg failed " + status);
	    return status;
	}
        
        /* Set the content type to be p772 */
        status = com.isode.x400api.X400.x400_msgaddstrparam(fwd_MM_obj, 
        X400_att.X400_S_EXTERNAL_CONTENT_TYPE, config.ms_msg_external_type,-1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddstrparam failed " + status);
	    return status;
	}
        
         status = com.isode.x400api.X400ms.x400_ms_msgaddmessagebodywtype(msmessage_obj,fwd_MM_obj,X400_att.X400_T_MM);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddstrparam failed " + status);
	    return status;
	}
                
        return X400_att.X400_E_NOERROR;
    }


    private static int add_x420_fwd_content (MSMessage msmessage_obj) {
        int status;
        byte [] fwd_cnt_bytes;
        Message fwd_cnt_message_obj = new Message();
        System.out.println("Adding Forwarded content bodypart ");
        
        fwd_cnt_bytes = read_bin_file("example_fwd_content.ber"); 
        
        /* Add Envelope information to this forwarded encrypted
         * message */
        status = X400BuildFwdMsg.build_fwd_msg(fwd_cnt_message_obj,
                                               config.fwd_recip_oraddress);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("build_fwd_msg_env failed " + status);
	    return status;
	}
        
        /* Set the content type to be p772
         * NB: This doesn't actually have to be P772, but it doesn't hurt*/
        status = com.isode.x400api.X400.x400_msgaddstrparam(fwd_cnt_message_obj, 
        X400_att.X400_S_EXTERNAL_CONTENT_TYPE, config.ms_msg_external_type,-1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddstrparam failed " + status);
	    return status;
	}

        status = com.isode.x400api.X400.x400_msgaddbyteparam(fwd_cnt_message_obj, 
        X400_att.X400_S_FWD_CONTENT_STRING,fwd_cnt_bytes);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddbyteparam failed " + status);
	    return status;
	}

        status = com.isode.x400api.X400ms.x400_ms_msgaddmessagebodywtype(msmessage_obj,fwd_cnt_message_obj,X400_att.X400_T_FWD_CONTENT);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddstrparam failed " + status);
	    return status;
	}
        
        
        return X400_att.X400_E_NOERROR;
    }
    
    
    private static int add_adatp3 (MSMessage msmessage_obj) {
        int status;
        BodyPart bp = new BodyPart();
        
        System.out.println("Adding P772 adatp3 ");
        
        status = com.isode.x400api.X400.x400_bodypartnew(X400_att.X400_T_ADATP3,bp);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_bodypartnew failed " + status);
            return status;
        }
        
        status = com.isode.x400api.X400ms.x400_ms_msgaddbodypart(msmessage_obj,bp);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_msgaddbodypart failed " + status);
            return status;
        }
        
        status = com.isode.x400api.X400.x400_bodypartaddintparam(
            bp,
            X400_att.X400_N_ADATP3_PARM,
            1
        );
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_bodypartaddintparam failed " + status);
            return status;
        }
        
        
        status = com.isode.x400api.X400.x400_bodypartaddintparam(
            bp,
            X400_att.X400_N_ADATP3_CHOICE,
            1
        );
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_bodypartaddintparam failed " + status);
            return status;
        }
                
        status = com.isode.x400api.X400.x400_bodypartaddstrparam(
            bp,
            X400_att.X400_S_ADATP3_DATA,
            config.adatp3_line1, -1
        );
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_bodypartaddstrparam failed " + status);
            return status;
        }
        
            
        return X400_att.X400_E_NOERROR;
    }
    
    private static int add_corrections (MSMessage msmessage_obj) {
        int status;
        BodyPart bp = new BodyPart();
        System.out.println("Adding P772 correction ");
        status = com.isode.x400api.X400.x400_bodypartnew(X400_att.X400_T_CORRECTIONS,bp);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_bodypartnew failed " + status);
            return status;
        }
        
        status = com.isode.x400api.X400ms.x400_ms_msgaddbodypart(msmessage_obj,bp);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_msgaddbodypart failed " + status);
            return status;
        }
        
        status = com.isode.x400api.X400.x400_bodypartaddintparam(
            bp,
            X400_att.X400_N_CORREC_PARM,
            1
        );
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_bodypartaddintparam failed " + status);
            return status;
        }
        
        
        status = com.isode.x400api.X400.x400_bodypartaddstrparam(
            bp,
            X400_att.X400_S_CORREC_DATA,
            config.ms_correction, -1
        );
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_bodypartaddstrparam failed " + status);
            return status;
        }
        
            
        return X400_att.X400_E_NOERROR;
    }
    
    private static int add_acp127data (MSMessage msmessage_obj) {
        int status;
        BodyPart bp = new BodyPart();
        System.out.println("Adding P772 ACP127Data ");
        status = com.isode.x400api.X400.x400_bodypartnew(X400_att.X400_T_ACP127DATA,bp);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_bodypartnew failed " + status);
            return status;
        }
        
        status = com.isode.x400api.X400ms.x400_ms_msgaddbodypart(msmessage_obj,bp);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_ms_msgaddbodypart failed " + status);
            return status;
        }
        
        status = com.isode.x400api.X400.x400_bodypartaddintparam(
            bp,
            X400_att.X400_N_ACP127DATA_PARM,
            1
        );
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_bodypartaddintparam failed " + status);
            return status;
        }
        
        
        status = com.isode.x400api.X400.x400_bodypartaddstrparam(
            bp,
            X400_att.X400_S_ACP127_DATA,
            config.ms_acp127data, -1
        );
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_bodypartaddstrparam failed " + status);
            return status;
        }
        
            
        return X400_att.X400_E_NOERROR;
    }
     private static int add_otherrecip(MSMessage msmessage_obj) {
        int status;
        OtherRecip otherrecip = new OtherRecip();
        status = com.isode.x400api.X400ms.x400_ms_otherrecipnew(msmessage_obj,otherrecip);
       	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("add_otherrecip failed " + status);
	    return status;
	}

        status  = com.isode.x400api.X400.x400_otherrecipaddstrparam(otherrecip, config.ms_otherrecip_str);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_otherrecipaddstrparam failed " + status);
            return status;
        }
        
        status  = com.isode.x400api.X400.x400_otherrecipaddintparam(otherrecip,
                                                                    config.ms_otherrecip_int);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_otherrecipaddintparam failed " + status);
            return status;
        }
        
        return X400_att.X400_E_NOERROR;
     }
    
    private static int add_ali(MSMessage msmessage_obj) {
        int status;
        ALI ali = new ALI();
        status = com.isode.x400api.X400ms.x400_ms_alinew(msmessage_obj,ali);
       	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("alinew failed " + status);
	    return status;
	}
        
        status  = com.isode.x400api.X400.x400_aliaddstrparam(ali,X400_att.X400_S_IOB_OR_ADDRESS, config.ms_ali_or1);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_aliaddstrparam failed " + status);
            return status;
        }
          
        status  = com.isode.x400api.X400.x400_aliaddstrparam(ali,X400_att.X400_S_IOB_DN_ADDRESS, config.ms_ali_dn1);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_aliaddstrparam failed " + status);
            return status;
        }
        
        status  = com.isode.x400api.X400.x400_aliaddstrparam(ali,X400_att.X400_S_IOB_FREE_FORM_NAME, config.ms_ali_free_form_name1);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_aliaddstrparam failed " + status);
            return status;
        }
        
        status  = com.isode.x400api.X400.x400_aliaddstrparam(ali,X400_att.X400_S_IOB_TEL, config.ms_ali_tel1);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_aliaddstrparam failed " + status);
            return status;
        }

        status  = com.isode.x400api.X400.x400_aliaddintparam(ali,X400_att.X400_N_ALI_TYPE, config.ms_ali_type1);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_aliaddintparam failed " + status);
            return status;
        }


        status  = com.isode.x400api.X400.x400_aliaddintparam(ali,X400_att.X400_N_ALI_NOTIFICTAION_REQUEST, config.ms_ali_noti_request1);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_aliaddintparam failed " + status);
            return status;
        }
        
        status  = com.isode.x400api.X400.x400_aliaddintparam(ali,X400_att.X400_N_ALI_REPLY_REQUEST, config.ms_ali_reply_request1);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_aliaddintparam failed " + status);
            return status;
        }
        
        
        return X400_att.X400_E_NOERROR;
    }



    private static int add_handling_instructions(MSMessage msmessage_obj) {
        int status;
        PSS hi = new PSS();
        status = com.isode.x400api.X400ms.x400_ms_pssnew(msmessage_obj,hi,X400_att.X400_S_HANDLING_INSTRUCTIONS);
       	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("add_handling_instructions failed " + status);
	    return status;
	}

        status  = com.isode.x400api.X400.x400_pssaddstrparam(hi, config.ms_handling_instruction1);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_pssaddstrparam failed " + status);
            return status;
        }

         PSS hi2 = new PSS();
        status = com.isode.x400api.X400ms.x400_ms_pssnew(msmessage_obj,hi2,X400_att.X400_S_HANDLING_INSTRUCTIONS);
       	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("add_handling_instructions failed " + status);
	    return status;
	}

        status  = com.isode.x400api.X400.x400_pssaddstrparam(hi2,config.ms_handling_instruction2);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_pssaddstrparam failed " + status);
            return status;
        }

        return X400_att.X400_E_NOERROR;
    }
 private static int add_messaging_instructions(MSMessage msmessage_obj) {
        int status;
        PSS mi = new PSS();
        status = com.isode.x400api.X400ms.x400_ms_pssnew(msmessage_obj,mi,X400_att.X400_S_MESSAGE_INSTRUCTIONS);
       	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("add_message_instructions failed " + status);
	    return status;
	}

        status  = com.isode.x400api.X400.x400_pssaddstrparam(mi,config.mt_message_instruction1);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_pssaddstrparam failed " + status);
            return status;
        }

        PSS mi2 = new PSS();
        status = com.isode.x400api.X400ms.x400_ms_pssnew(msmessage_obj,mi2,X400_att.X400_S_MESSAGE_INSTRUCTIONS);
       	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("add_message_instructions failed " + status);
	    return status;
	}

        status  = com.isode.x400api.X400.x400_pssaddstrparam(mi2,config.ms_message_instruction2);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_pssstrparam failed " + status);
            return status;
        }
        
        return X400_att.X400_E_NOERROR;
 }
    private static int add_dist_codes(MSMessage msmessage_obj) {
        int status;
        PSS dc = new PSS();
        status = com.isode.x400api.X400ms.x400_ms_pssnew(msmessage_obj,dc,X400_att.X400_S_DIST_CODES_SIC);
       	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("add_dist_codes failed " + status);
	    return status;
	}

        status  = com.isode.x400api.X400.x400_pssaddstrparam(dc,config.mt_distcode1);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_pssaddstrparam failed " + status);
            return status;
        }

        PSS dc2 = new PSS();
        status = com.isode.x400api.X400ms.x400_ms_pssnew(msmessage_obj,dc2,X400_att.X400_S_DIST_CODES_SIC);
       	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("add_dist_codes failed " + status);
	    return status;
	}

        status  = com.isode.x400api.X400.x400_pssaddstrparam(dc2,config.mt_distcode2);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_pssstrparam failed " + status);
            return status;
        }

        /* Now add distcode fields */
        DistField distfield = new DistField();
        System.out.println("Adding dist fields ");
        status = com.isode.x400api.X400ms.x400_ms_distfieldnew(msmessage_obj,distfield);
       	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("add_dist_codes failed " + status);
	    return status;
	}

        status  = com.isode.x400api.X400.x400_distfieldaddstrparam(distfield,X400_att.X400_S_DIST_CODES_EXT_OID,config.mt_distfieldoid1);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_distfieldaddstrparam failed " + status);
            return status;
        }

        status  = com.isode.x400api.X400.x400_distfieldaddbyteparam(distfield,X400_att.X400_S_DIST_CODES_EXT_VALUE,config.mt_distfieldvalue1);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_distfieldaddstrparam failed " + status);
            return status;
        }


        DistField distfield2 = new DistField();
        status = com.isode.x400api.X400ms.x400_ms_distfieldnew(msmessage_obj,distfield2);
       	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("add_dist_codes failed " + status);
	    return status;
	}

        status  = com.isode.x400api.X400.x400_distfieldaddstrparam(distfield2,X400_att.X400_S_DIST_CODES_EXT_OID,config.mt_distfieldoid2);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_distfieldaddstrparam failed " + status);
            return status;
        }

        status  = com.isode.x400api.X400.x400_distfieldaddbyteparam(distfield2,X400_att.X400_S_DIST_CODES_EXT_VALUE,config.mt_distfieldvalue2);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_distfieldaddstrparam failed " + status);
            return status;
        }
        
        
        return X400_att.X400_E_NOERROR;
    }

    
    private static int add_exempt_address(MSMessage msmessage_obj) {
        int status;
        Recip recip_obj = new Recip();
    	status = com.isode.x400api.X400ms.x400_ms_recipnew(msmessage_obj, X400_att.X400_EXEMPTED_ADDRESS, recip_obj);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_recipnew failed " + status);
	    return status;
	}

        status = com.isode.x400api.X400.x400_recipaddstrparam(recip_obj, 
	    X400_att.X400_S_IOB_OR_ADDRESS, config.ms_ea_or_address, -1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_recipaddstrparam failed " + status);
	    return status;
	}

        status = com.isode.x400api.X400.x400_recipaddstrparam(recip_obj, 
	    X400_att.X400_S_IOB_DN_ADDRESS, config.ms_ea_dn_address, -1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_recipaddstrparam failed " + status);
	    return status;
	}

        status = com.isode.x400api.X400.x400_recipaddstrparam(recip_obj, 
	    X400_att.X400_S_IOB_FREE_FORM_NAME, config.ms_ea_ffn, -1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_recipaddstrparam failed " + status);
	    return status;
	}

        status = com.isode.x400api.X400.x400_recipaddstrparam(recip_obj, 
	    X400_att.X400_S_IOB_TEL, config.ms_ea_tel, -1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_recipaddstrparam failed " + status);
	    return status;
	}

        return X400_att.X400_E_NOERROR;
    }

    private static int build_p772 (MSMessage msmessage_obj)
    {
        int status;
        
        if (config.mt_use_p772 == false)
            return X400_att.X400_E_NOERROR;

        /* STANAG 4406 A1.6 Exempted Address */
        System.out.println("Adding P772 exempted address ");
        status = add_exempt_address(msmessage_obj);
        if ( status != X400_att.X400_E_NOERROR ) {
            return status;
        }


        /*STANAG 4406 A1.3	Distribution codes */
        System.out.println("Adding P772 Distribution codes ");
        status = add_dist_codes(msmessage_obj);
        if ( status != X400_att.X400_E_NOERROR ) {
            return status;
        }
        
        
        /* STANAG 4406 A1.4	Handling instructions */
        System.out.println("Adding P772 Handling instructions ");
        status = add_handling_instructions(msmessage_obj);
        if ( status != X400_att.X400_E_NOERROR ) {
            return status;
        }

        /* STANAG 4406 A1.5	Message instructions */
        System.out.println("Adding P772 Message instructions ");
        status = add_messaging_instructions(msmessage_obj);
        if ( status != X400_att.X400_E_NOERROR ) {
            return status;
        }
        
        
        System.out.println("Adding P772 Codress ");
        /* STANAG 4406 A1.6	Codress message */

        status = com.isode.x400api.X400ms.x400_ms_msgaddintparam(msmessage_obj, 
                          X400_att.X400_N_EXT_CODRESS, config.ms_codress);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_msgaddintparam failed " + status);
            return status;
        }

        System.out.println("Adding P772 originator reference");
        /*STANAG 4406 A1.7	Originator reference*/
        status = com.isode.x400api.X400ms.x400_ms_msgaddstrparam(msmessage_obj, 
	    X400_att.X400_S_ORIG_REF, config.ms_orig_ref,-1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddstrparam failed " + status);
	    return status;
	}
        
        System.out.println("Adding P772 primary precedence ");
        /* STANAG 4406 A1.8	Primary precedence */
        status = com.isode.x400api.X400ms.x400_ms_msgaddintparam(msmessage_obj, 
                          X400_att.X400_N_EXT_PRIM_PREC, config.ms_prim_prec);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_msgaddintparam failed " + status);
            return status;
        }

        System.out.println("Adding P772 Copy precedence ");
        /* STANAG 4406 A1.9    Copy precedence */
        status = com.isode.x400api.X400ms.x400_ms_msgaddintparam(msmessage_obj, 
                          X400_att.X400_N_EXT_COPY_PREC, config.ms_copy_prec);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_msgaddintparam failed " + status);
            return status;
        }

        System.out.println("Adding P772 message type ");
        /* STANAG 4406 A1.10	Message type */
        status = com.isode.x400api.X400ms.x400_ms_msgaddintparam(msmessage_obj, 
                          X400_att.X400_N_EXT_MSG_TYPE, config.ms_msg_type);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_msgaddintparam failed " + status);
            return status;
        }

        System.out.println("Adding P772 Address list indicator");
        /* STANAG 4406 A1.11	Address list indicator */
        status = add_ali(msmessage_obj);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("Failed to add ALI" + status);
	    return status;
	}

        System.out.println("Adding P772 Other recipient ");
        /* STANAG 4406 A1.12	Other recipients indicator */
        status = add_otherrecip(msmessage_obj);
        if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("Failed to add Other Recip" + status);
	    return status;
	}
        
        System.out.println("Adding P772 Ext Auth info ");
        /*STANAG 4406 A1.2	Extended authorisation information */
        status = com.isode.x400api.X400ms.x400_ms_msgaddstrparam(msmessage_obj, 
	    X400_att.X400_S_EXT_AUTH_INFO, config.ms_ext_auth_info,-1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddstrparam failed " + status);
	    return status;
	}

        System.out.println("Adding P772 ACP127 Msg ID ");
        /* STANAG 4406 A1.14	ACP127 message identifier */
        status = com.isode.x400api.X400ms.x400_ms_msgaddstrparam(msmessage_obj, 
	    X400_att.X400_S_ACP127_MSG_ID, config.ms_acp127_msg_id,-1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_msgaddstrparam failed " + status);
	    return status;
	}
        
        System.out.println("Adding P772 Orig PLAD ");
        /* STANAG 4406 A1.15	Originator PLAD */
         status = com.isode.x400api.X400ms.x400_ms_msgaddstrparam(msmessage_obj, 
	    X400_att.X400_S_ORIG_PLAD, config.ms_orig_plad,-1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_mt_msgaddstrparam failed " + status);
	    return status;
	}

        /* STANAG 4406 A1.13	Pilot forwarding information */
        
        byte [] pilot_bytes;
        
        System.out.println("Adding Pilot forwarding info ");
        pilot_bytes = read_bin_file("pilot_fwd_info.ber");
        
        if (pilot_bytes != null) {
            System.out.println("Adding pilot forwarding info bytes");
            status = com.isode.x400api.X400ms.x400_ms_msgaddbyteparam(
                msmessage_obj, 
                X400_att.X400_S_PILOT_FWD_INFO,
                pilot_bytes);
        }
        
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("Adding pilot forwarding info failed " + status);
            return status;
        }
        
        /* STANAG 4406 A1.16	Security Information Labels */
        byte [] sec_label_bytes;
        
        System.out.println("Adding security info label ");
        sec_label_bytes = read_bin_file("info_sec_label.ber");
        
        if (sec_label_bytes != null) {
            System.out.println("Adding security info label bytes");
            status = com.isode.x400api.X400ms.x400_ms_msgaddbyteparam(
                msmessage_obj, 
                X400_att.X400_S_INFO_SEC_LABEL,
                sec_label_bytes);
        }
        
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("Adding pilot forwarding info failed " + status);
            return status;
        } 
        
        

        return X400_att.X400_E_NOERROR;
    }

    
    
    /**
     * Submit a message into via the Message Store.
     */
    public static void send_msg(String[] args)
    {
        int type = 0;
	int status;

	// Use the OR address used in the P7 bind as the recipient
	String p7_sender = config.p7_bind_oraddr;

	// instantiate a message object, and make it an API object 
	// by opening an API session 
	Session session_obj = new Session();

	// Open the session using the instance field values
	if (config.use_opencheck) {
	    String ret_psw = config.p7_ret_psw;

	    status = com.isode.x400api.X400ms.x400_ms_opencheck(type, 
		config.p7_bind_oraddr, config.p7_bind_dn, 
		config.p7_credentials, config.p7_pa, ret_psw, session_obj);
	} else {
	    status = com.isode.x400api.X400ms.x400_ms_open(type, 
		config.p7_bind_oraddr, config.p7_bind_dn, 
		config.p7_credentials, config.p7_pa, session_obj);
	}
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_open failed " + status);
	    return;
	}
	System.out.println("Opened MS session successfully");

	// turn on all logging for this session
    	status = com.isode.x400api.X400ms.x400_ms_setstrdefault(session_obj, 
	    X400_att.X400_S_LOG_CONFIGURATION_FILE, "x400api.xml", -1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_setstrdefault failed " + status);
	    return;
	}

	// instantiate a message object, and make it an API object 
	MSMessage msmessage_obj = new MSMessage();
	if (config.building_simple_message) {
	    status = build_simple_msmsg(session_obj, msmessage_obj, p7_sender);
	} else {
	    status = build_msmsg(session_obj, msmessage_obj, p7_sender);
	}
	if (status != X400_att.X400_E_NOERROR) {
	    return;
	}

	// Set up a default security env so we can sign messages
	if (config.building_signed_message) {
	    status = X400msTestSignUtils.setup_default_sec_env(session_obj, 
		config.sec_id, config.sec_id_dn, config.sec_pphr);
	    if (status != X400_att.X400_E_NOERROR) {
		return;
	    }
	}
        
	// message all assembled - submit it 
    	status = com.isode.x400api.X400ms.x400_ms_msgsend(msmessage_obj);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgsend failed " + status);
	    return;
	}
	System.out.println("Submitted message successfully");

	// close the API session 
    	status = com.isode.x400api.X400ms.x400_ms_close(session_obj);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_close failed " + status);
	    return;
	}
	System.out.println("Closed MS Session successfully\n");
	return;
    }

    /**
     * Build a message to submit into the Message Store.
     */
    private static int build_simple_msmsg(Session session_obj, MSMessage msmessage_obj, String orig)
    {
	return X400_att.X400_E_NYI;
    }

    /**
     * Build a message to submit into the Message Store.
     */
    private static int build_msmsg(Session session_obj, MSMessage msmessage_obj, String orig)
    {
	int status;
	int rno = 1;
        String unknown_recip
            = "/CN=invalid/O=GatewayMTA/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";


    	// make message object an API object
    	status = com.isode.x400api.X400ms.x400_ms_msgnew(session_obj, 
	    X400_att.X400_MSG_MESSAGE, msmessage_obj);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgnew failed " + status);
	    return status;
	}

	System.out.println("Building message envelope");
	status = build_ms_env(msmessage_obj);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgnew failed " + status);
	    return status;
	}

        
	System.out.println("Building message content");
	status = build_ms_content(session_obj, orig, msmessage_obj);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgnew failed " + status);
	    return status;
	}

        if (config.ms_use_p772 == true) {
            System.out.println("Adding P772 attributes");
            status = build_p772(msmessage_obj);
            if (status != X400_att.X400_E_NOERROR) {
                System.out.println("x400_ms_msgnew failed " + status);
                return status;
            }            
        }
        
	System.out.println("Adding originator");
	// envelope 
    	status = com.isode.x400api.X400ms.x400_ms_msgaddstrparam(msmessage_obj, 
	    X400_att.X400_S_OR_ADDRESS, orig, -1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddstrparam failed " + status);
	    return status;
	}
	// header 
	status = add_ms_recip(msmessage_obj, orig, 
	    X400_att.X400_ORIGINATOR, rno++);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgnew failed " + status);
	    return status;
	}



	System.out.println("Adding recipients");

	// add recip to msg - use originator as recip 1
	status = add_ms_recip(msmessage_obj, orig, 
	    X400_att.X400_RECIP_STANDARD, rno++);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgnew failed " + status);
	    return status;
	}

	// add unknown (unroutable) recip to message
	status = add_ms_recip(msmessage_obj, unknown_recip, 
	    X400_att.X400_RECIP_STANDARD, rno++);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgnew failed " + status);
	    return status;
	}

        // Add DL exempted recipents
        status = add_ms_recip(msmessage_obj, config.mt_msg_dl_exempted_addr, 
	    X400_att.X400_DL_EXEMPTED_RECIP, rno++);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_mt_msgnew failed " + status);
	    return status;
	}
        
        
        
	// cause the message to be signed 
	if (config.building_signed_message) {
	    status = com.isode.x400api.X400ms.x400_ms_msgaddintparam(
		msmessage_obj, X400_att.X400_B_SEC_GEN_MOAC, 1);
	    if (status != X400_att.X400_E_NOERROR) {
		System.out.println("x400_ms_msgaddintparam failed " + status);
		return status;
	    }
	}

	return status;
    }

    /**
     * Add a recipient into a message for submission into the Message Store.
     */
    private static int add_ms_recip(MSMessage msmessage_obj, 
	String recip, int type, int rno)
    {
	int status;
	// value to use as the alternate recipient
	String alt_recip = "/CN=lppt2/OU=lppt/O=" 
	    + config.hostname + "/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
	String phone_num = "02087830203";

	// instantiate a recip object, and make it an API object 
	Recip recip_obj = new Recip();
    	status = com.isode.x400api.X400ms.x400_ms_recipnew(msmessage_obj, 
	    type, recip_obj);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_recipnew failed " + status);
	    return status;
	}

	// add some X.400 envelope attributes into the recipient 

	// use length of -1 to indicate NULL terminated 
    	status = com.isode.x400api.X400ms.x400_ms_recipaddstrparam(recip_obj, 
	    X400_att.X400_S_OR_ADDRESS, recip, -1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_recipaddstrparam failed " + status);
	    return status;
	}

    	status = com.isode.x400api.X400ms.x400_ms_recipaddintparam(recip_obj, 
	    X400_att.X400_N_MTA_REPORT_REQUEST, 3);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_recipaddintparam failed " + status);
	    return status;
	}

    	status = com.isode.x400api.X400ms.x400_ms_recipaddintparam(recip_obj, 
	    X400_att.X400_N_REPORT_REQUEST, 2);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_recipaddintparam failed " + status);
	    return status;
	}

    	status = com.isode.x400api.X400ms.x400_ms_recipaddintparam(recip_obj, 
	    X400_att.X400_N_ORIGINAL_RECIPIENT_NUMBER, rno);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_recipaddintparam failed " + status);
	    return status;
	}

//    	status = com.isode.x400api.X400ms.x400_ms_recipaddstrparam(recip_obj, 
//	    X400_att.X400_S_ORIGINATOR_REQUESTED_ALTERNATE_RECIPIENT, 
//	    	alt_recip, -1);
//	if (status != X400_att.X400_E_NOERROR) {
//	    System.out.println("x400_ms_recipaddstrparam failed " + status);
//	    return status;
//	}

	// add some X.400 recip attributes into the recipient 
    	status = com.isode.x400api.X400ms.x400_ms_recipaddintparam(recip_obj, 
	    X400_att.X400_N_NOTIFICATION_REQUEST, 7);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_recipaddstrparam failed " + status);
	    return status;
	}
    	status = com.isode.x400api.X400ms.x400_ms_recipaddintparam(recip_obj, 
	    X400_att.X400_N_REPLY_REQUESTED, 1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_recipaddstrparam failed " + status);
	    return status;
	}
    	status = com.isode.x400api.X400ms.x400_ms_recipaddstrparam(recip_obj, 
	    X400_att.X400_S_FREE_FORM_NAME, recip, recip.length());
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_recipaddstrparam failed " + status);
	    return status;
	}
	
    	status = com.isode.x400api.X400ms.x400_ms_recipaddstrparam(recip_obj, 
	    X400_att.X400_S_TELEPHONE_NUMBER, phone_num, phone_num.length());
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_recipaddstrparam failed " + status);
	    return status;
	}

        if (config.mt_use_p772 == true) {
            /* Add P772 per-recipient extensions */
            System.out.println("Adding STANAG 4406 A2.1 ACP127 notification request");
             
            /* STANAG 4406 A2.1 ACP127 notification request */
            status = com.isode.x400api.X400.x400_recipaddintparam(recip_obj, 
                      X400_att.X400_N_ACP127_NOTI_TYPE,
                      X400_att.X400_ACP127_NOTI_TYPE_NEG);
            if (status != X400_att.X400_E_NOERROR) {
                System.out.println("x400_recipaddintparam X400_N_ACP127_NOTI_TYPE failed " + status);
                return status;
            }
            
            /* STANAG 4406 A2.1 ACP127 notification request */
             status = com.isode.x400api.X400.x400_recipaddintparam(recip_obj, 
                      X400_att.X400_N_ACP127_NOTI_TYPE,
                      X400_att.X400_ACP127_NOTI_TYPE_POS);
            if (status != X400_att.X400_E_NOERROR) {
                System.out.println("x400_recipaddintparam X400_N_ACP127_NOTI_TYPE failed " + status);
                return status;
            }

            /* STANAG 4406 A2.1 ACP127 notification request */
            status = com.isode.x400api.X400.x400_recipaddintparam(recip_obj, 
                      X400_att.X400_N_ACP127_NOTI_TYPE,
                      X400_att.X400_ACP127_NOTI_TYPE_TRANS);
            if (status != X400_att.X400_E_NOERROR) {
                System.out.println("x400_recipaddintparam X400_N_ACP127_NOTI_TYPE failed " + status);
                return status;
            }

            status = add_acp127resp(recip_obj);
            if (status != X400_att.X400_E_NOERROR) {
                System.out.println("add_acp127resp " + status);
                return status;
            }
                                    
        }
        
	System.out.println("Added recipient " + recip + "to MS Message");
	return status;
    }

    /**
     * Build the envelope for a message for submission into the Message Store.
     */
    private static int build_ms_env(MSMessage msmessage_obj)
    {
	int status;
	// String hostname = "attlee";
	String orig_ret_addr = "/CN=origretr/OU=lppt/O=" 
	    + config.hostname + "/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
	String msg_id = "/PRMD=TestPRMD/ADMD=TestADMD/C=GB/;" 
	    + config.hostname + ".2810401-030924.140212";
	String  content_id = "030924.140212";
	String latest_del_time = "090927120000Z";


	// X400_N_CONTENT_LENGTH is probe only 

	// Priority: 0 - normal, 1 - non-urgent, 2 - urgent 
    	status = com.isode.x400api.X400ms.x400_ms_msgaddintparam(msmessage_obj, 
	    X400_att.X400_N_PRIORITY, 2);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddintparam failed " + status);
	    return status;
	}

	// Disclosure of recipients: 0 - no, 1 - yes 
    	status = com.isode.x400api.X400ms.x400_ms_msgaddintparam(msmessage_obj, 
	    X400_att.X400_N_DISCLOSURE, 1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddintparam failed " + status);
	    return status;
	}
     
        
	//  Implicit conversion prohibited: 0 - no, 1 - yes
    	status = com.isode.x400api.X400ms.x400_ms_msgaddintparam(msmessage_obj, 
	    X400_att.X400_N_IMPLICIT_CONVERSION_PROHIBITED, 1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddintparam failed " + status);
	    return status;
	}

	// Alternate recipient allowed: 0 - no, 1 - yes
    	status = com.isode.x400api.X400ms.x400_ms_msgaddintparam(msmessage_obj, 
	    X400_att.X400_N_ALTERNATE_RECIPIENT_ALLOWED, 1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddintparam failed " + status);
	    return status;
	}

	// Content return request: 0 - no, 1 - yes
    	status = com.isode.x400api.X400ms.x400_ms_msgaddintparam(msmessage_obj, 
	    X400_att.X400_N_CONTENT_RETURN_REQUEST, 1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddintparam failed " + status);
	    return status;
	}

	// Recipient reassignment prohibited: 0 - no, 1 - yes
    	status = com.isode.x400api.X400ms.x400_ms_msgaddintparam(msmessage_obj, 
	    X400_att.X400_N_RECIPIENT_REASSIGNMENT_PROHIBITED, 1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddintparam failed " + status);
	    return status;
	}

	// Distribution List expansion prohibited: 0 - no, 1 - yes
    	status = com.isode.x400api.X400ms.x400_ms_msgaddintparam(msmessage_obj, 
	    X400_att.X400_N_DL_EXPANSION_PROHIBITED, 1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddintparam failed " + status);
	    return status;
	}

	// Conversion with loss prohibited: 0 - no, 1 - yes
    	status = com.isode.x400api.X400ms.x400_ms_msgaddintparam(msmessage_obj, 
	    X400_att.X400_N_CONVERSION_WITH_LOSS_PROHIBITED, 1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddintparam failed " + status);
	    return status;
	}

	// Message Identifier. In RFC 2156 String form
    	status = com.isode.x400api.X400ms.x400_ms_msgaddstrparam(msmessage_obj, 
	    X400_att.X400_S_MESSAGE_IDENTIFIER, msg_id, msg_id.length());
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddintparam failed " + status);
	    return status;
	}

	// Content Identifier
    	status = com.isode.x400api.X400ms.x400_ms_msgaddstrparam(msmessage_obj, 
	    X400_att.X400_S_CONTENT_IDENTIFIER, content_id, 
	    content_id.length());
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddintparam failed " + status);
	    return status;
	}

	//  Latest Delivery Time: UTCTime format YYMMDDHHMMSS<zone>
    	status = com.isode.x400api.X400ms.x400_ms_msgaddstrparam(msmessage_obj, 
	    X400_att.X400_S_LATEST_DELIVERY_TIME, latest_del_time, 
	    latest_del_time.length());
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddintparam failed " + status);
	    return status;
	}

	//  Originator Return Address (X.400 String format)
    	status = com.isode.x400api.X400ms.x400_ms_msgaddstrparam(msmessage_obj, 
	    X400_att.X400_S_ORIGINATOR_RETURN_ADDRESS, orig_ret_addr, 
	    orig_ret_addr.length());
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddintparam failed " + status);
	    return status;
	}
	return status;

    }

    /**
     * Build the content for a message for submission into the Message Store.
     */
    private static int build_ms_content(Session session_obj, String oraddress, MSMessage msmessage_obj)
    {
	int status;
	int num_atts = 0;
	String subject = "Test message from Java";
	String content_ia5= "IA5 content from Java\r\nLine 1\r\nLine 2";
	String content_ia5_att = "IA5 att content from Java\r\nLine 1\r\nLine 2";
	String content_8859_1 = "8859_1 content from Java\r\nLine 1\r\nLine 2\u00a3\u00a3";
	String content_8859_2 = "8859_2 content from Java\r\nLine 1\r\nLine 2\u00a3\u00a3";
	String content_bin = "binary content from Java\r\nLine 1\r\nLine 2\u00a3\u00a3";
	String content_ftbp = "ftbp content from Java\r\nLine 1\r\nLine 2\u00a3\u00a3";
	byte[] emptybinarydata = new byte[0];
	/* test binary data: note the NULs near the end */
	byte[] binarydata = new byte[] {98,105,110,97,114,121,32,99,111,110,116,101,110,116,32,102,114,111,109,32,74,97,118,97,13,10,76,105,110,101,32,49,13,10,76,105,110,101,32,50,0,97};

	String ipm_id = "1064400656.24922*";
	String ipm_rep_id = "1064400656.24923*";
	String ipm_obs_id = "1064400656.24924*";
	String ipm_rel_id = "1064400656.24925*";
	String orig_ref = "orig-ref-val";
	String def_utc = "050924120000";

	int importance = 2;
	int sensitivity = 3;
	int autoforwarded = 1;

    	status = com.isode.x400api.X400ms.x400_ms_msgaddstrparam(msmessage_obj, 
	    X400_att.X400_S_IPM_IDENTIFIER, ipm_id, ipm_id.length());
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddstrparam failed " + status);
	    return status;
	}

    	status = com.isode.x400api.X400ms.x400_ms_msgaddstrparam(msmessage_obj, 
	    X400_att.X400_S_SUBJECT, subject, -1);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddstrparam failed " + status);
	    return status;
	}

    	status = com.isode.x400api.X400ms.x400_ms_msgaddstrparam(msmessage_obj, 
	    X400_att.X400_S_REPLIED_TO_IDENTIFIER, ipm_rep_id, 
	    ipm_rep_id.length());
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddstrparam failed " + status);
	    return status;
	}

    	status = com.isode.x400api.X400ms.x400_ms_msgaddstrparam(msmessage_obj, 
	    X400_att.X400_S_OBSOLETED_IPMS, ipm_obs_id, 
	    ipm_obs_id.length());
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddstrparam failed " + status);
	    return status;
	}

    	status = com.isode.x400api.X400ms.x400_ms_msgaddstrparam(msmessage_obj, 
	    X400_att.X400_S_RELATED_IPMS, ipm_rel_id, ipm_rel_id.length());
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddstrparam failed " + status);
	    return status;
	}

    	status = com.isode.x400api.X400ms.x400_ms_msgaddstrparam(msmessage_obj, 
	    X400_att.X400_S_EXPIRY_TIME, def_utc, def_utc.length());
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddstrparam failed " + status);
	    return status;
	}

    	status = com.isode.x400api.X400ms.x400_ms_msgaddstrparam(msmessage_obj, 
	    X400_att.X400_S_REPLY_TIME, def_utc, def_utc.length());
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddstrparam failed " + status);
	    return status;
	}

    	status = com.isode.x400api.X400ms.x400_ms_msgaddintparam(msmessage_obj, 
	    X400_att.X400_N_IMPORTANCE, importance);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddintparam failed " + status);
	    return status;
	}

    	status = com.isode.x400api.X400ms.x400_ms_msgaddintparam(msmessage_obj, 
	    X400_att.X400_N_SENSITIVITY, sensitivity);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddintparam failed " + status);
	    return status;
	}

    	status = com.isode.x400api.X400ms.x400_ms_msgaddintparam(msmessage_obj, 
	    X400_att.X400_N_AUTOFORWARDED, autoforwarded);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddintparam failed " + status);
	    return status;
	}

	// now add the attachments/bodyparts

	// Add an IA5 attachment using AddStrParam
    	status = com.isode.x400api.X400ms.x400_ms_msgaddstrparam(msmessage_obj, 
	    X400_att.X400_T_IA5TEXT, content_ia5, content_ia5.length());
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddstrparam failed " + status);
	    return status;
	}
	num_atts++;

	// Add an IA5 attachment
    	status = com.isode.x400api.X400ms.x400_ms_msgaddattachment(
	    msmessage_obj, X400_att.X400_T_IA5TEXT, content_ia5_att, 
	    content_ia5_att.length(), emptybinarydata);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddattachment failed " + status);
	    return status;
	}
	num_atts++;

	// Add an 8859-1 attachment
    	status = com.isode.x400api.X400ms.x400_ms_msgaddattachment(
	    msmessage_obj, X400_att.X400_T_ISO8859_1, content_8859_1, 
	    content_8859_1.length(), emptybinarydata);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddattachment failed " + status);
	    return status;
	}
	num_atts++;

	// Add an 8859-2 attachment
    	status = com.isode.x400api.X400ms.x400_ms_msgaddattachment(msmessage_obj, 
	    X400_att.X400_T_ISO8859_1, content_8859_2, content_8859_2.length(), emptybinarydata);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddattachment failed " + status);
	    return status;
	}
	num_atts++;

	// add a forwarded msg attachment
	// instantiate a message object putting it into an API object 
	Message fwd_message_obj = new Message();
	String fwd_recip_oraddress = config.fwd_recip_oraddress;
	//"/CN=lppt2/OU=lppt/O=" 
	    //+ config.hostname + "/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";

	System.out.println("Building message for message bodypart");
	status = X400BuildFwdMsg.build_fwd_msg(fwd_message_obj, 
	    fwd_recip_oraddress);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("build_fwd_msg failed " + status);
	    return status;
	}

	// add the X400Message as a bodypart to the X400msMessage
	status = com.isode.x400api.X400ms.x400_ms_msgaddmessagebody(
	    msmessage_obj, fwd_message_obj); 
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_addmessagebody failed " + status);
	    return status;
	} 
	num_atts++;

        // Add an bin attachment
        status = com.isode.x400api.X400ms.x400_ms_msgaddattachment(
            msmessage_obj, X400_att.X400_T_BINARY, content_bin,
            content_bin.length(), binarydata);
        if (status != X400_att.X400_E_NOERROR) {
            System.out.println("x400_ms_msgaddattachment failed (bin)" + status);
            return status;
        }
        num_atts++;


        /* Now add P772 bodyparts */
        if (config.ms_use_p772 == true) {
            status = add_adatp3 (msmessage_obj);
            if (status != X400_att.X400_E_NOERROR)
                return status;
            num_atts++;

            status = add_corrections (msmessage_obj);
            if (status != X400_att.X400_E_NOERROR)
                return status;
            num_atts++;
            
            status = add_acp127data (msmessage_obj);
            if (status != X400_att.X400_E_NOERROR)
                return status;
            num_atts++;

            status = add_fwd_enc (msmessage_obj);
            if (status != X400_att.X400_E_NOERROR)
                return status;
            num_atts++;
            
            
            status = add_fwd_MM (msmessage_obj);
            if (status != X400_att.X400_E_NOERROR)
                return status;
            num_atts++;
            
        }

        status = add_x420_fwd_content (msmessage_obj);
        if (status != X400_att.X400_E_NOERROR)
            return status;
        num_atts++;

        
	// record number of attachments
    	status = com.isode.x400api.X400ms.x400_ms_msgaddintparam(msmessage_obj, 
	    X400_att.X400_N_NUM_ATTACHMENTS, num_atts);
	if (status != X400_att.X400_E_NOERROR) {
	    System.out.println("x400_ms_msgaddintparam failed " + status);
	    return status;
	}

	return status;

    }
}

