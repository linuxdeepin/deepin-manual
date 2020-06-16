import React, { Component } from 'react';
import ReactDOM from 'react-dom';
import { Link } from 'react-router-dom';
import Scrollbar from './scrollbar.jsx';
import { kMaxLength } from 'buffer';
// import webjs from './web.js'



class Nav extends Component {
  componentDidMount() {
    document.getElementById('article').style.marginLeft = ReactDOM.findDOMNode(
      this
    ).clientWidth;
    this.componentDidUpdate();
  }
  shouldComponentUpdate(newProps, newState) {
    console.log("global.hash" + global.hash);
    console.log("shouldComponentUpdate newProps:" + newProps.hash + ", old hash:" + global.oldHash);

    if (' ' == global.hash) {
      return true;
    }

    if (' ' == global.oldHash) {
      return true;
    }

    if ('POP' == global.lastAction) {
      return true;
    }

    if (global.hash != global.oldHash) {
      return false;
    }
    if (newProps.hash != global.oldHash) {
      return false;
    }
    return true;
  }

  componentWillUpdate() {
    console.log("componentWillUpdate");
  }
  componentDidUpdate() {
    let hashDOM = ReactDOM.findDOMNode(this).querySelector('.hash');
    if (hashDOM == null) {
      return;
    }
    if (hashDOM.getAttribute('cid') == this.props.hlist[0].id) {
      document.getElementById('backHome').scrollIntoViewIfNeeded(false);
      return;
    }
    hashDOM.scrollIntoViewIfNeeded(false);
  }
  click(e) {
    let cid = e.target.getAttribute('cid');
    if (cid) {
      console.log('搜索结果', cid);
      global.hash = cid;
      global.oldHash = cid;
      global.isMouseClickNav = true;
      global.isMouseScrollArticle = false;
      this.props.setHash(cid);
    }
  }
  wheel(e) {
    let nav = ReactDOM.findDOMNode(this);
    if (e.deltaY > 0) {
      if (
        e.deltaY > 0 &&
        nav.scrollHeight == nav.clientHeight + nav.scrollTop
      ) {
        e.preventDefault();
      } else if (e.deltaY < 0 && nav.scrollTop == 0) {
        e.preventDefault();
      }
    }
  }

  //右键菜单事件,去除选中状态
  contentMenu(e){
    e.preventDefault();
    document.getSelection().empty();
  }
/*
  mouseOver(e){
    var value =  e.currentTarget.innerHTML;
    console.log("mouse over:" + value);
    // var showFloatTimer=null;
    
    // function(event){
      clearTimeout(this.showFloatTimer);
      this.showFloatTimer=setTimeout(function(){
          $('.tooltip-wp').attr('data-title', value); //动态设置data-title属性
          $('.tooltip-wp').fadeIn(200);//浮动框淡出
      },300);
  }

  mouseOut(e){
    console.log("mouse out" + e.target.getAttribute('cid'));
    $('.tooltip-wp').hide();
    // }
  }

  mouseMove(e){
    console.log("mouse move : ");

    var xClient = e.clientX;
    var yClient = e.clientY;
    console.log(" --:"+ xClient + "  "+ yClient);

    var xPage = e.pageX;
    var yPage = e.pageY + 20;
    var canRun=true;
    // return function(){//e是mousemove的event参数
        if(!canRun){return;}//如果有一个定时方法，直接返回
        canRun=false;
        setTimeout(function(){
            var top = e.pageY+5;
            var left = e.pageX+5;
            $('.tooltip-wp').css({
                'top' : yPage + 'px',
                'left': xPage+ 'px'
            });
            canRun=true;
        },150);
    
  }
  */

  render() {
    let max = this.props.hlist[0];
    this.props.hlist.map(h => {
      if (max.text.length < h.text.length) {
        max = h;
      }
    });
    console.log(max, max.text.length);
    // let maxWidth = 0;
    // if (global.lang == 'zh_CN') {
    //   maxWidth = max.text.length * 16;
    // } else {
    //   maxWidth = max.text.length / 1.8;
    // }
    let maxWidth = max.text.length;
    let c = 0;
    if (global.lang === 'zh_CN') {
      if (maxWidth <= 6) {
        c = 3;
      } else {
        c = 1;
      }
      maxWidth *= 18;
    } else {
      if (maxWidth <= 20) {
        c = 2;
      }
      maxWidth *= 9;
    }
    return (
      <div
        id="nav"
        lang={global.lang}
        onClick={e => this.click(e)}
        onContextMenu={this.contentMenu.bind(this)}
        style={{
          width: `calc(${maxWidth}px + ${c}rem`
        }}
      >
        <Scrollbar>
          <div
            type="h2"
            id="backHome"
            className="h"
            onClick={() => global.backHome()}
          >
            {global.i18n['ToIndexPage']}
          </div>
          {this.props.hlist.map(h => {
            return (
              <div
                key={h.id}
                cid={h.id}
                type={h.type}
                className={this.props.hash == h.id ? 'h hash' : 'h'}
                // onMouseOver={this.mouseOver.bind(this)}
                // onMouseOut={this.mouseOut.bind(this)}
                // onMouseMove={this.mouseMove.bind(this)}
                onMouseOver={(e)=>this.props.onNavOver(e)}
                onMouseOut ={(e)=>this.props.onNavOut(e)}
                onMouseMove={(e)=>this.props.onNavMove(e)}
              >
                {h.text}
              </div>
            );
          })}
        </Scrollbar>
      </div>
    );
  }
}

export default Nav;
