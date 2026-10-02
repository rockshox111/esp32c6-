#!/bin/bash
FONT_OUT=main/ui/zh_font.c
CHARS="文件管理播放系统设置返回上级删除新建时间日期内存负载任务堆栈确认取消主页监控CPU状态SD卡打开查看"
which lv_font_conv >/dev/null 2>&1 || { echo "请先安装 lv_font_conv: npm i -g lv_font_conv"; exit 1; }
lv_font_conv --font DejaVuSans.ttf --font NotoSansCJKsc-Regular.otf \
  --size 16 --bpp 2 --dpi 130 --symbols "$CHARS" --format lvgl8 --output $FONT_OUT
echo "生成 $FONT_OUT"
