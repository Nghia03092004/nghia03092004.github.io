import { createRequire } from 'node:module';
import { promises as fs } from 'node:fs';
import path from 'node:path';
import { pathToFileURL } from 'node:url';

const require = createRequire(import.meta.url);
const PROJECT_ROOT = process.cwd();
const EULER_ROOT = path.join(PROJECT_ROOT, 'project_euler_unified');
const MEDIA_ROOT = path.join(PROJECT_ROOT, 'public', 'project-euler-media');
const DEFAULT_USER_AGENT =
	'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/123.0.0.0 Safari/537.36';

function padProblem(problem) {
	return String(problem).padStart(4, '0');
}

function getProblemDirectory(problem) {
	return path.join(EULER_ROOT, `problem_${String(problem).padStart(3, '0')}`);
}

function getStatementPath(problem) {
	return path.join(getProblemDirectory(problem), 'statement.html');
}

function getMediaDirectory(problem) {
	return path.join(MEDIA_ROOT, `problem-${padProblem(problem)}`);
}

function getProblemUrl(problem) {
	return `https://projecteuler.net/problem=${problem}`;
}

function isRelativeUrl(value) {
	return Boolean(value) && !/^(?:[a-z]+:)?\/\//i.test(value) && !value.startsWith('#') && !value.startsWith('mailto:');
}

function getLocalMediaPath(problem, sourceUrl) {
	const url = new URL(sourceUrl, 'https://projecteuler.net/');
	const filename = path.basename(url.pathname);
	return {
		filePath: path.join(getMediaDirectory(problem), filename),
		publicPath: `/project-euler-media/problem-${padProblem(problem)}/${filename}`,
	};
}

async function fileExists(filePath) {
	try {
		await fs.access(filePath);
		return true;
	} catch {
		return false;
	}
}

async function loadPlaywright() {
	try {
		return require('playwright');
	} catch {
		const fallbackRoot = path.resolve(PROJECT_ROOT, '..', '.tmp-playwright', 'node_modules', 'playwright');
		if (await fileExists(fallbackRoot)) {
			return import(pathToFileURL(path.join(fallbackRoot, 'index.mjs')).href);
		}

		throw new Error(
			'Playwright is not available. Install it in this repo or keep the workspace-level .tmp-playwright package.',
		);
	}
}

function parseArgs(argv) {
	const options = {
		all: false,
		overwrite: false,
		problems: [],
		from: null,
		to: null,
	};

	for (let index = 0; index < argv.length; index += 1) {
		const arg = argv[index];

		if (arg === '--all') {
			options.all = true;
			continue;
		}

		if (arg === '--overwrite') {
			options.overwrite = true;
			continue;
		}

		if (arg === '--problem') {
			const raw = argv[index + 1];
			if (!raw) {
				throw new Error('--problem requires a value');
			}

			options.problems.push(
				...raw
					.split(',')
					.map((value) => Number(value.trim()))
					.filter((value) => Number.isInteger(value) && value > 0),
			);
			index += 1;
			continue;
		}

		if (arg === '--from') {
			options.from = Number(argv[index + 1]);
			index += 1;
			continue;
		}

		if (arg === '--to') {
			options.to = Number(argv[index + 1]);
			index += 1;
			continue;
		}
	}

	return options;
}

async function discoverProblems() {
	const entries = await fs.readdir(EULER_ROOT, { withFileTypes: true });
	return entries
		.filter((entry) => entry.isDirectory())
		.map((entry) => entry.name.match(/^problem_(\d+)$/)?.[1])
		.filter(Boolean)
		.map((value) => Number(value))
		.sort((left, right) => left - right);
}

function buildProblemList(allProblems, options) {
	if (options.all) {
		return allProblems;
	}

	if (options.problems.length) {
		return [...new Set(options.problems)].sort((left, right) => left - right);
	}

	if (Number.isInteger(options.from) && Number.isInteger(options.to)) {
		return allProblems.filter((problem) => problem >= options.from && problem <= options.to);
	}

	throw new Error('Provide --all, --problem <id>[,<id>...], or --from <id> --to <id>.');
}

async function ensureDirectory(directoryPath) {
	await fs.mkdir(directoryPath, { recursive: true });
}

async function collectStatementPayload(page) {
	return page.evaluate(() => {
		const root = document.querySelector('.problem_content');
		if (!(root instanceof HTMLElement)) {
			throw new Error('Project Euler statement container ".problem_content" was not found.');
		}

		const assets = [];
		const links = [];

		for (const element of root.querySelectorAll('[src], [href], [srcset]')) {
			if (!(element instanceof HTMLElement)) {
				continue;
			}

			if (element.hasAttribute('src')) {
				assets.push(element.getAttribute('src'));
			}

			if (element.hasAttribute('srcset')) {
				for (const candidate of element
					.getAttribute('srcset')
					.split(',')
					.map((value) => value.trim())
					.filter(Boolean)) {
					const [url] = candidate.split(/\s+/);
					assets.push(url);
				}
			}

			if (element.hasAttribute('href')) {
				links.push(element.getAttribute('href'));
			}
		}

		return {
			html: root.innerHTML.trim(),
			assets: [...new Set(assets.filter(Boolean))],
			links: [...new Set(links.filter(Boolean))],
		};
	});
}

