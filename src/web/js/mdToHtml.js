import marked from 'marked';

    //转义特定字符
function escapeRegExp(text) {
  return text.replace(/[-[\]{}()*+?.,\\^$|#\s]/g, '\\$&');
};

export default function(mdFile, mdData, key='') {
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
  console.log(path);
  renderer.image = (href, title, text) => {
    let hrefX2 = href;
    if (devicePixelRatio >= 1.5 && href.indexOf('.svg') == -1) {
      let path = href.split('.');
      let ext = path.pop();
      hrefX2 = `${path.join('.')}x2.${ext}`;
    }
    return `<img src="${hrefX2}" data-src="${href}" alt="${text}" />`;
  };
  
  html = marked(mdData, { renderer }).replace(/src="/g, `$&${path}`);
  console.log("-----------------------------------");
  if (key != '')
  {
    //将'-+'字符串 反向还原成'/'
    key = key.replace(/-+/g,'/');

    //将关键字转义
    const keyTemp = new RegExp(escapeRegExp(key), 'gi');

    
    // key = re;
    console.log("--------->",keyTemp);
    // var formatKeyword = key.replace(/[-\/\\^$*+?.()|[\]{}]/g, '\\$&')
    var finder = new RegExp(">.*?<",'g') // 提取位于标签内的文本，避免误操作 class、id 等
    html = html.replace(finder,function(matched){
            return matched.replace(new RegExp(keyTemp,'gi'),"<span style='background-color: yellow'>$&</span>");
    })
  }
  
  return { html, hlist, info };
}
