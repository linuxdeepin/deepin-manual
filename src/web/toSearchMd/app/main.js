const marked = require('marked');
const fsK = require('fs');
const cheerio = require('cheerio');

function init()
{
    if (typeof qt != 'undefined')
    {
        new QWebChannel(qt.webChannelTransport, function(channel)
        {
          global.context = channel.objects.context;
        }
        );
    }
    else
    {
        alert("qt对象获取失败！");
    }
}
init();


global.parseMdList=(listMdPath)=>{
  console.log("=====>",listMdPath);

  var mdList = listMdPath.split(',');

  mdList.map(path=>{
    console.log("==>",path);
  })

  mainTmp(mdList);
}

global.readFile = (fileName, callback) => {
  console.log("global.readFile...");
  let xhr = new XMLHttpRequest();
  xhr.open('GET', fileName);
  xhr.onload = () => {
    if (xhr.responseText != '') {
      callback(xhr.responseText);
    }
  };
  xhr.send();
};

function delHtmlTagAndNewLine(str) {
    let result = str.toString();
    result = result.replace(/[\n]/g, ""); 
    result = result.replace(/<(?!img).*?>/g, '');
  
    let reg = /<(img.*?)(alt=\".*?\")>/g;
    result = result.replace(reg, '<$1>');
    return result;
}

function mainTmp(pathList){
    console.log("======>", pathList);
    if (pathList.length > 0)
    {
        pathList.forEach(function(pathObj){
          global.readFile(pathObj, data => {
            let count = 0;
            let renderer = new marked.Renderer();
            renderer.heading = (text, level) => {
              if (level <= 2) {
                text = text.split('|')[0];
              }
              let id = 'h' + count;
              count++;
              let type = 'h' + level;
              return `<${type} id="${id}">${text}</${type}>\n`;
            };

            // var htmlData = marked(data, { renderer }).replace(/src="/g, ``);
            var htmlData = marked(data,{renderer});

            console.log("========>",htmlData);
            const $ = cheerio.load(htmlData, {decodeEntities: false});
            let hlist = $(':header');
            let searchIndex = [];
            console.log("main init..........");
            hlist.map(i => {
              let el = $(hlist[i]);
              searchIndex.push([
                el.attr('id'),
                el.text(),
                delHtmlTagAndNewLine(el.nextUntil(hlist[i + 1]).contents())
              ]);
            });
            var ret = JSON.stringify(searchIndex);
            console.log('=============>', ret);
            global.context.recvParseMsg(ret,pathObj);
          })
        })
    }
}
