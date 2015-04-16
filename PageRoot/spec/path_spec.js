"use strict";

var utils = require("../www/jssrc/utils");
var expect = require("expect.js");


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