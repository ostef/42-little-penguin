// SPDX-License-Identifier: GPL-2.0-only

#include <linux/module.h>
#include <linux/printk.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stefan Oumansour");
MODULE_DESCRIPTION("Simple hello world kernel module");

static int __init hellokeyboard_init(void)
{
	pr_info("Hello Keyboard!\n");
	return 0;
}

static void __exit hellokeyboard_exit(void)
{
	pr_info("Cleaning up hello keyboard module\n");
}

module_init(hellokeyboard_init);
module_exit(hellokeyboard_exit);
