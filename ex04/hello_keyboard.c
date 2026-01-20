#include <linux/module.h>
#include <linux/printk.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stefan Oumansour");
MODULE_DESCRIPTION("Simple hello world kernel module, with keyboard plugging detection");

static int __init hellokeyboard_init(void)
{
        pr_info("Hello World!\n");
        return 0;
}

static void __exit hellokeyboard_exit(void)
{
        pr_info("Cleaning up module.\n");
}

module_init(hellokeyboard_init);
module_exit(hellokeyboard_exit);
