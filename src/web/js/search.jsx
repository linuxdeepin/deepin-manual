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
    console.log("==========>",this.props.idList.length);

    for (let i = 0; i < this.props.idList.length; i++) {

      // let contentTrans = this.props.contentList[i];
      // let index = contentTrans.indexOf(this.props.keyword);
      // console.log("keyword index: ",index);
      // if (index > 100)
      // {
      //   contentTrans = "..." + contentTrans.slice(index-100);
      // }

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
          {global.i18n['NoResult'].replace('%1', decodeURIComponent(props.keyword))}
        </div>
        {/* <div id="WikiSearch">{global.i18n['WikiSearch']}</div>
        <span
          class="button"
          onClick={() =>
            global.openWindow(
              `https://wiki.deepin.org/index.php?title&search=${encodeURIComponent(
                props.keyword
              )}`
            )}
        >
          {global.i18n['SearchInWiki']}
        </span> */}
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
          // onMouseOver={e => document.getElementById('search').focus()}
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
