import { promises as fs } from 'node:fs';
import path from 'node:path';

import { getCollection, type CollectionEntry } from 'astro:content';

const EULER_ROOT = path.resolve(process.cwd(), 'project_euler_unified');
const SOURCE_FILE_NAMES = {
	cpp: 'solution.cpp',
	python: 'solution.py',
} as const;

// The unified local archive does not carry Project Euler's live per-problem ratings.
// We mirror the public "Level NN" presentation by assigning one level per 25 solved
// problems after sorting the archive by problem number.
const EULER_LEVEL_GROUP_SIZE = 25;

export interface EulerRecord {
	id: string;
	problem: number;
	title: string;
	description: string;
	answer?: string;
	updated: Date;
	wordCount: number;
	difficultyLevel: number;
	difficultyLabel: string;
	difficultyColor: string;
	difficultyInk: string;
	languages: ['C++', 'Python'];
	entry: CollectionEntry<'euler'>;
	sourcePaths: {
		cpp: string;
		python: string;
	};
}

export interface EulerSourceBundle {
	cpp: string;
	python: string;
}

let recordsPromise: Promise<EulerRecord[]> | undefined;
const sourcePromiseCache = new Map<number, Promise<EulerSourceBundle>>();

// Record shape before derived level metadata is attached in `loadEulerRecords`.
interface BaseEulerRecord
	extends Omit<
		EulerRecord,
		'difficultyLevel' | 'difficultyLabel' | 'difficultyColor' | 'difficultyInk'
	> {}

export function getEulerProblemNumberFromSlug(slug: string) {
	const match = slug.match(/problem-(\d+)/);
	if (!match) {
		throw new Error(`Invalid Project Euler slug: ${slug}`);
	}

	return Number(match[1]);
}

export function getEulerProblemDirectory(problem: number) {
	return path.join(EULER_ROOT, `problem_${String(problem).padStart(3, '0')}`);
}

export function getEulerProblemUrl(problem: number) {
	return `https://projecteuler.net/problem=${problem}`;
}

export function formatProblemNumber(problem: number) {
	return `#${String(problem).padStart(4, '0')}`;
}

// Match the Level 00, Level 01, ... label format used across the archive UI.
export function formatEulerDifficultyLevel(level: number) {
	return `Level ${String(level).padStart(2, '0')}`;
}

// Interpolate from green toward light red so low levels stay cool and higher levels
// read as more demanding without using the original warmup/medium/hard buckets.
export function getEulerDifficultyColor(level: number, maxLevel: number) {
	const ratio = maxLevel <= 0 ? 0 : level / maxLevel;
	const hue = 112 - Math.round(ratio * 104);
	const saturation = 50 + Math.round(ratio * 12);
	const lightness = 82 - Math.round(ratio * 5);

	return `hsl(${hue} ${saturation}% ${lightness}%)`;
}

// Use a darker companion ink value so level badges remain legible across the ramp.
export function getEulerDifficultyInk(level: number, maxLevel: number) {
	const ratio = maxLevel <= 0 ? 0 : level / maxLevel;
	const hue = 112 - Math.round(ratio * 104);
	const saturation = 38 + Math.round(ratio * 24);
	const lightness = 22 + Math.round(ratio * 8);

	return `hsl(${hue} ${saturation}% ${lightness}%)`;
}

export async function getEulerRecords() {
	if (!recordsPromise) {
		recordsPromise = loadEulerRecords();
	}

	return recordsPromise;
}

export async function getEulerRecord(slug: string) {
	const records = await getEulerRecords();
	return records.find((record) => record.id === slug);
}

export async function getEulerSources(problem: number) {
	const cached = sourcePromiseCache.get(problem);
	if (cached) {
		return cached;
	}

	const promise = readEulerSources(problem);
	sourcePromiseCache.set(problem, promise);
	return promise;
}

function escapeRegExp(value: string) {
	return value.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
}

function extractSection(body: string, heading: string) {
	const expression = new RegExp(
		`^## ${escapeRegExp(heading)}\\s*$([\\s\\S]*?)(?=^##\\s+|\\Z)`,
		'm',
	);

	return expression.exec(body)?.[1]?.trim() ?? '';
}

