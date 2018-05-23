#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

static char* module_name = "template module";

static int __init kmodule_init(void)
{
    printk(KERN_INFO "%s being loaded.\n", module_name);


    return 0;
}

static void __exit kmodule_exit(void)
{
    printk(KERN_INFO "%s being unloaded.\n", module_name);
}

module_init(kmodule_init);
module_exit(kmodule_exit);

MODULE_LICENSE("GPL"); 
