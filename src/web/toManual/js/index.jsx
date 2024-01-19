// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

import React, { Component, useState, useEffect } from 'react';
import { Link } from 'react-router-dom';
import ReactDOM from 'react-dom';
import $ from 'jquery';
import { useRef } from 'react';

import Scrollbar from './scrollbar.jsx';
import { type } from 'os';
global.show = false;
class Item extends Component {
  constructor(props) {
    super(props);
    this.state = {
      name: '',
      title: '',
      logo: '',
      show: false,
      desktopname: '',
    };
    console.log('main item constructor...');
    this.init();
  }

  init() {
    var filePath;
    var appName = this.props.appName;
    var myPromise = new Promise(function (resolve, reject) {
      global.qtObjects.manual.appToPath(appName, function (filepath) {
        filePath = filepath;
        //error： 目标文件不存在
        if (filePath == 'error') {
          return;
        }
        resolve()
      })
    });

    myPromise.then(() => {
      global.readFile(filePath, data => {

        let [title, desktopname] = data
          .substr('# '.length, data.indexOf('\n'))
          .split('|');

        global.qtObjects.manual.getAppIconPath(desktopname, (logopath) => {
          //按约定会在图标主题放置dde图标，但为保险起见如果未获取到则取common中的
          if (logopath == '' && desktopname == "dde") {
            logopath = filePath.substr(0, filePath.lastIndexOf('/') + 1) + '../common/dde.svg';
          }
          // console.log("logopath:", logopath);
          this.setState({ logo: logopath });
        });

        global.qtObjects.manual.getLocalAppName(desktopname, (appname) => {
          this.setState({ title: appname });
        });

        this.setState({ desktopname, file: this.file, show: true });
      });
    });
    global.show = true;
    //2021.3.4产品决定取消图标动态切换，使用default图标主题，暂时注释
    //   global.qtObjects.manual.iconThemeChanged.connect(
    //     this.iconThemeChange.bind(this)
    //  );

  }

  iconThemeChange(themeType) {
    global.qtObjects.manual.getAppIconPath(this.state.desktopname, (logopath) => {
      if (global.show === true) {
        this.setState({ logo: logopath });
      }
    });
  }

  componentWillReceiveProps(nextProps) {
    console.log("index item componentWillReceivePropss........");
    this.init();
  }

  componentWillUnmount() {
    global.show = false;
  }

  render() {
    var contentSpan = null;
    if (this.props.isOpened) {
      contentSpan = (<span className="content" lang={global.lang}>{this.state.title}</span>)
    } else {
      contentSpan = (<span className="content" lang={global.lang}><span className="tag"></span>{this.state.title}</span>)
    }

    return (
      this.state.show && (
        <div
          draggable="false"
          tabIndex="1"
          className="item"
          onClick={() => {
            global.open(this.props.appName)
          }}
          onKeyPress={e => {
            if (e.key === 'Enter') {
              global.open(this.props.appName);
            }
          }}
        >

          <img
            draggable="false"
            src={this.state.logo}
            alt={this.props.appName}
          />
          <br />
          {contentSpan}
        </div>
      )
    );
  }
}

class QuickStartItem extends Component {
  constructor(props) {
    super(props);
    this.state = {
      name: '',
      title: '',
      background: './resource/banner/bg1.png',
      icon: './resource/banner/app.png',
    };

    global.qtObjects.manual.getLocalAppName(this.props.appName, (_title) => {
      this.setState({ title: _title });
    });
  }

  render() {
    const backgroundPath = "./resource/banner/" + this.props.appName + "_bg.png";
    const iconPath = "./resource/banner/" + this.props.appName + ".png";

    return (
      <div className="quickitem" style={{
        backgroundImage: `url(${backgroundPath})`,
        backgroundRepeat: 'no-repeat',
        backgroundPosition: 'center', 
        backgroundSize: 'cover',
      }}
        onClick={() => {
          global.open(this.props.appName);
        }}
        onKeyPress={e => {
          if (e.key === 'Enter') {
            global.open(this.props.appName);
          }
        }}
      >
        <img id="icon" src={iconPath} />
        <p id="title">{this.state.title}</p>
        <p id="click">{global.i18n['ClicktoView']}
          <img src="./resource/banner/arrow.png" width="10" height="10" />
        </p>
      </div>
    )
  }
}

