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