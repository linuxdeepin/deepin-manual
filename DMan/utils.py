# -*- coding: utf-8 -*-

from pathlib import Path
from gi.repository import GLib
from collections import namedtuple

import os
from urllib import parse

_DMAN_HOME = "/usr/share/dman"
_UI_NLS_DIR = os.path.normpath(os.path.dirname(__file__) + "/../PageRoot/www/nls")


def _getAvailableLanguagesFor(appName: str):
    # return an iterable of things like ["zh_CN", "en_US"]
    matches = Path("{}/{}".format(_DMAN_HOME, appName)).glob("*/index.md")
    return map(lambda match: match.parts[-2], matches)


def _getBestMatch(availables: list, prefers: list) -> str:
    for prefer in prefers:
        if prefer in availables:
            return prefer

    return "en_US"


def getDocumentLanguageFor(appName: str):
    return _getBestMatch(
        GLib.get_language_names(),
        _getAvailableLanguagesFor(appName))


def _getAvailableUILanguages():
    matches = Path(_UI_NLS_DIR).glob("*.json")
    return map(lambda match: match.name.split(".")[0], matches)


def getUILanguage():
    return _getBestMatch(
        GLib.get_language_names(),
        _getAvailableUILanguages())

DManInfo = namedtuple("DManInfo", ["dir", "lang"])


def processMarkdownPath(url: str, langProvider) -> DManInfo:
    url = url.strip()

    if "/" not in url:
        url = "dman://" + url

    parsed = parse.urlparse(url)

    if parsed.scheme == "":
        parsed = parse.urlparse("file://" + url)

    if parsed.scheme not in ("file", "dman"):
        raise ValueError("Unknown scheme")

    if parsed.scheme == "dman":
        appName = parsed.netloc
        lang = langProvider(appName)

    if parsed.scheme == "dman":
        return DManInfo(dir = "file://" + os.path.normpath("/".join([_DMAN_HOME, appName, lang])),
                        lang = lang)
    else:
        return DManInfo(dir = "file://" + os.path.normpath(parsed.path),
                        lang = "")


def getLockPath(lastArgv: str) -> str:
    appName = lastArgv.replace("/", "_")
    return os.path.expanduser("~/.config/deepin-manual/{appName}.lock".format(appName = appName))


def getSocketPath(lastArgv: str) -> str:
    appName = lastArgv.replace("/", "_")
    return os.path.expanduser("~/.config/deepin-manual/{appName}.lock.socket".format(appName = appName))
