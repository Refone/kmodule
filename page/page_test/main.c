#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/mm.h>
#include <linux/string.h>

#include <asm/page.h>

static struct page *p;
u64 shadow_paddr;
u64 *shadow_vaddr;

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

	shadow_paddr = paddr | ((1ULL)<<(0+9+9+9+12));
	shadow_vaddr = ioremap(shadow_paddr, PAGE_SIZE);

	printk("Spaddr[%016llx] Svaddr[%016llx]\n", shadow_paddr, (u64)shadow_vaddr);
	printk("content:[%016llx]\n", *shadow_vaddr);
    
	return 0;
}

static void __exit bye(void)
{
	iounmap(shadow_vaddr);
	__free_page(p);	
    printk(KERN_INFO "page alloc being unloaded.\n");
}

module_init(hi);
module_exit(bye);

MODULE_LICENSE("GPL"); 
