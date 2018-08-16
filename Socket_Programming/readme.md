<h1>Socket Programming Basics</h1>
<hr>
<h2>How to Transmit Data</h2>
<p>First, this is a path used to transmit data when an application calls the write system call.</p><br>
<hr>
<p><strong>
SYSCALL_DEFINE3(write, unsigned int, fd, const char __user *, buf, ...)
 
{
 
struct file *file;
 
[...]
 
file = fget_light(fd, &fput_needed);
 
[...] ===>
 
ret = filp->f_op->aio_write(&kiocb, &iov, 1, kiocb.ki_pos);
 
 
 
struct file_operations {
 
[...]
 
ssize_t (*aio_read) (struct kiocb *, const struct iovec *, ...)
 
ssize_t (*aio_write) (struct kiocb *, const struct iovec *, ...)
 
[...]
 
};
 
 
 
static const struct file_operations socket_file_ops = {
 
[...]
 
.aio_read = sock_aio_read,
 
.aio_write = sock_aio_write,
 
[...]
 
};

</strong></p>
<hr>
