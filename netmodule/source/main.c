#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/net.h>
#include <linux/in.h>
#include <linux/kthread.h>

static struct sockaddr_in servaddr;
static struct socket *serv_sock = NULL;
static struct socket *connect_sock = NULL;
static struct task_struct *accept_worker = NULL;

int ktcp_accept_worker(void)
{
	int r = -1;

	printk("[SOURCE] accept worker fired!\n");

	//allow_signal(SIGKILL|SIGSTOP);

	r = sock_create(PF_INET,SOCK_STREAM,IPPROTO_TCP,&connect_sock);
	if (r<0) {
			printk("[SOURCE] connect_sock create error.\n");
			return -1;
	}		

	r = serv_sock->ops->accept(serv_sock, connect_sock, 0);
	if (r<0) {
		printk("[SOURCE] accept error.\n");
		sock_release(connect_sock);
		return -1;
	}

	//handle connection
	//sock_release(connect_sock);

	printk("[SOURCE] accept worker retired.\n");

	return 0;
}

static int __init hi(void)
{
	int r = -1;

    printk(KERN_INFO "[SOURCE] netmodule being loaded.\n");

	r = sock_create(PF_INET, SOCK_STREAM, IPPROTO_TCP, &serv_sock);
	if (r<0) {
		printk("[CUCKOO-SENDER] serv_sock create error.\n");
		return -1;
	}		

	memset(&servaddr,0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(11603);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	r = serv_sock->ops->bind(serv_sock,
					(struct sockaddr *)&servaddr,
					sizeof (servaddr));
	if (r<0) {
		printk("[CUCKOO-SENDER] serv_sock bind error.\n");
		sock_release(serv_sock);
		return -1;
	}		

	r = serv_sock->ops->listen(serv_sock, 1);
	if (r<0) {
		printk("[CUCKOO-SENDER] serv_sock listen error.\n");
		sock_release(serv_sock);
		return -1;
	}		

	//sender_serving = 1;

	accept_worker = kthread_run((void *)ktcp_accept_worker, NULL, "accepter");

    return 0;
}

static void __exit bye(void)
{
	kthread_stop(accept_worker);
    printk(KERN_INFO "[SOURCE] netmodule being unloaded.\n");
}

module_init(hi);
module_exit(bye);

MODULE_AUTHOR("NAM.REKCUF.EMOS");
MODULE_LICENSE("GPL"); 
