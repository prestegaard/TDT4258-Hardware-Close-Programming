/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include "efm32gg.h"

#define BASE_MINOR 0
#define DEV_COUNT 1
#define DRIVER_NAME "gamepad"
#define GPIO_EVEN_IRQ 17
#define GPIO_ODD_IRQ 16

static dev_t dev_number;
struct cdev *gamepad_cdev;

static struct file_operations fops = {
  .owner   = THIS_MODULE,
  .open    = gamepad_open,
  .release = gamepad_release,
  .read    = gamepad_read,
  .fasync  = gamepad_fasync,  // asyncronous notification
};
/*
 * template_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */

static int __init gamepad_init(void)
{
	if (alloc_chdev_region(&dev_number, BASE_MINOR, DEV_COUNT, DRIVER_NAME) < 0){
		printk(KERN_ALERT "feilmelding alloc_chdev_region\n");
		return -1;
	}

	//request_mem_region(start, length of region, name)
	//requesting control of entire PC register
	if (request_mem_region(GPIO_PC_BASE, 0x24, DRIVER_NAME) == NULL){
		printk(KERN_ALERT "GPIO_PC_BASE request denied\n");
		return -1;
	}

	//init cdev
	printk(KERN_INFO "Initializing cdev.\n");
	gamepad_cdev = cdev_alloc();
	gamepad_cdev.owner = THIS_MODULE;
	gamepad_cdev.ops = &gamepad_fops;
	cdev_add(&gamepad_cdev, dev_number, DEV_COUNT);

	//DRIVER = File
	printk("Creating class");
	driver_class = class_create(THIS_MODULE, DRIVER_NAME);
	//device_create(class, parent, devt, fmt)
	device_create(driver_class, NULL, dev_number, NULL, DRIVER_NAME);

	//Setting up GPIO
	iowrite32(0x33333333, GPIO_PC_MODEL); //setting pin 0-7 as input
	iowrite32(0xff, GPIO_PC_DOUT); //enable pullup for inputs

	//setup interrupts
	//request_irq(irq, handler, irqflags, devname, dev_id)
	printk("setting up interrupts")
	request_irq(GPIO_EVEN_IRQ, (irq_handler_t)gpio_interrupt_handler, 0, DRIVER_NAME, &gamepad_cdev);
	request_irq(GPIO_ODD_IRQ, (irq_handler_t)gpio_interrupt_handler, 0, DRIVER_NAME, &gamepad_cdev);

	iowrite32(0x22222222, GPIO_EXTISELL);
	iowrite32(0xff, GPIO_EXTIRISE);
	iowrite32(0xff, GPIO_EXTIFALL);
	iowrite32(0x00ff, GPIO_IEN);
	iowrite32(0xff, GPIO_IFC);


	printk("Hello World, here is your module speaking\n");
	return 0;
}

/*
 * template_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */

static void __exit gamepad_cleanup(void)
{
	printk("Disable interrupts");
	iowrite32(0x0000, GPIO_IEN);
	free_irq(GPIO_ODD_IRQ, &gamepad_cdev);
	free_irq(GPIO_EVEN_IRQ, &gamepad_cdev);

	printk("destroy class");
	device_destroy(driver_class, dev_num);
	class_destroy(driver_class);
	cdev_del(gamepad_cdev);
	unregister_chrdev_region(dev_number, DEV_COUNT);

	release_mem_region(GPIO_PC_BASE, 0x24);
	printk("Short life for a small module...\n");
}

module_init(gamepad_init);
module_exit(gamepad_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");
