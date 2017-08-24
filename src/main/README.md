# Logging

The program will log data to UART on the bare metal board, and stdout
on host and embedded linux system. To view the logging data over serial:


    sudo minicom --displayhex --wrap --device /dev/ttyACM1

Where /dev/ttyACM1 should be replaced with the tty that is actually
created when plugging in the device. If multiple devices are present
and it is not obvious which one to use, run `dmesg --follow` prior to
plugging in the device to see which tty is created.

The logging data is binary. As viewed from minicom, each pair of hex
digits is a byte. Translation to human readable form is handled
ese-process-logs.py. Note, this works on the ascii text version of the
log, i.e. output to stdout on host and embedded linux, copy and paste
of the minicom output for serial output. The spaces seperating bytes
must be removed from the serial output.

TODO:

  - add option to process-logs.py to process the raw binary instead of
    ascii so that it can open the tty device directly and display the
    human readable output.
