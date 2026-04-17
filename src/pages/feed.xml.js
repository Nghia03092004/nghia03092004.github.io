import rss from '@astrojs/rss';
import { getPublishedEntries } from '../lib/content';
import { getEulerRecords } from '../features/project-euler/lib/euler';
import { withBase } from '../lib/paths';
import { siteConfig } from '../site.config';

export async function GET(context) {
	const [essays, code, euler] = await Promise.all([
		getPublishedEntries('essays'),
		getPublishedEntries('code'),
		getEulerRecords(),
	]);

	const datedEssays = essays.map((entry) => ({
		title: entry.data.title,
		description: entry.data.description,
		pubDate: entry.data.published,
		link: withBase(`/essays/${entry.id}`),
	}));

	const datedCode = code.map((entry) => ({
		title: entry.data.title,
		description: entry.data.description,
		pubDate: entry.data.published,
		link: withBase(`/code/${entry.id}`),
	}));

	const datedEuler = euler.map((entry) => ({
		title: `${entry.title} (${entry.id})`,
		description: entry.description,
		pubDate: entry.updated,
		link: withBase(`/project-euler/${entry.id}`),
	}));

	const items = [...datedEssays, ...datedCode, ...datedEuler].sort(
		(a, b) => b.pubDate.valueOf() - a.pubDate.valueOf(),
	);

	return rss({
		title: siteConfig.title,
		description: siteConfig.description,
		site: new URL(import.meta.env.BASE_URL, context.site),
		items,
	});
}
