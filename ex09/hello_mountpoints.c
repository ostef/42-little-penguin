#include <linux/module.h>
#include <linux/printk.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stefan Oumansour");
MODULE_DESCRIPTION("Simple module to list mountpoints");

static int __init hellomountpoints_init(void)
{
}

static void __exit hellomountpoints_exit(void)
{
}

module_init(hellomountpoints_init);
module_exit(hellomountpoints_exit);
