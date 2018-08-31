<h1>DHCP (Dynamic Host Configuration Protocol) </h1>
<hr>
<p>DHCP (Dynamic Host Configuration Protocol) is a network management protocol used to dynamically assign an Internet Protocol (IP) address to any device, or node, on a network so they can communicate using IP. DHCP automates and centrally manages these configurations rather than requiring network administrators to manually assign IP addresses to all network devices. DHCP can be implemented on small local networks as well as large enterprise networks.</p>
<br>
<p>DHCP will assign new IP addresses in each location when devices are moved from place to place, which means network administrators do not have to manually initially configure each device with a valid IP address or reconfigure the device with a new IP address if it moves to a new location on the network. Versions of DHCP are available for use in Internet Protocol version 4 (IPv4) and Internet Protocol version 6 (IPv6).</p>
<h2>How DHCP works</h2>
<p>DHCP runs at the application layer of the Transmission Control Protocol/IP (TCP/IP) protocol stack to dynamically assign IP addresses to DHCP clients and to allocate TCP/IP configuration information to DHCP clients. This includes subnet mask information, default gateway IP addresses and domain name system (DNS) addresses.
DHCP is a client-server protocol in which servers manage a pool of unique IP addresses, as well as information about client configuration parameters, and assign addresses out of those address pools. DHCP-enabled clients send a request to the DHCP server whenever they connect to a network.
Clients configured with DHCP broadcast a request to the DHCP server and request network configuration information for the local network to which they're attached. A client typically broadcasts a query for this information immediately after booting up. The DHCP server responds to the client request by providing IP configuration information previously specified by a network administrator. This includes a specific IP address as well as for the time period, also called a lease, for which the allocation is valid. When refreshing an assignment, a DHCP client requests the same parameters, but the DHCP server may assign a new IP address based on policies set by administrators.</p>
<p>A DHCP server manages a record of all the IP addresses it allocates to network nodes. If a node is relocated in the network, the server identifies it using its Media Access Control (MAC) address, which prevents accidentally configuring multiple devices with the same IP address.

DHCP is not a routable protocol, nor is it a secure one. DHCP is limited to a specific local area network (LAN), which means a single DHCP server per LAN is adequate, or two servers for use in case of a failover. Larger networks may have a wide area network (WAN) containing multiple individual locations. Depending on the connections between these points and the number of clients in each location, multiple DHCP servers can be set up to handle the distribution of addresses. If network administrators want a DHCP server to provide addressing to multiple subnets on a given network, they must configure DHCP relay services located on interconnecting routers that DHCP requests have to cross. These agents relay messages between DHCP clients and servers located on different subnets.

DHCP lacks any built-in mechanism that would allow clients and servers to authenticate each other. Both are vulnerable to deception (one computer pretending to be another) and to attack, where rogue clients can exhaust a DHCP server's IP address pool.</p>
<br>
<hr>
<h2>Static vs. dynamic DHCP leases</h2>
<p>With dynamic DHCP, a client does not own the IP address assigned to it but instead "leases" it for a period of time. Each time a device with a dynamic IP address is powered up, it must communicate with the DHCP server to lease another IP address. Wireless devices are examples of clients that are assigned dynamic IP addresses when they connect to a network.

Devices assigned static IP addresses have permanent IP addresses and are used for devices like web servers or switches.

Under a dynamic DHCP setup, a client may also have to perform certain activities that lead to terminating its IP address and then reconnecting to the network using another IP address. DHCP lease times can vary depending on how long a user is likely to need an internet connection at a particular location. Devices release their IP addresses when their DHCP leases expire and then request a renewal from the DHCP server if they are staying online. The DHCP server may assign a new address rather than renewing an old one.</p>
<br>
<p>
<b>The typical dynamic DHCP lease cycle is as follows:</b>

  <li>  A client acquires an IP address lease through allocation process of requesting one from the DHCP server.</li>
   <li> If a client already has an IP address from an existing lease, it will need to refresh its IP address when it reboots after being shut down and will contact the DHCP server to have an IP address reallocated.</li>
  <li>  Once a lease is active, the client is said to be bound to the lease and to the address.</li>
  <li>  Once the lease has expired, a client will contact the server that initially granted the least to renew it so it can keep using its IP address.</li>
   <li> If a client is moving to a different network, its dynamic IP address will be terminated and it will request a new IP address from the DHCP server of the new network</li>
   </p>
