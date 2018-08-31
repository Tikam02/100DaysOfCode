<h1>Socket Programming With UDP</h1>
<hr>
<p>processes running on different machines communi-
cate with each other by sending messages into sockets. We said that each process is
analogous to a house and the process’s socket is analogous to a door. The application
resides on one side of the door in the house; the transport-layer protocol resides on
the other side of the door in the outside world. The application developer has control
of everything on the application-layer side of the socket; however, it has little control
of the transport-layer side.
<br>
Before the sending process can push a packet of
data out the socket door, when using UDP, it must first attach a destination address
to the packet. After the packet passes through the sender’s socket, the Internet will
use this destination address to route the packet through the Internet to the socket in
the receiving process. When the packet arrives at the receiving socket, the receiving
process will retrieve the packet through the socket, and then inspect the packet’s
contents and take appropriate action.
</p>
