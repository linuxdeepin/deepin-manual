import React, { Component } from "react"
import PropTypes from "prop-types"

class Items extends Component {
	constructor(props) {
		super(props)
		this.state = {
			title: "",
			logo: "",
			show: false
		}
		let path = props.file.slice(0, props.file.lastIndexOf("/") + 1)
		global.readFile(props.file, data => {
			let [title, logo] = data
				.substr("# ".length, data.indexOf("\n"))
				.split("|")
			logo = `${path}${logo}`
			this.setState({ title, logo, show: true })
		})
	}
	render() {
		let resultList = []
		let re = new RegExp(this.props.keyword, "gi")
		for (let i = 0; i < this.props.idList.length; i++) {
			resultList.push(
				<div
					className="item"
					key={i}
					onClick={() => global.openFile(this.props.file, this.props.idList[i])}
				>
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
			)
		}
		return (
			this.state.show && (
				<div className="items">
					<div
						className="itemsTitle"
						onClick={() => global.openFile(this.props.file)}
					>
						<img src={this.state.logo} />
						<span>{this.state.title}</span>
					</div>
					{resultList}
				</div>
			)
		)
	}
}
function Mismatch(props) {
	return (
		<div id="mismatch">
			<div>
				<div id="NoResult">
					{global.i18n["NoResult"].replace("%1", props.keyword)}
				</div>
				<div id="WikiSearch">{global.i18n["WikiSearch"]}</div>
				<span id="button">
					<a
						target="_blank"
						href={
							"https://wiki.deepin.org/index.php?title&search=" +
							encodeURIComponent(props.keyword)
						}
					>
						{global.i18n["SearchInWiki"]}
					</a>
				</span>
			</div>
		</div>
	)
}
export default class SearchPage extends Component {
	constructor(props, context) {
		super(props, context)
	}
	render() {
		let c = null
		if (this.context.mismatch) {
			c = <Mismatch keyword={this.props.match.params.keyword} />
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
			))
		}
		return <div id="search">{c}</div>
	}
}

SearchPage.contextTypes = {
	searchResult: PropTypes.array,
	mismatch: PropTypes.bool
}
