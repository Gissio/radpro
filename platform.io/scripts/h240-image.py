# Build the checksummed H240 image alongside PlatformIO's raw binary.
# License: MIT

import subprocess

from SCons.Subst import Literal

Import("env")


def prepare_image(target, source, env):
    subprocess.run(
        [env.subst("$PYTHONEXE"), source[1].get_abspath(), source[0].get_abspath(), target[0].get_abspath()],
        check=True,
    )


map_path = env.File("$BUILD_DIR/firmware.map").get_abspath()
env.Append(LINKFLAGS=[Literal(f"-Wl,-Map,{map_path}")])

image = env.Command(
    "$BUILD_DIR/radpro-h240.bin",
    ["$BUILD_DIR/${PROGNAME}.bin", "$PROJECT_DIR/../tools/h240/prepare-image.py"],
    env.VerboseAction(prepare_image, "Preparing $TARGET"),
)
env.Depends("buildprog", image)
