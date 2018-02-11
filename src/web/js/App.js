import React from 'react';
import PropTypes from 'prop-types';
import { render } from 'react-dom';
import { HashRouter as Router, Switch, Route, Link } from 'react-router-dom';

import Index from './index.jsx';
import Main from './main.jsx';
import Search from './search.jsx';
import sIndex from './searchIndex';

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
    });
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
