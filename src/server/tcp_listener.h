/*
 * This file is internal. It must not be
 * included into enduser application.
 */

#ifndef __TCP_LISTENER_H
#define __TCP_LISTENER_H

/*
 * Server side function.
 * Start listening for a client on the given port.
 *
 * user_func is called with the received message
 * for each incoming message from the client, and
 * with NULL when a new client has connected.
 * Pass the message to ced_process_input().
 *
 * Returns 0 on success, -1 if the port can't be
 * bound or listened on.
 *
 * Example:
 *      tcp_server(7285,my_process_input)
 */
int tcp_server(unsigned short port, void (*user_func)(void *data));

#endif /* __TCP_LISTENER_H  */
