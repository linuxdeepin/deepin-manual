// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

import React from 'react';
import { Scrollbars } from 'react-custom-scrollbars';

function renderScrollBarTrackHorizontal(props) {
  return (
    <span/>
  );
}

export default function(props) {
  return (
    <Scrollbars {...props} className="scrollbar" autoHide renderTrackHorizontal={renderScrollBarTrackHorizontal} autoHideTimeout={800}>
      {props.children}
    </Scrollbars>
  );
}
