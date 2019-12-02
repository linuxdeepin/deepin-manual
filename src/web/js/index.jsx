import React, { Component } from 'react';
import { Link } from 'react-router-dom';
import ReactDOM from 'react-dom';

import Scrollbar from './scrollbar.jsx';

class Item extends Component {
  constructor(props) {
    super(props);
    this.state = {
      name: '',
      title: '',
      logo: '',
      show: false
    };
    const path = `${global.path}/${this.props.appName}/${global.lang}/`;
    const file = path + `index.md`;
    global.readFile(file, data => {
      let [title, logo] = data
        .substr('# '.length, data.indexOf('\n'))
        .split('|');
      logo = `${path}${logo}`;

      this.setState({ title, logo, file, show: true });
    });
  }
  render() {
    return (
      this.state.show && (
        <div
          draggable="false"
          tabIndex="1"
          className="item"
          onClick={() => global.open(this.state.file)}
          onMouseEnter={e => e.target.focus()}
          onKeyPress={e => {
            if (e.key === 'Enter') {
              global.open(this.state.file);
            }
          }}
        >
          <img
            draggable="false"
            src={this.state.logo}
            alt={this.props.appName}
          />
          <br />
          <span lang={global.lang}>{this.state.title}</span>
        </div>
      )
    );
  }
}

export default class Index extends Component {
  constructor(props) {
    super(props);
    let sequence = [
      // '浏览器',
      'dde-file-manager',
      'deepin-app-store',
      // '输入法',
      // '邮件',
      'deepin-contacts',
      'deepin-screen-recorder',
      'deepin-image-viewer',
      'deepin-album',
      'deepin-music',
      'deepin-movie',
      'deepin-draw',
      'dde-calendar',
      'deepin-voice-note',
      'deepin-reader',
      'deepin-editor',
      'deepin-compressor',
      'dde-printer',
      'deepin-terminal',
      // '安全中心',
      // '下载器',
      'deepin-deb-installer',
      'deepin-font-manager',
      'deepin-calculator',
      'deepin-graphics-driver-manager',
      'deepin-devicemanager',
      'deepin-system-monitor',
      'deepin-boot-maker',
      // 'deepin-log-viewer',
      'deepin-repair-tools',
      'deepin-clone',
      'deepin-cloud-print',
      'deepin-cloud-scan',
      'deepin-voice-recorder',
      'deepin-picker',
      'deepin-remote-assistance',
      'deepin-presentation-assistant'
    ];
    this.state = {
      sequence,
      appList: []
    };
    global.qtObjects.manual.getSystemManualList(appList =>
      this.setState({ appList })
    );
  }
  shouldComponentUpdate(nextProps, nextState) {
    if (nextState.appList.toString() == this.state.appList.toString()) {
      return false;
    }
    return true;
  }
  componentDidUpdate() {
    ReactDOM.findDOMNode(this)
      .querySelector('#index')
      .focus();
  }
  render() {
    let sysSoft = ['dde'].filter(
      appName => this.state.appList.indexOf(appName) != -1
    );
    let appSoft = this.state.sequence.filter(
      appName => this.state.appList.indexOf(appName) != -1
    );
    let otherSoft = this.state.appList.filter(
      appName =>
        this.state.sequence.indexOf(appName) == -1 &&
        sysSoft.indexOf(appName) == -1
    );
    return (
      <Scrollbar>
        <div id="index" tabIndex="-1">
          <h2>{global.i18n['System']}</h2>
          {sysSoft.length > 0 && (
            <div id="forMargin">
                <div className="items">
                  {sysSoft.map(appName => <Item key={appName} appName={appName} />)}
                </div>
            </div>
          )}
          <h2>{global.i18n['Applications']}</h2>
          <div id="forMargin">
              <div className="items">
                {appSoft.map(appName => <Item key={appName} appName={appName} />)}
                {otherSoft.map(appName => <Item key={appName} appName={appName} />)}
                {Array.from(new Array(10), (val, index) => index).map(i => (
                  <a key={i} className="empty" />
                ))}
              </div>
           </div>
        </div>
      </Scrollbar>
    );
  }
}
