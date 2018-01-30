import React from 'react';
import { Scrollbars } from 'react-custom-scrollbars';

export default function(props) {
  return (
    <Scrollbars {...props} className="scrollbar" autoHide autoHideTimeout={800}>
      {props.children}
    </Scrollbars>
  );
}
