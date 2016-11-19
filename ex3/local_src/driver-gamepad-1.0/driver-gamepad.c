#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/moduleparam.h>
#include <linux/kdev_t.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/signal.h>
#include <asm/siginfo.h>
#include <linux/interrupt.h>
#include "efm32gg.h"


#define BASE_MINOR 0
#define DEV_COUNT 1
#define DRIVER_NAME "gamepad"
#define GPIO_EVEN_IRQ 17
#define GPIO_ODD_IRQ 18

static dev_t dev_number;
struct fasync_struct* async_queue;
struct cdev gamepad_cdev;
struct class *cl;

static int __init gamepad_init(void);
static void __exit gamepad_cleanup(void);
static irqreturn_t gpio_interrupt_handler(int, void*, struct pt_regs*);

static int gamepad_open(struct inode*, struct file*);
static int gamepad_release(struct inode*, struct file*);
static ssize_t gamepad_read(struct file*, char* __user, size_t, loff_t*);
static int gamepad_fasync(int, struct file*, int mode);
module_init(gamepad_init);
module_exit(gamepad_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

static struct file_operations gamepad_fops = {
  .owner   = THIS_MODULE,
  .open    = gamepad_open,
  .release = gamepad_release,
  .read    = gamepad_read,
  .fasync  = gamepad_fasync,  // asyncronous notification
};


irqreturn_t gpio_interrupt_handler(int irq, void* dev_id, struct pt_regs* regs){
	iowrite32(ioread32(GPIO_IF), GPIO_IFC);
	printk(KERN_INFO "Interrupt: Button is toggled\n");
		if (async_queue) {
			kill_fasync(&async_queue, SIGIO, POLL_IN);
		}
	return IRQ_HANDLED;
}


static int gamepad_fasync(int fd, struct file* filp, int mode) {
    return fasync_helper(fd, filp, mode, &async_queue);
}


static int __init gamepad_init(void){
	int number;
	number = alloc_chrdev_region(&dev_number, BASE_MINOR, DEV_COUNT, DRIVER_NAME);
	if ( number < 0){
		printk(KERN_ALERT "Error alloc_chdev_region\n");
		return -1;
	}


	//request_mem_region(start, length of region, name)
	//requesting control of entire PC register
	if (request_mem_region(GPIO_PC_BASE, 0x24, DRIVER_NAME) == NULL){
		printk(KERN_ALERT "GPIO_PC_BASE request denied\n");
		return -1;
	}

	//Init cdev
	printk(KERN_INFO "Initializing cdev.\n");
	cdev_init(&gamepad_cdev, &gamepad_fops);
    gamepad_cdev.owner = THIS_MODULE;
	cdev_add(&gamepad_cdev, dev_number, DEV_COUNT);

	//Make driver available as file. 
	printk("Creating class\n");
	cl = class_create(THIS_MODULE, DRIVER_NAME);
	device_create(cl, NULL, dev_number, NULL, DRIVER_NAME);

	//Setting up GPIO
	iowrite32(0x33333333, GPIO_PC_MODEL); //setting pin 0-7 as input
	iowrite32(0xff, GPIO_PC_DOUT); //enable pullup for inputs

	//setup interrupts
	//request_irq(irq, handler, irqflags, devname, dev_id)
	printk("Setting up interrupts\n");
	request_irq(GPIO_EVEN_IRQ, (irq_handler_t)gpio_interrupt_handler, 0, DRIVER_NAME, &gamepad_cdev);
	request_irq(GPIO_ODD_IRQ, (irq_handler_t)gpio_interrupt_handler, 0, DRIVER_NAME, &gamepad_cdev);

	iowrite32(0x22222222, GPIO_EXTIPSELL);
	iowrite32(0xff, GPIO_EXTIRISE);
	iowrite32(0xff, GPIO_EXTIFALL);
	iowrite32(0x00ff, GPIO_IEN);
	iowrite32(0xff, GPIO_IFC);


	printk("Hello World, here is your module speaking\n");
	printk("The gamepad driver is ready for use\n");
	return 0;
}


static void __exit gamepad_cleanup(void){
	printk("Disable interrupts\n");
	iowrite32(0x0000, GPIO_IEN);
	free_irq(GPIO_ODD_IRQ, &gamepad_cdev);
	free_irq(GPIO_EVEN_IRQ, &gamepad_cdev);

	printk("Destroy class\n");
	device_destroy(cl, dev_number);
	class_destroy(cl);
	cdev_del(&gamepad_cdev);
	unregister_chrdev_region(dev_number, DEV_COUNT);

	release_mem_region(GPIO_PC_BASE, 0x24);
	printk("Short life for a small module...\n");
}


static int gamepad_open(struct inode *inode, struct file* filp){
    printk(KERN_INFO "Gamepad opened\n");
    return 0;
}


static int gamepad_release(struct inode *inode, struct file* filp){
    printk(KERN_INFO "Gamepad closed\n");
    return 0;
}


//Read buttons, return to user
static ssize_t gamepad_read(struct file* filp, char* __user buffer,
        size_t count, loff_t *offset){
	printk(KERN_INFO "Gamepad read function called\n");
    uint8_t button_state = ioread8(GPIO_PC_DIN);		//only read the LSB
    copy_to_user(buffer, &button_state, count);
    return 0;
}
