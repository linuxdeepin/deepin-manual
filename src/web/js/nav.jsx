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
    console.log(max);
    let maxWidth =
      global.lang === 'zh_CN' ? max.text.length : max.text.length / 2;
    maxWidth += 1;
    console.log(maxWidth);
    return (
      <div
        id="nav"
        lang={global.lang}
        onClick={e => this.click(e)}
        style={{ width: maxWidth + 'rem' }}
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
                <br />
              </div>
            );
          })}
        </Scrollbar>
      </div>
    );
  }
}

export default Nav;
