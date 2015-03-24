"use strict";

var utils = require("../www/jssrc/utils");
var expect = require("expect.js");
var gdmfi = utils.getDManFileInfo;

describe("Resolution: DMan scheme", function() {
    it("Dman scheme", function() {
        let p = "dman://app2#Intro";
        expect(gdmfi(p, "en_US")).to.eql({
            baseDir: "file:///usr/share/dman/app2",
            dir: "file:///usr/share/dman/app2/en_US",
            hash: "Intro",
            lang: "en_US",
        });
    });

    it("Dman scheme with language code in the url", function() {
        let p = "dman://app2?lang=en_US#Conclusion";
        expect(gdmfi(p)).to.eql({
            baseDir: "file:///usr/share/dman/app2",
            dir: "file:///usr/share/dman/app2/en_US",
            hash: "Conclusion",
            lang: "en_US",
        });
    });

    it("File scheme(explicit)", function() {
        let p = "file:///home/user/Desktop/work/app3_enUS.md";
        expect(gdmfi(p)).to.eql({
            baseDir: "file:///home/user/Desktop/work",
            dir: "file:///home/user/Desktop/work",
            hash: null,
            lang: null,
        })
    });

    it("File scheme(implicit)", function() {
        let p = "/home/user/Desktop/work/app2_enUS.md";
        expect(gdmfi(p)).to.eql({
            baseDir: "file:///home/user/Desktop/work",
            dir: "file:///home/user/Desktop/work",
            hash: null,
            lang: null,
        });
    });

    it("HTTP scheme", function() {
        let p = "http://docs.example.com/dman/app3/zhCN/main.md";
        expect(gdmfi(p)).to.eql({
            baseDir: "http://docs.example.com/dman/app3",
            dir: "http://docs.example.com/dman/app3/zhCN",
            hash: null,
            lang: "zhCN",
        });
    });

    it("HTTPS scheme", function() {
        let p = "https://docs.example.com/dman/app3/zhCN/main.md#使用说明";
        expect(gdmfi(p)).to.eql({
            baseDir: "https://docs.example.com/dman/app3",
            dir: "https://docs.example.com/dman/app3/zhCN",
            hash: "使用说明",
            lang: "zhCN",
        });
    });
});


describe("Other Paths", function() {
    describe("Content Style", function() {
        let gcsp = utils.getContentStylePath;
        it("HTTP & HTTPS", function() {
            {
                let r = gcsp("http://192.168.1.101:82/dman/index.html?a=b&c=d#Hello");
                expect(r).to.equal("http://192.168.1.101:82/dman/style");
            }
            {
                let r = gcsp("https://192.168.1.101/dman/index.html?a=b&c=d#Hello");
                expect(r).to.equal("https://192.168.1.101/dman/style");
            }
        });

        it("file (implicit)", function() {
            let r = gcsp("/usr/doc/dman/index.html?a=b&c=d#Hello");
            expect(r).to.equal("file:///usr/doc/dman/style");
        });

        it("file (explicit)", function() {
            let r = gcsp("file:///usr/doc/dman/index.html?a=b&c=d#Hello");
            expect(r).to.equal("file:///usr/doc/dman/style");
        });

    });
});