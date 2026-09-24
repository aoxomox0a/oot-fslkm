#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

// first define struct
static struct file_system_type minifs_type = {
        .name = "minifs",
        .owner = THIS_MODULE,
        .kill_sb = kill_anon_super,
};

// kill_sb = kill_litter_super


// from fs.h
extern int register_filesystem(struct file_system_type *);
extern int unregister_filesystem(struct file_system_type *);

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
