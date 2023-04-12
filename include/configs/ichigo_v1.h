#ifndef __CONFIG_ICHIGO_V1_H
#define __CONFIG_ICHIGO_V1_H

/* Cache options */
#ifndef CONFIG_SYS_L2CACHE_OFF
# define CFG_SYS_PL310_BASE 0xf8f02000
#endif

#define ZYNQ_SCUTIMER_BASEADDR 0xF8F00600
#define CFG_SYS_TIMERBASE ZYNQ_SCUTIMER_BASEADDR
#define CFG_SYS_TIMER_COUNTER (CFG_SYS_TIMERBASE + 0x4)

/* Serial drivers */
/* The following table includes the supported baudrates */
#define CFG_SYS_BAUDRATE_TABLE  \
    {300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400}

/* Miscellaneous configurable options */

#define CFG_SYS_INIT_RAM_ADDR 0xFFFF0000
#define CFG_SYS_INIT_RAM_SIZE 0x2000

#ifdef CONFIG_SPL_BUILD
#define BOOTENV
#else
#define BOOTENV_COMMON\
    "kernel_addr_r=0x2000000\0"\
    "devtree_addr_r=0x3000000\0"\
    "kernel_img=uImage\0"\
    "device_tree=system.dtb\0"\
    "load_dev=mmc 0:1\0"\
    "boot_dev=mmcblk0p2\0"\
    "boot_os="\
        "setenv bootargs "\
            "console=ttyPS0,115200n8 "\
            "root=/dev/${boot_dev} "\
            "rootwait earlyprintk;"\
        "load ${load_dev} ${kernel_addr_r} ${kernel_img}; "\
        "load ${load_dev} ${devtree_addr_r} ${device_tree}; "\
        "bootm ${kernel_addr_r} - ${devtree_addr_r}\0"

#define BOOTENV_QSPI \
    "dfu_alt_info="\
        "spl raw 0x0 0x40000;"\
        "environment-1 raw 0x40000 0x20000;"\
        "environment-2 raw 0x60000 0x20000;"\
        "bootscript raw 0x80000 0x40000;"\
        "spare raw 0xC0000 0x40000;"\
        "u-boot raw 0x100000 0x100000;"\
        "bitstream raw 0x200000 0xE00000\0"\
    "dfu_sf=dfu 0 sf 0:0:50000000:0\0"

#define BOOTENV \
    BOOTENV_COMMON\
    BOOTENV_QSPI

#define BOOTCMD_COMMON\
    "disable_stdout="\
        "stdout_=${stdout}; "\
        "setenv stdout nulldev;\0"\
    "restore_stdout="\
        "setenv stdout ${stdout_};\0"

#define BOOTCMD \
    BOOTCMD_COMMON\
    "distro_bootcmd="\
        "run disable_stdout; "\
        "if gpio input 0; then "\
            "dfu_led_pin=7; "\
            "gpio set ${dfu_led_pin}; "\
            "run restore_stdout; "\
            "echo Entering DFU SF mode ...; "\
            "run dfu_sf; "\
            "run disable_stdout; "\
            "gpio clear ${dfu_led_pin}; "\
            "run restore_stdout; "\
        "else "\
            "run restore_stdout; "\
            "echo Booting OS ...;"\
            "run boot_os; "\
        "fi;\0"

/* Default environment */
#ifndef CFG_EXTRA_ENV_SETTINGS
#define CFG_EXTRA_ENV_SETTINGS \
    BOOTENV\
    BOOTCMD
#endif

#endif /*CONFIG_SPL_BUILD*/

/* qspi mode is working fine */
#ifdef CONFIG_ZYNQ_QSPI
#define CFG_SYS_SPI_ARGS_OFFS	0x200000
#define CFG_SYS_SPI_ARGS_SIZE	0x80000
#define CFG_SYS_SPI_KERNEL_OFFS	(CFG_SYS_SPI_ARGS_OFFS + \
        CFG_SYS_SPI_ARGS_SIZE)
#endif

#endif /*__CONFIG_ICHIGO_V1_H*/
