/*
Sample-Simple char drv example code
*/

#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/uaccess.h>
#include<linux/slab.h>

#define DRVNAME "sample_chrdev"
#define BUFFER_LEN 4000
int count=1;
dev_t mydev;
struct cdev *sample_cdev;
char *kernel_buffer;

static int sample_char_drv_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int sample_char_drv_release(struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t sample_char_drv_read(struct file *file, char __user *user_ptr, size_t len, loff_t *off)
{
	int copylen;

	copylen=copy_to_user(user_ptr,kernel_buffer,len);
	return(len-copylen);
	
}

static ssize_t sample_char_drv_write(struct file *file, const char __user *user_ptr,size_t len,loff_t *off)
{
	int copylen;
	copylen=copy_from_user(kernel_buffer,user_ptr,len);
	return(len-copylen);
}

struct file_operations  sample_char_drv_fops = {
	.open = sample_char_drv_open,
	.release = sample_char_drv_release,
	.read = sample_char_drv_read,
	.write = sample_char_drv_write,
};

static int __init sample_char_drv_init(void)
{
	int ret;
	ret=alloc_chrdev_region(&mydev,0,count,DRVNAME);
	if(ret < 0)
		goto fail1;
	sample_cdev=cdev_alloc();
	if(sample_cdev == NULL)
		goto fail2;
	cdev_init(sample_cdev,&sample_char_drv_fops);
	cdev_add(sample_cdev,mydev,count);
	
	printk("major num=%d minor num=%d\n",MAJOR(mydev),MINOR(mydev));
	kernel_buffer=kmalloc(GFP_KERNEL,BUFFER_LEN);
	return 0;
fail1:

fail2:
	unregister_chrdev_region(mydev,count);
	return -1;

}
static void __exit sample_char_drv_exit(void)
{
	kfree(kernel_buffer);
	cdev_del(sample_cdev);
	unregister_chrdev_region(mydev,count);
}

module_init(sample_char_drv_init);
module_exit(sample_char_drv_exit);
MODULE_AUTHOR("Alexander");
MODULE_DESCRIPTION("Sample char drv module");
MODULE_LICENSE("GPL");
