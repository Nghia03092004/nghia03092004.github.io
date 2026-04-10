import { createRequire } from 'node:module';
import { promises as fs } from 'node:fs';
import { spawn } from 'node:child_process';
import path from 'node:path';
import { pathToFileURL } from 'node:url';
import { extractBookLatexStatements } from './project-euler-latex-fallback.mjs';

const require = createRequire(import.meta.url);
const PROJECT_ROOT = process.cwd();
const EULER_ROOT = path.join(PROJECT_ROOT, 'project_euler_unified');
const MEDIA_ROOT = path.join(PROJECT_ROOT, 'public', 'project-euler-media');
const BOOK_PROJECT_ROOT = process.env.PROJECT_EULER_BOOK_ROOT
	? path.resolve(PROJECT_ROOT, process.env.PROJECT_EULER_BOOK_ROOT)
	: path.resolve(PROJECT_ROOT, '..', 'Project Euler');
const BOOK_BUILD_ROOT = path.join(BOOK_PROJECT_ROOT, 'build_statement_books');
const BOOK_HTML_ROOT = path.join(PROJECT_ROOT, '.cache', 'project-euler-book-html');
const PLACEHOLDER_MARKER = 'No compile-safe local statement fragment is available';
const LATEX_ASSET_EXTENSIONS = ['.png', '.jpg', '.jpeg', '.gif', '.svg', '.webp'];

function padProblem(problem) {
	return String(problem).padStart(4, '0');
}

