"use strict";

let expect = require("expect.js");
let Search = require("../www/jssrc/search");
let dedupKeywords = Search.dedupKeywords;
let splitSentences = Search.splitSentences;

describe("Search & Highlight", function() {
    describe("Split sentences", function() {
        it("can split Chinese sentences", function() {
            let s = "汇仁肾宝。他好，我也好。";
            expect(splitSentences(s, "。")).to.eql(
                ["汇仁肾宝。", "他好，我也好。"]
            );
        });

        it("can split English sentences", function() {
            let s = "Five score years ago, a great American, in whose symbolic shadow we stand today, signed the Emancipation Proclamation. This momentous decree came as a great beacon light of hope to millions of Negro slaves who had been seared in the flames of withering injustice. It came as a joyous daybreak to end the long night of their captivity.";
            expect(splitSentences(s, ".")).to.eql([
                "Five score years ago, a great American, in whose symbolic shadow we stand today, signed the Emancipation Proclamation.",
                "This momentous decree came as a great beacon light of hope to millions of Negro slaves who had been seared in the flames of withering injustice.",
                "It came as a joyous daybreak to end the long night of their captivity.",
            ]);
        });

        it("can handle empty text", function() {
            let s = "";
            expect(splitSentences(s)).to.eql([]);
        });
    });

    describe("De-duplicate keywords", function() {
        it("can do this", function() {
            let arr = [
                "This", "is", "an", "example",
                "That", "IS", "ALSO", "an", "example",
            ];
            expect(dedupKeywords(arr)).to.eql(
                ["This", "is", "an", "example", "That", "ALSO"]
            );
        });
    });
});
