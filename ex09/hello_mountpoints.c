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

// static int show_mount_info(struct seq_file *file, void *v)
// {
// 	struct mnt_namespace *ns;
// 	struct mount *mnt;
// 	char *buf;

// 	ns = current->nsproxy->mnt_ns;
// 	if (!ns)
// 		return 0;

// 	buf = kmalloc(PAGE_SIZE, GFP_KERNEL);
// 	if (!buf)
// 		return -ENOMEM;

// 	seq_printf(file, "%-8s %-16s %s\n", "FS", "DEVICE", "MOUNTPOINT");
// 	seq_puts(file, "----------------------------------------\n");

// 	list_for_each_entry(mnt, &ns->list, mnt_list) {
// 		struct path p = {
// 			.mnt = &mnt->mnt,
// 			.dentry = mnt->mnt.mnt_root
// 		};
// 		char *path;

// 		path = d_path(&p, buf, PAGE_SIZE);
// 		if (IS_ERR(path))
// 			continue;

// 		seq_printf(file, "%-8s %-16s %s\n", mnt->mnt.mnt_sb->s_type->name, mnt->mnt.mnt_sb->s_id, path);
// 	}

// 	kfree(buf);
// 	return 0;
// }

static int walk_mounts(struct seq_file *file, void *p)
{
	struct mount *mnt = p;

	struct path p = {
		.mnt = &mnt->mnt,
		.dentry = mnt->mnt.mnt_root,
	};

	char *buf = kmalloc(PAGE_SIZE, GFP_KERNEL);
	char *path = d_path(&p, buf, PAGE_SIZE);

	if (!IS_ERR(path)) {
		seq_printf(file, "%-8s %-16s %s\n", mnt->mnt.mnt_sb->s_type->name, mnt->mnt.mnt_sb->s_id, path);
	}

	kfree(buf);

	list_for_each_entry(child, &mnt->mnt_mounts, mnt_child) {
		walk_mounts(file, child);
	}
}

static int mymounts_procfs_open(struct inode *inode, struct file *file)
{
	struct mnt_namespace *ns = current->nsproxy->mnt_ns;
	struct mount *root;

	root = real_mount(ns->root);
	return single_open(file, walk_mounts, root);
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
