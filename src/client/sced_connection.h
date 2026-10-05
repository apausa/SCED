/* "C" event display.
 * Enduser accessable API (client side): connection and event handling.
 * The elements API is in ced_cli.h.
 *
 * Alexey Zhelezov, DESY/ITEP, 2005
 */

#ifndef __SCED_CONNECTION_H
#define __SCED_CONNECTION_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * This is the first function to call (before any other).
 *
 *  host - host with CED (must be "localhost")
 *  port - server port number (let say 7285 :)
 *
 * NOTE: ced_register_elements() must be called
 *       separately.
 */
void ced_client_init(const char *host,unsigned short port);

/*
 * Cancel current event output. So, all elements
 * queued will be discarded.
 *
 * Good to call at the begining of every event processing.
 */
void ced_new_event(void);


/*
 * This function really attempt to display event in CED.
 * When CED is not available, this function discard
 * current event information.
 *
 * NOTE: between ced_new_event() and ced_draw_event()
 *       must be some element creation calls.
 */
void ced_draw_event(void);

/*
 * This function really attempt to display event in CED.
 * Unlike ced_draw_event() does not reset the event.
 *
 * NOTE: between ced_new_event() and ced_draw_event()
 *       must be some element creation calls.
 */
void ced_send_event(void);

int ced_selected_id(void);

//hauke
int ced_selected_id_noblock(void);


#ifdef __cplusplus
 }
#endif

#endif /* __SCED_CONNECTION_H */
