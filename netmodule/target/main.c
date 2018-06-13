#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/net.h>		// struct socket
#include <linux/in.h>		// struct sockaddr_in

static struct sockaddr_in serv_addr;
static struct socket *connect_sock = NULL;

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

static int __init hi(void)
{
	int r = -1;

    printk(KERN_INFO "[TARGET] target netmodule being loaded.\n");

	r = sock_create(PF_INET, SOCK_STREAM, IPPROTO_TCP, &connect_sock);
	if (r < 0) {
		printk("[TARGET]: connect sock create failed.\n");
		return -1;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(11603);		// migration port
	serv_addr.sin_addr.s_addr = htonl(create_address(192, 168, 13, 92));

	r = connect_sock->ops->connect(connect_sock,
					(struct sockaddr *) &serv_addr,
					sizeof(serv_addr), O_RDWR);

	if (r==0) {
		printk("[TARGET]: connect to server.\n");
	} else {
		printk("[TARGET]: connect failed.\n");
	}

    return 0;
}

static void __exit bye(void)
{
    printk(KERN_INFO "module m1 being unloaded.\n");
}

module_init(hi);
module_exit(bye);

MODULE_AUTHOR("NAM.REKCUF.EMOS");
MODULE_LICENSE("GPL"); 
