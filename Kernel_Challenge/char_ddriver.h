/**
* @file char_ddriver.h
* @Synopsis  This is the 
* @author Shreyas Atre
* @version 1.0
* @date 2021-03-19
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>

#define DEVICE_NAME "char_ddriver"
#define CLASS_NAME "chardd"
#define _MAX_DATA_ 256
#define WR_VALUE _IOW('a',1,char*)
#define RD_VALUE _IOR('a',2,char*)
#define SHA1_LENGTH 20

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SHREYAS ATRE");
MODULE_DESCRIPTION("CHAR DEVICE DRIVER THAT GENERATES CHECKSUM");
MODULE_VERSION("1.0");

static int major_num;
static char data[_MAX_DATA_] = {0};
static short _data_size;
static unsigned char *_chk;
static struct class* cdd_class = NULL;
static struct device* cdd = NULL;

static int c_open(struct inode*,struct file *);
static int c_release(struct inode*,struct file *);
static ssize_t c_read(struct file*,char*,size_t,loff_t *);
static ssize_t c_write(struct file*,const char*,size_t,loff_t *);
static long device_ioctl(struct file *, unsigned int , long unsigned int );
static unsigned char* get_chksum(char *,char*);

