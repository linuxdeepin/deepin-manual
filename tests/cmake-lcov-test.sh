# SPDX-FileCopyrightText: 2022-2026 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: CC0-1.0

#!/bin/bash
# 定位到项目根目录（脚本所在目录的上一级）
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

utdir=build-ut

rm -rf "$PROJECT_ROOT/$utdir"
mkdir -p "$PROJECT_ROOT/$utdir"
cd "$PROJECT_ROOT/$utdir"

cmake -DCMAKE_BUILD_TYPE=Debug "$PROJECT_ROOT"
make -j$(nproc)

workdir="$PROJECT_ROOT/$utdir"

app_name=deepin-manual-test

mkdir -p report

./tests/$app_name --gtest_output=xml:./report/report.xml

lcov -d "$workdir" -c -o ./report/coverage.info

#以下几行是过滤一些我们不感兴趣的文件的覆盖率信息
lcov --extract ./report/coverage.info '*/src/*' -o ./report/coverage.info

lcov --remove ./report/coverage.info '*/tests/*' -o ./report/coverage.info

genhtml -o ./report ./report/coverage.info

exit 0
