import React, { Component } from 'react';
import ReactDOM from 'react-dom';
import PropTypes from 'prop-types';
import Scrollbar from './scrollbar.jsx';
import { Link } from 'react-router-dom';

class Items extends Component {
  constructor(props) {
    super(props);
    this.state = {
      title: '',
      logo: '',
      show: false
    };
    let path = props.file.slice(0, props.file.lastIndexOf('/') + 1);
    console.log("========path======>",path);
    global.readFile(props.file, data => {
      let [title, desktopname] = data
        .substr('# '.length, data.indexOf('\n'))
        .split('|');
      // logo = `${path}${logo}`;
      // this.setState({ title, logo, show: true });
      global.qtObjects.manual.getAppIconPath(desktopname,(logopath) =>{
        this.setState({ logo:logopath});     
        });
  
      global.qtObjects.manual.getLocalAppName(desktopname,(appname) =>{
          this.setState({ title:appname});     
          });
      this.setState({show: true });
    });
  }

  //转义特定字符
  escapeRegExp(text) {
    return text.replace(/[-[\]{}()*+?.,\\^$|#\s]/g, '\\$&');
  }

  render() {
    let resultList = [];

    //将关键字转义

    let keyTemp = this.props.keyword;
    if (this.props.keyword !== '%')
    {
      keyTemp = decodeURIComponent(this.props.keyword)
    }

    // let keyTemp = decodeURIComponent(this.props.keyword)
    let re = new RegExp(this.escapeRegExp(keyTemp), 'gi');

    let cTitle =(
      <span
        className="resulttitle"
        dangerouslySetInnerHTML={{
          __html: this.state.title.replace(
            re,
            "<span class='highlight'>$&</span>"
          )
        }}
      />
    );

    for (let i = 0; i < this.props.idList.length; i++) {
      if (this.props.idList[i] == 'h0')
      {
        continue;
      }

      let c = (
        <div className="item" key={i} onClick={() => global.open(this.props.file, this.props.idList[i], this.props.keyword)}>
          <div
            className="itemTitle"
            dangerouslySetInnerHTML={{
              __html: this.props.titleList[i].replace(
                re,
                "<span class='highlight'>$&</span>"
              )
            }}
          />
          <div
            className="context"
            dangerouslySetInnerHTML={{
              __html: this.props.contentList[i]
              // __html:contentTrans
            }}
          />
        </div>
      );
      if (resultList.length < 5)
      {
        resultList.push(c);
      }
    }
    var sresultnum;
      if(this.props.idList.length >1)
      sresultnum=this.props.idList.length+global.i18n['ResultNumSuffixs'];
      else
      sresultnum=this.props.idList.length+global.i18n['ResultNumSuffix'];
    return (
      this.state.show && (
        <div className="items">
          <div className="itemsTitle" onClick={() => global.open(this.props.file,'',this.props.keyword)}>
            <img src={this.state.logo} />
            {cTitle}
            <span className="resultnum">{sresultnum}</span>
          </div>
          {resultList}
        </div>
      )
    );
  }
}
function Mismatch(props) {
  return (
    <div id="mismatch">
      <div>
        <div id="NoResult">
          {/* {global.i18n['NoResult'].replace('%1', keyword)} */}
          {global.i18n['NoResult']}
        </div>
      </div>
    </div>
  );
}
export default class SearchPage extends Component {
  constructor(props, context) {
    super(props, context);

    // console.log('search constructor:',this.context);
  }

  componentWillReceiveProps(nextProps){
    // console.log("search componentWillReceiveProps..",this.context.searchResult);
    console.log("search componentWillReceiveProps..");
  }

  shouldComponentUpdate(nextProps, nextState) {
    console.log("search shouldComponentUpdate..");
    return true;
  }

  componentDidUpdate() {
    ReactDOM.findDOMNode(this)
      .querySelector('#search')
      .focus();
  }
  render() {
    let c = null;
    if (this.context.mismatch) {
      c = <Mismatch keyword={this.props.match.params.keyword} />;
    } else {
      c = this.context.searchResult.map(result => (
        <Items
          key={result.file}
          file={result.file}
          idList={result.idList}
          titleList={result.titleList}
          contentList={result.contentList}
          keyword={this.props.match.params.keyword}
        />
      ));
    }
    return (
      <Scrollbar>
        <div
          id="search"
          tabIndex="-1"
        >
          {c}
        </div>
      </Scrollbar>
    );
  }
}

SearchPage.contextTypes = {
  searchResult: PropTypes.array,
  mismatch: PropTypes.bool
};
