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

```
Mar 21 11:30:33 ubuntu kernel: [35966.145900] char_ddriver : Initializing 
Mar 21 11:30:33 ubuntu kernel: [35966.145908] char_ddriver : Registered with major number 507
Mar 21 11:30:33 ubuntu kernel: [35966.146025] char_ddriver : Device created 
Mar 21 11:30:54 ubuntu kernel: [35987.476205] char_ddriver : Opened
Mar 21 11:30:57 ubuntu kernel: [35990.650704] char_ddriver : previous data is 
Mar 21 11:30:57 ubuntu kernel: [35990.650713] char_ddriver : Data received : hello , Data Size: 5
Mar 21 11:30:58 ubuntu kernel: [35991.425957] char_ddriver : Data inside data buffer : hello , Data Size: 5
Mar 21 11:30:58 ubuntu kernel: [35991.425964] char_ddriver : Sent 5 chars to user
Mar 21 11:31:02 ubuntu kernel: [35995.585459] char_ddriver : previous data is 
Mar 21 11:31:02 ubuntu kernel: [35995.585467] char_ddriver : Data received : hi , Data Size: 2
Mar 21 11:31:02 ubuntu kernel: [35995.585475] char_ddriver : Data inside data buffer : hi , Data Size: 2
Mar 21 11:31:02 ubuntu kernel: [35995.585656] char_ddriver : Device released
Mar 21 11:31:29 ubuntu kernel: [36022.637021] char_ddriver : module cleanup
```

- GIF

<html>
    <p align="center">
        <a href="#">
            <img src="./assets/render1616306510649.gif">
        </a>
    </p>
</html>

### Removal of module

```
sudo rmmod char_ddriver
```

### Further additions

- [x] IOCTL calls 
- [x] Simplest Checksum Function
- [ ] Procfs entry
- [ ] Task 2