import { promises as fs } from 'node:fs';
import path from 'node:path';

const PROJECT_EULER_ROOT = path.resolve(process.cwd(), 'project_euler');
const TAXONOMY_PATH = path.join(PROJECT_EULER_ROOT, 'taxonomy.json');
const METADATA_PATH = path.join(PROJECT_EULER_ROOT, 'metadata.json');

export interface EulerTaxonomyFile {
	tags: string[];
}

export interface EulerMetadataEntry {
	tags: string[];
}

type EulerMetadataFile = Record<string, EulerMetadataEntry>;

let taxonomyPromise: Promise<Set<string>> | undefined;
let metadataPromise: Promise<Map<number, EulerMetadataEntry>> | undefined;

function normalizeTags(tags: string[], allowedTags: Set<string>) {
	return [...new Set(tags)]
		.map((tag) => tag.trim())
		.filter((tag) => /^[a-z0-9]+(?:_[a-z0-9]+)*$/.test(tag))
		.filter((tag) => allowedTags.has(tag));
}

export async function getEulerTaxonomy() {
	if (!taxonomyPromise) {
		taxonomyPromise = fs
			.readFile(TAXONOMY_PATH, 'utf8')
			.then((contents) => JSON.parse(contents) as EulerTaxonomyFile)
			.then((taxonomy) => new Set(normalizeTags(taxonomy.tags ?? [], new Set(taxonomy.tags ?? []))));
	}

	return taxonomyPromise;
}

export async function getEulerMetadata() {
	if (!metadataPromise) {
		metadataPromise = (async () => {
			const allowedTags = await getEulerTaxonomy();
			const contents = await fs.readFile(METADATA_PATH, 'utf8');
			const metadata = JSON.parse(contents) as EulerMetadataFile;

			return new Map(
				Object.entries(metadata).map(([problem, entry]) => [
					Number(problem),
					{
						tags: normalizeTags(entry?.tags ?? [], allowedTags),
					},
				]),
			);
		})();
	}

	return metadataPromise;
}
