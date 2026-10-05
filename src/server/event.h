/*
 * This file is internal. It must not be
 * included into enduser application.
 */

#ifndef __EVENT_H
#define __EVENT_H

/*
 * To be called in paint function
 *
 * It calls user defined functions for
 * each item of all elements types.
 */
void ced_do_draw_event(void);

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

#endif /* __EVENT_H  */
