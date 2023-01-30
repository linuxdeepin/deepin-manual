# SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: CC0-1.0

#!/bin/bash

# Start new chromium profile with web security off.

chromium --user-data-dir=/tmp/deepin-manual --disable-web-security