async function rewriteStatementHtml(page, assetMap, linkMap) {
	return page.evaluate(
		({ assetMap, linkMap }) => {
			const root = document.querySelector('.problem_content');
			if (!(root instanceof HTMLElement)) {
				throw new Error('Project Euler statement container ".problem_content" was not found.');
			}

			const clone = root.cloneNode(true);
			const assetEntries = Object.entries(assetMap);
			const linkEntries = Object.entries(linkMap);

			for (const element of clone.querySelectorAll('[src], [href], [srcset]')) {
				if (!(element instanceof HTMLElement)) {
					continue;
				}

				if (element.hasAttribute('src')) {
					const source = element.getAttribute('src');
					if (source) {
						const match = assetEntries.find(([original]) => original === source);
						if (match) {
							element.setAttribute('src', match[1]);
						}
					}
				}

				if (element.hasAttribute('srcset')) {
					const value = element.getAttribute('srcset');
					if (value) {
						const rewritten = value
							.split(',')
							.map((candidate) => candidate.trim())
							.filter(Boolean)
							.map((candidate) => {
								const [url, descriptor] = candidate.split(/\s+/, 2);
								const replacement = assetMap[url] ?? url;
								return descriptor ? `${replacement} ${descriptor}` : replacement;
							})
							.join(', ');
						element.setAttribute('srcset', rewritten);
					}
				}

				if (element.hasAttribute('href')) {
					const href = element.getAttribute('href');
					if (href) {
						const match = linkEntries.find(([original]) => original === href);
						if (match) {
							element.setAttribute('href', match[1]);
						}
					}
				}
			}

			return clone.innerHTML.trim();
		},
		{ assetMap, linkMap },
	);
}

async function downloadAsset(request, problem, sourceUrl, overwrite) {
	const { filePath, publicPath } = getLocalMediaPath(problem, sourceUrl);
	if (!overwrite && (await fileExists(filePath))) {
		return publicPath;
	}

	await ensureDirectory(path.dirname(filePath));
	const response = await request.get(new URL(sourceUrl, 'https://projecteuler.net/').toString());
	if (!response.ok()) {
		throw new Error(`Failed to download asset ${sourceUrl}: ${response.status()}`);
	}

	await fs.writeFile(filePath, await response.body());
	return publicPath;
}

async function syncProblem(page, problem, overwrite) {
	await page.goto(getProblemUrl(problem), { waitUntil: 'domcontentloaded', timeout: 120000 });
	await page.waitForFunction(
		() => document.title.includes('Project Euler') && !document.title.includes('Security Verification'),
		{ timeout: 120000 },
	);

	const payload = await collectStatementPayload(page);
	const assetMap = {};

	for (const asset of payload.assets.filter((value) => isRelativeUrl(value))) {
		assetMap[asset] = await downloadAsset(page.context().request, problem, asset, overwrite);
	}

	const linkMap = {};
	for (const href of payload.links.filter((value) => isRelativeUrl(value))) {
		linkMap[href] = new URL(href, 'https://projecteuler.net/').toString();
	}

	const html = await rewriteStatementHtml(page, assetMap, linkMap);
	await ensureDirectory(getProblemDirectory(problem));
	await fs.writeFile(getStatementPath(problem), `${html}\n`, 'utf8');

	return {
		problem,
		assets: Object.keys(assetMap).length,
	};
}

async function main() {
	const options = parseArgs(process.argv.slice(2));
	const problems = buildProblemList(await discoverProblems(), options);
	const playwright = await loadPlaywright();
	const browser = await playwright.chromium.launch({ headless: true });
	const page = await browser.newPage({ userAgent: DEFAULT_USER_AGENT });

	const startedAt = Date.now();
	let totalAssets = 0;

	try {
		for (let index = 0; index < problems.length; index += 1) {
			const result = await syncProblem(page, problems[index], options.overwrite);
			totalAssets += result.assets;

			const position = String(index + 1).padStart(String(problems.length).length, ' ');
			console.log(`[${position}/${problems.length}] Synced problem ${padProblem(result.problem)} (${result.assets} assets)`);
		}
	} finally {
		await browser.close();
	}

	const elapsedSeconds = ((Date.now() - startedAt) / 1000).toFixed(1);
	console.log(`Completed ${problems.length} problem statements in ${elapsedSeconds}s with ${totalAssets} mirrored assets.`);
}

main().catch((error) => {
	console.error(error instanceof Error ? error.message : error);
	process.exitCode = 1;
});
