(self.webpackChunk_N_E=self.webpackChunk_N_E||[]).push([[570],{2720:function(e,s,t){(window.__NEXT_P=window.__NEXT_P||[]).push(["/examples/core/hello_world",function(){return t(8542)}])},5160:function(e,s,t){"use strict";var l=t(5893);function Image(e){let{src:s,...t}=e;return(0,l.jsx)("img",{src:"".concat("/carbuncle").concat(s),...t})}s.Z=Image},2458:function(e,s,t){"use strict";var l=t(5893),n=t(4184),a=t.n(n),c=t(3585),o=t.n(c);function Panel(e){let{children:s,stretch:t,noOverflow:n}=e,c=a()({[o().panel]:!0,[o().stretch]:t}),r=a()({[o().contents]:!0,[o()["no-overflow"]]:n});return(0,l.jsxs)("div",{className:c,children:[(0,l.jsx)("div",{className:o().tr}),(0,l.jsx)("div",{className:o().bl}),(0,l.jsx)("div",{className:o().left}),(0,l.jsx)("div",{className:o().right}),(0,l.jsx)("div",{className:o().top}),(0,l.jsx)("div",{className:o().bottom}),(0,l.jsx)("div",{className:r,children:s})]})}s.Z=Panel},6254:function(e,s,t){"use strict";var l=t(5893),n=t(7294),a=t(2481),c=t(6671),o=t(9603),r=t(8352),i=t(2458),u=t(7105),d=t(9417),_=t(6372),h=t.n(_);c.vs.background="transparent";let copyToClipboard=e=>{let s=document.createElement("textarea");s.value=e,s.setAttribute("readonly",""),s.style.position="absolute",s.style.left="-9999px",document.body.appendChild(s),s.select(),document.execCommand("copy"),document.body.removeChild(s)};function CodeBlock(e){let{id:s,t,code:n}=e;return n?(0,l.jsx)("div",{className:h()["code-container"],children:(0,l.jsx)(a.Z,{showLineNumbers:!0,showInlineLineNumbers:!0,language:"ruby",style:c.vs,customStyle:{background:"rgba(255, 255, 255, 0.4)"},children:n})}):(0,l.jsx)(i.Z,{children:t("Loading...")})}function Button(e){let{onClick:s,visible:t,t:n}=e,a=t?d.faEye:d.faEyeSlash;return(0,l.jsxs)("button",{type:"button",onClick:s,className:h().button,children:[(0,l.jsx)(o.G,{icon:a}),"\xa0",n(t?"Hide code":"Show code")]})}function CopyButton(e){let{onClick:s,disabled:t,t:n}=e;return(0,l.jsxs)("button",{type:"button",onClick:s,className:h().copy,disabled:t,children:[(0,l.jsx)(o.G,{icon:d.faCopy}),"\xa0",n("Copy Example")]})}function CarbuncleExample(e){let{id:s,t}=e,[a,c]=(0,n.useState)(null),[o,i]=(0,n.useState)(!1);return(0,n.useEffect)(()=>{fetch("".concat("/carbuncle","/data/examples").concat(s,"/main_").concat(null===r.ag||void 0===r.ag?void 0:r.ag.language,".rb")).then(e=>e.ok&&200===e.status?e.text():fetch("".concat("/carbuncle","/data/examples").concat(s,"/main.rb")).then(e=>e.text())).then(e=>c(e)).catch(console.error)},[s,null===r.ag||void 0===r.ag?void 0:r.ag.language]),(0,l.jsx)(u.Z,{children:(0,l.jsxs)("div",{className:h().wrapper,children:[(0,l.jsx)("div",{className:h().frame,children:(0,l.jsx)("iframe",{src:"".concat("/carbuncle","/data/examples").concat(s,"/mruby-carbuncle.html"),width:"640",height:"480",frameBorder:"0"})}),o&&(0,l.jsx)("div",{className:h().code,children:(0,l.jsx)("div",{className:h()["code-wrapper"],children:(0,l.jsx)(CodeBlock,{id:s,t:t,code:a})})}),(0,l.jsx)(Button,{onClick:()=>i(!o),visible:o,t:t}),(0,l.jsx)(CopyButton,{onClick:()=>copyToClipboard(a),disabled:!a,t:t})]})})}s.Z=(0,r.Zh)("examples")(CarbuncleExample)},7105:function(e,s,t){"use strict";t.d(s,{Z:function(){return examples_layout}});var l=t(5893),n=t(7294),a=t(9603),c=t(8352),o=t(4184),r=t.n(o),i=t(1163),u=t(1664),d=t.n(u),_=t(9417),h=t(580),m=t.n(h),x=t(5160);let p=(0,c.Zh)("examples")(e=>{let{label:s,to:t,t:n}=e,a=(0,i.useRouter)(),c="/examples".concat(t),o=r()({[m().link]:!0,[m().active]:c===a.pathname});return(0,l.jsx)("li",{children:(0,l.jsx)(d(),{href:c,className:o,children:n(s)})})});function MenuButton(e){let{onClick:s,visible:t}=e,n=t?_.faEye:_.faEyeSlash;return(0,l.jsx)("button",{type:"button",onClick:s,className:m().button,children:(0,l.jsx)(a.G,{icon:n})})}let j=(0,c.Zh)("examples")(e=>{let{label:s,children:t,t:a}=e,[c,o]=(0,n.useState)(!0);return(0,l.jsxs)("div",{className:m().group,children:[(0,l.jsxs)("div",{className:m()["group-header"],children:[(0,l.jsx)("strong",{children:a(s)}),(0,l.jsx)(MenuButton,{onClick:()=>o(!c),visible:c})]}),c&&(0,l.jsx)("ul",{className:m().items,children:t})]})}),y=(0,c.Zh)("examples")(e=>{let{t:s}=e;return(0,l.jsxs)(l.Fragment,{children:[(0,l.jsxs)("header",{className:m().header,children:[(0,l.jsx)(x.Z,{src:"/img/logo.svg",className:m().logo}),(0,l.jsxs)("div",{children:[(0,l.jsxs)("h1",{children:["Ca",(0,l.jsx)("span",{className:m().hightlight,children:"r"}),"buncle"]}),(0,l.jsx)("h2",{children:s("Examples")})]})]}),(0,l.jsx)("hr",{})]})});var list=function(){return(0,l.jsxs)("nav",{className:m().menu,children:[(0,l.jsx)(y,{}),(0,l.jsx)(j,{label:"Core examples",children:(0,l.jsx)(p,{to:"/core/hello_world",label:"Hello World"})}),(0,l.jsx)(j,{label:"Graphics",children:(0,l.jsx)(p,{to:"/graphics/image_generation",label:"Image Generation"})}),(0,l.jsx)(j,{label:"Demos",children:(0,l.jsx)(p,{to:"/demos/snake",label:"Snake"})})]})},examples_layout=function(e){let{children:s}=e;return(0,l.jsxs)("div",{className:m().page,children:[(0,l.jsx)(list,{}),(0,l.jsx)("div",{className:m().view,children:s})]})}},8542:function(e,s,t){"use strict";t.r(s);var l=t(5893),n=t(6254);s.default=()=>(0,l.jsx)(n.Z,{id:"/core/hello_world"})},3585:function(e){e.exports={panel:"styles_panel__hRJ3H",tr:"styles_tr__j_fj0",bl:"styles_bl__r8OcQ",contents:"styles_contents__f_z7q",left:"styles_left__AU6l0",right:"styles_right__YA8kG",top:"styles_top__BtzBq",bottom:"styles_bottom__qOunZ",stretch:"styles_stretch__amO_k","no-overflow":"styles_no-overflow___x0LL"}},6372:function(e){e.exports={frame:"styles_frame__QUt_h",code:"styles_code__x6a2m",wrapper:"styles_wrapper__ITq5b",button:"styles_button__2JQOS",copy:"styles_copy__9UqHS","code-wrapper":"styles_code-wrapper___XeZM"}},580:function(e){e.exports={page:"styles_page__ccWhl",view:"styles_view__LKhWa",menu:"styles_menu__2TIov",group:"styles_group__hfaVh","group-header":"styles_group-header__2Kllj",button:"styles_button__w3on1",header:"styles_header__wdi1s",logo:"styles_logo__snJH_",items:"styles_items__w4QwB",link:"styles_link__3HV6s",active:"styles_active__3vbjy"}}},function(e){e.O(0,[976,664,202,424,774,888,179],function(){return e(e.s=2720)}),_N_E=e.O()}]);