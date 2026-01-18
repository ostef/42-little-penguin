#include <linux/module.h>
#include <linux/printk.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/miscdevice.h>

#define LOGIN "soumanso"
#define LOGIN_LENGTH (sizeof(LOGIN) - 1)

static ssize_t hellodevice_read(struct file *f, char __user *buff, size_t len, loff_t *offset)
{
        if (!buff) {
                return -EINVAL;
        }
        if (*offset >= LOGIN_LENGTH) {
                return 0;
        }

        size_t copy_length = min(LOGIN_LENGTH - *offset, len);
        size_t copied = copy_length - copy_to_user(buff, LOGIN + *offset, copy_length);
        *offset += copied;

        return (ssize_t)copied;
}

static ssize_t hellodevice_write(struct file *f, const char __user *buff, size_t len, loff_t *offset)
{
        char text[LOGIN_LENGTH];

        if (!buff || len != LOGIN_LENGTH) {
                return -EINVAL;
        }
        if (copy_from_user(text, buff, len) > 0) {
                return -EIO;
        }
        if (memcmp(text, LOGIN, LOGIN_LENGTH) != 0) {
                return -EINVAL;
        }
        return LOGIN_LENGTH;
}

static struct file_operations file_ops = {
        .owner = THIS_MODULE,
        .read = hellodevice_read,
        .write = hellodevice_write,
};

static struct miscdevice device = {
        .minor = MISC_DYNAMIC_MINOR,
        .name = "fortytwo",
        .fops = &file_ops,
};

static int __init hellodevice_init(void)
{
        if (misc_register(&device) != 0) {
                pr_err("Could not register misc device driver\n");
                return -1;
        }

        pr_info("Created '%s' misc device driver (minor=%d)\n", device.name, device.minor);
        return 0;
}

static void __exit hellodevice_exit(void)
{
        misc_deregister(&device);
        pr_info("Removed device '%s'\n", device.name);
}

module_init(hellodevice_init);
module_exit(hellodevice_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stefan Oumansour");
MODULE_DESCRIPTION("Simple misc device driver");
