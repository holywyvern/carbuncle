_N_E=(window.webpackJsonp_N_E=window.webpackJsonp_N_E||[]).push([[14],{FeUR:function(e,n,t){e.exports={example:"styles_example__3lVoQ",panel:"styles_panel__3w28V"}},"L3C+":function(e,n,t){e.exports={intro:"styles_intro__2RiVG"}},LTOh:function(e,n,t){e.exports={footer:"styles_footer__3x-UG",content:"styles_content__32y_4"}},RNiq:function(e,n,t){"use strict";t.r(n);var s=t("nKUr"),c=t("fhFx"),a=t.n(c),r=t("k7Sn");var i=function(e){var n=e.to,t=e.children;return Object(s.jsx)("a",{href:n,target:"_blank",className:a.a.link,children:t})},l=t("53eK");var o=Object(r.withTranslation)("common")((function(e){var n=e.t;return Object(s.jsxs)("header",{className:a.a.header,children:[Object(s.jsx)(l.a,{className:a.a.logo,src:"/img/logo.svg",alt:"Carbuncle Logo",width:"auto",height:"auto"}),Object(s.jsxs)("div",{className:a.a.container,children:[Object(s.jsxs)("h1",{className:a.a.title,children:["Ca",Object(s.jsx)("span",{children:"r"}),"buncle",Object(s.jsx)("sup",{className:a.a.sup,children:"beta"})]}),Object(s.jsxs)("div",{className:a.a.subtitle,children:[Object(s.jsx)("h2",{children:"Game Engine"}),Object(s.jsxs)("div",{className:a.a.links,children:[Object(s.jsx)(i,{to:"https://github.com/holywyvern/carbuncle",children:n("Source")}),Object(s.jsx)(i,{to:"".concat("/carbuncle","/examples"),children:n("Examples")})]})]})]})]})})),j=t("IP2g"),d=t("8tEE"),h=t("wHSu"),b=t("ks2/"),x=t("hhPK"),u=t.n(x);var O=Object(r.withTranslation)("common")((function(e){var n=e.t;return Object(s.jsxs)(b.a,{stretch:!0,children:[Object(s.jsx)("h3",{children:n("Downloads")}),Object(s.jsxs)("div",{className:u.a.downloads,children:[Object(s.jsxs)("div",{className:u.a.platform,children:[Object(s.jsx)(j.a,{icon:d.faWindows,size:"4x"}),Object(s.jsx)("h4",{children:n("Windows Builds")}),Object(s.jsx)("a",{href:"https://github.com/holywyvern/carbuncle/releases/download/0.6.0/win32-x64.zip",children:"0.6.0 (x64)"})]}),Object(s.jsxs)("div",{className:u.a.platform,children:[Object(s.jsx)(j.a,{icon:d.faApple,size:"4x"}),Object(s.jsx)("h4",{children:n("OS X Builds")}),Object(s.jsx)("a",{children:n("(Pending)")})]}),Object(s.jsxs)("div",{className:u.a.platform,children:[Object(s.jsx)(j.a,{icon:d.faLinux,size:"4x"}),Object(s.jsx)("h4",{children:n("Linux Builds")}),Object(s.jsx)("a",{children:n("(Pending)")})]}),Object(s.jsxs)("div",{className:u.a.platform,children:[Object(s.jsx)(j.a,{icon:h.faGlobe,size:"4x"}),Object(s.jsx)("h4",{children:n("Web Builds")}),Object(s.jsx)("a",{href:"https://github.com/holywyvern/carbuncle/releases/download/0.6.0/web.zip",children:"0.6.0"})]})]})]})})),p=t("L3C+"),m=t.n(p);var f=Object(r.withTranslation)("common")((function(e){var n=e.t;return Object(s.jsx)("section",{className:m.a.intro,children:Object(s.jsxs)("p",{children:[n("A Game library build with"),"\xa0",Object(s.jsx)("a",{href:"https://mruby.org/",target:"_blank",children:"MRuby"}),",\xa0",Object(s.jsx)("a",{href:"https://www.raylib.com/",target:"_blank",children:"Raylib"}),"\xa0",n("and more as a base for developing games while having fun across multiple platforms.")]})})}));var _=Object(r.withTranslation)("common")((function(e){var n=e.title,t=e.logo,c=e.children,a=e.t;return Object(s.jsxs)("section",{children:[Object(s.jsxs)("h3",{children:[Object(s.jsx)(j.a,{icon:h[t]||d[t]}),"\xa0",a(n)]}),Object(s.jsx)("p",{children:c})]})})),w=t("uK//"),g=t.n(w);var v=Object(r.withTranslation)("common")((function(e){var n=e.t;return Object(s.jsxs)("div",{className:g.a.features,children:[Object(s.jsx)(_,{title:"An Open Source Framework",logo:"faGithub",children:n("Completeley Open Source, and free to use.")}),Object(s.jsx)(_,{title:"Out of the Box",logo:"faBoxOpen",children:n("Contains many features including graphics, audio, input, networking and GUI!")}),Object(s.jsx)(_,{title:"Cross Platform",logo:"faLaptopCode",children:n("Works on Windows, Mac OSX and Linux with planned iOS and Android support.")}),Object(s.jsxs)(_,{title:"Web Support",logo:"faGlobe",children:[n("Deploy it as a website without much effort thanks to"),"\xa0",Object(s.jsx)("a",{href:"https://emscripten.org/",target:"_blank",children:"Emscripten"}),"."]})]})})),y=t("Nf1K"),N=t.n(y),k=t("FeUR"),T=t.n(k);var R=Object(r.withTranslation)("common")((function(e){var n=e.title,t=e.children,c=e.t;return Object(s.jsxs)("section",{className:T.a.example,children:[Object(s.jsx)("header",{children:Object(s.jsx)("h4",{children:c(n)})}),Object(s.jsx)(b.a,{stretch:!0,noOverflow:!0,children:Object(s.jsx)("div",{className:T.a.panel,children:Object(s.jsx)(N.a,{language:"ruby",children:t})})})]})})),E="\ninclude Carbuncle\n\nclass Example < Game\n  def load\n    @text = Text.new\n    @text.value = 'Hello World'\n  end\n\n  def draw\n    @text.draw\n  end\nend\n".trim(),G="\ninclude Carbuncle\n\nclass Example < Game\n  def load\n    texture = Texture.new('img/carbuncle.png')\n    @sprite = Sprite.new(texture)\n    @sprite.position.set(center)\n    @sprite.pivot.set(0.5, 0.5)\n  end\n\n  def draw\n    @sprite.draw\n  end\n\n  def center\n    Point.new(\n      screen.width / 2,\n      screen.height / 2\n    )\n  end\nend\n".trim(),C="\ninclude Carbuncle\n\nclass Example < Game\n  def load\n    @music = Music.new('bgm/awesome.ogg')\n    @sound = Sound.new('sfx/click.wav')\n    @music.play\n  end\n\n  def update(dt)\n    @sound.play if Keyboard.press?(:space)\n  end\nend\n".trim();var S=Object(r.withTranslation)("common")((function(e){var n=e.t;return Object(s.jsxs)(s.Fragment,{children:[Object(s.jsx)("h3",{children:n("Quick examples")}),Object(s.jsx)(R,{title:"Hello World",children:E}),Object(s.jsx)(R,{title:"Drawing an Image",children:G}),Object(s.jsx)(R,{title:"Playing music and sound",children:C})]})})),L=t("z86Z"),P=t.n(L),z=t("LTOh"),D=t.n(z);var F=Object(r.withTranslation)("common")((function(e){var n=e.t;return Object(s.jsx)("footer",{className:D.a.footer,children:Object(s.jsxs)("div",{className:D.a.content,children:[Object(s.jsxs)("section",{children:[Object(s.jsx)("h4",{children:n("Special Thanks")}),Object(s.jsxs)("ul",{children:[Object(s.jsx)("li",{children:Object(s.jsx)("a",{href:"https://mruby.org/",target:"_blank",children:"MRuby"})}),Object(s.jsx)("li",{children:Object(s.jsx)("a",{href:"https://www.raylib.com/",target:"_blank",children:"Raylib"})})]})]}),Object(s.jsxs)("section",{children:[Object(s.jsx)("header",{children:Object(s.jsx)("h4",{children:n("Developers")})}),Object(s.jsxs)("ul",{children:[Object(s.jsx)("li",{children:Object(s.jsx)("a",{href:"https://ramirorojo.com/",target:"_blank",children:"Ramiro Rojo"})}),Object(s.jsx)("li",{children:Object(s.jsx)("a",{href:"https://github.com/chronno",target:"_blank",children:"chronno"})})]})]})]})})}));var K=function(e){var n=e.children;return Object(s.jsxs)("div",{className:P.a.wrapper,children:[Object(s.jsx)("div",{className:P.a.page,children:n}),Object(s.jsx)(F,{})]})};n.default=function(){return Object(s.jsx)(s.Fragment,{children:Object(s.jsxs)(K,{children:[Object(s.jsx)(o,{}),Object(s.jsx)(f,{}),Object(s.jsx)(O,{}),Object(s.jsx)(v,{}),Object(s.jsx)(S,{})]})})}},fhFx:function(e,n,t){e.exports={header:"styles_header__15DYH",logo:"styles_logo__2-Dc4",title:"styles_title__3In2G",sup:"styles_sup__j65Oh",container:"styles_container__3IJ0O",subtitle:"styles_subtitle__1ZTJx",links:"styles_links__2b69C",link:"styles_link__3G7nV"}},hhPK:function(e,n,t){e.exports={downloads:"styles_downloads__2TN76",platform:"styles_platform__1u0Ru"}},"uK//":function(e,n,t){e.exports={features:"styles_features__1FUvf"}},vlRD:function(e,n,t){(window.__NEXT_P=window.__NEXT_P||[]).push(["/",function(){return t("RNiq")}])},z86Z:function(e,n,t){e.exports={wrapper:"styles_wrapper__1R4LX",page:"styles_page__2jOQ0"}}},[["vlRD",1,2,5,7,0,3,4,6]]]);