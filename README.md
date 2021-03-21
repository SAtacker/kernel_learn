# Tasks

* Currently it consists of code for learning ldd and further custom kernels too.

- [Apertus GSoC Task](https://lab.apertus.org/T884)

```
Task 1:
Create a simple character device driver which generates a checksum of the data sent to the kernel.
Provide the checksum via a procfs entry as well as an IOCTL.

Task 2:
Create an SPI bus driver which uses bit banging on an FTDI (or similar) USB interface.
Make sure that the driver can be used with spi-tools.
```

### Getting Started

* Cloning and building the module

```sh
git clone https://github.com/SAtacker/kernel_learn.git
cd Kernel_Challenge/
make
```

* Loading the kernel module

```sh
sudo insmod char_ddriver.ko
```

### Methodology and Testing

The userspace program initially opens the device `/dev/char_ddriver` and sends the user non-formatted input to the device. The LKM echoes back the received buffer. After that user space program uses `ioctl` for i/o with kernel space and echoes back the user input.

```sh
./test
```

### Output

- Logs

```sh
tail -20f /var/log/kern.log
```

### Removal of module

```
sudo rmmod char_ddriver
```

### Further additions

- [x] IOCTL calls 
- [ ] Checksum Function
- [ ] Procfs entry
- [ ] Task 2