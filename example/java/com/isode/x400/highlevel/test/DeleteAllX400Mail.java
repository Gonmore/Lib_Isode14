/* 
 * Copyright (c) 2008-2009, Isode Limited, London, England.
 * All rights reserved.
 * 
 * Acquisition and use of this software and related materials for any
 * purpose requires a written licence agreement from Isode Limited,
 * or a written licence from an organisation licenced by Isode Limited
 * to grant such a licence.
 */

package com.isode.x400.highlevel.test;

import java.util.ArrayList;

import com.isode.x400.highlevel.ListResult;
import com.isode.x400.highlevel.P7BindSession;
import com.isode.x400.highlevel.ReceiveMsg;
import com.isode.x400.highlevel.X400APIException;


/**
 * Test program that connects to either a P7 Message Store, fetches all the messages (without
 * showing them or doing anything with them) and then deletes them.
 * 
 */

public class DeleteAllX400Mail {


	// Mandatory P7 configuration settings. You must change these values for this program to work with P7
	private static String p7_message_store_presentation_address = "\"3001\"/Internet=nova.isode.net+3001";
	private static String p7_user_or_address = "/CN=P7User1/OU=Sales/O=nova/PRMD=Isode/ADMD= /C=GB/";
	private static String p7_user_password = "secret";

	public static void main(String[] args) {

		try {

			System.out.println("Connecting to the P7 Message Store");

			// Create a new P7 bind session object with the session values necessary for the bind
			P7BindSession connection = new P7BindSession(p7_message_store_presentation_address,
					p7_user_or_address, p7_user_password);

			// It is possible to request a the number of unread messages at the time of bind (SUMMARIZE)
			// If you are interested in this result, set SetSummarizeOnBind(true), otherwise
			// don't set it, as the operation isn't needed.
			// connection.getSession().SetSummarizeOnBind(true);

			// Bind to the P7 Message Store
			connection.bind();


			// List messages older than the value of "since", or all messages if "since" is null
			// The "since" string is formatted in UTC time, for example, 02 October 2008 at 14:15:16 hours  
			// is 081002141516
			String since = null;

			// The P7 Message Store can store two kinds of messages: submitted messages are the ones
			// that the user submitted to the Message Store (for submission into the MTA)
			// Stored messages are selected by setting entry_class to MS_ENTRY_CLASS_STORED_MESSAGES.
			// Submitted messages are selected by setting entry_class to MS_ENTRY_CLASS_SUBMITTED_MESSAGES.
			// It is not possible to specify a value that return both of them, you have to choose
			P7BindSession.Entry_Class entry_class = P7BindSession.Entry_Class.MS_ENTRY_CLASS_STORED_MESSAGES;

			// Messages in the Message Store can be in one of three status:
			// 1 = new -
			// 2 = listed - 
			// 3 = fetched 
			boolean only_new_messages = false;

			ArrayList<ListResult> list_array = connection.listMailbox(since, entry_class, only_new_messages);
			System.out.println("There are " + list_array.size() + " messages in the Inbox");

			//try {Thread.sleep(10000);} catch (InterruptedException e) {}

			ReceiveMsg rm;
			for (int i=1; i<list_array.size(); i++) {
				ListResult lm = list_array.get(i);
				System.out.println("Deleting message number " + i);
				rm = new ReceiveMsg(connection, lm.getSequenceNumber());
				connection.deleteMessage(rm);
			}

			// Unbind cleanly from the Message Store
			connection.unbind();


		} catch (X400APIException e) {

			// Catch all exceptions, report it and exit the program
			e.printStackTrace();
			System.exit(1);
		}
		
		
	}
}
