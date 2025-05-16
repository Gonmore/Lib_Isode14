/*  Copyright (c) 2003-2008, Isode Limited, London, England.
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
/*! \file amhs_att.h
 *  \brief AMHS Symbolic Constants
 */

#ifndef _AMHS_ATT_H
#define _AMHS_ATT_H


#include "x400_att.h"

/*! \defgroup aftn AFTN Attributes
    These attributes map to different X400 Attributes depending on the
    environment.
    Note that if you are using the basic message service (i.e. ATS_N_EXTENDED is not set, or is set to FALSE) you must supply all of the mandatory ATS attributes - ATS-S_PRIORITY_INDICATOR, ATS_S_FILING_TIME and ATS_S_TEXT as a set.
    \{*/

#define ATS_S_PRIORITY_INDICATOR		  1001
/*!< Two letter ATS Priority Indicator. This affects both the envelope
  priority and IPM properties. Write only.
*/

#define ATS_S_FILING_TIME			  1002
/*!< Filing time: specified as 6 digit string: DDHHMM (in UTC timezone) */

#define ATS_S_OPTIONAL_HEADING_INFO		  1003
/*!< Optional Heading Information, ASCII string */

#define ATS_S_TEXT                               1004
/*!< Text part of ATS message */

#define ATS_N_EXTENDED                           1005
/*!< Boolean value. If TRUE then the message is for the extended
     ATS Message Service.
 */

#define ATS_EOH_MODE 1006
/*!< Integer value. Configures what end-of-header indicator to
     use. If not specified, will default to using the
     amended end-of-heading-blank-line (CRLF) for Outlook compatibility,
     but can also be set to use original mode (LF-only) or
     no-end-of-heading (PDR M3040002) mode.
*/

/* \} */

/*! \defgroup aftneoh AFTN EOH Settings
  \{ */
#define ATS_EOH_MODE_CRLF 1
/*!< Use <CR><LF> as end-of-ats-heading indicator */

#define ATS_EOH_MODE_LF 2
/*!< Use <LF> as end-of-ats-heading indicator */

#define ATS_EOH_MODE_NONE 3
/*!< No end-of-ats-heading indicator (PDR M3040002) */

/* \} */


#endif /* _X400_ATT_H */
