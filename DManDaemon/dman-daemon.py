#!/usr/bin/env python3
# Copyright (c) 2015 XuShaohua. All rights reserved.
# Use of this code is governed by a GPLv3-style license that can be
# found in the LICENSE file.

import subprocess
import sys
import time

def main():
    args = ("/usr/bin/xvfb-run", "dman")
    duration = 10

    pipe = subprocess.Popen(args)
    time.sleep(duration)
    sys.exit(0)

if __name__ == '__main__':
    main()
