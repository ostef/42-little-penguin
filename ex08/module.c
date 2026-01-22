#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Louis Solofrizzo <louis@ne02ptzero.me>");
MODULE_DESCRIPTION("Useless module");

static ssize_t myfd_read(struct file *fp, char __user *user, size_t size, loff_t *offs);
static ssize_t myfd_write(struct file *fp, const char __user *user, size_t size, loff_t *offs);

static struct file_operations myfd_fops = {
        .owner = THIS_MODULE,
        .read = &myfd_read,
        .write = &myfd_write
};

static struct miscdevice myfd_device = {
        .minor = MISC_DYNAMIC_MINOR,
        .name = "reverse",
        .fops = &myfd_fops
};

static char str[PAGE_SIZE];
static size_t str_length;
static char *tmp;
static DEFINE_MUTEX(myfd_mutex);

static int __init myfd_init(void)
{
        return misc_register(&myfd_device);
}

static void __exit myfd_cleanup(void)
{
        kfree(tmp);
        misc_deregister(&myfd_device);
}

ssize_t myfd_read(struct file *fp, char __user *user, size_t size, loff_t *offs)
{
        mutex_lock(&myfd_mutex);

        tmp = kmalloc(PAGE_SIZE, GFP_KERNEL);
        if (!tmp) {
                return -ENOMEM;
        }

        ssize_t t = strlen(str) - 1;

        ssize_t i;
        for (i = 0; t >= 0; t--, i++) {
                tmp[i] = str[t];
        }

        tmp[i] = 0;

        ssize_t bytes_read = simple_read_from_buffer(user, size, offs, tmp, i + 1);

        kfree(tmp);

        mutex_unlock(&myfd_mutex);

        return o;
}

ssize_t myfd_write(struct file *fp, const char __user *user, size_t size, loff_t *offs)
{
        mutex_lock(&myfd_mutex);

        ssize_t bytes_written = simple_write_to_buffer(str, PAGE_SIZE - 1, offs, user, size);

        mutex_unlock(&myfd_mutex);

        return bytes_written;
}

module_init(myfd_init);
module_exit(myfd_cleanup);
