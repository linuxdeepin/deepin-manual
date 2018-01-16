import React, { Component } from "react"
import ReactDOM from "react-dom"
import { Link } from "react-router-dom"

class Nav extends Component {
	componentDidMount() {
		document.getElementById("article").style.marginLeft = ReactDOM.findDOMNode(
			this
		).clientWidth
		this.componentDidUpdate()
	}
	componentDidUpdate() {
		let hashDOM = ReactDOM.findDOMNode(this).querySelector(".hash")
		if (hashDOM.getAttribute("cid") == this.props.hlist[0].id) {
			ReactDOM.findDOMNode(this).scrollTop = 0
			return
		}
		hashDOM.scrollIntoViewIfNeeded(false)
	}
	click(e) {
		let cid = e.target.getAttribute("cid")
		if (cid) {
			this.props.setHash(cid)
		}
	}
	wheel(e) {
		let nav = ReactDOM.findDOMNode(this)
		if (e.deltaY > 0) {
			if (
				e.deltaY > 0 &&
				nav.scrollHeight == nav.clientHeight + nav.scrollTop
			) {
				e.preventDefault()
			} else if (e.deltaY < 0 && nav.scrollTop == 0) {
				e.preventDefault()
			}
		}
	}
	render() {
		return (
			<div
				id="nav"
				lang={global.lang}
				onClick={e => this.click(e)}
				onWheel={this.wheel.bind(this)}
			>
				<div id="hlist">
					<Link to="/index">
						<div type="h2" id="backHome">
							{global.i18n["ToIndexPage"]}
						</div>
					</Link>
					{this.props.hlist.map(h => {
						return (
							<div
								key={h.id}
								cid={h.id}
								type={h.type}
								className={this.props.hash == h.id ? "hash" : undefined}
							>
								{h.text}
								<br />
							</div>
						)
					})}
				</div>
			</div>
		)
	}
}

export default Nav
