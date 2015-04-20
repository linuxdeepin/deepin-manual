#!/usr/bin/python3 -OO
# -*- coding: utf-8 -*-

from dae.daeclient import DAEClient
import os, sys

if __name__ == "__main__":
    path = os.path.normpath(os.path.dirname(__file__) + "/../PageRoot")
    DAEClient(path)
