import marked from 'marked';

export default function(mdFile, mdData) {
  let hlist = [];
  let info = {};
  let html = '';

  let path = mdFile.slice(0, mdFile.lastIndexOf('/') + 1);
  let renderer = new marked.Renderer();
  let count = 0;
  renderer.heading = (text, level) => {
    let id = 'h' + count;
    count++;
    if (level == 1) {
      let [title, logo] = text.split('|');
      logo = path + logo;
      console.log(logo);
      info = { title, logo };
      return '';
    }
    if (level == 2) {
      text = text.split('|')[0];
    }
    let type = 'h' + level;
    if (level == 2 || level == 3) {
      hlist.push({ id, text, type });
    }
    return `<${type} id="${id}" text="${text}">${text}</${type}>\n`;
  };
  html = marked(mdData, { renderer }).replace(/src="/g, `$&${path}`);
  return { html, hlist, info };
}
