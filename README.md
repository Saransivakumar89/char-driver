# char-driver
# Pseudo Character Device Driver

## Description
A simple Linux kernel module that implements a pseudo character device driver.  
It supports basic file operations like **open, read, write, lseek, and release**  
using a **1024-byte buffer** for simulated storage.

## Features
- Implements character device operations (`open`, `read`, `write`, `lseek`, `release`).
- Uses a **1024-byte buffer** as device storage.
- Creates `/dev/pseudo` dynamically.
- Allocates major/minor numbers using `alloc_chrdev_region()`.
- Provides a user-space interface via `copy_to_user()` and `copy_from_user()`.

## Build & Run
```bash
make
sudo insmod pseudo.ko
dmesg | grep "Device number"
sudo mknod /dev/pseudo c <major> 0
echo "Hello" > /dev/pseudo
cat /dev/pseudo
sudo rmmod pseudo

Author
Saran Sivakumar