function stripMarkdown(value: string) {
	return value
		.replace(/```[\s\S]*?```/g, ' ')
		.replace(/\$\$[\s\S]*?\$\$/g, ' ')
		.replace(/`([^`]+)`/g, '$1')
		.replace(/!\[[^\]]*\]\([^)]+\)/g, ' ')
		.replace(/\[([^\]]+)\]\([^)]+\)/g, '$1')
		.replace(/[*_>#-]/g, ' ')
		.replace(/\s+/g, ' ')
		.trim();
}

function clampText(value: string, maxLength: number) {
	if (value.length <= maxLength) {
		return value;
	}

	return `${value.slice(0, maxLength - 3).trimEnd()}...`;
}

function extractTitle(body: string, fallbackProblem: number) {
	const match = body.match(/^#\s+Problem\s+\d+\s*:\s*(.+)$/m) ?? body.match(/^#\s+(.+)$/m);
	return match?.[1]?.trim() ?? `Problem ${fallbackProblem}`;
}

function extractDescription(body: string) {
	const statement = stripMarkdown(extractSection(body, 'Problem Statement'));
	if (statement) {
		return clampText(statement, 200);
	}

	const firstParagraph = stripMarkdown(body.split(/\n\s*\n/)[0] ?? '');
	return clampText(firstParagraph, 200);
}

function extractAnswer(body: string) {
	const answerSection = extractSection(body, 'Answer');
	if (!answerSection) {
		return undefined;
	}

	const boxed = answerSection.match(/\\boxed\{([^}]+)\}/);
	if (boxed?.[1]) {
		return boxed[1].replace(/\s+/g, ' ').trim();
	}

	const plain = stripMarkdown(answerSection);
	return plain || undefined;
}

function countWords(body: string) {
	const text = stripMarkdown(body);
	if (!text) {
		return 0;
	}

	return text.split(/\s+/).filter(Boolean).length;
}

async function loadEulerRecords() {
	const entries = await getCollection('euler');

	const baseRecords = await Promise.all(
		entries.map(async (entry) => {
			const problem = getEulerProblemNumberFromSlug(entry.id);
			const body = entry.body ?? '';
			const directory = getEulerProblemDirectory(problem);
			const solutionPath = path.join(directory, 'solution.md');
			const stats = await fs.stat(solutionPath);

			return {
				id: entry.id,
				problem,
				title: extractTitle(body, problem),
				description: extractDescription(body),
				answer: extractAnswer(body),
				updated: stats.mtime,
				wordCount: countWords(body),
				languages: ['C++', 'Python'],
				entry,
				sourcePaths: {
					cpp: path.relative(process.cwd(), path.join(directory, SOURCE_FILE_NAMES.cpp)),
					python: path.relative(process.cwd(), path.join(directory, SOURCE_FILE_NAMES.python)),
				},
			} satisfies BaseEulerRecord;
		}),
	);

	const sortedRecords = baseRecords.sort((left, right) => left.problem - right.problem);
	// With 988 local entries this yields Level 00 through Level 39.
	const maxLevel = Math.floor(Math.max(sortedRecords.length - 1, 0) / EULER_LEVEL_GROUP_SIZE);

	return sortedRecords.map((record, index) => {
		const difficultyLevel = Math.min(maxLevel, Math.floor(index / EULER_LEVEL_GROUP_SIZE));

		return {
			...record,
			difficultyLevel,
			difficultyLabel: formatEulerDifficultyLevel(difficultyLevel),
			difficultyColor: getEulerDifficultyColor(difficultyLevel, maxLevel),
			difficultyInk: getEulerDifficultyInk(difficultyLevel, maxLevel),
		} satisfies EulerRecord;
	});
}

async function readEulerSources(problem: number) {
	const directory = getEulerProblemDirectory(problem);
	const [cpp, python] = await Promise.all([
		fs.readFile(path.join(directory, SOURCE_FILE_NAMES.cpp), 'utf-8'),
		fs.readFile(path.join(directory, SOURCE_FILE_NAMES.python), 'utf-8'),
	]);

	return { cpp, python };
}
