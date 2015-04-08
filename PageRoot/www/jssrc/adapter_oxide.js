"use strict";

let usMsgId = "JSMESSAGE";

window.addEventListener("OxideSendMessage", function(e){
    oxide.sendMessage(e.detail.msgId, {
        "detail": e.detail.args,
    });
});

window.addEventListener("OxideSendMessageNoReply", function(e){
    oxide.sendMessageNoReply(e.detail.msgId, {
        "detail": e.detail.args,
    });
});

oxide.addMessageHandler(usMsgId, function(msg){
    let newEvent = new CustomEvent("OxideSignalMessage", {
        detail: msg.args.detail,
    });
    document.dispatchEvent(newEvent);
});
