#!/usr/bin/env python3

#
# Copyright 2017 Benjamin J. Andre.
# All Rights Reserved.
# 
# This Source Code Form is subject to the terms of the Mozilla
# Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

"""Process the the binary log output from the Embedded Systems
Engineering project and prettyy print it.

Copyright (c) 2017 Benjamin J. Andre

This Source Code Form is subject to the terms of the Mozilla Public
License, v.  2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.

"""

#
# built-in modules
#
import argparse
import os
import struct
import sys
import time
import traceback

from configparser import ConfigParser

#
# installed dependencies
#

#
# other modules in this package
#

if sys.hexversion < 0x03050000:
    print(70 * "*")
    print("ERROR: {0} requires python >= 3.5.x. ".format(sys.argv[0]))
    print("It appears that you are running python {0}".format(
        ".".join(str(x) for x in sys.version_info[0:3])))
    print(70 * "*")
    sys.exit(1)


# -------------------------------------------------------------------------------
#
# User input
#
# -------------------------------------------------------------------------------
def commandline_options():
    """Process the command line arguments.

    """
    parser = argparse.ArgumentParser(
        description='''
Process the binary log output from the Embedded Systems Engineering project
and prettyy print it.

Use: ./ese-project.out < console-input.txt | ./log-analyzer.py
''')

    parser.add_argument('--backtrace', action='store_true',
                        help='show exception backtraces as extra debugging '
                        'output')

    parser.add_argument('--debug', action='store_true',
                        help='extra debugging output')

    parser.add_argument('--config', nargs=1, required=False,
                        help='path to config file')

    options = parser.parse_args()
    return options


def read_config_file(filename):
    """Read the configuration file and process

    """
    print("Reading configuration file : {0}".format(filename))

    cfg_file = os.path.abspath(filename)
    if not os.path.isfile(cfg_file):
        raise RuntimeError("Could not find config file: {0}".format(cfg_file))

    config = ConfigParser()
    config.read(cfg_file)

    return config


# -------------------------------------------------------------------------------
#
# work functions
#
# -------------------------------------------------------------------------------
def translate_id_to_string(item_id):
    """Look up the name of the ID
    """
    id_names = ["LOGGER_INITIALIZED",
                "RTC_INITIALIZED",
                "GPIO_INITIALIZED",
                "DMA_INITIALIZED",
                "SPI_INITIALIZED",
                "SYSTEM_INITIALIZED",
                "SYSTEM_HALTED",
                "HEARTBEAT",
                "INFO",
                "WARNING",
                "ERROR",
                "POST_START",
                "POST_ERROR",
                "POST_STATUS",
                "POST_NUM_TESTS_RUN",
                "POST_NUM_TESTS_SKIPPED",
                "POST_NUM_TESTS_PASSED",
                "POST_COMPLETE",
                "DATA_RECEIVED",
                "DATA_ANALYSIS_STARTED",
                "DATA_ALPHA_COUNT",
                "DATA_NUMERIC_COUNT",
                "DATA_PUNCTUATION_COUNT",
                "DATA_MISC_COUNT",
                "DATA_ANALYSIS_COMPLETED",
                "PROFILING_CLOCKS_PER_SEC",
                "PROFILING_START",
                "PROFILING_VALUE",
                "PROFILING_END",
                ]
    try:
        name = id_names[item_id]
    except IndexError as e:
        msg = "Invalid logger ID: {0}".format(item_id)
        print(msg)
        raise e
    return name


def format_string_from_num_bytes(num_bytes, name):
    """Determine the stuct pack format string for a give number of bytes.

    """
    if num_bytes == 1:
        size_fmt = ">B"
    elif num_bytes == 2:
        size_fmt = "<H"
    elif num_bytes == 4:
        size_fmt = "<I"
    elif num_bytes == 8:
        size_fmt = "<Q"
    else:
        msg = ("Error: incorrect number of bytes for {0} size. "
               "Received {1}. Valid values [1, 2, 4, 8].".format(
                   name, num_bytes))
        raise RuntimeError(msg)
    return size_fmt


def convert_logger_bytes_to_string():
    """
    """
    # read the header
    byte_stream = bytes.fromhex(sys.stdin.read(2))
    print("Byte 1: {0}".format(byte_stream.hex()))
    byte_stream = bytes.fromhex(sys.stdin.read(2))
    print("Byte 2: {0}".format(byte_stream.hex()))

    byte_stream = bytes.fromhex(sys.stdin.read(2))
    sizeof_id = struct.unpack(">B", byte_stream)[0]
    print("sizeof(ID): {0}".format(sizeof_id))
    id_fmt = format_string_from_num_bytes(sizeof_id, "ID")

    byte_stream = bytes.fromhex(sys.stdin.read(2))
    sizeof_time = struct.unpack(">B", byte_stream)[0]
    print("sizeof(timestamp): {0}".format(sizeof_time))

    byte_stream = bytes.fromhex(sys.stdin.read(2))
    sizeof_size = struct.unpack(">B", byte_stream)[0]
    print("sizeof(size): {0}".format(sizeof_size))
    size_fmt = format_string_from_num_bytes(sizeof_size, "data")

    num_id_chars = sizeof_id * 2
    num_time_chars = sizeof_time * 2
    num_size_chars = sizeof_size * 2
    # print("num chars per id = {0}".format(num_id_chars))
    # print("num chars per size = {0}".format(num_size_chars))

    # read the data stream
    while byte_stream is not None:
        char_data = sys.stdin.read(num_id_chars)
        byte_stream = bytes.fromhex(char_data)
        item_id = struct.unpack(id_fmt, byte_stream)[0]
        item_name = translate_id_to_string(item_id)

        byte_stream = bytes.fromhex(sys.stdin.read(num_time_chars))
        timestamp = int.from_bytes(byte_stream, byteorder='little')

        byte_stream = bytes.fromhex(sys.stdin.read(num_size_chars))
        payload_size = struct.unpack(size_fmt, byte_stream)[0]

        byte_stream = bytes.fromhex(sys.stdin.read(payload_size*2))
        payload = byte_stream.hex()
        translated_payload = ''
        if (item_name == "POST_STATUS" or item_name == "POST_ERROR" or
            item_name == "PROFILING_START" or item_name == "PROFILING_END"):
            fmt = ">{0}s".format(payload_size)
            msg = struct.unpack(fmt, byte_stream)[0]
            translated_payload = "{0}".format(msg.decode('utf-8'))

        elif item_name == "HEARTBEAT":
            epoch_time = time.strftime("%Y-%m-%d %H:%M:%S",
                                       time.gmtime(timestamp))
            payload = "{0}".format(epoch_time)

        elif (item_name == "PROFILING_VALUE" or
              item_name == "PROFILING_CLOCKS_PER_SEC"):
            fmt = format_string_from_num_bytes(payload_size, 'data')
            profile_time = struct.unpack(fmt, byte_stream)[0]
            translated_payload = "{0}".format(profile_time)

        print("{0}({1}) : {2} : size = {3} : {4} : {5}".format(
            item_name, item_id, timestamp, payload_size, payload, translated_payload))


# -------------------------------------------------------------------------------
#
# main
#
# -------------------------------------------------------------------------------
def main(options):
    # config = read_config_file(options.config[0])
    convert_logger_bytes_to_string()
    return 0


if __name__ == "__main__":
    options = commandline_options()
    try:
        status = main(options)
        sys.exit(status)
    except Exception as error:
        print(str(error))
        if options.backtrace:
            traceback.print_exc()
        sys.exit(1)
