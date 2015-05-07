#!/usr/bin/python3 -OO
# -*- coding: utf-8 -*-

from dae.daeclient import DAEClient
import os, sys

import fcntl
from utils import getLockPath, getSocketPath


if __name__ == "__main__":
    lockPath = getLockPath(sys.argv[-1])
    fd = os.open(lockPath, os.O_RDWR | os.O_CREAT)
    alreadyRunning = False
    try:
        fcntl.flock(fd, fcntl.LOCK_EX | fcntl.LOCK_NB)
    except BlockingIOError:
        alreadyRunning = True

    if alreadyRunning:
        from multiprocessing.connection import Client

        with Client(getSocketPath(sys.argv[-1]), "AF_UNIX") as conn:
            conn.send(1)

    else:
        path = os.path.normpath(os.path.dirname(os.path.realpath(__file__)) + "/../PageRoot")
        DAEClient(path)
