import { promises as fs } from 'node:fs';
import path from 'node:path';

const PROJECT_ROOT = process.cwd();
const PROJECT_EULER_ROOT = path.join(PROJECT_ROOT, 'project_euler');
const TAXONOMY_PATH = path.join(PROJECT_EULER_ROOT, 'taxonomy.json');
const OUTPUT_PATH = path.join(PROJECT_EULER_ROOT, 'metadata.json');

const TAXONOMY_ORDER = [
	'algebra',
	'analytic_math',
	'arithmetic',
	'brute_force',
	'combinatorics',
	'constructive',
	'digit_dp',
	'dynamic_programming',
	'game_theory',
	'geometry',
	'graph',
	'greedy',
	'linear_algebra',
	'modular_arithmetic',
	'number_theory',
	'optimization',
	'probability',
	'recursion',
	'search',
	'sequence',
	'simulation',
];

const TAG_OVERRIDES = {
	761: ['geometry', 'game_theory', 'analytic_math'],
};

function scoreMatches(text, regex) {
	const matches = text.match(regex);
	return matches ? matches.length : 0;
}

function addScore(scores, tag, amount) {
	scores.set(tag, (scores.get(tag) ?? 0) + amount);
}

function countLoopDepth(source) {
	return scoreMatches(source, /\bfor\b|\bwhile\b/g);
}

function buildScores(title, markdown, cpp, python) {
	const scores = new Map();
	const prose = `${title}\n${markdown}`.toLowerCase();
	const code = `${cpp}\n${python}`.toLowerCase();
	const fullText = `${prose}\n${code}`;

	addScore(scores, 'number_theory', scoreMatches(fullText, /\bprime\b|\bdivisor\b|\bgcd\b|\blcm\b|\btotient\b|\bvarphi\b|\bdiophantine\b|\bcongruence\b/g) * 2);
	addScore(scores, 'modular_arithmetic', scoreMatches(fullText, /\bmod(?:ulo|ular)?\b|\bcongruence\b|%/g) * 2);
	addScore(scores, 'sequence', scoreMatches(fullText, /\bfibonacci\b|\bsequence\b|\brecurrence\b|\bcontinued fraction\b|\btribonacci\b/g) * 2);
	addScore(scores, 'dynamic_programming', scoreMatches(fullText, /\bdynamic programming\b|\bmemo(?:ization)?\b|\bstate transition\b|\bdp\b/g) * 2);
	addScore(scores, 'digit_dp', scoreMatches(fullText, /\bdigit[- ]?dp\b|\bdigit dynamic\b|\bdigit-state\b/g) * 3);
	addScore(scores, 'combinatorics', scoreMatches(fullText, /\bcombinator(?:ics|ial)\b|\bpermutation\b|\bcombination\b|\bbinomial\b|\barrangement\b/g) * 2);
	addScore(scores, 'geometry', scoreMatches(fullText, /\btriangle\b|\bcircle\b|\bpolygon\b|\bhexagon\b|\bconvex\b|\bpoint\b|\bdistance\b|\bangle\b|\barea\b|\bperimeter\b|\bboundary\b|\bline segment\b/g) * 2);
	addScore(scores, 'graph', scoreMatches(fullText, /\bgraph\b|\bvertex\b|\badjacen(?:t|cy)\b|\bspanning tree\b|\bhamilton(?:ian)?\b|\beulerian\b|\btopological\b|\bnetwork\b/g) * 2);
	addScore(scores, 'probability', scoreMatches(fullText, /\bprobab(?:ility|ilistic)?\b|\bexpected value\b|\brandom\b|\bdice\b|\bcard\b|\burn\b|\bmarkov\b/g) * 2);
	addScore(scores, 'linear_algebra', scoreMatches(fullText, /\bmatrix\b|\bdeterminant\b|\bvector\b|\beigen/g) * 2);
	addScore(scores, 'greedy', scoreMatches(fullText, /\bgreedy\b/g) * 3);
	addScore(scores, 'optimization', scoreMatches(fullText, /\boptimi[sz]\w*\b|\bmaximize\b|\bminimum\b|\bminimi[sz]\w*\b/g) * 2);
	addScore(scores, 'simulation', scoreMatches(fullText, /\bsimulat\w*\b|\bstate machine\b|\bprocess simulation\b|\biterate the process\b/g) * 2);
	addScore(scores, 'search', scoreMatches(fullText, /\bsearch\b|\bdfs\b|\bbfs\b|\bbinary search\b|\bbacktrack\w*\b/g) * 2);
	addScore(scores, 'constructive', scoreMatches(fullText, /\bconstruct\w*\b|\bexplicit construction\b|\breconstruct\b|\barrangement\b/g) * 2);
	addScore(scores, 'analytic_math', scoreMatches(fullText, /\bgenerating function\b|\bintegral\b|\bderivative\b|\basymptotic\b|\bnumerical procedure\b|\bapproximation\b|\banalytic\b/g) * 3);
	addScore(scores, 'algebra', scoreMatches(fullText, /\bpolynomial\b|\bquadratic\b|\bcubic\b|\balgebraic\b|\broots?\s+of\b/g) * 2);
	addScore(scores, 'game_theory', scoreMatches(fullText, /\bgame\b|\bwinning\b|\blosing\b|\bstrategy\b|\bnim\b/g) * 2);
	addScore(scores, 'recursion', scoreMatches(fullText, /\brecursive\b|\brecursion\b/g) * 2);

	if (/\bdigit\b/.test(prose) && /\bdp\b|\bmemo/.test(fullText)) {
		addScore(scores, 'digit_dp', 3);
		addScore(scores, 'dynamic_programming', 2);
	}

	if (countLoopDepth(code) >= 3 || /\bbrute force\b|\benumerate\b|\bexhaustive\b|\btry all\b/g.test(fullText)) {
		addScore(scores, 'brute_force', 2);
	}

	if (/\barithmetic series\b|\bsum\b|\bmultiple\b|\bfraction\b|\bpalindrome\b|\barithmetic\b/g.test(prose)) {
		addScore(scores, 'arithmetic', 1);
	}

	return scores;
}

