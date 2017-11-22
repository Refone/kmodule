#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

static int whatever;
static int answer = 42;
static char __initdata howdymsg[] = "Good day, eh?";
static char __exitdata exitmsg[] = "Taking off, eh?";

void useless(void)
{
    printk(KERN_INFO "I am totally useless.\n");
}

static int __init hi(void)
{
    printk(KERN_INFO "module m1 being loaded.\n");
    printk(KERN_INFO "%s\n", howdymsg);
    printk(KERN_INFO "The answer is %d.\n", answer);
    answer = 999;
    return 0;
}

static void __exit bye(void)
{
    printk(KERN_INFO "module m1 being unloaded.\n");
    printk(KERN_INFO "%s\n", exitmsg);
    printk(KERN_INFO "The answer is now %d.\n", answer);
}

module_init(hi);
module_exit(bye);

MODULE_AUTHOR("NAM.REKCUF.EMOS");
MODULE_LICENSE("GPL"); 
