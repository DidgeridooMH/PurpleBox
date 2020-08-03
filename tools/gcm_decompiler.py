#!/usr/bin/python3

import sys


BOOT_BIN_SIZE = 0x440
BI2_BIN_SIZE = 0x2000


def main(filename):
    with open(filename, 'rb') as f:
        boot_bin = f.read(BOOT_BIN_SIZE)
        bi2_bin = f.read(BI2_BIN_SIZE)
        appldr_header = f.read(0x20)
        appldr_size = ((appldr_header[0x14] & 0xFF) << 24) \
            | ((appldr_header[0x15] & 0xFF) << 16) \
            | ((appldr_header[0x16] & 0xFF) << 8) \
            | (appldr_header[0x17] & 0xFF)
        print(f'App loader size found to be {appldr_size}')
        appldr_content = f.read(appldr_size - 0x20)
        fst_bin = f.read()

        print('Writing boot.bin')
        with open('boot.bin', 'wb') as boot:
            boot.write(boot_bin)
        
        print('Writing bi2.bin')
        with open('bi2.bin', 'wb') as bi2:
            bi2.write(bi2_bin)
        
        print('Writing appldr.bin')
        with open('appldr.bin', 'wb') as appldr:
            appldr.write(appldr_header)
            appldr.write(appldr_content)

        print('Writing fst.bin')
        with open('fst.bin', 'wb') as fst:
            fst.write(fst_bin)


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print('Usage: python gcm_decompiler [GCMFILE]')
        exit(1)
    main(sys.argv[1])
