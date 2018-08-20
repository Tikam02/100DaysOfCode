<h1>Socket Programming Basics</h1>
<hr>
<h2>How to Transmit Data</h2>
<p>First, this is a path used to transmit data when an application calls the write system call.</p><br>
<hr>
<!-- HTML generated using hilite.me --><div style="background: #111111; overflow:auto;width:auto;border:solid gray;border-width:.1em .1em .1em .8em;padding:.2em .6em;"><pre style="margin: 0; line-height: 125%"><span style="color: #ffffff">SYSCALL_DEFINE3(write,</span> <span style="color: #cdcaa9; font-weight: bold">unsigned</span> <span style="color: #cdcaa9; font-weight: bold">int</span><span style="color: #ffffff">,</span> <span style="color: #ffffff">fd,</span> <span style="color: #fb660a; font-weight: bold">const</span> <span style="color: #cdcaa9; font-weight: bold">char</span> <span style="color: #ffffff">__user</span> <span style="color: #ffffff">*,</span> <span style="color: #ffffff">buf,</span> <span style="color: #ffffff">...)</span>
 
<span style="color: #ffffff">{</span>
 
<span style="color: #fb660a; font-weight: bold">struct</span> <span style="color: #ffffff">file</span> <span style="color: #ffffff">*file;</span>
 
<span style="color: #ffffff">[...]</span>
 
<span style="color: #ffffff">file</span> <span style="color: #ffffff">=</span> <span style="color: #ffffff">fget_light(fd,</span> <span style="color: #ffffff">&amp;fput_needed);</span>
 
<span style="color: #ffffff">[...]</span> <span style="color: #ffffff">===&gt;</span>
 
<span style="color: #ffffff">ret</span> <span style="color: #ffffff">=</span> <span style="color: #ffffff">filp-&gt;f_op-&gt;aio_write(&amp;kiocb,</span> <span style="color: #ffffff">&amp;iov,</span> <span style="color: #0086f7; font-weight: bold">1</span><span style="color: #ffffff">,</span> <span style="color: #ffffff">kiocb.ki_pos);</span>
 
 
 
<span style="color: #fb660a; font-weight: bold">struct</span> <span style="color: #ffffff">file_operations</span> <span style="color: #ffffff">{</span>
 
<span style="color: #ffffff">[...]</span>
 
<span style="color: #cdcaa9; font-weight: bold">ssize_t</span> <span style="color: #ffffff">(*aio_read)</span> <span style="color: #ffffff">(</span><span style="color: #fb660a; font-weight: bold">struct</span> <span style="color: #ffffff">kiocb</span> <span style="color: #ffffff">*,</span> <span style="color: #fb660a; font-weight: bold">const</span> <span style="color: #fb660a; font-weight: bold">struct</span> <span style="color: #ffffff">iovec</span> <span style="color: #ffffff">*,</span> <span style="color: #ffffff">...)</span>
 
<span style="color: #cdcaa9; font-weight: bold">ssize_t</span> <span style="color: #ffffff">(*aio_write)</span> <span style="color: #ffffff">(</span><span style="color: #fb660a; font-weight: bold">struct</span> <span style="color: #ffffff">kiocb</span> <span style="color: #ffffff">*,</span> <span style="color: #fb660a; font-weight: bold">const</span> <span style="color: #fb660a; font-weight: bold">struct</span> <span style="color: #ffffff">iovec</span> <span style="color: #ffffff">*,</span> <span style="color: #ffffff">...)</span>
 
<span style="color: #ffffff">[...]</span>
 
<span style="color: #ffffff">};</span>
 
 
 
<span style="color: #fb660a; font-weight: bold">static</span> <span style="color: #fb660a; font-weight: bold">const</span> <span style="color: #fb660a; font-weight: bold">struct</span> <span style="color: #ffffff">file_operations</span> <span style="color: #ffffff">socket_file_ops</span> <span style="color: #ffffff">=</span> <span style="color: #ffffff">{</span>
 
<span style="color: #ffffff">[...]</span>
 
<span style="color: #ffffff">.aio_read</span> <span style="color: #ffffff">=</span> <span style="color: #ffffff">sock_aio_read,</span>
 
<span style="color: #ffffff">.aio_write</span> <span style="color: #ffffff">=</span> <span style="color: #ffffff">sock_aio_write,</span>
 
<span style="color: #ffffff">[...]</span>
 
<span style="color: #ffffff">};</span>
</pre></div>

<hr>
<p>when the application calls the write system call,the kernel performs the <strong>write()</strong> function of the file layer.First,the actual file structure of the file descriptor fd is fetched.And then the <strong>aio_write</strong> is called.
This is the function pointer.In the file structure,you will see the <strong>file_operations</strong> structure pointer. The structure is generally called function table and includes the function pointers such as aio_read and aio_write. The actual table for the socket is socket_file_ops. The aio_write function used by the socket is sock_aio_write. The function table is used for the purpose that is similar to the Java interface. It is generally used for the kernel to perform code abstraction or refactoring.
</p>
<hr>
<pre>
static ssize_t sock_aio_write(struct kiocb *iocb, const struct iovec *iov, ..)
 
{
 
[...]
 
struct socket *sock = file->private_data;
 
[...] ===>
 
return sock->ops->sendmsg(iocb, sock, msg, size);
 
 
 
struct socket {
 
[...]
 
struct file *file;
 
struct sock *sk;
 
const struct proto_ops *ops;
 
};
 
 
 
const struct proto_ops inet_stream_ops = {
 
.family = PF_INET,
 
[...]
 
.connect = inet_stream_connect,
 
.accept = inet_accept,
 
.listen = inet_listen, .sendmsg = tcp_sendmsg,
 
.recvmsg = inet_recvmsg,
 
[...]
 
};
 
 
 
struct proto_ops {
 
[...]
 
int (*connect) (struct socket *sock, ...)
 
int (*accept) (struct socket *sock, ...)
 
int (*listen) (struct socket *sock, int len);
 
int (*sendmsg) (struct kiocb *iocb, struct socket *sock, ...)
 
int (*recvmsg) (struct kiocb *iocb, struct socket *sock, ...)
 
[...]
 
};
</pre>
<hr>
<p>

The <strong>sock_aio_write()</strong> function gets the socket structure from the file and then calls sendmsg. It is also the function pointer. The socket structure includes the <strong>proto_ops</strong> function table. The proto_ops implemented by the IPv4 TCP is inet_stream_ops and the sendmsg is implemented by tcp_sendmsg.</p>
<hr>
<pre>
int tcp_sendmsg(struct kiocb *iocb, struct socket *sock,
 
struct msghdr *msg, size_t size)
 
{
 
struct sock *sk = sock->sk;
 
struct iovec *iov;
 
struct tcp_sock *tp = tcp_sk(sk);
 
struct sk_buff *skb;
 
[...]
 
mss_now = tcp_send_mss(sk, &size_goal, flags);
 
 
 
/* Ok commence sending. */
 
iovlen = msg->msg_iovlen;
 
iov = msg->msg_iov;
 
copied = 0;
 
[...]
 
while (--iovlen >= 0) {
 
int seglen = iov->iov_len;
 
unsigned char __user *from = iov->iov_base;
 
 
 
iov++;
 
while (seglen > 0) {
 
int copy = 0;
 
int max = size_goal;
 
[...]
 
skb = sk_stream_alloc_skb(sk,
 
select_size(sk, sg),
 
sk->sk_allocation);
 
if (!skb)
 
goto wait_for_memory;
 
/*
 
* Check whether we can use HW checksum.
 
*/
 
if (sk->sk_route_caps & NETIF_F_ALL_CSUM)
 
skb->ip_summed = CHECKSUM_PARTIAL;
 
[...]
 
skb_entail(sk, skb);
 
[...]
 
/* Where to copy to? */
 
if (skb_tailroom(skb) > 0) {
 
/* We have some space in skb head. Superb! */
 
if (copy > skb_tailroom(skb))
 
copy = skb_tailroom(skb);
 
if ((err = skb_add_data(skb, from, copy)) != 0)
 
goto do_fault;
 
[...]
 
if (copied)
 
tcp_push(sk, flags, mss_now, tp->nonagle);
 
[...]
 
}
</pre>
<hr>
tcp_sengmsg gets tcp_sock (i.e.,TCP control block) from the socket and copies the data that the application has requested to transmit to the send socket buffer. When copying data to sk_buff, how many bytes will one sk_buff include? One sk_buff copies and includes MSS (tcp_send_mss) bytes to help the code that actually creates packets. Maximum Segment Size (MSS) stands for the maximum payload size that one TCP packet includes. By using TSO and GSO, one sk_buff can save more data than MSS. This will be discussed later, not in this document.<br>

The sk_stream_alloc_skb function creates a new sk_buff, and skb_entail adds the new sk_buff to the tail of the send_socket_buffer. The skb_add_data function copies the actual application data to the data buffer of the sk_buff. All the data is copied by repeating the procedure (creating an sk_buff and adding it to the send socket buffer) several times. Therefore, sk_buffs at the size of the MSS are in the send socket buffer as a list. Finally, the tcp_push is called to make the data which can be transmitted now as a packet, and the packet is sent.
<hr>
<pre>
static inline void tcp_push(struct sock *sk, int flags, int mss_now, ...)
 
[...] ===>
 
static int tcp_write_xmit(struct sock *sk, unsigned int mss_now, ...)
 
int nonagle,
 
{
 
struct tcp_sock *tp = tcp_sk(sk);
 
struct sk_buff *skb;
 
[...]
 
while ((skb = tcp_send_head(sk))) {
 
[...]
 
cwnd_quota = tcp_cwnd_test(tp, skb);
 
if (!cwnd_quota)
 
break;
 
 
 
if (unlikely(!tcp_snd_wnd_test(tp, skb, mss_now)))
 
break;
 
[...]
 
if (unlikely(tcp_transmit_skb(sk, skb, 1, gfp)))
 
break;
 
 
 
/* Advance the send_head. This one is sent out.
 
* This call will increment packets_out.
 
*/
 
tcp_event_new_data_sent(sk, skb);
 
[...]
</pre>
<hr>
<p>
The <b>tcp_push</b> function transmits as many of the <b>sk_buffs</b> in the send socket buffer as the TCP allows in sequence. First, the <b>tcp_send_head</b> is called to get the first <b>sk_buff</b> in the socket buffer and the <b>tcp_cwnd_test</b> and the <b>tcp_snd_wnd_test</b> are performed to check whether the congestion window and the receive window of the receiving TCP allow new packets to be transmitted. Then, the <b>tcp_transmit_skb</b> function is called to create a packet.
</p>
<pre>
static int tcp_transmit_skb(struct sock *sk, struct sk_buff *skb,
 
int clone_it, gfp_t gfp_mask)
 
{
 
const struct inet_connection_sock *icsk = inet_csk(sk);
 
struct inet_sock *inet;
 
struct tcp_sock *tp;
 
[...]
 
 
 
 
 
if (likely(clone_it)) {
 
if (unlikely(skb_cloned(skb)))
 
skb = pskb_copy(skb, gfp_mask);
 
else
 
skb = skb_clone(skb, gfp_mask);
 
if (unlikely(!skb))
 
return -ENOBUFS;
 
}
 
 
 
[...]
 
skb_push(skb, tcp_header_size);
 
skb_reset_transport_header(skb);
 
skb_set_owner_w(skb, sk);
 
 
 
/* Build TCP header and checksum it. */
 
th = tcp_hdr(skb);
 
th->source = inet->inet_sport;
 
th->dest = inet->inet_dport;
 
th->seq = htonl(tcb->seq);
 
th->ack_seq = htonl(tp->rcv_nxt);
 
[...]
 
icsk->icsk_af_ops->send_check(sk, skb);
 
[...]
 
err = icsk->icsk_af_ops->queue_xmit(skb);
 
if (likely(err <= 0))
 
return err;
 
 
 
tcp_enter_cwr(sk, 1);
 
 
 
return net_xmit_eval(err);
 
}
</pre>
<hr>
<p>
<b>tcp_transmit_skb</b> creates the copy of the given <b>sk_buff</b> (pskb_copy). At this time, it does not copy the entire data of the application but the metadata. And then it calls <b>skb_push</b> to secure the header area and records the header field value. Send_check computes the TCP checksum. With the checksum offload, the payload data is not computed. Finally, <b>queue_xmit</b> is called to send the packet to the IP layer. Queue_xmit for IPv4 is implemented by the <b>ip_queue_xmit</b> function.
</p>
<hr>
<pre>
int ip_queue_xmit(struct sk_buff *skb)
 
[...]
 
rt = (struct rtable *)__sk_dst_check(sk, 0);
 
[...]
 
/* OK, we know where to send it, allocate and build IP header. */
 
skb_push(skb, sizeof(struct iphdr) + (opt ? opt->optlen : 0));
 
skb_reset_network_header(skb);
 
iph = ip_hdr(skb);
 
*((__be16 *)iph) = htons((4 << 12) | (5 << 8) | (inet->tos & 0xff));
 
if (ip_dont_fragment(sk, &rt->dst) && !skb->local_df)
 
iph->frag_off = htons(IP_DF);
 
else
 
iph->frag_off = 0;
 
iph->ttl = ip_select_ttl(inet, &rt->dst);
 
iph->protocol = sk->sk_protocol;
 
iph->saddr = rt->rt_src;
 
iph->daddr = rt->rt_dst;
 
[...]
 
res = ip_local_out(skb);
 
[...] ===>
 
int __ip_local_out(struct sk_buff *skb)
 
[...]
 
ip_send_check(iph);
 
return nf_hook(NFPROTO_IPV4, NF_INET_LOCAL_OUT, skb, NULL,
 
skb_dst(skb)->dev, dst_output);
 
[...] ===>
 
int ip_output(struct sk_buff *skb)
 
{
 
struct net_device *dev = skb_dst(skb)->dev;
 
[...]
 
skb->dev = dev;
 
skb->protocol = htons(ETH_P_IP);
 
 
 
return NF_HOOK_COND(NFPROTO_IPV4, NF_INET_POST_ROUTING, skb, NULL, dev,
 
ip_finish_output,
 
[...] ===>
 
static int ip_finish_output(struct sk_buff *skb)
 
[...]
 
if (skb->len > ip_skb_dst_mtu(skb) && !skb_is_gso(skb))
 
return ip_fragment(skb, ip_finish_output2);
 
else
 
return ip_finish_output2(skb);
</pre>
<hr>
<p>
The <b>ip_queue_xmit</b> function executes tasks required by the IP layers. <b>__sk_dst_check</b> checks whether the cached route is valid. If there is no cached route or the cached route is invalid, it performs IP routing. And then it calls <b>skb_push</b> to secure the IP header area and records the IP header field value. After that, as following the function call, <b>ip_send_check</b> computes the IP header checksum and calls the netfilter function. IP fragment is created when <b>ip_finish_output</b> function needs IP fragmentation. No fragmentation is generated when TCP is used. Therefore, <b>ip_finish_output2</b> is called and it adds the Ethernet header. Finally, a packet is completed.
</p>
<hr>
<pre>
int dev_queue_xmit(struct sk_buff *skb)
 
[...] ===>
 
static inline int __dev_xmit_skb(struct sk_buff *skb, struct Qdisc *q, ...)
 
[...]
 
if (...) {
 
....
 
} else
 
if ((q->flags & TCQ_F_CAN_BYPASS) && !qdisc_qlen(q) &&
 
 
 
qdisc_run_begin(q)) {
 
[...]
 
if (sch_direct_xmit(skb, q, dev, txq, root_lock)) {
 
[...] ===>
 
int sch_direct_xmit(struct sk_buff *skb, struct Qdisc *q, ...)
 
[...]
 
HARD_TX_LOCK(dev, txq, smp_processor_id());
 
if (!netif_tx_queue_frozen_or_stopped(txq))
 
ret = dev_hard_start_xmit(skb, dev, txq);
 
 
 
HARD_TX_UNLOCK(dev, txq);
 
[...]
 
}
 
 
 
int dev_hard_start_xmit(struct sk_buff *skb, struct net_device *dev, ...)
 
[...]
 
if (!list_empty(&ptype_all))
 
dev_queue_xmit_nit(skb, dev);
 
[...]
 
rc = ops->ndo_start_xmit(skb, dev);
 
[...]
 
}
</pre>
<hr>
<p>
The completed packet is transmitted through the <b>dev_queue_xmit</b> function. First, the packet passes via the qdisc. If the default qdisc is used and the queue is empty, the <b>sch_direct_xmit</b> function is called to directly send down the packet to the driver, skipping the queue. <b>Dev_hard_start_xmit</b> function calls the actual driver. Before calling the driver, the device TX is locked first. This is to prevent several threads from accessing the device simultaneously. As the kernel locks the device TX, the driver transmission code does not need an additional lock. It is closely related to the parallel processing that will be discussed next time.
<br>
<b>Ndo_start_xmit</b> function calls the driver code. Just before, you will see <b>ptype_all</b> and <b>dev_queue_xmit_nit</b>. The <b>ptype_all</b> is a list that includes the modules such as packet capture. If a capture program is running, the packet is copied by <p>type_all</b> to the separate program. Therefore, the packet that tcpdump shows is the packet transmitted to the driver. When checksum offload or TSO is used, the NIC manipulates the packet. So the tcpdump packet is different from the packet transmitted to the network line. After completing packet transmission, the driver interrupt handler returns the sk_buff.
</p>
<hr>
<h2>How to Receive Data</h2>
<hr>
<p>
The general executed path is to receive a packet and then to add the data to the receive socket buffer. After executing the driver interrupt handler, follow the napi poll handle first.</p>
<hr>
<pre>
static void net_rx_action(struct softirq_action *h)
 
{
 
struct softnet_data *sd = &__get_cpu_var(softnet_data);
 
unsigned long time_limit = jiffies + 2;
 
int budget = netdev_budget;
 
void *have;
 
 
 
local_irq_disable();
 
 
 
while (!list_empty(&sd->poll_list)) {
 
struct napi_struct *n;
 
[...]
 
n = list_first_entry(&sd->poll_list, struct napi_struct,
 
poll_list);
 
if (test_bit(NAPI_STATE_SCHED, &n->state)) {
 
work = n->poll(n, weight);
 
trace_napi_poll(n);
 
}
 
[...]
 
}
 
 
 
int netif_receive_skb(struct sk_buff *skb)
 
[...] ===>
 
static int __netif_receive_skb(struct sk_buff *skb)
 
{
 
struct packet_type *ptype, *pt_prev;
 
[...]
 
__be16 type;
 
[...]
 
list_for_each_entry_rcu(ptype, &ptype_all, list) {
 
if (!ptype->dev || ptype->dev == skb->dev) {
 
if (pt_prev)
 
ret = deliver_skb(skb, pt_prev, orig_dev);
 
pt_prev = ptype;
 
}
 
}
 
[...]
 
type = skb->protocol;
 
list_for_each_entry_rcu(ptype,
 
&ptype_base[ntohs(type) & PTYPE_HASH_MASK], list) {
 
if (ptype->type == type &&
 
 
 
(ptype->dev == null_or_dev || ptype->dev == skb->dev ||
 
ptype->dev == orig_dev)) {
 
if (pt_prev)
 
ret = deliver_skb(skb, pt_prev, orig_dev);
 
pt_prev = ptype;
 
}
 
}
 
 
 
if (pt_prev) {
 
ret = pt_prev->func(skb, skb->dev, pt_prev, orig_dev);
 
 
 
static struct packet_type ip_packet_type __read_mostly = {
 
.type = cpu_to_be16(ETH_P_IP),
 
.func = ip_rcv,
 
[...]
 
};
</pre>
<hr>
<p>
As mentioned before, the <b>net_rx_action</b> function is the softirq handler that receives a packet. First, the driver that has requested the napi poll is retrieved from the <b>poll_list</b> and the poll handler of the driver is called. The driver wraps the received packet with <b>sk_buff</b> and then calls <b>netif_receive_skb</b>.<br>

When there is a module that requests all packets, the <b>netif_receive_skb</b> sends packets to the module. Like packet transmission, the packets are transmitted to the module registered to the <b>ptype_all</b> list. The packets are captured here.
<br>
Then, the packets are transmitted to the upper layer based on the packet type. The Ethernet packet includes 2-byte ethertype field in the header. The value indicates the packet type. The driver records the value in <b>sk_buff (skb->protocol)</b>. Each protocol has its own packet_type structure and registers the pointer of the structure to the <b>ptype_base</b> hash table. IPv4 uses <b>ip_packet_type</b>. The Type field value is the IPv4 ethertype (ETH_P_IP) value. Therefore, the IPv4 packet calls the <b>ip_rcv</b> function.
</p>
<pre>
int ip_rcv(struct sk_buff *skb, struct net_device *dev, ...)
 
{
 
struct iphdr *iph;
 
u32 len;
 
[...]
 
iph = ip_hdr(skb);
 
[...]
 
if (iph->ihl < 5 || iph->version != 4)
 
goto inhdr_error;
 
 
 
if (!pskb_may_pull(skb, iph->ihl*4))
 
goto inhdr_error;
 
 
 
iph = ip_hdr(skb);
 
 
 
if (unlikely(ip_fast_csum((u8 *)iph, iph->ihl)))
 
goto inhdr_error;
 
 
 
len = ntohs(iph->tot_len);
 
if (skb->len < len) {
 
IP_INC_STATS_BH(dev_net(dev), IPSTATS_MIB_INTRUNCATEDPKTS);
 
goto drop;
 
} else if (len < (iph->ihl*4))
 
goto inhdr_error;
 
[...]
 
return NF_HOOK(NFPROTO_IPV4, NF_INET_PRE_ROUTING, skb, dev, NULL,
 
ip_rcv_finish);
 
[...] ===>
 
int ip_local_deliver(struct sk_buff *skb)
 
[...]
 
if (ip_hdr(skb)->frag_off & htons(IP_MF | IP_OFFSET)) {
 
if (ip_defrag(skb, IP_DEFRAG_LOCAL_DELIVER))
 
return 0;
 
}
 
 
 
return NF_HOOK(NFPROTO_IPV4, NF_INET_LOCAL_IN, skb, skb->dev, NULL,
 
ip_local_deliver_finish);
 
[...] ===>
 
 
 
 
 
static int ip_local_deliver_finish(struct sk_buff *skb)
 
[...]
 
__skb_pull(skb, ip_hdrlen(skb));
 
[...]
 
int protocol = ip_hdr(skb)->protocol;
 
int hash, raw;
 
const struct net_protocol *ipprot;
 
[...]
 
hash = protocol & (MAX_INET_PROTOS - 1);
 
ipprot = rcu_dereference(inet_protos[hash]);
 
if (ipprot != NULL) {
 
[...]
 
ret = ipprot->handler(skb);
 
[...] ===>
 
 
 
static const struct net_protocol tcp_protocol = {
 
.handler = tcp_v4_rcv,
 
[...]
 
};
</pre>
<hr>
<p>
The <b>ip_rcv</b> function executes tasks required by the IP layers. It examines packets such as the length and header checksum. After passing through the netfilter code, it performs the <b>ip_local_deliver</b> function. If required, it assembles IP fragments. Then, it calls <b>ip_local_deliver_finish</b> through the netfilter code. The <b>ip_local_deliver_finish</b> function removes the IP header by using the <b>__skb_pull</b> and then searches the upper protocol whose value is identical to the IP header protocol value. Similar to the <b>Ptype_base</b>, each transport protocol registers its own <b>net_protocol</b> structure in <b>inet_protos</b>. IPv4 TCP uses <b>tcp_protocol</b> and calls <b>tcp_v4_rcv</b> that has been registered as a handler.<br>

When packets come into the TCP layer, the packet processing flow varies depending on the TCP status and the packet type. Here, we will see the packet processing procedure when the expected next data packet has been received in the ESTABLISHED status of the TCP connection. This path is frequently executed by the server receiving data when there is no packet loss or out-of-order delivery.
</p>
<pre>
int tcp_v4_rcv(struct sk_buff *skb)
 
{
 
const struct iphdr *iph;
 
struct tcphdr *th;
 
struct sock *sk;
 
[...]
 
th = tcp_hdr(skb);
 
 
 
if (th->doff < sizeof(struct tcphdr) / 4)
 
goto bad_packet;
 
if (!pskb_may_pull(skb, th->doff * 4))
 
goto discard_it;
 
[...]
 
th = tcp_hdr(skb);
 
iph = ip_hdr(skb);
 
TCP_SKB_CB(skb)->seq = ntohl(th->seq);
 
TCP_SKB_CB(skb)->end_seq = (TCP_SKB_CB(skb)->seq + th->syn + th->fin +
 
skb->len - th->doff * 4);
 
TCP_SKB_CB(skb)->ack_seq = ntohl(th->ack_seq);
 
TCP_SKB_CB(skb)->when = 0;
 
TCP_SKB_CB(skb)->flags = iph->tos;
 
TCP_SKB_CB(skb)->sacked = 0;
 
 
 
sk = __inet_lookup_skb(&tcp_hashinfo, skb, th->source, th->dest);
 
[...]
 
ret = tcp_v4_do_rcv(sk, skb);
</pre>
<hr>
<p>
First, the <b>tcp_v4_rcv</b> function validates the received packets. When the header size is larger than the data offset <b>(th->doff < sizeof(struct tcphdr) / 4)</b>, it is the header error. And then<b> __inet_lookup_skb</b> is called to look for the connection where the packet belongs from the TCP connection hash table. From the sock structure found, all required structures such as <b>tcp_sock</b> and socket can be got.
</p>
<hr>
<pre>
int tcp_v4_do_rcv(struct sock *sk, struct sk_buff *skb)
 
[...]
 
if (sk->sk_state == TCP_ESTABLISHED) { /* Fast path */
 
sock_rps_save_rxhash(sk, skb->rxhash);
 
if (tcp_rcv_established(sk, skb, tcp_hdr(skb), skb->len)) {
 
[...] ===>
 
int tcp_rcv_established(struct sock *sk, struct sk_buff *skb,
 
[...]
 
/*
 
* Header prediction.
 
*/
 
if ((tcp_flag_word(th) & TCP_HP_BITS) == tp->pred_flags &&
 
 
 
TCP_SKB_CB(skb)->seq == tp->rcv_nxt &&
 
 
 
!after(TCP_SKB_CB(skb)->ack_seq, tp->snd_nxt))) {
 
[...]
 
if ((int)skb->truesize > sk->sk_forward_alloc)
 
goto step5;
 
 
 
NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_TCPHPHITS);
 
 
 
/* Bulk data transfer: receiver */
 
__skb_pull(skb, tcp_header_len);
 
__skb_queue_tail(&sk->sk_receive_queue, skb);
 
skb_set_owner_r(skb, sk);
 
tp->rcv_nxt = TCP_SKB_CB(skb)->end_seq;
 
[...]
 
if (!copied_early || tp->rcv_nxt != tp->rcv_wup)
 
__tcp_ack_snd_check(sk, 0);
 
[...]
 
step5:
 
if (th->ack && tcp_ack(sk, skb, FLAG_SLOWPATH) < 0)
 
goto discard;
 
 
 
tcp_rcv_rtt_measure_ts(sk, skb);
 
 
 
/* Process urgent data. */
 
tcp_urg(sk, skb, th);
 
 
 
/* step 7: process the segment text */
 
tcp_data_queue(sk, skb);
 
 
 
tcp_data_snd_check(sk);
 
tcp_ack_snd_check(sk);
 
return 0;
 
[...]
 
}
</pre>
<p>
 The actual protocol is executed from the <b>tcp_v4_do_rcv</b> function. If the TCP is in the ESTABLISHED status, <b>tcp_rcv_esablished</b> is called. Processing of the ESTABLISHED status is separately handled and optimized since it is the most common status. The tcp_rcv_established first executes the header prediction code. The header prediction is also quickly processed to detect in the common state. The common case here is that there is no data to transmit and the received data packet is the packet that must be received next time, i.e., the sequence number is the sequence number that the receiving TCP expects. In this case, the procedure is completed by adding the data to the socket buffer and then transmitting ACK.
<br>
Go forward and you will see the sentence comparing truesize with sk_forward_alloc. It is to check whether there is any free space in the receive socket buffer to add new packet data. If there is, header prediction is "hit" (prediction succeeded). Then __skb_pull is called to remove the TCP header. After that, __skb_queue_tail is called to add the packet to the receive socket buffer. Finally, __tcp_ack_snd_check is called for transmitting ACK if necessary. In this way, packet processing is completed.
<br>
If there is not enough free space, a slow path is executed. The tcp_data_queue function newly allocates the buffer space and adds the data packet to the socket buffer. At this time, the receive socket buffer size is automatically increased if possible. Different from the quick path, tcp_data_snd_check is called to transmit a new data packet if possible. Finally, tcp_ack_snd_check is called to create and transmit the ACK packet if necessary.
<br>
The amount of code executed by the two paths is not much. This is accomplished by optimizing the common case. In other words, it means that the uncommon case will be processed significantly more slowly. The out-of-order delivery is one of the uncommon cases.
</p>
<hr>
