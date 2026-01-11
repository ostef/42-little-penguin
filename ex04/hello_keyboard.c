#include <linux/module.h>
#include <linux/printk.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/hid.h>

static int keyboard_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
        printk(KERN_INFO "Keyboard connected\n");
        return 0;
}

static void keyboard_disconnect(struct usb_interface *interface)
{
        printk(KERN_INFO "Keyboard disconnected\n");
}

static const struct usb_device_id driver_id_table[] = {
        { USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT, USB_INTERFACE_PROTOCOL_KEYBOARD) },
        {},
};

MODULE_DEVICE_TABLE(usb, driver_id_table);

static struct usb_driver helloworld_driver = {
        .name = "Hello world USB keyboard driver",
        .probe = keyboard_probe,
        .disconnect = keyboard_disconnect,
        .id_table = driver_id_table,
};

static int __init helloworld_init(void)
{
        printk(KERN_INFO "Hello World!\n");
        return usb_register(&helloworld_driver);
}

static void __exit helloworld_exit(void)
{
        printk(KERN_INFO "Cleaning up module.\n");
        usb_deregister(&helloworld_driver);
}

module_init(helloworld_init);
module_exit(helloworld_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stefan Oumansour");
MODULE_DESCRIPTION("Simple hello world kernel module, with keyboard plugging detection");
