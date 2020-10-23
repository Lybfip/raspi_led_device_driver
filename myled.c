#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>   //追加

MODULE_AUTHOR("Lybfip Ruxeacrh");
MODULE_DESCRIPTION("driver for LED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;    //追加

static ssize_t led_write(struct file* filp, const char* buf, size_t count, loff_t* pos)
{
    printk(KERN_INFO "led_write is called\n");
    return 1;
}

static struct file_operations led_fops = {
    .owner = THIS_MODULE,
    .write = led_write
};

static int __init init_mod(void)
{
    int retval;
    retval = alloc_chrdev_region(&dev, 0, 1, "myled");
    if (retval < 0) {
        printk(KERN_ERR "alloc_chrdev_region failure.\n");
        return retval;
    }
    printk(KERN_INFO "%s is loaded. major:%d\n",__FILE__,MAJOR(dev));

    cdev_init(&cdv, &led_fops);
    cdv.owner = THIS_MODULE;
    retval = cdev_add(&cdv, dev, 1);
    if (retval < 0) {
        printk(KERN_ERR "cdev_add faild. major:%d, minor:%d\n",MAJOR(dev),MINOR(dev));
        return retval;
    }

    cls = class_create(THIS_MODULE,"myled");
    if(IS_ERR(cls)) {
        printk(KERN_ERR "class_create failed.");
        return PTR_ERR(cls);
    }
    device_create(cls, NULL, dev, NULL, "myled%d", MINOR(dev)); //追加

    return 0;
}

static void __exit cleanup_mod(void)
{
    cdev_del(&cdv);
    device_destroy(cls, dev);   //追加
    class_destroy(cls);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "%s is unloaded. major:%d\n",__FILE__,MAJOR(dev));
}

module_init(init_mod);
module_exit(cleanup_mod);