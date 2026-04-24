import { promises as fs } from 'node:fs';
import path from 'node:path';

const COMPETITIVE_PROGRAMMING_ROOT = path.resolve(process.cwd(), 'competitive_programming');
const COMPETITIVE_PROGRAMMING_ASSET_PREFIX = '/competitive-programming-assets/';

const TRACK_CONFIG = {
	icpc: {
		label: 'ICPC',
		longLabel: 'ICPC World Finals',
		root: path.join(COMPETITIVE_PROGRAMMING_ROOT, 'icpc'),
	},
	ioi: {
		label: 'IOI',
		longLabel: 'International Olympiad in Informatics',
		root: path.join(COMPETITIVE_PROGRAMMING_ROOT, 'ioi'),
	},
} as const;

export type CompetitiveProgrammingTrack = keyof typeof TRACK_CONFIG;

interface IcpcMeta {
	year: string;
	letter: string;
	title: string;
	slug: string;
	source_pdf?: string;
	page_start?: number;
	page_end?: number;
}

export interface CompetitiveProgrammingRecord {
	id: string;
	track: CompetitiveProgrammingTrack;
	trackLabel: string;
	trackLongLabel: string;
	year: number;
	slug: string;
	folderName: string;
	title: string;
	displayTitle: string;
	description: string;
	updated: Date;
	hasStatementText: boolean;
	hasStatementPdf: boolean;
	statementPreview?: string;
	letter?: string;
	sourceDirectory: string;
	sourcePaths: {
		tex: string;
		cpp: string;
		statementText?: string;
		statementPdf?: string;
		meta?: string;
		yearPacket?: string;
	};
	assetUrls: {
		tex: string;
		cpp: string;
		statementText?: string;
		statementPdf?: string;
		meta?: string;
		yearPacket?: string;
	};
}

export interface CompetitiveProgrammingSourceBundle {
	tex: string;
	cpp: string;
	statementText?: string;
}

export interface CompetitiveProgrammingTrackSummary {
	track: CompetitiveProgrammingTrack;
	label: string;
	longLabel: string;
	entryCount: number;
	yearCount: number;
	latestUpdated?: Date;
	statementTextCount: number;
	statementPdfCount: number;
}

export interface CompetitiveProgrammingYearGroup {
	track: CompetitiveProgrammingTrack;
	year: number;
	entryCount: number;
	packetUrl?: string;
	statementTextCount: number;
	statementPdfCount: number;
	latestUpdated?: Date;
	records: CompetitiveProgrammingRecord[];
}

let recordsPromise: Promise<CompetitiveProgrammingRecord[]> | undefined;
const sourcePromiseCache = new Map<string, Promise<CompetitiveProgrammingSourceBundle>>();

function escapeRegExp(value: string) {
	return value.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
}

function normalizeSlug(value: string) {
	return value
		.trim()
		.toLowerCase()
		.replace(/[_\s]+/g, '-')
		.replace(/[^a-z0-9-]/g, '-')
		.replace(/-+/g, '-')
		.replace(/^-|-$/g, '');
}

function humanizeFolderName(value: string) {
	return value
		.replace(/[_-]+/g, ' ')
		.replace(/\b[a-z]/g, (match) => match.toUpperCase());
}

function clampText(value: string, maxLength: number) {
	if (value.length <= maxLength) {
		return value;
	}

	return `${value.slice(0, maxLength - 3).trimEnd()}...`;
}

