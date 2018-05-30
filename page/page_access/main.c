#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/mm.h>
#include <linux/string.h>

#include <asm/page.h>

u64 paddr = 0x607fe000;
u64 shadow_paddr;
u64 *shadow_vaddr;

static int __init hi(void)
{

    printk(KERN_INFO "page access [%016llx]\n", paddr);

	shadow_paddr = paddr | ((1ULL)<<(0+9+9+9+12));
	shadow_vaddr = ioremap(shadow_paddr, PAGE_SIZE);
	//shadow_vaddr = ioremap(paddr, PAGE_SIZE);
	//shadow_vaddr = __va(shadow_paddr);

	printk("Spaddr[%016llx] Svaddr[%016llx]\n", shadow_paddr, (u64)shadow_vaddr);
	//if(shadow_vaddr){
	printk("content:[%016llx]\n", *shadow_vaddr);
	//	iounmap(shadow_vaddr);
	//}

    return 0;
}

static void __exit bye(void)
{
	iounmap(shadow_vaddr);
    printk(KERN_INFO "page access being unloaded.\n");
}

module_init(hi);
module_exit(bye);

MODULE_LICENSE("GPL"); 
