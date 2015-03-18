#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from dae.daeclient import DAEClient
from dae.utils import get_parent_dir

import signal
signal.signal(signal.SIGINT, signal.SIG_DFL)

if __name__ == '__main__':
    app_dir = get_parent_dir(__file__)
    DAEClient(app_dir)
