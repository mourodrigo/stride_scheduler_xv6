cd stride_scheduler_xv6
make clean
make
qemu-system-i386 -serial mon:stdio -hdb fs.img xv6.img -smp 1 -m 512
