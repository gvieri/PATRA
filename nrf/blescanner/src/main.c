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

#include "aux.h"

#define DEVICE_NAME     CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)
#define NAME_LEN        30
#define TIMEOUT_SYNC_CREATE K_SECONDS(10)


static void start_scan(void);





static bool eir_found(struct bt_data *data, void *user_data)
{
	char *res; 
	printk("[AD]: %u data_len %u\n", data->type, data->data_len);
	switch (data->type) {
	case BT_DATA_UUID16_SOME:
		printk("data->type: %s \n","BT_DATA_UUID16_SOME"); 
		break; 
	case BT_DATA_UUID16_ALL:
		printk("data->type: %s \n","BT_DATA_UUID16_ALL"); 
		break; 
	case BT_DATA_UUID32_SOME:
		printk("data->type: %s \n","BT_DATA_UUID32_SOME"); 
		break; 
	case BT_DATA_UUID32_ALL:
		printk("data->type: %s \n","BT_DATA_UUID32_ALL"); 
		break; 
	

	default:
		printk("data-type value unknown: %u\n", data->type);
	}
	res = print_hex(data->data,data->data_len); 
        if(res) { 
		printk("address is: >%s<\n", res);
	} else {
		printk("null from print_hex");
	}
	free(res);
	res = print_asc(data->data,data->data_len); 
        if(res) { 
		printk("stringified data are: >%s<", res);
	} else {
		printk("null from print_asc");
	}
	free(res);


	return true;
}

/*
-----------------------------------------------------------------
*/

static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type, struct net_buf_simple *ad)
{
	char dev[BT_ADDR_LE_STR_LEN];
	bt_addr_le_to_str(addr, dev, sizeof(dev));
//	printk("----<%s\n",dev);
	if (strncmp(dev, "24:0A:C4:59:63:32",17)==0) {
		printk("[DEVICE]: %s, AD evt type %u, AD data len %u, RSSI %i\n", dev, type, ad->len, rssi);
/*		sprintf(buffer,"0x%02x",		*/
		bt_data_parse(ad, eir_found, (void *)addr);

	}
}


static void start_scan(void)
{
	int err;

	/* Use active scanning and disable duplicate filtering to handle any
	 * devices that might update their advertising data at runtime. */
	struct bt_le_scan_param scan_param = {
		.type       = BT_LE_SCAN_TYPE_ACTIVE,
		.options    = BT_LE_SCAN_OPT_NONE,
		.interval   = BT_GAP_SCAN_FAST_INTERVAL,
		.window     = BT_GAP_SCAN_FAST_WINDOW,
	};

	err = bt_le_scan_start(&scan_param, device_found);
	if (err) {
		printk("Scanning failed to start (err %d)\n", err);
		return;
	}

	printk("Scanning successfully started\n");
}





void main(void)
{
	int err;
	printk("Starting .... "); 

	err = bt_enable(NULL);

	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	printk("Bluetooth initialized\n");

	start_scan();
}