function getBookNumber(problem) {
	return Math.floor((problem - 1) / 100) + 1;
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

function getBookOutputDirectory(book) {
	return path.join(BOOK_HTML_ROOT, `book_${book}`);
}

function getBookHtmlPath(book) {
	return path.join(getBookOutputDirectory(book), `book_${book}.html`);
}

function isRelativeUrl(value) {
	return Boolean(value) && !/^(?:[a-z]+:)?\/\//i.test(value) && !value.startsWith('#') && !value.startsWith('mailto:');
}

function stripQueryAndHash(value) {
	return value.split('#', 1)[0].split('?', 1)[0];
}

function getLinkedMediaBasename(value) {
	if (!value) {
		return null;
	}

	try {
		const url = new URL(value, 'https://projecteuler.net/');
		if (url.origin !== 'https://projecteuler.net' || !url.pathname.startsWith('/resources/images/')) {
			return null;
		}

		const extension = path.posix.extname(url.pathname).toLowerCase();
		if (!LATEX_ASSET_EXTENSIONS.includes(extension)) {
			return null;
		}

		return path.posix.basename(url.pathname).toLowerCase();
	} catch {
		return null;
	}
}

function htmlStatementHasMissingLinkedMedia(statement) {
	if (!statement || statement.sourceType !== 'book-html') {
		return false;
	}

	const assetBasenames = new Set(
		statement.assets
			.filter(Boolean)
			.map((asset) => path.posix.basename(stripQueryAndHash(asset).replace(/\\/g, '/')).toLowerCase()),
	);

	return statement.links.some((link) => {
		const basename = getLinkedMediaBasename(link);
		return basename && !assetBasenames.has(basename);
	});
}

function normalizeBookAssetPath(sourcePath) {
	return sourcePath.replace(/\\/g, '/').replace(/^\.?\//, '').replace(/^Images\/Images_\d+\//, '');
}

function getLocalMediaPath(problem, relativeAssetPath) {
	const normalized = normalizeBookAssetPath(relativeAssetPath);
	return {
		filePath: path.join(getMediaDirectory(problem), ...normalized.split('/')),
		publicPath: `/project-euler-media/problem-${padProblem(problem)}/${normalized}`,
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

async function runMake4ht(book, overwrite) {
	const outputDirectory = getBookOutputDirectory(book);
	const htmlPath = getBookHtmlPath(book);

	if (!overwrite && (await fileExists(htmlPath))) {
		return htmlPath;
	}

	await fs.rm(outputDirectory, { recursive: true, force: true });
	await ensureDirectory(outputDirectory);

	const wrapperPath = path.join('build_statement_books', 'wrappers', `book_${book}.tex`);
	const args = ['-d', outputDirectory, wrapperPath.replace(/\\/g, '/'), 'mathjax'];

	const result = await new Promise((resolve, reject) => {
		const child = spawn('make4ht', args, {
			cwd: BOOK_PROJECT_ROOT,
			stdio: ['ignore', 'pipe', 'pipe'],
		});
		let output = '';

		child.stdout.on('data', (chunk) => {
			output += chunk.toString();
		});
		child.stderr.on('data', (chunk) => {
			output += chunk.toString();
		});
		child.on('error', reject);
		child.on('close', (code) => {
			resolve({ code: code ?? 1, output });
		});
	});

	const htmlExists = await fileExists(htmlPath);
	if (result.code !== 0 && !htmlExists) {
		throw new Error(`make4ht failed for book ${book} and did not produce ${htmlPath}\n${result.output}`);
	}

	if (result.code !== 0) {
		console.warn(`make4ht reported issues for book ${book}, but HTML output was produced. Continuing.`);
	}

	return htmlPath;
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

async function loadBookIntoPage(page, book) {
	const html = await fs.readFile(getBookHtmlPath(book), 'utf8');
	const sanitizedHtml = html.replace(/<script[\s\S]*?<\/script>/gi, '');
	await page.setContent(sanitizedHtml, { waitUntil: 'domcontentloaded' });
}

async function extractBookStatements(page, requestedProblems) {
	return page.evaluate(
		({ requestedProblems, placeholderMarker }) => {
			const requested = new Set(
				requestedProblems.map((problem) => String(Number.parseInt(String(problem), 10))),
			);
			const results = [];
			const seen = new Set();

			for (const statementBox of document.querySelectorAll('.tcolorbox')) {
				if (!(statementBox instanceof HTMLElement)) {
					continue;
				}

				let headingProblem = null;
				let previous = statementBox.previousElementSibling;
				while (previous) {
					if (previous instanceof HTMLElement && previous.classList.contains('sectionHead')) {
						const match = previous.textContent?.match(/Problem\s+(\d+)/);
						if (match) {
							headingProblem = String(Number.parseInt(match[1], 10));
						}
						break;
					}

					previous = previous.previousElementSibling;
				}

				let problem = headingProblem;
				if (!problem) {
					const href = statementBox.querySelector('.tcolorbox-title a[href*="problem="]')?.getAttribute('href');
					const match = href?.match(/problem=(\d+)/);
					if (match) {
						problem = String(Number.parseInt(match[1], 10));
					}
				}

				if (!problem || !requested.has(problem) || seen.has(problem)) {
					continue;
				}

				const content = statementBox.querySelector('.tcolorbox-content');
				if (!(content instanceof HTMLElement)) {
					continue;
				}

				const clone = content.cloneNode(true);
				const assets = [];
				const links = [];

				for (const comment of Array.from(clone.childNodes).filter((node) => node.nodeType === Node.COMMENT_NODE)) {
					comment.remove();
				}

				for (const element of clone.querySelectorAll('[src], [href]')) {
					if (!(element instanceof HTMLElement)) {
						continue;
					}

					if (element.hasAttribute('src')) {
						assets.push(element.getAttribute('src'));
					}

					if (element.hasAttribute('href')) {
						links.push(element.getAttribute('href'));
					}
				}

				results.push({
					problem: Number(problem),
					html: clone.innerHTML.trim(),
					assets: [...new Set(assets.filter(Boolean))],
					links: [...new Set(links.filter(Boolean))],
					placeholder: (clone.textContent ?? '').includes(placeholderMarker),
					sourceType: 'book-html',
				});
				seen.add(problem);
			}

			return results;
		},
		{ requestedProblems, placeholderMarker: PLACEHOLDER_MARKER },
	);
}

async function rewriteStatementHtml(page, html, assetMap, linkMap) {
	return page.evaluate(
		({ html, assetMap, linkMap }) => {
			const wrapper = document.createElement('div');
			wrapper.innerHTML = html;
			const assetEntries = Object.entries(assetMap);
			const linkEntries = Object.entries(linkMap);

			for (const element of wrapper.querySelectorAll('[src], [href]')) {
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

			return wrapper.innerHTML.replace(/<!--[\s\S]*?-->/g, '').trim();
		},
		{ html, assetMap, linkMap },
	);
}

async function copyBookAsset(book, problem, assetPath) {
	const sourcePath = path.join(getBookOutputDirectory(book), ...assetPath.replace(/\\/g, '/').split('/'));
	if (!(await fileExists(sourcePath))) {
		throw new Error(`Expected local statement asset not found: ${sourcePath}`);
	}

	const { filePath, publicPath } = getLocalMediaPath(problem, assetPath);
	await ensureDirectory(path.dirname(filePath));
	await fs.copyFile(sourcePath, filePath);
	return publicPath;
}

async function resolveLatexAssetSource(assetPath) {
	const normalized = assetPath.replace(/\\/g, '/').replace(/^\.?\//, '');
	const directPath = path.join(BOOK_PROJECT_ROOT, ...normalized.split('/'));
	if (await fileExists(directPath)) {
		return {
			sourcePath: directPath,
			relativePath: normalized,
		};
	}

	for (const extension of LATEX_ASSET_EXTENSIONS) {
		const candidate = `${directPath}${extension}`;
		if (await fileExists(candidate)) {
			return {
				sourcePath: candidate,
				relativePath: `${normalized}${extension}`,
			};
		}
	}

	const directoryPath = path.dirname(directPath);
	const parentDirectoryPath = path.dirname(directoryPath);
	const siblingPath = path.join(parentDirectoryPath, path.basename(directPath));
	if (await fileExists(siblingPath)) {
		const relativeDirectory = normalized.includes('/') ? normalized.slice(0, normalized.lastIndexOf('/')) : '';
		const siblingRelativeDirectory = relativeDirectory.includes('/')
			? relativeDirectory.slice(0, relativeDirectory.lastIndexOf('/'))
			: '';
		const relativePath = siblingRelativeDirectory
			? `${siblingRelativeDirectory}/${path.basename(siblingPath)}`
			: path.basename(siblingPath);
		return {
			sourcePath: siblingPath,
			relativePath,
		};
	}

	if (await fileExists(directoryPath)) {
		const basename = path.basename(normalized).replace(/[-_]+$/, '');
		const entries = await fs.readdir(directoryPath, { withFileTypes: true });
		const matched = entries.find(
			(entry) =>
				entry.isFile() &&
				LATEX_ASSET_EXTENSIONS.includes(path.extname(entry.name).toLowerCase()) &&
				(entry.name.startsWith(path.basename(normalized)) || entry.name.startsWith(basename)),
		);

		if (matched) {
			const relativeDirectory = normalized.includes('/') ? normalized.slice(0, normalized.lastIndexOf('/')) : '';
			const relativePath = relativeDirectory ? `${relativeDirectory}/${matched.name}` : matched.name;
			return {
				sourcePath: path.join(directoryPath, matched.name),
				relativePath,
			};
		}
	}

	throw new Error(`Expected local statement asset not found in source images: ${assetPath}`);
}

async function copyLatexAsset(problem, assetPath) {
	const resolved = await resolveLatexAssetSource(assetPath);
	const { filePath, publicPath } = getLocalMediaPath(problem, resolved.relativePath);
	await ensureDirectory(path.dirname(filePath));
	await fs.copyFile(resolved.sourcePath, filePath);
	return publicPath;
}

async function syncProblem(page, book, statement) {
	const assetMap = {};

	for (const asset of statement.assets.filter((value) => isRelativeUrl(value))) {
		assetMap[asset] =
			statement.sourceType === 'latex'
				? await copyLatexAsset(statement.problem, asset)
				: await copyBookAsset(book, statement.problem, asset);
	}

	const linkMap = {};
	for (const href of statement.links.filter((value) => isRelativeUrl(value))) {
		linkMap[href] = new URL(href, 'https://projecteuler.net/').toString();
	}

	const html = await rewriteStatementHtml(page, statement.html, assetMap, linkMap);
	await ensureDirectory(getProblemDirectory(statement.problem));
	await fs.writeFile(getStatementPath(statement.problem), `${html}\n`, 'utf8');

	return {
		problem: statement.problem,
		assets: Object.keys(assetMap).length,
	};
}

async function main() {
	const options = parseArgs(process.argv.slice(2));
	if (!(await fileExists(BOOK_BUILD_ROOT))) {
		throw new Error(`Local Project Euler book build not found at ${BOOK_BUILD_ROOT}`);
	}

	const problems = buildProblemList(await discoverProblems(), options);
	const books = [...new Set(problems.map((problem) => getBookNumber(problem)))].sort((left, right) => left - right);
	const playwright = await loadPlaywright();
	const browser = await playwright.chromium.launch({ headless: true });
	const page = await browser.newPage();

	const startedAt = Date.now();
	let totalAssets = 0;
	let syncedProblems = 0;

	try {
		for (const book of books) {
			await runMake4ht(book, options.overwrite);
			await loadBookIntoPage(page, book);

			const requestedProblems = problems.filter((problem) => getBookNumber(problem) === book);
			const htmlStatements = await extractBookStatements(page, requestedProblems);
			const latexStatements = await extractBookLatexStatements(
				{ bookBuildRoot: BOOK_BUILD_ROOT, placeholderMarker: PLACEHOLDER_MARKER },
				book,
				requestedProblems,
			);
			const htmlStatementsByProblem = new Map(htmlStatements.map((statement) => [statement.problem, statement]));
			const latexStatementsByProblem = new Map(latexStatements.map((statement) => [statement.problem, statement]));

			for (const problem of requestedProblems) {
				const htmlStatement = htmlStatementsByProblem.get(problem);
				const latexStatement = latexStatementsByProblem.get(problem);
				const useHtmlStatement =
					htmlStatement &&
					!htmlStatement.placeholder &&
					!htmlStatementHasMissingLinkedMedia(htmlStatement);
				const statement =
					useHtmlStatement
						? htmlStatement
						: latexStatement && !latexStatement.placeholder
							? latexStatement
							: htmlStatement ?? latexStatement;
				const position = String(syncedProblems + 1).padStart(String(problems.length).length, ' ');

				if (!statement) {
					console.warn(`[${position}/${problems.length}] Problem ${padProblem(problem)} not found in book ${book}; kept existing statement.`);
					syncedProblems += 1;
					continue;
				}

				if (statement.placeholder) {
					console.warn(`[${position}/${problems.length}] Problem ${padProblem(problem)} has no compile-safe book fragment; kept existing statement.`);
					syncedProblems += 1;
					continue;
				}

				await fs.rm(getMediaDirectory(problem), { recursive: true, force: true });
				const result = await syncProblem(page, book, statement);
				totalAssets += result.assets;
				syncedProblems += 1;
				console.log(
					`[${position}/${problems.length}] Synced problem ${padProblem(result.problem)} from local ${statement.sourceType === 'latex' ? 'latex' : 'book'} build (${result.assets} assets)`,
				);
			}
		}
	} finally {
		await browser.close();
	}

	const elapsedSeconds = ((Date.now() - startedAt) / 1000).toFixed(1);
	console.log(`Completed ${problems.length} problem statements in ${elapsedSeconds}s with ${totalAssets} copied assets.`);
}

main().catch((error) => {
	console.error(error instanceof Error ? error.message : error);
	process.exitCode = 1;
});
