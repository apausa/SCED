/*
 * This file is internal. It must not be
 * included into enduser application.
 */

#ifndef __EVENT_H
#define __EVENT_H

#include <sced_types.h>

/*
 * Server side function.
 * Must be used to process all incoming
 * messages from client.
 *
 * It return positive value when
 * new event must be drawn.
 *
 * Example:
 *      tcp_server(7285,my_process_input)
 *
 *      my_process_input(x){
 *        if(ced_process_input(x)>0)
 *          <do redraw>
 */
int ced_process_input(void *data);

/*
 * The event on screen: the last one completed
 * by a DRAW_EVENT message. It stays valid until
 * the next DRAW_EVENT is processed.
 */
const ced_event &get_event(void);

#endif /* __EVENT_H  */
