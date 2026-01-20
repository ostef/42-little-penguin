#include <linux/module.h>
#include <linux/printk.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stefan Oumansour");
MODULE_DESCRIPTION("Simple hello world kernel module");

static int __init hellomodule_init(void)
{
        pr_info("Hello World!\n");
        return 0;
}

static void __exit hellomodule_exit(void)
{
        pr_info("Cleaning up module.\n");
}

module_init(hellomodule_init);
module_exit(hellomodule_exit);
