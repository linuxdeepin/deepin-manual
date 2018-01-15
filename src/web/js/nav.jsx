import React, { Component } from "react"
import ReactDOM from "react-dom"
import { Link } from "react-router-dom"

class Nav extends Component {
	componentDidUpdate() {
		let nav = ReactDOM.findDOMNode(this)
		let hashDOM = nav.querySelector(".hash")
		if (this.props.hash == this.props.hlist[0].id) {
			nav.scrollTop = 0
		}
		if (hashDOM) {
			let { top, bottom } = hashDOM.getBoundingClientRect()
			if (top < 0) {
				nav.scrollTop += top
			} else if (bottom > nav.clientHeight) {
				nav.scrollTop += bottom - nav.clientHeight
			}
		}
	}
	click(e) {
		let cid = e.target.getAttribute("cid")
		if (cid) {
			this.props.setHash(cid)
		}
	}
	render() {
		return (
			<div id="nav" lang={global.lang} onClick={e => this.click(e)}>
				<div id="hlist">
					<div type="h2" id="backHome">
						<Link to="/index">{global.i18n["ToIndexPage"]}</Link>
					</div>
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
