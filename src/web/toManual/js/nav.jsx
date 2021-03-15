import React, { Component } from 'react';
import ReactDOM from 'react-dom';
import { Link } from 'react-router-dom';
import Scrollbar from './scrollbar.jsx';

class Nav extends Component {
  constructor(props) {
    super(props);
    this.contentMenu = this.contentMenu.bind(this)
  }

  componentWillMount(){
    console.log("nav componentWillMount");
  }
  componentDidMount() {
    console.log("nav componentDidMount");
    document.getElementById('article').style.marginLeft = ReactDOM.findDOMNode(
      this
    ).clientWidth;
    this.componentDidUpdate();
  }
  
  shouldComponentUpdate(newProps, newState) {
    console.log("nav shouldComponentUpdate newProps:" + newProps.hash +" global hash:" + global.hash);

    if ('' == global.hash) {
      return true;
    }

    if ('POP' == global.lastAction) {
      return true;
    }
    //why........
    if (newProps.hash != global.hash) {
      return false;
    }
    return true;
  }

  componentWillUpdate() {
    console.log("nav componentWillUpdate");
  }

  componentDidUpdate() {
    console.log("nav componentDidUpdate");
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

  render() {
    console.log("nav render...",this.props.hlist.length);
    let maxWidth = 0;
    let c = 0;

    if (this.props.hlist.length > 0)
    {
      let max = this.props.hlist[0];
      this.props.hlist.map(h => {
        if (max.text.length < h.text.length) {
          max = h;
        }
      });
      maxWidth = max.text.length;
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
    }
    
    return (
      <div
        id="nav"
        lang={global.lang}
        onMouseDown={e => this.click(e)}
        onContextMenu={this.contentMenu}
        // style={{
        //   width: `calc(${maxWidth}px + ${c}rem)`
        // }}
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
