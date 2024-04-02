import logging
import subprocess

from datetime import date


today = date.today()

try:
    revision = (
        subprocess.check_output(
            [
                "git",
                "describe",
                "--abbrev=8",
                "--dirty",
                "--always",
                "--tags",
            ]
        )
        .strip()
        .decode("utf-8")
    )
except Exception as e:
    logging.warning(f"Error fetching revision! {e}")
    revision = "UnknownRevision"

try:
    host = (
        subprocess.check_output(
            [
                "hostname",
            ]
        )
        .strip()
        .decode("utf-8")
    )
except Exception as e:
    logging.warning(f"Error fetching build host! {e}")
    revision = "UnknownHost"

try:
    username = (
        subprocess.check_output(
            [
                "id",
                "-u",
                "-n",
            ]
        )
        .strip()
        .decode("utf-8")
    )
except Exception as e:
    logging.warning(f"Error fetching build user! {e}")
    username = "UnknownUser"

# Cleanup CI
if username == "root":
    username = "github"
    host = "github"

print(f"-DREVISION='\"{revision}\"'")
print(f"-DHOST='\"{host}\"'")
print(f"-DUSER='\"{username}\"'")


# These come from the platform defs
print(f"-D PICO_STDIO_USB")  # Both
print(f"-D PICO_STDIO_UART")  # Baremetal
print(f"-D USE_FREERTOS")  # Both
print(f"-D USE_VFS")
print(f"-D USE_LFS")  # Both
print(
    f"-D FATFS_SPI_BRG=100000"
)  # Compilation fix, SPI frqeuency for FAT SD card (goes unused here)
