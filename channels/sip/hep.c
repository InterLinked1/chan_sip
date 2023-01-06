/*
 * Asterisk -- An open source telephony toolkit.
 *
 * Copyright (C) 2022, Naveen Albert
 *
 * See http://www.asterisk.org for more information about
 * the Asterisk project. Please do not directly contact
 * any of the maintainers of this project for assistance;
 * the project provides a web site, mailing lists and IRC
 * channels for your use.
 *
 * This program is free software, distributed under the terms of
 * the GNU General Public License Version 2. See the LICENSE file
 * at the top of the source tree.
 */

/*!
 * \file
 * \brief chan_sip HEP support
 */

/*** MODULEINFO
	<support_level>extended</support_level>
 ***/

#include "asterisk.h"

#include "include/sip.h"
#include "asterisk/res_hep.h"
#include "include/hep.h"

static char *assign_uuid(struct sip_pvt *p)
{
	char *uuid = NULL;
	enum hep_uuid_type uuid_type = hepv3_get_uuid_type();

	if ((uuid_type == HEP_UUID_TYPE_CHANNEL) && p->owner) {
		uuid = ast_strdup(ast_channel_name(p->owner));
	}

	/* If we couldn't get the channel or we never wanted it, default to the call-id */
	if (!uuid) {
		uuid = ast_strdup(p->callid);
	}

	return uuid;
}

int hep_handler(struct sip_pvt *p, struct sip_request *req, int outgoing)
{
	int plen;
	char *packet;
	struct hepv3_capture_info *capture_info;
	char *uuid;
	struct ast_sockaddr *src, *dst;

	packet = ast_str_buffer(req->data);
	plen = ast_str_strlen(req->data);

	capture_info = hepv3_create_capture_info(packet, plen);
	if (!capture_info) {
		ast_debug(1, "Failed to create capture info\n");
		return 0;
	}

	uuid = assign_uuid(p);
	if (!uuid) {
		ast_debug(1, "Failed to create UUID\n");
		ao2_ref(capture_info, -1);
		return 0;
	}

	/* Incoming: src_addr should be peer (sa), dst_addr should be us (ourip)
	 * Outgoing: src_addr should be us (ourip), dst_addr should be peer (sa) */
	src = outgoing ? &p->ourip : &p->sa;
	dst = outgoing ? &p->sa : &p->ourip;
	memcpy(&capture_info->src_addr, src, sizeof(capture_info->src_addr));
	memcpy(&capture_info->dst_addr, dst, sizeof(capture_info->dst_addr));

	capture_info->protocol_id = req->socket.type == AST_TRANSPORT_UDP ? IPPROTO_UDP : IPPROTO_TCP;
	capture_info->capture_time = ast_tvnow();
	capture_info->capture_type = HEPV3_CAPTURE_TYPE_SIP;
	capture_info->uuid = uuid;
	capture_info->zipped = 0;

#if 0
	ast_debug(3, "Sending HEP packet\n");
#endif

	if (hepv3_send_packet(capture_info)) {
		ast_debug(1, "Failed to send HEP packet\n");
		return -1;
	}
	return 0;
}
