import { promises as fs } from 'node:fs';
import path from 'node:path';

import { getCollection, type CollectionEntry } from 'astro:content';

import eulerSolvedCountsData from '../data/euler-solved-counts.json';

const EULER_ROOT = path.resolve(process.cwd(), 'project_euler_unified');
const SOURCE_FILE_NAMES = {
	cpp: 'solution.cpp',
	python: 'solution.py',
} as const;

interface EulerSolvedCountSnapshot {
	source: string;
	capturedAt: string;
	counts: Record<string, number>;
}

const EULER_SOLVED_COUNT_SNAPSHOT = eulerSolvedCountsData as EulerSolvedCountSnapshot;
const EULER_SOLVED_COUNTS = new Map(
	Object.entries(EULER_SOLVED_COUNT_SNAPSHOT.counts).map(([problem, solvedBy]) => [
		Number(problem),
		Number(solvedBy),
	]),
);

// Project Euler's public pages now present difficulty as "Level NN" bands.
// The public archive HTML still exposes solve counts rather than direct levels,
// so we mirror the official style by ranking local entries by public solve counts
// and then grouping every 25 ranked problems into one level band.
const EULER_LEVEL_GROUP_SIZE = 25;

export interface EulerRecord {
	id: string;
	problem: number;
	title: string;
	description: string;
	answer?: string;
	updated: Date;
	wordCount: number;
	solvedByCount?: number;
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

export function formatEulerSolvedByCount(solvedByCount: number) {
	return solvedByCount.toLocaleString('en-US');
}

export function getEulerDifficultySnapshotDate() {
	return new Date(EULER_SOLVED_COUNT_SNAPSHOT.capturedAt);
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
	const headingExpression = new RegExp(`^## ${escapeRegExp(heading)}\\s*$`, 'm');
	const headingMatch = headingExpression.exec(body);
	if (!headingMatch) {
		return '';
	}

	const remainder = body.slice((headingMatch.index ?? 0) + headingMatch[0].length);
	const nextHeadingMatch = /^##\s+/m.exec(remainder);
	const section = nextHeadingMatch ? remainder.slice(0, nextHeadingMatch.index) : remainder;

	return section.trim();
}

function formatMathScript(prefix: '^' | '_', content: string) {
	const trimmed = content.trim();
	if (!trimmed) {
		return '';
	}

	return /^[A-Za-z0-9]+$/.test(trimmed) ? `${prefix}${trimmed}` : `${prefix}(${trimmed})`;
}

function simplifyInlineMath(value: string) {
	const leftBraceToken = '__EULER_LBRACE__';
	const rightBraceToken = '__EULER_RBRACE__';

	return value
		.replace(/\\\{/g, leftBraceToken)
		.replace(/\\\}/g, rightBraceToken)
		.replace(/\\frac\{([^{}]+)\}\{([^{}]+)\}/g, '($1)/($2)')
		.replace(/\\binom\{([^{}]+)\}\{([^{}]+)\}/g, 'C($1, $2)')
		.replace(/\\sqrt\{([^{}]+)\}/g, 'sqrt($1)')
		.replace(/(\d)\\,(\d)/g, '$1,$2')
		.replace(/\\(?:displaystyle|textstyle|scriptstyle|scriptscriptstyle)\b/g, '')
		.replace(/\\(?:left|right|bigl|bigr|Bigl|Bigr|big|Big)\b/g, '')
		.replace(/\\pmod\{([^{}]+)\}/g, '(mod $1)')
		.replace(/\\pmod\s*([A-Za-z0-9]+)/g, '(mod $1)')
		.replace(/\\operatorname\*?\{([^}]*)\}/g, '$1')
		.replace(/\\text\{([^}]*)\}/g, '$1')
		.replace(/\\mathbb\{([^}]*)\}/g, '$1')
		.replace(/\\mathcal\{([^}]*)\}/g, '$1')
		.replace(/\\mathrm\{([^}]*)\}/g, '$1')
		.replace(/\\mathbf\{([^}]*)\}/g, '$1')
		.replace(/\\mathsf\{([^}]*)\}/g, '$1')
		.replace(/\\mathtt\{([^}]*)\}/g, '$1')
		.replace(/\\mathscr\{([^}]*)\}/g, '$1')
		.replace(/\\substack\{([\s\S]*?)\}/g, (_match, content: string) =>
			content.replace(/\\\\/g, ', ').replace(/\s+/g, ' ').trim(),
		)
		.replace(/\\(?:cdots|ldots|dots)\b/g, '...')
		.replace(/\\cdot\b/g, '*')
		.replace(/\\times\b/g, 'x')
		.replace(/\\div\b/g, '/')
		.replace(/\\pm\b/g, '+/-')
		.replace(/\\mp\b/g, '-/+')
		.replace(/\\oplus\b/g, '+')
		.replace(/\\otimes\b/g, 'x')
		.replace(/\\geqslant\b|\\geq\b|\\ge\b/g, '>=')
		.replace(/\\leqslant\b|\\leq\b|\\le\b/g, '<=')
		.replace(/\\neq\b/g, '!=')
		.replace(/\\equiv\b/g, 'equiv')
		.replace(/\\approx\b/g, 'approx')
		.replace(/\\sim\b/g, '~')
		.replace(/\\to\b|\\rightarrow\b|\\mapsto\b/g, '->')
		.replace(/\\mid\b/g, '|')
		.replace(/\\notin\b/g, 'not in')
		.replace(/\\in\b/g, 'in')
		.replace(/\\cup\b/g, 'union')
		.replace(/\\cap\b/g, 'intersection')
		.replace(/\\subseteq\b/g, 'subseteq')
		.replace(/\\subset\b/g, 'subset')
		.replace(/\\bmod\b|\\mod\b/g, 'mod')
		.replace(/\\infty\b/g, 'infinity')
		.replace(/\\sum\b/g, 'sum')
		.replace(/\\prod\b/g, 'prod')
		.replace(/\\gcd\b/g, 'gcd')
		.replace(/\\log\b/g, 'log')
		.replace(/\\ln\b/g, 'ln')
		.replace(/\\max\b/g, 'max')
		.replace(/\\min\b/g, 'min')
		.replace(/\\lfloor\b/g, 'floor(')
		.replace(/\\rfloor\b/g, ')')
		.replace(/\\lceil\b/g, 'ceil(')
		.replace(/\\rceil\b/g, ')')
		.replace(/\\lvert\b|\\rvert\b|\\vert\b/g, '|')
		.replace(/\\quad\b|\\qquad\b|\\,|\\;|\\:|\\!/g, ' ')
		.replace(/\\\\/g, ' ')
		.replace(/\\(?:begin|end)\{[^}]+\}/g, ' ')
		.replace(/\^\{([^{}]+)\}/g, (_match, content: string) => formatMathScript('^', content))
		.replace(/_\{([^{}]+)\}/g, (_match, content: string) => formatMathScript('_', content))
		.replace(/\\([_%#$&])/g, '$1')
		.replace(/\\([A-Za-z]+)/g, '$1')
		.replace(/\\([{}])/g, '$1')
		.replace(/[{}]/g, '')
		.replace(new RegExp(leftBraceToken, 'g'), '{')
		.replace(new RegExp(rightBraceToken, 'g'), '}')
		.replace(/\s+([,.;:!?])/g, '$1')
		.replace(/([([{])\s+/g, '$1')
		.replace(/\s+([)\]}])/g, '$1')
		.replace(/\s+/g, ' ')
		.trim();
}

function stripMarkdown(value: string, options?: { preserveInlineMath?: boolean }) {
	const preserveInlineMath = options?.preserveInlineMath ?? false;
	const inlineMathReplacement = (_match: string, math: string) =>
		preserveInlineMath ? ` ${simplifyInlineMath(math)} ` : ' ';

	return value
		.replace(/```[\s\S]*?```/g, ' ')
		.replace(/\$\$([\s\S]*?)\$\$/g, inlineMathReplacement)
		.replace(/\\\[([\s\S]*?)\\\]/g, inlineMathReplacement)
		.replace(/\$([^$\n]+)\$/g, inlineMathReplacement)
		.replace(/\\\(([\s\S]*?)\\\)/g, inlineMathReplacement)
		.replace(/`([^`]+)`/g, '$1')
		.replace(/!\[[^\]]*\]\([^)]+\)/g, ' ')
		.replace(/\[([^\]]+)\]\([^)]+\)/g, '$1')
		.replace(/^\s{0,3}#{1,6}\s+/gm, '')
		.replace(/^\s{0,3}>\s?/gm, '')
		.replace(/^\s{0,3}[-+*]\s+/gm, '')
		.replace(/\*\*([^*]+)\*\*/g, '$1')
		.replace(/\*([^*]+)\*/g, '$1')
		.replace(/\s+([,.;:!?])/g, '$1')
		.replace(/([([{])\s+/g, '$1')
		.replace(/\s+([)\]}])/g, '$1')
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
	const problemStatement = extractSection(body, 'Problem Statement');
	const statementWithMath = stripMarkdown(problemStatement, { preserveInlineMath: true });
	if (statementWithMath.length >= 32) {
		return clampText(statementWithMath, 200);
	}

	const statement = stripMarkdown(problemStatement);
	if (statement.length >= 32) {
		return clampText(statement, 200);
	}

	if (statementWithMath) {
		return clampText(statementWithMath, 200);
	}

	const firstParagraph = body.split(/\n\s*\n/)[0] ?? '';
	const plainFirstParagraph = stripMarkdown(firstParagraph);
	if (plainFirstParagraph.length >= 32) {
		return clampText(plainFirstParagraph, 200);
	}

	return clampText(stripMarkdown(firstParagraph, { preserveInlineMath: true }), 200);
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
				solvedByCount: EULER_SOLVED_COUNTS.get(problem),
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
	const difficultyRankedRecords = [...sortedRecords].sort((left, right) => {
		const leftSolvedBy = left.solvedByCount ?? -1;
		const rightSolvedBy = right.solvedByCount ?? -1;
		if (leftSolvedBy !== rightSolvedBy) {
			return rightSolvedBy - leftSolvedBy;
		}

		return left.problem - right.problem;
	});
	// With 988 local entries this yields Level 00 through Level 39.
	const maxLevel = Math.floor(Math.max(difficultyRankedRecords.length - 1, 0) / EULER_LEVEL_GROUP_SIZE);
	const levelByProblem = new Map(
		difficultyRankedRecords.map((record, index) => [
			record.problem,
			Math.min(maxLevel, Math.floor(index / EULER_LEVEL_GROUP_SIZE)),
		]),
	);

	return sortedRecords.map((record) => {
		const difficultyLevel = levelByProblem.get(record.problem) ?? maxLevel;

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
