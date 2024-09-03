# Saw a lot of people using small python scripts to
# help automate the process of getting the uf2 on the rp2040

# This dosn't use scons or anything, its really just a bunch of shortcuts
# so you can quickly debug and fire up

import argparse
import subprocess
import os
import sys
from pathlib import Path


def build():
    """
    Function to handle building using CMake.
    """
    print("Building with CMake...")

    build_dir = Path("build")
    if not build_dir.exists():
        build_dir.mkdir()

    try:
        # Run CMake commands
        subprocess.run(["cmake", ".."], cwd=build_dir, check=True)
        subprocess.run(["make"], cwd=build_dir, check=True)
        print("Build completed successfully.")
    except subprocess.CalledProcessError as e:
        print(f"Build failed: {e}")
        sys.exit(1)


def find_and_mount_pico_storage():
    """
    Function to find and mount the storage media for the RP2040.
    Returns the path to the storage if found and mounted, otherwise returns None.
    """
    print("Looking for RP2040 storage and mounting it...")

    # Define the potential mount points and device paths for Linux and MacOS
    mount_points = [
        ("/dev/sdb1", "/run/media/joe/RPI-RP2/"),
        ("/dev/sdb1", "/tmp/PICO"),
        ("/dev/sdc1", "/tmp/PICO"),
    ]

    # Try mounting the device
    for device, mount_point in mount_points:
        try:
            if not Path(mount_point).exists():
                Path(mount_point).mkdir(parents=True, exist_ok=True)

            print(f"Attempting to mount {device} to {mount_point}...")
            subprocess.run(["sudo", "mount", device, mount_point], check=True)
            print(f"Mounted {device} at {mount_point}.")
            return mount_point
        except subprocess.CalledProcessError as e:
            print(f"Could not mount {device}: {e}")

    print(
        "Could not find or mount RP2040 storage. Ensure the device is in bootloader mode and try again."
    )
    return None


def flash():
    """
    Function to handle flashing the UF2 file onto the RP2040.
    """
    pico_storage = find_and_mount_pico_storage()
    if not pico_storage:
        print("Cannot flash: RP2040 storage not found or mounted.")
        return

    uf2_file = "build/src/kit-controller.uf2"

    try:
        print(f"Copying {uf2_file}")
        subprocess.run(["picotool", "load", uf2_file], check=True)

        print("Rebooting...")
        subprocess.run(["picotool", "reboot"], check=True)
    except Exception as e:
        print(f"Error flashing UF2: {e}")


def main():
    parser = argparse.ArgumentParser(description="RP2040 Development Tools")
    parser.add_argument(
        "option",
        choices=["build", "flash", "all"],
        help="Choose an operation: build, flash, or all",
    )
    args = parser.parse_args()

    if args.option == "build":
        build()
    elif args.option == "flash":
        flash()
    elif args.option == "all":
        build()
        flash()
    else:
        print(f"Unknown option: {args.option}")
        sys.exit(1)


if __name__ == "__main__":
    main()
