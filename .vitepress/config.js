export default {
  base: '/blogs/',
  lang: 'en-US',
  title: "rfhits's blogs",
  description: "rfhits's blogs",
  themeConfig: {
    sidebar: [
      {
        text: '操作系统',
        collapsed: true,
        items: [
          {
            text: 'CAS 在 ARM 架构与 x86_64 架构上的实现',
            link: './operating-system/cas-on-arm-and-x86-64/cas-on-arm-and-x86-64.md'
          }
        ]
      }
    ]
  },
  markdown: {
    toc: {
      pattern: /^\[toc\]$/i,
      level: [1, 2]
    }
  }
}
