// @ts-check
import mdx from '@astrojs/mdx';
import sitemap from '@astrojs/sitemap';
import { defineConfig } from 'astro/config';

export default defineConfig({
	site: process.env.SITE_URL ?? 'https://example.com',
	base: process.env.BASE_PATH ?? '/',
	integrations: [mdx(), sitemap()],
	markdown: {
		syntaxHighlight: 'shiki',
	},
});
