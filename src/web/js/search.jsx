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
    global.readFile(props.file, data => {
      let [title, logo] = data
        .substr('# '.length, data.indexOf('\n'))
        .split('|');
      logo = `${path}${logo}`;
      this.setState({ title, logo, show: true });
    });
  }
  render() {
    let resultList = [];
    let re = new RegExp(this.props.keyword, 'gi');
    for (let i = 0; i < this.props.idList.length; i++) {
      let c = (
        <div className="item" key={i} onClick={() => global.open(this.props.file, this.props.idList[i])}>
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
              __html: this.props.contentList[i].replace(
                re,
                "<span class='highlight'>$&</span>"
              )
            }}
          />
        </div>
      );
      resultList.push(c);
    }
    var sresultnum;
      if(resultList.length>1)
      sresultnum=resultList.length+global.i18n['ResultNumSuffixs'];
      else
      sresultnum=resultList.length+global.i18n['ResultNumSuffix'];
    return (
      this.state.show && (
        <div className="items">
          <div className="itemsTitle" onClick={() => global.open(this.props.file)}>
            <img src={this.state.logo} />
            <span className="resulttitle">{this.state.title}</span>
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
          {global.i18n['NoResult'].replace('%1', props.keyword)}
        </div>
        <div id="WikiSearch">{global.i18n['WikiSearch']}</div>
        <span
          id="button"
          onClick={() =>
            global.openWindow(
              `https://wiki.deepin.org/index.php?title&search=${encodeURIComponent(
                props.keyword
              )}`
            )}
        >
          {global.i18n['SearchInWiki']}
        </span>
      </div>
    </div>
  );
}
export default class SearchPage extends Component {
  constructor(props, context) {
    super(props, context);
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
          onMouseOver={e => document.getElementById('search').focus()}
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
