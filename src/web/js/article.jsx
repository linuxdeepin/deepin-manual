import React, { Component } from 'react';
import ReactDOM from 'react-dom';
import scrollIntoView from 'smooth-scroll-into-view-if-needed';

import m2h from './mdToHtml';
import Scrollbar from './scrollbar.jsx';

export default class Article extends Component {
  constructor(props) {
    super(props);
    this.state = {
      preview: null,
      smoothScroll: false,
      fillblank: null,
      bIsTimerOut:true
    };

    var timerObj;
  }
  //滚动到锚点
  scrollToHash() {
    let tempHash = this.hash;

    // if (tempHash == 'h21')
    // {
    //   tempHash = 'h250';
    // }
    
    const hashNode = document.getElementById(tempHash);
    console.log("article scrollToHash temphash: " + tempHash + " " + hashNode);

    if (this.state.preview != null) {
      this.setState({ preview: null });
    }

    if (hashNode) {

      clearTimeout(this.timerObj);
      this.setState({ smoothScroll: true });

      this.timerObj = setTimeout(() => {
          this.setState({ smoothScroll: false });
      },800);

      scrollIntoView(hashNode, { behavior: 'smooth', block: 'start' }).then(() => {
        console.log("scrollIntoView finish..");
        //find parent h3 title of h4 title
        let hList = ReactDOM.findDOMNode(this).querySelectorAll('h2,h3,h4,h5');
        var currH3Hash = '';
        for (let i = 0; i < hList.length; i++) {
          if (hList[i].tagName == 'H3') {
            currH3Hash = hList[i].id; 
          }

          if (tempHash == hList[i].id && (hList[i].tagName == 'H4' || hList[i].tagName == 'H5')) {
            console.log("article: scroll hlist:" + hList[i].tagName  + "," + hList[i].id);
            console.log("currH3Hash:" + currH3Hash);
            this.hash = currH3Hash;
            this.props.setHash(currH3Hash);
            this.props.setScroll(currH3Hash);
            break;
          }
        }
      });
    } else {
      this.props.setHash(this.props.hlist[0].id);
    }
  }
  componentDidUpdate() {
    console.log("article componentDidUpdate.." + this.hash + " props hash->"+ this.props.hash);
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
            // console.log('image loaded');
            this.load = true;
            this.scrollToHash();
            let last = article.querySelector(
              '#' + this.props.hlist[this.props.hlist.length - 1].id,
            );
            let fillblank = {
              marginBottom: article.clientHeight - (read.clientHeight - last.offsetTop),
            };
            this.setState({
              fillblank,
            });
          }
        };
        el.onerror = () => {
          if (el.getAttribute('src') == el.dataset.src) {
            el.onload();
          } else {
            el.src = el.dataset.src;
          }
        };
      });
    }
  }

  componentWillUpdate() {
    console.log("article componentWillUpdate..");
    var alink_arr = document.getElementsByTagName('a');
    for(var i=0; i<alink_arr.length; i++) {
      alink_arr[i].onclick = function () {
        global.isLinkClicked = true;
      };
    }
  }

  componentDidMount() {
    this.componentDidUpdate();
  }

  componentWillReceiveProps(nextProps) {
    console.log("article componentWillReceiveProps..");
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

  handleWheelScroll(e){
    global.isMouseScrollArticle = true;
  }

  handleKeyDown(e) {
    if (38 == e.keyCode || 40 == e.keyCode)
    {
      global.isMouseScrollArticle = true;
    }
  }

  handleKeyUp(e) {
    if (38 == e.keyCode || 40 == e.keyCode)
    {
      global.isMouseScrollArticle = true;
    }
  }

  //滚动事件
  scroll() {
    if (!this.load) {
      return;
    }
    if (this.state.smoothScroll) {
      return;
    }
    if (this.state.preview != null) {
      this.setState({ preview: null });
    }
    let hList = ReactDOM.findDOMNode(this).querySelectorAll('h2,h3');
    let aritleView = this.refs.articleView;

    let hash = hList[0].id;
    for (let i = 0; i < hList.length; i++) {
      //console.log("article: scroll hlist:" + hList[i]);
      //console.log("article: scroll hlist offset top:" + hList[i].getBoundingClientRect().top);
      let articleTop = Math.abs(aritleView.getBoundingClientRect().top);
      //console.log(hList[i].id + "," + hList[i].nodeName + ", hList[" + i + "].offsetTop" + hList[i].offsetTop + ", articleTop" + articleTop);
      let offsetY = 10;
      if (hList[i].nodeName == 'H2') {
        offsetY = 10;
      }
      else if (hList[i].nodeName == 'H3') {
        offsetY = 30;
      }
      if (hList[i].offsetTop - offsetY >= articleTop) {
        //console.log("article: scroll hlist offset top:" + hList[i].offsetTop);
        break;
      }
      hash = hList[i].id;
    }
    console.log("article: scroll this.hash:"  + this.hash  + "   hash:" + hash);
    if (this.hash != hash) {
      console.log('article: scroll hash update');
      this.hash = hash;
      this.props.setHash(hash);
      if (global.isMouseScrollArticle)
      {
        this.props.setScroll(hash);
      }
    }
  }
  //内部链接预览
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
      let { top, left, right } = rect;
      let style = {};
      let tClass = 't_';

      console.log("left-right:"+(left) + "  - "+ right);
      //center
      if (((right + left)/2 > (300 + 170)) && (((right + left)/2 + 300 < document.body.clientWidth))) {
        style.left　=　(right + left)/2 - 300;
        tClass += 'center_';
      }
      //right
      else if (((right + left)/2  > (600 + 170)))
      {
        style.left　=　right - 600;
        tClass += 'right_';
      }
      //left
      else if ((right + left)/2 <= (300 + 170))
      {
        style.left　=　left;
        tClass += 'left_';
      }
      //left
      else {
        style.left　= 170;
        tClass += 'left_';
        
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
        const httpProtocol = 'http';
        const href = e.target.getAttribute('href');
        console.log("href:"+ href);
        switch (0) {
          case href.indexOf(hashProtocol):
            e.preventDefault();
            this.props.setHash(document.querySelector(`[text="${href.slice(1)}"]`).id);
            return;
          case href.indexOf(dmanProtocol):
            e.preventDefault();
            const [appName, hash] = href.slice(dmanProtocol.length + 1).split('#');
            const rect = e.target.getBoundingClientRect();
            this.showPreview(appName, hash, rect);
            return;
          case href.indexOf(httpProtocol):
            e.preventDefault();
            global.qtObjects.imageViewer.openHttpUrl(href);
            return;
        }
    }
  }

  //右键菜单事件
  contentMenu(e){
    switch (e.target.nodeName) {
      //当前为图片或者链接时,右键清除选中状态.
      //当前为图片
      case 'IMG':
        e.preventDefault();
        document.getSelection().empty();  
        return;
      //当前为链接
      case 'A':
        e.preventDefault();
        document.getSelection().empty();
        return;
      }
  }

  render() {
    return (
          <div id="article">
            <div id="article_bg">
              <Scrollbar onScroll={this.scroll.bind(this)}
                         onWheel={(e) => this.handleWheelScroll(e)}
                         onKeyUp={(e) => this.handleKeyUp(e)}
                         onKeyDown={(e) => this.handleKeyDown(e)}>
                <div
                  id="read"
                  ref='articleView'
                  className="read"
                  tabIndex="-1"
                  dangerouslySetInnerHTML={{ __html: this.props.html }}
                  style={this.state.fillblank}
                  onClick={this.click.bind(this)}
                  onContextMenu={this.contentMenu.bind(this)}
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
                            __html: this.state.preview.html,
                          }}
                        />
                      </Scrollbar>
                    </div>
                  </div>
                )}
              </Scrollbar>
            </div>
          </div>
    );
  }
}
