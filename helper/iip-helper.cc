/*
 *
 * Copyright 2024 Kenichi Yasukata
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "iip-helper.h"

#if defined(NDEBUG)
#undef NDEBUG
#undef assert
#define assert(_c) NS_ASSERT(_c)
#define DEF_ASSERT
#endif

#define NUM_NETSTACK_PB (8192)
#define NUM_NETSTACK_TCP_CONN (512)
#define ETH_RX_BATCH (32)
#define ETH_TX_BATCH (32)

#define APP_IIP_OPS_UTIL_NOW_NS_NONE

#define IOSUB_MAIN_C ../iip-helper.h

#ifndef IIP_OPS_DEBUG_PRINTF
#define IIP_OPS_DEBUG_PRINTF(_fmt, ...) \
	do { \
		printf("\x1b[32m%ld " _fmt "\x1b[39m", ns3::Simulator::Now().GetNanoSeconds(), ##__VA_ARGS__); \
	} while (0)
#endif

#ifndef __APP_PRINTF
#define __APP_PRINTF(_fmt, ...) \
	do { \
		printf("\x1b[35m%ld " _fmt "\x1b[39m", ns3::Simulator::Now().GetNanoSeconds(), ##__VA_ARGS__); \
	} while (0)
#endif

static int __iosub_main(int argc, char *const *argv)
{
	/* unused */
	return 0;
}

#define main __iip_app_main
#include "bench-iip/main.c"
#undef main

#undef IOSUB_MAIN_C

static uint16_t helper_ip4_get_connection_affinity(uint16_t protocol, uint32_t local_ip4_be, uint16_t local_port_be, uint32_t peer_ip4_be, uint16_t peer_port_be, void *opaque)
{
	return 0;
	{ /* unused */
		(void) protocol;
		(void) local_ip4_be;
		(void) local_port_be;
		(void) peer_ip4_be;
		(void) peer_port_be;
		(void) opaque;
	}
}

static uint16_t iip_ops_l2_hdr_len(void *pkt, void *opaque)
{
	return 0;
	{ /* unused */
		(void) opaque;
		(void) pkt;
	}
}

static uint8_t *iip_ops_l2_hdr_src_ptr(void *pkt, void *opaque)
{
	struct __pkt *p = (struct __pkt *) pkt;
	return p->src_mac;
	{ /* unused */
		(void) opaque;
	}
}

static uint8_t *iip_ops_l2_hdr_dst_ptr(void *pkt, void *opaque)
{
	struct __pkt *p = (struct __pkt *) pkt;
	return p->dst_mac;
	{ /* unused */
		(void) opaque;
	}
}

static uint8_t iip_ops_l2_skip(void *pkt, void *opaque)
{
	return 0;
	{ /* unused */
		(void) pkt;
		(void) opaque;
	}
}

static uint16_t iip_ops_l2_ethertype_be(void *pkt, void *opaque)
{
	struct __pkt *p = (struct __pkt *) pkt;
	return p->ether_type;
	{ /* unused */
		(void) opaque;
	}
}

static uint16_t iip_ops_l2_addr_len(void *opaque)
{
	void **opaque_array = (void **) opaque;
	ns3::IipHelper *ih = (ns3::IipHelper *) opaque_array[0];
	return ih->device->GetAddress().GetLength();
}

static void iip_ops_l2_broadcast_addr(uint8_t bc_mac[], void *opaque)
{
	void **opaque_array = (void **) opaque;
	ns3::IipHelper *ih = (ns3::IipHelper *) opaque_array[0];
	ih->device->GetBroadcast().CopyTo(bc_mac);
}

static void iip_ops_l2_hdr_craft(void *pkt, uint8_t src[], uint8_t dst[], uint16_t ethertype_be, void *opaque)
{
	struct __pkt *p = (struct __pkt *) pkt;
	memcpy(p->src_mac, src, iip_ops_l2_addr_len(opaque));
	memcpy(p->dst_mac, dst, iip_ops_l2_addr_len(opaque));
	p->ether_type = __iip_ntohs(ethertype_be);
}

static uint8_t iip_ops_arp_lhw(void *opaque)
{
	return iip_ops_l2_addr_len(opaque);
}

static uint8_t iip_ops_arp_lproto(void *opaque)
{
	return 4;
	{ /* unused */
		(void) opaque;
	}
}

