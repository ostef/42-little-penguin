#include <linux/module.h>
#include <linux/printk.h>
#include <linux/init.h>

static int __init helloworld_init(void)
{
        printk(KERN_INFO "Hello World!\n");
        return 0;
}

static void __exit helloworld_exit(void)
{
        printk(KERN_INFO "Cleaning up module.\n");
}

module_init(helloworld_init);
module_exit(helloworld_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stefan Oumansour");
MODULE_DESCRIPTION("Simple hello world kernel module");
