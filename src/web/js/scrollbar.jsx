import React from 'react';
import { Scrollbars } from 'react-custom-scrollbars';
var ScrollbarsIdNum=0;
export default function(props) {
  ScrollbarsIdNum++;
  var ScrollbarsId="scrollbar"+ScrollbarsIdNum;

function scrollbarsmover(e,id){
   var ev = e || window.event ;
   //console.log(id);
   var getDoc = document.getElementById(id);
   //console.log("getDoc:",getDoc);
   var getObj = document.querySelector("[id="+id+"] >div:last-child div");
   //console.log("getObj:",getObj);
   var vtransform= getObj.style.transform;
   var vtranslateY=vtransform.replace("translateY(","").replace("px)","");
   //console.log("vtranslateY:"+vtranslateY);
   var vY = e.clientY-vtranslateY-18;
  //  console.log("vY:"+vY);
   var result = document.getElementsByClassName('scrollbar')[0]
   result.style.setProperty('--mouse-y', vY+('px'));
}
  return (
    <Scrollbars {...props} className="scrollbar" id={ScrollbarsId} onMouseOver={(e) => scrollbarsmover(e,ScrollbarsId)} autoHide autoHideTimeout={800}>
      {props.children}
    </Scrollbars>
  );
}
