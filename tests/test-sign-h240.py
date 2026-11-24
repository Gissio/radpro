"""Exercise the H240 release package without PlatformIO or hardware."""

from hashlib import sha256
from importlib.util import module_from_spec, spec_from_file_location
from pathlib import Path
from runpy import run_path
from shutil import copyfile
from struct import pack
from tempfile import TemporaryDirectory
import unittest
from unittest.mock import patch


ROOT = Path(__file__).resolve().parent.parent
SIGN_SPEC = spec_from_file_location("sign", ROOT / "tools/sign.py")
SIGN = module_from_spec(SIGN_SPEC)
SIGN_SPEC.loader.exec_module(SIGN)
PREPARE = run_path(str(ROOT / "tools/h240/prepare-image.py"))["prepare_image"]
IMAGE_NAME = "radpro-h240-ca32f0156l2-fr-9.8.7-swd.bin"
LOADER = b"Reviewed loader fixture"


class H240PackageTest(unittest.TestCase):
    def setUp(self):
        temp = TemporaryDirectory(prefix="radpro-sign-h240-")
        self.addCleanup(temp.cleanup)
        self.project = Path(temp.name)
        self.output = self.project / "release"
        self.build = self.project / "platform.io/.pio/build/h240-ca32f0156l2-fr"
        self.tools = self.project / "tools/h240"
        self.package = self.output / "h240-ca32f0156l2"
        self.tools.mkdir(parents=True)
        copyfile(ROOT / "tools/h240/prepare-image.py", self.tools / "prepare-image.py")
        self.installer = {
            "run-flash-page.py": f"LOADER_SHA = '{sha256(LOADER).hexdigest()}'\n".encode("ascii"),
            "run-flash-page.tcl": b"# Installer fixture\n",
            "openocd-sram.cfg": b"# Configuration fixture\n",
            "openocd-support.tcl": b"# Helper fixture\n",
            "flash-page.bin": LOADER,
            "flash-install.md": b"# Installation fixture\n",
        }
        for name, data in self.installer.items():
            (self.tools / name).write_bytes(data)
        (self.project / "LICENSE").write_bytes(b"License fixture\n")
        header = self.project / "platform.io/src/system/system.h"
        header.parent.mkdir(parents=True)
        header.write_text('#define FIRMWARE_VERSION "9.8.7"\n', encoding="ascii")
        project_path = patch.object(SIGN, "get_radpro_path", return_value=str(self.project) + "/")
        project_path.start()
        self.addCleanup(project_path.stop)

    def build_h240(self):
        self.build.mkdir(parents=True)
        raw = pack("<II", 0x20002000, 0x080000c1).ljust(200, b"\xff")
        (self.build / "firmware.bin").write_bytes(raw)
        image = PREPARE(raw)
        (self.build / "radpro-h240.bin").write_bytes(image)
        return image

    def test_absent_target_leaves_other_artifacts(self):
        legacy = self.output / "fs2011-stm32f051c8/firmware/radpro-legacy.bin"
        legacy.parent.mkdir(parents=True)
        legacy.write_bytes(b"Existing release artifact")
        SIGN.package_h240(self.output)
        self.assertFalse(self.package.exists())
        self.assertEqual(legacy.read_bytes(), b"Existing release artifact")

    def test_preserves_image_and_copies_only_installer_files(self):
        image = self.build_h240()
        SIGN.package_h240(self.output)
        firmware = self.package / "firmware"
        self.assertEqual((firmware / IMAGE_NAME).read_bytes(), image)
        self.assertEqual((firmware / "SHA256SUMS").read_text(),
                         f"{sha256(image).hexdigest()}  {IMAGE_NAME}\n")
        self.assertEqual({file.name: file.read_bytes() for file in (self.package / "swd").iterdir()},
                         self.installer)
        self.assertEqual((self.package / "LICENSE").read_bytes(), b"License fixture\n")

    def test_existing_package_cannot_include_factory_captures(self):
        self.build_h240()
        backup = self.package / "h240-factory.example" / "factory-1.bin"
        backup.parent.mkdir(parents=True)
        backup.write_bytes(b"Factory backup")
        with self.assertRaises(FileExistsError):
            SIGN.package_h240(self.output)
        self.assertEqual(backup.read_bytes(), b"Factory backup")
        self.assertFalse((self.package / "firmware").exists())

    def test_incomplete_build_fails(self):
        self.build_h240()
        (self.build / "radpro-h240.bin").unlink()
        with self.assertRaises(FileNotFoundError):
            SIGN.package_h240(self.output)
        self.assertFalse(self.package.exists())

    def test_stale_or_invalid_packed_image_fails(self):
        image = self.build_h240()
        raw = bytearray((self.build / "firmware.bin").read_bytes())
        raw[20] ^= 1
        corrupted = image[:-1] + bytes([image[-1] ^ 1])
        for invalid in (PREPARE(raw), corrupted):
            with self.subTest(image=invalid):
                (self.build / "radpro-h240.bin").write_bytes(invalid)
                with self.assertRaisesRegex(ValueError, "does not match the checksummed"):
                    SIGN.package_h240(self.output)
                self.assertFalse(self.package.exists())

    def test_incomplete_installer_fails(self):
        self.build_h240()
        (self.tools / "openocd-support.tcl").unlink()
        with self.assertRaises(FileNotFoundError):
            SIGN.package_h240(self.output)
        self.assertFalse(self.package.exists())

    def test_unreviewed_loader_fails(self):
        self.build_h240()
        (self.tools / "flash-page.bin").write_bytes(b"Different loader")
        with self.assertRaisesRegex(ValueError, "does not match the reviewed"):
            SIGN.package_h240(self.output)
        self.assertFalse(self.package.exists())


if __name__ == "__main__":
    unittest.main()
