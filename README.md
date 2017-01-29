# TinyPicoBoard

This is a tiny implementation of PicoBoard that communicates with Scratch on Raspberry Pi via the serial port on GPIO (/dev/ttyAMA0). 
[PIC12F1822](http://www.digikey.com/product-detail/en/microchip-technology/PIC12F1822-I-P/PIC12F1822-I-P-ND/2258573 "PIC12F1822")
and
[PIC16F1823](http://www.digikey.com/product-detail/en/microchip-technology/PIC16F1823-I-P/PIC16F1823-I-P-ND/2258579 "PIC16F1823")
are supported.


## Demonstration

[A demonstration of TinyPicoBoard on YouTube](https://www.youtube.com/watch?v=zczEgQNWSyQ "A demonstration of TinyPicoBoard on YouTube")



## How to use

### 1. Set up of Raspberry Pi

Scratch on NOOBS 1.9.3 and 2.0.0 (Raspbian 2016-09-23) has a bug and it cannot detect /dev/ttyAMA0. Please use NOOBS 2.1.0 (Raspbian 2016-11-25) or later.

In order to disable the serial console, please remove "console=serial0,115200" from /boot/cmdline.txt. The result would be as follows.

```
dwc_otg.lpm_enable=0 console=tty1 root=/dev/mmcblk0p7 rootfstype=ext4 elevator=deadline fsck.repair=yes rootwait
```

Then please execute the following command.

```
$ sudo systemctl disable serial-getty@ttyAMA0.service
```

Finally, if you are using Raspberry Pi 3, please disable the on-board bluetooth device to avoid an interference of the serial device by adding the following line in /boot/config.txt.

```
dtoverlay=pi3-disable-bt
```

Then please reboot Raspberry Pi.


### 2. Installation of pickle

To write a program to PIC micro-controller, I use a software [pickle](https://wiki.kewl.org/dokuwiki/projects:pickle "pickle") on Raspberry Pi.

Please download pickle-4.0d.tar.gz, and please execute the following commands.

```
$ tar zxf pickle-4.0d.tar.gz
$ cd pickle
$ make
$ sudo make install
```

Then please create /home/pi/.pickle/config with the following lines. This configuration is for Raspberry Pi 2 and 3. If you are using Raspberry Pi Model B+, the first line should be "DEVICE=RPI".

```
DEVICE=RPI2
SLEEP=1
BITRULES=0x1000
BUSY=0
VPP=4
PGM=65535
PGC=17
PGD=27
MCP=0x20
FWSLEEP=30
DEBUG=1
```


### 3. Set up of PIC

Using the installed pickle, we write a program on the PIC micro-controller.

For PIC12F1822, please prepare a circuit shown in [this figure](https://2.bp.blogspot.com/-bS5xsVonnr0/WAnljXMbn3I/AAAAAAAAE38/ncv8KZARBJwUEn8usSqQ8ekira3MThHAwCLcB/s1600/PicProgrammingNMOS_12f1822-e.png "Circuit for PIC12F1822").


[2N7000](http://www.digikey.com/product-detail/en/fairchild-semiconductor/2N7000/2N7000FS-ND/244278 "2N7000")
is a N-ch MOSFET. This is used for high-voltage (9V) programming of PIC as a level converter.

Then please execute the following commands.

```
$ p14 program 12f1822.hex
```

For PIC16F1823, please prepare a circuit shown in [this figure](https://2.bp.blogspot.com/-5P1uI1Bv-gs/WAnlkuIkQFI/AAAAAAAAE4A/oqi7fX-U4do1yTnh_cVAEtsVu3eFk8HRACLcB/s1600/PicProgrammingNMOS_16f1823-e.png "Circuit for PIC16F1823"). 

Then please execute the following commands.

```
$ p14 program 16f1823.hex
```


### 4. How to use TinyPicoBoard

For PIC12F1822, please prepare a circuit shown in [this figure](https://3.bp.blogspot.com/-6vmyDYdPa4M/WAnpobq7nYI/AAAAAAAAE4Q/XTh45Nan7xg9fjyDROl3NypAq9jMz5l1QCLcB/s1600/HowToUse12f1822.png "How to use PIC12F1822"). Only "A", "B", "C" can be used as analog inputs.

For PIC16F1823, please prepare a circuit shown in [this figure](https://4.bp.blogspot.com/-8ebGp4wgwns/WAnpoR7wsdI/AAAAAAAAE4M/yF0oLhvu7CEfbZ2BaFJs0520sYSFzRXHgCLcB/s1600/HowToUse16f1823.png "How to use PIC16F1823"). All the inputs can be used.

Please right click the "slider sensor value" block, and show ScratchBoard watcher. Then please connect to /dev/ttyAMA0 as shown in [this figure](https://2.bp.blogspot.com/-wbSVyw2vh7g/WAnrb3HAIwI/AAAAAAAAE4U/9BRvERE-lxQrwVTfKuYsKLba149MpqZLACLcB/s1600/HowToConnect.png "How to connect").
