// SPDX-License-Identifier: GPL-2.0-only

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
MODULE_DESCRIPTION("Simple misc device driver");

#define LOGIN "soumanso"
#define LOGIN_LENGTH (sizeof(LOGIN) - 1)

static ssize_t id_read(struct file *f, char __user *buff, size_t len, loff_t *offset)
{
	if (!buff)
		return -EINVAL;
	if (*offset >= LOGIN_LENGTH)
		return 0;

	size_t copy_length = min(LOGIN_LENGTH - *offset, len);
	size_t copied = copy_length - copy_to_user(buff, LOGIN + *offset, copy_length);

	*offset += copied;

	return (ssize_t)copied;
}

static ssize_t id_write(struct file *f, const char __user *buff, size_t len, loff_t *offset)
{
	char text[LOGIN_LENGTH];

	if (!buff || len != LOGIN_LENGTH)
		return -EINVAL;
	if (copy_from_user(text, buff, len) > 0)
		return -EIO;
	if (memcmp(text, LOGIN, LOGIN_LENGTH) != 0)
		return -EINVAL;

	return LOGIN_LENGTH;
}

static const struct file_operations id_file_ops = {
	.owner = THIS_MODULE,
	.read = id_read,
	.write = id_write,
};

static ssize_t jiffies_read(struct file *f, char __user *buff, size_t len, loff_t *offset)
{
	char jiffies_buffer[32];
	int written = snprintf(jiffies_buffer, sizeof(jiffies_buffer), "%llu", get_jiffies_64());

	return simple_read_from_buffer(buff, len, offset, jiffies_buffer, written);
}

static const struct file_operations jiffies_file_ops = {
	.owner = THIS_MODULE,
	.read = jiffies_read,
};

static size_t foo_length;
static char foo_buffer[PAGE_SIZE];
static DEFINE_MUTEX(foo_buffer_mutex);

static ssize_t foo_read(struct file *f, char __user *buff, size_t len, loff_t *offset)
{
	if (!buff)
		return -EINVAL;
	if (*offset >= foo_length)
		return 0;

	mutex_lock(&foo_buffer_mutex);

	size_t copy_length = min(foo_length - *offset, len);
	size_t copied = copy_length - copy_to_user(buff, foo_buffer + *offset, copy_length);

	*offset += copied;

	mutex_unlock(&foo_buffer_mutex);

	return (ssize_t)copied;
}

static ssize_t foo_write(struct file *f, const char __user *buff, size_t len, loff_t *offset)
{
	if (!buff)
		return -EINVAL;

	mutex_lock(&foo_buffer_mutex);

	size_t copy_length = min(PAGE_SIZE - foo_length, len);
	size_t copied = copy_length - copy_from_user(foo_buffer + foo_length, buff, copy_length);

	foo_length += copied;
	*offset = foo_length;

	mutex_unlock(&foo_buffer_mutex);

	return (ssize_t)copied;
}

static const struct file_operations foo_file_ops = {
	.owner = THIS_MODULE,
	.read = foo_read,
	.write = foo_write,
};

static struct dentry *debug_dir;
static struct dentry *id_file;
static struct dentry *jiffies_file;
static struct dentry *foo_file;

static int __init hellodebugfs_init(void)
{
	debug_dir = debugfs_create_dir("fortytwo", NULL);
	if (!debug_dir) {
		pr_err("Could not create debugfs directory 'fortytwo'\n");
		return -PTR_ERR(debug_dir);
	}

	id_file = debugfs_create_file("id", 0666, debug_dir, NULL, &id_file_ops);
	if (!id_file) {
		pr_err("Could not create 'id' file\n");
		debugfs_remove_recursive(debug_dir);
		return -PTR_ERR(id_file);
	}

	jiffies_file = debugfs_create_file("jiffies", 0444, debug_dir, NULL, &jiffies_file_ops);
	if (!jiffies_file) {
		pr_err("Could not create 'jiffies' file\n");
		debugfs_remove_recursive(debug_dir);
		return -PTR_ERR(jiffies_file);
	}

	foo_file = debugfs_create_file("foo", 0644, debug_dir, NULL, &foo_file_ops);
	if (!foo_file) {
		pr_err("Could not create 'foo' file\n");
		debugfs_remove_recursive(debug_dir);
		return -PTR_ERR(foo_file);
	}

	pr_info("Initialized debugfs module\n");

	return 0;
}

static void __exit hellodebugfs_exit(void)
{
	debugfs_remove_recursive(debug_dir);
	pr_info("Cleaned up debugfs module\n");
}

module_init(hellodebugfs_init);
module_exit(hellodebugfs_exit);
