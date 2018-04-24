#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <net/sock.h>
#include <linux/tcp.h>
#include <linux/in.h>
#include <asm/uaccess.h>
#include <linux/socket.h>
#include <linux/slab.h>

#define PORT 11603
#define DATA_LEN 8

static struct sockaddr_in address;
static struct socket *sock = NULL;

static uint32_t create_address(uint8_t a1,uint8_t a2,uint8_t a3,uint8_t a4)
{
	uint32_t address;

	address = 0;
	address = address | (a1 << 24);
	address = address | (a2 << 16);
	address = address | (a3 << 8);
	address = address | (a4);

	return address;
}

int send_sync_buf(struct socket *sock, 
		  const char *buf, 
		  const size_t length, 
		  unsigned long flags)
{
    struct msghdr msg;
    struct iovec iov;
    //struct iov_iter iter;
    //int len, written = 0, left = length;
    int len = 0;
    mm_segment_t oldmm;

    //copy_to_iter(buf, length, &iter);
	
    iov.iov_base = buf;
    iov.iov_len = length;

    msg.msg_name     = 0;
    msg.msg_namelen  = 0;
    //msg.msg_iov      = &iov;
    //msg.msg_iovlen   = 1;
    msg.msg_iter.type = 0;
    msg.msg_iter.count = length;
    msg.msg_iter.iov_offset = 0;
    msg.msg_iter.iov = &iov;

    //copy_to_iter(buf, length, &(msg.msg_iter));
    msg.msg_control  = NULL;
    msg.msg_controllen = 0;
    msg.msg_flags    = flags;

    oldmm = get_fs(); set_fs(KERNEL_DS);

//repeat_send:
    //msg.msg_iov->iov_len = left;
    //msg.msg_iov->iov_base = (char *) buf +
    //                            written;

    len = sock_sendmsg(sock, &msg);
    
    set_fs(oldmm);

    printk("[ktcp_r]: send bytes: %d\n", len);

    return len;
}

void send_reply(struct socket *sock, char *str)
{
    send_sync_buf(sock, str, strlen(str),
                  MSG_DONTWAIT);
}

static int __init hi(void)
{
	int r = -1;
	char test[DATA_LEN + 1]={0};

	printk("[ktcp_r]: module init.\n");

	memset(test, 'A', DATA_LEN);
	r = sock_create(PF_INET, SOCK_STREAM, IPPROTO_TCP, &sock);
	if (r < 0) {
		printk("[ktcp_r]: tsock create");
		sock_release(sock);
		return -1;
	}

	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	//address.sin_addr.s_addr = inet_addr("192.168.10.196");
	address.sin_addr.s_addr = htonl(create_address(192, 168, 12, 96));

	r = sock->ops->connect(sock,
					(struct sockaddr *) &address,
					sizeof(address), O_RDWR);

	if (r==0) {
		printk("[ktcp_r]: connect to server.\n");
	} else {
		printk("[ktcp_r]: connect failed.\n");
	}

	send_reply(sock, test);

    return 0;
}

static void __exit bye(void)
{
	sock_release(sock);
	printk("[ktcp_r] module exit.\n");
}

module_init(hi);
module_exit(bye);

MODULE_AUTHOR("NAM.REKCUF.EMOS");
MODULE_LICENSE("GPL"); 
