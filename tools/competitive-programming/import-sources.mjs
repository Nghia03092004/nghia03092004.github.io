import { promises as fs } from 'node:fs';
import path from 'node:path';

const DEFAULT_DESTINATION = path.resolve(process.cwd(), 'competitive_programming');
const REQUIRED_TRACKS = ['icpc', 'ioi'];

function parseArgs(argv) {
	const options = {
		icpc: '',
		ioi: '',
		dest: DEFAULT_DESTINATION,
	};

	for (let index = 0; index < argv.length; index += 1) {
		const argument = argv[index];
		const next = argv[index + 1];

		if (argument === '--icpc' && next) {
			options.icpc = path.resolve(next);
			index += 1;
			continue;
		}

		if (argument === '--ioi' && next) {
			options.ioi = path.resolve(next);
			index += 1;
			continue;
		}

		if (argument === '--dest' && next) {
			options.dest = path.resolve(next);
			index += 1;
		}
	}

	return options;
}

async function ensureDirectory(directoryPath) {
	await fs.mkdir(directoryPath, { recursive: true });
}

async function resetDirectory(directoryPath) {
	await fs.rm(directoryPath, { recursive: true, force: true });
	await ensureDirectory(directoryPath);
}

async function copyFile(sourcePath, destinationPath) {
	await ensureDirectory(path.dirname(destinationPath));
	await fs.copyFile(sourcePath, destinationPath);
}

function isYearDirectory(name) {
	return /^\d{4}$/.test(name);
}

function getSingleTaskFile(files, folderName, extension) {
	const exactName = `${folderName}${extension}`;
	if (files.includes(exactName)) {
		return exactName;
	}

	const matches = files.filter((fileName) => fileName.endsWith(extension));
	if (matches.length === 1) {
		return matches[0];
	}

	return undefined;
}

async function importIcpc(icpcRoot, destinationRoot) {
	const destinationTrackRoot = path.join(destinationRoot, 'icpc');
	await resetDirectory(destinationTrackRoot);

	const yearEntries = await fs.readdir(icpcRoot, { withFileTypes: true });
	const years = yearEntries
		.filter((entry) => entry.isDirectory() && isYearDirectory(entry.name))
		.map((entry) => entry.name)
		.sort();

	let importedProblems = 0;
	let copiedPackets = 0;

	for (const year of years) {
		const sourceYearRoot = path.join(icpcRoot, year);
		const destinationYearRoot = path.join(destinationTrackRoot, year);
		await ensureDirectory(destinationYearRoot);

		const packetPdfPath = path.join(sourceYearRoot, 'contest_problems.pdf');
		try {
			await copyFile(packetPdfPath, path.join(destinationYearRoot, 'contest_problems.pdf'));
			copiedPackets += 1;
		} catch {
			// Some years may not have a copied packet PDF; the archive still works without it.
		}

		const yearEntriesInner = await fs.readdir(sourceYearRoot, { withFileTypes: true });
		const problemFolders = yearEntriesInner
			.filter((entry) => entry.isDirectory())
			.map((entry) => entry.name)
			.sort((left, right) => left.localeCompare(right));

		for (const folderName of problemFolders) {
			const sourceProblemRoot = path.join(sourceYearRoot, folderName);
			const destinationProblemRoot = path.join(destinationYearRoot, folderName);
			const files = await fs.readdir(sourceProblemRoot);

			const requiredFiles = ['meta.json', 'solution.tex', 'solution.cpp'];
			const optionalFiles = ['statement.txt', 'statement.pdf'];

			for (const fileName of requiredFiles) {
				if (!files.includes(fileName)) {
					throw new Error(`ICPC problem is missing ${fileName}: ${sourceProblemRoot}`);
				}
				await copyFile(path.join(sourceProblemRoot, fileName), path.join(destinationProblemRoot, fileName));
			}

			for (const fileName of optionalFiles) {
				if (files.includes(fileName)) {
					await copyFile(path.join(sourceProblemRoot, fileName), path.join(destinationProblemRoot, fileName));
				}
			}

			importedProblems += 1;
		}
	}

	return {
		years: years.length,
		problems: importedProblems,
		packets: copiedPackets,
	};
}

async function importIoi(ioiRoot, destinationRoot) {
	const destinationTrackRoot = path.join(destinationRoot, 'ioi');
	await resetDirectory(destinationTrackRoot);

	const yearEntries = await fs.readdir(ioiRoot, { withFileTypes: true });
	const years = yearEntries
		.filter((entry) => entry.isDirectory() && isYearDirectory(entry.name))
		.map((entry) => entry.name)
		.sort();

	let importedProblems = 0;

	for (const year of years) {
		const sourceYearRoot = path.join(ioiRoot, year);
		const destinationYearRoot = path.join(destinationTrackRoot, year);
		await ensureDirectory(destinationYearRoot);

		const yearEntriesInner = await fs.readdir(sourceYearRoot, { withFileTypes: true });
		const problemFolders = yearEntriesInner
			.filter((entry) => entry.isDirectory())
			.map((entry) => entry.name)
			.sort((left, right) => left.localeCompare(right));

		for (const folderName of problemFolders) {
			const sourceProblemRoot = path.join(sourceYearRoot, folderName);
			const destinationProblemRoot = path.join(destinationYearRoot, folderName);
			const files = await fs.readdir(sourceProblemRoot);
			const texFile = getSingleTaskFile(files, folderName, '.tex');
			const cppFile = getSingleTaskFile(files, folderName, '.cpp');

			if (!texFile || !cppFile) {
				throw new Error(`IOI problem is missing a main tex/cpp pair: ${sourceProblemRoot}`);
			}

			await copyFile(path.join(sourceProblemRoot, texFile), path.join(destinationProblemRoot, 'solution.tex'));
			await copyFile(path.join(sourceProblemRoot, cppFile), path.join(destinationProblemRoot, 'solution.cpp'));
			importedProblems += 1;
		}
	}

	return {
		years: years.length,
		problems: importedProblems,
	};
}

async function main() {
	const options = parseArgs(process.argv.slice(2));

	for (const track of REQUIRED_TRACKS) {
		if (!options[track]) {
			throw new Error(`Missing required --${track} path.`);
		}
	}

	await ensureDirectory(options.dest);

	const [icpcSummary, ioiSummary] = await Promise.all([
		importIcpc(options.icpc, options.dest),
		importIoi(options.ioi, options.dest),
	]);

	console.log(
		JSON.stringify(
			{
				destination: options.dest,
				icpc: icpcSummary,
				ioi: ioiSummary,
			},
			null,
			2,
		),
	);
}

main().catch((error) => {
	console.error(error);
	process.exitCode = 1;
});