function stripLatex(value: string) {
	let text = value
		.replace(/(^|[^\\])%.*$/gm, '$1')
		.replace(/\\label\{[^}]*\}/g, ' ')
		.replace(/\\(?:cite|eqref|autoref)\{[^}]*\}/g, ' ')
		.replace(/\\begin\{[^}]+\}/g, ' ')
		.replace(/\\end\{[^}]+\}/g, ' ')
		.replace(/\\item\b/g, ' ')
		.replace(/\$\$([\s\S]*?)\$\$/g, ' $1 ')
		.replace(/\$([^$\n]+)\$/g, ' $1 ')
		.replace(/\\\[((?:.|\n)*?)\\\]/g, ' $1 ')
		.replace(/\\\(((?:.|\n)*?)\\\)/g, ' $1 ')
		.replace(/\\\\/g, ' ')
		.replace(/~+/g, ' ')
		.replace(/\\(?:qquad|quad|,|;|:|!)/g, ' ');

	for (let index = 0; index < 6; index += 1) {
		text = text.replace(/\\[A-Za-z@*]+(?:\[[^\]]*\])?\{([^{}]*)\}/g, '$1');
	}

	return text
		.replace(/\\[A-Za-z@*]+/g, ' ')
		.replace(/[{}]/g, ' ')
		.replace(/\s+([,.;:!?])/g, '$1')
		.replace(/\(\s+/g, '(')
		.replace(/\s+\)/g, ')')
		.replace(/\s+/g, ' ')
		.trim();
}

function extractTexTitle(texSource: string) {
	const titleMatch = texSource.match(/\\title\{([\s\S]*?)\}/);
	if (!titleMatch?.[1]) {
		return '';
	}

	return stripLatex(titleMatch[1]);
}

function extractTexSection(texSource: string, sectionNames: string[]) {
	const pattern = /\\section\*?\{([^}]*)\}/g;
	const matches = [...texSource.matchAll(pattern)];

	for (let index = 0; index < matches.length; index += 1) {
		const match = matches[index];
		const sectionName = match[1]?.trim();
		if (!sectionName || !sectionNames.includes(sectionName)) {
			continue;
		}

		const start = (match.index ?? 0) + match[0].length;
		const next = matches[index + 1];
		const end = next?.index ?? texSource.length;
		return texSource.slice(start, end).trim();
	}

	return '';
}

function extractDescriptionFromTex(texSource: string, fallbackTitle: string) {
	const sectionText =
		extractTexSection(texSource, ['Problem Summary', 'Summary', 'Overview']) ||
		extractTexSection(texSource, ['Solution', 'Idea']);
	const plain = stripLatex(sectionText);

	if (plain.length >= 24) {
		return clampText(plain, 220);
	}

	const documentBody = texSource
		.replace(/^[\s\S]*?\\begin\{document\}/, '')
		.replace(/\\end\{document\}[\s\S]*$/, '');
	const fallback = stripLatex(documentBody);
	if (fallback.length >= 24) {
		return clampText(fallback, 220);
	}

	return fallbackTitle;
}

function extractStatementPreview(statementText: string) {
	const lines = statementText
		.split(/\r?\n/)
		.map((line) => line.trim())
		.filter(
			(line) =>
				Boolean(line) &&
				!/^This page is intentionally left blank\.?$/i.test(line) &&
				!/ICPC Foundation/i.test(line),
		);

	while (lines.length && /^Problem\b/i.test(lines[0] ?? '')) {
		lines.shift();
	}

	while (lines.length && /^Time\s+limit:/i.test(lines[0] ?? '')) {
		lines.shift();
	}

	if (lines.length && lines[0]!.length <= 80 && lines[0]!.split(/\s+/).length <= 8) {
		lines.shift();
	}

	while (lines.length && /^Time\s+limit:/i.test(lines[0] ?? '')) {
		lines.shift();
	}

	const normalized = lines.join(' ');

	return normalized ? clampText(normalized, 260) : undefined;
}

function getRouteSlug(track: CompetitiveProgrammingTrack, folderName: string, letter?: string) {
	if (track === 'icpc' && letter) {
		return normalizeSlug(`${letter}-${folderName.replace(/^[A-Z]-/, '')}`);
	}

	return normalizeSlug(folderName);
}

function getRelativePath(absolutePath: string) {
	return path.relative(process.cwd(), absolutePath);
}

function toAssetUrl(relativePath: string) {
	const normalized = relativePath.replace(/^competitive_programming[\\/]/, '').replaceAll('\\', '/');
	return `${COMPETITIVE_PROGRAMMING_ASSET_PREFIX}${normalized}`;
}

function getMaxDate(dates: Date[]) {
	return dates.reduce((latest, current) => (current > latest ? current : latest), dates[0]);
}

