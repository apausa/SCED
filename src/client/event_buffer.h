/*
 * This file is internal. It must not be
 * included into enduser application.
 */

#ifndef __CED_H
#define __CED_H

#include <sced_types.h>

//#ifdef __cplusplus
// extern "C" {
//#endif
		

//char trusted_hosts[50];
//extern static char testchar;

// the event being built (client) or received (server)
extern ced_event eve;

void ced_reset(void);

// Allocate the data of an element for count items, HDR_SIZE bytes are reserved before them.
void ced_buf_alloc(ced_element *pe,unsigned count);


/*
 * Register new element type. Order is important!
 * Appropriate code must be defined in both
 * client and server parts.
 *
 *  item_size - size of one item in bytes.
 *  draw_func - function to call to draw one item,
 *              called from ced_do_draw_event()
 *              not used on client side.
 */
unsigned ced_register_element(unsigned item_size,ced_draw_cb draw_func);

/*
 * To be called from element functions
 * on client side.
 * Return allocated space for one item
 * with size, specified by ced_register_element()
 *
 * Example: assume struct Dummy { int i; }; is item.
 *
 *          DummyID=ced_register_element(sizeof(struct Dummy),0);
 *          ...
 *          struct Dummy *item=(struct Dummy *)ced_add_element(DummyID);
 *            item->i=0;
 * This will add one Dummy item to the event
 */
void *ced_add(unsigned id);

//#ifdef __cplusplus
// }
//#endif

	

#endif /* __CED_H  */