function selectTags(scores, allowedTags) {
	const ranked = [...scores.entries()]
		.filter(([tag, score]) => allowedTags.has(tag) && score > 0)
		.sort((left, right) => {
			if (right[1] !== left[1]) {
				return right[1] - left[1];
			}

			return TAXONOMY_ORDER.indexOf(left[0]) - TAXONOMY_ORDER.indexOf(right[0]);
		})
		.map(([tag]) => tag);

	if (!ranked.length) {
		return ['arithmetic'];
	}

	return ranked.slice(0, 3);
}

async function main() {
	const taxonomy = JSON.parse(await fs.readFile(TAXONOMY_PATH, 'utf8'));
	const allowedTags = new Set(taxonomy.tags ?? []);
	const entries = await fs.readdir(PROJECT_EULER_ROOT, { withFileTypes: true });
	const metadata = {};

	for (const entry of entries) {
		if (!entry.isDirectory()) {
			continue;
		}

		const match = entry.name.match(/^problem_(\d+)$/);
		if (!match) {
			continue;
		}

		const problemDirectory = path.join(PROJECT_EULER_ROOT, entry.name);
		const [markdown, cpp, python] = await Promise.all([
			fs.readFile(path.join(problemDirectory, 'solution.md'), 'utf8'),
			fs.readFile(path.join(problemDirectory, 'solution.cpp'), 'utf8'),
			fs.readFile(path.join(problemDirectory, 'solution.py'), 'utf8'),
		]);
		const titleMatch = markdown.match(/^#\s+Problem\s+\d+\s*:\s*(.+)$/m) ?? markdown.match(/^#\s+(.+)$/m);
		const title = titleMatch?.[1]?.trim() ?? `Problem ${Number(match[1])}`;
		const tags = TAG_OVERRIDES[Number(match[1])] ?? selectTags(buildScores(title, markdown, cpp, python), allowedTags);

		metadata[String(Number(match[1]))] = { tags };
	}

	await fs.writeFile(OUTPUT_PATH, `${JSON.stringify(metadata, null, 2)}\n`, 'utf8');
	console.log(`Wrote metadata for ${Object.keys(metadata).length} Project Euler problems.`);
}

main().catch((error) => {
	console.error(error instanceof Error ? error.message : error);
	process.exitCode = 1;
});
