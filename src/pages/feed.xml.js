import rss from '@astrojs/rss';
import { getPublishedEntries } from '../lib/content';
import { withBase } from '../lib/paths';
import { siteConfig } from '../site.config';

export async function GET(context) {
	const [essays, code, euler] = await Promise.all([
		getPublishedEntries('essays'),
		getPublishedEntries('code'),
		getPublishedEntries('euler'),
	]);

	const items = [...essays, ...code, ...euler]
		.sort((a, b) => b.data.published.valueOf() - a.data.published.valueOf())
		.map((entry) => ({
			title: entry.data.title,
			description: entry.data.description,
			pubDate: entry.data.published,
			link: withBase(
				entry.collection === 'essays'
					? `/essays/${entry.id}`
					: entry.collection === 'code'
						? `/code/${entry.id}`
						: `/project-euler/${entry.id}`,
			),
		}));

	return rss({
		title: siteConfig.title,
		description: siteConfig.description,
		site: new URL(import.meta.env.BASE_URL, context.site),
		items,
	});
}
