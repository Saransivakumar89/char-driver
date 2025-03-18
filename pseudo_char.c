#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/kdev_t.h>
#include<linux/uaccess.h>
#include<linux/err.h>


#undef pr_fmt
#define pr_fmt(fmt) "%s :" fmt,__func__

#define BUFFER_SIZE 1024

/*Pseudo Device*/

char buffer_driver[BUFFER_SIZE];

/*Device Number*/

dev_t device_number;

/* cdev */

struct cdev pseudo_cdev;

loff_t pseudo_lseek(struct file * filp, loff_t offset, int whence)
{
    loff_t temp;
    pr_info("lseek requested\n");
    pr_info("current value of the file position + %lld\n",filp->f_pos);

    switch(whence)
    {
        case SEEK_SET:
            if((offset > BUFFER_SIZE)||(offset < 0 ))
                return -EINVAL;

            filp->f_pos = offset;
            break;
           
        case SEEK_CUR:
           temp = filp->f_pos + offset;
           if((temp > BUFFER_SIZE)||(temp <  0))
                return -EINVAL;

           filp->f_pos += offset;
           break;
        
        case SEEK_END:
            temp = filp->f_pos + offset;

            if((temp > BUFFER_SIZE)||(temp < 0 ))
                    return -EINVAL;

            filp->f_pos = BUFFER_SIZE + offset;
            break;
        default:
             return -EINVAL;
    }

    pr_info("new value of the file position = %lld\n", filp->f_pos);

	return filp->f_pos;
}
	
ssize_t pseudo_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos)
{
    pr_info("read requested for %zu bytes\n",count);
    pr_info("current file file position = %lld\n",*f_pos);

    if((*f_pos + count) > BUFFER_SIZE)
    {
        count = BUFFER_SIZE - *f_pos;
    }

    if(copy_to_user(buff,&buffer_driver[*f_pos],count))
    {
        return -EFAULT;
    }
	
    *f_pos +=count;

    pr_info("Number of bytes successfully read = %zu\n",count);
    pr_info("Updated file position = %lld\n",*f_pos);

    return count;
}
	
ssize_t pseudo_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos)
{
    pr_info("write requested for %zu bytes\n",count);
    pr_info("current file file position = %lld\n",*f_pos);
    

    /*Adjust the count*/
    if((*f_pos + count) > BUFFER_SIZE)
    {
        count = BUFFER_SIZE - *f_pos;
    }

    if(!count)
    {
	pr_err("No Space Left\n");
        return -ENOMEM;
    }
    
    /*Copy from user*/

    if(copy_from_user(&buffer_driver[*f_pos],buff,count))
    {
        return -EFAULT;
    }
	    
    /*update the current file position*/

    *f_pos +=count;

    pr_info("Number of bytes sucessfully written = %zu\n",count);
    pr_info("Updated file position = %lld\n",*f_pos);

    /*REturn number of bytes which have been successfully write*/

    return count;
}
	
int pseudo_open(struct inode * inode, struct file *filp)
{
    pr_info("open was successfull\n");
	return 0;
}
	
int pseudo_release(struct inode * inode, struct file * filp)
{
    pr_info("release was successfull\n");
    return 0;
}

struct file_operations pseudo_fops={
    .owner = THIS_MODULE,
    .open = pseudo_open,
    .release = pseudo_release,
    .write = pseudo_write,
    .llseek = pseudo_lseek,
    .read = pseudo_read
};

struct class *class_pseudo;

struct device * device_pseudo;


static int __init pseudo_init(void)
{
    int ret ;

    ret = alloc_chrdev_region(&device_number,0,1,"pseudo_devices");
    
    if(ret < 0)
    {
        pr_err("Alloc chrdev failed\n");
        goto out;
    }

    pr_info("Device number <major>:<minor> = %d:%d\n",MAJOR(device_number),MINOR(device_number));

    cdev_init(&pseudo_cdev,&pseudo_fops);

    pseudo_cdev.owner = THIS_MODULE;

    ret = cdev_add(&pseudo_cdev,device_number,1);
   
    if(ret < 0)
    {
        pr_err("Cdev add failed\n");
        goto unreg_chrdev;
    }

    class_pseudo = class_create(THIS_MODULE,"pseudo_class");

    if(IS_ERR(class_pseudo))
    {
        pr_err("Class Creation failed\n");
        ret = PTR_ERR(class_pseudo);
        goto cdev_del;
    }

    device_pseudo =  device_create(class_pseudo,NULL,device_number,NULL,"pseudo");

    if(IS_ERR(device_pseudo))
    {
        pr_err("Device creation failed\n");
        ret = PTR_ERR(device_pseudo);
        goto class_del;
    }

    pr_info("Module init successfull\n");

    return 0;

class_del:
    class_destroy(class_pseudo);
cdev_del:
    cdev_del(&pseudo_cdev);    
unreg_chrdev:    
    unregister_chrdev_region(device_number,1);
out :
    pr_info("Module Insertion failed\n");
    return ret;
}

static void __exit pseudo_exit(void)
{
    device_destroy(class_pseudo,device_number);
    class_destroy(class_pseudo);
    cdev_del(&pseudo_cdev);
    unregister_chrdev_region(device_number,1);
    pr_info("module unloaded\n");
}

module_init(pseudo_init);
module_exit(pseudo_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Saran Sivakumar");
MODULE_DESCRIPTION("A Simple Pseudo Char Device Driver");
MODULE_INFO(Board,"Beagle Bone Black Rev C");


