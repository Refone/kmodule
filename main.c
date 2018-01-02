#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

static int __init hi(void)
{
    printk(KERN_INFO "module m1 being loaded.\n");


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
