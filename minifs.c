#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/pagemap.h>
#include <linux/slab.h>

#define MAGIC_NBR 0x6461726B

/*TODO:
* add super operations
* add slab mm allocator
* define inode operations
*
*
*/

extern int register_filesystem(struct file_system_type *);
extern int unregister_filesystem(struct file_system_type *);
static const struct super_operations deadfs_ops;
static const struct inode_operations deadfs_dir_inode_operations;
static int deadfs_fill_super(struct super_block *sb, struct fs_context *fc);
static int deadfs_get_tree(struct fs_context *fc);

static inline struct deadfs_inode *DEADFS_I(struct inode *inode);
static void deadfs_free_inode(struct inode *inode);
static void deadfs_alloc_inode(struct inode *inode);

struct deadfs_inode {
        struct inode vfs_inode;
}:

static inline struct deadfs_inode *DEADFS_I(struct inode *inode)
{
        return container_of(inode, struct deadfs_inode, vfs_inode);
}

static const struct inode_operations deadfs_dir_inode_operations = {

}

static const struct super_operations deadfs_ops = {
	.statfs		= simple_statfs,
	.drop_inode	= inode_just_drop,
	.show_options	= ramfs_show_options,
        .alloc_inode    = deadfs_alloc_inode,
        .destroy_inode  = deadfs_destroy_inode,
        .free_inode     = deadfs_free_inode,
};







static struct kmem_cache *deadfs_inode_cachep;

static struct inode *deadfs_alloc_inode(struct super_block *sb)
{
        struct deadfs_inode *di = kmem_cache_alloc(deadfs_inode_cachep, GFP_KERNEL);
        return di ? &di->vfs_inode : NULL;
}

static void deadfs_free_inode(struct inode *inode)
{
        struct deadfs_
}

/*
* sets superblock metadata (sb->)
* allocate root inode in ram and configure
* use default i_op and i_fop structs for inode operations
* d_make_root wraps inode in root dentry /
* fs mounted in /mnt and root dir ready in ram
* */
static int deadfs_fill_super(struct super_block *sb, struct fs_context *fc)
{
        struct inode *inode;

        // include/fs/super_types.h
        sb->s_magic = MAGIC_NBR;
        sb->s_blocksize = PAGE_SIZE;
        sb->s_blocksize_bits = PAGE_SHIFT;
        sb->s_op = &deadfs_ops;

        /* root inode */
        inode = new_inode(sb);
        if (inode == NULL) return -ENOMEM;

        /* configure inode */
        inode->i_ino = 1;
        inode->i_mode = S_IFDIR | 0755;
        inode->i_op = &simple_dir_inode_operations;
        inode->i_fop = &simple_dir_operations;

        /* inode live inside sb */
        sb->s_root = d_make_root(inode);
        if (!sb->s_root) return -ENOMEM;

        return 0;
}

static int deadfs_get_tree(struct fs_context *fc){
        /* invoke get_tree
         extern int get_tree_nodev(struct fs_context *fc,
			 int (*fill_super)(struct super_block *sb,
					   struct fs_context *fc));*/
        return get_tree_nodev(fc, deadfs_fill_super);
}

/* scope: mounting, unmounting, parsing mount options */
static const struct fs_context_operations deadfs_context_ops = {
        .get_tree = deadfs_get_tree,
};

static int deadfs_init_fs_context(struct fs_context *fc){
        fc->ops = &deadfs_context_ops;
        return 0;
}

static struct file_system_type deadfs_type = {
        .name = "deadfs",
        .owner = THIS_MODULE,
        .kill_sb = kill_anon_super, /* umoutn: destroy root dentry, frees root inode memory, deallocates sb struct */
        .init_fs_context = deadfs_init_fs_context /* called on mount */
};

static void init_once(void *foo)
{
	struct deadfs_inode_info *di = foo;

	inode_init_once(&di->vfs_inode);
}

static int __init init_inodecache(void)
{
	struct kmem_cache_args args = {
		.useroffset = offsetof(struct ext4_inode_info, i_data),
		.usersize = sizeof_field(struct deadfs_inode_info, i_data),
		.use_freeptr_offset = true,
		.freeptr_offset = offsetof(struct dead_inode_info, i_flags),
		.ctor = init_once,
	};

	deadfs_inode_cachep = kmem_cache_create("deadfs_inode_cache",
				sizeof(struct deads_inode_info),
				&args,
				SLAB_RECLAIM_ACCOUNT | SLAB_ACCOUNT);

	if (deadfs_inode_cachep == NULL)
		return -ENOMEM;
	return 0;
}

static void destroy_inodecache(void)
{
	/*
	 * Make sure all delayed rcu free inodes are flushed before we
	 * destroy cache.
	 */
	rcu_barrier();
	kmem_cache_destroy(deadfs_inode_cachep);
}

static int __init deadfs_init(void)
{
        ret = init_inodecache();
        if (ret != 0) return ret

        /* insmod */
        int ret = register_filesystem(&deadfs_type);
        if (ret != 0){
                pr_err("Error");
                return ret;
        }
        return 0;
}

static void __exit deadfs_exit(void)
{
        /* rmmod */
        int ret = unregister_filesystem(&deadfs_type);
        if (ret != 0){
                pr_err("Error");
        }
}

module_init(deadfs_init);
module_exit(deadfs_exit);

MODULE_LICENSE("GPL");
