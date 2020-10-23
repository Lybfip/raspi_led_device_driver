#include <linux/module.h>
#include <linux/fs.h>       // 追加

MODULE_AUTHOR("Lybfip Ruxeacrh");
MODULE_DESCRIPTION("driver for LED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

static dev_t dev;   // 追加

static int __init init_mod(void)
{
    int retval;                                             // ここから
    retval = alloc_chrdev_region(&dev, 0, 1, "myled");
    if (retval < 0) {
        printk(KERN_ERR "alloc_chrdev_region failure.\n");
        return retval;
    }                                                       // ここまで追加

    printk(KERN_INFO "%s is loaded. major:%d\n",__FILE__,MAJOR(dev));   // 変更
    return 0;
}

static void __exit cleanup_mod(void)
{
    unregister_chrdev_region(dev, 1);               // 追加
    printk(KERN_INFO "%s is unloaded. major:%d\n",__FILE__,MAJOR(dev)); // 変更
}

module_init(init_mod);
module_exit(cleanup_mod);