# iip-ns: a helper for running the iip TCP/IP stack on ns-3

This repository has a helper subsystem for testing the [iip](https://github.com/yasukata/iip) TCP/IP stack implementation on ns-3.

ns-3 allows us to run iip on varied network settings in a reproducible manner; it is helpful for debugging the behavior of the protocol implementation.

## how to use

### ns-3 setup

This repository requires ns-3; this ns-3 is the standard one, and there is no specific procedure for this repository.

ns-3's official installation guide is found at https://www.nsnam.org/docs/installation/html/index.html and a quick start instruction is at https://www.nsnam.org/docs/installation/html/quick-start.html .

Please note that the code in this repository is tested with ns-3 version 3.40 on Linux.

The following commands are picked up from the official instruction; if these do not work, it would be recommended to check the required packages described in the official instruction.

```
wget https://www.nsnam.org/releases/ns-allinone-3.40.tar.bz2
```

```
tar xvf ns-allinone-3.40.tar.bz2
```

```
cd ns-allinone-3.40/ns-3.40
```

```
./ns3 configure --enable-examples --enable-tests
```

```
./ns3 build
```

### putting iip-relevant code

After setting up ns-3, please download the code of this repository in ```ns-allinone-3.40/ns-3.40/src```.

```
cd ns-allinone-3.40/ns-3.40/src
```

```
git clone https://github.com/yasukata/iip-ns.git
```

Afterward, please download the source code of the benchmark application program and iip by the follow commands.

```
cd iip-ns/helper
```

```
git clone https://github.com/yasukata/bench-iip.git
```

```
cd bench-iip
```

```
git clone https://github.com/yasukata/iip.git
```

Supposedly, we will have the following directories after typing the commands above.

- ```ns-allinone-3.40/ns-3.40/src/iip-ns```
- ```ns-allinone-3.40/ns-3.40/src/iip-ns/helper/bench-iip```
- ```ns-allinone-3.40/ns-3.40/src/iip-ns/helper/bench-iip/iip```

## example program

Please save the following program as ```ns-allinone-3.40/ns-3.40/scratch/use-iip.cc```.

```c++
#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/iip-ns-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("use iip");

int main(int argc, char* argv[])
{
	CommandLine cmd(__FILE__);
	cmd.Parse(argc, argv);

	GlobalValue::Bind("ChecksumEnabled", BooleanValue(true));

	Time::SetResolution(Time::NS);

	NodeContainer nodes;
	nodes.Create(2);

	PointToPointHelper p2p;
	p2p.SetDeviceAttribute("DataRate", StringValue("100Mbps"));
	p2p.SetChannelAttribute("Delay", StringValue("2ms"));

	NetDeviceContainer link = p2p.Install(nodes);
	DynamicCast<PointToPointNetDevice>(link.Get(0))->GetQueue()->SetMaxSize(QueueSize(QueueSizeUnit::PACKETS, 32));
	DynamicCast<PointToPointNetDevice>(link.Get(1))->GetQueue()->SetMaxSize(QueueSize(QueueSizeUnit::PACKETS, 32));
	printf("queue 0 len %u\n", DynamicCast<PointToPointNetDevice>(link.Get(0))->GetQueue()->GetMaxSize().GetValue());
	printf("queue 1 len %u\n", DynamicCast<PointToPointNetDevice>(link.Get(1))->GetQueue()->GetMaxSize().GetValue());

#if 1
	/* server application using iip */
	const char *_argv_s[8] = {
		"none",
		"-p", "10000",
		"-g", "2",
		"-v", "1",
		NULL,
	};
	IipHelper iip_s;
	iip_s.ip4_addr_be = (10U << 0) | (1U << 8) | (1U << 16) | (1U << 24);
	iip_s.Install(nodes.Get(0), 7, (char *const *) _argv_s);
	iip_s.Start(Seconds(.5));
	//iip_s.ccstat = true;
#else
	/* server application using the TCP/IP implementation in ns-3 */
	InternetStackHelper internet;
	internet.Install(nodes.Get(0));

	Ipv4AddressHelper ipv4;
	ipv4.SetBase("10.1.1.0", "255.255.255.0");
	ipv4.Assign(link.Get(0));

	PacketSinkHelper receiver("ns3::TcpSocketFactory",
			InetSocketAddress(Ipv4Address("10.1.1.1"), 10000));

	ApplicationContainer rx_app = receiver.Install(nodes.Get(0));
	rx_app.Start(Seconds(.5));
#endif
#if 1
	/* client application using iip */
	const char *_argv_c[16] = {
		"none",
		"-p", "10000",
		"-g", "2",
		"-d", "200",
		"-l", "1440",
		"-s", "10.1.1.1",
		"-v", "1",
		"-c", "1",
		NULL,
	};
	IipHelper iip_c;
	iip_c.ip4_addr_be = (10U << 0) | (1U << 8) | (1U << 16) | (2U << 24);
	iip_c.Install(nodes.Get(1), 15, (char *const *) _argv_c);
	{
		ns3::Address addr = link.Get(0)->GetAddress();
		{
			uint8_t mac[6];
			NS_ASSERT(addr.GetLength() <= sizeof(mac));
			addr.CopyTo(mac);
			iip_c.SetDstMac(mac);
		}
	}
	iip_c.Start(Seconds(1));
	iip_c.ccstat = true;
#else
	/* client application using the TCP/IP implementation in ns-3 */
	InternetStackHelper internet;
	internet.Install(nodes.Get(1));

	Ipv4AddressHelper ipv4;
	ipv4.SetBase("10.1.1.0", "255.255.255.0");
	ipv4.NewAddress();
	ipv4.Assign(link.Get(1));

	BulkSendHelper sender("ns3::TcpSocketFactory",
			InetSocketAddress(Ipv4Address("10.1.1.1"), 10000));

	ApplicationContainer tx_app = sender.Install(nodes.Get(1));
	tx_app.Start(Seconds(1));
#endif

	Simulator::Stop(Seconds(3));
	Simulator::Run();
	Simulator::Destroy();

	printf("Done.\n");

	return 0;
}
```

The current implementation of IipHelper executes the benchmark application (bench-iip) found at https://github.com/yasukata/bench-iip .

The IipHelper class implements the following functions.

- ```Install(int argc, char *const *argv)```: argc and argv passed to the application; please refer to https://github.com/yasukata/bench-iip#3rd-section-for-the-benchmark-tool for the argument setting.
- ```Start```: the program starts after the specified time (in the simulator time scale) has passed.
- ```Stop```: the program stops after the specified time (in the simulator time scale) has passed.

If ```ccstat``` is true, IipHelper prints logs relevant to congestion control.

## run the program

The following command executes the example program above and redirects the output to ```less```.

```
cd ns-allinone-3.40/ns-3.40
```

```
./ns3 run use-iip 2>&1 | less -R
```

The following command extracts specific lines showing the congestion window and saves the lines in ```./log-cw.txt```.

```
./ns3 run use-iip 2>&1 | grep "local 10.1.1.2" | awk '{ print $1 ", " $7 }' | sed -r "s/\x1B\[([0-9]{1,2}(;[0-9]{1,2})?)?[mGK]//g" > ./log-cw.txt
```

If your system has gnuplot and the xterm environment, the following script will show the transition of the congestion window.

Please save the following script as ```./log-cw.plt```.

```
set terminal xterm
set yrange [0:]
plot "./log-cw.txt" u ($1/1000000000.0):2 w l
```

Then, please type the following command.

```
gnuplot ./log-cw.plt
```

Or, if you wish to have the output as a PNG file, please replace the content of ```./log-cw.plt``` with the following; after typing the gnuplot command shown above, you will have a PNG file named ```./log-cw.png```.

```
set terminal png
set yrange [0:]
set output "log-cw.png"
plot "./log-cw.txt" u ($1/1000000000.0):2 w l
```
