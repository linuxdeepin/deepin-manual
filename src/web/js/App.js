import React from 'react';
import PropTypes from 'prop-types';
import { render } from 'react-dom';
import { HashRouter as Router, Switch, Route, Link } from 'react-router-dom';

import Index from './index.jsx';
import Main from './main.jsx';
import Search from './search.jsx';
import sIndex from './searchIndex';

global.hash = ' ';
global.oldHash = ' ';
global.isMouseClickNav = false;
global.isMouseScrollArticle = false;

global.readFile = (fileName, callback) => {
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
      channel.objects.manual.getSystemManualDir(path => {
        global.path = path;
        this.setState({ init: true });
      });
      global.openWindow = global.qtObjects.manual.openExternalLink;
      global.qtObjects.titleBar.setBackwardButtonActive(true);
      global.qtObjects.titleBar.setForwardButtonActive(false);
      global.qtObjects.titleBar.backwardButtonClicked.connect(() => {
        this.setState({ historyGO: this.state.historyGO - 1 });
        this.context.router.history.goBack();
      });
      global.qtObjects.titleBar.forwardButtonClicked.connect(() => {
        this.setState({ historyGO: this.state.historyGO + 1 });
        this.context.router.history.goForward();
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
    });
  }
  themeChange(themeType) {
    console.log('主题切换', themeType);
    if(navigator.language.toString().indexOf('en_') != -1) {
      document.documentElement.style.setProperty(`--span-line-height`, '13px');
      document.documentElement.style.setProperty(`--span-font-size`, '12px');
    }
    else{
      document.documentElement.style.setProperty(`--span-line-height`, '18px');
      document.documentElement.style.setProperty(`--span-font-size`, '17px');
    }
    if("DarkType"==themeType){
    // console.log('DarkType');
    document.documentElement.style.setProperty(`--body-background-color`, '#252525');
    document.documentElement.style.setProperty(`--body-color-white2black`, '#000000');
    document.documentElement.style.setProperty(`--app-word-color`, '#C0C6D4');
    document.documentElement.style.setProperty(`--nav-background-color`, '#282828');
    document.documentElement.style.setProperty(`--nav-h2-word-color`, '#C0C6D4');
    document.documentElement.style.setProperty(`--nav-h3-word-color`, '#C0C0C0');
    document.documentElement.style.setProperty(`--nav-hash-word-color`, '#0059D2');
    document.documentElement.style.setProperty(`--article-read-word-color`, '#C0C6D4');
    document.documentElement.style.setProperty(`--article-read-h2-word-color`, '#0082FA');
    document.documentElement.style.setProperty(`--index-item-background-color`, 'rgba(255,255,255,0.05)');
    document.documentElement.style.setProperty(`--index-item-hover-color`, 'rgba(255,255,255,0.2)');
    document.documentElement.style.setProperty(`--index-item-span-word-color`, '#C0C6D4');
    document.documentElement.style.setProperty(`--search-noresult-word-color`, '#C0C6D4');
    document.documentElement.style.setProperty(`--search-button-word-color`, '#C0C6D4');
    document.documentElement.style.setProperty(`--search-items-word-color`, '#6D7C88');
    document.documentElement.style.setProperty(`--search-items-resultnum-word-color`, '#6D7C88');
    document.documentElement.style.setProperty(`--search-item-background-color`, 'rgba(255,255,255,0.05)');
    document.documentElement.style.setProperty(`--scrollbar-div-background-color`, '#1A1A1A');
    document.documentElement.style.setProperty(`--scrollbar-div-hover-background-color`, '#1A1A1A');
    document.documentElement.style.setProperty(`--scrollbar-div-select-background-color`, '#121212');
    document.documentElement.style.setProperty(`--index-h2-color`, 'rgba(255,255,255,0.05)');
    document.documentElement.style.setProperty(`--search-button-background-color-start`, '#484848');
    document.documentElement.style.setProperty(`--search-button-background-color-end`, '#414141');
    document.documentElement.style.setProperty(`--search-WikiSearch-color`, '#6D7C88');
    document.documentElement.style.setProperty(`--search-itemTitle-word-color`, '#C0C6D4');
document.documentElement.style.setProperty(`--search-context-word-color`, '#6D7C88');
    }else if("LightType"==themeType){
    // console.log('LightType');
    document.documentElement.style.setProperty(`--body-background-color`, '#F8F8F8');
    document.documentElement.style.setProperty(`--body-color-white2black`, '#FFFFFF');
    document.documentElement.style.setProperty(`--app-word-color`, '#414D68');
    document.documentElement.style.setProperty(`--nav-background-color`, '#FFFFFF');
    document.documentElement.style.setProperty(`--nav-h2-word-color`, '#001A2E');
    document.documentElement.style.setProperty(`--nav-h3-word-color`, '#414D68');
    document.documentElement.style.setProperty(`--nav-hash-word-color`, '#0081FF');
    document.documentElement.style.setProperty(`--article-read-word-color`, '#000000');
    document.documentElement.style.setProperty(`--article-read-h2-word-color`, '#2CA7F8');
    document.documentElement.style.setProperty(`--index-item-background-color`, '#FFFFFF');
    document.documentElement.style.setProperty(`--index-item-hover-color`, 'rgba(0,0,0,0.05)');
    document.documentElement.style.setProperty(`--index-item-span-word-color`, '#001A2E');
    document.documentElement.style.setProperty(`--search-noresult-word-color`, '#000000');
    document.documentElement.style.setProperty(`--search-button-word-color`, '#414D68');
    document.documentElement.style.setProperty(`--search-items-word-color`, '#000000');
    document.documentElement.style.setProperty(`--search-items-resultnum-word-color`, '#303030');
    document.documentElement.style.setProperty(`--search-item-background-color`, 'rgba(255,255,255,1)');
    document.documentElement.style.setProperty(`--scrollbar-div-background-color`, 'rgba(83,96,118,0.4)');
    document.documentElement.style.setProperty(`--scrollbar-div-hover-background-color`, 'rgba(83,96,118,0.5)');
    document.documentElement.style.setProperty(`--scrollbar-div-select-background-color`, 'rgba(83,96,118,0.6)');
    document.documentElement.style.setProperty(`--index-h2-color`, 'rgba(0, 0, 0, 0.1)');
    document.documentElement.style.setProperty(`--search-button-background-color-start`, '#E6E6E6');
    document.documentElement.style.setProperty(`--search-button-background-color-end`, '#E3E3E3');
    document.documentElement.style.setProperty(`--search-WikiSearch-color`, '#7a7a7a');
    document.documentElement.style.setProperty(`--search-itemTitle-word-color`, '#000000');
	document.documentElement.style.setProperty(`--search-context-word-color`, '#000000');
    }else{
      console.log('Null');
    }
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
    console.log(this.context);
    if (this.context.router.history.action == 'PUSH') {
      this.setState({ historyGO: this.context.router.history.length - 1 });
    }
  }
  componentDidMount() {
    global.index = () => {
      this.context.router.history.push('/');
    };
    global.open = (file, hash = '') => {
      file = encodeURIComponent(file);
      hash = encodeURIComponent(hash);
      global.hash = hash;
      global.oldHash = hash;
      let url = `/open/${file}/${hash}`;
      console.log(url);
      this.context.router.history.push(url);
    };
    global.openSearchPage = keyword => {
      this.setState({ searchResult: [] });
      this.context.router.history.push(
        '/search/' + encodeURIComponent(keyword)
      );
    };
    global.back = () => {
      this.context.router.history.goBack();
    };
    this.componentDidUpdate();
  }
  componentDidUpdate() {
    if (global.qtObjects) {
      global.qtObjects.titleBar.setForwardButtonActive(
        this.context.router.history.length - this.state.historyGO > 1
      );
      global.qtObjects.titleBar.setBackwardButtonActive(
        this.state.historyGO > 0
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
  <Router>
    <App />
  </Router>,
  document.getElementById('app')
);
