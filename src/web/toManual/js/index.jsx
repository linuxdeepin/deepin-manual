// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

import React, { Component } from 'react';
import { Link } from 'react-router-dom';
import ReactDOM from 'react-dom';

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
      desktopname:'',
    };
    console.log('main item constructor...');
    this.init();
  }

  init()
  {
    var filePath;
    var appName = this.props.appName;
    var myPromise = new Promise(function(resolve, reject){
      global.qtObjects.manual.appToPath(appName, function (filepath) {
        filePath = filepath;
        //error： 目标文件不存在
        if (filePath == 'error')
        {
          return;
        }
        resolve()
      })
    });
   
    myPromise.then(()=>{
      global.readFile(filePath, data => {

        let [title, desktopname] = data
          .substr('# '.length, data.indexOf('\n'))
          .split('|');
          
     global.qtObjects.manual.getAppIconPath(desktopname,(logopath) =>{
       //按约定会在图标主题放置dde图标，但为保险起见如果未获取到则取common中的
       if(logopath==''&&desktopname=="dde"){
        logopath=filePath.substr(0,filePath.lastIndexOf('/')+1)+'../common/dde.svg';       
       }
      this.setState({ logo:logopath});     
      });

      global.qtObjects.manual.getLocalAppName(desktopname,(appname) =>{
        this.setState({ title:appname});     
        });
    
     this.setState({desktopname, file:this.file, show: true}); 
      });
    });
    global.show=true;
    //2021.3.4产品决定取消图标动态切换，使用default图标主题，暂时注释
  //   global.qtObjects.manual.iconThemeChanged.connect(
  //     this.iconThemeChange.bind(this)
  //  );
    
  }

  iconThemeChange(themeType) {
     global.qtObjects.manual.getAppIconPath(this.state.desktopname,(logopath) =>{      
      if( global.show===true)   {      
        this.setState({ logo:logopath});     
      }      
      });
  }
 
  componentWillReceiveProps(nextProps) {
    console.log("index item componentWillReceivePropss........");   
    this.init();
  }

  componentWillUnmount(){
    global.show=false;  
  }

  render() {
    var contentSpan = null;
    if (this.props.isOpened)
    {
      contentSpan = (<span className="content" lang={global.lang}>{this.state.title}</span>)
    }
    else
    {
      contentSpan = (<span className="content" lang={global.lang}><span className="tag"></span>{this.state.title}</span>)
    }

    return (
      this.state.show && (
        <div
          draggable="false"
          tabIndex="1"
          className="item"
          onClick={() => global.open(this.props.appName)}
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

export default class Index extends Component {
  constructor(props) {
    super(props);
    this.state = {
      appList: [],
      openedAppList:[]
    };

    global.qtObjects.manual.getSystemManualList(appList =>{
      console.log("======>applist==+>",appList);
      this.setState({ appList });
    });

    global.qtObjects.manual.getUsedAppList(openedAppList =>
      this.setState({openedAppList})
    );
  }

  bIsBeOpen(app){
    if (this.state.openedAppList.indexOf(app) != -1)
    {
      return true;
    }
    return false;
  }

  shouldComponentUpdate(nextProps, nextState) {
    console.log("index shouldcomponentupdate");
    if (global.bIsReload)
    {
      global.qtObjects.manual.getSystemManualList(appList =>{
        this.setState({ appList })
      });
  
      global.qtObjects.manual.getUsedAppList(openedAppList =>
        this.setState({openedAppList})
      );
      global.bIsReload = false;
    }
    // else if (nextState.appList.toString() == this.state.appList.toString() 
    //           && nextState.openedAppList.toString() == this.state.openedAppList.toString()) 
    // {
    //   console.log("index no update");
    //   return false;
    // }
    return true;
  }
  componentDidUpdate() {
    ReactDOM.findDOMNode(this)
      .querySelector('#index')
      .focus();
  }
  render() {
    console.log('index render...');
    let sysSoft = ['dde'].filter(
      appName => this.state.appList.indexOf(appName) != -1
    );
    let appSoft = JSON.parse(JSON.stringify(this.state.appList))//使用数据副本
    var index = appSoft.indexOf("dde");
    if (index !== -1)
    {
      appSoft.splice(index, 1); 
    }

    return (
      <Scrollbar>
        <div id="index" tabIndex="-1">
          <h2>{global.i18n['System']}</h2>
          {sysSoft.length > 0 && (
            <div id="forMargin">
                <div className="items">
                  {sysSoft.map(appName => <Item key={appName} appName={appName} isOpened={this.bIsBeOpen(appName)} type={"system"}/>)}
                </div>
            </div>
          )}
          <h2>{global.i18n['Applications']}</h2>
          <div id="forMargin">
              <div className="items">
                {appSoft.map(appName => <Item key={appName} appName={appName} isOpened={this.bIsBeOpen(appName)} type={"application"}/>)}
                {/* {otherSoft.map(appName => <Item key={appName} appName={appName} isOpened={this.bIsBeOpen(appName)}/>)}
                {Array.from(new Array(10), (val, index) => index).map(i => (
                  <a key={i} className="empty" />
                ))} */}
              </div>
           </div>
        </div>
      </Scrollbar>
    );
  }
}