function sortTrackRecords(left: CompetitiveProgrammingRecord, right: CompetitiveProgrammingRecord) {
	if (left.year !== right.year) {
		return right.year - left.year;
	}

	if (left.track === 'icpc' && right.track === 'icpc' && left.letter && right.letter) {
		return left.letter.localeCompare(right.letter);
	}

	return left.title.localeCompare(right.title);
}

async function pathExists(targetPath: string) {
	return fs
		.access(targetPath)
		.then(() => true)
		.catch(() => false);
}

async function loadIcpcRecord(year: number, folderName: string) {
	const baseDirectory = path.join(TRACK_CONFIG.icpc.root, String(year), folderName);
	const [metaRaw, texSource, statementTextExists, statementPdfExists, yearPacketExists] = await Promise.all([
		fs.readFile(path.join(baseDirectory, 'meta.json'), 'utf8'),
		fs.readFile(path.join(baseDirectory, 'solution.tex'), 'utf8'),
		pathExists(path.join(baseDirectory, 'statement.txt')),
		pathExists(path.join(baseDirectory, 'statement.pdf')),
		pathExists(path.join(TRACK_CONFIG.icpc.root, String(year), 'contest_problems.pdf')),
	]);
	const meta = JSON.parse(metaRaw) as IcpcMeta;
	const statementTextPath = path.join(baseDirectory, 'statement.txt');
	const statementText = statementTextExists ? await fs.readFile(statementTextPath, 'utf8') : undefined;
	const directoryEntries = await fs.readdir(baseDirectory);
	const stats = await Promise.all(
		directoryEntries.map((entry) => fs.stat(path.join(baseDirectory, entry))),
	);
	const updated = getMaxDate(stats.map((stat) => stat.mtime));
	const texPath = getRelativePath(path.join(baseDirectory, 'solution.tex'));
	const cppPath = getRelativePath(path.join(baseDirectory, 'solution.cpp'));
	const statementPdfPath = statementPdfExists
		? getRelativePath(path.join(baseDirectory, 'statement.pdf'))
		: undefined;
	const yearPacketPath = yearPacketExists
		? getRelativePath(path.join(TRACK_CONFIG.icpc.root, String(year), 'contest_problems.pdf'))
		: undefined;

	return {
		id: `icpc-${year}-${getRouteSlug('icpc', folderName, meta.letter)}`,
		track: 'icpc',
		trackLabel: TRACK_CONFIG.icpc.label,
		trackLongLabel: TRACK_CONFIG.icpc.longLabel,
		year,
		slug: getRouteSlug('icpc', folderName, meta.letter),
		folderName,
		title: meta.title,
		displayTitle: `${meta.letter}. ${meta.title}`,
		description: extractDescriptionFromTex(texSource, meta.title),
		updated,
		hasStatementText: statementTextExists,
		hasStatementPdf: statementPdfExists,
		statementPreview: statementText ? extractStatementPreview(statementText) : undefined,
		letter: meta.letter,
		sourceDirectory: getRelativePath(baseDirectory),
		sourcePaths: {
			tex: texPath,
			cpp: cppPath,
			statementText: statementTextExists ? getRelativePath(statementTextPath) : undefined,
			statementPdf: statementPdfPath,
			meta: getRelativePath(path.join(baseDirectory, 'meta.json')),
			yearPacket: yearPacketPath,
		},
		assetUrls: {
			tex: toAssetUrl(texPath),
			cpp: toAssetUrl(cppPath),
			statementText: statementTextExists ? toAssetUrl(getRelativePath(statementTextPath)) : undefined,
			statementPdf: statementPdfPath ? toAssetUrl(statementPdfPath) : undefined,
			meta: toAssetUrl(getRelativePath(path.join(baseDirectory, 'meta.json'))),
			yearPacket: yearPacketPath ? toAssetUrl(yearPacketPath) : undefined,
		},
	} satisfies CompetitiveProgrammingRecord;
}

