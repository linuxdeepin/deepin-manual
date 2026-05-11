// SPDX-FileCopyrightText: 2022 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

import React, { useRef, useEffect } from 'react';
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
  const scrollbarsRef = useRef(null);
  const timeoutRef = useRef(null);

  useEffect(() => {
    const scrollbars = scrollbarsRef.current;
    if (!scrollbars) return;

    const showScrollbar = () => {
      const container = scrollbars.container;
      if (container) {
        container.style.setProperty('--scrollbar-opacity', '1');
      }
      if (timeoutRef.current) {
        clearTimeout(timeoutRef.current);
      }
      timeoutRef.current = setTimeout(hideScrollbar, 800);
    };

    const hideScrollbar = () => {
      if (timeoutRef.current) {
        clearTimeout(timeoutRef.current);
        timeoutRef.current = null;
      }
      const container = scrollbars.container;
      if (container) {
        container.style.setProperty('--scrollbar-opacity', '0');
      }
    };

    const view = scrollbars.view;
    const container = scrollbars.container;
    if (view) {
      view.addEventListener('scroll', showScrollbar);
    }
    if (container) {
      container.addEventListener('mousemove', showScrollbar);
      container.addEventListener('mouseleave', hideScrollbar);
    }

    return () => {
      if (view) {
        view.removeEventListener('scroll', showScrollbar);
      }
      if (container) {
        container.removeEventListener('mousemove', showScrollbar);
        container.removeEventListener('mouseleave', hideScrollbar);
      }
      if (timeoutRef.current) {
        clearTimeout(timeoutRef.current);
      }
    };
  }, []);

  return (
    <Scrollbars
      {...props}
      ref={scrollbarsRef}
      className="scrollbar"
      renderTrackHorizontal={renderScrollBarTrackHorizontal}
      renderView={renderView}
    >
      {props.children}
    </Scrollbars>
  );
}
