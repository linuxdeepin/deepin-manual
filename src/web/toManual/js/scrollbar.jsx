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
  return (
    <div {...props} style={Object.assign({}, props.style, { overflowX: 'hidden', marginBottom: 0 })} />
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
