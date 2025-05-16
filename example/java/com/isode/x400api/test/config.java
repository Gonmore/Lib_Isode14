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
 * Config file for Java X.400 API test program
 */

package com.isode.x400api.test;

public class config
{

    /** Global values used across the example application */

    /** This is the hostname. It's used in oraddresses and presentation
     * addresses. It works with a configuration created by
     * x400quick. See the Isode X.400 Quick Install Guide.
     * Change this value to your hostname.
     */
    public static final String hostname = "dhcp-164";
    public static final String domain   = "isode.net";
    public static final String country  = "GB";
    
    
    /* location of sample binary files */
    public static final String sharedir = "/opt/isode/share/x400sdk/example/c/";
    /* location the sample program should use to write binary files */
    public static final String outdir = "/tmp/";
    /* ----------------------------------------------------------------*/
    /** Values used to bind to the p7 Store */
    /** oraddress - makes up orname used to bind to the store */
    public static final String p7_bind_oraddr =
	"/CN=P7User1/OU=Sales/O=" + hostname + "/PRMD=TestPRMD/ADMD=TestADMD/C=" + country + "/";

    /** dn - makes up orname used to bind to the store */
    public static final String p7_bind_dn = "c=eu";
    /** password */
    public static final String p7_credentials = "secret";
    /** presentation address of P7 Message Store */
    public static final String p7_pa 
    	= "\"3001\"/Internet=" + hostname + "." + domain + "+3001"; 
    public static final boolean use_opencheck = true;
    public static final String p7_ret_psw = "secret";

    /* ----------------------------------------------------------------*/
    /* These are the values when binding to the P3 server */
    /** oraddress - makes up orname used to bind to the store */
    public static final String p3oraddr = "/CN=P3User1/OU=Sales/O=" + hostname + "/PRMD=TestPRMD/ADMD=TestADMD/C=" + country + "/";
    /** dn - makes up orname used to bind to the store */
    public static final String p3dn = "c=eu";
    /** password */
    public static final String p3credentials = "p3secret";
    /** presentation address of P3 MTA channel */
    public static final String p3pa 
	= "\"594\"/Internet=" + hostname + "." + domain; 

    /* ----------------------------------------------------------------*/
    /* Gateway user values */
    public static final String mt_rcvoraddr = "/CN=GatewayUser/OU=Sales/O=" + hostname + "/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
    public static final String mt_sendoraddr = "/CN=GatewayUser/OU=Sales/O=" + hostname + "/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
    public static final String mt_sendaltrecip = "/cn=SendAltUser/O=GatewayMTA/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
    public static final String mt_origretaddr = "/cn=SendUser/O=GatewayMTA/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
    public static final String x400mt_chan_name = "x400mt";
    public static final String mt_phone_num = "02087830203";

    public static final String mt_redi_time1 = "080427120000Z";
    public static final String mt_redi_or1 = "/CN=redihist1/O=GatewayMTA/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
    public static final String mt_redi_dn1 = "cn=redihist1,c=gb";
     public static final String mt_redi_time2 = "080527120000Z";
    public static final String mt_redi_or2 = "/CN=redihist2/O=GatewayMTA/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
    public static final String mt_redi_dn2 = "cn=redihist2,c=gb";
    
    /* Gateway DLexp hist values */
    public static final String mt_dlexp_or1 = "/cn=dlexphist1/O=GatewayMTA/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
    public static final String mt_dlexp_dn1 = "cn=dlexphisty1,c=gb";
    public static final String mt_dlexp_exp_time1 = "120801120000+0100";
    public static final String mt_dlexp_or2 = "/cn=dlexphist2/O=GatewayMTA/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
    public static final String mt_dlexp_dn2 = "cn=dlexphisty2,c=gb";
    public static final String mt_dlexp_exp_time2 = "120801120000+0100";
    
