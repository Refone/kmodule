#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

#include <xen/interface/xen.h>
#include <asm/xen/hypercall.h>

static int __init hi(void)
{
    printk(KERN_INFO "hypercall test module init.\n");

	//_hypercall1(int, cuckoo_copy, 4096);
	//HYPERVISOR_cuckoo_copy(4096);
//	int fd, ret;
//	char * message;
//	privcmd_hypercall_t cuckoo_copy={
//			__HYPERVISOR_cuckoo_copy,
//			{4096,0,0,0,0}
//	};
//	fd = open("/proc/xen/privcmd", O_RDWR);
//	if(fd<0){
//			perror("can't open /proc/xen/privcmd");
//			exit(1);
//	} else {
//			printf("privcmd's fd = %d\n", fd);
//	}
//
//	ret = ioctl(fd, IOCTL_PRIVCMD_HYPERCALL, &cuckoo_copy);
//	printk("ret = %d\n", ret);

    return 0;
}

static void __exit bye(void)
{
    printk(KERN_INFO "hypercall test module exit.\n");
}

module_init(hi);
module_exit(bye);

MODULE_AUTHOR("NAM.REKCUF.EMOS");
MODULE_LICENSE("GPL"); 
