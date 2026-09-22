#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

// from fs.h
extern int register_filesystem(struct file_system_type *);
extern int unregister_filesystem(struct file_system_type *);

// how register and unregister in init and exit functions
static int __init minifs_init(void)
{
        // somehow register
        // from fs.h
        // use registered fs?
        register_filesystem();
        
}

static int __exit minifs_exit(void)
{
        // unregister
}

module_init(minifs_init);
module_exit(minifs_exit);

MODULE_LICENSE("GPL");
