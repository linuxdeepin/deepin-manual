# SPDX-FileCopyrightText: 2022-2026 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: CC0-1.0

#!/bin/bash
# 定位到项目根目录（脚本所在目录的上一级）
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

builddir=build
reportdir=build-ut

rm -rf "$PROJECT_ROOT/$builddir"
rm -rf "$PROJECT_ROOT/$reportdir"
mkdir -p "$PROJECT_ROOT/$builddir"
mkdir -p "$PROJECT_ROOT/$reportdir"
cd "$PROJECT_ROOT/$builddir"

#编译
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_SAFETYTEST_ARG="CMAKE_SAFETYTEST_ARG_ON" "$PROJECT_ROOT"
make -j$(nproc)

#生成asan日志和ut测试xml结果
mkdir -p report
./tests/deepin-manual-test --gtest_output=xml:./report/report_deepin-manual.xml

workdir="$PROJECT_ROOT/$builddir"

#统计代码覆盖率并生成html报告
lcov -d "$workdir" -c -o ./coverage.info

lcov --extract ./coverage.info '*/src/*' -o ./coverage.info

lcov --remove ./coverage.info '*/tests/*' -o ./coverage.info

genhtml -o ./html ./coverage.info

mv ./html/index.html ./html/cov_deepin-manual.html
#对asan、ut、代码覆盖率结果收集至指定文件夹
cp -r html "$PROJECT_ROOT/$reportdir/"
cp -r report "$PROJECT_ROOT/$reportdir/"
cp -r asan*.log* "$PROJECT_ROOT/$reportdir/asan_deepin-manual.log" 2>/dev/null || true

exit 0
