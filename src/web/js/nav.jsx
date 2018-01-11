import React, { Component } from "react"
import ReactDOM from "react-dom"
import { Scrollbars } from "react-custom-scrollbars"

class Nav extends Component {
	componentDidUpdate() {
		let hashDOM = ReactDOM.findDOMNode(this).querySelector(".hash")
		if (hashDOM) {
			let { top, bottom } = hashDOM.getBoundingClientRect()
			if (top < 0) {
				this.scrollbars.scrollTop(this.scrollbars.getScrollTop() + top)
			} else if (bottom > this.scrollbars.getClientHeight()) {
				let h = bottom - this.scrollbars.getClientHeight()
				this.scrollbars.scrollTop(this.scrollbars.getScrollTop() + h)
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
			<div id="nav" onClick={e => this.click(e)}>
				<Scrollbars
					autoHide
					autoHideTimeout={1000}
					ref={s => {
						this.scrollbars = s
					}}
				>
					{this.props.hlist.map(h => {
						let NodeName = h.type
						return (
							<NodeName
								key={h.id}
								cid={h.id}
								className={this.props.hash == h.id ? "hash" : undefined}
								lang={global.lang}
							>
								{h.text}
							</NodeName>
						)
					})}
				</Scrollbars>
			</div>
		)
	}
}

export default Nav
