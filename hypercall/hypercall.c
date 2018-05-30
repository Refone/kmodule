#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <asm/kvm_para.h> 

static int __init hi(void)
{
    printk(KERN_INFO "hypercall test module init.\n");

	kvm_hypercall0(24);//EPT APPEND
//	kvm_hypercall0(22);//EPT DUMP
	kvm_hypercall0(23);//EPT SET RO
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
	kvm_hypercall0(21);	//EPT CLEAR
    printk(KERN_INFO "hypercall test module exit.\n");
}

module_init(hi);
module_exit(bye);

MODULE_AUTHOR("NAM.REKCUF.EMOS");
MODULE_LICENSE("GPL"); 
