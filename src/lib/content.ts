import { getCollection, type CollectionEntry } from 'astro:content';

type CollectionName = 'essays' | 'code';

type DatedEntry<C extends CollectionName> = CollectionEntry<C>;

const byPublishedDesc = <C extends CollectionName>(
	a: DatedEntry<C>,
	b: DatedEntry<C>,
) => b.data.published.valueOf() - a.data.published.valueOf();

export function getPublishedEntries(
	collection: 'essays',
): Promise<CollectionEntry<'essays'>[]>;
export function getPublishedEntries(collection: 'code'): Promise<CollectionEntry<'code'>[]>;
export async function getPublishedEntries(collection: CollectionName) {
	const entries = await getCollection(collection);
	return entries.filter((entry) => !entry.data.draft).sort(byPublishedDesc);
}

export function pickFeatured<C extends CollectionName>(entries: DatedEntry<C>[]) {
	return entries.find((entry) => entry.data.featured) ?? entries[0];
}

export function getUniqueTags<C extends CollectionName>(entries: DatedEntry<C>[]) {
	return [...new Set(entries.flatMap((entry) => entry.data.tags))].sort((a, b) =>
		a.localeCompare(b),
	);
}

export function formatDate(date: Date) {
	return new Intl.DateTimeFormat('en', {
		year: 'numeric',
		month: 'short',
		day: 'numeric',
	}).format(date);
}

export function formatProblemNumber(problem: number) {
	return `#${String(problem).padStart(4, '0')}`;
}

export function getEulerProblemUrl(problem: number) {
	return `https://projecteuler.net/problem=${problem}`;
}
