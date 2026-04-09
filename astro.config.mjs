// @ts-check
import mdx from '@astrojs/mdx';
import sitemap from '@astrojs/sitemap';
import { defineConfig } from 'astro/config';
import rehypeKatex from 'rehype-katex';
import remarkMath from 'remark-math';

export default defineConfig({
	site: process.env.SITE_URL ?? 'https://nghia03092004.github.io',
	base: process.env.BASE_PATH ?? '/',
	integrations: [mdx(), sitemap()],
	markdown: {
		remarkPlugins: [remarkMath],
		rehypePlugins: [rehypeKatex],
		syntaxHighlight: 'shiki',
	},
});
