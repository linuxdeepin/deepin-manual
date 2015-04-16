# -*- coding: utf-8 -*-

from unittest import TestCase
from unittest.mock import Mock
from utils import processMarkdownPath, _DMAN_HOME


class DManSchemeTest(TestCase):
    def test_normal(self):
        fn = Mock(return_value = "en_US")
        r = processMarkdownPath("dman://MyApp", fn)
        self.assertEqual(r.dir,
                         "file://" + _DMAN_HOME + "/MyApp/en_US")
        fn.assert_called_once_with("MyApp")
        self.assertEqual(r.lang, "en_US")

    def test_override_lang(self):
        fn = Mock(return_value = "zh_CN")
        r = processMarkdownPath("dman://MyApp2", fn)
        self.assertEqual(r.dir,
                         "file://" + _DMAN_HOME + "/MyApp2/zh_CN")
        fn.assert_called_once_with("MyApp2")
        self.assertEqual(r.lang, "zh_CN")

    def test_dman_omitted(self):
        fn = Mock(return_value = "ja_JP")
        r = processMarkdownPath("MyApp3", fn)
        self.assertEqual(r.dir,
                         "file://" + _DMAN_HOME + "/MyApp3/ja_JP")
        fn.assert_called_once_with("MyApp3")
        self.assertEqual(r.lang, "ja_JP")


class FileSchemeTest(TestCase):
    def test_file_omitted(self):
        fn = Mock(return_value = "zh_CN")
        r = processMarkdownPath("/home/user/documents/MyApp", fn)
        self.assertEqual(r.dir, "file:///home/user/documents/MyApp")
        self.assertEqual(0, fn.call_count)
        self.assertEqual(r.lang, "")

    def test_file_scheme(self):
        fn = Mock(return_value = "")
        r = processMarkdownPath("file:///home/user/documents/MyApp", fn)
        self.assertEqual(r.dir, "file:///home/user/documents/MyApp")
        self.assertEqual(0, fn.call_count)
        self.assertEqual(r.lang, "")
