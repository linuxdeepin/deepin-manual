"use strict";

let expect = require("expect.js");
let sh = require("../www/jssrc/utils").searchHighlight;
let gai = require("../www/jssrc/utils").getAnchorItem;

describe("Text searching and highlighting", function() {
    it("can do the simple one", function() {
        let src = "I am fun.";
        expect(sh(src, ["am"])).to.equal(
            `I <span class="highlight">am</span> fun.`);
    });

    it("doesn't freak out when there's no match.", function() {
        let src = "";
        expect(sh(src, ["am"])).to.equal('');
    });

    it("doesn't freak out when there're no keywords", function() {
        let src = "I am fun.";
        expect(sh(src, [])).to.equal('');
    });

    it("can do multiple occurences", function() {
        let src = "I am fun; and fun am I.";
        expect(sh(src, ["am"])).to.equal(
            'I <span class="highlight">am</span> fun; ' +
            'and fun <span class="highlight">am</span> I.'
        );
    });

    it("should partial match", function() {
        let src = "I am fun.";
        expect(sh(src, ["am"])).to.equal('I <span class="highlight">am</span> fun.');
    });

    it("supports multiple keywords", function() {
        let src = "I am fun";
        expect(sh(src, ["fun", "am"])).to.equal(
            'I <span class="highlight">am</span> <span class="highlight">fun</span>'
        );
    });

    it("supports multiple keywords, even they're far apart", function() {
        let src = "I am really fun";
        expect(sh(src, ["am", "fun"])).to.equal(
            'I <span class="highlight">am</span> really <span class="highlight">fun</span>'
        );
    });

    it("ignores cases", function() {
        let src = "I AM FUN";
        expect(sh(src, ["am"])).to.equal(
            'I <span class="highlight">AM</span> FUN'
        );
    });

    it("supports CJK characters", function() {
        let src = "小地球大宇宙";
        expect(sh(src, ["地球"])).to.equal(
            '小<span class="highlight">地球</span>大宇宙'
        );
    });

    it("can handle backslashes", function() {
        let src = "Original Text";
        expect(sh(src, ["\\", "text"])).to.equal(
            'Original <span class="highlight">Text</span>'
        );
    });
});

describe("Search & Filter", function() {
    describe("can find added header(filterHighlight)", function() {
        it("returns null when not found", function() {
            expect(gai([], "NONEXIST")).to.equal(null);
        });
        it("returns the item if found", function() {
            let xs = [{
                anchorId: "existed",
            }];
            expect(gai(xs, "existed")).to.equal(xs[0]);
        });
        it("returns null when id doesn't match", function() {
            let xs = [{
                anchorId: "existed",
            }];
            expect(gai(xs, "wrongname")).to.equal(null);
        });
    });
});
