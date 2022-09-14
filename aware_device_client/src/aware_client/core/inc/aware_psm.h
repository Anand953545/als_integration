/*
 * aware_psm_intf.h
 *
 *  Created on: May28, 2019
 *  
 */

#ifndef AWARE_PSM_H
#define AWARE_PSM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "qapi_psm.h"

typedef struct
{
    bool  is_modem_loaded;             /* Modem is loaded or not.*/
    bool  psm_registered;              /* client registered with PSM or not.*/
    bool  psm_enabled;                 /* PSM enabled from NW/Modem side or not*/
    bool  psm_voted;                   /* client voted for PSM or not*/
    int32_t  client_id;                /* Client id recieved after registering with PSM daemon */
    psm_status_msg_type  psm_msg;      /* Client id recieved after registering with PSM daemon */
    psm_status_type_e  psm_status;     /* PSM status message type. */
    psm_wakeup_type_e psm_wakeup_type;
    unsigned int psm_wakeup_time;      /* time to vote for psm wakeup*/
} aware_psm_context_t;

int aware_psm_init(void);
void aware_psm_deinit(void);
void aware_cancel_vote_for_psm(void);
void aware_vote_for_psm(void);
void aware_compute_psm_time(void);

#endif /* AWARE_PSM_H */