function cleanIoiTitle(rawTitle: string, year: number, folderName: string) {
	if (!rawTitle) {
		return humanizeFolderName(folderName);
	}

	const stripped = rawTitle
		.replace(new RegExp(`^${escapeRegExp(`IOI ${year}`)}\\s*(?:(?:--|—|–|:|-)+\\s*)?`, 'i'), '')
		.replace(/^International Olympiad in Informatics\s*/i, '')
		.replace(/^[\s:–—-]+/, '')
		.trim();

	if (stripped) {
		const normalized = stripped
			.replace(/^Problem\s+\d+\s*[:.-]\s*/i, '')
			.replace(/^Task\s+\d+\s*[:.-]\s*/i, '')
			.trim();

		if (normalized) {
			return normalized;
		}
	}

	return humanizeFolderName(folderName);
}

async function loadIoiRecord(year: number, folderName: string) {
	const baseDirectory = path.join(TRACK_CONFIG.ioi.root, String(year), folderName);
	const [texSource, directoryEntries] = await Promise.all([
		fs.readFile(path.join(baseDirectory, 'solution.tex'), 'utf8'),
		fs.readdir(baseDirectory),
	]);
	const stats = await Promise.all(
		directoryEntries.map((entry) => fs.stat(path.join(baseDirectory, entry))),
	);
	const updated = getMaxDate(stats.map((stat) => stat.mtime));
	const texPath = getRelativePath(path.join(baseDirectory, 'solution.tex'));
	const cppPath = getRelativePath(path.join(baseDirectory, 'solution.cpp'));
	const title = cleanIoiTitle(extractTexTitle(texSource), year, folderName);

	return {
		id: `ioi-${year}-${getRouteSlug('ioi', folderName)}`,
		track: 'ioi',
		trackLabel: TRACK_CONFIG.ioi.label,
		trackLongLabel: TRACK_CONFIG.ioi.longLabel,
		year,
		slug: getRouteSlug('ioi', folderName),
		folderName,
		title,
		displayTitle: title,
		description: extractDescriptionFromTex(texSource, title),
		updated,
		hasStatementText: false,
		hasStatementPdf: false,
		sourceDirectory: getRelativePath(baseDirectory),
		sourcePaths: {
			tex: texPath,
			cpp: cppPath,
		},
		assetUrls: {
			tex: toAssetUrl(texPath),
			cpp: toAssetUrl(cppPath),
		},
	} satisfies CompetitiveProgrammingRecord;
}

async function loadTrackRecords(track: CompetitiveProgrammingTrack) {
	const yearEntries = await fs.readdir(TRACK_CONFIG[track].root, { withFileTypes: true });
	const years = yearEntries
		.filter((entry) => entry.isDirectory() && /^\d{4}$/.test(entry.name))
		.map((entry) => Number(entry.name))
		.sort((left, right) => left - right);

	const records = await Promise.all(
		years.flatMap(async (year) => {
			const yearRoot = path.join(TRACK_CONFIG[track].root, String(year));
			const problemEntries = await fs.readdir(yearRoot, { withFileTypes: true });
			const folderNames = problemEntries
				.filter((entry) => entry.isDirectory())
				.map((entry) => entry.name)
				.sort((left, right) => left.localeCompare(right));

			return Promise.all(
				folderNames.map((folderName) =>
					track === 'icpc' ? loadIcpcRecord(year, folderName) : loadIoiRecord(year, folderName),
				),
			);
		}),
	);

	return records.flat();
}

async function loadCompetitiveProgrammingRecords() {
	const [icpcRecords, ioiRecords] = await Promise.all([loadTrackRecords('icpc'), loadTrackRecords('ioi')]);
	return [...icpcRecords, ...ioiRecords];
}

export async function getCompetitiveProgrammingRecords(track?: CompetitiveProgrammingTrack) {
	if (!recordsPromise) {
		recordsPromise = loadCompetitiveProgrammingRecords();
	}

	const records = await recordsPromise;
	const filtered = track ? records.filter((record) => record.track === track) : records;
	return [...filtered].sort(sortTrackRecords);
}

export async function getCompetitiveProgrammingRecord(
	track: CompetitiveProgrammingTrack,
	year: number,
	slug: string,
) {
	const records = await getCompetitiveProgrammingRecords(track);
	return records.find((record) => record.year === year && record.slug === slug);
}