    /* Gateway probe values */
    public static final String mt_prb_id = "/PRMD=TestPRMD/ADMD=TestADMD/C=GB/;" + hostname +".2810401-030924.140212";
    public static final int mt_prb_resp = 1;
    public static final int mt_prb_dlexp = 1;
    public static final int mt_prb_rep_req = 0;
    public static final int mt_prb_mta_rep_req = 0;
    public static final int mt_prb_alt_recip_allowed = 0;
    public static final int mt_prb_recip_res_prohib = 0;
    public static final int mt_prb_imp_conv_prohib = 0;
    public static final int mt_prb_conv_with_loss_prohib = 0;
    public static final String mt_prb_OEITS = "ia5-text";
    public static final int mt_prb_cont_type = 22;
    //public static final String mt_prb_external_type = "1.3.26.0.4406.0.4.1";
    public static final String mt_prb_external_type = "";
    public static final String mt_prb_cont_id = "081024.140212";
    public static final String mt_prb_cont_corr = "ABCDEFG";
    public static final int mt_prb_cont_length = 100;

    /* Gateway report values */
    public static final String mt_rep_id =  "/cn=repid/PRMD=TestPRMD/ADMD=TestADMD/C=GB/;" + hostname +".2810401-030924.140212";
    public static final String mt_rep_orig_name ="/cn=reporig/O=GatewayMTA/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
    public static final String mt_rep_dest_name ="/cn=RepRecUser/O=GatewayMTA/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
    public static final String mt_rep_msg_id = "/cn=repmsgid/PRMD=TestPRMD/ADMD=TestADMD/C=GB/;" + hostname +".2810401-030924.140212";
    public static final String mt_rep_sub_id = "/cn=subid/PRMD=TestPRMD/ADMD=TestADMD/C=GB/;" + hostname +".2810401-030924.140212";
    public static final String mt_rep_arr_time = "080927120000Z";
    public static final String mt_rep_dl_name = "/cn=repdlname/O=GatewayMTA/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
    public static final String mt_rep_CEIT = "ia5-text";
    public static final String mt_rep_sup_info = "This is supplementary info";
    public static final String mt_rep_OEIT = "ia5-text";
    public static final int mt_rep_cont_type = 22;
    //public static final String mt_rep_external_type = "1.3.26.0.4406.0.4.1";
    public static final String mt_rep_external_type = "";
    public static final String mt_rep_cont_id = "911024.140212";
    public static final String mt_rep_cont_corr = "ABCDEF";
    public static final String mt_rep_msg_del_time = "080827120000Z";
    public static final int mt_rep_non_del_reason = 0;
    public static final int mt_rep_non_del_diag = 0;
    public static final int mt_rep_dr = 1; /* 1 = DR, 0 = NDR */
    public static final String mt_rep_oranddl_exap_time1 = "071121125704Z";
    public static final String mt_rep_oranddl_exap_dn1 = "CN=origandlorig,c=GB";
    public static final String mt_rep_oranddl_exap_or1 = "/cn=origandlorig/prmd=TestPRMD/admd=TestPRMD/C=gb/";
    
    /* Gateway Message values */
    public static final String mt_msg_subj = "Test message from Java";
    public static final String mt_msg_cnt_ia5 = "IA5 content from Java\r\nLine 1\r\nLine 2";
    public static final String mt_msg_cnt_ia5_att = "IA5 att content from Java\r\nLine 1\r\nLine 2";
    public static final String mt_msg_cnt_8859_1 = "8859_1 content from Java\r\nLine 1\r\nLine 2\u00a3\u00a3";
    public static final String mt_msg_cnt_8859_2 = "8859_2 content from Java\r\nLine 1\r\nLine 2\u00a3\u00a3";
    public static final String mt_msg_cnt_bin = "binary content from Java\r\nLine 1\r\nLine 2\u00a3\u00a3";
    public static final byte[] mt_msg_cnt_emptybinarydata = new byte[0];
    public static final byte[] mt_msg_cnt_binarydata = new byte[] {98,105,110,97,114,121,32,99,111,110,116,101,110,116,32,102,114,111,109,32,74,97,118,97,13,10,76,105,110,101,32,49,13,10,76,105,110,101,32,50,0,97};
    public static final String mt_msg_ipm_id = "1064400656.24922*";
    public static final String mt_msg_ipm_rep_id = "1064400656.24922*";
    public static final String mt_msg_ipm_obs_id  = "1064400656.24922*";
    public static final String mt_msg_ipm_rel_id  = "1064400656.24922*";
    public static final String mt_msg_def_utc = "050924120000";
    public static final int mt_msg_importance = 2;
    public static final int mt_msg_sensitivity = 3;
    public static final int mt_msg_autoforwarded = 1;
    public static final String mt_msg_id = "/PRMD=TestPRMD/ADMD=TestADMD/C=GB/;"  + config.hostname + ".2810401-030924.140212";
    public static final String mt_msg_content_id = "030924.140212";
    public static final String mt_msg_latest_del_time = "120927120000Z";
    public static final String mt_msg_def_del_time = "080927120000Z";
   
