import marked from 'marked';

//转义特定字符
function escapeRegExp(text) {
    return text.replace(/[-[\]{}()*+?.,\\^$|#\s]/g, '\\$&');
};

export default function(mdFile, mdData, key = '') {
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
        let titlekey = text;
        console.log("key======text========>", titlekey);
        let type = 'h' + level;
        if (level == 2) {
            hlist.push({ id, text, type });
        }else if(level == 3){
            if(text.split('|').length > 1) {
                titlekey = text.split('|')[1];
                text = text.split('|')[0];
                //global.qtObjects.manual.LogPrint('start key:' + key);
                //global.qtObjects.manual.LogPrint('start key:' + text);
            }
            
            hlist.push({ id, text, type });
        }

        return `<${type} id="${id}" text="${titlekey}">${text}</${type}>\n`;
    };
    console.log(path);
    renderer.image = (href, title, text) => {
        let hrefX2 = href;

        // if (devicePixelRatio >= 5.0 && href.indexOf('.svg') == -1) {
        //     // global.qtObjects.manual.LogPrint('start hrefX2:' + hrefX2);
        //     let path = href.split('.');
        //     let ext = path.pop();
        //     // global.qtObjects.manual.LogPrint(path + '--' + ext);
        //     hrefX2 = `${path.join('.')}x2.${ext}`;
        //     // global.qtObjects.manual.LogPrint('end hrefX2:' + hrefX2);
        // }

        return `<img src="${hrefX2}" data-src="${href}" alt="${text}" />`;
    };

    html = marked(mdData, { renderer }).replace(/src="/g, `$&${path}`);
    console.log("-----------------------------------");
    if (key != '') {
        console.log("regexp==============>", key);
        //将'=-='字符串 反向还原成'%'
        key = key.replace(/=-=/g, '%');

        console.log("regexp===>", key);

        //将关键字转义
        const keyTemp = new RegExp(escapeRegExp(key), 'gi');


        // key = re;
        var finder = new RegExp(">.*?<", 'g') // 提取位于标签内的文本，避免误操作 class、id 等
        html = html.replace(finder, function(matched) {
            return matched.replace(new RegExp(keyTemp, 'gi'), "<span style='background-color: yellow'>$&</span>");
        })
    }

    return { html, hlist, info };
}
