import React, { Component } from 'react'
import ReactDOM from 'react-dom'
import { Scrollbars } from 'react-custom-scrollbars'

import m2h from './mdToHtml'

export default class Article extends Component {
	constructor(props) {
		super(props)
		this.hash = this.props.hash
		this.state = {
			preview: null,
			contentMenuStyle: null,
		}
	}
	componentDidUpdate() {
		if (this.hash != this.props.hash) {
			this.hash = this.props.hash
			console.log(this.hash, ReactDOM.findDOMNode(this))
			let hashDOM = document.getElementById(this.hash)
			if (hashDOM) {
				this.scrollbars.scrollTop(this.scrollbars.getScrollTop() + hashDOM.getBoundingClientRect().top)
			}
		}
	}
	scroll() {
		if (this.state.preview != null || this.state.contentMenuStyle != null) {
			this.setState({ preview: null, contentMenuStyle: null })
		}
		let hList = ReactDOM.findDOMNode(this).querySelectorAll("h2,h3")
		let hash = hList[0].id
		for (let i = 0; i < hList.length; i++) {
			if (hList[i].getBoundingClientRect().top > 1) {
				break
			}
			hash = hList[i].id
		}
		if (this.hash != hash) {
			console.log("hash update")
			this.hash = hash
			this.props.setHash(hash)
		}
	}
	showPreview(appName, hash, rect) {
		let file = `${global.path}/${appName}/${global.lang}/index.md`
		global.readFile(file, data => {
			let {html} = m2h(file, data)
			let d = document.createElement("div")
			d.innerHTML = html
			let hashDom = d.querySelector("#" + hash)
			let DomList = [hashDom]
			let nextDom = hashDom.nextElementSibling
			while (nextDom) {
				if (nextDom.nodeName == hashDom.nodeName) {
					break
				}
				DomList.push(nextDom)
				nextDom = nextDom.nextElementSibling
			}
			d.innerHTML = ""
			DomList.map(el => d.appendChild(el))
			html = d.innerHTML
			let {top, left} = rect
			let style = {
				top, left
			}
			let tClass = "t_"
			if (left > document.body.clientWidth / 2) {
				style.left -= 350
				tClass += "right_"
			} else {
				tClass += "left_"
				style.left += rect.width - 60 - 15
			}
			if (top > document.body.clientHeight / 2) {
				tClass += "down"
				style.top -= 250 + 20
			} else {
				tClass += "up"
				style.top += rect.height + 10
			}
			this.setState({ preview: { html, style, tClass } })
		})
	}
	click(e) {
		switch (e.target.nodeName) {
			case "IMG":
				e.preventDefault()
				let src = e.target.src
				if (src.indexOf(".svg") != -1) {
					return
				}
				console.log("imageViewer", src)
				global.qtObjects.imageViewer.open(src)
				return
			case "A":
				const dmanProtocol = "dman://"
				const hashProtocol = "#"
				const href = e.target.getAttribute("href")
				switch (0) {
					case href.indexOf(hashProtocol):
						e.preventDefault()
						this.props.setHash(href.slice(1))
						return
					case href.indexOf(dmanProtocol):
						e.preventDefault()
						const [appName, hash] = href.slice(dmanProtocol.length + 1).split("#")
						const rect = e.target.getBoundingClientRect()
						this.showPreview(appName, hash, rect)
						return
				}
		}
		if (window.getSelection().toString() != "") {
			if (this.state.contentMenuStyle != null) {
				this.setState({ contentMenuStyle: null })
			}
			return
		}
		if (this.state.preview != null || this.state.contentMenuStyle != null) {
			this.setState({ preview: null, contentMenuStyle: null })
		}
	}
	contextMenu(e) {
		e.preventDefault()
		if (window.getSelection().toString() != "") {
			this.setState({
				contentMenuStyle: {
					top: e.clientY,
					left: e.clientX
				}
			})
		}
	}
	render() {
		return <div id="article" onContextMenu={e => this.contextMenu(e)} onClick={this.click.bind(this)} >
			<Scrollbars autoHide autoHideTimeout={1000} onScroll={e => this.scroll(e)} ref={s => { this.scrollbars = s } }>
				<div className="read" dangerouslySetInnerHTML={{ __html: this.props.html }}></div>
				<div id="fillblank" />
			</Scrollbars>
			{
				this.state.preview != null &&
				<div style={this.state.preview.style} className={this.state.preview.tClass} id="preview" >
					<Scrollbars>
						<div className="read" dangerouslySetInnerHTML={{ __html: this.state.preview.html }}></div>
					</Scrollbars>
				</div>
			}
			{
				this.state.contentMenuStyle != null &&
				<div id="contextMenu" style={this.state.contentMenuStyle}>
					<h4 onClick={() => document.execCommand('Copy')}>
						{global.i18n["Copy"]}
					</h4>
				</div>
			}
		</div>
	}
}
