import React, { Component } from 'react';
import ReactDOM from 'react-dom';

import m2h from './mdToHtml';
import Scrollbar from './scrollbar.jsx';

export default class Article extends Component {
  constructor(props) {
    super(props);
    this.state = {
      preview: null,
      fillblank: null
    };
  }
  //滚动到锚点
  scrollToHash() {
    document.getElementById(this.hash).scrollIntoView();
  }
  componentDidUpdate() {
    if (this.hash != this.props.hash) {
      this.hash = this.props.hash;
      this.scrollToHash();
    }
    if (!this.load) {
      let article = ReactDOM.findDOMNode(this);
      let read = article.querySelector('#read');
      read.focus();
      let imgList = [...article.querySelectorAll('img')];
      let loadCount = 0;
      imgList.map(el => {
        el.onload = () => {
          loadCount++;
          if (loadCount == imgList.length) {
            this.load = true;
            this.scrollToHash();
            let last = article.querySelector(
              '#' + this.props.hlist[this.props.hlist.length - 1].id
            );
            // console.log(article, article.clientHeight - last.offsetTop);
            let fillblank = {
              marginBottom:
                article.clientHeight - (read.clientHeight - last.offsetTop)
            };
            console.log(fillblank);
            this.setState({
              fillblank
            });
          }
        };
        el.onerror = el.onload;
      });
    }
  }
  componentDidMount() {
    this.componentDidUpdate();
  }
  componentWillReceiveProps(nextProps) {
    if (nextProps.file != this.props.file) {
      this.hash = '';
      this.load = false;
    }
  }
  gethID(htext) {
    let id = this.props.hlist[0].id;
    console.log(this.props.hlist[0]);
    let hlist = this.props.hlist.filter(h => h.text == htext);
    if (hlist.length > 0) {
      id = hlist[0].id;
    }
    console.log(htext, id);
    return id;
  }
  //滚动事件
  scroll() {
    if (!this.load) {
      return;
    }
    if (this.state.preview != null) {
      this.setState({ preview: null });
    }
    let hList = ReactDOM.findDOMNode(this).querySelectorAll('h2,h3');
    let hash = hList[0].id;
    for (let i = 0; i < hList.length; i++) {
      if (hList[i].getBoundingClientRect().top > 1) {
        break;
      }
      hash = hList[i].id;
    }
    if (this.hash != hash) {
      console.log('hash update');
      this.hash = hash;
      this.props.setHash(hash);
    }
  }
  //图片全屏预览
  showPreview(appName, hash, rect) {
    let file = `${global.path}/${appName}/${global.lang}/index.md`;
    global.readFile(file, data => {
      let { html } = m2h(file, data);
      let d = document.createElement('div');
      d.innerHTML = html;
      let hashDom = d.querySelector(`[text="${hash}"]`);
      let DomList = [hashDom];
      let nextDom = hashDom.nextElementSibling;
      while (nextDom) {
        if (nextDom.nodeName == hashDom.nodeName) {
          break;
        }
        DomList.push(nextDom);
        nextDom = nextDom.nextElementSibling;
      }
      d.innerHTML = '';
      DomList.map(el => d.appendChild(el));
      html = d.innerHTML;
      let { top, left } = rect;
      let style = {};
      let tClass = 't_';
      if (left > document.body.clientWidth / 2) {
        tClass += 'right_';
        style.left = left - 500 + 60 + rect.width - 5;
      } else {
        tClass += 'left_';
        style.left = left + rect.width - 60 - 15;
      }
      if (top > document.body.clientHeight / 2) {
        tClass += 'down';
        style.top = top - 250 - 20;
      } else {
        tClass += 'up';
        style.top = top + rect.height + 10;
      }
      this.setState({ preview: { html, style, tClass } });
    });
  }
  //链接处理
  click(e) {
    if (this.state.preview != null) {
      this.setState({ preview: null });
    }
    switch (e.target.nodeName) {
      case 'IMG':
        e.preventDefault();
        let src = e.target.src;
        if (src.indexOf('.svg') != -1) {
          return;
        }
        console.log('imageViewer', src);
        global.qtObjects.imageViewer.open(src);
        return;
      case 'A':
        const dmanProtocol = 'dman://';
        const hashProtocol = '#';
        const href = e.target.getAttribute('href');
        switch (0) {
          case href.indexOf(hashProtocol):
            e.preventDefault();
            this.props.setHash(
              document.querySelector(`[text="${href.slice(1)}"]`).id
            );
            return;
          case href.indexOf(dmanProtocol):
            e.preventDefault();
            const [appName, hash] = href
              .slice(dmanProtocol.length + 1)
              .split('#');
            const rect = e.target.getBoundingClientRect();
            this.showPreview(appName, hash, rect);
            return;
        }
    }
  }
  render() {
    return (
      <div id="article" onClick={this.click.bind(this)}>
        <Scrollbar onScroll={this.scroll.bind(this)}>
          <div
            id="read"
            className="read"
            tabIndex="-1"
            dangerouslySetInnerHTML={{ __html: this.props.html }}
            style={this.state.fillblank}
            onMouseOver={e => document.getElementById('read').focus()}
          />
          {this.state.preview != null && (
            <div
              style={this.state.preview.style}
              className={this.state.preview.tClass}
              id="preview"
            >
              <div id="view">
                <Scrollbar>
                  <div
                    className="read"
                    dangerouslySetInnerHTML={{
                      __html: this.state.preview.html
                    }}
                  />
                </Scrollbar>
              </div>
            </div>
          )}
        </Scrollbar>
      </div>
    );
  }
}
