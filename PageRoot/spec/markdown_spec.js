"use strict";

let Renderer = require("../www/jssrc/renderer");
let expect = require("expect.js");

let r = Renderer.getHTMLRenderer();
let pr = Renderer.getPlainRenderer();
let p = Renderer.processMarkdown;

describe("Markdown HTML Renderer", function() {
    describe("Headers", function() {
        it("understands h1", function() {
            let result = r.heading("This is a test", 1, "This is a test");
            expect(result).to.equal('');
        });
        it("understands h2 - h3", function() {
            {
                let result = r.heading("This is header2", 2, "This is header2");
                expect(result).to.equal('<h2 id="This-is-header2">This is header2</h2>\n');
            }
            {
                let result = r.heading("This is header3", 3, "This is header3");
                expect(result).to.equal('<h3 id="This-is-header3">This is header3</h3>\n');
            }
        });

        it("understands h4 - h6", function() {
            let result = r.heading("Header4", 4, "Header4");
            expect(result).to.equal('<h4>Header4</h4>\n');

            result = r.heading("Header5", 5, "Header5");
            expect(result).to.equal('<h5>Header5</h5>\n');

            result = r.heading("Header6", 6, "Header6");
            expect(result).to.equal('<h6>Header6</h6>\n');
        });

        it("warns about duplicate anchors", function() {
            let src = "# Hello World\n##Hello  World\n";
            expect(p).withArgs(src).to.throwError();
        });

        it("warns about redefinition of titles", function() {
            let src = "# Hello World\n# 你好世界\n";
            expect(p).withArgs(src).to.throwError();
        });

        describe("insists about the header hierarchy", function() {
            it("h2 under h1", function() {
                let src = "## H2";
                expect(p).withArgs(src).to.throwError();
            })

        })
    });

    describe("Images & Icons", function() {
        it("understands inline icons", function() {
            let src = "![Alt](alt.png)";
            let result = p(src).html;
            expect(result).to.equal(
                '<p><img src="alt.png" alt="Alt" class="inline"></p>\n')
        });

        it("understands images", function() {
            {
                let src = "![1|Alt](alt.png)";
                let result = p(src).html;
                expect(result).to.equal(
                    '<p><img src="alt.png" alt="Alt" class="block1"></p>\n')
            }
            {
                let src = "![2|Alt](alt.png)";
                let result = p(src).html;
                expect(result).to.equal(
                    '<p><img src="alt.png" alt="Alt" class="block2"></p>\n')
            }
            {
                let src = "![3|Alt](alt.png)";
                let result = p(src).html;
                expect(result).to.equal(
                    '<p><img src="alt.png" alt="Alt" class="block3"></p>\n')
            }
        });

        it("understands non-autosized images", function() {
            let src = "![0|Alt](alt.png)";
            let result = p(src).html;
            expect(result).to.equal(
                '<p><img src="alt.png" alt="Alt"></p>\n')
        });
    });

    describe("Links", function() {
        it("understands in-page links", function() {
            let src = "[Link Description](#Test)";
            let result = p(src).html;
            expect(result).to.equal(
                '<p><a href="javascript: window.parent.jumpTo(\'Test\');">Link Description</a></p>\n'
            );
        });

        it("understands external dman pages", function() {

        });

        it("emits dbus signal", function() {

        });

        it("runs command", function() {

        });
    });

    it("understands column control characters", function() {
        let dict = {
            "!←←": `<table class="columns"><tbody><tr><td>`,
            "!←← ": `<table class="columns"><tbody><tr><td>`,
            "←!→": `</td><td>`,
            "←!→\t": `</td><td>`,
            "→→!": `</td></tr></tbody></table>\n`,
            "→→! Useless Tail": `</td></tr></tbody></table>\n`,
        };
        for (let k in dict) {
            let v = dict[k];
            expect(r.paragraph(k)).to.equal(v);
        }
    });

    it("ignores non column control characters", function() {
        expect(r.paragraph("BIG-NEWS!")).to.equal("<p>BIG-NEWS!</p>\n");
    });
});

