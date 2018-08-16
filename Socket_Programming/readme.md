<h1>Socket Programming Basics</h1>
<hr>
<h2>How to Transmit Data</h2>
<p>First, this is a path used to transmit data when an application calls the write system call.</p><br>
<hr>
<!-- HTML generated using hilite.me --><div style="background: #ffffff; overflow:auto;width:auto;border:solid gray;border-width:.1em .1em .1em .8em;padding:.2em .6em;"><pre style="margin: 0; line-height: 125%">SYSCALL_DEFINE3(write, <span style="color: #333399; font-weight: bold">unsigned</span> <span style="color: #333399; font-weight: bold">int</span>, fd, <span style="color: #008800; font-weight: bold">const</span> <span style="color: #333399; font-weight: bold">char</span> __user <span style="color: #333333">*</span>, buf, ...)
 
{
 
<span style="color: #008800; font-weight: bold">struct</span> file <span style="color: #333333">*</span>file;
 
[...]
 
file <span style="color: #333333">=</span> fget_light(fd, <span style="color: #333333">&amp;</span>fput_needed);
 
[...] <span style="color: #333333">===&gt;</span>
 
ret <span style="color: #333333">=</span> filp<span style="color: #333333">-&gt;</span>f_op<span style="color: #333333">-&gt;</span>aio_write(<span style="color: #333333">&amp;</span>kiocb, <span style="color: #333333">&amp;</span>iov, <span style="color: #0000DD; font-weight: bold">1</span>, kiocb.ki_pos);
 
 
 
<span style="color: #008800; font-weight: bold">struct</span> file_operations {
 
[...]
 
<span style="color: #333399; font-weight: bold">ssize_t</span> (<span style="color: #333333">*</span>aio_read) (<span style="color: #008800; font-weight: bold">struct</span> kiocb <span style="color: #333333">*</span>, <span style="color: #008800; font-weight: bold">const</span> <span style="color: #008800; font-weight: bold">struct</span> iovec <span style="color: #333333">*</span>, ...)
 
<span style="color: #333399; font-weight: bold">ssize_t</span> (<span style="color: #333333">*</span>aio_write) (<span style="color: #008800; font-weight: bold">struct</span> kiocb <span style="color: #333333">*</span>, <span style="color: #008800; font-weight: bold">const</span> <span style="color: #008800; font-weight: bold">struct</span> iovec <span style="color: #333333">*</span>, ...)
 
[...]
 
};
 
 
 
<span style="color: #008800; font-weight: bold">static</span> <span style="color: #008800; font-weight: bold">const</span> <span style="color: #008800; font-weight: bold">struct</span> file_operations socket_file_ops <span style="color: #333333">=</span> {
 
[...]
 
.aio_read <span style="color: #333333">=</span> sock_aio_read,
 
.aio_write <span style="color: #333333">=</span> sock_aio_write,
 
[...]
 
};
</pre></div>

<hr>
