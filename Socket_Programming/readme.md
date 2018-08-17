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