export async function getCompetitiveProgrammingSources(record: CompetitiveProgrammingRecord) {
	const cacheKey = record.id;
	const cached = sourcePromiseCache.get(cacheKey);
	if (cached) {
		return cached;
	}

	const promise = Promise.all([
		fs.readFile(path.resolve(process.cwd(), record.sourcePaths.tex), 'utf8'),
		fs.readFile(path.resolve(process.cwd(), record.sourcePaths.cpp), 'utf8'),
		record.sourcePaths.statementText
			? fs.readFile(path.resolve(process.cwd(), record.sourcePaths.statementText), 'utf8')
			: Promise.resolve(undefined),
	]).then(([tex, cpp, statementText]) => ({ tex, cpp, statementText }));

	sourcePromiseCache.set(cacheKey, promise);
	return promise;
}

export async function getCompetitiveProgrammingTrackSummary(
	track: CompetitiveProgrammingTrack,
): Promise<CompetitiveProgrammingTrackSummary> {
	const records = await getCompetitiveProgrammingRecords(track);
	const latestUpdated = [...records].sort((left, right) => right.updated.getTime() - left.updated.getTime())[0]
		?.updated;

	return {
		track,
		label: TRACK_CONFIG[track].label,
		longLabel: TRACK_CONFIG[track].longLabel,
		entryCount: records.length,
		yearCount: new Set(records.map((record) => record.year)).size,
		latestUpdated,
		statementTextCount: records.filter((record) => record.hasStatementText).length,
		statementPdfCount: records.filter((record) => record.hasStatementPdf).length,
	};
}

export async function getCompetitiveProgrammingTrackSummaries() {
	return Promise.all([
		getCompetitiveProgrammingTrackSummary('icpc'),
		getCompetitiveProgrammingTrackSummary('ioi'),
	]);
}

export async function getCompetitiveProgrammingYearGroups(track: CompetitiveProgrammingTrack) {
	const records = await getCompetitiveProgrammingRecords(track);
	const groups = new Map<number, CompetitiveProgrammingRecord[]>();

	for (const record of records) {
		const current = groups.get(record.year) ?? [];
		current.push(record);
		groups.set(record.year, current);
	}

	return [...groups.entries()]
		.sort((left, right) => right[0] - left[0])
		.map(([year, yearRecords]) => ({
			track,
			year,
			entryCount: yearRecords.length,
			packetUrl: track === 'icpc' ? yearRecords[0]?.assetUrls.yearPacket : undefined,
			statementTextCount: yearRecords.filter((record) => record.hasStatementText).length,
			statementPdfCount: yearRecords.filter((record) => record.hasStatementPdf).length,
			latestUpdated: [...yearRecords].sort((left, right) => right.updated.getTime() - left.updated.getTime())[0]
				?.updated,
			records: yearRecords,
		})) satisfies CompetitiveProgrammingYearGroup[];
}

export async function getCompetitiveProgrammingYearGroup(
	track: CompetitiveProgrammingTrack,
	year: number,
) {
	const groups = await getCompetitiveProgrammingYearGroups(track);
	return groups.find((group) => group.year === year);
}

export async function getCompetitiveProgrammingAssetMap() {
	const records = await getCompetitiveProgrammingRecords();
	const assets = new Map<
		string,
		{
			absolutePath: string;
			contentType: string;
		}
	>();

	const getContentType = (relativePath: string) => {
		if (relativePath.endsWith('.pdf')) return 'application/pdf';
		if (relativePath.endsWith('.json')) return 'application/json; charset=utf-8';
		if (relativePath.endsWith('.txt') || relativePath.endsWith('.tex') || relativePath.endsWith('.cpp')) {
			return 'text/plain; charset=utf-8';
		}

		return 'application/octet-stream';
	};

	for (const record of records) {
		for (const relativePath of Object.values(record.sourcePaths)) {
			if (!relativePath) {
				continue;
			}

			const assetKey = relativePath
				.replace(/^competitive_programming[\\/]/, '')
				.replaceAll('\\', '/');
			if (!assets.has(assetKey)) {
				assets.set(assetKey, {
					absolutePath: path.resolve(process.cwd(), relativePath),
					contentType: getContentType(relativePath),
				});
			}
		}
	}

	return assets;
}
