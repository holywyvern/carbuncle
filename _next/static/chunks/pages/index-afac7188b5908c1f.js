(self.webpackChunk_N_E=self.webpackChunk_N_E||[]).push([[405],{5557:function(e,n,s){(window.__NEXT_P=window.__NEXT_P||[]).push(["/",function(){return s(126)}])},5160:function(e,n,s){"use strict";var t=s(5893);function Image(e){let{src:n,...s}=e;return(0,t.jsx)("img",{src:"".concat("/carbuncle").concat(n),...s})}n.Z=Image},2458:function(e,n,s){"use strict";var t=s(5893),l=s(4184),r=s.n(l),a=s(3585),o=s.n(a);function Panel(e){let{children:n,stretch:s,noOverflow:l}=e,a=r()({[o().panel]:!0,[o().stretch]:s}),c=r()({[o().contents]:!0,[o()["no-overflow"]]:l});return(0,t.jsxs)("div",{className:a,children:[(0,t.jsx)("div",{className:o().tr}),(0,t.jsx)("div",{className:o().bl}),(0,t.jsx)("div",{className:o().left}),(0,t.jsx)("div",{className:o().right}),(0,t.jsx)("div",{className:o().top}),(0,t.jsx)("div",{className:o().bottom}),(0,t.jsx)("div",{className:c,children:n})]})}n.Z=Panel},9354:function(e,n,s){"use strict";var t=s(5893),l=s(2481),r=s(6671);function RubyCode(e){let{children:n,style:s=r.Xv,background:a="transparent"}=e;return(0,t.jsx)(l.Z,{language:"ruby",style:s,customStyle:{background:a},children:n})}n.Z=RubyCode},126:function(e,n,s){"use strict";s.r(n),s.d(n,{default:function(){return pages}});var t=s(5893),l=s(9571),r=s.n(l),a=s(8352),header_link=function(e){let{to:n,children:s}=e;return(0,t.jsx)("a",{href:n,target:"_blank",className:r().link,children:s})},o=s(5160),c=s(9603),i=s(3024),d=s(9417);function Header(e){let{t:n}=e;return(0,t.jsxs)("header",{className:r().header,children:[(0,t.jsx)(o.Z,{className:r().logo,src:"/img/logo.svg",alt:"Carbuncle Logo",width:"auto",height:"auto"}),(0,t.jsxs)("div",{className:r().container,children:[(0,t.jsxs)("h1",{className:r().title,children:["Ca",(0,t.jsx)("span",{children:"r"}),"buncle",(0,t.jsx)("sup",{className:r().sup,children:"beta"})]}),(0,t.jsxs)("div",{className:r().subtitle,children:[(0,t.jsx)("h2",{children:"Game Engine"}),(0,t.jsxs)("div",{className:r().links,children:[(0,t.jsxs)(header_link,{to:"https://github.com/holywyvern/carbuncle",children:[(0,t.jsx)(c.G,{icon:i.faGithub}),"\xa0",n("Source")]}),(0,t.jsxs)(header_link,{to:"".concat("/carbuncle","/docs"),children:[(0,t.jsx)(c.G,{icon:d.faBook}),"\xa0",n("API Docs")]})]})]})]})]})}var u=(0,a.Zh)("common")(Header),h=s(7294),x=s(2458),m=s(7161),p=s.n(m);let _=(0,a.Zh)("common")(e=>{let{t:n,icon:s,title:l,download:r,version:a,loading:o}=e;return(0,t.jsxs)("div",{className:p().platform,children:[(0,t.jsx)(c.G,{icon:s,size:"4x"}),(0,t.jsx)("h4",{children:n(l)}),(0,t.jsx)("a",{href:o?void 0:r,children:o?n("Loading..."):a})]})}),f={loading:!0,tag:"0.7.0"};function DownloadSection(e){let{t:n}=e,[{loading:s,tag:l},r]=h.useState(f);return h.useEffect(()=>{fetch("https://api.github.com/repos/holywyvern/carbuncle/tags").then(e=>e.json()).then(e=>{var n;let s=(null===(n=e[0])||void 0===n?void 0:n.name)||"0.7.0";r({loading:!1,tag:s})}).catch(()=>r({loading:!1,tag:"0.7.0"}))},[]),(0,t.jsxs)(x.Z,{stretch:!0,children:[(0,t.jsx)("h3",{children:n("Downloads")}),(0,t.jsxs)("div",{className:p().downloads,children:[(0,t.jsx)(_,{loading:s,icon:i.faWindows,title:"Windows Builds",download:"https://github.com/holywyvern/carbuncle/releases/download/".concat(l,"/mruby-carbuncle.exe"),version:"".concat(l," (x64)")}),(0,t.jsx)(_,{loading:s,icon:i.faApple,title:"OS X Builds",download:"https://github.com/holywyvern/carbuncle/releases/download/".concat(l,"/mruby-carbuncle.app.zip"),version:"".concat(l," (x64)")}),(0,t.jsx)(_,{loading:s,icon:i.faLinux,title:"Linux Builds",download:"https://github.com/holywyvern/carbuncle/releases/download/".concat(l,"/mruby-carbuncle"),version:"".concat(l," (x64)")}),(0,t.jsx)(_,{loading:s,icon:d.faGlobe,title:"Web Builds",download:"https://github.com/holywyvern/carbuncle/releases/download/".concat(l,"/mruby-carbuncle.web.zip"),version:l})]})]})}var j=(0,a.Zh)("common")(DownloadSection),b=s(2080),g=s.n(b);function Introduction(e){let{t:n}=e;return(0,t.jsx)("section",{className:g().intro,children:(0,t.jsxs)("p",{children:[n("A Game library build with"),"\xa0",(0,t.jsx)("a",{href:"https://mruby.org/",target:"_blank",children:"MRuby"}),",\xa0",(0,t.jsx)("a",{href:"https://www.raylib.com/",target:"_blank",children:"Raylib"}),"\xa0",n("and more as a base for developing games while having fun across multiple platforms.")]})})}var y=(0,a.Zh)("common")(Introduction);function Feature(e){let{title:n,logo:s,children:l,t:r}=e;return(0,t.jsxs)("section",{children:[(0,t.jsxs)("h3",{children:[(0,t.jsx)(c.G,{icon:d[s]||i[s]}),"\xa0",r(n)]}),(0,t.jsx)("p",{children:l})]})}var w=(0,a.Zh)("common")(Feature),v=s(7373),k=s.n(v);function Features(e){let{t:n}=e;return(0,t.jsxs)("div",{className:k().features,children:[(0,t.jsx)(w,{title:"An Open Source Framework",logo:"faGithub",children:n("Completeley Open Source, and free to use.")}),(0,t.jsx)(w,{title:"Out of the Box",logo:"faBoxOpen",children:n("Contains many features including graphics, audio, input, networking and GUI!")}),(0,t.jsx)(w,{title:"Cross Platform",logo:"faLaptopCode",children:n("Works on Windows, Mac OSX and Linux with planned iOS and Android support.")}),(0,t.jsxs)(w,{title:"Web Support",logo:"faGlobe",children:[n("Deploy it as a website without much effort thanks to"),"\xa0",(0,t.jsx)("a",{href:"https://emscripten.org/",target:"_blank",children:"Emscripten"}),"."]})]})}var N=(0,a.Zh)("common")(Features),Z=s(1705),E=s.n(Z),C=s(9354);function Example(e){let{title:n,children:s,t:l}=e;return(0,t.jsxs)("section",{className:E().example,children:[(0,t.jsx)("header",{children:(0,t.jsx)("h4",{children:l(n)})}),(0,t.jsx)(x.Z,{stretch:!0,noOverflow:!0,children:(0,t.jsx)("div",{className:E().panel,children:(0,t.jsx)(C.Z,{children:s})})})]})}var S=(0,a.Zh)("common")(Example),G=s(1664),O=s.n(G);let R="\ninclude Carbuncle\n\nclass Example < Game\n  def load\n    @text = Text.new\n    @text.value = 'Hello World'\n  end\n\n  def draw\n    @text.draw\n  end\nend\n".trim(),F="\ninclude Carbuncle\n\nclass Example < Game\n  def load\n    texture = Texture.new('img/carbuncle.png')\n    @sprite = Sprite.new(texture)\n    @sprite.position.set(center)\n    @sprite.pivot.set(0.5, 0.5)\n  end\n\n  def draw\n    @sprite.draw\n  end\n\n  def center\n    Point.new(\n      screen.width / 2,\n      screen.height / 2\n    )\n  end\nend\n".trim(),B="\ninclude Carbuncle\n\nclass Example < Game\n  def load\n    @music = Music.new('bgm/awesome.ogg')\n    @sound = Sound.new('sfx/click.wav')\n    @music.play\n  end\n\n  def update(dt)\n    @sound.play if Keyboard.press?(:space)\n  end\nend\n".trim();function Examples(e){let{t:n}=e;return(0,t.jsxs)(t.Fragment,{children:[(0,t.jsxs)("h3",{children:[(0,t.jsx)(c.G,{icon:d.faCode})," ",n("Quick examples")]}),(0,t.jsx)(S,{title:"Hello World",children:R}),(0,t.jsx)(S,{title:"Drawing an Image",children:F}),(0,t.jsx)(S,{title:"Playing music and sound",children:B}),(0,t.jsx)("hr",{}),"Check out more examples in the"," ",(0,t.jsx)(O(),{href:"".concat("/carbuncle","/examples"),children:"Examples subdirectory"})]})}var L=(0,a.Zh)("common")(Examples),P=s(439),A=s.n(P),W=s(6964),D=s.n(W);let I=(0,a.Zh)("common")(e=>{let{t:n,title:s,links:l}=e;return(0,t.jsxs)("section",{children:[(0,t.jsx)("h4",{children:n(s)}),(0,t.jsx)("ul",{children:l.map(e=>{let{label:n,url:s}=e;return(0,t.jsx)("li",{children:(0,t.jsx)("a",{href:s,target:"_blank",children:n})},n)})})]})});function Footer(){return(0,t.jsx)("footer",{className:D().footer,children:(0,t.jsxs)("div",{className:D().content,children:[(0,t.jsx)(I,{title:"Special Thanks",links:[{label:"MRuby",url:"https://mruby.org/"},{label:"Raylib",url:"https://www.raylib.com/"}]}),(0,t.jsx)(I,{title:"Developers",links:[{label:"Ramiro Rojo",url:"https://ramirorojo.com/"},{label:"chronno",url:"https://github.com/chronno"}]})]})})}var X=(0,a.Zh)("common")(Footer),home_layout=function(e){let{children:n}=e;return(0,t.jsxs)("div",{className:A().wrapper,children:[(0,t.jsx)("div",{className:A().page,children:n}),(0,t.jsx)(X,{})]})},pages=function(){return(0,t.jsx)(t.Fragment,{children:(0,t.jsxs)(home_layout,{children:[(0,t.jsx)(u,{}),(0,t.jsx)(y,{}),(0,t.jsx)(j,{}),(0,t.jsx)(N,{}),(0,t.jsx)(L,{})]})})}},7161:function(e){e.exports={downloads:"styles_downloads__fm5jK",platform:"styles_platform__ywphE"}},1705:function(e){e.exports={example:"styles_example__zPrL6",panel:"styles_panel__kTRif"}},7373:function(e){e.exports={features:"styles_features__veYC5"}},6964:function(e){e.exports={footer:"styles_footer__1cZ8q",content:"styles_content__uRX4R"}},9571:function(e){e.exports={header:"styles_header__fpulf",logo:"styles_logo__SSUhE",title:"styles_title__POuvW",sup:"styles_sup__trFdD",container:"styles_container__CZwiU",subtitle:"styles_subtitle__nApj3",links:"styles_links__5qJaS",link:"styles_link__cNKXL"}},2080:function(e){e.exports={intro:"styles_intro__cFmBm"}},3585:function(e){e.exports={panel:"styles_panel__hRJ3H",tr:"styles_tr__j_fj0",bl:"styles_bl__r8OcQ",contents:"styles_contents__f_z7q",left:"styles_left__AU6l0",right:"styles_right__YA8kG",top:"styles_top__BtzBq",bottom:"styles_bottom__qOunZ",stretch:"styles_stretch__amO_k","no-overflow":"styles_no-overflow___x0LL"}},439:function(e){e.exports={wrapper:"styles_wrapper__fAfJh",page:"styles_page__Rfmmm"}}},function(e){e.O(0,[976,948,180,202,774,888,179],function(){return e(e.s=5557)}),_N_E=e.O()}]);