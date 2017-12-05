#include <linux/module.h>
#include <linux/init.h>


#include <asm/arch-at91rm9200/gpio.h>

MODULE_AUTHOR("optionESE");
MODULE_DESCRIPTION("Projet");
MODULE_LICENSE("none");



static ssize_t spi_write(struct file *file, char *buf, size_t count, loff_t *ppos)
{
  return 0;
}

static ssize_t spi_read(struct file *file, const char *buf, size_t count, loff_t *ppos)
{
  return 0;
}

static ssize_t spi_open(struct inode *inode, struct file *file)
{
  return 0;
}

static ssize_t spi_close(struct inode *inode, struct file *file)
{
  return 0;
}

static struct file_operations fops =
{
  read : spi_read,
  write : spi_write,
  open : spi_open,
  release : spi_close
};

static int __init module_spi_init(void)
{
  return 0;

} 

static void __exit module_spi_cleanup(void)
{

}

