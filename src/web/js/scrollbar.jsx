import React from 'react';
import ReactDOM from 'react-dom';
import { Scrollbars } from 'react-custom-scrollbars';
var meventy=0;
export default class extends React.Component {
  componentDidMount() {
    var myself=ReactDOM.findDOMNode(this.refs.myselfref);
    //console.log("myself:",myself);
    var getObj = myself.querySelector(":scope >div:last-child div");
    getObj.addEventListener("mouseenter", function(event){ 
    console.log("mouseenter");
    getObj.setAttribute("user-using","none;");
    var vtransform= getObj.style.transform;
    var vtranslateY=vtransform.replace("translateY(","").replace("px)","");
    console.log("enter vtranslateY:"+vtranslateY);
    console.log("enter y:"+event.clientY);
    var vY = event.clientY-vtranslateY-18;
    meventy=event.clientY;
    console.log("enter vY:"+vY);
    var result = document.getElementsByClassName('scrollbar')[0];
    result.style.setProperty('--mouse-y', vY+('px'));
    });
    getObj.addEventListener("mouseleave", function(event){ 
      console.log("mouseleave");
      getObj.removeAttribute('user-using');
    });
    getObj.addEventListener("mousemove", function(event){ 
      console.log("mousemove");
      if(event.clientY-meventy>3||event.clientY-meventy<-3)
      meventy=event.clientY;
      console.log("meventy",meventy);
    });
  }
  scrollbarsscrollstart(){
    console.log("ScrollStart");
    // console.log("event",event);
    var myself=ReactDOM.findDOMNode(this.refs.myselfref);
      console.log("myself:",myself);
      var getObj = myself.querySelector(":scope >div:last-child div");
      console.log("getObj:",getObj);
    var vtransform= getObj.style.transform;
      var vtranslateY=vtransform.replace("translateY(","").replace("px)","");
      console.log("vtranslateY:"+vtranslateY);
      var vY = meventy-vtranslateY-18;
      console.log("vY:"+vY);
      var result = document.getElementsByClassName('scrollbar')[0];
      result.style.setProperty('--mouse-y', vY+('px'));
  }
  render() {
    return (
      <Scrollbars {...this.props} className="scrollbar" ref="myselfref" onScrollStart={() => this.scrollbarsscrollstart()} autoHide autoHideTimeout={800}>
      {this.props.children}
    </Scrollbars>
    );
  };
}