describe("Markdown Plain Renderer", function() {
    it("outputs nothing for headers", function() {
        {
            let result = pr.heading("This is a test", 1, "This is a test");
            expect(result).to.equal("");
        }
        {
            let result = pr.heading("This is a test", 2, "This is a test");
            expect(result).to.equal("");
        }
        {
            let result = pr.heading("This is a test", 3, "This is a test");
            expect(result).to.equal("");
        }
        {
            let result = pr.heading("This is a test", 4, "This is a test");
            expect(result).to.equal("");
        }
        {
            let result = pr.heading("This is a test", 5, "This is a test");
            expect(result).to.equal("");
        }
        {
            let result = pr.heading("This is a test", 6, "This is a test");
            expect(result).to.equal("");
        }
    });

    it("outputs nothing with header even with icons", function() {
        {
            let result = pr.heading("This is a test|a.png|", 1, "This is a test|a.png|");
            expect(result).to.equal("");
        }
        {
            let result = pr.heading("This is a test|a.png|", 2, "This is a test|a.png|");
            expect(result).to.equal("");
        }
        {
            let result = pr.heading("This is a test|a.png|", 3, "This is a test|a.png|");
            expect(result).to.equal("");
        }
        {
            let result = pr.heading("This is a test|a.png|", 4, "This is a test|a.png|");
            expect(result).to.equal("");
        }
        {
            let result = pr.heading("This is a test|a.png|", 5, "This is a test|a.png|");
            expect(result).to.equal("");
        }
        {
            let result = pr.heading("This is a test|a.png|", 6, "This is a test|a.png|");
            expect(result).to.equal("");
        }
    });

    it("outputs the alt text for non-header images/icons", function() {
        { // self-sized
            let result = pr.image("./hello.png", "This is an alt!", "This is an alt!");
            expect(result).to.equal('This is an alt!');
        }
        {
            let result = pr.image("./hello.png", "0|This is an alt!", "0|This is an alt!");;
            expect(result).to.equal('This is an alt!');
        }
        {
            let result = pr.image("./hello.png", "1|This is an alt!", "1|This is an alt!");;
            expect(result).to.equal('This is an alt!');
        }
        {
            let result = pr.image("./hello.png", "2|This is an alt!", "2|This is an alt!");;
            expect(result).to.equal('This is an alt!');
        }
        {
            let result = pr.image("./hello.png", "3|This is an alt!", "3|This is an alt!");;
            expect(result).to.equal('This is an alt!');
        }
    });

    it("outputs text only for tables", function() {
        let result = pr.table("header", "body");
        expect(result).to.equal("header body");
    });

    it("outputs text for table rows", function() {
        let result = pr.tablerow("content");
        expect(result).to.equal("content");
    });

    it("outputs text for table cells", function() {
        let result = pr.tablecell("content");
        expect(result).to.equal("content");
    });

    it("de-tag HTMLs", function() {
        let html = `
        <table>
            <caption>A Detagging Test</caption>
            <thead></thead>
            <tbody>
                <tr>
                    <td>1</td>
                    <td>2</td>
                    <td>5</td>
                    <td>3</td>
                    <td>8</td>
                </tr>
            </tbody>
        </table>
        <div>这一切都结束了</div>
        `;
        expect(pr.html(html)).to.equal("A Detagging Test 1 2 5 3 8 这一切都结束了");
    });

    it("ignores column control characters", function() {
        for (let txt of ["!←←", "←!→", "→→!", "→→! TAIL"]) {
            expect(pr.paragraph(txt)).to.equal("");
        }
    });

    it("shouldn't ignore non-column control characters", function() {
        let txt = "Hello world!";
        expect(pr.paragraph(txt)).to.equal(txt);
    })

});

describe("Navigation Parsing", function() {
    it("can do this sample", function() {
        let src = [
            "# H1|H1.png|",
            "## H2|H2.png|",
            "H2 Text",
            "### H3",
            "H3 Text",
            "## H2a|H2a.png|",
            "## H2b",
            "### H3a",
            "#### H4",
            "H4 Text",
            "##### H5",
            "H5 Text",
        ].join("\n");
        let result = p(src).parsed;
        expect(result.appInfo.name).to.equal("H1");
        expect(result.appInfo.icon).to.equal("H1.png");
        expect(result.anchors).to.eql([
            {
                "id": "H2",
                "children": [
                    {
                        "id": "H3",
                        "children": [],
                        "icon": null,
                        "text": "H3"
                    }
                ],
                "icon": "H2.png",
                "text": "H2"
            },
            {
                "id": "H2a",
                "children": [],
                "icon": "H2a.png",
                "text": "H2a",
            },
            {
                "id": "H2b",
                "children": [
                    {
                        "id": "H3a",
                        "children": [],
                        "icon": null,
                        "text": "H3a"
                    }
                ],
                "icon": null,
                "text": "H2b",
            }
        ]);
        expect(result.indices).to.eql([
            { headerId: 'H2', headerText: 'H2', texts: [ 'H2 Text' ] },
            { headerId: 'H3', headerText: 'H3', texts: [ 'H3 Text' ] },
            { headerId: 'H4', headerText: 'H4', texts: [ 'H4 Text' ] },
            { headerId: "H5", headerText: "H5", texts: [ 'H5 Text' ] },
        ]);
        expect(result.headers).to.eql(['H1', 'H2', 'H3', 'H2a', 'H2b', 'H3a', 'H4']);
    });
});

describe("Indexing", function() {

});
