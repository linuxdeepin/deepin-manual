// SPDX-FileCopyrightText: 2022 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

import React from 'react';
import { Scrollbars } from 'react-custom-scrollbars';

function renderScrollBarTrackHorizontal(props) {
  return (
    <span/>
  );
}

function renderView(props) {
  // 修复水平滚动条和最后一行被遮挡问题：
  // 1. overflowX: 'hidden' - 防止水平滚动条
  // 2. marginBottom: 0 - 修复最后一行被遮挡（移除负边距）
  // 注意：保留 marginRight 的负边距，不破坏 react-custom-scrollbars 隐藏滚动条的机制
  const mergedStyle = Object.assign({}, props.style, {
    overflowX: 'hidden',
    marginBottom: 0
  });

  return (
    <div {...props} style={mergedStyle} />
  );
}

export default function(props) {
  return (
    <Scrollbars
      {...props}
      className="scrollbar"
      autoHide
      renderTrackHorizontal={renderScrollBarTrackHorizontal}
      renderView={renderView}
      autoHideTimeout={800}
    >
      {props.children}
    </Scrollbars>
  );
}