static void *iip_ops_pkt_alloc(void *opaque)
{
	struct __pkt *p = (struct __pkt *) calloc(1, sizeof(struct __pkt));
	assert(p);
	p->buf = (uint8_t *) calloc(2048, sizeof(uint8_t));
	*((uint64_t *) p->buf) = 1;
	p->len = 0;
	p->head = sizeof(uint64_t);
	return p;
	{ /* unused */
		(void) opaque;
	}
}

static void iip_ops_pkt_free(void *pkt, void *opaque)
{
	struct __pkt *p = (struct __pkt *) pkt;
	*((uint64_t *) p->buf) -= 1;
	if (*((uint64_t *) p->buf) == 0)
		free(p->buf);
	free(p);
	{ /* unused */
		(void) opaque;
	}
}

static void *iip_ops_pkt_get_data(void *pkt, void *opaque)
{
	struct __pkt *p = (struct __pkt *) pkt;
	return &p->buf[p->head];
	{ /* unused */
		(void) pkt;
		(void) opaque;
	}
}

static uint16_t iip_ops_pkt_get_len(void *pkt, void *opaque)
{
	struct __pkt *p = (struct __pkt *) pkt;
	return p->len;
	{ /* unused */
		(void) pkt;
		(void) opaque;
	}
}

static void iip_ops_pkt_set_len(void *pkt, uint16_t len, void *opaque)
{
	struct __pkt *p = (struct __pkt *) pkt;
	p->len = len;
	{ /* unused */
		(void) opaque;
	}
}

static void iip_ops_pkt_increment_head(void *pkt, uint16_t len, void *opaque)
{
	struct __pkt *p = (struct __pkt *) pkt;
	p->head += len;
	{ /* unused */
		(void) pkt;
		(void) len;
		(void) opaque;
	}
}

static void iip_ops_pkt_decrement_tail(void *pkt, uint16_t len, void *opaque)
{
	struct __pkt *p = (struct __pkt *) pkt;
	p->head -= len;
	{ /* unused */
		(void) opaque;
	}
}

static void *iip_ops_pkt_clone(void *pkt, void *opaque)
{
	struct __pkt *new_p = (struct __pkt *) calloc(1, sizeof(struct __pkt));
	assert(new_p);
	{
		struct __pkt *p = (struct __pkt *) pkt;
		memcpy(new_p, p, sizeof(struct __pkt));
		*((uint64_t *) p->buf) += 1;
	}
	return new_p;
	{ /* unused */
		(void) opaque;
	}
}

static void iip_ops_pkt_scatter_gather_chain_append(void *pkt_head, void *pkt_tail, void *opaque)
{
	struct __pkt *p = (struct __pkt *) pkt_head;
	while (p->next)
		p = p->next;
	p->next = (struct __pkt *) pkt_tail;
	{ /* unused */
		(void) opaque;
	}
}

static void *iip_ops_pkt_scatter_gather_chain_get_next(void *pkt_head, void *opaque)
{
	struct __pkt *p = (struct __pkt *) pkt_head;
	return p->next;
	{ /* unused */
		(void) pkt_head;
		(void) opaque;
	}
}

static void iip_ops_l2_flush(void *opaque)
{
	void **opaque_array = (void **) opaque;
	ns3::IipHelper *ih = (ns3::IipHelper *) opaque_array[0];
	{
		uint32_t cnt = ih->tx_queue.size(), i;
		for (i = 0; i < cnt; i++) {
			struct __pkt *p = ih->tx_queue.front();
			ih->tx_queue.pop_front();
			ih->device->Send(ns3::Ptr<ns3::Packet>(new ns3::Packet((const uint8_t *) &p->buf[p->head], p->len)), ns3::Address(0, p->dst_mac, iip_ops_l2_addr_len(opaque)), p->ether_type);
		}
	}
}

static void iip_ops_l2_push(void *_m, void *opaque)
{
	void **opaque_array = (void **) opaque;
	ns3::IipHelper *ih = (ns3::IipHelper *) opaque_array[0];
	ih->tx_queue.push_back((struct __pkt *) _m);
	if (ETH_TX_BATCH == ih->tx_queue.size())
		iip_ops_l2_flush(opaque);
}

static uint8_t iip_ops_nic_feature_offload_tx_scatter_gather(void *opaque)
{
	return 0;
	{ /* unused */
		(void) opaque;
	}
}

static uint8_t iip_ops_nic_feature_offload_ip4_rx_checksum(void *opaque)
{
	return 0;
	{ /* unused */
		(void) opaque;
	}
}