    public static final String mt_msg_traceinfo1_GDI= "/PRMD=GDI1/ADMD=TestADMD/C=GB/";
    public static final String mt_msg_traceinfo1_AT= "070927120000Z";
    public static final String mt_msg_traceinfo1_AD= "/PRMD=attempteddomain1/ADMD=TestADMD/C=GB/";
    public static final String mt_msg_traceinfo1_AA_DEF_TIME= "080427120000Z";
    public static final String mt_msg_traceinfo1_AA_CEIT= "ia5-text";
    public static final int mt_msg_traceinfo1_DSI_RA= 1;
    public static final int mt_msg_traceinfo1_DSI_redirected= 0;
    public static final int mt_msg_traceinfo1_DSI_AA_DLOP= 0;
    public static final String mt_msg_dl_exempted_addr = "/CN=dlexempted/O=GatewayMTA/PRMD=TestPRMD/ADMD=TestADMD/C=GB/";
    public static final String mt_msg_cont_corr = "ABCDEFG";
    public static final String mt_msg_inttraceinfo1_GDI= "/PRMD=intGDI1/ADMD=TestADMD/C=GB/";
    public static final String mt_msg_inttraceinfo1_mtaname= "/cn=mtaname/PRMD=intGDI1/ADMD=TestADMD/C=GB/";
    public static final String mt_msg_inttraceinfo1_SItime = "080427120000Z";
    public static final String mt_msg_inttraceinfo1_SIattempted_MTA = "/cn=attemptedmta/PRMD=intGDI1/ADMD=TestADMD/C=GB/";
    public static final String mt_msg_inttraceinfo1_SIdeferred_time ="070427120000Z";
    public static final String mt_msg_inttraceinfo1_SICEIT ="ia5-text";
    
    
    
    
    /* ----------------------------------------------------------------*/
    /* Gateway P772 Attributes */
    public static final boolean mt_use_p772 = true;
    public static final int mt_codress = 0;
    public static final int mt_prim_prec = 0;
    public static final int mt_copy_prec = 0;
    public static final int mt_msg_type = 0;
    public static final int mt_noti_type = 0;
    public static final String mt_ext_auth_info = "ext auth info";
    public static final String mt_acp127_msg_id = "ACP127msgid";
    public static final String mt_orig_plad = "origplad";
    public static final String mt_ea_or_address = "/cn=eadn/PRMD=intGDI1/ADMD=TestADMD/C=GB/";
    public static final String mt_ea_dn_address = "cn=eadn,c=gb";
    public static final String mt_ea_ffn = "An exempted recipient";
    public static final String mt_ea_tel = "0123456789";
    

    public static final String mt_handling_instruction1 = "Handling instruction 1";
    public static final String mt_handling_instruction2 = "Handling instruction 2";
    public static final String mt_message_instruction1 = "Message instruction 1";
    public static final String mt_message_instruction2 = "Message instruction 2";
    public static final String mt_distcode1 = "Distcode 1";
    public static final String mt_distcode2 = "Distcode 2";
    public static final String mt_orig_ref = "orig ref1";


    public static final String mt_ali_or1 = "/cn=alior/PRMD=intGDI1/ADMD=TestADMD/C=GB/";
    public static final String mt_ali_dn1 = "cn=eadn,c=gb";
    public static final String mt_ali_free_form_name1 = "ali free form";
    public static final String mt_ali_tel1 = "0123456789";

    public static final int mt_ali_type1 = 1;
    public static final int mt_ali_noti_request1 = 1;
    public static final int mt_ali_reply_request1 = 1;

    public static final String mt_otherrecip_str = "Other recip";
    public static final int mt_otherrecip_int = 1;

