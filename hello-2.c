// simple module to print when insmod and rmmod
#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>

// init and exit hooks that call pr_info()
// must be static cause else global kernel symbol table polluted
static int __init hello_2_init(void)
{
        pr_info("module plugged in.\n");
        return 0;
}

static void __exit hello_2_exit(void)
{
        pr_info("module unplugged.\n");
}

module_init(hello_2_init);
module_exit(hello_2_exit);

MODULE_LICENSE("GPL");
