import React from 'react';
import PropTypes from 'prop-types';
import { render } from 'react-dom';
import { Router as Router, Switch, Route, Link } from 'react-router-dom';
import { createMemoryHistory } from 'history'

import Index from './index.jsx';
import Main from './main.jsx';
import Search from './search.jsx';
import sIndex from './searchIndex';

global.hash = ' ';
global.oldHash = ' ';
global.linktitle = '';
global.isMouseClickNav = false;
global.isMouseScrollArticle = false;

global.isLinkClicked = false;

global.lastUrlBeforeSearch = '/';
global.lastHistoryIndex = 0;
global.lastAction = 'PUSH';

global.readFile = (fileName, callback) => {
  console.log("global.readFile...");
  let xhr = new XMLHttpRequest();
  xhr.open('GET', fileName);
  xhr.onload = () => {
    if (xhr.responseText != '') {
      callback(xhr.responseText);
    }
  };
  xhr.send();
};

class App extends React.Component {
  constructor(props, context) {
    super(props, context);

    this.state = {
      init: false,
      searchResult: [],
      mismatch: false,
      historyGO: 0
    };
    new QWebChannel(qt.webChannelTransport, this.initQt.bind(this));
  }
  initQt(channel) {
    console.log("channel initqt.....");
    channel.objects.i18n.getSentences(i18n => {
      channel.objects.i18n.getLocale(lang => {
        if (lang === 'en_US' || lang === 'zh_CN') {
          global.lang = lang;
        } else {
          global.lang = 'en_US';
        }
      });
      global.i18n = i18n;
      global.qtObjects = channel.objects;
      console.log("finsh global.qtObjects = channel.objects...");
      channel.objects.manual.getSystemManualDir(path => {
        global.path = path;
        this.setState({ init: true });
      });
      global.openWindow = global.qtObjects.manual.openExternalLink;
      global.qtObjects.titleBar.setBackwardButtonActive(false);
      global.qtObjects.titleBar.setForwardButtonActive(false);
      global.qtObjects.titleBar.backwardButtonClicked.connect(() => {
        console.log("----------backwardButtonClicked----------");
        this.setState({ historyGO: this.state.historyGO - 1 });
        console.log("==========backwardButtonClicked=========>");
        this.context.router.history.goBack();
        console.log("back history location: "+this.context.router.history.location.pathname);
      });
      global.qtObjects.titleBar.forwardButtonClicked.connect(() => {
        console.log("----------forwardButtonClicked----------");
        this.setState({ historyGO: this.state.historyGO + 1 });
        console.log("==========forwardButtonClicked=========>");
        this.context.router.history.goForward();
        console.log("forward history location: "+this.context.router.history.location.pathname);
      });
      global.qtObjects.search.mismatch.connect(() =>
        this.setState({ mismatch: true })
      );
      global.qtObjects.search.onContentResult.connect(
        this.onContentResult.bind(this)
      );
      global.qtObjects.theme.getTheme(themeType => 
        this.themeChange(themeType));
      global.qtObjects.theme.themeChange.connect(
        this.themeChange.bind(this)
      );
      global.qtObjects.settings.fontChangeRequested.connect((fontFamily, fontSize) => {
        console.log("fontChangeRequested: fontFamily:"+fontFamily+",fontSize:"+fontSize);
        console.log("fontSize/13.0:"+(fontSize));
        const HTMLGlobal = document.querySelector('html');
        HTMLGlobal.style.fontFamily = fontFamily;    
        HTMLGlobal.style.fontSize = fontSize;   //设置rem标准   设计图上默认是在14px字体上设计,所以默认1rem = 14px.
        if ( fontSize >= 18)
        {
          document.documentElement.style.setProperty(`--index-item-size`, '170px');
          document.documentElement.style.setProperty(`--index-span-width`, '140px');
        }
        else{
          document.documentElement.style.setProperty(`--index-item-size`, '160px');
          document.documentElement.style.setProperty(`--index-span-width`, '130px');
        }
      });
    });
  }
  themeChange(themeType) {
    global.setTheme(themeType);
  }
  onContentResult(appName, titleList, idList, contentList) {
    // console.log('搜索结果', appName, titleList, idList, contentList);
    let { searchResult } = this.state;
    searchResult.push({
      file: `${global.path}/${appName}/${global.lang}/index.md`,
      idList,
      titleList,
      contentList
    });
    this.setState({ searchResult, mismatch: false });
  }
  getChildContext() {
    let { searchResult, mismatch } = this.state;
    return { searchResult, mismatch };
  }
  componentWillReceiveProps(nextProps) {
    console.log("componentWillReceiveProps", this.context.router.history);
    if (this.context.router.history.action == 'PUSH') {
      let entriesLen = this.context.router.history.entries.length;
      //console.log("entriesLen" + entriesLen);
      if (entriesLen > 1) {
        let entry = this.context.router.history.entries[entriesLen-1];
        if (entry.pathname.toString().indexOf("/search/") != -1) {
          //console.log(entry.pathname);
          this.setState({ historyGO: entriesLen - 1 });
          return;
        }
      }
      this.setState({ historyGO: entriesLen - 1 });
    }
  }
  componentDidMount() {
    global.index = () => {
      // this.context.router.history.push('/');
    };
    global.backHome = () => {
      console.log("global.backHome()" + this.context.router.history.entries.length);
      console.log("global.backHome()" + this.state.historyGO);
      let goNum = this.state.historyGO;
      this.setState({ historyGO: 0 });
      console.log("global.backHome()" + goNum);

      if (this.context.router.history.canGo(-1 * goNum)) {
        this.context.router.history.go(-1 * goNum);
        // this.context.router.history.go(0);
      }
    };

    //打开某一个文件,并定位到具体hash
    global.open = (file, hash = '') => {
      console.log("global.open()....file:"+ file + " hash:" + hash);
      //h0默认为应用名称，内容为空，所以当打开h0，将其变为h1概述的位置。
      if (hash == 'h0')
      {
        hash = 'h1'
      }
      file = encodeURIComponent(file);
      hash = encodeURIComponent(hash);
      global.hash = hash;
      global.oldHash = hash;
      let url = `/open/${file}/${hash}`;
      console.log("global.open: " +url);
      this.context.router.history.push(url);


      console.log("router.history--->", this.context.router.history);

      //延时通知qt对象, 以避免通过F1开启帮助时,会直接调用此JS方法,但未完成channnel中qt对象和js中global对象的绑定.
      setTimeout(function(){
        global.qtObjects.manual.setApplicationState(file);
      },200);
    };

    global.linkTitle = (title) => {
      console.log("===============");
      console.log(title);
      global.linktitle = title;
    };

    global.setHashWordColor = (strRgb) => {
      document.documentElement.style.setProperty(`--nav-hash-word-color`, strRgb);     //btnlist 改这行
    }

    global.setWordFontfamily = (strFontFamily) => {
      document.documentElement.style.setProperty(`--nav-world-font-family`, strFontFamily);     //btnlist 改这行
    }


    global.setTheme = (themeType) => {
      console.log('主题切换', themeType);
      if(navigator.language.toString().indexOf('en_') != -1) {
        document.documentElement.style.setProperty(`--span-line-height`, '1.0rem');
        document.documentElement.style.setProperty(`--span-font-size`, '0.9rem');
      }
      else{
        document.documentElement.style.setProperty(`--span-line-height`, '1.4rem');
        document.documentElement.style.setProperty(`--span-font-size`, '1.03rem');
      }
      if("DarkType"==themeType){
      console.log('DarkType');
      document.documentElement.style.setProperty(`--nav-hover-color`, 'rgba(255,255,255,0.1)');
      document.documentElement.style.setProperty(`--body-background-color`, '#252525');
      document.documentElement.style.setProperty(`--body-color-white2black`, '#000000');
      document.documentElement.style.setProperty(`--app-word-color`, '#C0C6D4');
      document.documentElement.style.setProperty(`--nav-background-color`, '#282828');
      document.documentElement.style.setProperty(`--nav-h2-word-color`, '#C0C6D4');
      document.documentElement.style.setProperty(`--nav-h3-word-color`, '#C0C0C0');
      //document.documentElement.style.setProperty(`--nav-hash-word-color`, '#0059D2');     //btnlist 改这行
      document.documentElement.style.setProperty(`--article-read-word-color`, '#C0C6D4');
      document.documentElement.style.setProperty(`--article-read-h2-word-color`, '#0082FA');
      document.documentElement.style.setProperty(`--article-table-text-color`, '#6D7C88');
      document.documentElement.style.setProperty(`--article-table-border-color`, 'rgba(96, 96, 96, 0.5)');
      document.documentElement.style.setProperty(`--article-table-cell-border-color`, 'rgba(96, 96, 96, 0.1)');
      document.documentElement.style.setProperty(`--index-item-background-color`, 'rgba(255,255,255,0.05)');
      document.documentElement.style.setProperty(`--index-item-hover-color`, 'rgba(255,255,255,0.2)');
      document.documentElement.style.setProperty(`--index-item-span-word-color`, '#C0C6D4');
      document.documentElement.style.setProperty(`--search-noresult-word-color`, '#C0C6D4');
      document.documentElement.style.setProperty(`--search-button-word-color`, '#C0C6D4');
      document.documentElement.style.setProperty(`--search-button-hover-word-color`, '#FFFFFF');
      document.documentElement.style.setProperty(`--search-items-word-color`, '#6D7C88');
      document.documentElement.style.setProperty(`--search-items-resultnum-word-color`, '#6D7C88');
      document.documentElement.style.setProperty(`--search-item-background-color`, 'rgba(255,255,255,0.05)');
      document.documentElement.style.setProperty(`--scrollbar-div-background-color`, '#444444');
      document.documentElement.style.setProperty(`--scrollbar-div-hover-background-color`, '#3D3D3D');
      document.documentElement.style.setProperty(`--scrollbar-div-select-background-color`, '#303B69');
      document.documentElement.style.setProperty(`--index-h2-color`, 'rgba(255,255,255,0.05)');
      document.documentElement.style.setProperty(`--search-button-background-color-start`, '#484848');
      document.documentElement.style.setProperty(`--search-button-background-color-end`, '#414141');
      document.documentElement.style.setProperty(`--search-button-hover-color-start`, '#676767');
      document.documentElement.style.setProperty(`--search-button-hover-color-end`, '#606060');
      document.documentElement.style.setProperty(`--search-WikiSearch-color`, '#6D7C88');
      document.documentElement.style.setProperty(`--search-itemTitle-word-color`, '#C0C6D4');
      document.documentElement.style.setProperty(`--search-context-word-color`, '#6D7C88');
      }else if("LightType"==themeType){
      console.log('LightType');
      document.documentElement.style.setProperty(`--nav-hover-color`, 'rgba(0,0,0,0.1)');
      document.documentElement.style.setProperty(`--body-background-color`, '#F8F8F8');
      document.documentElement.style.setProperty(`--body-color-white2black`, '#FFFFFF');
      document.documentElement.style.setProperty(`--app-word-color`, '#414D68');
      document.documentElement.style.setProperty(`--nav-background-color`, '#FFFFFF');
      document.documentElement.style.setProperty(`--nav-h2-word-color`, '#001A2E');
      document.documentElement.style.setProperty(`--nav-h3-word-color`, '#001A2E');
      // document.documentElement.style.setProperty(`--nav-hash-word-color`, '#ca0c16');   //btn list 改这一行
      document.documentElement.style.setProperty(`--article-read-word-color`, '#000000');
      document.documentElement.style.setProperty(`--article-read-h2-word-color`, '#2CA7F8');
      document.documentElement.style.setProperty(`--article-table-text-color`, '#606060');
      document.documentElement.style.setProperty(`--article-table-border-color`, 'rgba(0, 0, 0, 0.1)');
      document.documentElement.style.setProperty(`--article-table-cell-border-color`, 'rgba(0, 0, 0, 0.05)');
      document.documentElement.style.setProperty(`--index-item-background-color`, '#FFFFFF');
      document.documentElement.style.setProperty(`--index-item-hover-color`, 'rgba(0,0,0,0.05)');
      document.documentElement.style.setProperty(`--index-item-span-word-color`, '#414D68');
      document.documentElement.style.setProperty(`--search-noresult-word-color`, '#000000');
      document.documentElement.style.setProperty(`--search-button-word-color`, '#414D68');
      document.documentElement.style.setProperty(`--search-button-hover-word-color`, '#001B2E');
      document.documentElement.style.setProperty(`--search-items-word-color`, '#000000');
      document.documentElement.style.setProperty(`--search-items-resultnum-word-color`, '#8AA1B4'); 
      document.documentElement.style.setProperty(`--search-item-background-color`, 'rgba(255,255,255,1)');
      document.documentElement.style.setProperty(`--scrollbar-div-background-color`, 'rgba(83,96,118,0.4)');
      document.documentElement.style.setProperty(`--scrollbar-div-hover-background-color`, 'rgba(83,96,118,0.5)');
      document.documentElement.style.setProperty(`--scrollbar-div-select-background-color`, 'rgba(83,96,118,0.6)');
      document.documentElement.style.setProperty(`--index-h2-color`, 'rgba(0, 0, 0, 0.1)');
      document.documentElement.style.setProperty(`--search-button-background-color-start`, '#E6E6E6');
      document.documentElement.style.setProperty(`--search-button-background-color-end`, '#E3E3E3');
      document.documentElement.style.setProperty(`--search-button-hover-color-start`, '#CACACA');
      document.documentElement.style.setProperty(`--search-button-hover-color-end`, '#C6C6C6');
      document.documentElement.style.setProperty(`--search-WikiSearch-color`, '#7a7a7a');
      document.documentElement.style.setProperty(`--search-itemTitle-word-color`, '#000000');
      document.documentElement.style.setProperty(`--search-context-word-color`, '#000000');
      }else{
        console.log('Null');
      }
    }

    let Base64 = {
      encode(str) {
          // first we use encodeURIComponent to get percent-encoded UTF-8,
          // then we convert the percent encodings into raw bytes which
          // can be fed into btoa.
          return btoa(encodeURIComponent(str).replace(/%([0-9A-F]{2})/g,
              function toSolidBytes(match, p1) {
                  return String.fromCharCode('0x' + p1);
              }));
      },
      decode(str) {
          // Going backwards: from bytestream, to percent-encoding, to original string.
          return decodeURIComponent(atob(str).split('').map(function (c) {
              return '%' + ('00' + c.charCodeAt(0).toString(16)).slice(-2);
          }).join(''));
      }
    };

    global.openSearchPage = keyword => {
      let decodeKeyword = Base64.decode(keyword);
      console.log("decodeKeyword", decodeKeyword);
      console.log("openSearchPage", this.context.router.history);
      console.log(`lastUrl:${global.lastUrlBeforeSearch}, lastHistoryIndex: ${global.lastHistoryIndex}`);

      let entriesLen = this.context.router.history.entries.length;
      if ('POP' == global.lastAction && lastHistoryIndex > 0 && lastHistoryIndex < entriesLen-1) {
        this.context.router.history.entries.length = lastHistoryIndex;
        this.context.router.history.length = lastHistoryIndex;
        this.context.router.history.index = lastHistoryIndex-1;

        this.setState({ searchResult: [] });
        this.context.router.history.push(
          '/search/' + encodeURIComponent(decodeKeyword)
        );

        return;
      }

      entriesLen = this.context.router.history.entries.length;
      if (entriesLen > 1) {
        let entry = this.context.router.history.entries[entriesLen-1];
        let entryIndex = entry.pathname.toString().indexOf("/search/");
        if (entryIndex != -1) {
          this.context.router.history.entries.length = entriesLen-1;
          this.context.router.history.length = entriesLen-1;
          this.context.router.history.index = this.context.router.history.entries.length-1;
        }
      }

      this.setState({ searchResult: [] });
      this.context.router.history.push(
        '/search/' + encodeURIComponent(decodeKeyword)
      );
    };

    this.context.router.history.listen((location, action) => {
      //console.log(`The current URL is ${location.pathname}${location.search}${location.hash}` );
      //console.log(`The last navigation action was ${action}`);
      //console.log("index:" + this.context.router.history.index);
      global.lastUrlBeforeSearch = location.pathname;
      global.lastHistoryIndex = this.context.router.history.index;
      global.lastAction = action;
    });

    global.back = () => {
      console.log("global.back()");
      this.context.router.history.goBack();
    };
    this.componentDidUpdate();
  }
  componentDidUpdate() {
    if (global.qtObjects) {
      global.qtObjects.titleBar.setBackwardButtonActive(
        this.state.historyGO > 0
      );
      global.qtObjects.titleBar.setForwardButtonActive(
        this.context.router.history.length - this.state.historyGO > 1
      );
    }
  }
  render() {
    return (
      <div>
        {this.state.init && (
          <Switch>
            <Route exact path="/" component={Index} />
            <Route path="/index" component={Index} />
            <Route path="/open/:file/:hash?" component={Main} />
            <Route path="/search/:keyword" component={Search} />
          </Switch>
        )}
      </div>
    );
  }
}
App.contextTypes = {
  router: PropTypes.object
};
App.childContextTypes = {
  searchResult: PropTypes.array,
  mismatch: PropTypes.bool
};

render(
  <Router history={createMemoryHistory('/')}>
    <App />
  </Router>,
  document.getElementById('app')
);
