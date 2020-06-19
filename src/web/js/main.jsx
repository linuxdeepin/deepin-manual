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
      bTest:true
    };
    let { file, hash } = this.props.match.params;
    this.init(decodeURIComponent(file), hash ? decodeURIComponent(hash) : null);
    var showFloatTimer=null;
  }
  init(file, hash) {
    console.log("main init"+file);
    if (file.indexOf('/') == -1) {
      file = `${global.path}/${file}/${global.lang}/index.md`;
    }
    global.readFile(file, data => {
      let { html, hlist } = m2h(file, data);
      this.setState({
        html,
        hlist,
        init: true,
        hash: hash ? hash : hlist[0].id
      });

      console.log("main init linktitle"+global.linktitle);
    
      if (global.linktitle != '')
      {
        var nHash = '';
        for (let i = 0; i < this.state.hlist.length; i++)
        {
          var element = this.state.hlist[i];

          if (element.text == global.linktitle)
          {
            console.log("----------"+element.text);
            nHash = element.id;
          }
        }
        this.setScrollTitle(nHash);
        global.linktitle = '';
      }
    });
  }

  setHash(hash) {
    console.log("main setHash:" + hash);
    if (global.isLinkClicked) {
      console.log("main --setHash");
      global.hash = hash;
      global.oldHash = hash;
      global.isLinkClicked = false;
    }
    console.log("main*********setHash");
    this.setState({ hash });
  }

  setScrollTitle(hash){
    // console.log("main title setHash:" + hash);
    setTimeout(() => {
      global.hash = hash;
      global.oldHash = hash;
      global.isMouseClickNav = true;
      global.isMouseScrollArticle = false;
      this.setState({ hash });
    },800);
  }

  setScroll(hash) {
    console.log("main setScroll:" + hash);
    global.hash = hash;
    global.oldHash = hash;
    this.setState({ hash });
  }

  //处理Nav类的Over Out Move事件,自定义Title框
  handleNavOver(e){
    var value =  e.currentTarget.innerHTML;
    clearTimeout(this.showFloatTimer);
    this.showFloatTimer=setTimeout(function(){
        $('.tooltip-wp').attr('data-title', value); //动态设置data-title属性
        $('.tooltip-wp').fadeIn(200);//浮动框淡出
    },300);
  }

  handleNavOut(e){
    clearTimeout(this.showFloatTimer);
    $('.tooltip-wp').hide();
  }

  handleNavMove(e){
    var xPage = e.pageX;
    var yPage = e.pageY + 20;
    setTimeout(function(){
        $('.tooltip-wp').css({
            'top' : yPage + 'px',
            'left': xPage+ 'px'
        });
    },150);
  }

  componentWillReceiveProps(nextProps) {
    console.log("main componentWillReceivePropss");
    let { file, hash } = nextProps.match.params;
    this.init(decodeURIComponent(file), hash ? decodeURIComponent(hash) : null);
  }

  componentWillUnmount(){
    global.hash = ' ';
    global.oldHash = ' ';
    global.linktitle = '';
    global.isMouseClickNav = false;
    global.isMouseScrollArticle = false;
    global.isLinkClicked = false;
  }

  render() {
    return (
      this.state.init && (
        <div id="main">
          <Nav
            hlist={this.state.hlist}
            hash={this.state.hash}
            setHash={this.setHash.bind(this)}
            onNavOver={(e)=>this.handleNavOver(e)}
            onNavOut={(e)=>this.handleNavOut(e)}
            onNavMove={(e)=>this.handleNavMove(e)}
          />
          <Article
            file={this.props.match.params.file}
            hlist={this.state.hlist}
            html={this.state.html}
            hash={this.state.hash}
            setHash={this.setHash.bind(this)}
            setScroll={this.setScroll.bind(this)}
          />
          <div className="support-div"><img className="support" src="./pic.svg"></img></div>
          <div className="tooltip-wp"></div>
        </div>
      )
    );
  }
}
