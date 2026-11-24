"""Prepare a bounded H240 flash transaction and run it with OpenOCD."""

import argparse
import hashlib
import os
from pathlib import Path
import secrets
import struct
import subprocess
import tempfile
import zlib


HERE = Path(__file__).resolve().parent
FACTORY_SHA = "604638ea4dbdd9553c7803e87cf8ce7bdbf1ad0a3afc52028293f7fca92ed46f"
LOADER_SHA = "13857b233cf4718050c90bcba6142b6b48563d3e673628153e5db81be0312e53"
FLASH_BASE = 0x08000000
PAGE_SIZE = 4096
APP_SIZE = 10 * PAGE_SIZE


def checked_bytes(path, expected):
    data = path.read_bytes()
    if hashlib.sha256(data).hexdigest() != expected:
        raise ValueError(f"Unexpected SHA-256 for {path}")
    return data


def application_image(factory, image):
    if len(factory) != 128 * 1024 or not 196 <= len(image) <= APP_SIZE or len(image) % 4:
        raise ValueError("Unexpected factory or Rad Pro image size")
    stack, reset = struct.unpack_from("<II", image)
    checksum, = struct.unpack_from("<I", image, len(image) - 4)
    if stack != 0x20002000 or not reset & 1 or not 0x080000c1 <= reset < FLASH_BASE + len(image) - 4:
        raise ValueError("Unexpected Rad Pro boot vectors")
    if checksum != zlib.crc32(image[:-4]):
        raise ValueError("Rad Pro image CRC mismatch")
    return image.ljust(APP_SIZE, b"\xff") + factory[APP_SIZE:]


def recovery_image(factory, snapshot):
    if len(snapshot) != len(factory) or snapshot[APP_SIZE:] != factory[APP_SIZE:]:
        raise ValueError("Recovery preimage must be exactly 128 KiB with the original factory suffix")
    return snapshot


def words(data):
    return " ".join(f"0x{word:08x}" for (word,) in struct.iter_unpack("<I", data))


def make_plan(directory, loader, before, after, recovery=False):
    # The vector page is changed only after all other pages have passed readback.
    offsets = [*range(PAGE_SIZE, APP_SIZE, PAGE_SIZE), 0]
    while True:
        challenge = secrets.randbits(32)
        if challenge not in (0, 0xffffffff) and (challenge & ~15) not in (0xa4b86310, 0x731ef2c0):
            break
    plan = [
        f"set h240_page_recovery {int(recovery)}",
        f"set h240_page_challenge {challenge:#x}",
        f"set h240_page_loader {{{words(loader)}}}",
        "set h240_page_plan {",
    ]
    for offset in offsets:
        old = before[offset:offset + PAGE_SIZE]
        new = after[offset:offset + PAGE_SIZE]
        plan.append(f" {{{FLASH_BASE + offset:#x} {zlib.crc32(old):#x} {zlib.crc32(new):#x} {{{words(new)}}}}}")
    # OpenOCD prints the script result, so return an empty value after loading the plan.
    plan.extend(("}", "return"))
    (directory / "plan.tcl").write_text("\n".join(plan) + "\n", encoding="ascii")
    (directory / "expected-before.bin").write_bytes(before)
    (directory / "expected-after.bin").write_bytes(after)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("action", choices=("install", "restore"))
    parser.add_argument("factory", type=Path)
    parser.add_argument("image", type=Path)
    parser.add_argument("image_sha")
    parser.add_argument("--recovery-preimage", type=Path, help="Fresh 128 KiB capture of an interrupted installation, for restore only")
    args = parser.parse_args()
    try:
        loader = checked_bytes(HERE / "flash-page.bin", LOADER_SHA)
        factory = checked_bytes(args.factory, FACTORY_SHA)
        application = application_image(factory, checked_bytes(args.image, args.image_sha))
        before, after = (factory, application) if args.action == "install" else (application, factory)
        recovery = args.recovery_preimage is not None
        if recovery:
            if args.action != "restore":
                raise ValueError("--recovery-preimage is only valid with restore")
            before = recovery_image(factory, args.recovery_preimage.read_bytes())
        build = HERE / "build"
        build.mkdir(exist_ok=True)
        directory = Path(tempfile.mkdtemp(prefix=f"h240-{args.action}.", dir=build))
        make_plan(directory, loader, before, after, recovery)
    except (OSError, ValueError) as error:
        parser.error(str(error))

    print(f"{args.action}: transaction files in {directory}", flush=True)
    command = [
        "openocd", "-f", str(HERE / "openocd-sram.cfg"),
        "-c", "h240.cpu configure -defer-examine",
        "-c", "target create h240.mem mem_ap -dap h240.dap -ap-num 0",
        "-c", "gdb_port disabled", "-c", "init", "-c", "poll off",
        "-f", str(HERE / "openocd-support.tcl"), "-f", str(directory / "plan.tcl"),
        "-f", str(HERE / "run-flash-page.tcl"), "-c", "h240_page_main",
    ]
    with (directory / "openocd.log").open("w", encoding="utf-8") as log:
        with subprocess.Popen(command, env={**os.environ, "H240_FLASH_TRIAL_DIR": str(directory)},
                              stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True,
                              errors="replace") as process:
            for line in process.stdout:
                print(line, end="", flush=True)
                log.write(line)
                log.flush()
        if process.returncode:
            return process.returncode
    if (directory / "before.bin").read_bytes() != before or (directory / "after.bin").read_bytes() != after:
        raise SystemExit("OpenOCD captures do not match the expected preimage and final image")
    for filename in ("plan.tcl", "expected-before.bin", "expected-after.bin"):
        (directory / filename).unlink()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
