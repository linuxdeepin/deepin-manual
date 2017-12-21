const Request = require("browser-request")

exports.read=function (fname) {
    return new Promise((resolve, reject) => {
        console.log("1")
        let url=`file:///usr/share/dman/${fname}`
        Request.get(url,(error, response,body)=>resolve(body))
    })
}