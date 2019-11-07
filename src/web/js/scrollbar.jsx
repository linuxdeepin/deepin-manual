import React from 'react';
import { Scrollbars } from 'react-custom-scrollbars';

function renderScrollBarTrackHorizontal(props) {
  return (
    <div/>
  );
}

export default function(props) {
  return (
    <Scrollbars {...props} className="scrollbar" autoHide renderTrackHorizontal={renderScrollBarTrackHorizontal} autoHideTimeout={800}>
      {props.children}
    </Scrollbars>
  );
}
