#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/mm.h>
#include <linux/string.h>

#include <asm/page_types.h>

static char* module_name = "Mem detect module";
extern struct e820map *e820_saved;

static int __init kmodule_init(void)
{
	int i;
	u64 paddr, shadow_paddr;
	u64* vaddr, shadow_vaddr;
	char content[PAGE_SIZE];

    printk(KERN_INFO "%s being loaded.\n", module_name);

	//e820_print_map("LRF");
	if (e820_saved) {
		printk("[LRF MEM] e820_saved->nr_map [%d]\n", 
						e820_saved->nr_map);
		for (i=0;i<e820_saved->nr_map;i++) {
			printk("[LRF MEM] e820: addr:[%016llx] size:[%016llx] type:[%d]\n",
							e820_saved->map[i].addr, 
							e820_saved->map[i].size, 
							e820_saved->map[i].type);
		}
	} else {
		printk("[LRF MEM] OK GG\n");
	}

	paddr = 0x100000;
	vaddr = __va(paddr);

	printk("vaddr:[%016llx]\n", *vaddr);

	memcpy(content, vaddr, PAGE_SIZE);

	printk("content:\n");
	printk("[%016llx]\n", *((u64*)(content+0)));
	printk("[%016llx]\n", *((u64*)(content+8)));
	printk("[%016llx]\n", *((u64*)(content+16)));

	//shadow_paddr = paddr | ((1ULL)<<(0+9+9+9+12));
	//shadow_vaddr = ioremap(shadow_paddr, PAGE_SIZE);

	//printk("content:[%016llx]\n", *shadow_vaddr);

    return 0;
}

static void __exit kmodule_exit(void)
{
    printk(KERN_INFO "%s being unloaded.\n", module_name);
}

module_init(kmodule_init);
module_exit(kmodule_exit);

MODULE_LICENSE("GPL"); 
