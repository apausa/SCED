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

/*
 * Server side function.
 * Send the id of the picked element to the client, as a plain int.
 * Does nothing if no client is connected.
 */
void net_send_selected_id(int id);

/*
 * Server side function.
 * Check, without waiting, if a client wants to connect or has sent
 * something, and handle it. That calls the user_func of tcp_server().
 * To be called once per pass of the main loop.
 */
void net_poll(void);

/*
 * Server side function.
 * Accept connections from this host (a dotted IPv4 address,
 * at most 49 characters) as well as from localhost.
 */
void net_trust_host(const char *ip);

#endif /* __TCP_LISTENER_H  */
