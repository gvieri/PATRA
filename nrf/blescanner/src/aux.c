/* main.c - Application main entry point */

/*
 * Copyright (c) 2021 Giovambattista Vieri
 *
 * License: Apache-2.0
 *
 * Note: I borrowed some code from zephyr/example 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <zephyr/types.h>
#include <stddef.h>
#include <errno.h>
#include <zephyr.h>
#include <sys/printk.h>

#include <settings/settings.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <bluetooth/addr.h>
#include <sys/byteorder.h>
#include <bluetooth/direction.h>


#define DEVICE_NAME     CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)
#define NAME_LEN        30
#define TIMEOUT_SYNC_CREATE K_SECONDS(10)


char *print_hex(const char *buffer, int len)
{
        unsigned char *p = (unsigned char *) buffer;
	char *dbuffer,*pbuffer; 
	printk("called print_hex with len = %d\n", len);
	pbuffer= dbuffer = malloc(len*6); 
	if(dbuffer==NULL) {
	       printk("memory allocation error!");
	       
        }	       
        for (int i=0; i < len; ++i) {
                if (! (i % 16) && i)
                        sprintf(pbuffer++,"\n");

                sprintf(pbuffer,"0x%02x ", p[i]);

//                printk("0x%02x ", p[i]);

		pbuffer += 5; 
        }
        sprintf(pbuffer++,"\n\n");
        return(dbuffer);
}


char *print_asc(const char *buffer, int len)
{
        unsigned char *p = (unsigned char *) buffer;
	char *dbuffer,*pbuffer; 
	pbuffer= dbuffer = malloc(len*6); 
	printk("called print_asc with len = %d\n", len);
	if(dbuffer==NULL) {
	       printk("memory allocation error!");
	       
        }	       
        for (int i=0; i < len; ++i) {
/*                if (! (i % 16) && i)
                        sprintf(pbuffer++,"\n");
*/		
		if((p[i]>=32) && (p[i]<127)) {
                	sprintf(pbuffer,"%c ", p[i]);
//                	printk("%d ", p[i]);
		} else {
                	sprintf(pbuffer,"%c ", ' ');
		}
		pbuffer++;
        }
//        sprintf(pbuffer++,"\n\n");
        return(dbuffer);
}

