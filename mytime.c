//Create a character device 'mytime' that returns the current time using current_kernel_time() and getnstimeofday().

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/time.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "mytime"
#define BUFFER_SIZE 200

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arpita Chakraborty");

static int open(struct inode *, struct file *);
static int release(struct inode *, struct file *);
static ssize_t read(struct file *, char *, size_t, loff_t *);
static ssize_t write(struct file *, const char *, size_t, loff_t *);

// Major number assigned to our device driver
static int MajorNum;

//stores user space string
static char BufferforTime[BUFFER_SIZE];





// The file operations structure. To support the file operations.
static struct file_operations fops = {
	.read = read,
	.write = write,
	.open = open,
	.release = release
};


 // call when the module is loaded 
static int mytime_init(void)
{
	MajorNum  = register_chrdev(0, DEVICE_NAME, &fops);

	if (MajorNum < 0) 
	{
		printk(KERN_ALERT "Registering failed. Returned %d\n", MajorNum);
	  	return MajorNum;
	}

	printk(KERN_INFO "The major number is %d.\n", MajorNum);
	printk(KERN_INFO "Run 'mknod /dev/%s c %d 0' before using this device(if not already done)!\n", DEVICE_NAME, MajorNum);

	return 0;
}




//call when the module is removed
void mytime_exit(void)
{
	unregister_chrdev(MajorNum,DEVICE_NAME);
	printk(KERN_INFO "Exiting module!\n");
}




//The function is invoked when another program calls this charecter device.
static ssize_t read(struct file *filp, char __user *buff, size_t len, loff_t *offset)
{
	struct timespec kernel_time;
	struct timespec func_time;

	kernel_time = current_kernel_time();
	getnstimeofday(&func_time);

	snprintf(BufferforTime, BUFFER_SIZE, "current_kernel_time: %ld\n getnstimeofday: %ld\n", (kernel_time.tv_sec*1000000000)+kernel_time.tv_nsec, (func_time.tv_sec*1000000000)+func_time.tv_nsec);

	// If position is behind the end of a file we have nothing to read
	if( *offset >= sizeof(BufferforTime) )
		return 0;

	if( *offset + len > sizeof(BufferforTime) )
		len = sizeof(BufferforTime) - *offset;

	if( copy_to_user(buff, BufferforTime + *offset, len) != 0 )
		return -EFAULT;	

	*offset = *offset+len;
        printk(KERN_INFO "%s\n", BufferforTime);
	return len;
}




//does nothing
static int release(struct inode *inode, struct file *file)
{
	module_put(THIS_MODULE);
	return 0;
}





//does nothing
static int open(struct inode *inode, struct file *file)
{
	try_module_get(THIS_MODULE);
	return 0;
}






//does nothing - not supported
static ssize_t write(struct file *filp, const char *buff, size_t len, loff_t *offset)
{
	printk(KERN_ALERT "This operation isn't supported!\n");
	return -EINVAL;
}

module_init(mytime_init);
module_exit(mytime_exit);