// 带toolip的文本框
const TitleWithTip = ({ text }) => {
  const [isHovered, setIsHovered] = useState(false);
  const [tooltipText, setTooltipText] = useState(text);
  const [tooltipPosition, setTooltipPosition] = useState({ left: 0, top: 0 });
  const titleRef = useRef(null);

  useEffect(() => {
    setTooltipText(text);
    setTooltipPosition({ left: 0, top: 0 });
  }, [text]);

  useEffect(() => {
    if (isHovered) {
      setTooltipText(text);
    }
  }, [isHovered, text]);

  useEffect(() => {
    if (!isHovered) {
      setTooltipText('');
    }
  }, [isHovered]);

  // 当鼠标移动时，更新提示框的位置为鼠标位置  
  const handleMouseMove = (event) => {
    const rect = titleRef.current.parentNode.getBoundingClientRect();
    const x = event.pageX - titleRef.current.parentNode.offsetLeft - titleRef.current.parentNode.offsetWidth / 2;
    const y = event.pageY - rect.y;
    // console.log("x:", x, "   y:", y);
    setTooltipPosition({ left: x + 10, top: y });
  };

  return (
    <div className="title-div"
      ref={titleRef}
      onMouseEnter={() => setIsHovered(true)}
      onMouseLeave={() => setIsHovered(false)}
      onMouseMove={handleMouseMove}
      style={{ userSelect: 'none' }}
    >
      {text}
      {isHovered && <div className={`title-wrapper ${isHovered ? 'show' : ''}`}
        style={{ left: tooltipPosition.left, top: tooltipPosition.top }}
      >
        {tooltipText}
      </div>}
    </div>
  );
};

class VideoGuideItem extends Component {
  constructor(props) {
    super(props);
    this.state = {
      title: this.props.title,
      imgpath: this.props.cover,
      videourl: this.props.url,
    };
  }

  render() {
    return (
      <div className="videoitem"
        onClick={() => {
          global.qtObjects.manual.openVideo(this.props.url);
        }}
        onKeyPress={e => {
          if (e.key === 'Enter') {
            global.qtObjects.manual.openVideo(this.props.url);
          }
        }}
      >
        <img id="image" src={this.state.imgpath} />

        <TitleWithTip text={this.state.title}></TitleWithTip>
      </div>
    )
  }
}

export default class Index extends Component {
  constructor(props) {
    super(props);
    this.state = {
      appList: [],
      openedAppList: [],
      videoList: []
    };

    global.qtObjects.manual.getSystemManualList(appList => {
      console.log("======>applist===>", appList);
      this.setState({ appList });
    });

    global.qtObjects.manual.getUsedAppList(openedAppList =>
      this.setState({ openedAppList })
    );

    global.qtObjects.manual.getVideoGuideInfo(_videoList => {
      console.log("======>videoList===>", _videoList);
      this.setState({ videoList: _videoList });
    });
  }

  bIsBeOpen(app) {
    if (this.state.openedAppList.indexOf(app) != -1) {
      return true;
    }
    return false;
  }

  shouldComponentUpdate(nextProps, nextState) {
    console.log("index shouldcomponentupdate");
    if (global.bIsReload) {
      global.qtObjects.manual.getSystemManualList(appList => {
        this.setState({ appList })
      });

      global.qtObjects.manual.getUsedAppList(openedAppList =>
        this.setState({ openedAppList })
      );
      global.bIsReload = false;
    }
    return true;
  }
  componentDidUpdate() {
    ReactDOM.findDOMNode(this)
      .querySelector('#index')
      .focus();
  }

  render() {
    console.log('index render...');

    let videoSoft = JSON.parse(JSON.stringify(this.state.videoList));
    let sysSoft = ['dde'].filter(
      appName => this.state.appList.indexOf(appName) != -1
    );

    let appSoft = JSON.parse(JSON.stringify(this.state.appList))//使用数据副本
    let startSoft = [];
    var index = appSoft.indexOf("dde");
    if (index !== -1) {
      appSoft.splice(index, 1);
    }
    index = appSoft.indexOf("learn-basic-operations");
    if (index !== -1) {
      startSoft.push(appSoft[index]);
      appSoft.splice(index, 1);
    }
    index = appSoft.indexOf("common-application-libraries");
    if (index !== -1) {
      startSoft.push(appSoft[index]);
      appSoft.splice(index, 1);
    }

    return (
      <div>
        <Scrollbar>
          <div id="index" tabIndex="-1">
            {startSoft.length == 2 && (
              <div>
                <h2>{global.i18n['QuickStart']}</h2>

                <div className="items">
                  <QuickStartItem appName="learn-basic-operations" />
                  <QuickStartItem appName="common-application-libraries" />
                </div>

              </div>
            )}

            {videoSoft.length > 0 && (
              <div>
                <h2>{global.i18n['VideoGuide']}
                  <span id="righttext" onClick={() => {
                    global.qtObjects.manual.openVideo('');
                  }}>
                    {global.i18n['ViewAll']}
                  </span>
                </h2>

                <div className="items">
                  <div className="tooltip-wp"></div>
                  {videoSoft.map((item, index) => (
                    <VideoGuideItem key={index} index={index} title={item.name} cover={item.cover} url={item.url} />
                  ))}
                </div>
              </div>
            )}

            <h2>{global.i18n['System']}</h2>
            {sysSoft.length > 0 && (
              <div className="appitems">
                {sysSoft.map(appName => <Item key={appName} appName={appName} isOpened={this.bIsBeOpen(appName)} type={"system"} />)}
              </div>
            )}

            <h2>{global.i18n['Applications']}</h2>
            <div className="appitems">
              {appSoft.map(appName => <Item key={appName} appName={appName} isOpened={this.bIsBeOpen(appName)} type={"application"} />)}
            </div>
          </div>
        </Scrollbar>
      </div>
    );
  }
}
