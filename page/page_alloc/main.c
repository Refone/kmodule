#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/mm.h>
#include <linux/string.h>

#include <asm/page.h>

static struct page *p;

static int __init hi(void)
{
	u64 paddr;
	u64 *vaddr;
	
	p = alloc_page(GFP_KERNEL | __GFP_ZERO);

    printk(KERN_INFO "page alloc being loaded.\n");

	if (!p) {
		printk("page alloc failed. no memory");
		return 0;
	}

	paddr = page_to_phys(p);
	vaddr = __va(paddr);

	memset(vaddr, 'A', PAGE_SIZE);

	printk("paddr[%016llx] vaddr[%016llx]\n", paddr, (u64)vaddr);
	printk("content:[%016llx]\n", *vaddr);

    return 0;
}

static void __exit bye(void)
{
	__free_page(p);	
    printk(KERN_INFO "page alloc being unloaded.\n");
}

module_init(hi);
module_exit(bye);

MODULE_LICENSE("GPL"); 
