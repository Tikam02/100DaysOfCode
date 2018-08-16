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
