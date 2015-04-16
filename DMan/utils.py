# -*- coding: utf-8 -*-

from pathlib import Path
from gi.repository import GLib
from collections import namedtuple

import os
from urllib import parse

_DMAN_HOME = "/usr/share/dman"


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
