"""Fill the H240 application's linker-reserved CRC word."""

import argparse
from pathlib import Path
import struct
import zlib


APPLICATION_SIZE = 40 * 1024


def prepare_image(image):
    if len(image) < 196 or len(image) > APPLICATION_SIZE or len(image) % 4:
        raise ValueError("Unexpected H240 image size")

    stack, reset = struct.unpack_from("<II", image)
    valid_reset = 0x080000C1 <= reset < 0x08000000 + len(image) - 4 and reset & 1
    if stack != 0x20002000 or not valid_reset or image[-4:] != b"\xff" * 4:
        raise ValueError("Expected an H240 build with an empty CRC word")

    payload = image[:-4]
    return payload + struct.pack("<I", zlib.crc32(payload))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()
    try:
        image = prepare_image(args.input.read_bytes())
        with args.output.open("xb") as output:
            output.write(image)
    except (ValueError, OSError) as error:
        parser.error(str(error))


if __name__ == "__main__":
    main()