static uint8_t iip_ops_nic_feature_offload_ip4_tx_checksum(void *opaque)
{
	return 0;
	{ /* unused */
		(void) opaque;
	}
}

static uint8_t iip_ops_nic_offload_ip4_rx_checksum(void *m, void *opaque)
{
	return 0;
	{ /* unused */
		(void) m;
		(void) opaque;
	}
}

static uint8_t iip_ops_nic_offload_tcp_rx_checksum(void *m, void *opaque)
{
	return 0;
	{ /* unused */
		(void) m;
		(void) opaque;
	}
}

static uint8_t iip_ops_nic_offload_udp_rx_checksum(void *m, void *opaque)
{
	return 0;
	{ /* unused */
		(void) m;
		(void) opaque;
	}
}

static void iip_ops_nic_offload_ip4_tx_checksum_mark(void *m, void *opaque)
{
	{ /* unused */
		(void) m;
		(void) opaque;
	}
}

static uint8_t iip_ops_nic_feature_offload_tcp_rx_checksum(void *opaque)
{
	return 0;
	{ /* unused */
		(void) opaque;
	}
}

static uint8_t iip_ops_nic_feature_offload_tcp_tx_checksum(void *opaque)
{
	return 0;
	{ /* unused */
		(void) opaque;
	}
}

static uint8_t iip_ops_nic_feature_offload_tcp_tx_tso(void *opaque)
{
	return 0;
	{ /* unused */
		(void) opaque;
	}
}

static void iip_ops_nic_offload_tcp_tx_checksum_mark(void *m, void *opaque)
{
	{ /* unused */
		(void) m;
		(void) opaque;
	}
}

static void iip_ops_nic_offload_tcp_tx_tso_mark(void *m, void *opaque)
{
	{ /* unused */
		(void) m;
		(void) opaque;
	}
}

static uint8_t iip_ops_nic_feature_offload_udp_rx_checksum(void *opaque)
{
	return 0;
	{ /* unused */
		(void) opaque;
	}
}

static uint8_t iip_ops_nic_feature_offload_udp_tx_checksum(void *opaque)
{
	return 0;
	{ /* unused */
		(void) opaque;
	}
}

static uint8_t iip_ops_nic_feature_offload_udp_tx_tso(void *opaque)
{
	return 0;
	{ /* unused */
		(void) opaque;
	}
}

static void iip_ops_nic_offload_udp_tx_checksum_mark(void *m, void *opaque)
{
	{ /* unused */
		(void) m;
		(void) opaque;
	}
}

static void iip_ops_nic_offload_udp_tx_tso_mark(void *m, void *opaque)
{
	{ /* unused */
		(void) m;
		(void) opaque;
	}
}

static void iip_ops_util_now_ns(uint32_t t[3], void *opaque)
{
	uint64_t _t = (uint64_t) ns3::Simulator::Now().GetNanoSeconds();
	t[0] = ((_t / 1000000000UL) >> 32) & 0xffffffff;
	t[1] = ((_t / 1000000000UL) >>  0) & 0xffffffff;
	t[2] = _t % 1000000000UL;
	{ /* unused */
		(void) opaque;
	}
}

