#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kvm_para.h>

//static unsigned long get_cr3(void)
//{
//    unsigned long val;
//    asm volatile("mov %%cr3,%0\n\t" : "=r" (val), "=m" (__force_order));
//    return val;
//}
//
//static unsigned long get_cr0(void)
//{
//    unsigned long val;
//    asm volatile("mov %%cr0,%0\n\t" : "=r" (val), "=m" (__force_order));
//    return val;
//}
//
//static unsigned long get_rsp(void)
//{
//    unsigned long val;
//    asm volatile("mov %%rsp,%0\n\t" : "=r" (val), "=m" (__force_order));
//    return val;
//}
//
//static unsigned long get_rbp(void)
//{
//    unsigned long val;
//    asm volatile("mov %%rbp,%0\n\t" : "=r" (val), "=m" (__force_order));
//    return val;
//}
//
//static void set_cr3(unsigned long val)
//{
//    asm volatile("mov %0,%%cr3": : "r" (val), "m" (__force_order));
//}
//
//static void set_cr0(unsigned long val)
//{
//    asm volatile("mov %0,%%cr1": : "r" (val), "m" (__force_order));
//}
//
//static void set_rsp(unsigned long val)
//{
//    asm volatile("mov %0,%%rsp": : "r" (val), "m" (__force_order));
//}
//
//static void set_rbp(unsigned long val)
//{
//    asm volatile("mov %0,%%rbp": : "r" (val), "m" (__force_order));
//}
#define ASM_VMX_VMFUNC ".byte 0x0f, 0x01, 0xd4"
static inline unsigned char __vmfunc(u32 eptp, u32 func)
{
		unsigned char error;

		__asm __volatile(ASM_VMX_VMFUNC
						: "=q" (error) : "c" (eptp), "a" (func) : "memory");
		return error;
}

static int __init vmfunc_init(void)
{
	int error=0;
	printk("vmfunc_init invoke!\n");

	//error = kvm_hypercall0(21);
	//printk("error: %x", error);

	//error = kvm_hypercall0(22);
	//printk("error: %x", error);

	//error = kvm_hypercall0(20);
	//printk("error: %x", error);

	error = __vmfunc(1,0);
	printk("vmfunc_init end! error:[%x]\n", error);

    return 0;
}

static void __exit vmfunc_exit(void)
{
	printk("vmfunc_exit invoked~\n");
}

module_init(vmfunc_init);
module_exit(vmfunc_exit);

MODULE_AUTHOR("NAM.REKCUF.EMOS");
MODULE_LICENSE("GPL");