    public static final String mt_acp127resp_resp_time = "070427120000Z";
    public static final String mt_acp127resp_resp_recip = "ACP127 Recipient";
    public static final String mt_acp127resp_supp_info  = "ACP127 sup info";

    public static final String mt_distfieldoid1 = "1.2.3.4";
    public static final byte[] mt_distfieldvalue1 = new byte [] {0x02, 0x01, 0x0a};
    public static final String mt_distfieldoid2 = "1.2.3.5";

    public static final byte[] mt_distfieldvalue2 = new byte [] {0x13, 0x0a, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,0x6a};
    public static final String adatp3_line1 = "ADatP3 line 1\r\nADatP3 line 2";

    public static final String mt_correction = "A simple correction";
    public static final String mt_acp127data = "ACP127 data";
    
    public static final String mt_msg_external_type = "1.3.26.0.4406.0.4.1";


    /* MS P772 Attributes */
    public static final boolean ms_use_p772 = true;
    public static final int ms_codress = 0;
    public static final int ms_prim_prec = 0;
    public static final int ms_copy_prec = 0;
    public static final int ms_msg_type = 0;
    public static final int ms_noti_type = 0;
    public static final String ms_ext_auth_info = "ext auth info";
    public static final String ms_acp127_msg_id = "ACP127msgid";
    public static final String ms_orig_plad = "origplad";
    public static final String ms_ea_or_address = "/cn=eadn/PRMD=intGDI1/ADMD=TestADMD/C=GB/";
    public static final String ms_ea_dn_address = "cn=eadn,c=gb";
    public static final String ms_ea_ffn = "An exempted recipient";
    public static final String ms_ea_tel = "0123456789";
    

    public static final String ms_handling_instruction1 = "Handling instruction 1";
    public static final String ms_handling_instruction2 = "Handling instruction 2";
    public static final String ms_message_instruction1 = "Message instruction 1";
    public static final String ms_message_instruction2 = "Message instruction 2";
    public static final String ms_distcode1 = "Distcode 1";
    public static final String ms_distcode2 = "Distcode 2";
    public static final String ms_orig_ref = "orig ref1";


    public static final String ms_ali_or1 = "/cn=alior/PRMD=intGDI1/ADMD=TestADMD/C=GB/";
    public static final String ms_ali_dn1 = "cn=eadn,c=gb";
    public static final String ms_ali_free_form_name1 = "ali free form";
    public static final String ms_ali_tel1 = "0123456789";

    public static final int ms_ali_type1 = 1;
    public static final int ms_ali_noti_request1 = 1;
    public static final int ms_ali_reply_request1 = 1;

    public static final String ms_otherrecip_str = "Other recip";
    public static final int ms_otherrecip_int = 1;

    public static final String ms_acp127resp_resp_time = "070427120000Z";
    public static final String ms_acp127resp_resp_recip = "ACP127 Recipient";
    public static final String ms_acp127resp_supp_info  = "ACP127 sup info";

    public static final String ms_distfieldoid1 = "1.2.3.4";
    public static final String ms_distfieldvalue1 = "Some dist field value1";
    public static final String ms_distfieldoid2 = "1.2.3.5";
    public static final String ms_distfieldvalue2 = "Some dist field value2";


    public static final String ms_correction = "A simple correction";
    public static final String ms_acp127data = "ACP127 data";
    
    public static final String ms_msg_external_type = "1.3.26.0.4406.0.4.1";
    
    
    /* ----------------------------------------------------------------*/
    /** size of byte arrays for binary data */
    public static final int maxlen = 32000;

    public static final boolean building_simple_message = false;

    /* ----------------------------------------------------------------*/
    /** default security environment */
    public static final boolean building_signed_message = true;
    public static final boolean verify_signed_message = true;

    // public static final String sec_id "c:/isode/share";
    public static final String sec_id 	 = "/opt/isode/share";
    public static final String sec_id_dn = "cn=AMHS Tester, cn=isode, c=GB";
    public static final String sec_pphr  = "secret";

    /* ----------------------------------------------------------------*/
    /** values for message to be attached as forwarded message */
    public static final String fwd_recip_oraddress = 
    	"/CN=forwarded msg recip/O=" + hostname + 
	"/PRMD=TestPRMD/ADMD=TestADMD/C=" + country  + "/";

}