namespace ns3
{
IipHelper::IipHelper() { }

IipHelper::~IipHelper()
{
	if (this->opaque[1]) __app_exit(this->opaque[1]);
	if (this->workspace) free(this->workspace);
	if (this->_premem[0]) free(this->_premem[0]);
	if (this->_premem[1]) free(this->_premem[1]);
}

void IipHelper::Install(Ptr<Node> node, int argc, char *const *argv)
{
	this->device = node->GetDevice(0);
	this->device->SetReceiveCallback(MakeCallback(&IipHelper::NicReceive, this));
	{
		ns3::Address addr = this->device->GetAddress();
		this->mac_len = addr.GetLength();
		assert(this->mac_len < sizeof(this->mac_addr));
		addr.CopyTo(this->mac_addr);
	}
	assert((this->workspace = calloc(1, iip_workspace_size())) != NULL);
	assert((this->_premem[0] = (uint8_t *) calloc(1, iip_pb_size() * NUM_NETSTACK_PB)) != NULL);
	assert((this->_premem[1] = (uint8_t *) calloc(1, iip_tcp_conn_size() * NUM_NETSTACK_TCP_CONN)) != NULL);
	{ /* associate memory for packet representation structure */
		uint32_t i;
		for (i = 0; i < NUM_NETSTACK_PB; i++)
			iip_add_pb(this->workspace, &_premem[0][i * iip_pb_size()]);
	}
	{ /* associate memory for tcp connection */
		uint16_t i;
		for (i = 0; i < NUM_NETSTACK_TCP_CONN; i++)
			iip_add_tcp_conn(this->workspace, &_premem[1][i * iip_tcp_conn_size()]);
	}
	(void) __iip_app_main;
	this->opaque[0] = this;
	{
		int prev_optind = optind;
		optind = 1;
		this->opaque[1] = __app_init(argc, argv);;
		optind = prev_optind;
	}
	this->opaque[2] = __app_thread_init(this->workspace, 0 /* core id */, opaque);
}

bool IipHelper::NicReceive(Ptr<NetDevice> dev,
			   Ptr<const Packet> packet,
			   uint16_t protocol,
			   const Address& src)
{
	struct __pkt *p = (struct __pkt *) iip_ops_pkt_alloc(opaque);
	assert(p);
	p->len = packet->GetSize();
	src.CopyTo(p->src_mac);
	dev->GetAddress().CopyTo(p->dst_mac);
	p->ether_type = __iip_htons(protocol);
	packet->CopyData(&p->buf[p->head], p->len);
	this->rx_queue.push_back(p);
	Simulator::Schedule(MicroSeconds(1), &IipHelper::Run, this);
	return true;
}

void IipHelper::Run(void)
{
	uint32_t next_us = 1000000U;
	{
		uint32_t cnt;
		struct __pkt *m[ETH_RX_BATCH];
		for (cnt = 0; cnt < ETH_RX_BATCH && !this->rx_queue.empty(); cnt++) {
			m[cnt] = this->rx_queue.front();
			this->rx_queue.pop_front();
		}
		{
			uint32_t _next_us = 1000000U;
			iip_run(this->workspace, this->mac_addr, this->ip4_addr_be, (void **) m, cnt, &_next_us, this->opaque);
			next_us = _next_us < next_us ? _next_us : next_us;
		}
		{
			uint32_t _next_us = 1000000U;
			__app_loop(this->mac_addr, this->ip4_addr_be, &_next_us, this->opaque);
			next_us = _next_us < next_us ? _next_us : next_us;
		}
		if (cnt && this->ccstat) {
			struct workspace *s = (struct workspace *) this->workspace;
			{
				struct iip_tcp_conn *conn, *_conn_n;
				__iip_q_for_each_safe(s->tcp.conns, conn, _conn_n, 0) {
					printf("\x1b[33m%ld local %u.%u.%u.%u:%u peer %u.%u.%u.%u:%u cc-window %u ssthresh %u (seq %u acked %u) (rx batch %u)\n\x1b[39m",
							ns3::Simulator::Now().GetNanoSeconds(),
							((conn->local_ip4_be >>  0) & 0xff),
							((conn->local_ip4_be >>  8) & 0xff),
							((conn->local_ip4_be >> 16) & 0xff),
							((conn->local_ip4_be >> 24) & 0xff),
							__iip_ntohs(conn->local_port_be),
							((conn->peer_ip4_be >>  0) & 0xff),
							((conn->peer_ip4_be >>  8) & 0xff),
							((conn->peer_ip4_be >> 16) & 0xff),
							((conn->peer_ip4_be >> 24) & 0xff),
							__iip_ntohs(conn->peer_port_be),
							conn->cc.win,
							conn->cc.ssthresh,
							__iip_ntohl(conn->seq_be),
							conn->acked_seq,
							cnt);
				}
			}
		}
	}
	iip_ops_l2_flush(this->opaque);
	if (!__app_should_stop(this->opaque))
		Simulator::Schedule(MicroSeconds((!next_us ? 1 : next_us)), &IipHelper::Run, this);
}

void IipHelper::Start(Time delay)
{
	Simulator::Schedule(delay, &IipHelper::Run, this);
}

void IipHelper::DoStop(void)
{
	sig_h(0);
}

void IipHelper::Stop(Time delay)
{
	Simulator::Schedule(delay, &IipHelper::DoStop, this);
}

void IipHelper::SetDstMac(uint8_t mac[6])
{
	struct app_data *ad = (struct app_data *) this->opaque[1];
	NS_ASSERT(ad);
	memcpy(ad->remote_mac, mac, 6);
}

}

#ifdef DEF_ASSERT
#undef DEF_ASSERT
#define NDEBUG
#undef assert
#define assert
#endif
