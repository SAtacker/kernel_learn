/**
 * @file char_ddriver.h
 * @author Shreyas Atre (shreyasatre16@gmail.com)
 * @brief A Kernel Loadable Module (Here a character device driver) that communicates with userspace through ioctl , file descriptor and provides checksum of the data 
 * @version 1.0
 * @date 2021-03-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>

#define DEVICE_NAME "char_ddriver"
#define CLASS_NAME "chardd"
#define _MAX_DATA_ 256
#define WR_VALUE _IOW('a', 1, char *)
#define RD_VALUE _IOR('a', 2, char *)
#define SHA1_LENGTH 20

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SHREYAS ATRE");
MODULE_DESCRIPTION("CHAR DEVICE DRIVER THAT GENERATES CHECKSUM");
MODULE_VERSION("1.0");

/**
 * @brief This is the major number of the module
 * 
 */
static int major_num;

/**
 * @brief This is a data buffer for userspace data
 * 
 */
static char data[_MAX_DATA_] = {0};

/**
 * @brief This is used to store the current data size or termination of null string in @param data
 * 
 */
static short _data_size;

/**
 * @brief This is used to store the checksum 
 * 
 */
static int _chk = 0;
module_param(_chk,int,0660);

/**
 * @brief This is the class of driver , udev creates a node in /dev with this 
 * 
 */
static struct class *cdd_class = NULL;

/**
 * @brief This is used to create a device with sysfs
 * 
 */
static struct device *cdd = NULL;

/**
 * @brief create proc entry with device name , mode and parent
 * 
 */
static struct proc_dir_entry *ent;

/**
 * @brief This is the device open function 
 * 
 * @return int (0) on Success
 */
static int c_open(struct inode *, struct file *);

/**
 * @brief Releases the driver
 * 
 * @return int (0) on Success
 */
static int c_release(struct inode *, struct file *);

/**
 * @brief This function is invoked when requested to read the VFS
 * 
 * @return ssize_t Returns 0 for Success , -EFAULT on failure
 */
static ssize_t c_read(struct file *, char *, size_t, loff_t *);

/**
 * @brief This function is invoked when requested to write into the VFS
 * 
 * @return ssize_t Returns 0 for Success , -EFAULT on failure
 */
static ssize_t c_write(struct file *, const char *, size_t, loff_t *);

/**
 * @brief This is the ioctl call handler
 * 
 * @return long -ENOTTY on Failure , 0 on success
 */
static long device_ioctl(struct file *, unsigned int, long unsigned int);

/**
 * @brief Get the simplest possible checksum of the characters 
 * @param data A pointer to a data buffer
 * @return int returns the checksum
 */
static int get_chksum(char *);
