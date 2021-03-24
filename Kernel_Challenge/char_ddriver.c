/**
 * @file char_ddriver.c
 * @author Shreyas Atre (shreyasatre16@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-03-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "char_ddriver.h"

/**
 * @brief This struct holds pointers to functions defined by this driver
 * 
 */
static struct file_operations fops =
    {
        .owner = THIS_MODULE,
        .open = c_open,
        .read = c_read,
        .write = c_write,
        .release = c_release,
        .unlocked_ioctl = device_ioctl,
};

static struct proc_ops pops =
    {
        .proc_open = c_open,
        .proc_read = c_read,
        .proc_write = c_write,
};

static int __init cdd_init(void)
{
    printk(KERN_INFO "%s : Initializing \n", DEVICE_NAME);
    major_num = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_num < 0)
    {
        printk(KERN_ALERT "%s : Failed to register major number\n", DEVICE_NAME);
        return major_num;
    }
    printk(KERN_INFO "%s : Registered with major number %d\n", DEVICE_NAME, major_num);
    ent = proc_create(DEVICE_NAME, 0660, NULL, &pops);
    if (ent == NULL)
    {
        return -ENOMEM;
    }
    cdd_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(cdd_class))
    {
        unregister_chrdev(major_num, DEVICE_NAME);
        printk(KERN_ALERT "%s : Failed to create class\n", DEVICE_NAME);
        return PTR_ERR(cdd_class);
    }
    cdd = device_create(cdd_class, NULL, MKDEV(major_num, 0), NULL, DEVICE_NAME);
    if (IS_ERR(cdd))
    {
        class_destroy(cdd_class);
        unregister_chrdev(major_num, DEVICE_NAME);
        printk(KERN_ALERT "%s : Failed to create device\n", DEVICE_NAME);
        return PTR_ERR(cdd);
    }
    printk(KERN_INFO "%s : Device created \n", DEVICE_NAME);
    return 0;
}

static void __exit cdd_clean(void)
{
    device_destroy(cdd_class, MKDEV(major_num, 0));
    class_unregister(cdd_class);
    class_destroy(cdd_class);
    unregister_chrdev(major_num, DEVICE_NAME);
    proc_remove(ent);
    printk(KERN_INFO "%s : module cleanup\n", DEVICE_NAME);
}

static int c_open(struct inode *iptr, struct file *fptr)
{
    printk(KERN_INFO "%s : Opened\n", DEVICE_NAME);
    try_module_get(THIS_MODULE);
    return 0;
}

static ssize_t c_read(struct file *fptr, char *buffer, size_t len, loff_t *off)
{
    int err_cnt = copy_to_user(buffer, data, _data_size);
    printk(KERN_INFO "%s : Data inside data buffer : %s , Data Size: %d\n", DEVICE_NAME, data, _data_size);
    if (err_cnt == 0)
    {
        printk(KERN_INFO "%s : Sent %d chars to user\n", DEVICE_NAME, _data_size);
        return _data_size = 0;
    }
    else
    {
        printk(KERN_INFO "%s : Failed to send %d chars\n", DEVICE_NAME, _data_size);
        return -EFAULT;
    }
}

static ssize_t c_write(struct file *fptr, const char __user *buffer, size_t len, loff_t *off)
{
    if (*off > _MAX_DATA_)
    {
        printk(KERN_ALERT "%s : Offset is larger than _MAX_DATA_\n", DEVICE_NAME);
    }
    else if (*off == 0)
    {
        strcpy(data, "");
        printk(KERN_INFO "%s : previous data is %s\n", DEVICE_NAME, data);
        if (copy_from_user(data, buffer, len) != 0)
        {
            return -EFAULT;
        }
    }
    _data_size = strlen(data);
    printk(KERN_INFO "%s : Data received : %s , Data Size: %d\n", DEVICE_NAME, data, _data_size);
    return len;
}

static int c_release(struct inode *iptr, struct file *fptr)
{
    printk(KERN_INFO "%s : Device released\n", DEVICE_NAME);
    module_put(THIS_MODULE);
    return 0;
}

static int get_chksum(char *data)
{
    printk(KERN_INFO "%s : Getting checksum\n", DEVICE_NAME);
    int chk = 0;
    int len = strlen(data), i = 0;
    for (i = 0; i < len; i++)
    {
        chk += data[i] << 8;
    }
    printk(KERN_INFO "%s : Checksum is %d\n", DEVICE_NAME, chk);
    return chk;
}

static long device_ioctl(struct file *filep, unsigned int cmd, long unsigned int arg)
{
    switch (cmd)
    {
    case RD_VALUE:
        printk(KERN_INFO "%s : Data inside data buffer : %s , Data Size: %d\n", DEVICE_NAME, data, _data_size);
        _chk = 0;
        _chk = get_chksum(data);
        sprintf(data, "\nChecksum : [%d]\n", _chk);
        copy_to_user((char *)arg, data, _data_size);
        _data_size = 0;
        break;

    case WR_VALUE:
        strcpy(data, "");
        printk(KERN_INFO "%s : previous data is %s\n", DEVICE_NAME, data);
        copy_from_user(data, (char *)arg, _MAX_DATA_);
        _data_size = strlen(data);
        printk(KERN_INFO "%s : Data received : %s , Data Size: %d\n", DEVICE_NAME, data, _data_size);
        break;

    default:
        return -ENOTTY;
    }
    return 0;
}

module_init(cdd_init);
module_exit(cdd_clean);
