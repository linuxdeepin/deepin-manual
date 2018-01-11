import React, { Component } from "react"

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
		for (let i = 0; i < this.props.keys.length; i++) {
			resultList.push(
				<div
					className="item"
					key={i}
					onClick={() => global.openFile(this.props.file, this.props.keys[i])}
				>
					<div className="title">{this.props.keys[i]}</div>
					{this.props.values[i]}
				</div>
			)
		}
		console.log(resultList)
		return (
			this.state.show && (
				<div className="items">
					<div
						className="title"
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
export default class SearchPage extends Component {
	render() {
		return (
			<div id="search">
				{this.props.searchResult.map(result => (
					<Items
						key={result.file}
						file={result.file}
						keys={result.keys}
						values={result.values}
					/>
				))}
			</div>
		)
	}
}
