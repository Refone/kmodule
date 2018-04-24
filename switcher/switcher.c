#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

extern int lrf_memory_on;
extern int lrf_memory_print;
extern int lrf_memory_net;

static int __init hi(void)
{
    printk(KERN_INFO "module m1 being loaded.\n");
    
	lrf_memory_on = !lrf_memory_on;
    //lrf_memory_print = !lrf_memory_print;
    //lrf_memory_net = !lrf_memory_net;

	return 0;
}

static void __exit bye(void)
{
    printk(KERN_INFO "module m1 being unloaded.\n");

	lrf_memory_on = !lrf_memory_on;
    //lrf_memory_print = !lrf_memory_print;
    //lrf_memory_net = !lrf_memory_net;
}

module_init(hi);
module_exit(bye);

MODULE_AUTHOR("NAM.REKCUF.EMOS");
MODULE_LICENSE("GPL"); 
