#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/net.h>
#include <net/sock.h>
#include <linux/tcp.h>
#include <linux/in.h>
#include <asm/uaccess.h>
#include <linux/file.h>
#include <linux/socket.h>
//#include <linux/smp_lock.h>
#include <linux/slab.h>

#define PORT 11603
#define MAX_RECV_LEN 32

static struct sockaddr_in servaddr;
static struct socket *serv_sock = NULL;
static struct socket *c_sock = NULL;

static char buf[MAX_RECV_LEN + 1];

//static bool sanity(const struct iov_iter *i)
//{
//    struct pipe_inode_info *pipe = i->pipe;
//    int idx = i->idx;
//    int next = pipe->curbuf + pipe->nrbufs;
//    if (i->iov_offset) {
//
//        struct pipe_buffer *p;
//        if (unlikely(!pipe->nrbufs))
//
//            goto Bad;// pipe must be non-empty
//                if (unlikely(idx != ((next - 1) & (pipe->buffers - 1))))
//                    goto Bad;// must be at the last buffer...
//
//                        p = &pipe->bufs[idx];
//        if (unlikely(p->offset + p->len != i->iov_offset))
//            goto Bad;// ... at the end of segment
//    } else {
//
//        if (idx != (next & (pipe->buffers - 1)))
//            goto Bad; // must be right after the last buffer
//    }
//    return true;
//Bad:
//    printk(KERN_ERR "idx = %d, offset = %zd\n", i->idx, i->iov_offset);
//    printk(KERN_ERR "curbuf = %d, nrbufs = %d, buffers = %d\n",
//            pipe->curbuf, pipe->nrbufs, pipe->buffers);
//    for (idx = 0; idx < pipe->buffers; idx++)
//        printk(KERN_ERR "[%p %p %d %d]\n",
//                pipe->bufs[idx].ops,
//                pipe->bufs[idx].page,
//                pipe->bufs[idx].offset,
//                pipe->bufs[idx].len);
//    WARN_ON(1);
//    return false;
//}

int read_response(struct socket *sock, char *str)
{
    struct msghdr msg;
    struct iov_iter iter;
    struct iovec iov;
    int len = 0;
    mm_segment_t oldfs;

    oldfs = get_fs();
    set_fs(KERNEL_DS);

    iov.iov_base = str;
    iov.iov_len = MAX_RECV_LEN;

    iter.type = 0;
    iter.iov_offset = 0;
    iter.count = MAX_RECV_LEN;
    iter.iov = &iov;
    iter.nr_segs = 1;

    msg.msg_name     = 0;
    msg.msg_namelen  = 0;
    //msg.msg_iov      = &iov;
    //msg.msg_iovlen   = 1;
    msg.msg_iter     = iter;
    msg.msg_control  = NULL;
    msg.msg_controllen = 0;
    msg.msg_flags    = 0;

    printk("sock_recvmsg before.\n");
    //while (len==0) {
    len = sock_recvmsg(sock, &msg, 0);
    //}

    printk("sock_recvmsg len %d.\n", len);

    set_fs(oldfs);

    //len = copy_from_iter(str, MAX_RECV_LEN, &(msg.msg_iter));
    printk("copy_from_iter len %d.\n", msg.msg_iter.count);

    return len;
}

static int __init hi(void)
{
		int r = -1;

		printk("module init.\n");

		r = sock_create(PF_INET, SOCK_STREAM, IPPROTO_TCP, &serv_sock);
		if (r<0) {
			printk("sock create error.\n");
            sock_release(serv_sock);
            return -1;
		}		

		memset(&servaddr,0, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(PORT);
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

		r = serv_sock->ops->bind(serv_sock,
						(struct sockaddr *)&servaddr,
						sizeof (servaddr));

		if (r<0) {
			printk("sock bind error.\n");
            sock_release(serv_sock);
            return -1;
		}		

		r = serv_sock->ops->listen(serv_sock, 1);
		if (r<0) {
			printk("sock listen error.\n");
            sock_release(serv_sock);
            return -1;
		}		

		//r = control->ops->connect(control, 
		//				(struct sockaddr *) &servaddr, 
		//				sizeof(servaddr), O_RDWR);
		//c_scok=(struct socket*)kmalloc(sizeof(struct socket),GFP_KERNEL);
		
		r = sock_create(PF_INET,SOCK_STREAM,IPPROTO_TCP,&c_sock);
		if (r<0) {
			printk("c_sock create error.\n");
            sock_release(serv_sock);
            sock_release(c_sock);
            return -1;
		}		

		//for (;;)
		//{
			r = serv_sock->ops->accept(serv_sock, c_sock, 0);
			if (r<0) {
				printk("r<0\n");
			}
            
            memset(buf, 0, MAX_RECV_LEN + 1);
            r = read_response(c_sock, buf);

			printk("buf:%s %d bytes received.\n", buf, r);
	//		if (r<0) {
	//			continue;
	//		}
	//		else {
	//			printk("sock connected.\n");
	//		}
	//	}

		return 0;
}

static void __exit bye(void)
{
	sock_release(serv_sock);
    sock_release(c_sock);
	printk("module exit.\n");
}


module_init(hi);
module_exit(bye);

MODULE_AUTHOR("NAM.REKCUF.EMOS");
MODULE_LICENSE("GPL"); 
