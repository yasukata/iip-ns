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

#ifndef NS3_IIP_HELPER_H
#define NS3_IIP_HELPER_H

#include "ns3/core-module.h"
#include "ns3/mac48-address.h"
#include "ns3/net-device.h"
#include "ns3/boolean.h"
#include "ns3/node.h"
#include "ns3/packet.h"

#include <deque>

#include <linux/if_ether.h>
#include <numa.h>

struct __pkt {
	uint8_t src_mac[20];
	uint8_t dst_mac[20];
	uint16_t ether_type;
	uint16_t len;
	uint16_t head;
	uint8_t *buf;
	struct __pkt *next;
	struct __pkt *prev;
};

namespace ns3
{

class IipHelper
{
public:
	static TypeId GetTypeId();
	IipHelper();
	~IipHelper();
	void Install(Ptr<Node> node, int argc, char *const *argv);
	void *opaque[3];
	uint8_t mac_addr[20];
	uint8_t mac_len;
	uint32_t ip4_addr_be;
	std::deque<struct __pkt *> tx_queue;
	std::deque<struct __pkt *> rx_queue;
	Ptr<NetDevice> device;
	void *workspace;
	uint8_t *_premem[2];
	bool NicReceive(Ptr<NetDevice> dev,
			Ptr<const Packet> packet,
			uint16_t protocol,
			const Address& src);
	void Run(void);
	void Start(Time delay);
	void DoStop(void);
	void Stop(Time delay);
	void SetDstMac(uint8_t mac[6]);
	bool ccstat;
};

}

#endif
