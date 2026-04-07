import { promises as fs } from 'node:fs';
import path from 'node:path';
import { fileURLToPath, pathToFileURL } from 'node:url';

import { defineCollection } from 'astro/content/config';
import { glob } from 'astro/loaders';
import { z } from 'astro/zod';

import { normalizeEulerMarkdown } from './lib/euler-markdown';

const sharedFields = {
	title: z.string(),
	description: z.string(),
	published: z.coerce.date(),
	updated: z.coerce.date().optional(),
	draft: z.boolean().default(false),
	featured: z.boolean().default(false),
	tags: z.array(z.string()).default([]),
};

const essays = defineCollection({
	loader: glob({
		pattern: '**/*.{md,mdx}',
		base: './src/data/essays',
	}),
	schema: z.object({
		...sharedFields,
		readingTime: z.string().optional(),
	}),
});

const code = defineCollection({
	loader: glob({
		pattern: '**/*.{md,mdx}',
		base: './src/data/code',
	}),
	schema: z.object({
		...sharedFields,
		readingTime: z.string().optional(),
		repository: z.string().optional(),
	}),
});

const euler = defineCollection({
	loader: {
		name: 'euler-loader',
		load: async ({ config, parseData, renderMarkdown, store, generateDigest }) => {
			const rootPath = path.join(fileURLToPath(config.root), 'project_euler_unified');
			const directoryEntries = await fs.readdir(rootPath, { withFileTypes: true });
			const untouchedEntries = new Set(store.keys());

			for (const directoryEntry of directoryEntries.sort((left, right) => left.name.localeCompare(right.name))) {
				if (!directoryEntry.isDirectory()) {
					continue;
				}

				const match = directoryEntry.name.match(/^problem_(\d+)$/);
				if (!match) {
					continue;
				}

				const absoluteFilePath = path.join(rootPath, directoryEntry.name, 'solution.md');
				const rawBody = await fs.readFile(absoluteFilePath, 'utf8');
				const body = normalizeEulerMarkdown(rawBody).trim();
				const id = `problem-${match[1].padStart(4, '0')}`;
				const relativeFilePath = path.relative(fileURLToPath(config.root), absoluteFilePath).replaceAll('\\', '/');
				const data = await parseData({
					id,
					data: {},
					filePath: absoluteFilePath,
				});
				const rendered = await renderMarkdown(body, {
					fileURL: pathToFileURL(absoluteFilePath),
				});

				store.set({
					id,
					data,
					body,
					filePath: relativeFilePath,
					digest: generateDigest(body),
					rendered,
				});
				untouchedEntries.delete(id);
			}

			for (const id of untouchedEntries) {
				store.delete(id);
			}
		},
	},
	schema: z.object({}),
});

export const collections = { essays, code, euler };
