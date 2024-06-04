# Saw a lot of people using small python scripts to
# help automate the process of getting the uf2 on the rp2040

# This dosn't use scons or anything, its really just a bunch of shortcuts
# so you can quickly debug and fire up

import argparse
import subprocess

parser = argparse.ArgumentParser()

parser.add_argument("option", help="one of build, flash or all")

parser.parse_args()

if __name__ == "__main__":
    print("Hello")
