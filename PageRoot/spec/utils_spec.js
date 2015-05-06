"use strict";

let getSmallSvg = require("../www/jssrc/renderer").getSmallSvg;
let expect = require("expect.js");

describe("Utils", function() {
    it("can get small svg path from big ones", function() {
        {
            let big = "file:///path/to/big.svg";
            expect(getSmallSvg(big)).to.equal("file:///path/to/big_small.svg");
        }
        {
            let big = "http://example.com/images/big.svg";
            expect(getSmallSvg(big)).to.equal("http://example.com/images/big_small.svg");
        }
        {
            let n = null;
            expect(getSmallSvg(n)).to.equal(null);
        }
    });
});
