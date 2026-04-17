import { promises as fs } from 'node:fs';
import path from 'node:path';

const PROJECT_ROOT = process.cwd();
const WEBSITE_EULER_ROOT = path.join(PROJECT_ROOT, 'project_euler');

function parseArgs(argv) {
	const options = {
		sourceRoot: process.env.PROJECT_EULER_SOURCE_ROOT ?? null,
	};

	for (let index = 0; index < argv.length; index += 1) {
		if (argv[index] === '--from') {
			options.sourceRoot = argv[index + 1] ?? null;
			index += 1;
		}
	}

	return options;
}

async function fileExists(filePath) {
	try {
		await fs.access(filePath);
		return true;
	} catch {
		return false;
	}
}

async function loadManifest(sourceRoot) {
	const manifestPath = path.join(sourceRoot, 'website_manifest.json');
	const contents = await fs.readFile(manifestPath, 'utf8');
	return JSON.parse(contents);
}

async function main() {
	const options = parseArgs(process.argv.slice(2));
	if (!options.sourceRoot) {
		throw new Error('Provide --from <external-project-euler-root> or set PROJECT_EULER_SOURCE_ROOT.');
	}

	const sourceRoot = path.resolve(options.sourceRoot);
	const manifest = await loadManifest(sourceRoot);
	let copied = 0;
	let missing = 0;
	let skipped = 0;

	for (const problem of manifest.problems ?? []) {
		const latexRelativePath = problem?.canonical?.latex;
		if (!latexRelativePath) {
			skipped += 1;
			continue;
		}

		const sourcePath = path.join(sourceRoot, latexRelativePath);
		const targetDirectory = path.join(WEBSITE_EULER_ROOT, problem.slug);
		const targetPath = path.join(targetDirectory, 'solution.tex');

		if (!(await fileExists(targetDirectory))) {
			skipped += 1;
			continue;
		}

		if (!(await fileExists(sourcePath))) {
			missing += 1;
			continue;
		}

		await fs.copyFile(sourcePath, targetPath);
		copied += 1;
	}

	console.log(`Copied ${copied} solution.tex files into project_euler (${missing} missing, ${skipped} skipped).`);
}

main().catch((error) => {
	console.error(error instanceof Error ? error.message : error);
	process.exitCode = 1;
});
