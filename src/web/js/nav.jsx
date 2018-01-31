import React, { Component } from 'react';
import ReactDOM from 'react-dom';
import { Link } from 'react-router-dom';
import Scrollbar from './scrollbar.jsx';

class Nav extends Component {
  componentDidMount() {
    document.getElementById('article').style.marginLeft = ReactDOM.findDOMNode(
      this
    ).clientWidth;
    this.componentDidUpdate();
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
        style={{
          width: `calc(${maxWidth}px + ${c}rem`
        }}
      >
        <Scrollbar>
          <Link to="/index">
            <div type="h2" id="backHome" className="h">
              {global.i18n['ToIndexPage']}
            </div>
          </Link>
          {this.props.hlist.map(h => {
            return (
              <div
                key={h.id}
                cid={h.id}
                type={h.type}
                className={this.props.hash == h.id ? 'h hash' : 'h'}
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
