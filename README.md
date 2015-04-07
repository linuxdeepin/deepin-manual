# 安装编译说明

## 环境预备
1. 安装node 0.12或更新。
2. 安装依赖。
```bash
npm install
```
3.
```bash
gulp watch
```

## I18N
``` bash
gulp pot
gulp translations
```

## 运行
```bash
./main.js Markdown_Path
```

如果需要带调试运行，设置DEBUG环境变量为非空值。
