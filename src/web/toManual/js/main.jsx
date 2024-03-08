// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

import React, { Component } from 'react';
import $ from 'jquery';

import Nav from './nav.jsx';
import Article from './article.jsx';
import m2h from './mdToHtml.js';

export default class Main extends Component {
  constructor(props) {
    super(props);
    this.state = {
      init: false,
      bTest: true
    };
    let { file, hash, key } = this.props.match.params;
    console.log("main constructor...");
    this.init(decodeURIComponent(file), hash ? decodeURIComponent(hash) : null, key);
    var showFloatTimer = null;

    this.setHash = this.setHash.bind(this);
    this.setScroll = this.setScroll.bind(this);
  }
  init(file, hash, key = '') {
    if (key !== '%') {
      key = decodeURIComponent(key)
    }
    console.log("main init==>file:", file, " hash:", hash, " key:", key);

    global.hash = hash;
    var filePath = file;

    var myPromise = new Promise(function (resolve, reject) {
      if (filePath.indexOf('/') == -1) {
        global.qtObjects.manual.appToPath(file, function (filepath) {
          filePath = filepath;
          resolve()
        })
      }
      else {
        resolve()
      }
    });
    myPromise.then(() => {
      global.readFile(filePath, data => {
        console.log("main init===>readfile finish...", filePath);
        let { html, hlist } = m2h(filePath, data, key);
        this.setState({
          file,
          html,
          hlist,
          init: true,
          hash: hash ? hash : hlist[0].id
        });
      });
    })
  }

  setHash(hash) {
    console.log("main setHash: " + hash);
    if (global.isLinkClicked) {
      console.log("main --setHash");
      global.hash = hash;
      global.isLinkClicked = false;
    }
    console.log("main*********setHash");
    this.setState({ hash });
  }

  setScroll(hash) {
    console.log("main setScroll:" + hash);
    global.hash = hash;
    this.setState({ hash });
  }

  //处理Nav类的Over Out Move事件,自定义Title框
  handleNavOver(e) {
    var value = e.currentTarget.innerHTML;
    clearTimeout(this.showFloatTimer);
    this.showFloatTimer = setTimeout(function () {
      $('.tooltip-wp').attr('data-title', value); //动态设置data-title属性
      $('.tooltip-wp').fadeIn(200);//浮动框淡出
    }, 300);
  }

  handleNavOut(e) {
    clearTimeout(this.showFloatTimer);
    $('.tooltip-wp').hide();
  }

  handleNavMove(e) {
    var xPage = e.pageX + 10;
    var yPage = e.pageY;
    console.log("...", document.body.scrollHeight);
    if ((yPage + 40) > document.body.scrollHeight) {
      yPage = document.body.scrollHeight - 40;
    }
    setTimeout(function () {
      $('.tooltip-wp').css({
        'top': yPage + 'px',
        'left': xPage + 'px'
      });
    }, 150);
  }

  componentWillReceiveProps(nextProps) {
    let { file, hash, key } = nextProps.match.params;

    if (global.bIsReload) {
      var parentText;
      var hashText = '';

      for (var i = 0; i < this.state.hlist.length; i++) {
        var h = this.state.hlist[i];
        if (h.type == 'h2') {
          parentText = h.text;
        }
        if (h.id == this.state.hash) {
          hashText = h.text;
          break;
        }
      }

      var filePath;

      // if (this.state.file == "dde")
      // {
      //   filePath = `${global.path}/system/${this.state.file}/${global.lang}/index.md`;
      // }
      // else{
      //   filePath = `${global.path}/application/${this.state.file}/${global.lang}/index.md`;
      // }
      var myPromise = new Promise(function (resolve, reject) {
        global.qtObjects.manual.appToPath(file, function (filepath) {
          filePath = filepath;
          resolve()
        })
      });
      myPromise.then(() => {
        global.readFile(filePath, data => {
          console.log("main init===>readfile finish...", filePath);
          let { html, hlist } = m2h(filePath, data, key);
          var newParentHash = 'h1';
          var newChildHash;
          var curHash;

          for (var i = 0; i < hlist.length; i++) {
            var h = hlist[i];
            if (h.text == parentText) {
              newParentHash = h.id;
            }
            else if (h.text == hashText) {
              newChildHash = h.id;
              break;
            }
          }

          if (newChildHash) {
            curHash = newChildHash;
          }
          else {
            curHash = newParentHash;
          }
          console.log('================>', curHash);

          this.init(decodeURIComponent(file), curHash, key);
          global.bIsReload = false;
        });
      })
    } else {
      console.log("main componentWillReceivePropss: " + file + " " + hash + "  this.file:" + this.state.file + " this.hash" + this.state.hash + " key:", key);
      //仅当页面文件发生改变时(文件改变或hash值发生改变),才刷新页面.
      if (decodeURIComponent(file) != this.state.file || ((file == this.state.file) && (hash != this.state.hash))) {
        this.init(decodeURIComponent(file), hash ? decodeURIComponent(hash) : null, key);
      }
    }
  }

  shouldComponentUpdate(nextProps, nextState) {
    console.log("main shouldComponentUpdate====");
    return true;
  }

  componentWillUpdate() {
    console.log("main componentWillUpdate..");
  }

  componentWillUnmount() {
    global.hash = '';
    global.isMouseClickNav = false;
    global.isMouseScrollArticle = false;
    global.isLinkClicked = false;
  }

  render() {
    console.log("main render....hash:", this.state.hash);
    console.log("main render....hList:", this.state.hlist);

    return (
      this.state.init && (
        <div id="main">
          <Nav
            hlist={this.state.hlist}
            hash={this.state.hash}
            setHash={this.setHash}
            onNavOver={(e) => this.handleNavOver(e)}
            onNavOut={(e) => this.handleNavOut(e)}
            onNavMove={(e) => this.handleNavMove(e)}
          />
          <Article
            file={this.props.match.params.file}
            hlist={this.state.hlist}
            html={this.state.html}
            hash={this.state.hash}
            setHash={this.setHash}
            setScroll={this.setScroll}
          />
          <div className="tooltip-wp"></div>
        </div>
      )
    );
  }
}
