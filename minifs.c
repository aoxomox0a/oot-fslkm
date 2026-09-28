#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include PAGE_SHIFT
#include PAGE_SIZE

#define MAGIC_NBR 0x6461726B

static int minifs_fill_super(struct super_block *sb, struct fs_context *fc){
        // include/fs/super_types.h
        sb->s_magic = MAGIC_NBR;
        sb->s_blocksize = PAGE_SIZE;
        sb->s_blocksize_bits = PAGE_SHIFT;
        // super operations helper function
        sb->s_op = &simple_super_operations;
        // root inode
        inode = new_inode(sb);
        if (inode == NULL) return -ENOMEM;
        // inode nb
        inode->i_ino = 1;
        // dir and permission
        inode->i_mode = S_IFDIR | 0755;
        // use prewritten struct
        inode->i_op = &simple_dir_inode_operations;
        inode->i_fop = &simple_dir_operations;

        // d entry
        sb->s_root = d_make_root(inode);
        if (!sb->s_root) return -ENOMEM;
        return 0;
}

static int minifs_get_tree(struct fs_context *fc){
        // invoke get_tree
        // extern int get_tree_nodev(struct fs_context *fc,
			 //int (*fill_super)(struct super_block *sb,
					//   struct fs_context *fc));
        return get_tree_nodev(fc, minifs_fill_super);
}

static const struct fs_context_operations minifs_context_ops = {
        // create moutnable superblock
        .get_tree = minifs_get_tree,
};

// first define struct
static int minifs_init_fs_context(struct fs_context *fc){
        fc->ops = &minifs_context_ops;
        return 0;
}

static struct file_system_type minifs_type = {
        .name = "minifs",
        .owner = THIS_MODULE,
        .kill_sb = kill_anon_super,
        .init_fs_context = minifs_init_fs_context // struct to fs context
};

// kill_sb = kill_litter_super


// from fs.h
extern int register_filesystem(struct file_system_type *);
extern int unregister_filesystem(struct file_system_type *);

// superblock allocation



// how register and unregister in init and exit functions
static int __init minifs_init(void)
{
        // somehow register
        // from fs.h
        // use registered fs?
        //register_filesystem(&file_system_type); // pass address 

        // add to global list of available filesystems
        int ret = register_filesystem(&minifs_type);
        if (ret != 0){
                // error and abort
                pr_err("Error");
                return ret;
        }
        return 0;
}

static void __exit minifs_exit(void)
{
        // unregister
        int ret = unregister_filesystem(&minifs_type);
        if (ret != 0){
                // handle error
                pr_err("Error");
        }
}

module_init(minifs_init);
module_exit(minifs_exit);

MODULE_LICENSE("GPL");
