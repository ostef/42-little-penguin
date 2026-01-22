#include <linux/module.h>
#include <linux/printk.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stefan Oumansour");
MODULE_DESCRIPTION("Simple module to list mountpoints");

//static void show_mount_info(struct seq_file *file, struct mount *mnt)
//{
//        if (strcmp(mnt->mnt_devname, "rootfs") == 0) {
//                return;
//        }
//
//        struct path mnt_path = {
//                .dentry = mnt->mnt.mnt_root,
//                .mnt = &mnt->mnt,
//        };
//
//        struct super_block *block = mnt_path.dentry->d_sb;
//
//        if (!sb->s_op->show_devname) {
//                seq_puts(file, mnt->mnt_devname ? mnt->mnt_devname : "unnamed");
//        } else {
//                sb->s_op->show_devname(file, mnt_path.dentry);
//        }
//
//        seq_puts(file, " ");
//        seq_path(file, &mnt_path, " \t\n\\");
//        seq_puts(file, " ");
//}

static int mymounts_procfs_show(struct seq_file *file, void *)
{
        return 0;
}

static int mymounts_procfs_open(struct inode *inode, struct file *file)
{
        return single_open(file, mymounts_procfs_show, NULL);
}

static struct proc_dir_entry *mymounts;

static struct proc_ops mymounts_fops = {
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
        if (mymounts) {
                proc_remove(mymounts);
        }
        pr_info("Shutdown module\n");
}

module_init(mymounts_init);
module_exit(mymounts_exit);
