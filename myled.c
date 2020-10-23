#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>     // 追加

MODULE_AUTHOR("Lybfip Ruxeacrh");
MODULE_DESCRIPTION("driver for LED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

static dev_t dev;
static struct cdev cdv;                                     // ここから追記

static ssize_t led_write(struct file* filp, const char* buf, size_t count, loff_t* pos)
{
    printk(KERN_INFO "led_write is called\n");
    return 1;
}

static struct file_operations led_fops = {
    .owner = THIS_MODULE,
    .write = led_write
};                                                          // ここまで

static int __init init_mod(void)
{
    int retval;
    retval = alloc_chrdev_region(&dev, 0, 1, "myled");
    if (retval < 0) {
        printk(KERN_ERR "alloc_chrdev_region failure.\n");
        return retval;
    }
    printk(KERN_INFO "%s is loaded. major:%d\n",__FILE__,MAJOR(dev));

    cdev_init(&cdv, &led_fops);                             // ここから追記
    cdv.owner = THIS_MODULE;
    retval = cdev_add(&cdv, dev, 1);
    if (retval < 0) {
        printk(KERN_ERR "cdev_add faild. major:%d, minor:%d\n",MAJOR(dev),MINOR(dev));
        return retval;
    }                                                       // ここまで

    return 0;
}

static void __exit cleanup_mod(void)
{
    cdev_del(&cdv);                                         // 追加
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "%s is unloaded. major:%d\n",__FILE__,MAJOR(dev));
}

module_init(init_mod);
module_exit(cleanup_mod);