// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

module.exports = {
    entry:  __dirname + "/app/main.js", // 之前提到的唯一入口文件
    output: {
        path: __dirname + "../../../web_dist/toSearchMd/", // 打包后的文件存放的地方
        filename: "index.js" // 打包后输出文件的文件名
    },
    module: {
        rules: [
        //   {
        //     test: require.resolve('jquery'),
        //     loader: 'expose-loader',
        //     options: {
        //       exposes: {
        //         globalName: '$',
        //         override: true,
        //       },
        //     },
        //   },
        ],
      },
    node: {
        fs: 'empty'
    },
}
