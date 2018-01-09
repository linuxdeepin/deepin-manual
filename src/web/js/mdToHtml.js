import marked from 'marked'

export default function (mdFile, mdData) {
	let hlist = []
	let info = {}
	let html = ""

	let path = mdFile.slice(0, mdFile.lastIndexOf("/") + 1)
	let renderer = new marked.Renderer()
	let count = {}
	renderer.heading = (text, level) => {
		if (level == 1) {
			let [title, logo] = text.split("|")
			logo = path + logo
			console.log(logo)
			info = { title, logo }
			return ''
		}
		if (level == 2) {
			text = text.split("|")[0]
		}
		let id
		if (count[text] == null) {
			id = text
			count[text] = 1
		} else {
			id = `${text}-${count[text]}`
			count[text]++
		}
		let type = 'h' + level
		if (level == 2 || level == 3) {
			hlist.push({ id, text, type })
		}
		return `<${type} id="${id}">${text}</${type}>\n`
	}
	html = marked(mdData, { renderer }).replace(/src="/g, `$&${path}`)
	return { html, hlist, info }
}