// SPDX-License-Identifier: GPL-2.0-only

#include <linux/module.h>
#include <linux/printk.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/mount.h>
#include <linux/nsproxy.h>
#include <linux/path.h>
#include <linux/dcache.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stefan Oumansour");
MODULE_DESCRIPTION("Simple module to list mountpoints");

// Open /proc/self/mounts and read each line, print them back to seq_file
static int show_mounts(struct seq_file *file, void *data)
{
	loff_t mounts_file_pos = 0;
	struct file *mounts_file = filp_open("/proc/self/mounts", O_RDONLY, 0);

	if (IS_ERR(mounts_file)) {
		pr_err("Could not open /proc/self/mounts file for reading\n");
		return -PTR_ERR(mounts_file);
	}

	char *buff = kmalloc(PAGE_SIZE, GFP_KERNEL);
	if (!buff) {
		filp_close(mounts_file, NULL);
		return -ENOMEM;
	}

	while (true) {
		ssize_t read_bytes = kernel_read(mounts_file, buff, PAGE_SIZE - 1, &mounts_file_pos);

		if (read_bytes <= 0) {
			break;
		}

		buff[read_bytes] = 0;

		char *line = buff;

		while (line) {
			char *newline = strchr(line, '\n');

			if (newline) {
				*newline = 0;
				newline += 1;
			}

			if (strlen(line) == 0) {
				line = newline;
				continue;
			}

			char *device = strsep(&line, " ");
			char *mount = strsep(&line, " ");
			if (!device && !mount) {
				line = newline;
				continue;
			}

			if (device) {
				seq_printf(file, "%s ", device);
			} else {
				seq_printf(file, "? ");
			}

			if (mount) {
				seq_printf(file, "%s\n", mount);
			} else {
				seq_printf(file, "?\n");
			}

			line = newline;
		}
	}

	kfree(buff);
	filp_close(mounts_file, NULL);

	return 0;
}

static int mymounts_procfs_open(struct inode *inode, struct file *file)
{
	// struct mnt_namespace *ns = current->nsproxy->mnt_ns;
	// struct mount *root;

	// root = real_mount(ns->root);
	// return single_open(file, walk_mounts, root);
	return single_open(file, show_mounts, NULL);
}

static struct proc_dir_entry *mymounts;

static const struct proc_ops mymounts_fops = {
	.proc_open = mymounts_procfs_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
};

static int __init mymounts_init(void)
{
	mymounts = proc_create("mymounts", 0444, NULL, &mymounts_fops);
	if (!mymounts) {
		pr_err("proc_create failed\n");
		return -PTR_ERR(mymounts);
	}

	pr_info("Created /proc/mymounts\n");
	return 0;
}

static void __exit mymounts_exit(void)
{
	if (mymounts)
		proc_remove(mymounts);

	pr_info("Shutdown module\n");
}

module_init(mymounts_init);
module_exit(mymounts_exit);
